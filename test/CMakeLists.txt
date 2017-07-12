include_directories(${CMAKE_SOURCE_DIR}/src)

set(cheat_INCLUDES cheat.h cheats.h)

add_executable(test-hashtable test_hashtable.c ${cheat_INCLUDES})
target_link_libraries(test-hashtable hashtable utils -lm)

add_executable(test-simplefs test_simplefs.c ${cheat_INCLUDES})
target_link_libraries(test-simplefs simplefs hashtable utils -lm)

add_test(HashtableTest test-hashtable)
add_test(FileSystemTest test-simplefs)