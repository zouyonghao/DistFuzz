mkdir build
cd build
cmake ..
make

cd ..

cd test
../build/proxy_server/proxy_server config.json