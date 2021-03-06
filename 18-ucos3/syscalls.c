/**
 * @file syscalls.c
 *
 * @note    Following 11. System Calls in Newlib LibC documentation
 */

#include <stdlib.h>
#include <sys/stat.h>
#include <sys/times.h>

/**
 * @brief errno
 *
 * @note  The C library must be compatible with development environments that
 *        supply fully functional versions of these subroutines. Such
 *        environments usually return error codes in a global errno. However,
 *        the Red Hat newlib C library provides a macro definition for errno
 *        in the header file errno.h, as part of its support for reentrant
 *        routines (see Reentrancy).
 *
 * @note  The bridge between these two interpretations of errno is
 *        straightforward: the C library routines with OS interface calls
 *        capture the errno values returned globally, and record them in
 *        the appropriate field of the reentrancy structure (so that you can
 *        query them using the errno macro from errno.h).
 *
 * @note  This mechanism becomes visible when you write stub routines for OS
 *        interfaces. You must include errno.h, then disable the macro
 *        like below.
 */

#include <errno.h>
#undef errno
extern int errno;


/**
 * @brief   including CMSIS functions
 */

#include "em_device.h"

/**
 * @brief compatibility layer
 */

static inline char * GetStackPointer(void) { return (char *) __get_MSP(); }

/**
 *  @brief
 */
#include "syscalls.h"

/**
 * @brief Interface to a serial interface for a minimal implementation
 *
 */
//@{
#include "uart.h"

void SerialInit(void)           { UART_Init();                }
void SerialWrite(char c)        { UART_SendChar(c);           }
int  SerialRead(void)           { return UART_GetChar();  }
int  SerialStatus(void)         { return UART_GetStatus();    }
//@}

/**
 * @brief   Library initialization
 *
 */

void _main(void) {
    SerialInit();
}

/**
 * @brief   _exit
 *
 * @note    Exit a program without cleaning up files. If your system doesn’t provide this,
 *          it is best to avoid linking with subroutines that require it (exit, system).
 */

void _exit(void) {
    while (1) {}        // eternal loop
}

/**
 * @brief   close
 *
 * @note    Close a file. Minimal implementation.
 */
int _close(int file) {
    return -1;
}

/**
 * @brief   environ
 *
 * @note    A pointer to a list of environment variables and their values.
 *          For a minimal environment, this empty list is adequate.
 */

char *__env[1] = { 0 };
char **environ = __env;

/**
 * @brief   execve
 *
 * @note    Transfer control to a new process. Minimal implementation
 *          (for a system without processes)
 */

int _execve(char *name, char **argv, char **env) {
      errno = ENOMEM;
      return -1;
}

/**
 * @brief   fork
 *
 * @note    Create a new process. Minimal implementation
 *          (for a system without processes)
 */

int _fork(void) {
      errno = EAGAIN;
      return -1;
}

/**
 * @brief   fstat
 *
 * @note    Status of an open file. For consistency with other minimal implementations
 *          in these examples, all files are regarded as character special devices.
 *          The sys/stat.h header file required is distributed in the include subdirectory
 *          for this C library.
 */

int _fstat(int file, struct stat *st) {
    st->st_mode = S_IFCHR;
    return 0;
}

/**
 * @brief   getpid
 *
 * @note    Process-ID; this is sometimes used to generate strings unlikely to conflict with
 *          other processes. Minimal implementation, for a system without processes.
 */

int _getpid(void) {
    return 1;
}

/**
 * @brief   isatty
 *
 * @note    Query whether output stream is a terminal.
 *          For consistency with the other minimal implementations,
 *          which only support output to stdout, this minimal implementation is suggested.
 */

int _isatty(int file) {
    return 1;
}

/**
 * @brief   kill
 *
 * @note    Send a signal. Minimal implementation.
 */
int _kill(int pid, int sig) {
    errno = EINVAL;
    return -1;
}

/**
 * @brief   link
 *
 * @note    Establish a new name for an existing file. Minimal implementation.
 */

int _link(char *old, char *new) {
    errno = EMLINK;
    return -1;
}

/**
 * @brief   lseek
 *
 * @note    Set position in a file. Minimal implementation.
 */

int _lseek(int file, int ptr, int dir) {
    return 0;
}

/**
 * @brief   open
 *
 * @note    Open a file. Minimal implementation.
 */

int _open(const char *name, int flags, int mode) {
    return -1;
}

/**
 * @brief   read
 *
 * @note    Read from a file. Minimal implementation.
 */

int _read(int file, char *ptr, int len) {
    return 0;
}

/**
 * @brief   sbrk
 *
 * @note    Increase program data space. As malloc and related functions depend on this,
 *          it is useful to have a working implementation. The following suffices for
 *          a standalone system; it exploits the symbol _end automatically defined
 *          by the GNU linker.
 */

caddr_t _sbrk(int incr) {
extern char _end;		/* Defined in the linker script */
static char *heap_end;
char *prev_heap_end;

    if (heap_end == 0) {
        heap_end = &_end;
    }
    prev_heap_end = heap_end;
    if( (heap_end + incr) > GetStackPointer() ) {
        _write(1, "Heap and stack collision\n", 25);
        abort ();
    }

    heap_end += incr;
    return (caddr_t) prev_heap_end;
}

/**
 * @brief   stat
 *
 * @note    Status of a file (by name). Minimal implementation.
 */

int _stat(char *file, struct stat *st) {
    st->st_mode = S_IFCHR;
    return 0;
}

/**
 * @brief   times
 *
 * @note    Timing information for current process. Minimal implementation.
 */

int _times(struct tms *buf) {
    return -1;
}

/**
 * @brief   unlink
 *
 * @note    Remove a file’s directory entry. Minimal implementation.
 */

int _unlink(char *name) {
  errno = ENOENT;
  return -1;
}

/**
 * @brief   wait
 *
 * @note    Wait for a child process. Minimal implementation.
 */

int _wait(int *status) {
    errno = ECHILD;
    return -1;
}

/**
 * @brief   write
 *
 * @note    Write to a file. libc subroutines will use this system routine for output to
 *          all files, including stdout— so if you need to generate any output,
 *          for example to a serial port for debugging, you should make your minimal write
 *          capable of doing this. The following minimal implementation is an incomplete
 *          example; it relies on a outbyte subroutine (not shown; typically, you must write this
 *          in assembler from examples provided by your hardware manufacturer) to
 *          actually perform the output.
 */

int _write(int file, char *ptr, int len) {
    int todo;

    for (todo = 0; todo < len; todo++) {
        SerialWrite(*ptr++);
    }
    return len;
}
