cmake_minimum_required(VERSION 3.15)
project(finite-automaton)

set(CMAKE_CXX_STANDARD 17)

add_library(finite-automaton-library STATIC
        DFA.h DFA_functions.cpp DFA_constructors.cpp DFA_operators.cpp ValmariAlgorithm.cpp)

target_link_libraries(finite-automaton-library fin-auto-lib)
