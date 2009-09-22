/* include/config.h.in.  Generated from configure.ac by autoheader.  */
#include "oscar.h"

/* whether to build support for Code 128 symbology */
#undef ENABLE_CODE128

/* whether to build support for Code 39 symbology */
#undef ENABLE_CODE39

/* whether to build support for EAN symbologies */
#define ENABLE_EAN

/* whether to build support for Interleaved 2 of 5 symbology */
#undef ENABLE_I25

/* whether to build support for PDF417 symbology */
#undef ENABLE_PDF417

/* Define to 1 if you have the `atexit' function. */
#undef HAVE_ATEXIT

/* Define to 1 if you have the <dlfcn.h> header file. */
#undef HAVE_DLFCN_H

/* Define to 1 if you have the <fcntl.h> header file. */
#undef HAVE_FCNTL_H

/* Define to 1 if you have the `getpagesize' function. */
#undef HAVE_GETPAGESIZE

/* Define to 1 if you have the <inttypes.h> header file. */
#undef HAVE_INTTYPES_H

/* Define to 1 if you have the <jpeglib.h> header file. */
#undef HAVE_JPEGLIB_H

/* Define to 1 if you have the `jpeg' library (-ljpeg). */
#undef HAVE_LIBJPEG

/* Define to 1 if you have the `pthread' library (-lpthread). */
#undef HAVE_LIBPTHREAD

/* Define to 1 if you have the <linux/videodev2.h> header file. */
#undef HAVE_LINUX_VIDEODEV2_H

/* Define to 1 if you have the <linux/videodev.h> header file. */
#undef HAVE_LINUX_VIDEODEV_H

/* Define to 1 if you have the <memory.h> header file. */
#undef HAVE_MEMORY_H

/* Define to 1 if you have the `memset' function. */
#define HAVE_MEMSET

/* Define to 1 if you have a working `mmap' system call. */
#undef HAVE_MMAP

/* Define to 1 if you have the <poll.h> header file. */
#undef HAVE_POLL_H

/* Define to 1 if you have the <pthread.h> header file. */
#undef HAVE_PTHREAD_H

/* Define to 1 if you have the `setenv' function. */
#undef HAVE_SETENV

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H

/* Define to 1 if you have the <sys/ioctl.h> header file. */
#define HAVE_SYS_IOCTL_H

/* Define to 1 if you have the <sys/ipc.h> header file. */
#define HAVE_SYS_IPC_H

/* Define to 1 if you have the <sys/mman.h> header file. */
#undef HAVE_SYS_MMAN_H

/* Define to 1 if you have the <sys/shm.h> header file. */
#undef HAVE_SYS_SHM_H

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H

/* Define to 1 if you have the <sys/times.h> header file. */
#undef HAVE_SYS_TIMES_H

/* Define to 1 if you have the <sys/time.h> header file. */
#undef HAVE_SYS_TIME_H

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H

/* Define to 1 if the system has the type `uintptr_t'. */
#undef HAVE_UINTPTR_T

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H

/* Define to 1 if you have the <X11/extensions/XShm.h> header file. */
#undef HAVE_X11_EXTENSIONS_XSHM_H

/* Define to 1 if you have the <X11/extensions/Xvlib.h> header file. */
#undef HAVE_X11_EXTENSIONS_XVLIB_H

/* Define to the sub-directory in which libtool stores uninstalled libraries.
   */
#undef LT_OBJDIR

/* Define to 1 if assertions should be disabled. */
#undef NDEBUG

/* Define to 1 if your C compiler doesn't accept -c and -o together. */
#undef NO_MINUS_C_MINUS_O

/* Name of package */
#define PACKAGE leanXzebra

/* Define to the address where bug reports for this package should be sent. */
#undef PACKAGE_BUGREPORT

/* Define to the full name of this package. */
#undef PACKAGE_NAME

/* Define to the full name and version of this package. */
#undef PACKAGE_STRING

/* Define to the one symbol short name of this package. */
#undef PACKAGE_TARNAME

/* Define to the version of this package. */
#define PACKAGE_VERSION 0.1

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS

/* Version number of package */
#undef VERSION

/* Define to 1 if the X Window System is missing or not being used. */
#define X_DISPLAY_MISSING

/* Program major version (before the '.') as a number */
#define ZEBRA_VERSION_MAJOR 0

/* Program minor version (after '.') as a number */
#define ZEBRA_VERSION_MINOR 1

/* Define for Solaris 2.5.1 so the uint32_t typedef from <sys/synch.h>,
   <pthread.h>, or <semaphore.h> is not used. If the typedef was allowed, the
   #define below would cause a syntax error. */
#undef _UINT32_T

/* Define for Solaris 2.5.1 so the uint8_t typedef from <sys/synch.h>,
   <pthread.h>, or <semaphore.h> is not used. If the typedef was allowed, the
   #define below would cause a syntax error. */
#undef _UINT8_T

/* Define to empty if `const' does not conform to ANSI C. */
#undef const

/* Define to `__inline__' or `__inline' if that's what the C compiler
   calls it, or to nothing if 'inline' is not supported under any name.  */
#ifndef __cplusplus
#undef inline
#endif

/* Define to the type of a signed integer type of width exactly 32 bits if
   such a type exists and the standard includes do not define it. */
#define int32_t int32

/* Define to the type of an unsigned integer type of width exactly 32 bits if
   such a type exists and the standard includes do not define it. */
#define uint32_t uint32

/* Define to the type of an unsigned integer type of width exactly 8 bits if
   such a type exists and the standard includes do not define it. */
#define uint8_t uint8
#define uint16_t uint16

/* Define to the type of an unsigned integer type wide enough to hold a
   pointer, if such a type exists, and if the system does not define it. */
#define uintptr_t uint32

#ifndef X_DISPLAY_MISSING
# define HAVE_X
#endif

