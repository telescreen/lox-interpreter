FetchContent_Declare(
  linenoise
  GIT_REPOSITORY https://github.com/antirez/linenoise.git
  GIT_TAG master
 )
FetchContent_MakeAvailable(linenoise)

if(linenoise_POPULATED)
  message(STATUS, "linenoise source dir: ${linenoise_SOURCE_DIR}")
  message(STATUS, "linenoise binary dir: ${linenoise_BINARY_DIR}")
endif(linenoise_POPULATED)


add_executable(lox
  main.cpp
  parser.cpp
  scanner.cpp
  token.cpp
  token_type.cpp
  ${linenoise_SOURCE_DIR}/linenoise.c)

target_compile_features(lox PUBLIC cxx_std_11)
target_include_directories(lox PRIVATE ${linenoise_SOURCE_DIR})
