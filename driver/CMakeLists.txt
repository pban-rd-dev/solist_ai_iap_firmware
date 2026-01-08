# Driver library CMakeLists.txt

# Collect driver source files
set(DRIVER_SOURCES
    src/irq.c
    src/ssiof0.c
    src/wdt.c
    src/uartf0.c
    # Add other driver sources as needed
    # Note: Some sources might be optional depending on peripheral usage
    ../src/debug_uart.c
)

# Create driver library
add_library(driver STATIC ${DRIVER_SOURCES})

# Include directories
target_include_directories(driver PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
)

# Link with proprietary libraries if they exist and are compatible with GCC
# Note: The .lib files in lib/ directory are Keil-specific and may not work with GCC
# You may need to rebuild these libraries or find GCC-compatible versions
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/lib/solistAi_library.a")
    target_link_libraries(driver PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/solistAi_library.a
    )
endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/lib/solistAi_Scope.a")
    target_link_libraries(driver PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/solistAi_Scope.a
    )
endif()

# Add ML63Q25x7 include path
target_include_directories(driver PUBLIC
    ${CMAKE_SOURCE_DIR}/ml63q25x7/include
    ${CMAKE_SOURCE_DIR}/RTE/Device/ML63Q25x7
    ${CMAKE_SOURCE_DIR}/external/CMSIS/CMSIS/Core/Include
    ${CMAKE_SOURCE_DIR}/src
)

# Driver-specific compile definitions
target_compile_definitions(driver PRIVATE
    ${MCU_VARIANT}
)
