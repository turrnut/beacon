/**
 * Author: turrnut
 * Copyright © turrnut under the GNU General Public License Version 3
 *
 * interrupt.h
 */

#ifndef INTERRUPT_HEADER
#define INTERRUPT_HEADER

#include "stdtypes.h"
#include "port.h"
#include "gdt.h"

typedef struct {
    uint16_t handlerAddressLow;
    uint16_t gdtCodeSegmentSelector;
    uint8_t reserved;
    uint8_t access;
    uint16_t handlerAddressHigh;
} __attribute__((packed)) GateDescriptor;

typedef struct {
    uint16_t size;
    uint32_t base;
} __attribute__((packed)) interruptDescriptorTablePointer;

typedef struct {
    Port8BitSlow pic1Command;
    Port8BitSlow pic1Data;
    Port8BitSlow pic2Command;
    Port8BitSlow pic2Data;
} InterruptManager;

void initInterruptManager(GlobalDescriptorTable* gdt);
void setInterruptDescriptorTableEntry(uint8_t interrupt_number, uint16_t codeSegmentSelectorOffset, void (*handler)(), uint8_t descriptorPrivilegeLevel, uint8_t descriptorType);
extern void IgnoreInterruptRequest();
extern void HandleInterruptRequest0x00();
extern void HandleInterruptRequest0x01();
void activate();

uint32_t handleInterrupt(uint8_t interrupt_number, uint32_t esp);

#endif // INTERRUPT_HEADER
