#!/bin/sh

rm -rf build
mkdir build

path_c="./dumb_lib_tests.c"
path_cpp="./dumb_lib_tests.cpp"

echo '====================='
echo '====== C tests ======'
echo '====================='
echo

echo '==> Compiling with "clang"...'
clang -std=c89 -o ./build/TEST $path_c
echo '==> Finished'
echo '==> Running the tests...'
./build/TEST
rm ./build/TEST
echo '==> Finished'
echo

echo
echo '==> Compiling with "gcc"...'
gcc -std=c89 $path_c -o ./build/TEST
echo '==> Finished'
echo '==> Running the tests...'
./build/TEST
rm ./build/TEST
echo '==> Finished'
echo

echo
echo '==> Compiling with "tcc"...'
tcc $path_c -o ./build/TEST
echo '==> Finished'
echo '==> Running the tests...'
./build/TEST
rm ./build/TEST
echo '==> Finished'
echo

echo
echo '=============================='
echo '====== C tests finished ======'
echo '=============================='
echo

# C++ Tests

echo '#define WDD_C_PLUS_PLUS_TESTS' > $path_cpp
echo '' >> $path_cpp
cat $path_c >> $path_cpp

echo '======================='
echo '====== C++ tests ======'
echo '======================='
echo

echo '==> Compiling with "clang"...'
clang $path_cpp -o ./build/TEST
echo '==> Finished'
echo '==> Running the tests...'
./build/TEST
rm ./build/TEST
echo '==> Finished'
echo

echo
echo '==> Compiling with "gcc"...'
gcc $path_cpp -o ./build/TEST
echo '==> Finished'
echo '==> Running the tests...'
./build/TEST
rm ./build/TEST
echo '==> Finished'
echo

echo '================================'
echo '====== C++ tests finished ======'
echo '================================'

rm $path_cpp

rg "@NO_COMMIT" ../* --ignore-case -g "!*.bat" -g "!*.sh"
