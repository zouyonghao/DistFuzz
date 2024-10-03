mkdir data

for i in 0 1 2 3 4; do
    mkdir -p data/{log,work$i/{smd,{sys,usr}/udf/lua}}
done

bin_path=/home/zyh/aerospike-server/target/Linux-x86_64/bin

sudo /etc/init.d/networking restart
ulimit -n 1500

$bin_path/asd --config-file aerospike_conf0 &
$bin_path/asd --config-file aerospike_conf1 &
$bin_path/asd --config-file aerospike_conf2 &
$bin_path/asd --config-file aerospike_conf3 &
$bin_path/asd --config-file aerospike_conf4 &
