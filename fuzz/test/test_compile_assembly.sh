set -e
echo $0

export CLANG=clang-9
cp ../../configs/compiler-config.json /tmp
../../build/fuzz/default_compiler++ test.s

./a.out
rm a.out