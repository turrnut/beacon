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

#define MAX_ARGS 10

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
 * Outputs a byte to the specified port.
 */
void outb(uint16_t port, uint8_t value) {
    asm volatile("outb %0, %1" : : "a"(value), "Nd"(port));
}

/**
 * Copies up to n characters from src to dest.
 */
char* strncpy(char* dest, const char* src, size_t n) {
    size_t i;
    for (i = 0; i < n && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    for (; i < n; i++) {
        dest[i] = '\0';
    }
    return dest;
}

/**
 * Returns the length of the string.
 */
size_t strlen(const char* str) {
    size_t len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

/**
 * Compares two strings.
 */
int strcmp(const char* s1, const char* s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

/**
 * Finds the first occurrence of character c in string str.
 */
char* strchr(const char* str, char c) {
    while (*str) {
        if (*str == c) {
            return (char*)str;
        }
        str++;
    }
    return NULL;
}

/**
 * Inputs a byte from the specified port.
 */
uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

/**
 * Copies the string src to dest.
 */
char* strcpy(char* dest, const char* src) {
    char* original_dest = dest;
    while ((*dest++ = *src++) != '\0') {
        // Copy each character
    }
    return original_dest;
}

/**
 * Helper function to split command into command and arguments.
 */
int parse_command(const char* command, char* cmd, char args[MAX_ARGS][INPUT_BUFFER_SIZE]) {
    int arg_count = 0;
    const char* token = command;
    char* next_space = NULL;

    // Extract base command
    next_space = strchr(token, ' ');
    if (next_space) {
        strncpy(cmd, token, next_space - token);
        cmd[next_space - token] = '\0';
        token = next_space + 1;
    } else {
        strcpy(cmd, token);
        return 0; // No arguments
    }

    // Extract arguments
    while ((next_space = strchr(token, ' ')) != NULL && arg_count < MAX_ARGS) {
        strncpy(args[arg_count], token, next_space - token);
        args[arg_count][next_space - token] = '\0';
        token = next_space + 1;
        arg_count++;
    }

    if (*token != '\0' && arg_count < MAX_ARGS) {
        strcpy(args[arg_count], token);
        arg_count++;
    }

    return arg_count;
}

/**
 * Processes the entered command.
 */
void process_command(const char* command) {
    if (command[0] != '\0') {
        // Save command to history
        strncpy(command_history[command_history_index], command, INPUT_BUFFER_SIZE);
        command_history_index = (command_history_index + 1) % COMMAND_HISTORY_SIZE;
        current_history_index = -1; // Reset history navigation
    }

    char cmd[INPUT_BUFFER_SIZE] = {0};
    char args[MAX_ARGS][INPUT_BUFFER_SIZE] = {{0}};
    int arg_count = parse_command(command, cmd, args);

    if (strcmp(cmd, "test") == 0) {
        if (arg_count > 0) {
            if (strcmp(args[0], "argument") == 0) {
                println("You passed the magic argument 'argument'. Congrats, I guess.");
            } else {
                println("Unrecognized argument. Try harder, idiot.");
            }
        } else {
            println("This is a test command, but you didn't even give me any arguments. Nice one.");
        }
        curs_row++;
        if (curs_row >= NUM_ROWS) {
            scroll_screen();
            curs_row = NUM_ROWS - 1;
        }
        update_cursor();
    }
    else if (strcmp(cmd, "clear") == 0) {
        clear_screen();
        row = 0;
        println("");
        curs_row = 0;
        move_cursor_back();
        update_cursor();
    }
    else if (strcmp(cmd, "reboot") == 0) {
        println("Rebooting the system...");
        delay_ms(1000); // Add a short delay for the message to be seen
        reboot();
    }
    else if (strcmp(cmd, "") == 0) {
        move_cursor_back();
        update_cursor();
    }
    else {
        move_cursor_back();
        print("\"");
        print(cmd);
        println("\" is not a known command or executable program.");
        curs_row++;
        if (curs_row >= NUM_ROWS) {
            scroll_screen();
            curs_row = NUM_ROWS - 1;
        }
        update_cursor();
    }
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
