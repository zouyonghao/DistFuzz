#include <operator/dst_default_client_operator.hpp>
#include <rethinkdb_configuration_generator.hpp>

static RethinkdbClientConfigurationGenerator *rccg = new RethinkdbClientConfigurationGenerator();

REGISTER_NORMAL_OPERATOR(RethinkdbGet, new DefaultClientOperator(OP_READ, rccg));
REGISTER_NORMAL_OPERATOR(RethinkdbSet, new DefaultClientOperator(OP_WRITE, rccg));
REGISTER_NORMAL_OPERATOR(RethinkdbCas, new DefaultClientOperator(OP_CAS, rccg));
