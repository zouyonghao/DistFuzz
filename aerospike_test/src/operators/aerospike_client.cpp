#include <aerospike_client.hpp>

REGISTER_NORMAL_OPERATOR(AerospikeWrite, new AerospikeClient(OP_WRITE));
REGISTER_NORMAL_OPERATOR(AerospikeRead, new AerospikeClient(OP_READ));
REGISTER_NORMAL_OPERATOR(Init, new AerospikeInitOperator);
// REGISTER_NORMAL_OPERATOR(AerospikeCas, new AerospikeClient(ACTION_TYPE::CAS));
