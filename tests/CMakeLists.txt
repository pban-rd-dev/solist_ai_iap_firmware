# Test binary CMakeLists.txt

# Test framework and test suite files
set(TEST_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/test_main.c
    ${CMAKE_CURRENT_SOURCE_DIR}/test_framework.c
    ${CMAKE_CURRENT_SOURCE_DIR}/test_device.c
    # Add more test suite files here:
    # ${CMAKE_CURRENT_SOURCE_DIR}/test_uart.c
)

# Application support files needed by tests
set(TEST_APP_SOURCES
    ${CMAKE_SOURCE_DIR}/src/syscalls.c
    ${CMAKE_SOURCE_DIR}/src/app_irq.c
    ${CMAKE_SOURCE_DIR}/src/device.c
)

# Combine all test sources
set(ALL_TEST_SOURCES ${TEST_SOURCES} ${TEST_APP_SOURCES})

# RTE (Runtime Environment) sources
set(RTE_SOURCES
    ${CMAKE_SOURCE_DIR}/ml63q25x7/Source/startup_ML63Q25x7.c
    ${CMAKE_SOURCE_DIR}/ml63q25x7/Source/system_ML63Q25x7.c
)

# Create test executable
set(TEST_TARGET_NAME ${PROJECT_NAME}_test)
add_executable(${TEST_TARGET_NAME} ${ALL_TEST_SOURCES} ${RTE_SOURCES})

# Include directories
target_include_directories(${TEST_TARGET_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/ml63q25x7/include
)

# Link libraries
target_link_libraries(${TEST_TARGET_NAME} PRIVATE
    driver
    utility
)

# Set the linker script
set(LINKER_SCRIPT "${CMAKE_SOURCE_DIR}/ml63q25x7/Source/GCC/ML63Q25x7_gcc.ld")
if(EXISTS ${LINKER_SCRIPT})
    set_target_properties(${TEST_TARGET_NAME} PROPERTIES
        LINK_FLAGS "-T${LINKER_SCRIPT}"
    )
else()
    message(FATAL_ERROR "GCC linker script not found: ${LINKER_SCRIPT}")
endif()

# Add target-specific definitions
target_compile_definitions(${TEST_TARGET_NAME} PRIVATE
    ${MCU_VARIANT}
    ML63Q25x7
)

# Generate hex file
add_custom_command(TARGET ${TEST_TARGET_NAME} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${TEST_TARGET_NAME}> ${TEST_TARGET_NAME}.hex
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${TEST_TARGET_NAME}>
    COMMENT "Building ${TEST_TARGET_NAME}.hex"
)

# Generate binary file
add_custom_command(TARGET ${TEST_TARGET_NAME} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${TEST_TARGET_NAME}> ${TEST_TARGET_NAME}.bin
    COMMENT "Building ${TEST_TARGET_NAME}.bin"
)

# Print size information
add_custom_command(TARGET ${TEST_TARGET_NAME} POST_BUILD
    COMMAND ${CMAKE_SIZE} --format=berkeley $<TARGET_FILE:${TEST_TARGET_NAME}>
    COMMENT "Test binary size information:"
)
