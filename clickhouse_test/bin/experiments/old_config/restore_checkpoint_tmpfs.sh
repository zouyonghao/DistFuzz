set -x

cp -r run_backup_tmpfs/* run

sudo criu-ns restore --images-dir checkpoint_folder0_tmpfs -v4 -o rst.log --tcp-established &
sudo criu-ns restore --images-dir checkpoint_folder1_tmpfs -v4 -o rst.log --tcp-established &
sudo criu-ns restore --images-dir checkpoint_folder2_tmpfs -v4 -o rst.log --tcp-established &
