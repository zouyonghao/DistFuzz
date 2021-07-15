#include <dst_random.h>
#include <pthread.h>

// TODO: this variable should be put into the shared memory to keep its value
// when the program is shutdown/restart
static volatile uint32_t fuzz_file_index = 0;
static uint64_t fuzz_file_length = 0;
static uint8_t *fuzz_file_array = NULL;
static pthread_mutex_t lock;

static uint8_t read_byte()
{
    if (fuzz_file_index >= fuzz_file_length)
    {
        return 0;
    }
    uint8_t t = fuzz_file_array[fuzz_file_index++];
    return t;
}

static void file_to_string(const char *filename)
{
    FILE *f = fopen(filename, "rb");
    if (f)
    {
        fseek(f, 0, SEEK_END);
        fuzz_file_length = ftell(f);
        fseek(f, 0, SEEK_SET);
        fuzz_file_array = (uint8_t *)malloc(fuzz_file_length + 10);
        if (fuzz_file_array)
        {
            fread(fuzz_file_array, 1, fuzz_file_length, f);
        }
        fclose(f);
    }
    else
    {
        printf("fuzz file open fail.\n");
        exit(0);
    }
}

static void __attribute__((constructor)) init_random_file()
{
    printf("Reading random file %s\n", getenv(ENV_DST_RANDOM_FILE));
    if (getenv(ENV_DST_RANDOM_FILE))
    {
        file_to_string(getenv(ENV_DST_RANDOM_FILE));
    }
}

void __dst_reinit_random(const char *file_name)
{
    printf("Reading random file %s\n", file_name);
    if (file_name)
    {
        if (NULL != fuzz_file_array)
        {
            free(fuzz_file_array);
        }
        file_to_string(file_name);
    }
}

static void __attribute__((destructor)) uninit() { free(fuzz_file_array); }

uint8_t __dst_get_random_uint8_t()
{
    pthread_mutex_lock(&lock);
    uint8_t result = read_byte();
    pthread_mutex_unlock(&lock);
    return result;
}

uint16_t __dst_get_random_uint16_t()
{
    pthread_mutex_lock(&lock);
    uint16_t result = read_byte();
    result <<= 8u;
    result |= read_byte();
    pthread_mutex_unlock(&lock);
    return result;
}

uint32_t __dst_get_random_uint32()
{
    pthread_mutex_lock(&lock);
    uint32_t result = read_byte();
    result <<= 8u;
    result |= read_byte();
    result <<= 8u;
    result |= read_byte();
    result <<= 8u;
    result |= read_byte();
    pthread_mutex_unlock(&lock);
    return result;
}

char *__dst_get_random_string(uint32_t length)
{
    char *result = malloc(length);
    pthread_mutex_lock(&lock);
    for (uint32_t i = 0; i < length; i++)
    {
        result[i] = read_byte();
    }
    pthread_mutex_unlock(&lock);
    return result;
}