#include <operator/dst_default_client_operator.hpp>

REGISTER_NORMAL_OPERATOR(ClickHouseDeleteData0,
                         new DefaultClientOperator("rm -rf run/data0", "", {}));
REGISTER_NORMAL_OPERATOR(ClickHouseDeleteData1,
                         new DefaultClientOperator("rm -rf run/data1", "", {}));
REGISTER_NORMAL_OPERATOR(ClickHouseDeleteData2,
                         new DefaultClientOperator("rm -rf run/data2", "", {}));