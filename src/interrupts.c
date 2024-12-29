#include "interrupts.h"
#include "stdtypes.h"
#include "console.h"
#include "io.h"
#include <stdint.h>
void initIDT(IDTEntry* idt) {
    for (int i = 0; i < IDT_SIZE; i++) {
        setIDTEntry(&idt[i], (uint32_t)interruptHandler, 0x08, 0x8E);
    }

    uint64_t idt_ptr[2];
    idt_ptr[0] = ((uint64_t)(sizeof(IDTEntry) * IDT_SIZE - 1)) | ((uint64_t)(uintptr_t)idt << 16);
    idt_ptr[1] = (uintptr_t)idt >> 16;

    __asm__ volatile ("lidt (%0)" : : "r"(idt_ptr) : "memory");
}

void setIDTEntry(IDTEntry* entry, uint32_t base, uint16_t sel, uint8_t flags) {
    entry->base_low = base & 0xFFFF;
    entry->selector = sel;
    entry->always0 = 0;
    entry->flags = flags;
    entry->base_high = (base >> 16) & 0xFFFF;
}

// generic interrupt handler
void interruptHandler() {
    outb(0x20, 0x20);
    println(" INTERRUPT");
    // hang or do something useful
    while (1) {
        __asm__ volatile ("hlt");
    }
}