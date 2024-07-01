#include <operator/dst_default_operator.hpp>
#include <operator/dst_start_new_node.hpp>

/* craft example server does need a client, so we make a sleep operator to let it run for a while */
REGISTER_NORMAL_OPERATOR(CraftSleepOperator, new SimpleCommandOperator("sleep 2"));

REGISTER_CRITICAL_OPERATOR(StartNewNode, new StartNewNode);