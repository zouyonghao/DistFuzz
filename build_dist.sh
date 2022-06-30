mkdir dist_general_test
mkdir build
cd build
cmake -DCMAKE_RELEASE_TYPE=Release ..
make -j20
cd ..
cp build/fuzz/fuzzer dist_general_test/
cp build/general_test/general_test_main dist_general_test/
cp build/event_control/event_control dist_general_test/
cd dist_general_test
ldd fuzzer | awk '{print $3}' | xargs -I _ cp _ lib
ldd general_test_main | awk '{print $3}' | xargs -I _ cp _ lib
ldd event_control | awk '{print $3}' | xargs -I _ cp _ lib
cd ..
tar czf dist.tgz dist_general_test/
tar tvf dist.tgz
rm -rf dist_general_test