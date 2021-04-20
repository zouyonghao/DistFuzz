CEPH_BUILD_PATH=/home/zyh/ceph/build

. ./vstart_environment.sh

${CEPH_BUILD_PATH}/bin/ceph-mon -i a -c ./ceph.conf &
${CEPH_BUILD_PATH}/bin/ceph-mgr -i x -c ./ceph.conf &
${CEPH_BUILD_PATH}/bin/ceph-osd -i 0 -c ./ceph.conf &
${CEPH_BUILD_PATH}/bin/ceph-osd -i 1 -c ./ceph.conf &
${CEPH_BUILD_PATH}/bin/ceph-osd -i 2 -c ./ceph.conf &