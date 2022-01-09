# before run this script, you must run env_init_checkpoint.sh first!

mkdir data
sudo mount -t tmpfs none data

# mkdir run_backup_tmpfs
# sudo mount -t tmpfs none run_backup_tmpfs
# cp -r run_backup/* run_backup_tmpfs/
# cp -r run_backup_tmpfs/* run # this should be done for each test case

./env_init_checkpoint.sh

mkdir -p checkpoint_folder_tmpfs
sudo mount -t tmpfs none checkpoint_folder_tmpfs/

for i in 1 2 3 4 5; do
    # mkdir -p checkpoint_folder_tmpfs/$i
    cp -r checkpoint_folder/$i checkpoint_folder_tmpfs/
    sudo chmod -R 777 checkpoint_folder_tmpfs/$i
done