#!/bin/sh

set -xe

CXXFLAGS="-std=c++20 -ggdb -O0 -fno-exceptions -fno-rtti -march=native -Wall -Wextra -Wconversion"
CXX=clang++

bison -d pai.y
flex pai.l

# The C++ lexer generator is a bit broken. It defines the function `yywrap()`
# twice (once in pai_lexer.h and once in pai_lexer.cpp), leading to a linker
# error. The following command gets around this issue.
sed 's/.*yywrap.*return.*//g' pai_lexer.cpp -i

$CXX $CXXFLAGS "$@" pai.cpp pai_lexer.cpp pai_parser.cpp -o pai -lfmt
