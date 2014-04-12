//
//  Common.h
//  UGLEngine
//
//  Created by Jared Jones on 4/8/14.
//  Copyright (c) 2014 Jared Jones. All rights reserved.
//

#ifndef UGLEngine_Common_h
#define UGLEngine_Common_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <errno.h>
#include <signal.h>
#include <assert.h>

#include <set>
#include <list>
#include <string>
#include <map>
#include <queue>
#include <sstream>
#include <algorithm>
#include <cstdint>
#include <thread>
#include <mutex>

// current platform and compiler
#define PLATFORM_WIN32 0
#define PLATFORM_UNIX  1
#define PLATFORM_APPLE 2

#define UNIX_FLAVOUR_LINUX 1
#define UNIX_FLAVOUR_BSD 2
#define UNIX_FLAVOUR_OTHER 3
#define UNIX_FLAVOUR_OSX 4

#if defined( __WIN32__ ) || defined( WIN32 ) || defined( _WIN32 )
#  define PLATFORM PLATFORM_WIN32
#elif defined( __APPLE_CC__ )
#  define PLATFORM PLATFORM_APPLE
#else
#  define PLATFORM PLATFORM_UNIX
#endif

#define COMPILER_MICROSOFT 0
#define COMPILER_GNU	   1
#define COMPILER_BORLAND   2

#ifdef _MSC_VER
#  define COMPILER COMPILER_MICROSOFT
#elif defined( __BORLANDC__ )
#  define COMPILER COMPILER_BORLAND
#elif defined( __GNUC__ )
#  define COMPILER COMPILER_GNU
#else
#  pragma error "FATAL ERROR: Unknown compiler."
#endif

#if PLATFORM == PLATFORM_UNIX || PLATFORM == PLATFORM_APPLE
#ifdef __APPLE__
#define PLATFORM_TEXT "MacOSX"
#define UNIX_FLAVOUR UNIX_FLAVOUR_OSX
#else
#ifdef USE_KQUEUE
#define PLATFORM_TEXT "FreeBSD"
#define UNIX_FLAVOUR UNIX_FLAVOUR_BSD
#else
#define PLATFORM_TEXT "Linux"
#define UNIX_FLAVOUR UNIX_FLAVOUR_LINUX
#endif
#endif
#endif

#if PLATFORM == PLATFORM_WIN32
#define PLATFORM_TEXT "Win32"
#endif

#ifdef DEBUG
#define CONFIG "Debug"
#else
#define CONFIG "Release"
#endif

#ifdef _LP64
#define ARCH "X64"
#else
#define ARCH "X86"
#endif


enum TimeConstants
{
    MINUTE          = 60,
    HOUR            = MINUTE*60,
    DAY             = HOUR*24,
    WEEK            = DAY*7,
    MONTH           = DAY*30,
    YEAR            = MONTH*12,
    IN_MILLISECONDS = 1000
};

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;
typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

const int UNIXSTARTYEAR = 1900;

#ifndef M_PI
#define M_PI    3.14159265358979323846f
#endif



#endif
