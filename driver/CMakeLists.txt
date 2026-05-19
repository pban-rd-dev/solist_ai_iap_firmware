# Driver library CMakeLists.txt
#
# All driver sources are picked up by glob. Unused functions are removed by
# the linker (-ffunction-sections + --gc-sections), so this is safe.

file(GLOB DRIVER_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")

add_library(driver STATIC ${DRIVER_SOURCES})

target_include_directories(driver PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
)

# Link with proprietary libraries if present (Keil-built; may not be GCC-compatible).
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

target_include_directories(driver PUBLIC
    ${CMAKE_SOURCE_DIR}/ml63q25x7/include
    ${CMAKE_SOURCE_DIR}/RTE/Device/ML63Q25x7
    ${CMAKE_SOURCE_DIR}/external/CMSIS/CMSIS/Core/Include
    ${CMAKE_SOURCE_DIR}/src
)

target_compile_definitions(driver PRIVATE
    ${MCU_VARIANT}
)
