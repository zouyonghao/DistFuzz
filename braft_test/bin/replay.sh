rm log*
rm *.txt
rm -rf data*
echo "./test_cases/$1/random*"
cp ./test_cases/$1/init_random.txt ./
 __DST_ENV_RANDOM_FILE__=init_random.txt REPLAY=true ../../build/braft_test/braft_test_main init_random.txt 
