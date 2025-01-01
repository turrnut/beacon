/**
 * Copyright (c) Turrnut Open Source Organization
 * Under the GPL v3 License
 * See COPYING for information on how you can use this file
 * 
 * os.c
 */

#include "os.h"
#include "screen.h"
#include "keyboard.h"
#include "console.h"    // Assumes you have a console module
#include "stdtypes.h"

// External variables from screen.c
extern volatile struct Char* vga_buffer;
extern uint8_t default_color;

// External variables from keyboard.c
extern char input_buffer[INPUT_BUFFER_SIZE];
extern size_t input_len;

// Cursor position from screen.c
extern size_t curs_row;
extern size_t curs_col;

// Function to add delay (static, used only in os.c)
static void delay_ms(int milliseconds) {
    volatile int i = 0;
    while (i < milliseconds * 1000) {
        i++;
    }
}

// Output byte to port
void outb(uint16_t port, uint8_t value) {
    asm volatile("outb %0, %1" : : "a"(value), "Nd"(port));
}

// Input byte from port
uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

// Static strcmp function for command comparison
static int my_strcmp(const char* s1, const char* s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

// Process user commands
void process_command(const char* command) {
    if (my_strcmp(command, "test") == 0) {
        println("this is a test command.");
        // Move the cursor down after the message
        curs_row++;
        if (curs_row >= NUM_ROWS) {
            scroll_screen();
            curs_row = NUM_ROWS - 1;
        }
        update_cursor();
    } else if(my_strcmp(command, "clear") == 0){
        clear_screen();
        row = 0;
        println("");
        curs_row = 0;
        move_cursor_back();
        update_cursor();
    } else if(my_strcmp(command, "") == 0) {
        move_cursor_back();
        update_cursor();
    } else {
        // Move the cursor back to the beginning of the line
        move_cursor_back();

        // Print the unknown command message on the same line
        print("\"");
        print(command);
        println("\" is not a known command or executable program.");
        curs_row++;
        if (curs_row >= NUM_ROWS) {
            scroll_screen();
            curs_row = NUM_ROWS - 1;
        }
        update_cursor();
    }
}

// Move cursor to the beginning of the current line
void move_cursor_back() {
    curs_col = 0;
    update_cursor();
}

// Entry point of the OS
void start() {
    clear_screen();
    set_color(GREEN_COLOR, WHITE_COLOR);

    enable_cursor(0, 15); // Enable the cursor

    println("Welcome to Beacon Operating System!");
    println("Copyright (c) 2025 Turrnut Open Source Organization.");
    println("Type a command:");

    curs_row = 3;
    curs_col = 0;
    update_cursor();

    while (1) {
        uint8_t scancode;
        // Poll for keyboard input manually
        scancode = inb(0x60);  // Read scancode from the keyboard port
        if (scancode) {
            handle_keypress(scancode);  // Call the handler directly
        }

        delay_ms(150); // Add delay to prevent excessive keypress spamming
    }
}
