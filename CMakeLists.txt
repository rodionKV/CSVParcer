cmake_minimum_required(VERSION 3.21)
project(CSVParcer)

set(CMAKE_CXX_STANDARD 17)

add_executable(CSVParcer main.cpp CSVParser.h CSVIterator.h TupleS.h)
