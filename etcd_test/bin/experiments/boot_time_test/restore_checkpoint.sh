set -x

sudo criu-ns \
    restore -D checkpoint_folder/0 -v4 --tcp-established -vvv -o restore.log &
sudo criu-ns \
    restore -D checkpoint_folder/1 -v4 --tcp-established -vvv -o restore.log &
sudo criu-ns \
    restore -D checkpoint_folder/2 -v4 --tcp-established -vvv -o restore.log &
sudo criu-ns \
    restore -D checkpoint_folder/3 -v4 --tcp-established -vvv -o restore.log &
sudo criu-ns \
    restore -D checkpoint_folder/4 -v4 --tcp-established -vvv -o restore.log &