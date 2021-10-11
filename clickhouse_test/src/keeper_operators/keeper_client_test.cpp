#include <keeper_configuration_generator.hpp>

static ClientConfigurationGenerator *keeper_client_configuration_generator = new KeeperClientConfigurationGenerator();

REGISTER_NORMAL_OPERATOR(KeeperGet, new DefaultClientOperator(OP_READ, keeper_client_configuration_generator));
REGISTER_NORMAL_OPERATOR(KeeperSet, new DefaultClientOperator(OP_WRITE, keeper_client_configuration_generator));