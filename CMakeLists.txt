cmake_minimum_required(VERSION 3.16)
project(simple_render C ASM)

set(LINKER_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/linker/stm32f103c8.ld")

set(PLATFORM "linux" CACHE STRING "Target platform: linux or stm")
set_property(CACHE PLATFORM PROPERTY STRINGS "linux" "stm")

set(COMMON_SOURCES
    src/display.c
    src/utils.c
)

set(COMMON_INCLUDES
    include
)

if(PLATFORM STREQUAL "linux")
    list(APPEND COMMON_SOURCES src/platform/linux/linux_render.c)
    list(APPEND COMMON_SOURCES examples/game_of_life.c)
    add_definitions(-DPLATFORM_LINUX)
    find_package(SDL2 REQUIRED)
    add_executable(game_of_life ${COMMON_SOURCES})
    target_link_libraries(game_of_life PRIVATE SDL2::SDL2)
    target_include_directories(game_of_life PRIVATE ${COMMON_INCLUDES})

elseif(PLATFORM STREQUAL "stm")
    list(APPEND COMMON_SOURCES src/platform/stm32/stm32_render.c)
    list(APPEND COMMON_SOURCES examples/game_of_life.c)
    list(APPEND COMMON_SOURCES startup/startup_stm32f103.s)
    add_definitions(-DSTM)
    
    add_executable(game_of_life.elf ${COMMON_SOURCES})
    target_link_options(game_of_life.elf PRIVATE
        -T${LINKER_SCRIPT}
        -Wl,--gc-sections
        -Wl,-Map=game_of_life.map
    )
    target_include_directories(game_of_life.elf PRIVATE ${COMMON_INCLUDES})
    
    add_custom_command(TARGET game_of_life.elf POST_BUILD
        COMMAND arm-none-eabi-objcopy -O binary game_of_life.elf game_of_life.bin
    )
endif()

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)