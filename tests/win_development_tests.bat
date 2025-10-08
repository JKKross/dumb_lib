@echo off

@rem The default encoding for command prompt is Windows-1252.
@rem This changes it to utf-8.
chcp 65001

@set pat_dir=%cd%

REM - Setup Visual Studio to be able to build from Command Prompt (See the very first video of Handmade Hero for more info, around 20 minute mark)
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

cd %pat_dir%

rmdir /S /Q build
mkdir build
pushd build

echo === Compiling...
REM cl /Zi /utf-8 /validate-charset /nologo /Wall ..\dumb_lib_tests.c /Fetest.exe
clang -std=c89 -Weverything ..\dumb_lib_tests.c -o test.exe
REM gcc -std=c89 -Wall ..\dumb_lib_tests.c -o test.exe

REM clang -std=c89 -O3 -Weverything ..\dumb_lib_tests.c -o test.exe
REM gcc -std=c89 -Wall -O3 ..\dumb_lib_tests.c -o test.exe

echo === Finished
echo === Running the tests...

test.exe

echo === Finished
echo.

popd

echo\
REM If there's a NO_COMMIT tag anywhere in the code, we want to know
rg "@NO_COMMIT" ../ --ignore-case -g "!*.bat" -g "!*.sh"

echo\

@rem ...and back to Windows-1252
chcp 1252
