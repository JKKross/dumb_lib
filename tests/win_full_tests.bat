@echo off

@rem The default encoding for command prompt is Windows-1252.
@rem This changes it to utf-8.
chcp 65001

@set pat_dir=%cd%

@set path_c=..\dumb_lib_tests.c
@set path_cpp=..\dumb_lib_tests.cpp

REM - Setup Visual Studio to be able to build from Command Prompt (See the very first video of Handmade Hero for more info, around 20 minute mark)
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

cd %pat_dir%

rmdir /S /Q build
mkdir build
pushd build

echo.
echo =====================
echo ====== C tests ======
echo =====================
echo.

echo === Compiling %path_c% with 'cl'...
cl /utf-8 /validate-charset /nologo %path_c% /Fetest.exe
echo === Finished
echo === Running the tests...

test.exe
del /Q test.exe
echo === Finished
echo.

echo === Compiling %path_c% with 'clang'...

clang -std=c89 %path_c% -o test.exe
echo === Finished
echo === Running the tests...

test.exe
del /Q test.exe
echo === Finished
echo.

echo === Compiling %path_c% with 'gcc'...

gcc -std=c89 %path_c% -o test.exe
echo === Finished
echo === Running the tests...

test.exe
del /Q test.exe
echo === Finished
echo.

echo === Compiling %path_c% with 'tcc'...

tcc %path_c% -o test.exe
echo === Finished
echo === Running the tests...

test.exe
del /Q test.exe
echo === Finished

echo.
echo ==============================
echo ====== C tests finished ======
echo ==============================
echo.

REM C++ Tests
echo #define WDD_C_PLUS_PLUS_TESTS > %path_cpp%
echo.  >> %path_cpp%
REM 'type' is a Windows equivalent of UNIX 'cat'
type %path_c% >> %path_cpp%

echo.
echo =======================
echo ====== C++ tests ======
echo =======================
echo.

echo === Compiling %path_cpp% with 'cl'...
cl /utf-8 /validate-charset /nologo %path_cpp% /Fetest.exe
echo === Finished
echo === Running the tests...

test.exe
del /Q test.exe
echo === Finished
echo.

echo === Compiling %path_cpp% with 'clang'...

clang %path_cpp% -o test.exe
echo === Finished
echo === Running the tests...

test.exe
del /Q test.exe
echo === Finished
echo.

echo === Compiling %path_cpp% with 'gcc'...

gcc %path_cpp% -o test.exe
echo === Finished
echo === Running the tests...

test.exe
del /Q test.exe
echo === Finished

echo.
echo ================================
echo ====== C++ tests finished ======
echo ================================
echo.

del /Q %path_cpp%

popd

echo\
REM If there's a NO_COMMIT tag anywhere in the code, we want to know
rg "@NO_COMMIT" ../ --ignore-case -g "!*.bat" -g "!*.sh"
echo\

@rem ...and back to Windows-1252
chcp 1252
timeout /t 60
