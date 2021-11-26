mkdir build
cd build
cmake ..
make -j12
make test

cd ..

# cd test
# ../build/proxy_server/proxy_server config.json