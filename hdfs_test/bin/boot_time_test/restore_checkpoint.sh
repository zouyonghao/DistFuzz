set -x

sudo /home/zyh/criu/scripts/criu-ns \
    restore -D checkpoint_folder/1 -v4 --tcp-established -j -d -o restore.log &
sudo /home/zyh/criu/scripts/criu-ns \
    restore -D checkpoint_folder/2 -v4 --tcp-established -j -d -o restore.log &
sudo /home/zyh/criu/scripts/criu-ns \
    restore -D checkpoint_folder/3 -v4 --tcp-established -j -d -o restore.log &
sudo /home/zyh/criu/scripts/criu-ns \
    restore -D checkpoint_folder/4 -v4 --tcp-established -j -d -o restore.log &
sudo /home/zyh/criu/scripts/criu-ns \
    restore -D checkpoint_folder/5 -v4 --tcp-established -j -d -o restore.log &