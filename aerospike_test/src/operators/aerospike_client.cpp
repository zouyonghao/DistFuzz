#include <aerospike/aerospike.h>
#include <aerospike/aerospike_key.h>

#include <operator/dst_default_operator.hpp>
#include <util/common_uils.hpp>

#include <dst_event.h>
#include <dst_random.h>

enum class ACTION_TYPE
{
    READ,
    WRITE,
    CAS
};

class AerospikeClient : public NormalOperator
{
private:
    ACTION_TYPE action_type;

    as_config config;
    aerospike as;
    as_key key;

public:
    AerospikeClient(ACTION_TYPE _action_type) : action_type(_action_type)
    {
        as_config_init(&config);
        as_config_add_host(&config, "127.0.1.1", 2000);
        as_config_add_host(&config, "127.0.1.1", 2001);
        as_config_add_host(&config, "127.0.1.1", 2002);
        aerospike_init(&as, &config);
        as_key_init_str(&key, "test", "test-set", "test-key");
    }

    bool _do() override
    {

        int random_thread_id = random() % INT_MAX;
        as_error err;
        if (aerospike_connect(&as, &err) != AEROSPIKE_OK)
        {
            fprintf(stderr, "err(%d) %s at [%s:%d]\n", err.code, err.message, err.file, err.line);
            return false;
        }

        fprintf(stderr, "connect success!\n");

        std::string op_name;
        std::vector<std::string> op_vector;

        switch (action_type)
        {
        case ACTION_TYPE::READ:
        {
            op_name = "read";
            // __dst_event_record(get_invoke_record(op_name, op_vector, random_thread_id).c_str());

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

            // __dst_event_record(get_result_record(op_name, op_vector, 0, val_as_str, random_thread_id).c_str());
            free(val_as_str);
            as_record_destroy(p_rec);
            break;
        }
        case ACTION_TYPE::WRITE:
        {
            op_name = "write";
            uint32_t random = __dst_get_random_uint32();
            op_vector.push_back(std::to_string(random));
            // __dst_event_record(get_invoke_record(op_name, op_vector, random_thread_id).c_str());

            as_record rec;
            as_record_inita(&rec, 1);
            printf("write value is %s\n", op_vector[0].c_str());
            as_record_set_int64(&rec, "test-bin", random);

            as_policy_write wpol;
            as_policy_write_init(&wpol);
            wpol.exists = AS_POLICY_EXISTS_CREATE_OR_REPLACE;

            if (aerospike_key_put(&as, &err, &wpol, &key, &rec) != AEROSPIKE_OK)
            {
                fprintf(stderr, "err(%d) %s at [%s:%d]\n", err.code, err.message, err.file, err.line);

                if (err.code != AEROSPIKE_ERR_TIMEOUT)
                {
                    printf("write failed!\n");
                    // __dst_event_record(get_result_record(op_name, op_vector, -1, "FAIL", random_thread_id).c_str());
                }
                else
                {
                    printf("write timeout!\n");
                }
                return false;
            }
            // __dst_event_record(get_result_record(op_name, op_vector, 0, "", random_thread_id).c_str());
            as_record_destroy(&rec);
            break;
        }
        case ACTION_TYPE::CAS:
            op_name = "cas";
            op_vector.push_back(std::to_string(__dst_get_random_uint32()));
            op_vector.push_back(std::to_string(__dst_get_random_uint32()));
            // TODO
            break;
        }

        return true;
    }

    ~AerospikeClient() { aerospike_destroy(&as); }
};

REGISTER_NORMAL_OPERATOR(AerospikeRead, new AerospikeClient(ACTION_TYPE::READ));
REGISTER_NORMAL_OPERATOR(AerospikeWrite, new AerospikeClient(ACTION_TYPE::WRITE));
// REGISTER_NORMAL_OPERATOR(AerospikeCas, new
// AerospikeClient(ACTION_TYPE::CAS));