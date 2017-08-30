add_library(utils SHARED utils.c utils.h)

add_library(hashtable SHARED hashtable.c hashtable.h)
target_link_libraries(hashtable utils)

add_library(simplefs SHARED simplefs.c simplefs.h)
target_link_libraries(simplefs hashtable utils)

add_executable(project main.c)
target_link_libraries(project simplefs utils)