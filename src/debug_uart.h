/*****************************************************************************
 debug_uart.h

 UART Debug Interface for ML63Q2537
 - UART0 on pins TX:P33, RX:P32
 - Provides simple debug printing functionality
 - Non-blocking operation with optional buffering

******************************************************************************/

#ifndef DEBUG_UART_H__
#define DEBUG_UART_H__

#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>

/*############################################################################*/
/*#                                 Defines                                  #*/
/*############################################################################*/

/* UART Configuration */
#define DEBUG_UART_BAUD_RATE    115200  /* Debug UART baud rate */
#define DEBUG_UART_TX_PIN       33      /* P33 for TX */
#define DEBUG_UART_RX_PIN       32      /* P32 for RX */

/* Debug levels */
#define DEBUG_LEVEL_NONE        0
#define DEBUG_LEVEL_ERROR       1
#define DEBUG_LEVEL_WARNING     2
#define DEBUG_LEVEL_INFO        3
#define DEBUG_LEVEL_DEBUG       4
#define DEBUG_LEVEL_VERBOSE     5

/* Default debug level */
#ifndef DEBUG_LEVEL
#define DEBUG_LEVEL             DEBUG_LEVEL_DEBUG
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
bool debug_uart_init(void);

/**
 * Deinitialize debug UART
 *
 * @param       -
 * @return      -
 */
void debug_uart_deinit(void);

/**
 * Send a single character
 *
 * @param[in]   c       Character to send
 * @return      -
 */
void debug_uart_putc(char c);

/**
 * Send a string
 *
 * @param[in]   str     Null-terminated string to send
 * @return      -
 */
void debug_uart_puts(const char *str);

/**
 * Send formatted string (printf-like)
 *
 * @param[in]   format  Format string
 * @param[in]   ...     Variable arguments
 * @return      Number of characters sent
 */
int debug_uart_printf(const char *format, ...);

/**
 * Send hex dump of data
 *
 * @param[in]   data    Pointer to data buffer
 * @param[in]   len     Length of data
 * @param[in]   prefix  Optional prefix string
 * @return      -
 */
void debug_uart_hex_dump(const uint8_t *data, uint32_t len, const char *prefix);

/**
 * Check if UART is ready for transmission
 *
 * @param       -
 * @return      true if ready, false if busy
 */
bool debug_uart_is_ready(void);

/**
 * Flush UART transmit buffer
 *
 * @param       -
 * @return      -
 */
void debug_uart_flush(void);


/**
 * Process interrupts
 *
 * @parm        -
 * @return      -
 */
void debug_procUartfInt( void );

/*############################################################################*/
/*#                              Debug Macros                                #*/
/*############################################################################*/

/* Debug print macros with level control */
#if DEBUG_LEVEL >= DEBUG_LEVEL_ERROR
    #define DEBUG_ERROR(fmt, ...)   debug_uart_printf("[ERROR] " fmt "\r\n", ##__VA_ARGS__)
#else
    #define DEBUG_ERROR(fmt, ...)   ((void)0)
#endif

#if DEBUG_LEVEL >= DEBUG_LEVEL_WARNING
    #define DEBUG_WARN(fmt, ...)    debug_uart_printf("[WARN]  " fmt "\r\n", ##__VA_ARGS__)
#else
    #define DEBUG_WARN(fmt, ...)    ((void)0)
#endif

#if DEBUG_LEVEL >= DEBUG_LEVEL_INFO
    #define DEBUG_INFO(fmt, ...)    debug_uart_printf("[INFO]  " fmt "\r\n", ##__VA_ARGS__)
#else
    #define DEBUG_INFO(fmt, ...)    ((void)0)
#endif

#if DEBUG_LEVEL >= DEBUG_LEVEL_DEBUG
    #define DEBUG_DEBUG(fmt, ...)   debug_uart_printf("[DEBUG] " fmt "\r\n", ##__VA_ARGS__)
#else
    #define DEBUG_DEBUG(fmt, ...)   ((void)0)
#endif

#if DEBUG_LEVEL >= DEBUG_LEVEL_VERBOSE
    #define DEBUG_VERBOSE(fmt, ...) debug_uart_printf("[VERB]  " fmt "\r\n", ##__VA_ARGS__)
#else
    #define DEBUG_VERBOSE(fmt, ...) ((void)0)
#endif

/* Hex dump macro */
#define DEBUG_HEX_DUMP(data, len, prefix) \
    do { \
        if (DEBUG_LEVEL >= DEBUG_LEVEL_INFO) { \
            debug_uart_hex_dump((data), (len), (prefix)); \
        } \
    } while(0)

/* Assert macro with debug output */
#define DEBUG_ASSERT(condition) \
    do { \
        if (!(condition)) { \
            DEBUG_ERROR("ASSERT FAILED: %s at %s:%d", #condition, __FILE__, __LINE__); \
            while(1); \
        } \
    } while(0)

#endif /* DEBUG_UART_H__ */
