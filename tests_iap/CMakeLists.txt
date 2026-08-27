# tests_iap CMakeLists.txt
#
# Builds a separate on-target test binary that exercises pieces of the IAP
# implementation (currently: xmodem_crc). Uses the flat linker layout — it does
# NOT need the split FLASH/FLASH2 layout that the IAP main image uses.

set(TEST_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/test_main.c
    ${CMAKE_CURRENT_SOURCE_DIR}/test_io.c
    ${CMAKE_CURRENT_SOURCE_DIR}/test_xmodem_crc.c
)

# Application support files needed by the test binary.
set(TEST_APP_SOURCES
    ${CMAKE_SOURCE_DIR}/src/syscalls.c
    ${CMAKE_SOURCE_DIR}/src/device.c
    ${CMAKE_SOURCE_DIR}/src/uartf0_i.c
    ${CMAKE_SOURCE_DIR}/src/xmodem_crc.c
    ${CMAKE_SOURCE_DIR}/src/codeoption.c
)

set(RTE_SOURCES
    ${CMAKE_SOURCE_DIR}/ml63q25x7/Source/startup_ML63Q25x7.c
    ${CMAKE_SOURCE_DIR}/ml63q25x7/Source/system_ML63Q25x7.c
)

set(TEST_TARGET_NAME ${PROJECT_NAME}_test)
add_executable(${TEST_TARGET_NAME}
    ${TEST_SOURCES}
    ${TEST_APP_SOURCES}
    ${RTE_SOURCES}
)

target_include_directories(${TEST_TARGET_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/ml63q25x7/include
)

target_link_libraries(${TEST_TARGET_NAME} PRIVATE
    driver
    utility
)

# Use the flat linker layout — the test binary is not the IAP firmware itself.
set(LINKER_SCRIPT "${CMAKE_SOURCE_DIR}/ml63q25x7/Source/GCC/ML63Q25x7_gcc.ld")
if(EXISTS ${LINKER_SCRIPT})
    target_link_options(${TEST_TARGET_NAME} PRIVATE "-T${LINKER_SCRIPT}")
    set_target_properties(${TEST_TARGET_NAME} PROPERTIES LINK_DEPENDS ${LINKER_SCRIPT})
else()
    message(FATAL_ERROR "GCC linker script not found: ${LINKER_SCRIPT}")
endif()

target_compile_definitions(${TEST_TARGET_NAME} PRIVATE
    ${MCU_VARIANT}
    ML63Q25x7
)

add_custom_command(TARGET ${TEST_TARGET_NAME} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex    $<TARGET_FILE:${TEST_TARGET_NAME}> ${TEST_TARGET_NAME}.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary  $<TARGET_FILE:${TEST_TARGET_NAME}> ${TEST_TARGET_NAME}.bin
    COMMAND ${CMAKE_SIZE}     --format=berkeley $<TARGET_FILE:${TEST_TARGET_NAME}>
    COMMENT "Building ${TEST_TARGET_NAME}.hex / .bin"
)
