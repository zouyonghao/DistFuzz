ulimit -s 81910
../../build/fuzz/fuzzer -p ssR2018Ssr -b -i input -o output -M fuzzer1 -f init_random.txt \
    /bin/bash ./run_test.sh
