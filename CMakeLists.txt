cmake_minimum_required(VERSION 3.8)
project(API)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES hashtable.c utils.c utils.h simplefs.c simplefs.h strlist.c strlist.h main.c)
add_executable(API ${SOURCE_FILES})