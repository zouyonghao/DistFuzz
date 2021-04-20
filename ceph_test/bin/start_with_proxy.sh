CEPH_BUILD_PATH=/home/zyh/ceph/build

. ./vstart_environment.sh

../../build/proxy_server/proxy_server ./proxy-config.json > log_proxy 2>&1 &
# ../../build/proxy_server/proxy_server ./proxy-config-backup.json > log_proxy 2>&1 &
../../build/event_control/event_control > log_event_control 2>&1 &

# . ./vstart_environment.sh

${CEPH_BUILD_PATH}/bin/ceph-mon -i a -c ./ceph.conf &

# sleep 5

${CEPH_BUILD_PATH}/bin/ceph-osd -i 0 -c ./ceph.conf &
${CEPH_BUILD_PATH}/bin/ceph-osd -i 1 -c ./ceph.conf &
${CEPH_BUILD_PATH}/bin/ceph-osd -i 2 -c ./ceph_with_proxy1.conf &


# ${CEPH_BUILD_PATH}/bin/ceph-mgr -i x -c ./ceph.conf &