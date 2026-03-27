cmake_minimum_required(VERSION 3.10)

project(SimuladorMRUA VERSION 1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

find_library(MPFR_LIBRARY mpfr REQUIRED)
find_library(GMP_LIBRARY gmp REQUIRED)

add_executable(simulador simulador.c)

target_link_libraries(simulador PRIVATE ${MPFR_LIBRARY} ${GMP_LIBRARY})