#include <iostream>
#include <string>
#include <stdio.h>
#include <string.h>
#include <openssl/rsa.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <linux/hdreg.h>
#include <unistd.h>

using namespace std;

#define PUB_KEY_FILE    "public_key.pem"
#define LICENSE_FILE	"config_tool.lic"
#define RUN_LOG_FILE	"run_log.txt"

typedef struct MyTime {
	int year;
	int month;
	int day;
	int hour;
	int minute;
	int second;
} MyTime;

bool CheckLicenseFile(string &checker_str);

bool CheckWriteRunLogFile();

