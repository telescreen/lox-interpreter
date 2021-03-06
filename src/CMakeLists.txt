include(FetchContent)
FetchContent_Declare(
  linenoise
  GIT_REPOSITORY https://github.com/antirez/linenoise.git
  GIT_TAG master
 )
FetchContent_MakeAvailable(linenoise)

#########################
# Fetch library         #
#########################
include(FetchContent)
FetchContent_Declare(
  fmtlib
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG        5.3.0
)
FetchContent_MakeAvailable(fmtlib)

#add_compile_options(-O3 -pthread)
add_compile_options(-Wall -pedantic -fstandalone-debug)
add_link_options(-lc++abi)


add_executable(lox
  main.cpp
  parser.cpp
  scanner.cpp
  token.cpp
  token_type.cpp
  value.cpp
  interpreter.cpp
  lox_function.cpp
  environment.cpp
  ${linenoise_SOURCE_DIR}/linenoise.c)

target_compile_features(lox PUBLIC cxx_std_11)
target_include_directories(lox PRIVATE ${linenoise_SOURCE_DIR})
target_link_libraries(lox PRIVATE fmt::fmt-header-only)
