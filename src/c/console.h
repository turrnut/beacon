#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

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
struct Character {
    uint8_t symbol;
    uint8_t color;
};
uint8_t color = WHITE_COLOR | BLACK_COLOR << 4;
struct Character* buffer = (struct Character*) 0xb8000;
size_t row = 0;
size_t col = 0;
const static size_t rownum = 80;
const static size_t colnum = 80;
void clearRow(size_t idx) {
    struct Character blank = (struct Character) {
        symbol:' ',
        color:color,
    };
    
    for(size_t col=0;col<colnum;col++) {
        buffer[col+colnum*idx] = blank;
    }
}
void newline(){
    col = 0;
    if (row<rownum-1) {row+=1;return;}
    for (size_t row=1;row<rownum;row++) {
        for (size_t col = 0; col < colnum; col++) {
            struct Character character = buffer[col+colnum*row];
            buffer[col+colnum*(row-1)]=character;
        }
    }
    clearRow(colnum - 1);
}
void clearScreen() {
    for (size_t index=0;index<rownum;index++)
    {
        clearRow(index);
    }
}
void screen_color(uint8_t fgcolor, uint8_t bgcolor) {
    color = fgcolor + (bgcolor << 4);
}
void printc(char c){
    if (c == '\n') {
        newline();
        return;
    } else if (col > colnum) {
        newline();
    }
    buffer[col + colnum * row] = (struct Character){
        symbol: (uint8_t) c,
        color: color,
    };
    col += 1;
}
void print(char *str){
    size_t index=0;
    while(true){
        if((uint8_t)str[index] != '\0'){
            printc((uint8_t)str[index]);
            index += 1;
            continue;
        }
        break;
    }
}
void println(char *str) {
    print(str);
    newline();
}