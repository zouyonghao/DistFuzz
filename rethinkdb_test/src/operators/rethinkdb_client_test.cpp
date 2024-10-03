#include <operator/dst_default_client_operator.hpp>
#include <operator/dst_default_init_operator.hpp>
#include <rethinkdb_configuration_generator.hpp>

static RethinkdbClientConfigurationGenerator *rccg = new RethinkdbClientConfigurationGenerator();

class RethinkdbInitOperator : public DefaultInitOperator
{
public:
    RethinkdbInitOperator() : DefaultInitOperator() {}

    std::string get_write_zero_command() override
    {
        std::string configure_string = "timeout 3 /usr/bin/java -jar "
                                       "/home/zyh/DistFuzz/rethinkdb_test/client/target/"
                                       "rethinkdb-test-client-1.0-SNAPSHOT-jar-with-dependencies.jar " +
                                       std::to_string(node_count) + " ";
        configure_string += "set 0";
        return configure_string;
    }
};

REGISTER_NORMAL_OPERATOR(RethinkdbSet, new DefaultClientOperator(OP_WRITE, rccg));
REGISTER_NORMAL_OPERATOR(RethinkdbGet, new DefaultClientOperator(OP_READ, rccg));
REGISTER_NORMAL_OPERATOR(RethinkdbCas, new DefaultClientOperator(OP_CAS, rccg));
REGISTER_NORMAL_OPERATOR(Init, new RethinkdbInitOperator);
