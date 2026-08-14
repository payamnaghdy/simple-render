# Minimal Graphics engine for STM32 

This is a bare metal graphics engine for stm32, you can use it to interact with displays that support SPI 
test environment is on linux using SDL2 lib.

# Build stm (production)
cmake -B build-stm -DPLATFORM=stm -DCMAKE_TOOLCHAIN_FILE=arm-none-eabi.cmake
cmake --build build-stm

# Build linux (test environment)
cmake -B build -DPLATFORM=linux   
