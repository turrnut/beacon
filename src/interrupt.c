/**
 * Author: turrnut
 * Copyright © turrnut under the GNU General Public License Version 3
 *
 * interrupt.c
 */

#include "interrupt.h"
#include "stdtypes.h"
#include "console.h"

// Global declaration of interrupt manager and IDT
InterruptManager interruptManager;
GateDescriptor interruptDescriptorTable[256];

void initInterruptManager(GlobalDescriptorTable* gdt) {
    // Initialize command and data ports for both PICs
    initPort8BitSlow(&interruptManager.pic1Command, 0x20);
    initPort8BitSlow(&interruptManager.pic1Data, 0x21);
    initPort8BitSlow(&interruptManager.pic2Command, 0xA0);
    initPort8BitSlow(&interruptManager.pic2Data, 0xA1);

    // Start initialization sequence in cascade mode
    writePort8BitSlow(&interruptManager.pic1Command, 0x11);
    writePort8BitSlow(&interruptManager.pic2Command, 0x11);

    // Set vector offset
    writePort8BitSlow(&interruptManager.pic1Data, 0x20); // IRQ 0-7 is mapped to INT 0x20-0x27
    writePort8BitSlow(&interruptManager.pic2Data, 0x28); // IRQ 8-15 is mapped to INT 0x28-0x2F

    // Tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
    writePort8BitSlow(&interruptManager.pic1Data, 0x04);
    // Tell Slave PIC its cascade identity (0000 0010)
    writePort8BitSlow(&interruptManager.pic2Data, 0x02);

    // 8086/88 (MCS-80/85) mode
    writePort8BitSlow(&interruptManager.pic1Data, 0x01);
    writePort8BitSlow(&interruptManager.pic2Data, 0x01);

    // End of initialization sequence
    writePort8BitSlow(&interruptManager.pic1Data, 0x00);
    writePort8BitSlow(&interruptManager.pic2Data, 0x00);

    // Set IDT entries
    uint16_t codeSegment = getCodeSegmentSelector(gdt);
    const uint8_t IDT_INTERRUPT_GATE = 0xE;
    for (uint16_t i = 0; i < 256; i++) {
        setInterruptDescriptorTableEntry(i, codeSegment, &IgnoreInterruptRequest, 0, IDT_INTERRUPT_GATE);
    }

    // Set specific handlers for example interrupts
    setInterruptDescriptorTableEntry(0x20, codeSegment, &HandleInterruptRequest0x00, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(0x21, codeSegment, &HandleInterruptRequest0x01, 0, IDT_INTERRUPT_GATE);

    // Load IDT
    interruptDescriptorTablePointer idt;
    idt.size = 256 * sizeof(GateDescriptor) - 1;
    idt.base = (uint32_t)interruptDescriptorTable;
    asm volatile("lidt %0" : : "m" (idt));
}

void activate() {
    asm volatile("sti");
}

void setInterruptDescriptorTableEntry(uint8_t interrupt_number, uint16_t codeSegmentSelectorOffset, void (*handler)(), uint8_t descriptorPrivilegeLevel, uint8_t descriptorType) {
    const uint8_t IDT_DESC_PRESENT = 0x80;
    interruptDescriptorTable[interrupt_number].handlerAddressLow = ((uint32_t)handler) & 0xFFFF;
    interruptDescriptorTable[interrupt_number].handlerAddressHigh = (((uint32_t)handler) >> 16) & 0xFFFF;
    interruptDescriptorTable[interrupt_number].gdtCodeSegmentSelector = codeSegmentSelectorOffset;
    interruptDescriptorTable[interrupt_number].access = IDT_DESC_PRESENT | descriptorType | ((descriptorPrivilegeLevel & 3) << 5);
    interruptDescriptorTable[interrupt_number].reserved = 0;
}

uint32_t handleInterrupt(uint8_t interrupt_number, uint32_t esp) {
    // Example interrupt handling logic
    // println("Handled Interrupt: %d", interrupt_number);
    print(" INTERRUPT");
    return esp;
}

// // Dummy implementations for interrupt handlers
// void IgnoreInterruptRequest() {
//     // No operation, just returns
// }

// void HandleInterruptRequest0x00() {
//     println("Timer Interrupt Handled");
// }

// void HandleInterruptRequest0x01() {
//     println("Keyboard Interrupt Handled");
// }
