# ARM GCC Toolchain file for Cortex-M0+

# Set the target system
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

# Find the ARM toolchain
find_program(ARM_GCC_COMPILER arm-none-eabi-gcc)
find_program(ARM_GXX_COMPILER arm-none-eabi-g++)
find_program(ARM_OBJCOPY arm-none-eabi-objcopy)
find_program(ARM_SIZE arm-none-eabi-size)
find_program(ARM_OBJDUMP arm-none-eabi-objdump)

# Set the compilers
set(CMAKE_C_COMPILER ${ARM_GCC_COMPILER})
set(CMAKE_CXX_COMPILER ${ARM_GXX_COMPILER})
set(CMAKE_ASM_COMPILER ${ARM_GCC_COMPILER})

# Set the objcopy and size tools
set(CMAKE_OBJCOPY ${ARM_OBJCOPY})
set(CMAKE_SIZE ${ARM_SIZE})

# Disable compiler checks
set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)

# Search for programs in the build host directories
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# For libraries and headers in the target directories
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)