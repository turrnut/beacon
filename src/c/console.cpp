/*
    console.cpp
    Implementation of the functions defined in console.h
*/
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "console.h"
/*
    Character class represents a single character.
*/
struct Character {
    uint8_t symbol;
    uint8_t color;
};
uint8_t color = WHITE_COLOR | BLACK_COLOR << 4;
struct Character* video = (struct Character*) 0xb8000;
/*
    The Position class help to keep track where is
    the video memory's pointer to determine where 
    to put the characters.
*/
class Position{
    public:
        size_t row_count;
        size_t col_count;
        Position(size_t row, size_t col){
            this->row_count = row;
            this->col_count = col;
        }
        void row(size_t arg){
            this->row_count = arg;
        }
        size_t row(){
            return this->row_count;
        }
        void col(size_t arg){
            this->col_count = arg;
        }
        size_t col(){
            return this->col_count;
        }
};
Position pos(0,0);
const static size_t rownum = 80;
const static size_t colnum = 80;
/*
    Clears a row of characters.
*/
void clearRow(size_t idx) {
    struct Character blank = (struct Character) {
        symbol:' ',
        color:color,
    };
    
    for(size_t col=0;col<colnum;col++) {
        video[col+colnum*idx] = blank;
    }
}

/*
    Create a new line in the video memory.
*/
void newline(){
    pos.col(0);
    if (pos.row()<rownum-1) {
        pos.row(pos.row() + 1);
        return;
    }
    for (size_t row=1;row<rownum;row++) {
        for (size_t col = 0; col < colnum; col++) {
            struct Character character = video[col+colnum*row];
            video[col+colnum*(row-1)]=character;
        }
    }
    clearRow(colnum - 1);
}

/*
    Empties the screen.
*/
void clearScreen() {
    for (size_t index=0;index<rownum;index++)
    {
        clearRow(index);
    }
}

/*
    Set the foreground and background screen color.
*/
void screen_color(uint8_t fgcolor, uint8_t bgcolor) {
    color = fgcolor + (bgcolor << 4);
}

/*
    Print a collection of characters to the video memory.
*/
void print(char *str){
    size_t index=0;
    while(true){
        if((uint8_t)str[index] != '\0'){
            if ((uint8_t)str[index] == '\n') {
                newline();
                break;
            } else if (pos.col() > colnum) {
                newline();
            }
            video[pos.col() + colnum * pos.row()] = (struct Character){
                symbol: (uint8_t) str[index],
                color: color,
            };
            pos.col(pos.col() + 1);
            index += 1;

            continue;
        }
        break;
    }
}

/*
    Print a collections of characters end with a new line
*/
void println(char *str) {
    print(str);
    newline();
}