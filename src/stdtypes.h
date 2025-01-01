/**
 * Author: turrnut
 * Copyrighted © turrnut under the Apache 2.0 license
 *
 * We hope that you will utilize this open-source software
 * responsibly. Please read the LICENSE for comprehensive
 * information on how you can employ it. You have the
 * freedom to distribute and incorporate this file into your
 * project. However, you must indicate any modifications
 * you've made and provide credit to the original author of
 * this file.
 *
 * stdtypes.h
 */

#ifndef STDTYPES_HEADER
#define STDTYPES_HEADER

// Define custom types
typedef signed char    int8_t;
typedef short          int16_t;
typedef int            int32_t;
typedef long long      int64_t;
typedef unsigned char  uint8_t;
typedef unsigned short uint16_t;
typedef unsigned       uint32_t;
typedef unsigned long long uint64_t;

// Define size_t manually (commonly used as the size of a type, usually unsigned)
typedef unsigned int size_t;  // Size type (typically unsigned int)

#endif // STDTYPES_HEADER
