# Application source files for the IAP firmware

set(APP_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/main.c
  ${CMAKE_CURRENT_SOURCE_DIR}/syscalls.c
  ${CMAKE_CURRENT_SOURCE_DIR}/app_irq.c
  ${CMAKE_CURRENT_SOURCE_DIR}/device.c
  ${CMAKE_CURRENT_SOURCE_DIR}/codeoption.c
  ${CMAKE_CURRENT_SOURCE_DIR}/uartf0_i.c
  ${CMAKE_CURRENT_SOURCE_DIR}/xmodem.c
  ${CMAKE_CURRENT_SOURCE_DIR}/xmodem_crc.c
  ${CMAKE_CURRENT_SOURCE_DIR}/remap_end.c
  ${CMAKE_CURRENT_SOURCE_DIR}/debug_log.c
  PARENT_SCOPE
)
