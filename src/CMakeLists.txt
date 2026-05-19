# Application source files

set(APP_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/main.c
  ${CMAKE_CURRENT_SOURCE_DIR}/syscalls.c
  ${CMAKE_CURRENT_SOURCE_DIR}/app_irq.c
  ${CMAKE_CURRENT_SOURCE_DIR}/device.c
  ${CMAKE_CURRENT_SOURCE_DIR}/uart_print.c
  ${CMAKE_CURRENT_SOURCE_DIR}/codeoption.c
  PARENT_SCOPE
)
