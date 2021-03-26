set -e
echo $0

export CLANG=clang-9
cp ../../configs/compiler-config.json /tmp

../../build/fuzz/default_compiler++ -c test_with_lib.cpp test_lib.cpp
../../build/fuzz/default_compiler++ test_with_lib.o test_lib.o
./a.out

rm test_with_lib.o test_lib.o test_with_lib.ll test_lib.ll a.out /tmp/compiler-config.json