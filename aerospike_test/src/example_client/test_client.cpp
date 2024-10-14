#include <aerospike_client.hpp>

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        std::cout << "Usage: " << argv[0] << " r/w [value]" << std::endl;
        return -1;
    }
    as_config config;
    aerospike as;
    as_key key;
    int node_count = 5;
    as_log_set_level(AS_LOG_LEVEL_DEBUG);
    as_log_set_callback(example_log_callback);
    as_config_init(&config);
    for (int i = 0; i < node_count; i++)
    {
        as_config_add_host(&config, ("n" + std::to_string(i + 1)).c_str(), SERVICE_BASE_PORT + i);
    }
    aerospike_init(&as, &config);
    as_key_init_str(&key, "test", "test-set", "test-key");

    as_error err;
    if (aerospike_connect(&as, &err) != AEROSPIKE_OK)
    {
        fprintf(stderr, "err(%d) %s at [%s:%d]\n", err.code, err.message, err.file, err.line);
        return false;
    }

    if (argv[1][0] == 'r')
    {
        as_record *p_rec = NULL;
        static const char *bins[] = {"test-bin", NULL};
        if (aerospike_key_select(&as, &err, NULL, &key, bins, &p_rec) != AEROSPIKE_OK || p_rec == NULL)
        {
            fprintf(stderr, "aerospike_key_get() returned %d - %s\n", err.code, err.message);
            // __dst_event_record(get_result_record(op_name, op_vector, -1, "FAIL", random_thread_id).c_str());
            return false;
        }
        char *val_as_str = as_val_tostring(as_bin_get_value(p_rec->bins.entries));
        printf("read value is %s\n", val_as_str);
        free(val_as_str);
        as_record_destroy(p_rec);
    } else {
        as_record rec;
        as_record_inita(&rec, 1);
        as_record_set_int64(&rec, "test-bin", atoi(argv[2]));
        if (aerospike_key_put(&as, &err, NULL, &key, &rec) != AEROSPIKE_OK)
        {
            fprintf(stderr, "err(%d) %s at [%s:%d]\n", err.code, err.message, err.file, err.line);
            return false;
        }
        as_record_destroy(&rec);
    }

    return 0;
}