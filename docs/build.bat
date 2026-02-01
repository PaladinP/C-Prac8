@echo off

REM Dr SP Sithungu
REM P4 Solution Batch File.
REM 15 August 2025

REM Create the bin directory if it doesn't exist
if not exist "..\bin" mkdir "..\bin"

REM Create the docs directory if it doesn't exist
if not exist "..\docs" mkdir "..\docs"

REM Create the output directory if it doesn't exist
if not exist "..\output" mkdir "..\output"

cd ../src

echo Compiling.
REM Listing the source files individually prevents issues that may arise if the compiler 
REM reads main.cpp before UJImage.cpp (module not yet exists).
g++ --std=c++20 -fmodules-ts -c LibUtility.cpp
g++ --std=c++20 -fmodules-ts -c UJImage.cpp
g++ --std=c++20 -fmodules-ts -c main.cpp
g++ LibUtility.o UJImage.o main.o -o ..\bin\prog

echo Cleaning up.
REM Deletes the gcm.cache directory and all its contents (/s) quietly (/q)
rmdir /s /q gcm.cache
del *.o

echo Testing Image generation.
REM ..\bin\prog > ..\output\image.ppm
..\bin\prog

echo Program executed successfully.
pause
