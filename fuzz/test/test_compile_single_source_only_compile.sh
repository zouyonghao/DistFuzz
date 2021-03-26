set -e
echo $0

export CLANG=clang-9
cp ../../configs/compiler-config.json /tmp
# cp ../../build/fuzz/*.so /tmp

cp ../../build/fuzz/CMakeFiles/default_instrument_lib.dir/instrumentor_lib/default_instrument_lib.c.o /tmp
../../build/fuzz/default_compiler++ test.cpp -c

# clang++-9 test.o -Wl,-R -Wl,/tmp /tmp/libdefault_instrument_lib.so
clang++-9 test.o /tmp/default_instrument_lib.c.o
./a.out

rm test.o test.ll a.out