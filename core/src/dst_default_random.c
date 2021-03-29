#include <dst_random.h>

// TODO: this variable should be put into the shared memory to keep its value
// when the program is shutdown/restart
static volatile uint32_t fuzz_file_index = 0;
static uint64_t fuzz_file_length = 0;
static uint8_t *fuzz_file_array;

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

void __dst_init_random() { init_random_file(); }

static void __attribute__((destructor)) uninit() { free(fuzz_file_array); }

uint8_t __dst_get_random_uint8_t() { return read_byte(); }

uint16_t __dst_get_random_uint16_t()
{
    uint16_t result = read_byte();
    result <<= 8u;
    result |= read_byte();
    return result;
}

uint32_t __dst_get_random_uint32()
{
    uint32_t result = read_byte();
    result <<= 8u;
    result |= read_byte();
    result <<= 8u;
    result |= read_byte();
    result <<= 8u;
    result |= read_byte();
    return result;
}