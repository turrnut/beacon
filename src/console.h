/**
 * Author: turrnut
 * Copyrighted © turrnut under the Apache 2.0 license
 *
 * We hoped that you will use this piece of open source
 * software fairly. read the LICENSE for details for
 * more details about how you can use it, you have freedom
 * to distribute and use this file in your project. However,
 * you will have to state changes you made and include the
 * orginal author of this file.
 *
 * console.h
 */

#pragma once
#ifndef console_header
#define console_header

#include <stdint.h>
#include <stddef.h>

enum Color
{
	BLACK_COLOR,
	BLUE_COLOR,
	GREEN_COLOR,
	CYAN_COLOR,
	RED_COLOR,
	MAGENTA_COLOR,
	BROWN_COLOR,
	LIGHT_GRAY_COLOR,
	DARK_GRAY_COLOR,
	LIGHT_BLUE_COLOR,
	LIGHT_GREEN_COLOR,
	LIGHT_CYAN_COLOR,
	LIGHT_RED_COLOR,
	PINK_COLOR,
	YELLOW_COLOR,
	WHITE_COLOR
};

void clear_screen();
void newline();
void printc(char c);
void print(const char *str);
void println(const char *str);
void set_color(uint8_t fg, uint8_t bg);

#endif
