pkg_check_modules(GTEST REQUIRED gtest)
if(NOT GTEST_FOUND)
  message(FATAL_ERROR "failed to find gtest on system")
endif()
enable_testing()

file(GLOB TEST_SRC "tests/*.cpp")

add_executable(unit ${TEST_SRC} tests/main.cpp $<FILTER:${EXEC_SRC},EXCLUDE,main.cpp>)

target_link_libraries(unit PRIVATE ${GTEST_LDFLAGS})
target_compile_options(unit PRIVATE ${GTEST_CFLAGS})
target_link_libraries(unit PRIVATE ${TREESITTER_LDFLAGS})
target_compile_options(unit PRIVATE ${TREESITTER_CFLAGS})

add_test(unit unit)
