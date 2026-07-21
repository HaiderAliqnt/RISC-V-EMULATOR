cmake_minimum_required(VERSION 3.10)
project(riscv_emulator C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra)

add_executable(emulator
    src/main.c
    src/memory.c
    src/cpu.c
    src/decoder.c
)
add_executable(test_decoder
    tests/test_decoder_functions.c
    src/decoder.c
)
add_executable(test_memory
    tests/test_memory_functions.c
    src/memory.c
)

target_include_directories(test_decoder PRIVATE include)
target_include_directories(test_memory PRIVATE include)
target_include_directories(emulator PRIVATE include)
