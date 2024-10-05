rm -rf run/*
rm log*
rm *log
rm -rf zookeeper/data

sudo umount run
sudo umount run_backup_tmpfs
sudo umount checkpoint_folder0_tmpfs
sudo umount checkpoint_folder1_tmpfs
sudo umount checkpoint_folder2_tmpfs

rm -rf run
rm -rf run_backup_tmpfs
rm -rf checkpoint_folder0_tmpfs
rm -rf checkpoint_folder1_tmpfs
rm -rf checkpoint_folder2_tmpfs