set -x

sudo /home/zyh/criu/scripts/criu-ns \
    restore -D checkpoint_folder_tmpfs/1 -v4 --tcp-established --shell-job -o restore.log &
sudo /home/zyh/criu/scripts/criu-ns \
    restore -D checkpoint_folder_tmpfs/2 -v4 --tcp-established --shell-job -o restore.log &
sudo /home/zyh/criu/scripts/criu-ns \
    restore -D checkpoint_folder_tmpfs/3 -v4 --tcp-established --shell-job -o restore.log &
sudo /home/zyh/criu/scripts/criu-ns \
    restore -D checkpoint_folder_tmpfs/4 -v4 --tcp-established --shell-job -o restore.log &
sudo /home/zyh/criu/scripts/criu-ns \
    restore -D checkpoint_folder_tmpfs/5 -v4 --tcp-established --shell-job -o restore.log &