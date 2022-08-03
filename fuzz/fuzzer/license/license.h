#include <fcntl.h>
#include <iostream>
#include <linux/hdreg.h>
#include <net/if.h>
#include <netinet/in.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

using namespace std;

#define PUB_KEY_FILE "public_key.pem"
#define LICENSE_FILE "config_tool.lic"
#define RUN_LOG_FILE "run_log.txt"

typedef struct MyTime
{
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
} MyTime;

bool CheckLicenseFile(string &checker_str);

bool CheckWriteRunLogFile();

MyTime *GetCurTime();

string EncryptData(string data);

int WriteFile(string file, string data);
