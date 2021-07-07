rm log*
rm *.txt
rm -rf data*
echo "./test_cases/$1/random*"
cp ./error_cases/$1/init_random.txt ./
/home/zyh/distributed-system-test/build/clickhouse_test/clickhouse_test_main init_random.txt 
