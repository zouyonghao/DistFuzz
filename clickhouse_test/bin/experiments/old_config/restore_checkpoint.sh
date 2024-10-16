set -x

cp -r run_backup/* run

sudo criu-ns restore --images-dir checkpoint_folder0 -v4 -o rst.log --tcp-established &
sudo criu-ns restore --images-dir checkpoint_folder1 -v4 -o rst.log --tcp-established &
sudo criu-ns restore --images-dir checkpoint_folder2 -v4 -o rst.log --tcp-established &
