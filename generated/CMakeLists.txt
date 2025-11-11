cmake_minimum_required(VERSION 3.15)

include_directories(/usr/include/antlr4-runtime/)

file(GLOB PyAntlrSrc *.cpp)

add_library(PyAntlr STATIC ${PyAntlrSrc})
