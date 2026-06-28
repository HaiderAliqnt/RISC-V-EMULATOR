cmake_minimum_required(VERSION 3.10)
project(riscv_emulator C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra)

add_executable(emulator
    src/main.c
    src/memory.c
)

target_include_directories(emulator PRIVATE include)
