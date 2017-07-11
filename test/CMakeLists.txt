add_executable(test-hashtable main.c)
add_dependencies(test-hashtable hashtable utils)
target_link_libraries(test-hashtable hashtable utils)

add_executable(test-filesystem main.c)
add_dependencies(test-filesystem simplefs utils)
target_link_libraries(test-filesystem simplefs  utils)

add_test(Hashtable test-hashtable)
add_test(Filesystem test-filesystem)

find_program (BASH_PROGRAM bash)
if (BASH_PROGRAM)
    add_test (TestCases ${BASH_PROGRAM} ${CMAKE_SOURCE_DIR}/test/test_cases.sh)
endif (BASH_PROGRAM)