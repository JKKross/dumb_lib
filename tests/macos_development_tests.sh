#!/bin/sh

echo '==> Compiling...'
clang -std=c89 -Weverything dumb_lib_tests.c -o TEST
echo '==> Finished'
echo '==> Running the tests...'
./TEST
rm ./TEST
echo '==> Finished'
echo
echo
