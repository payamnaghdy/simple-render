# Minimal Graphics Engine for STM32

Bare metal graphics engine for STM32 with SPI display support. Linux test environment uses SDL2.

## Build STM32 (Production)
```bash
cmake -B build-stm -DPLATFORM=stm -DCMAKE_TOOLCHAIN_FILE=arm-none-eabi.cmake
cmake --build build-stm
```
Output: `build-stm/simple_render.bin` (flashable binary)

## Build Linux (Test Environment)
```bash
cmake -B build -DPLATFORM=linux
cmake --build build
```
Output: `build/simple_render` (SDL2 executable)

## Requirements
- **STM32**: ARM GCC toolchain (`arm-none-eabi-gcc`)
- **Linux**: SDL2 development libraries