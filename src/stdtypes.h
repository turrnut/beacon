/**
 * Copyright (c) Turrnut Open Source Organization
 * Under the GPL v3 License
 * See COPYING for information on how you can use this file
 * 
 * stdtypes.h
 */

#ifndef STDTYPES_H
#define STDTYPES_H

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
