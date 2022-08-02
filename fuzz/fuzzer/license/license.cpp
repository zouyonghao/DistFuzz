#include "license.h"

#define RSA_CONTENT_LEN		512

static string g_encrypt_str;

static string g_pub_key; 	
extern char g_tool_dir[300];

static int WriteFile(string file, string data) {
	FILE *fp = fopen(file.c_str(), "a");
	if (!fp) {
		printf("[Error] Fail to open file: %s\n", file.c_str());
		return -1;
	}
	if (fwrite(data.c_str(), data.size(), 1, fp) < 0) {
		printf("[Error] Fail to write file: %s\n", file.c_str());
		fclose(fp);
		return -1;
	}
	fclose(fp);
	return 0;
}

static int ReadFile(string file, string &data) {
	FILE *fp = fopen(file.c_str(), "r");
	if (!fp) {
		printf("[Error] Fail to open file: %s\n", file.c_str());
		return -1;
	}
	data = "";
	char buf[] = {0, 0};
	while ((fread(buf, sizeof(char), 1, fp)))
		data = data + buf;
	fclose(fp);
	return 0;
}

static int ReadFileBinary(string file, unsigned char *data, int &len) {
	FILE *fp = fopen(file.c_str(), "rb");
	if (!fp) {
		printf("[Error] Fail to open file: %s\n", file.c_str());
		return -1;
	}
	unsigned char buf[] = {0};
	int count = 0;
	while ((fread(buf, sizeof(unsigned char), 1, fp)))
		data[count++] = buf[0];
	fclose(fp);
	len = count;
	return 0;
}

string EncryptData(string data) {
	string result = data;
	int len = result.size();
	int en_len = g_encrypt_str.size();
	for (int i = 0; i < len; i++) {
		for (int j = 0; j < en_len; j++) {
			result[i] = result[i] ^ (int)(g_encrypt_str[j]);
		}
	}
	return result;
}

string DecryptData(string data) {
	string result = data;
	int len = result.size();
	int en_len = g_encrypt_str.size();
	for (int i = 0; i < len; i++) {
		for (int j = 0; j < en_len; j++) {
			result[i] = result[i] ^ (int)(g_encrypt_str[j]);
		}
	}
	return result;
}

string DecryptFile(string file) {
	string data;
	if (ReadFile(file, data))
		return "";
	data = DecryptData(data);
	return data;
}

string RSA_DecryptFile(string file) {
	RSA *rsa = RSA_new();
	BIO *keyio = BIO_new_mem_buf((unsigned char *)g_pub_key.c_str(), -1);
	rsa = PEM_read_bio_RSAPublicKey(keyio, &rsa, NULL, NULL);
	int rsa_len = RSA_size(rsa);

	char *text_data = new char[rsa_len + 1];
	memset(text_data, 0, rsa_len + 1);

	char *cipper_data = new char[rsa_len + 1];
	memset(cipper_data, 0, rsa_len + 1);

	int len;
	if (ReadFileBinary(file, (unsigned char *)cipper_data, len)) {
		delete []text_data;
		delete []cipper_data;
		return "";
	}

	if (RSA_public_decrypt(len, (unsigned char *)(cipper_data), 
			(unsigned char *)text_data, rsa, RSA_PKCS1_PADDING) < 0) {
		printf("%s\n", ERR_error_string(ERR_get_error(), NULL));
		printf("[ERROR] Fail to decrypt data\n");
		return "";
	}
	string data = text_data;

	delete []text_data;
	delete []cipper_data;
	BIO_free(keyio);
	RSA_free(rsa);

	return data;
}

static string GetCPUID() {
	unsigned int eax, ebx, ecx, edx;
	char str[300];

	__asm__ (
		"cpuid"
		:"=a"(eax),"=b"(ebx),"=c"(ecx),"=d"(edx):"a"(0));
	sprintf(str, "%x%x%x%x", eax, ebx, ecx, edx);
	string cpu_id = str;
	return cpu_id;
}

string GetDiskID() {
	struct hd_driveid hid;
	char str[300] = {0};
	int fd = open("/dev/sda", O_RDONLY);
	if (fd < 0)
		return "";
	if (ioctl(fd, HDIO_GET_IDENTITY, &hid) < 0)
		return "";
	close(fd);
	for (int i = 0; i < 20; i++) 
		sprintf(str, "%s%x", str, hid.serial_no[i]);
	string disk_id = str;
	return disk_id;
}

static MyTime *GetCurTime() {
	time_t timep;
	time(&timep);
	struct tm *p = gmtime(&timep);

	MyTime *my_time = new MyTime;
	my_time->year = 1900 + p->tm_year;
	my_time->month = 1 + p->tm_mon;
	my_time->day = p->tm_mday;
	my_time->hour = p->tm_hour;
	my_time->minute = p->tm_min;
	my_time->second = p->tm_sec;

	return my_time;
}

static int CompareTime(MyTime *t1, MyTime *t2) {
	if (t1->year > t2->year)
		return 1;
	if (t1->year < t2->year)
		return -1;
	if (t1->month > t2->month)
		return 1;
	if (t1->month < t2->month)
		return -1;
	if (t1->day > t2->day)
		return 1;
	if (t1->day < t2->day)
		return -1;
	if (t1->hour > t2->hour)
		return 1;
	if (t1->hour < t2->hour)
		return -1;
	if (t1->minute > t2->minute)
		return 1;
	if (t1->minute < t2->minute)
		return -1;
	if (t1->second > t2->second)
		return 1;
	if (t1->second < t2->second)
		return -1;
	return 0;
}

static string GetLastLineFromString(string host, string sub) {
	size_t pos = host.rfind(sub);
	if (pos == string::npos)
		return "";
	size_t pos_end = host.find("\n", pos);
	string res_str = host.substr(pos, pos_end - pos);
	return res_str;
}

//#define GEN_KEY

bool CheckLicenseFile(string &checker_str) {
	checker_str = "";
	char file_path[300];
	
#ifdef GEN_KEY
	sprintf(file_path, "%s/%s", g_tool_dir, PUB_KEY_FILE);
	if (ReadFile(file_path, g_pub_key))
		return false;
#else
	g_pub_key = "-----BEGIN RSA PUBLIC KEY-----\n"\
	"MIICCAKCAgEA2U97x14LNmP1cxDqVJmiyrrXFjxhic7jrP1mxXF3idF7C/o9b4EB\n"\
	"KSiz6chsQR8YSzpomSXZc2Zp3bKYnooZtdXaqDfptohbH89ZjgcLOyE7tJ4+HXY4\n"\
	"TuhBgLk6j2KbQtC4jp7mTalCGQfYQ7Y8xJ48+XykNbDe2oqYW0zWCHpQ8FOQoNcu\n"\
	"JbDxgVacv10QvaXOe0U/dEK8IwGNKIoBjd1+mxraiNvZdTNR6yDEnaaqbnkImfcq\n"\
	"kWNyHF1F0cckbBZdB0jAyZlzAueDjitsJfigd5jj9TBetzj3mO5z0zRhqMCeLHlP\n"\
	"qMzWXTuCzME1r4zsL2cvKYjMgHKuMGoHpA+pFuCM+ycgTQVLnu/QoDCZgsY3Esqe\n"\
	"49chUBt2xYQZ9cdCAyPcTOEi51KLqrZ2LtL7CAwXXdvyq6n4M4jiWONDIcphKuvI\n"\
	"2gFwdu9TdhMYWJmAUv6x9bv9DX0qeTRfbtt5hWsSsGtqpZ5psnnE+CI+4nUdtSjn\n"\
	"sngDwUL7W3U0RrRcPTSZ1smFCL5zS47FdkAmIVsJJZUY74b8X2J23vlYAHUC0bE5\n"\
	"Vzu4ap6cK0lwRvLQAAGkpk7dcwCeOcmi1ElWDeLTcTfME74UNrOB1kNUSt2Ddbjg\n"\
	"hWhAYG0n6hmXgv00uOZ8sckcAcjZkR31ZZsSHZ8yDbgKrJKvetuixPkCAQM=\n"\
	"-----END RSA PUBLIC KEY-----";
#endif
	sprintf(file_path, "%s/%s", g_tool_dir, LICENSE_FILE);
	string data = RSA_DecryptFile(file_path);
	if (data == "")
		return false;

	g_encrypt_str = GetLastLineFromString(data, "USER:");

	// Check time
	MyTime *my_time = GetCurTime();
	string begin_time_str = GetLastLineFromString(data, "BEGIN_DATE:");
	if (begin_time_str == "")
		return false;
	string end_time_str = GetLastLineFromString(data, "END_DATE:");
	if (end_time_str == "")
		return false;
	int begin_year = 0, begin_month = 0, begin_day = 0;
	int end_year = 0, end_month = 0, end_day = 0;
	char tmp_str[100];
	sscanf(begin_time_str.c_str(), "%s %d-%d-%d", tmp_str, &begin_year,
									&begin_month, &begin_day);
	sscanf(end_time_str.c_str(), "%s %d-%d-%d", tmp_str, &end_year,
									&end_month, &end_day);
	int a1 = begin_year * 420 + begin_month * 35 + begin_day;
	int a2 = end_year * 420 + end_month * 35 + end_day;
	int aa = my_time->year * 420 + my_time->month * 35 + my_time->day;
	delete my_time;

	if (aa < a1 || aa > a2) {
		printf("[ERROR] Current licence is expired!\n");
		return false;
	}

	checker_str = GetLastLineFromString(data, "CHECKER:");
	if (checker_str == "")
		return false;

	return true;
}

bool CheckWriteRunLogFile() {
	string data;
	char file_path[300];
	sprintf(file_path, "%s/%s", g_tool_dir, RUN_LOG_FILE);
	data = DecryptFile(file_path);
	if (data == "")
		return false;

	// Check CPU ID and DISK ID
	string cpu_id_str = GetLastLineFromString(data, "CPU_ID:");
	string cpu_id_ss = "CPU_ID: " + GetCPUID();
	if (cpu_id_str != cpu_id_ss) {
		printf("[ERROR] Current machine is invalid!\n");
		return false;
	}
//	string disk_id_str = GetLastLineFromString(data, "DISK_ID:");
//	string disk_id_ss= "DISK_ID: " + GetDiskID();
//	if (disk_id_str != disk_id_ss) {
//		printf("[ERROR] Current machine is invalid!!\n");
//		return false;
//	}
	
	string str = GetLastLineFromString(data, "[EVENT]");
	if (str == "")
		return false;
	char tmp_str[100];

	MyTime *my_time1 = new MyTime;
	sscanf(str.c_str(), "%s %d-%d-%d-%d:%d:%d: %s", tmp_str, 
					&my_time1->year, &my_time1->month, &my_time1->day, 
					&my_time1->hour, &my_time1->minute, &my_time1->second, 
					tmp_str);

	MyTime *my_time2 = GetCurTime();
	int cmp = CompareTime(my_time1, my_time2);
	delete my_time1;
	if (cmp > 0) {
		delete my_time2;
		return false;
	}

	char tmp[200];
	sprintf(tmp, "[EVENT] %d-%d-%d-%d:%d:%d: RUN\n", 
			my_time2->year, my_time2->month, my_time2->day,
			my_time2->hour, my_time2->minute, my_time2->second);
	delete my_time2;

	data = EncryptData(tmp);
	WriteFile(file_path, data);
	return true;
}
