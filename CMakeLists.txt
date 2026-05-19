cmake_minimum_required(VERSION 3.16)

# Set up the ARM toolchain before project declaration
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

if(NOT CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/cmake/arm-none-eabi-toolchain.cmake")
endif()

project(solist_ai_template_iap
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

set(COMMON_FLAGS "${CPU_FLAGS} -ffunction-sections -fdata-sections -Wall -Wextra")
set(CMAKE_C_FLAGS "${COMMON_FLAGS}")
set(CMAKE_C_FLAGS_DEBUG "-O0 -g3 -DDEBUG")
set(CMAKE_C_FLAGS_RELEASE "-Os -DNDEBUG")

set(CMAKE_EXE_LINKER_FLAGS "${CPU_FLAGS} -specs=nano.specs -Wl,--gc-sections")

# Build options
option(BUILD_IAP_TESTS "Build IAP-specific test binary (tests_iap/)" OFF)

add_subdirectory(driver)
add_subdirectory(utility)
add_subdirectory(src)
if(BUILD_IAP_TESTS)
  add_subdirectory(tests_iap)
endif()

set(RTE_SOURCES
  ml63q25x7/Source/startup_ML63Q25x7.c
  ml63q25x7/Source/system_ML63Q25x7.c
)

add_executable(${PROJECT_NAME} ${APP_SOURCES} ${RTE_SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}/ml63q25x7/include
)

target_link_libraries(${PROJECT_NAME} PRIVATE
  driver
  utility
)

# IAP-specific linker script (FLASH/FLASH2/FLASH3/REMAP layout).
set(LINKER_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/ml63q25x7/Source/GCC/ML63Q25x7_iap.ld")
if(EXISTS ${LINKER_SCRIPT})
  set_target_properties(${PROJECT_NAME} PROPERTIES
    LINK_FLAGS "-T${LINKER_SCRIPT}"
  )
else()
  message(FATAL_ERROR "IAP linker script not found: ${LINKER_SCRIPT}")
endif()

target_compile_definitions(${PROJECT_NAME} PRIVATE
  ${MCU_VARIANT}
  ML63Q25x7
)

# Whole-image hex (parity with master; useful for inspection but the IAP is
# actually programmed via the three split bins below).
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${PROJECT_NAME}> ${PROJECT_NAME}.hex
  COMMENT "Building ${PROJECT_NAME}.hex"
)

# Three split bins matching the FLASH / FLASH2 / FLASH3 regions. Each gets
# programmed to a distinct base address by scripts/iap_flash.py.
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_OBJCOPY} -O binary
          --only-section=.text
          --only-section=.ARM.exidx
          --only-section=.copy.table
          --only-section=.zero.table
          $<TARGET_FILE:${PROJECT_NAME}> iap_code.bin
  COMMAND ${CMAKE_OBJCOPY} -O binary
          --only-section=.iap2
          --only-section=.remap_end
          --only-section=.data
          $<TARGET_FILE:${PROJECT_NAME}> iap_data.bin
  COMMAND ${CMAKE_OBJCOPY} -O binary
          --only-section=.codeoption
          $<TARGET_FILE:${PROJECT_NAME}> iap_codeoption.bin
  COMMENT "Splitting IAP image: iap_code.bin (0x1003E000) / iap_data.bin (0x1003C000) / iap_codeoption.bin (0x1003FFC0)"
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_SIZE} --format=berkeley $<TARGET_FILE:${PROJECT_NAME}>
  COMMENT "Size information:"
)
