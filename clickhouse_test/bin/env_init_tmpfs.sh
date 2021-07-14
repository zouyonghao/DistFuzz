# before run this script, you must run env_init_checkpoint.sh first!

./stop.sh

./env_clear_tmpfs.sh

mkdir run
sudo mount -t tmpfs none run

mkdir run_backup_tmpfs
sudo mount -t tmpfs none run_backup_tmpfs
cp -r run_backup/* run_backup_tmpfs/
# cp -r run_backup_tmpfs/* run # this should be done for each test case

mkdir checkpoint_folder0_tmpfs
mkdir checkpoint_folder1_tmpfs
mkdir checkpoint_folder2_tmpfs

sudo mount -t tmpfs none checkpoint_folder0_tmpfs
sudo mount -t tmpfs none checkpoint_folder1_tmpfs
sudo mount -t tmpfs none checkpoint_folder2_tmpfs

cp -r checkpoint_folder0/* checkpoint_folder0_tmpfs/
cp -r checkpoint_folder1/* checkpoint_folder1_tmpfs/
cp -r checkpoint_folder2/* checkpoint_folder2_tmpfs/

sudo chmod -R 777 checkpoint_folder0_tmpfs
sudo chmod -R 777 checkpoint_folder1_tmpfs
sudo chmod -R 777 checkpoint_folder2_tmpfs

sudo mount -l
