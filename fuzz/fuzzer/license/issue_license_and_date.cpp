#include "license.h"

const char *g_tool_dir;

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        printf("usage: ./issue <output-folder>");
        return -1;
    }
    g_tool_dir = argv[1];
    MyTime *my_time = GetCurTime();

    char tmp[200];
    sprintf(tmp, "[EVENT] %d-%d-%d-%d:%d:%d: RUN\n", my_time->year, my_time->month, my_time->day, my_time->hour,
            my_time->minute, my_time->second);
    delete my_time;

    string data = EncryptData(tmp);
    char file_path[300];
    sprintf(file_path, "%s/%s", g_tool_dir, RUN_LOG_FILE);
    WriteFile(file_path, data);

    return 0;
}
