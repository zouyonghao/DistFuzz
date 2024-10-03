rm checkpoint_folder -rf
mkdir checkpoint_folder

./run_fuzz_server_normal.sh
./ensure_nodes.sh

# cp -r data data_backup

for i in 0 1 2 3 4; do
    mkdir checkpoint_folder/$i
    PID=$(ps -ef | grep atomic_server | grep -v grep | grep data/$i | awk '{print $2}')
    echo $PID
    sudo /home/zyh/criu/scripts/criu-ns \
        dump --tree $(ps -ef | grep atomic_server | grep -v grep | grep data/$i | awk '{print $2}') \
        -D checkpoint_folder/$i -v4 --tcp-established -vvv -o dump.log && echo OK
done

cp -r data data_backup

sudo chmod 777 -R checkpoint_folder

./stop.sh

# Only when backuping via vscode terminal, we need these line....WTF!
# lsof -aU -p $PID
# lsof -aU -p $PID > checkpoint_folder/"$i"_external
# EXTERNAL=$(lsof -aU -p $PID | tail -n +2 | awk -v a="--external unix[" -v b="] " '{s=(a $8 b)s}END{print s}')
# echo $EXTERNAL
# sudo /home/zyh/criu/criu/criu \
#     dump --tree $(ps -ef | grep atomic_server | grep -v grep | grep data/$i | awk '{print $2}') \
#     -D checkpoint_folder/$i --shell-job -v4 -x --tcp-close -vvv -o dump.log --ext-unix-sk $EXTERNAL && echo OK
# sudo /home/zyh/criu/criu/criu \
