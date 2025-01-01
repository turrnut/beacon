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

// External variables from screen.c
extern volatile struct Char* vga_buffer;
extern uint8_t default_color;

// External variables from keyboard.c
extern char input_buffer[INPUT_BUFFER_SIZE];
extern size_t input_len;

// Cursor position from screen.c
extern size_t curs_row;
extern size_t curs_col;

// Command buffer for history
#define COMMAND_HISTORY_SIZE 5
char command_history[COMMAND_HISTORY_SIZE][INPUT_BUFFER_SIZE];
int command_history_index = 0;     // Index to store the next command
int current_history_index = -1;    // Index for navigating history

void delay_ms(int milliseconds) {
    volatile int i = 0;
    while (i < milliseconds * 1000) { // Test with a larger multiplier
        i++;
    }
}

void reboot() {
    asm volatile ("cli"); // Disable interrupts
    while (inb(0x64) & 0x02); // Wait until the keyboard controller is ready
    outb(0x64, 0xFE); // Send the reset command to the keyboard controller
    asm volatile ("hlt"); // Halt the CPU if reboot fails
}


// Output byte to port
void outb(uint16_t port, uint8_t value) {
    asm volatile("outb %0, %1" : : "a"(value), "Nd"(port));
}

// strcpy replacement
char* strncpy(char* dest, const char* src, size_t n) {
    size_t i;
    for (i = 0; i < n && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    for (; i < n; i++) { // Fill the rest with null bytes if needed
        dest[i] = '\0';
    }
    return dest;
}

// strlen replacement
size_t strlen(const char* str) {
    size_t len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
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
    if (command[0] != '\0') {
        // Save command to history
        strncpy(command_history[command_history_index], command, INPUT_BUFFER_SIZE);
        command_history_index = (command_history_index + 1) % COMMAND_HISTORY_SIZE;
        current_history_index = -1; // Reset history navigation
    }

    if (my_strcmp(command, "test") == 0) {
        println("this is a test command.");
        curs_row++;
        if (curs_row >= NUM_ROWS) {
            scroll_screen();
            curs_row = NUM_ROWS - 1;
        }
        update_cursor();
    } else if (my_strcmp(command, "clear") == 0) {
        clear_screen();
        row = 0;
        println("");
        curs_row = 0;
        move_cursor_back();
        update_cursor();
    } else if (my_strcmp(command, "reboot") == 0) {
        println("rebooting the system...");
        delay_ms(1000); // Add a short delay for the message to be seen
        reboot();
    } else if (my_strcmp(command, "") == 0) {
        move_cursor_back();
        update_cursor();
    } else {
        move_cursor_back();
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
        scancode = inb(0x60);  // Read scancode from the keyboard port
        if (scancode) {
            handle_keypress(scancode);  // Call the handler directly
        }
    }
}
