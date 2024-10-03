rm -rf data/*

# run for running folder
# run_backup_tmpfs for backup folder
# checkpoint folder for checkpoint backup folder

sudo umount data
sudo umount data_backup_tmpfs
# sudo umount checkpoint_folder
sudo umount checkpoint_folder_tmpfs

rm -rf data
rm -rf data_backup_tmpfs
# rm -rf checkpoint_folder
rm -rf checkpoint_folder_tmpfs

# rm -rf rethinkdb_data*
sudo tunctl -d tap0