set -x

# rm -rf data
# mkdir data
# cp -r data_backup/* data

sudo /home/zyh/criu/scripts/criu-ns \
    restore -D checkpoint_folder_tmpfs/0 -v4 --tcp-established -vvv -o restore.log &
sudo /home/zyh/criu/scripts/criu-ns \
    restore -D checkpoint_folder_tmpfs/1 -v4 --tcp-established -vvv -o restore.log &
sudo /home/zyh/criu/scripts/criu-ns \
    restore -D checkpoint_folder_tmpfs/2 -v4 --tcp-established -vvv -o restore.log &
sudo /home/zyh/criu/scripts/criu-ns \
    restore -D checkpoint_folder_tmpfs/3 -v4 --tcp-established -vvv -o restore.log &
sudo /home/zyh/criu/scripts/criu-ns \
    restore -D checkpoint_folder_tmpfs/4 -v4 --tcp-established -vvv -o restore.log &