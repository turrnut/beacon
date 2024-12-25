/**
 * Author: turrnut
 * Copyright © turrnut under the GNU General Public License Version 3
 *
 * os.c
 */

#include "console.h"
#include "gdt.h"
#include "interrupt.h"
#include "stdtypes.h"

void start() {
	clear_screen();
	set_color(GREEN_COLOR, WHITE_COLOR);
	println("Beacon Operating System");
	println("Copyright (c) Turrnut");

	GlobalDescriptorTable table;
	initTable(&table);

	initInterruptManager(&table);

	activate();
	
	while (1) {}
}
