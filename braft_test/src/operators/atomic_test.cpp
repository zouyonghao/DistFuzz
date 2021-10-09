#include <braft_test_common.hpp>

static ClientConfigurationGenerator *braft_client_configuration_generator = new BraftClientConfigurationGenerator();

REGISTER_NORMAL_OPERATOR(AtomicTestGet, new DefaultClientOperator(OP_READ, braft_client_configuration_generator));
REGISTER_NORMAL_OPERATOR(AtomicTestSet, new DefaultClientOperator(OP_WRITE, braft_client_configuration_generator));
REGISTER_NORMAL_OPERATOR(AtomicTestCas, new DefaultClientOperator(OP_CAS, braft_client_configuration_generator));
REGISTER_NORMAL_OPERATOR(Init, new DefaultClientOperator(OP_READ, braft_client_configuration_generator));