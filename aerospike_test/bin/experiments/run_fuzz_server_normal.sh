bin_path=/home/zyh/aerospike-server/target/Linux-x86_64/bin

mkdir -p run/{log,work1/{smd,{sys,usr}/udf/lua},work2/{smd,{sys,usr}/udf/lua},work3/{smd,{sys,usr}/udf/lua}}

LD_PRELOAD=/home/zyh/DistFuzz/build/preload_module/libdst_preload.so \
$bin_path/asd --config-file aerospike_mesh1.conf &
LD_PRELOAD=/home/zyh/DistFuzz/build/preload_module/libdst_preload.so \
$bin_path/asd --config-file aerospike_mesh2.conf &
LD_PRELOAD=/home/zyh/DistFuzz/build/preload_module/libdst_preload.so \
$bin_path/asd --config-file aerospike_mesh3.conf &

