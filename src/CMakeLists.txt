add_library(utils STATIC utils.c utils.h)

add_library(hashtable STATIC hashtable.c hashtable.h)
add_dependencies(hashtable utils)

add_library(simplefs STATIC simplefs.c simplefs.h)
add_dependencies(simplefs hashtable utils)

add_executable(project main.c)
target_link_libraries(project simplefs hashtable utils)