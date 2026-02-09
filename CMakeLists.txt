cmake_minimum_required(VERSION 3.10)
project(tree-query
  LANGUAGES CXX
  VERSION 0.1.0
  DESCRIPTION "query a codebase using tree sitter"
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(PkgConfig REQUIRED)

file(GLOB EXEC_SRC "src/*.cpp")
add_executable(tq ${EXEC_SRC})
add_compile_options(-Wall -Wextra -Wpedantic -Werror)
include_directories(src/)

pkg_check_modules(TREESITTER REQUIRED tree-sitter)
target_link_libraries(tq PRIVATE ${TREESITTER_LDFLAGS})
target_compile_options(tq PRIVATE ${TREESITTER_CFLAGS})

include_directories(lib/ts)
file(GLOB TS_SRC "lib/ts/*.cpp")
add_library(ts ${TS_SRC})
target_link_libraries(ts PRIVATE ${TREESITTER_LDFLAGS})
target_compile_options(ts PRIVATE ${TREESITTER_CFLAGS})

include_directories(lib/mfile)
file(GLOB MFILE_SRC "lib/mfile/*.cpp")
add_library(mfile ${MFILE_SRC})

target_link_libraries(tq PUBLIC ts)
target_link_libraries(tq PUBLIC mfile)


include(cmake/tests.cmake)

install(TARGETS tq RUNTIME DESTINATION bin)
