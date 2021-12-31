#include <time.h>
#include <sys/time.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

uint64_t GetCurTimeMs(void)
{
    struct timeval tv;
    struct timezone tz;

    gettimeofday(&tv, &tz);

    return ((tv.tv_sec * 1000000ULL) + tv.tv_usec) / 1000;
}

int main()
{
    printf("time is %ld\n", GetCurTimeMs());
    return 0;
}