#include "Fuzzer.h"

#define MAX_FILE_LEN    (1 * 1024 *1024)

/* Grab interesting test cases from other fuzzers. */
void SychronizeFuzzer(char **argv, string syncdir, string syncid) {
	DIR *sd;
	dirent *sd_ent;

	sd = opendir(syncdir.c_str());
	if (!sd) {
		cout << "opendir(" <<  syncdir.c_str() << ") fail in SychronizeFuzzer" << endl;
		exit(-1);
	}

	while ((sd_ent = readdir(sd)) != NULL) {
		DIR *qd;
		dirent *qd_ent;
		string qd_path, qd_synced_path;
		s32 id_fd;
		u32 min_accept = 0, next_min_accept;
		u32 res;

		if (sd_ent->d_name[0] == '.' || 
			!strcmp(syncid.c_str(), sd_ent->d_name)) {
			continue;
		}

		/* Skip anything that doesn't have a queue/ subdirectory. */
		qd_path = syncdir + "/" + sd_ent->d_name + "/queue";
		qd = opendir(qd_path.c_str());
		if (!qd) {
			cout << qd_path << " is used by other fuzzer now" << endl;
			continue;
		}

		qd_synced_path = outputDir + ".synced/" + sd_ent->d_name;
		id_fd = open(qd_synced_path.c_str(), O_RDWR | O_CREAT, 0600);
		if (id_fd < 0) {
			cout << "open(" << qd_synced_path << ") fail in SychronizeFuzzer" << endl;
			exit(-1);
		}

		// if read fail, min_accept = 0, because the file may be new
		if (read(id_fd, &min_accept, sizeof(u32)) > 0){ 
			lseek(id_fd, 0, SEEK_SET);
		}
		next_min_accept = min_accept;

		struct dirent **namelist;
		u32 n = scandir(qd_path.c_str(), &namelist, 0, alphasort);
		for (u32 index_file = 0; index_file < n; index_file++) {
			qd_ent = namelist[index_file];
		// while ((qd_ent = readdir(qd))) {
			string path;
			s32 fd;
			u32 syncing_id;
			struct stat st;

			if (qd_ent->d_name[0] == '.' ||
				sscanf(qd_ent->d_name, "%06u", &syncing_id) != 1 ||
				syncing_id < min_accept) {
				continue;
			}

			if (syncing_id >= next_min_accept) {
				next_min_accept = syncing_id + 1;
			}

			/* Allow this to fail in case the other fuzzer is resuming or so... */
			path = qd_path + "/" + qd_ent->d_name;
			fd = open(path.c_str(), O_RDONLY);
			if (fd < 0) {
				cout << "open(" << path << ") fail in SychronizeFuzzer" << endl;
				continue;
			}

			if (fstat(fd, &st)) {
				cout << "fstat() fail in SychronizeFuzzer" << endl;
				exit(-1);
			}

			if (st.st_size && st.st_size <= MAX_FILE_LEN) {
				u8 fault;
				u8* mem = (u8 *)mmap(0, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);

				if (mem == MAP_FAILED) {
					cout << "mmap() fail in SychronizeFuzzer" << endl;
					exit(-1);
				}

				auto errorlist_path = syncdir + "/" + sd_ent->d_name + "/errorList" + "/" + qd_ent->d_name;
				struct stat err_buffer;
				if (stat(errorlist_path.c_str(), &err_buffer) == 0) {
					ReadFile2Mem(errorlist_path, errorList, MAX_LIST_SIZE);
				}
				else {
					memset(errorList, 0, MAX_LIST_SIZE * sizeof(u8));
				}

				WriteToTestcase(mem, st.st_size, testfile);
				fault = ExecuteCase(target_path, argv, exec_tmout);
				AddInterestingSeed(argv, mem, st.st_size, fault);
			
				munmap(mem, st.st_size);
			}
			close(fd);
		}
		cout << "next_min_accept = "<< next_min_accept << endl;
		res = write(id_fd, &next_min_accept, sizeof(u32));
		if (res != sizeof(u32)) {
			cout << "write() fail in SychronizeFuzzer" << endl;
			exit(-1);
		}
		close(id_fd);
		closedir(qd);
	}
	closedir(sd);
}


void fix_up_sync(string sync_id, string& syncDir, string& outputDir) {
	string x;

	if (outputDir.back() != '/') {
		outputDir += "/";
	}
	x = outputDir + sync_id;
	syncDir = outputDir;
	outputDir = x + "/";

	if (sync_state == "SLAVE") {
		skip_deterministic = true;
	}
}

void sync_coverage(u8 *local_coverage, u8* global_coverage, int length) {
	for (int i = 0; i < length; i++) {
		global_coverage[i] = global_coverage[i] & local_coverage[i]; 
	}
	memcpy(local_coverage, global_coverage, length);
}
