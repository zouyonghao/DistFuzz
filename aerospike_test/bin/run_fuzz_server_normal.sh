bin_path=/home/zyh/aerospike-server/target/Linux-x86_64/bin

mkdir -p run/{log,work1/{smd,{sys,usr}/udf/lua},work2/{smd,{sys,usr}/udf/lua},work3/{smd,{sys,usr}/udf/lua}}

$bin_path/asd --config-file aerospike_mesh1.conf &
$bin_path/asd --config-file aerospike_mesh2.conf &
$bin_path/asd --config-file aerospike_mesh3.conf &

# rm calc1.pipe
# rm calc2.pipe
# rm calc3.pipe