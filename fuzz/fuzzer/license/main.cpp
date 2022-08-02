#include "handler.h"
#include "init_global.h"
#include <termios.h>
#include <execinfo.h>

#if not defined(CHECK_FREEBSD) && not defined(CHECK_NETBSD)
	#include <mcheck.h>
#endif
#ifdef USE_LICENSE
	#include "license/license.h"
#endif

//#define USE_LICENSE
//#define USE_PASSWORD
#define USE_DEBUG

#ifdef USE_DEBUG
void DebugBacktrace(int signo, siginfo_t*, void*) 
{
	void *array[10];
	size_t size;

	size = backtrace(array, 10);
	fprintf(stderr, "Error: signal %d:\n", signo);
	backtrace_symbols_fd(array, size, STDERR_FILENO);
	exit(1);
}
#endif

char g_tool_dir[300];

void initGlobalValue() {
	initGlobalValue_handler();
	initGlobalValue_summary();
}

int main(int argc, char **argv) {
	int mode = atoi(argv[1]);
	strcpy(g_tool_dir, argv[2]);

#ifdef USE_DEBUG
	struct sigaction stSigAction;
	stSigAction.sa_flags = SA_SIGINFO;
	stSigAction.sa_sigaction = DebugBacktrace;
	sigaction(SIGSEGV, &stSigAction, NULL);
#endif

#ifdef USE_PASSWORD
	char password[30] = {0};
	int ch = 'g';
	password[0] = ch + 12;
	password[1] = password[0];
	password[2] = ch + 11;
	password[3] = ch - 53;
	password[4] = ch - 55;
	password[5] = ch - 54;
	password[6] = ch - 47;
	password[7] = ch - 60;
	password[8] = ch - 4;
	password[9] = ch + 12;
	password[10] = password[8];
	password[11] = ch;
	password[12] = ch - 54;
	password[13] = ch - 46;
	password[14] = password[13];
	password[15] = password[13];
	char input[60];
	struct termios oldt, newt;
	int i = 0;
	printf("Please input password: ");
	while (i < 50) {
		tcgetattr(STDIN_FILENO, &oldt);
		newt = oldt;
		newt.c_lflag &= ~(ECHO | ICANON);
		tcsetattr(STDIN_FILENO, TCSANOW, &newt);
		input[i] = getchar();
		tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
		if (input[i] == '\r' || input[i] == '\n')
			break;
		i++;
//		printf("*");
	}
	input[i] = '\0';
	printf("\n");
	if (strcmp(password, input)) {
		printf("Incorrect password!\n");
		exit(-1);
	}
#endif

	string checker_str = "{ALL}";
#ifdef USE_LICENSE
	if (!CheckLicenseFile(checker_str)) {
		printf("License check is failed!\n");
		exit(-1);
	}
	if (!CheckWriteRunLogFile()) {
		printf("License check is failed!!\n");
		exit(-1);
	}
#endif

	// Analyze the module
	AnalyzeModule(mode, checker_str);

    return 0;
}
