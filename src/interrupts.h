#pragma once
#include "stdtypes.h"
#include <stdint.h>

#define IDT_SIZE 256

typedef struct {
    uint16_t base_low;  // lower 16 bits of the handler address
    uint16_t selector;  // kernel segment selector
    uint8_t always0;    // always zero
    uint8_t flags;      // type and attributes
    uint16_t base_high; // upper 16 bits of the handler address
} __attribute__((packed)) IDTEntry;

void initIDT(IDTEntry* idt);
void setIDTEntry(IDTEntry* entry, uint32_t base, uint16_t sel, uint8_t flags);
void interruptHandler();