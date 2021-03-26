set -e
echo $0

export CLANG=clang-9

g++ test_with_lib.cpp -c -o test.o
g++ test_lib.cpp -c -o /tmp/test_lib.o

cp ./test-config.json /tmp/compiler-config.json
../../build/fuzz/default_compiler++ test.o -o a.out
./a.out

rm test.o a.out /tmp/compiler-config.json