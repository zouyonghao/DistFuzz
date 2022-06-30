rm dist.tgz
mkdir dist_general_test
mkdir build
cd build
cmake -DCMAKE_RELEASE_TYPE=Release ..
make -j20
cd ..
cp build/fuzz/fuzzer dist_general_test/
cp build/general_test/general_test_main dist_general_test/
cp build/event_control/event_control dist_general_test/
cd strace/src
make
cd ../..
cp strace/src/strace dist_general_test/
pwd
cd dist_general_test
mkdir lib
ldd fuzzer | awk '{print $3}' | xargs -I _ cp _ lib
ldd general_test_main | awk '{print $3}' | xargs -I _ cp _ lib
ldd event_control | awk '{print $3}' | xargs -I _ cp _ lib
ldd strace | awk '{print $3}' | xargs -I _ cp _ lib
cd ..
cp general_test/bin dist_general_test/ -r
tar czf dist.tgz dist_general_test/
tar tvf dist.tgz
rm -rf dist_general_test