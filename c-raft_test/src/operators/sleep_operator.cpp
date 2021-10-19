#include <operator/dst_default_operator.hpp>

/* craft example server does need a client, so we make a sleep operator to let it run for a while */
REGISTER_NORMAL_OPERATOR(CraftSleepOperator, new SimpleCommandOperator("sleep 2"));