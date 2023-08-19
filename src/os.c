/**
 * Author: turrnut
 * Copyrighted © turrnut under the Apache 2.0 license
 *
 * We hope that you will utilize this open-source software
 * responsibly. Please read the LICENSE for comprehensive
 * information on how you can employ it. You have the
 * freedom to distribute and incorporate this file into your
 * project. However, you must indicate any modifications
 * you've made and provide credit to the original author of
 * this file.
 *
 * os.c
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "console.h"
#include "gdt.h"

void start() {
	clear_screen();
	set_color(WHITE_COLOR, BLACK_COLOR);
	println("Welcome to my operating system!");
	GlobalDescriptorTable table;
	initTable(&table);
	
	while (1) {}
}
