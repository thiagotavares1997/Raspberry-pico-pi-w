cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(temp_nuc C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(PICO_BOARD pico_w)

pico_sdk_init()

add_executable(temp_nuc main.c)

target_link_libraries(temp_nuc pico_stdlib hardware_adc)
target_link_libraries(temp_nuc hardware_spi)
target_link_libraries(temp_nuc pico_cyw43_arch_none pico_stdlib)

pico_enable_stdio_usb(temp_nuc 1)
pico_enable_stdio_uart(temp_nuc 0)

pico_add_extra_outputs(temp_nuc)
