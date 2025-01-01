/**
 * Copyright (c) Turrnut Open Source Organization
 * Under the GPL v3 License
 * See COPYING for information on how you can use this file
 * 
 * os.h
 */

#ifndef OS_H
#define OS_H

#include "stdtypes.h"

// Function declarations
void outb(uint16_t port, uint8_t value);
uint8_t inb(uint16_t port);
void start();
void process_command(const char* command);
void move_cursor_back();

#endif // OS_H
