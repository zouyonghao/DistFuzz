./stop.sh
rm log*
rm *.txt
rm -rf data*
echo "./test_cases/$1/random*"
cp ./error_cases/$1/init_random.txt ./
/home/zyh/DistFuzz/build/clickhouse_test/clickhouse_test_main init_random.txt > log_test 2> log_test_err
