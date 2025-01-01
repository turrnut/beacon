// keyboard.c

#include "keyboard.h"
#include "os.h"
#include "screen.h"
#include "console.h" // Assumes you have a console module

char input_buffer[INPUT_BUFFER_SIZE];
size_t input_len = 0;

volatile uint8_t key_buffer[KEY_BUFFER_SIZE];
volatile size_t buffer_head = 0;
volatile size_t buffer_tail = 0;

// Add a scancode to the buffer
void buffer_add(uint8_t scancode) {
    size_t next_head = (buffer_head + 1) % KEY_BUFFER_SIZE;
    if (next_head != buffer_tail) { // Don't overwrite
        key_buffer[buffer_head] = scancode;
        buffer_head = next_head;
    }
}

// Get a scancode from the buffer
int buffer_get(uint8_t* scancode) {
    if (buffer_head == buffer_tail) {
        return 0; // Buffer is empty
    }
    *scancode = key_buffer[buffer_tail];
    buffer_tail = (buffer_tail + 1) % KEY_BUFFER_SIZE;
    return 1;
}

// Scancode to ASCII conversion
char scancode_to_ascii(uint8_t scancode) {
    static const uint8_t scancode_map[128] = {
        0,  0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
        0, 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0,
        'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\',
        'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' ', 0,
        // Fill with 0s for unhandled keys
    };

    if (scancode < 128) {
        return scancode_map[scancode];
    }
    return 0; // Invalid scancode
}

// Handle keypress events
void handle_keypress(uint8_t scancode) {
    if (scancode & 0x80) {
        return; // Ignore key releases
    }

    char ascii = scancode_to_ascii(scancode);
    if (!ascii) {
        return; // Skip invalid keys
    }

        if (ascii == '\b') { // handle backspace
            if (input_len > 0) { // do nothing if there's no input
                input_len--; // shrink the buffer
                input_buffer[input_len] = '\0'; // null-terminate the buffer

                // move the cursor back
                if (curs_col == 0) { // we're at the start of a line
                    if (curs_row > 0) { // not at the top of the screen
                        curs_row--;
                        curs_col = NUM_COLS - 1;
                    }
                } else {
                    curs_col--;
                }

                // visually clear the character on screen
                vga_buffer[curs_row * NUM_COLS + curs_col] = (struct Char){' ', default_color};

                // redraw everything after the cursor (fix for already binted text)
                for (size_t i = input_len; i < INPUT_BUFFER_SIZE; i++) {
                    if (input_buffer[i] == '\0') {
                        break;
                    }
                    vga_buffer[(curs_row * NUM_COLS) + curs_col] = (struct Char){' ', default_color};
                    curs_col++;
                    if (curs_col >= NUM_COLS) {
                        curs_col = 0;
                        curs_row++;
                    }
                }

                update_cursor(); // update position
            }
        } else if (ascii == '\n') { // Handle newline
        input_buffer[input_len] = '\0';
        println(""); // Move to a new line
        process_command(input_buffer);
        input_len = 0;
        curs_row++;
        curs_col = 0;
        if (curs_row >= NUM_ROWS) {
            scroll_screen();
            curs_row = NUM_ROWS - 1;
        }
        update_cursor();
    } else { // Handle regular characters
        if (input_len < INPUT_BUFFER_SIZE - 1) {
            input_buffer[input_len++] = ascii;
        }
        vga_buffer[curs_row * NUM_COLS + curs_col] = (struct Char){ascii, default_color};
        curs_col++;
        if (curs_col >= NUM_COLS) { // Wrap to the next line
            curs_col = 0;
            curs_row++;
        }
        if (curs_row >= NUM_ROWS) { // Scroll if needed
            scroll_screen();
            curs_row = NUM_ROWS - 1;
        }
        update_cursor();
    }
}
