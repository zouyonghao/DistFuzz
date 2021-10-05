mkdir -p test_cases/$1

pwd

# CHECK_RAFT_LOG_ERROR=1
# # check log
# if ./check_log.sh > log_check_raft 2>&1; then
#     CHECK_RAFT_LOG_ERROR=0
# fi

mv log* ./test_cases/$1
mv c-raft_data* ./test_cases/$1
mv random_node0.txt ./test_cases/$1
mv random_node1.txt ./test_cases/$1
mv random_node2.txt ./test_cases/$1
mv random_proxy.txt ./test_cases/$1
mv random.txt ./test_cases/$1
mv init_random.txt ./test_cases/$1
mv operation_log ./test_cases/$1

# checking the operation log!
{
    # ./check.sh ${PWD}/test_cases/$1/operation_log >${PWD}/test_cases/$1/check_log 2>&1
    if grep -q '):' ./test_cases/$1/log*; then
        echo "Find errors!"
    # elif [ $CHECK_RAFT_LOG_ERROR == 1 ]; then
    #     echo "Check raft log failed!"
    elif grep -q "ERROR: AddressSanitizer" ./test_cases/$1/log[0-2]; then
        echo "Find ASan errors!"
	if grep -q "0x81c9b" ./test_cases/$1/log[0-2]; then
		echo "Find a duplicated case"
		rm -rf ./test_cases/$1
	fi
    # elif grep -q true ./test_cases/$1/check_log; then
    #     echo "No errors, deleting logs..."
    #     rm -rf ./test_cases/$1
    else
        echo "No errors, deleting logs..."
        rm -rf ./test_cases/$1
    fi
} &
