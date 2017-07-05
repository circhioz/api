cmake_minimum_required(VERSION 3.2)
project(SimpleFS)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES hashtable.c hashtable.h utils.c utils.h simplefs.c simplefs.h strlist.c strlist.h main.c)

add_executable(SimpleFS ${SOURCE_FILES})