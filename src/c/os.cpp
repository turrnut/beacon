#include "console.h"
extern "C" void kernel() {
    clearScreen();
    screen_color(CYAN_COLOR, BLACK_COLOR);
    println("Beacon operating system");
}