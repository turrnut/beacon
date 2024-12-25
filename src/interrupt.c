/**
 * Author: turrnut
 * Copyright © turrnut under the GNU General Public License Version 3
 *
 * interrupt.c
 */

#include "interrupt.h"

#include "stdtypes.h"
#include "console.h"

InterruptManager interruptManager;
GateDescriptor interruptDescriptorTable[256];

void initInterruptManager(GlobalDescriptorTable* gdt) {
    Port8BitSlow myPic1Command;
    Port8BitSlow myPic1Data;
    Port8BitSlow myPic2Command;
    Port8BitSlow myPic2Data;

    initPort8BitSlow(&myPic1Command, 0x20);
    initPort8BitSlow(&myPic1Data   , 0x21);
    initPort8BitSlow(&myPic2Command, 0xA0);
    initPort8BitSlow(&myPic2Data   , 0xA1);

    interruptManager.pic1Command = myPic1Command;
    interruptManager.pic1Data    = myPic1Data;
    interruptManager.pic2Command = myPic2Command;
    interruptManager.pic2Data    = myPic2Data;

    writePort8BitSlow(&interruptManager.pic1Command, 0x11);
    writePort8BitSlow(&interruptManager.pic2Command, 0x11);


    writePort8BitSlow(&interruptManager.pic1Data   , 0x20);
    writePort8BitSlow(&interruptManager.pic2Data   , 0x28);

    writePort8BitSlow(&interruptManager.pic1Data   , 0x04);
    writePort8BitSlow(&interruptManager.pic2Data   , 0x02);

    writePort8BitSlow(&interruptManager.pic1Data   , 0x01);
    writePort8BitSlow(&interruptManager.pic2Data   , 0x01);

    writePort8BitSlow(&interruptManager.pic1Data   , 0x00);
    writePort8BitSlow(&interruptManager.pic2Data   , 0x00);


    uint16_t codeSegment = getCodeSegmentSelector(gdt);
    const uint8_t IDT_INTERRUPT_GATE = 0xE;

    for(uint16_t i = 0; i < 256 ; i ++) {
        // ?
        setInterruptDescriptorTableEntry(i, codeSegment, &IgnoreInterruptRequest, 0, IDT_INTERRUPT_GATE);
    }
    setInterruptDescriptorTableEntry(0x20, codeSegment, &HandleInterruptRequest0x00, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(0x21, codeSegment, &HandleInterruptRequest0x01, 0, IDT_INTERRUPT_GATE);

    // ?
    interruptDescriptorTablePointer idt;
    idt.size = 256 * sizeof(GateDescriptor) - 1;
    idt.base = (uint32_t) interruptDescriptorTable;

    asm volatile("lidt %0" : : "m" (idt));
}

void activate() {
    asm("cli");
}

void setInterruptDescriptorTableEntry(uint8_t interrupt_number, uint16_t codeSegmentSelectorOffset, void (*handler)(), uint8_t descriptorPrivilegeLevel, uint8_t descriptorType) {
    const uint8_t IDT_DESC = 0x80;

    interruptDescriptorTable[interrupt_number].handlerAddressLow = ((uint32_t) handler) & 0xFFFF;
    interruptDescriptorTable[interrupt_number].handlerAddressHigh = (((uint32_t) handler) >> 16) & 0xFFFF;
    interruptDescriptorTable[interrupt_number].gdtCodeSegmentSelector = codeSegmentSelectorOffset;
    interruptDescriptorTable[interrupt_number].reservedByte = IDT_DESC | descriptorType | ((descriptorPrivilegeLevel & 3) << 5);
    interruptDescriptorTable[interrupt_number].reservedByte = 0;
}

uint32_t handleInterrupt(uint8_t interrupt_number, uint32_t esp) {
    println("INTERRUPT ");
    return esp;
}
