/*****************************************************************************
 uart_print.h

 UART Debug Interface for ML63Q2537
 - UART0 on pins TX:P33, RX:P32
 - Provides simple debug printing functionality
 - Non-blocking operation with optional buffering

******************************************************************************/

#ifndef UART_PRINT_H__
#define UART_PRINT_H__

#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>

/*############################################################################*/
/*#                                 Defines                                  #*/
/*############################################################################*/

/* UART Configuration */
#define UART_PRINT_BAUD_RATE    115200  /* Debug UART baud rate */
#define UART_PRINT_TX_PIN       33      /* P33 for TX */
#define UART_PRINT_RX_PIN       32      /* P32 for RX */

/* Debug levels */
#define UART_PRINT_LEVEL_NONE        0
#define UART_PRINT_LEVEL_ERROR       1
#define UART_PRINT_LEVEL_WARNING     2
#define UART_PRINT_LEVEL_INFO        3
#define UART_PRINT_LEVEL_DEBUG       4
#define UART_PRINT_LEVEL_VERBOSE     5

/* Default debug level */
#ifndef UART_PRINT_LEVEL
#define UART_PRINT_LEVEL             UART_PRINT_LEVEL_DEBUG
#endif

/*############################################################################*/
/*#                                  API                                     #*/
/*############################################################################*/

/**
 * Initialize debug UART
 * Configures UART0 with specified baud rate on P32/P33
 *
 * @param       -
 * @return      true if successful, false otherwise
 */
bool uart_print_init(void);

/**
 * Deinitialize debug UART
 *
 * @param       -
 * @return      -
 */
void uart_print_deinit(void);

/**
 * Send a single character
 *
 * @param[in]   c       Character to send
 * @return      -
 */
void uart_print_putc(char c);

/**
 * Send a string
 *
 * @param[in]   str     Null-terminated string to send
 * @return      -
 */
void uart_print_puts(const char *str);

/**
 * Send formatted string (printf-like)
 *
 * @param[in]   format  Format string
 * @param[in]   ...     Variable arguments
 * @return      Number of characters sent
 */
int uart_print_printf(const char *format, ...);

/**
 * Send hex dump of data
 *
 * @param[in]   data    Pointer to data buffer
 * @param[in]   len     Length of data
 * @param[in]   prefix  Optional prefix string
 * @return      -
 */
void uart_print_hex_dump(const uint8_t *data, uint32_t len, const char *prefix);

/**
 * Check if UART is ready for transmission
 *
 * @param       -
 * @return      true if ready, false if busy
 */
bool uart_print_is_ready(void);

/**
 * Flush UART transmit buffer
 *
 * @param       -
 * @return      -
 */
void uart_print_flush(void);


/**
 * Process interrupts
 *
 * @parm        -
 * @return      -
 */
void uart_procUartfInt( void );

/*############################################################################*/
/*#                              Debug Macros                                #*/
/*############################################################################*/

/* Debug print macros with level control */
#if UART_PRINT_LEVEL >= UART_PRINT_LEVEL_ERROR
    #define UART_PRINT_ERROR(fmt, ...)   uart_print_printf("[ERROR] " fmt "\r\n", ##__VA_ARGS__)
#else
    #define UART_PRINT_ERROR(fmt, ...)   ((void)0)
#endif

#if UART_PRINT_LEVEL >= UART_PRINT_LEVEL_WARNING
    #define UART_PRINT_WARN(fmt, ...)    uart_print_printf("[WARN]  " fmt "\r\n", ##__VA_ARGS__)
#else
    #define UART_PRINT_WARN(fmt, ...)    ((void)0)
#endif

#if UART_PRINT_LEVEL >= UART_PRINT_LEVEL_INFO
    #define UART_PRINT_INFO(fmt, ...)    uart_print_printf("[INFO]  " fmt "\r\n", ##__VA_ARGS__)
#else
    #define UART_PRINT_INFO(fmt, ...)    ((void)0)
#endif

#if UART_PRINT_LEVEL >= UART_PRINT_LEVEL_DEBUG
    #define UART_PRINT_DEBUG(fmt, ...)   uart_print_printf("[DEBUG] " fmt "\r\n", ##__VA_ARGS__)
#else
    #define UART_PRINT_DEBUG(fmt, ...)   ((void)0)
#endif

#if UART_PRINT_LEVEL >= UART_PRINT_LEVEL_VERBOSE
    #define UART_PRINT_VERBOSE(fmt, ...) uart_print_printf("[VERB]  " fmt "\r\n", ##__VA_ARGS__)
#else
    #define UART_PRINT_VERBOSE(fmt, ...) ((void)0)
#endif

/* Hex dump macro */
#define UART_PRINT_HEX_DUMP(data, len, prefix) \
    do { \
        if (UART_PRINT_LEVEL >= UART_PRINT_LEVEL_INFO) { \
            uart_print_hex_dump((data), (len), (prefix)); \
        } \
    } while(0)

/* Assert macro with debug output */
#define UART_PRINT_ASSERT(condition) \
    do { \
        if (!(condition)) { \
            UART_PRINT_ERROR("ASSERT FAILED: %s at %s:%d", #condition, __FILE__, __LINE__); \
            while(1); \
        } \
    } while(0)

#endif /* UART_PRINT_H__ */
