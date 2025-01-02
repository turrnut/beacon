/**
 * Copyright (c) Turrnut Open Source Organization
 * Under the GPL v3 License
 * See COPYING for information on how you can use this file
 * 
 * console.c
 */

#include "console.h"

const static size_t NUM_COLS = 80;
const static size_t NUM_ROWS = 25;

// Define col and row in this file
size_t col = 0;
size_t row = 0;

// Default color setting
uint8_t default_color = GREEN_COLOR | WHITE_COLOR << 4;

struct Char {
    uint8_t character;
    uint8_t color;
};


struct Char *buffer = (struct Char *)0xb8000;

void clear_row(size_t row)
{
    struct Char empty = (struct Char){
        .character = ' ',
        .color = default_color,
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
    col = 0;  // Reset the column to the start

    if (row < NUM_ROWS - 1)
    {
        row++;  // Move to the next row
        return;
    }

    // Scroll the screen if we're at the bottom
    for (size_t row = 1; row < NUM_ROWS; row++)
    {
        for (size_t col = 0; col < NUM_COLS; col++)
        {
            struct Char character = buffer[col + NUM_COLS * row];
            buffer[col + NUM_COLS * (row - 1)] = character;
        }
    }

    // Clear the last row after scrolling
    clear_row(NUM_ROWS - 1);
}

void printc(char character)
{
    if (character == '\n')
    {
        newline();
        return;
    }

    if (col >= NUM_COLS)
    {
        newline();
    }

    buffer[col + NUM_COLS * row] = (struct Char){
        .character = (uint8_t)character,
        .color = default_color,
    };

    col++;  // Move to the next column
}

void print(const char *str)
{
    for (size_t i = 0; str[i] != '\0'; i++)
    {
        printc(str[i]);  // Print each character
    }
}

void println(const char *str)
{
    print(str);
    newline();  // Move to a new line after printing
}

void set_color(uint8_t foreground, uint8_t background)
{
    default_color = foreground + (background << 4);  // Update color
}

void move_cursor_left()
{
    if (col > 0)  // Don't go past the first column
    {
        col--;  // Decrease the column position
        buffer[col + NUM_COLS * row].character = ' ';  // Clear the character in that position
    }
}


void int_to_string(int number, char *str) {
    int is_negative = 0;
    int i = 0;

    // Handle negative numbers
    if (number < 0) {
        is_negative = 1;
        number = -number; // Make it positive for easier processing
    }

    // Extract digits and store them in reverse order
    do {
        str[i++] = (number % 10) + '0'; // Convert digit to character
        number /= 10;
    } while (number > 0);

    // Add negative sign if needed
    if (is_negative) {
        str[i++] = '-';
    }

    // Null-terminate the string
    str[i] = '\0';

    // Reverse the string to get the correct order
    for (int j = 0, k = i - 1; j < k; ++j, --k) {
        char temp = str[j];
        str[j] = str[k];
        str[k] = temp;
    }
}
