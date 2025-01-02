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
#include "console.h"
#include "stdtypes.h"
#include "command.h"
#include "string.h"

// External variables from screen.c
extern volatile struct Char* vga_buffer;
extern uint8_t default_color;

// External variables from keyboard.c
extern char input_buffer[INPUT_BUFFER_SIZE];
extern size_t input_len;

// Cursor position from screen.c
extern size_t curs_row;
extern size_t curs_col;

/**
 * Delays execution for a specified number of milliseconds.
 */
void delay_ms(int milliseconds) {
    volatile int i = 0;
    while (i < milliseconds * 1000) { // Test with a larger multiplier
        i++;
    }
}

/**
 * Reboots the system by sending a reset command to the keyboard controller.
 */
void reboot() {
    asm volatile ("cli"); // Disable interrupts
    while (inb(0x64) & 0x02); // Wait until the keyboard controller is ready
    outb(0x64, 0xFE); // Send the reset command to the keyboard controller
    asm volatile ("hlt"); // Halt the CPU if reboot fails
}



/**
 * Moves the cursor to the beginning of the current line.
 */
void move_cursor_back() {
    curs_col = 0;
    update_cursor();
}

/**
 * Entry point of the OS.
 */
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
        scancode = inb(0x60);  // Read scancode from the keyboard port
        if (scancode) {
            handle_keypress(scancode);  // Call the handler directly
        }
    }
}
