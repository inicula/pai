#!/bin/sh

set -xe

CXXFLAGS="-std=c++20 -O0 -ggdb -march=native -Wall -Wextra -Wconversion"
CXX=clang++

bison -d pai.y
flex pai.l
sed 's/.*yywrap.*return.*//g' pai_lexer.cc -i
$CXX $CXXFLAGS "$@" pai.cpp pai_lexer.cc pai_parser.cc -o pai -lfmt
