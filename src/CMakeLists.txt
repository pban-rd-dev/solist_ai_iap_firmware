# Application source files

set(APP_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/main.c
  ${CMAKE_CURRENT_SOURCE_DIR}/syscalls.c
  ${CMAKE_CURRENT_SOURCE_DIR}/app_irq.c
  ${CMAKE_CURRENT_SOURCE_DIR}/device.c
  PARENT_SCOPE
)
