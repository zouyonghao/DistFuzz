# issue a license for 1 month
mkdir licenses
../../build/licensecc/extern/license-generator/src/license_generator/lccgen \
    license issue -p /home/zyh/distributed-system-test/projects/MocoTest/ \
    -o licenses/license.lic \
    --valid-to `date -d "$(date +%Y-%m-%d) 1 month" +%F`
../../build/fuzz/issue-date licenses