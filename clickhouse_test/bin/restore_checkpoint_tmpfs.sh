set -x

cp -r run_backup_tmpfs/* run

sudo /home/zyh/criu-criu-dev/scripts/criu-ns restore --images-dir checkpoint_folder0_tmpfs -v4 -o rst.log --tcp-established &
sudo /home/zyh/criu-criu-dev/scripts/criu-ns restore --images-dir checkpoint_folder1_tmpfs -v4 -o rst.log --tcp-established &
sudo /home/zyh/criu-criu-dev/scripts/criu-ns restore --images-dir checkpoint_folder2_tmpfs -v4 -o rst.log --tcp-established &
