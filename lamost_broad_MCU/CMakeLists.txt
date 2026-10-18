cmake_minimum_required(VERSION 3.22)

#
# This file is generated only once,
# and is not re-generated if converter is called multiple times.
#
# User is free to modify the file as much as necessary
#

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)


# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Set the project name
set(CMAKE_PROJECT_NAME lamost_broad_MCU)

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Core project settings
project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

# Enable CMake support for ASM and C languages
enable_language(C ASM)

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME})

# 板号默认沿用 PCB1；烧录其他板前显式指定 -DPCB_ID=N
set(PCB_ID "1" CACHE STRING "PCB board number (1-52)")
if(NOT PCB_ID MATCHES "^[1-9][0-9]*$" OR PCB_ID GREATER 52)
    message(FATAL_ERROR "PCB_ID must be an integer between 1 and 52")
endif()
option(CAN_COMMAND_EXAMPLE_ENABLE "Enable bench-only example LED command protocol" OFF)
message(STATUS "PCB_ID=${PCB_ID}; CAN example protocol=${CAN_COMMAND_EXAMPLE_ENABLE}")

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)

# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined library search paths
)

# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user sources here
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/pcb_broad/just_led.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/pcb_broad/can_rx_buffer.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/pcb_broad/can_command.c
)

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined include paths
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/pcb_broad
)

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined symbols
    PCB_ID=${PCB_ID}
    CAN_COMMAND_EXAMPLE_ENABLE=$<BOOL:${CAN_COMMAND_EXAMPLE_ENABLE}>
)

# Remove wrong libob.a library dependency when using cpp files
list(REMOVE_ITEM CMAKE_C_IMPLICIT_LINK_LIBRARIES ob)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    stm32cubemx

    # Add user defined libraries
)


message("Project ${CMAKE_CURRENT_SOURCE_DIR} setup complete.")
