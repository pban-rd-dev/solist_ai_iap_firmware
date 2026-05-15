cmake_minimum_required(VERSION 3.16)

# Set up the ARM toolchain before project declaration
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

# Set the toolchain file if not already set
if(NOT CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/cmake/arm-none-eabi-toolchain.cmake")
endif()

# Project definition
project(solist_ai_template
  VERSION 1.0.0
  LANGUAGES C ASM
)

# Target MCU specifications
set(MCU_FAMILY ML63Q25x7)
set(MCU_VARIANT ML63Q2537)
set(CPU_FLAGS "-mcpu=cortex-m0plus -mthumb")

# Global compile options
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Common compiler flags
set(COMMON_FLAGS "${CPU_FLAGS} -ffunction-sections -fdata-sections -Wall -Wextra")
set(CMAKE_C_FLAGS "${COMMON_FLAGS}")
set(CMAKE_C_FLAGS_DEBUG "-O0 -g3 -DDEBUG")
set(CMAKE_C_FLAGS_RELEASE "-Os -DNDEBUG")

# Linker flags
set(CMAKE_EXE_LINKER_FLAGS "${CPU_FLAGS} -specs=nano.specs -Wl,--gc-sections")

# Build options
option(BUILD_TESTS "Build test binary" OFF)

# Add subdirectories
add_subdirectory(driver)
add_subdirectory(utility)
add_subdirectory(src)
if(BUILD_TESTS)
  add_subdirectory(tests)
endif()

# Add RTE (Runtime Environment) sources
set(RTE_SOURCES
  ml63q25x7/Source/startup_ML63Q25x7.c
  ml63q25x7/Source/system_ML63Q25x7.c
)

# Create executable
add_executable(${PROJECT_NAME} ${APP_SOURCES} ${RTE_SOURCES})

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/src
  #${CMAKE_CURRENT_SOURCE_DIR}/RTE/Device/ML63Q25x7
  ${CMAKE_CURRENT_SOURCE_DIR}/ml63q25x7/include
)

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
  driver
  utility
)

# Set the linker script
set(LINKER_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/ml63q25x7/Source/GCC/ML63Q25x7_gcc.ld")
if(EXISTS ${LINKER_SCRIPT})
  set_target_properties(${PROJECT_NAME} PROPERTIES
    LINK_FLAGS "-T${LINKER_SCRIPT}"
  )
else()
  message(FATAL_ERROR "GCC linker script not found: ${LINKER_SCRIPT}")
endif()

# Add target-specific definitions
target_compile_definitions(${PROJECT_NAME} PRIVATE
  ${MCU_VARIANT}
  ML63Q25x7
)

# Generate hex file
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${PROJECT_NAME}> ${PROJECT_NAME}.hex
  COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${PROJECT_NAME}>
  COMMENT "Building ${PROJECT_NAME}.hex"
)

# Generate binary file
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${PROJECT_NAME}> ${PROJECT_NAME}.bin
  COMMENT "Building ${PROJECT_NAME}.bin"
)

# Print size information
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_SIZE} --format=berkeley $<TARGET_FILE:${PROJECT_NAME}>
  COMMENT "Size information:"
)
