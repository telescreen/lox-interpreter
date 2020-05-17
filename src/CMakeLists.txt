add_executable(lox main.cpp parser.cpp scanner.cpp token.cpp token_type.cpp)
target_compile_features(lox PUBLIC cxx_std_11)
