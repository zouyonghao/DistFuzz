mkdir -p test_cases/$1

pwd

cp data1/LOG LOG1
cp data2/LOG LOG2
cp data3/LOG LOG3

mv *.log ./test_cases/$1
mv log* ./test_cases/$1
mv LOG* ./test_cases/$1
# mv data* ./test_cases/$1
rm -rf data*
mv random_node0.txt ./test_cases/$1
mv random_node1.txt ./test_cases/$1
mv random_node2.txt ./test_cases/$1
mv random_proxy.txt ./test_cases/$1
mv random.txt ./test_cases/$1
mv init_random.txt ./test_cases/$1
mv operation_log ./test_cases/$1

# checking the operation log!
{
    ./check.sh ${PWD}/test_cases/$1/operation_log >${PWD}/test_cases/$1/check_log 2>&1
    if grep -q '^E' ./test_cases/$1/LOG*; then
        echo "Find errors!"
    elif grep -q "AddressSanitizer" ./test_cases/$1/LOG*; then
        echo "Find ASan errors!"
    elif grep -q "AddressSanitizer" ./test_cases/$1/log_test; then
        echo "Find ASan errors!"
    elif grep -q true ./test_cases/$1/check_log; then
        echo "No errors, deleting logs..."
        rm -rf ./test_cases/$1
    else
        echo "Find operation log errors!"
    fi
} &
