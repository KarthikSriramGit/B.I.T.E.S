cmake_minimum_required(VERSION 3.13)

project(BITS VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Teensy 4.1 specific settings
set(MCU "IMXRT1062")
set(F_CPU "600000000")
set(USB_TYPE "USB_SERIAL")

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/lib
)

# Source files
file(GLOB_RECURSE SOURCES
    "src/*.cpp"
    "src/*.c"
)

# Headers
file(GLOB_RECURSE HEADERS
    "src/*.h"
    "include/*.h"
)

# Add executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Compiler flags
target_compile_options(${PROJECT_NAME} PRIVATE
    -Wall
    -Wextra
    -O2
    -mthumb
    -mcpu=cortex-m7
    -mfloat-abi=hard
    -mfpu=fpv5-d16
)

# Linker flags
target_link_options(${PROJECT_NAME} PRIVATE
    -mthumb
    -mcpu=cortex-m7
    -mfloat-abi=hard
    -mfpu=fpv5-d16
    -T${CMAKE_SOURCE_DIR}/linker.ld
)

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    # Add library dependencies here
)

# Install rules
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
