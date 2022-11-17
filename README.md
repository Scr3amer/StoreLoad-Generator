# StoreLoad-Generator
A proof of concept on how to trigger CPU StoreLoad reordering efficiently

DISCLAIMER:
I am not an expert, maybe this code is totally wrong but the observed behavior seems to make sense.

To compile it with no barrier:
clang++ -std=c++20 -O3 -o app.exe main.cpp

To compile it with compiler barrier only:
clang++ -std=c++20 -O3 -DCOMPILER_BARRIER -o app.exe main.cpp 

To compile it with compiler and CPU barrier with clang/gcc specific inline asm:
clang++ -std=c++20 -O3 -DASM_FULL_BARRIER -o app.exe main.cpp 

To compile it with compiler and CPU barrier with C++11 barrier:
clang++ -std=c++20 -O3 -DCPP11_FULL_BARRIER -o app.exe main.cpp 

C++20 is required to use the constructor of std::barrier used in that file.

Tested on Clang 15.0.0 from https://github.com/mstorsjo/llvm-mingw
