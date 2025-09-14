#!/bin/sh

echo '==> Compiling...'
clang -std=c89 -Weverything dumb_lib_tests.c -o TEST
echo '==> Finished'
echo '==> Running the tests...'
./TEST
rm ./TEST
echo '==> Finished'

echo
# If there's a NO_COMMIT tag anywhere in the code, we want to know
rg "@NO_COMMIT" ../ --ignore-case -g "!*.bat" "!*.sh"
echo
