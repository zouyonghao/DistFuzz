./run_fuzz_server_normal.sh
./ensure_nodes.sh

# cp -r data data_backup

for i in 0 1 2 3 4; do
    mkdir checkpoint_folder/$i
    PID=$(ps -ef | grep calc_server | grep -v grep | grep data/$i | awk '{print $2}')
    echo $PID
    sudo ~/criu/criu/criu \
        dump --tree $(ps -ef | grep calc_server | grep -v grep | grep $(python -c "print 10000+$i") | awk '{print $2}') \
        -D checkpoint_folder/$i -v4 --tcp-established -vvv -o dump.log && echo OK
done

cp -r data data_backup

sudo chmod 777 -R checkpoint_folder

./stop.sh
