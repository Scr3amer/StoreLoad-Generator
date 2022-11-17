# StoreLoad-Generator
A proof of concept on how to trigger CPU StoreLoad reordering efficiently.

## DISCLAIMER:
I am not an expert, maybe this code is totally wrong but the observed behavior seems to make sense.

### To prevent StoreLoad reordering:
clang++ -std=c++20 -O3 -o app.exe main.cpp

### To trigger StoreLoad reordering:
clang++ -std=c++20 -O3 -DTRIGGER_STORELOAD -o app.exe main.cpp 

### Note
C++20 is required to use the constructor of std::barrier used in that file.
Tested on this [Clang 15.0.0 llvm-mingw](https://github.com/mstorsjo/llvm-mingw/releases/download/20220906/llvm-mingw-20220906-ucrt-x86_64.zip) 
