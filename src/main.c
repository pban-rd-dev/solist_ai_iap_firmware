/*****************************************************************************
 * @file     main.c
 * @brief    Hello World template for ML63Q2537 Solist-AI
 * @version  1.0
 *****************************************************************************/

#include <stdint.h>
#include "ML63Q25x7.h"

#include "wdt.h"

#include "device.h"

int main(void)
{
  if (device_initialize() != 0) {
    return -1;
  }

  while (1) {
    wdt_clear();
  }
  return 0;
}
