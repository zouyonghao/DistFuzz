mkdir -p test_cases/$1

pwd

mv run ./test_cases/$1
mv log* ./test_cases/$1
mv *log ./test_cases/$1
mv random_node0.txt ./test_cases/$1
mv random_node1.txt ./test_cases/$1
mv random_node2.txt ./test_cases/$1
mv random_proxy.txt ./test_cases/$1
mv random.txt ./test_cases/$1
mv init_random.txt ./test_cases/$1
mv operation_log ./test_cases/$1
mv zookeeper/data ./test_cases/$1

# checking the operation log!
{
    # ./check.sh ${PWD}/test_cases/$1/operation_log >${PWD}/test_cases/$1/check_log 2>&1
    if grep -q 'Error' ./test_cases/$1/run/log*; then
        echo "Find errors!"
    elif grep -q 'Error' ./test_cases/$1/run/err_log*; then
        echo "Find errors!"
    elif grep -q 'Fatal' ./test_cases/$1/run/err_log*; then
        echo "Find Fatal!"
    elif grep -q "ERROR: AddressSanitizer" ./test_cases/$1/run/log*; then
        echo "Find ASan errors!"
    elif grep -q "ERROR: AddressSanitizer" ./test_cases/$1/run/err_log*; then
        echo "Find ASan errors!"
    # elif grep -q true ./test_cases/$1/check_log; then
    #     echo "No errors, deleting logs..."
    #     rm -rf ./test_cases/$1
    else
        echo "No errors?"
        echo "No errors, deleting logs..."
    fi
} &

# ./env_clear.sh