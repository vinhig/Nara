mkdir -p build
cd build
cmake ..
make TArray
env CTEST_OUTPUT_ON_FAILURE=1 make test