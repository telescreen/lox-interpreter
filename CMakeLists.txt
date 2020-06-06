cmake_minimum_required(VERSION 3.16)

if (${CMAKE_VERSION} VERSION_LESS 3.16)
  cmake_policy(VERSION ${CMAKE_MAJOR_VERSION} ${CMAKE_MINOR_VERSION})
endif()

project(Lox
  VERSION 1.0
  DESCRIPTION "Lox Intepreter"
  LANGUAGES C CXX)

# Only do these if this is the main project, and not if it is included through add_subdirectory
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)

    # Optionally set things like CMAKE_CXX_STANDARD, CMAKE_POSITION_INDEPENDENT_CODE here

    # Let's ensure -std=c++xx instead of -std=g++xx
    set(CMAKE_CXX_EXTENSIONS OFF)

    # Let's nicely support folders in IDE's
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)

endif()

option(BUILD_EXAMPLE "Build examples (Including programs that are unrelated to lox interpreter)" OFF)
if ("${BUILD_EXAMPLE}")
    add_subdirectory(example)
  endif()


include(FetchContent)
#########################
# Fetch library         #
#########################

# Formatting library
FetchContent_Declare(
  fmtlib
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG        5.3.0
)
FetchContent_MakeAvailable(fmtlib)


add_subdirectory(src)
