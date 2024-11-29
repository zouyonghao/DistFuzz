export NO_FAULT_FILES=xml,html,jar
export JAVA_HOME=/usr/lib/jvm/java-11-openjdk-amd64
../../build/fuzz/fuzzer -b -i input -o output -M fuzzer1 -f init_random.txt \
    ../../build/hdfs_test/hdfs_test_main \
    --node_count=3 \
    --normal_count=2 --critic_count=2 \
    --check_after_fuzz=false