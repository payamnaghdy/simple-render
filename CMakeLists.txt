cmake_minimum_required(VERSION 4.3)
project(simple_render)

set(LINKER_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/stm32f103c8.ld")

set(PLATFORM "linux" CACHE STRING "Target platform for the renderer")
set_property(CACHE PLATFORM PROPERTY STRINGS "linux" "stm")

set(SOURCE_FILES main2.c display.c utils.c)

if(PLATFORM STREQUAL "linux")
  list(APPEND SOURCE_FILES linux_render.c)
  add_definitions(-DPLATFORM_LINUX)
  find_package(SDL2 REQUIRED)
elseif(PLATFORM STREQUAL "stm")
    list(APPEND SOURCE_FILES stm32_render.c startup_stm32f103.s) # Add your startup file
    add_executable(${PROJECT_NAME}.elf ${SOURCE_FILES})
    target_link_options(simple_render.elf PRIVATE 
    -T${LINKER_SCRIPT} 
    -Wl,--gc-sections  # Removes unused code
    -Wl,-Map=${PROJECT_NAME}.map
    )
    
    # Custom command to convert ELF to BIN
    add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
        COMMAND arm-none-eabi-objcopy -O binary ${PROJECT_NAME}.elf ${PROJECT_NAME}.bin
    )
endif()



add_executable(${PROJECT_NAME} ${SOURCE_FILES})


if(PLATFORM STREQUAL "linux")
  target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2)
endif()


