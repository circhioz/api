add_library(cutest STATIC CuTest.c CuTest.h)

include_directories(${CMAKE_SOURCE_DIR}/src)

add_executable(unit_tests AllTests.c HahtableTest.c)
target_link_libraries(unit_tests cutest hashtable simplefs utils)

add_test(UnitTests unit_tests)
find_program (BASH_PROGRAM bash)
if (BASH_PROGRAM)
    add_test (TestCases ${BASH_PROGRAM} ${CMAKE_SOURCE_DIR}/test/test_cases.sh)
endif (BASH_PROGRAM)