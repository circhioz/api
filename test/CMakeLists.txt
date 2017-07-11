include_directories(${CMAKE_SOURCE_DIR}/src)

add_executable(test-hashtable test_hashtable.c)
target_link_libraries(test-hashtable hashtable utils -lm)

add_test(HashtableTest test-hashtable)