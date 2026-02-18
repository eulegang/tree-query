find_package(GTest REQUIRED)

enable_testing()

file(GLOB TEST_SRC "tests/*.cpp")

add_executable(unit ${TEST_SRC} $<FILTER:${EXEC_SRC},EXCLUDE,main.cpp>)

target_link_libraries(unit PRIVATE ${TREESITTER_LDFLAGS})
target_compile_options(unit PRIVATE ${TREESITTER_CFLAGS})
target_link_libraries(unit PUBLIC ${MFILE_LDFLAGS})
target_compile_options(unit PUBLIC ${MFILE_CFLAGS})
target_link_libraries(unit PUBLIC ts)
target_link_libraries(unit PUBLIC GTest::gtest_main)

add_test(unit unit)
