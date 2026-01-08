/**
 * @file syscalls.c
 * @brief Minimal system calls for bare metal ARM Cortex-M0+
 * 
 * This file provides minimal implementations of system calls
 * required by newlib when linking with GCC for embedded systems.
 */

#include <sys/stat.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <sys/times.h>

/* Minimal implementation of _exit */
void _exit(int status)
{
    (void)status;
    /* Disable interrupts and enter infinite loop */
    __asm volatile("cpsid i");
    while(1) {
        __asm volatile("wfi");
    }
}

/* Minimal stubs for other required syscalls */
int _close(int file)
{
    (void)file;
    return -1;
}

int _fstat(int file, struct stat *st)
{
    (void)file;
    (void)st;
    return -1;
}

int _isatty(int file)
{
    (void)file;
    return 1;
}

int _lseek(int file, int ptr, int dir)
{
    (void)file;
    (void)ptr;
    (void)dir;
    return 0;
}

int _open(const char *name, int flags, int mode)
{
    (void)name;
    (void)flags;
    (void)mode;
    return -1;
}

int _read(int file, char *ptr, int len)
{
    (void)file;
    (void)ptr;
    (void)len;
    return 0;
}

int _write(int file, char *ptr, int len)
{
    (void)file;
    (void)ptr;
    (void)len;
    return len;
}

caddr_t _sbrk(int incr)
{
    extern char end;		/* Defined by the linker */
    static char *heap_end;
    char *prev_heap_end;
    
    if (heap_end == 0) {
        heap_end = &end;
    }
    prev_heap_end = heap_end;
    
    heap_end += incr;
    return (caddr_t) prev_heap_end;
}

int _kill(int pid, int sig)
{
    (void)pid;
    (void)sig;
    errno = EINVAL;
    return -1;
}

int _getpid(void)
{
    return 1;
}