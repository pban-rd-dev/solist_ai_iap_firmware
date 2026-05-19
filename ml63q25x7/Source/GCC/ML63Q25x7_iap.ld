/******************************************************************************
 * @file     ML63Q25x7_iap.ld
 * @brief    GNU Linker Script for ML63Q25x7 — IAP sample layout
 *
 * Memory map:
 *   FLASH  (0x1003E000, 8 KB)  : IAP main .text, vectors, copy/zero tables,
 *                                ARM.extab/exidx — runtime-remapped to REMAP.
 *   FLASH2 (0x1003C000, 8 KB)  : .iap2 (xmodem, uartf0_i, board helpers),
 *                                .remap_end LMA, .data LMA.
 *   FLASH3 (0x1003FFC0, 64 B)  : .codeoption (HW-level config block).
 *   REMAP  (0x00000000, 256K-64) : runtime-mapped view of FLASH or USER area.
 *   RAM    (0x20000000, 16 KB) : .data VMA, .remap_end VMA, .bss, heap, stack.
 *
 * The user firmware area (0x10000000–0x1003BFFF, 240 KB) is unused by this
 * sample — the IAP writes incoming XMODEM data there.
 ******************************************************************************/

__REMAP_BASE  = 0x00000000;
__REMAP_SIZE  = 0x0003FFC0;

__FLASH_BASE  = 0x1003E000;
__FLASH_SIZE  = 0x00002000;
__FLASH2_BASE = 0x1003C000;
__FLASH2_SIZE = 0x00002000;
__FLASH3_BASE = 0x1003FFC0;
__FLASH3_SIZE = 0x00000040;

__RAM_BASE    = 0x20000000;
__RAM_SIZE    = 0x00004000;

__STACK_SIZE  = 0x00000400;
__HEAP_SIZE   = 0x00000C00;

MEMORY
{
  REMAP  (rx)  : ORIGIN = __REMAP_BASE,  LENGTH = __REMAP_SIZE
  FLASH  (rx)  : ORIGIN = __FLASH_BASE,  LENGTH = __FLASH_SIZE
  FLASH2 (rx)  : ORIGIN = __FLASH2_BASE, LENGTH = __FLASH2_SIZE
  FLASH3 (rx)  : ORIGIN = __FLASH3_BASE, LENGTH = __FLASH3_SIZE
  RAM    (rwx) : ORIGIN = __RAM_BASE,    LENGTH = __RAM_SIZE
}

ENTRY(Reset_Handler)

SECTIONS
{
  /* Large helpers placed below the IAP main so FLASH (only 8 KB) stays for
   * the vectors + control flow. */
  .iap2 :
  {
    *xmodem.c.obj(.text*)
    *xmodem_crc.c.obj(.text*)
    *uartf0_i.c.obj(.text*)
    *smpl_common.c.obj(.text*)
    *smpl_common_led.c.obj(.text*)
  } > FLASH2

  /* Loaded in FLASH2, executed from RAM. Required because the function
   * rewrites REMAPCON, which would otherwise pull the running code out
   * from under its own PC. */
  .remap_end :
  {
    __remap_end_start__ = .;
    *remap_end.c.obj(.text*)
    __remap_end_end__ = .;
    PROVIDE(__remap_end_rom = LOADADDR(.remap_end));
  } > RAM AT > FLASH2

  .codeoption :
  {
    KEEP(*(.codeoption))
  } > FLASH3

  .text :
  {
    KEEP(*(.vectors))
    *(.text*)

    KEEP(*(.init))
    KEEP(*(.fini))

    *crtbegin.o(.ctors)
    *crtbegin?.o(.ctors)
    *(EXCLUDE_FILE(*crtend?.o *crtend.o) .ctors)
    *(SORT(.ctors.*))
    *(.ctors)

    *crtbegin.o(.dtors)
    *crtbegin?.o(.dtors)
    *(EXCLUDE_FILE(*crtend?.o *crtend.o) .dtors)
    *(SORT(.dtors.*))
    *(.dtors)

    *(.rodata*)

    KEEP(*(.eh_frame*))
  } > REMAP AT > FLASH

  .ARM.extab :
  {
    *(.ARM.extab* .gnu.linkonce.armextab.*)
  } > FLASH

  __exidx_start = .;
  .ARM.exidx :
  {
    *(.ARM.exidx* .gnu.linkonce.armexidx.*)
  } > FLASH
  __exidx_end = .;

  .copy.table :
  {
    . = ALIGN(4);
    __copy_table_start__ = .;

    LONG (LOADADDR(.data))
    LONG (__data_start__)
    LONG ((__data_end__ - __data_start__) / 4)

    __copy_table_end__ = .;
  } > FLASH

  .zero.table :
  {
    . = ALIGN(4);
    __zero_table_start__ = .;
    __zero_table_end__ = .;
  } > FLASH

  __etext = LOADADDR(.data);

  .data :
  {
    . = ALIGN(4);
    __data_start__ = .;
    *(vtable)
    *(.data)
    *(.data.*)

    . = ALIGN(4);
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP(*(.preinit_array))
    PROVIDE_HIDDEN (__preinit_array_end = .);

    . = ALIGN(4);
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP(*(SORT(.init_array.*)))
    KEEP(*(.init_array))
    PROVIDE_HIDDEN (__init_array_end = .);

    . = ALIGN(4);
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP(*(SORT(.fini_array.*)))
    KEEP(*(.fini_array))
    PROVIDE_HIDDEN (__fini_array_end = .);

    KEEP(*(.jcr*))
    . = ALIGN(4);
    __data_end__ = .;
  } > RAM AT > FLASH2

  .bss :
  {
    . = ALIGN(4);
    __bss_start__ = .;
    *(.bss)
    *(.bss.*)
    *(COMMON)
    . = ALIGN(4);
    __bss_end__ = .;
  } > RAM AT > RAM

  .heap (COPY) :
  {
    . = ALIGN(8);
    __end__ = .;
    PROVIDE(end = .);
    . = . + __HEAP_SIZE;
    . = ALIGN(8);
    __HeapLimit = .;
  } > RAM

  .stack (ORIGIN(RAM) + LENGTH(RAM) - __STACK_SIZE) (COPY) :
  {
    . = ALIGN(8);
    __StackLimit = .;
    . = . + __STACK_SIZE;
    . = ALIGN(8);
    __StackTop = .;
  } > RAM
  PROVIDE(__stack = __StackTop);

  ASSERT(__StackLimit >= __HeapLimit, "region RAM overflowed with stack")
}
