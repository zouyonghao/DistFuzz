#include <operator/dst_default_client_operator.hpp>

REGISTER_NORMAL_OPERATOR(ClickHouseDeleteData0,
                         new DefaultClientOperator("rm -rf run/log1", "", {}));
REGISTER_NORMAL_OPERATOR(ClickHouseDeleteData1,
                         new DefaultClientOperator("rm -rf run/log2", "", {}));
REGISTER_NORMAL_OPERATOR(ClickHouseDeleteData2,
                         new DefaultClientOperator("rm -rf run/log3", "", {}));