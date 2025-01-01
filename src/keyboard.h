// keyboard.h

#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "stdtypes.h"

// Keyboard buffer sizes
#define INPUT_BUFFER_SIZE 256
#define KEY_BUFFER_SIZE 256

// Keyboard-related variables
extern char input_buffer[INPUT_BUFFER_SIZE];
extern size_t input_len; // Ensure this is size_t

// Circular keyboard buffer
extern volatile uint8_t key_buffer[KEY_BUFFER_SIZE];
extern volatile size_t buffer_head; // Ensure this is size_t
extern volatile size_t buffer_tail; // Ensure this is size_t

// Keyboard-related functions
void buffer_add(uint8_t scancode);
int buffer_get(uint8_t* scancode);
char scancode_to_ascii(uint8_t scancode);
void handle_keypress(uint8_t scancode);

#endif // KEYBOARD_H
