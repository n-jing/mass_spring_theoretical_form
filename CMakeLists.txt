cmake_minimum_required(VERSION 3.10)
project(mass_spring)

# enable_language(Fortran)

find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})

# add_library(gh STATIC ms_gradient_and_hessian.f90)
# file(GLOB src *.cpp *.c)
# add_library(allsrc STATIC ${src})
# add_executable(MAIN main.cpp)
# target_link_libraries(MAIN allsrc gh)
add_executable(generate_maxima_script generate_maxima_script.cpp)


set(EXECUTABLE_OUTPUT_PATH "bin")
