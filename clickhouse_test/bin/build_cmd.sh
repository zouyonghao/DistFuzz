# cmake .. -DENABLE_CLICKHOUSE_ALL=OFF \
#      -DENABLE_CLICKHOUSE_SERVER=ON \
#      -DENABLE_CLICKHOUSE_CLIENT=ON \
#      -DENABLE_LIBRARIES=OFF \
#      -DENABLE_UTILS=OFF \
#      -DENABLE_TESTS=OFF \
#      -DSANITIZE=address

export CC=/home/zyh/distributed-system-test/build/fuzz/default_compiler CXX=/home/zyh/distributed-system-test/build/fuzz/default_compiler++
cmake ..  \
        -DCMAKE_BUILD_TYPE=Debug     \
        -DENABLE_CLICKHOUSE_ALL=OFF     \
        -DENABLE_CLICKHOUSE_SERVER=ON     \
        -DENABLE_CLICKHOUSE_CLIENT=ON     \
        -DENABLE_LIBRARIES=OFF     \
        -DUSE_UNWIND=ON     \
        -DENABLE_UTILS=OFF     \
        -DENABLE_TESTS=OFF -DSANITIZE=address \
        -DENABLE_NURAFT=ON \
        -DENABLE_CLICKHOUSE_KEEPER=ON

make -j8