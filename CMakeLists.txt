cmake_minimum_required(VERSION 3.21)
project(cpp_training_new)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS -pthread)

add_executable(cpp_training_new main.cpp my_vector.h my_std_vector.h filehandler.h concurrency.h)
