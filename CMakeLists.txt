cmake_minimum_required(VERSION 3.1...3.15)

if (${CMAKE_VERSION} VERSION_LESS 3.12)
  cmake_policy(VERSION ${CMAKE_MAJOR_VERSION} ${CMAKE_MINOR_VERSION})
endif()

project(lox VERSION 1.0
  DESCRIPTION "Lox Intepreter"
  LANGUAGES CXX)

add_executable(lox main.cpp token.cpp scanner.cpp token_type.cpp parser.cpp)
target_compile_features(lox PUBLIC cxx_std_11)
