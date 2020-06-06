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


add_executable(lox
  main.cpp
  parser.cpp
  scanner.cpp
  token.cpp
  token_type.cpp
  ${linenoise_SOURCE_DIR}/linenoise.c)

target_compile_features(lox PUBLIC cxx_std_11)
target_include_directories(lox PRIVATE ${linenoise_SOURCE_DIR})
target_link_libraries(lox PRIVATE fmt::fmt-header-only)
