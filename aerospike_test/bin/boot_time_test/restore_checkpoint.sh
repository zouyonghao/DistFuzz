set -x

sudo /home/zyh/criu/criu/criu \
    restore -D checkpoint_folder/0 -v4 --tcp-established --shell-job -vvv -o restore.log &
sudo /home/zyh/criu/criu/criu \
    restore -D checkpoint_folder/1 -v4 --tcp-established --shell-job -vvv -o restore.log &
sudo /home/zyh/criu/criu/criu \
    restore -D checkpoint_folder/2 -v4 --tcp-established --shell-job -vvv -o restore.log &
sudo /home/zyh/criu/criu/criu \
    restore -D checkpoint_folder/3 -v4 --tcp-established --shell-job -vvv -o restore.log &
sudo /home/zyh/criu/criu/criu \
    restore -D checkpoint_folder/4 -v4 --tcp-established --shell-job -vvv -o restore.log &