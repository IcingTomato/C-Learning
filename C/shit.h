#ifndef _SHIT_H
#define _SHIT_H

// Standard Libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <ctype.h>
#include <limits.h>
#include <float.h>
#include <assert.h>
#include <errno.h>
#include <stddef.h>
#include <signal.h>
#include <setjmp.h>
#include <locale.h>

// Platform detect
#if defined(_WIN32) || defined(WIN32) || defined(__CYGWIN__)
// Get process ID. In Unix-like use unistd.h
    // Windows NT
    #include <windows.h>
    #include <process.h>
    #define PLATFORM_WINDOWS 1
#elif defined(__APPLE__) && defined(__MACH__)
    // Darwin
    #include <unistd.h>
    #define PLATFORM_MACOS 1
#elif defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__))
    // Unix-like 
    #include <unistd.h>
    
    #if defined(__linux__)
        #define PLATFORM_LINUX 1
    #elif defined(__FreeBSD__)
        #define PLATFORM_FREEBSD 1
    #elif defined(__ANDROID__)
        #define PLATFORM_ANDROID 1
    #endif
    
    #define PLATFORM_UNIX 1
#else
    #error "Unsupported platform!"
#endif

// Extended Libraries
#include <pthread.h>

#endif