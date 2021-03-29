#include "Fuzzer.h"

#define MAX_LINE 8192
#define AVG_SMOOTHING 16

#define EXEC_TM_ROUND 20

static s32 cpu_core_count = 0;
static s32 cpu_aff = -1;	

static inline u32 randNum(u32 denominator) {
    static u32 count = 0;
    if (count >= 10000) {
        srand((int)time(0));
        count = 0;
    }
    if (denominator == 0) {
        return 0;
    }
    return rand() % denominator;
}

static double GetRunnableProcesses(void)
{
	static double res;
	FILE* f = fopen("/proc/stat", "r");
	char tmp[1024];
	u32 val = 0;

	if (!f) return 0;
	while (fgets(tmp, sizeof(tmp), f)) {
		if (!strncmp(tmp, "procs_running ", 14) ||
			!strncmp(tmp, "procs_blocked ", 14))
			val += atoi(tmp + 14);
	}

	fclose(f);

	if (!res) {
		res = val;
	} else {
		res = res * (1.0 - 1.0 / AVG_SMOOTHING) +
					((double)val) * (1.0 / AVG_SMOOTHING);
	}
	return res;
}

void GetCoreCount(void) 
{
	u32 cur_runnable = 0;
	cpu_core_count = sysconf(_SC_NPROCESSORS_ONLN);

	if (cpu_core_count > 0) {
		cur_runnable = (u32)GetRunnableProcesses();
	}

	cout << "CPU number = " << cpu_core_count << endl;
	cout << "runnable tasks = " << cur_runnable << endl;

	if (cpu_core_count > 1) {
		if (cur_runnable > cpu_core_count * 1.5) {
			cout << "System under apparent load, performance may be spotty." << endl;
		}
	}
	else {
		cpu_core_count = 0;
		cout << "Unable to figure out the number of CPU cores." << endl;
	}
}

void BindFreeCPU(void)
{
	DIR *d;
	struct dirent *de;
	cpu_set_t c;
	
	u8 cpu_used[4096] = {0};
	u32 i;

	if (cpu_core_count < 2) {
		cout << "cpu_core_count is less than 2" << endl;
		return;
	}

	d = opendir("/proc");
	if (!d) {
		cout << "Unable to access /proc - can't scan for free CPU cores." << endl;
		exit(-1);
	}

	usleep(randNum(1000) * 250);

	while ((de = readdir(d))) {
		string fn = "";
		FILE *f;
		char tmp[MAX_LINE];
		u8 has_vmsize = 0;

		if (!isdigit(de->d_name[0])) continue;

		fn = fn + "/proc/" + de->d_name + "/status";
		// cout << "fn = " << fn << endl;
		f = fopen(fn.c_str(), "r");
		if (!f) {
			continue;
		}
		// cout << "open fn is " << fn << endl;
		while (fgets(tmp, MAX_LINE, f)) {
			u32 hval;
			if (!strncmp(tmp, "VmSize:\t", 8)) has_vmsize = 1;
			if (!strncmp(tmp, "Cpus_allowed_list:\t", 19) && 
				!strchr(tmp, '-') &&
				!strchr(tmp, ',') && 
				sscanf(tmp + 19, "%u", &hval) == 1 &&
				hval < sizeof(cpu_used) && has_vmsize) {
				
				cpu_used[hval] = 1;
				break;
			}
		}
		fclose(f);
	}

	closedir(d);

	for (i = 0; i < cpu_core_count; i++) {
		if (!cpu_used[i]) break;
	}
	if (i == cpu_core_count) {
		cout << "No more free CPU cores" << endl;
		exit(-1);
	}

	cout << "Found a free CPU core, binding to " << i << endl;
	cpu_aff = i;

	CPU_ZERO(&c);
	CPU_SET(i, &c);

	if (sched_setaffinity(0, sizeof(c), &c)) {
		cout << "sched_setaffinity() fail" << endl;
		exit(-1);
	}
}

void init_stats(void) 
{
	u64 max_us = 0;
	u64 avg_us = 0;

	if (total_cal_cycles)
		avg_us = total_cal_us / total_cal_cycles;
	
	for (auto& q:queue) {
		if (q.execTime > max_us) max_us = q.execTime;
	}

	if (avg_us > 20000) {
		cout << "The speed is too slow!" << endl;
	}
	cout << "total_cal_us = " << total_cal_us << endl;
	cout << "total_cal_cycles = " << total_cal_cycles << endl;
	cout << "avg_us = " << avg_us << endl;
	if (avg_us > 50000) havoc_div =10; // 0-19 execs/sec
	else if (avg_us > 20000) havoc_div = 5; // 20-49 execs/sec
	else if (avg_us > 10000) havoc_div = 2; // 50-100 execs/sec

	if (!timeout_given) {
		if (avg_us > 50000) exec_tmout = avg_us * 2 / 1000;
		else if (avg_us > 10000) exec_tmout = avg_us * 3 / 1000;
		else exec_tmout = avg_us * 5 / 1000;

		exec_tmout = ((max_us / 1000) > exec_tmout) ? (max_us / 1000) : exec_tmout;
		exec_tmout = (exec_tmout + EXEC_TM_ROUND) / EXEC_TM_ROUND * EXEC_TM_ROUND;

		exec_tmout = (exec_tmout > EXEC_TIMEOUT) ? EXEC_TIMEOUT : exec_tmout;

		cout << "No timeout_given, so the exec_tmout = " << exec_tmout << "ms" << endl;

		timeout_given = 1;
	}

	hang_tmout = (EXEC_TIMEOUT < (exec_tmout * 2 + 100)) ? EXEC_TIMEOUT : (exec_tmout * 2 + 100);
	cout << "hang_tmout = " << hang_tmout << "ms" << endl;
}