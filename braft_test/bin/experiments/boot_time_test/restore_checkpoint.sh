set -x

# rm -rf data
# mkdir data
# cp -r data_backup/* data

# # TODO: set inherit-fd dyanmicly!
# sudo /home/zyh/criu/criu/criu restore -D checkpoint_folder/0 --ext-unix-sk --tcp-close \
#     --inherit-fd fd[0]:socket:[90540] --inherit-fd fd[1]:socket:[91583] --inherit-fd fd[2]:socket:[85906] -v

# for i in 0 1 2 3 4; do
#     # sudo /home/zyh/criu/criu/criu \
#     sudo /home/zyh/criu/scripts/criu-ns \
#         restore -D checkpoint_folder/$i -v4 --tcp-established -vvv -o restore.log &
# done

# sudo chmod 777 -R checkpoint_folder

sudo /home/zyh/criu/scripts/criu-ns \
    restore -D checkpoint_folder/0 -v4 --tcp-established -vvv -o restore.log &
sudo /home/zyh/criu/scripts/criu-ns \
    restore -D checkpoint_folder/1 -v4 --tcp-established -vvv -o restore.log &
sudo /home/zyh/criu/scripts/criu-ns \
    restore -D checkpoint_folder/2 -v4 --tcp-established -vvv -o restore.log &
sudo /home/zyh/criu/scripts/criu-ns \
    restore -D checkpoint_folder/3 -v4 --tcp-established -vvv -o restore.log &
sudo /home/zyh/criu/scripts/criu-ns \
    restore -D checkpoint_folder/4 -v4 --tcp-established -vvv -o restore.log &