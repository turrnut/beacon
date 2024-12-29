/**
 * Author: turrnut
 * Copyright © turrnut under the GNU General Public License Version 3
 *
 * os.c
 */

#include "console.h"
#include "gdt.h"
#include "interrupts.h"
#include "stdtypes.h"

#include "pic.h" // Include the new PIC header

void start() {
    clear_screen();
    set_color(GREEN_COLOR, WHITE_COLOR);
    println("Beacon Operating System");
    println("Copyright (c) Turrnut");

   println("Initializing GDT...");
    GlobalDescriptorTable table;
    initTable(&table);
   println("Initializing IDT...");
    IDTEntry idt[IDT_SIZE];
    initIDT(idt);

   println("Remapping PIC...");
    pic_remap(); // Remap PIC

   println("Enabling interrupts...");
    __asm__ volatile ("sti"); // Enable interrupts globally

    while (1) {
        __asm__ volatile ("hlt");
    }
}
