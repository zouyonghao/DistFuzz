#
# source this file into your shell to set up the environment.
# For example:
# $ . /home/zyh/DistFuzz/ceph_test/bin/vstart_environment.sh
#
export PYTHONPATH=/home/zyh/ceph//src/pybind:/home/zyh/ceph/build//lib/cython_modules/lib.3:/home/zyh/ceph//src/python-common:$PYTHONPATH
export LD_LIBRARY_PATH=/home/zyh/ceph/build//lib:$LD_LIBRARY_PATH
export PATH=/home/zyh/DistFuzz/ceph_test/bin/bin:$PATH
alias cephfs-shell=/home/zyh/ceph//src/tools/cephfs/cephfs-shell
