1. normal start (without checkpoint and tmpfs)
    - **`env_init.sh`**
        - currently do nothing.
    - **`run_fuzz_server_normal.sh`**

2. start with **checkpoint**
    - **`env_init_checkpoint.sh`**
        - This script will create checkpoints in disk with folder `checkpoint_folder0`, `checkpoint_folder1`, `checkpoint folder2`.

    - **`run_fuzz_server_with_checkpoint.sh`**
        - This script will restore the clickhouse process from the checkpoint folders on disk.

3. start with checkpoint and **tmpfs**
    - **`env_init_checkpoint.sh`**
        - This script will create checkpoints in disk with folder `checkpoint_folder0`, `checkpoint_folder1`, `checkpoint folder2`.

    - **`env_init_tmpfs.sh`**
        - This script will create several folders and mount `tmpfs` for them.

    - **`run_fuzz_server_with_checkpoint_tmpfs.sh`**
        - This script will copy files and restore checkpoints store in tmpfs folders.