#!/bin/sh

path="dumb_lib_tests.c"
path_cpp="dumb_lib_tests.cpp"

echo '====================='
echo '====== C tests ======'
echo '====================='

echo
echo '==> Compiling with "clang"...'
clang -std=c89 $path -o TEST
echo '==> Finished'
echo '==> Running the tests...'
./TEST
echo '==> Finished'
echo
echo

echo '==> Compiling with "gcc"...'
gcc -std=c89 $path -o TEST
echo '==> Finished'
echo '==> Running the tests...'
./TEST
echo '==> Finished'
echo
echo

echo '==> Compiling with "tcc"...'
tcc $path -o TEST
echo '==> Finished'
echo '==> Running the tests...'
./TEST
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
cat $path >> $path_cpp

echo '======================='
echo '====== C++ tests ======'
echo '======================='

echo
echo '==> Compiling with "clang"...'
clang $path_cpp -o TEST
echo '==> Finished'
echo '==> Running the tests...'
./TEST
echo '==> Finished'
echo
echo

echo '==> Compiling with "gcc"...'
gcc $path_cpp -o TEST
echo '==> Finished'
echo '==> Running the tests...'
./TEST
echo '==> Finished'
echo
echo

echo '================================'
echo '====== C++ tests finished ======'
echo '================================'

rm ./TEST
rm $path_cpp

echo
# If there's a NO_COMMIT tag anywhere in the code, we want to know
rg "@NO_COMMIT" ../ --ignore-case -g "!*.bat" -g "!*.sh"
echo
