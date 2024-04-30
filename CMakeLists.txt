cmake_minimum_required(VERSION 3.27)
project(CDataFrame2 C)

set(CMAKE_C_STANDARD 23)

add_executable(CDataFrame2 main.c
        column.h
        column.c
        cdataframe.h
        cdataframe.c)
