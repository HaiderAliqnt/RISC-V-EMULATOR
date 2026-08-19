cmake_minimum_required(VERSION 3.10)
project(riscv_emulator C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_PREFIX_PATH "C:/msys64/ucrt64")

add_compile_options(-Wall -Wextra)

find_package(SDL2 REQUIRED)
find_package(GTest REQUIRED)

# main emulator
add_executable(emulator
    src/main.c
    src/memory.c
    src/cpu.c
    src/decoder.c
    src/executor.c
    src/elf-loader.c
)
target_include_directories(emulator PRIVATE include)
target_link_libraries(emulator SDL2::SDL2)
target_compile_definitions(emulator PRIVATE SDL_MAIN_HANDLED)

# test_memory
add_executable(test_memory
    tests/test_memory.cpp
    src/memory.c
)
target_include_directories(test_memory PRIVATE include)
target_link_libraries(test_memory GTest::GTest GTest::Main SDL2::SDL2)
target_compile_definitions(test_memory PRIVATE SDL_MAIN_HANDLED)

# test_decoder
add_executable(test_decoder
    tests/test_decoder.cpp
    src/decoder.c
)
target_include_directories(test_decoder PRIVATE include)
target_link_libraries(test_decoder GTest::GTest GTest::Main)

# test_executor
add_executable(test_executor
    tests/test_executor.cpp
    src/memory.c
    src/cpu.c
    src/decoder.c
    src/executor.c
)
target_include_directories(test_executor PRIVATE include)
target_link_libraries(test_executor GTest::GTest GTest::Main SDL2::SDL2)
target_compile_definitions(test_executor PRIVATE SDL_MAIN_HANDLED)
