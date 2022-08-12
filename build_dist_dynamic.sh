rm dist.tgz
rm -rf dist_general_test build
mkdir dist_general_test
mkdir build
cd build
cmake -DDIST_FUZZ_STATIC_BUILD=OFF -DDIST_FUZZ_BUILD_COMPILER=OFF -DDIST_FUZZ_DEFAULT_TEST=OFF -DCMAKE_BUILD_TYPE=Release ..
make -j20
cd ..
cp build/fuzz/fuzzer-licensed dist_general_test/fuzzer
cp build/general_test/general_test_main dist_general_test/
cp build/event_control/event_control dist_general_test/
cd strace
./bootstrap
./configure --enable-static=no
make -j20
cd ..
cp strace/src/strace dist_general_test/
pwd

cd general_test/bin
rm -rf licenses output file_lock init_random.txt sync_file sync_branch_cov_file test_case_count test_cases
./issue-1m.sh
cd ../..
cp general_test/bin dist_general_test/ -r
rm -r dist_general_test/bin/issue-1m.sh \
  dist_general_test/bin/run_random_test.sh \
  dist_general_test/bin/output \
  dist_general_test/bin/test_cases \
  dist_general_test/bin/sync_file \
  dist_general_test/bin/sync_branch_cov_file \
  dist_general_test/bin/test_case_count
cp doc/doc.pdf dist_general_test/
tar czf dist.tgz dist_general_test/
tar tvf dist.tgz
rm -rf dist_general_test
