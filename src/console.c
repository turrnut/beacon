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
 * console.c
 */
#ifndef console
#define console
#include "console.h"

const static size_t NUM_COLS = 80;
const static size_t NUM_ROWS = 25;

struct Char{
	uint8_t character;
	uint8_t color;
};

struct Char *buffer = (struct Char *)0xb8000;
size_t col = 0;
size_t row = 0;
uint8_t default_color = GREEN_COLOR | WHITE_COLOR << 4;

void clear_row(size_t row)
{
	struct Char empty = (struct Char){
		character : ' ',
		color : default_color,
	};

	for (size_t col = 0; col < NUM_COLS; col++)
	{
		buffer[col + NUM_COLS * row] = empty;
	}
}

void clear_screen()
{
	for (size_t i = 0; i < NUM_ROWS; i++)
	{
		clear_row(i);
	}
}

void newline()
{
	col = 0;

	if (row < NUM_ROWS - 1)
	{
		row++;
		return;
	}

	for (size_t row = 1; row < NUM_ROWS; row++)
	{
		for (size_t col = 0; col < NUM_COLS; col++)
		{
			struct Char character = buffer[col + NUM_COLS * row];
			buffer[col + NUM_COLS * (row - 1)] = character;
		}
	}

	clear_row(NUM_COLS - 1);
}

void printc(char character)
{
	if (character == '\n')
	{
		newline();
		return;
	}

	if (col > NUM_COLS)
	{
		newline();
	}

	buffer[col + NUM_COLS * row] = (struct Char){
		character : (uint8_t)character,
		color : default_color,
	};

	col++;
}

void print(const char *str)
{
	for (size_t i = 0; 1; i++)
	{
		char character = (uint8_t)str[i];

		if (character == '\0')
		{
			return;
		}

		printc(character);
	}
}

void println(const char *str)
{
	print(str);
	newline();
}

void set_color(uint8_t foreground, uint8_t background)
{
	default_color = foreground + (background << 4);
}

#endif
