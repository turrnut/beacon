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
