set -e
echo $0

export CLANG=clang-9
cp ../../configs/compiler-config.json /tmp
../../build/fuzz/default_compiler++ test.cpp # this will fail for now

./a.out
rm a.out