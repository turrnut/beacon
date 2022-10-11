/*
    console.h
    Definition of functions of the console. specific implementation
    can be found in console.cpp
*/

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
enum ColorConstants {
    BLACK_COLOR=0,
    RED_COLOR=4,
    PINK_COLOR=13,
    YELLOW_COLOR=14,
    LIME_COLOR=10,
    GREEN_COLOR=2,
    CYAN_COLOR=3,
    BLUE_COLOR=1,
    GREY_COLOR=7,
    BROWN_COLOR=6,
    WHITE_COLOR=15
};
void clearRow(size_t idx);
void clearScreen();
void newline();
void print(char *str);
void println(char *str);
void screen_color(uint8_t fgcolor, uint8_t bgcolor);