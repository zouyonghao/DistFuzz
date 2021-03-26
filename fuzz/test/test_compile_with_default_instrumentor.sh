set -e
echo $0

export CLANG=clang-9
cp ../../configs/compiler-config.json /tmp
cp ../../build/fuzz/CMakeFiles/default_instrument_lib.dir/instrumentor_lib/default_instrument_lib.c.o /tmp

../../build/fuzz/default_compiler++ test.cpp -c

../../build/fuzz/default_compiler++ test.o
./a.out

rm test.o test.ll a.out