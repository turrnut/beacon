/**
 * Author: turrnut
 * Copyright © turrnut under the GNU General Public License Version 3
 *
 * gdt.c
 */
#ifndef global_descriptor_table
#define global_descriptor_table
#include "gdt.h"

void initTable(GlobalDescriptorTable* gdt) {
    gdt->emptySegmentSelector = createSegment(gdt, 0, 0, 0);
    gdt->unusedSegmentSelector = createSegment(gdt, 0, 0, 0);
    gdt->codeSegmentSelector = createSegment(gdt, 0, 64 * 1024 * 1024, 0x9A);
    gdt->dataSegmentSelector = createSegment(gdt, 0, 64 * 1024 * 1024, 0x92);

    uint32_t a[2];
    a[0] = (uint32_t)&gdt;
    a[1] = sizeof(GlobalDescriptorTable) << 16;

    asm volatile("lgdt (%0)" :: "p"(((uint8_t *)a) + 2));
}


uint16_t getCodeSegmentSelector(GlobalDescriptorTable* gdt) {

    return (uint8_t*)&gdt->codeSegmentSelector - (uint8_t*)gdt;
}

uint16_t getDataSegmentSelector(GlobalDescriptorTable* gdt) {
    return (uint8_t*)&gdt->dataSegmentSelector - (uint8_t*)gdt;
}

SegmentDescriptor createSegment(GlobalDescriptorTable* gdt, uint32_t base, uint32_t limit, uint8_t flags) {
    uint8_t* target = (uint8_t*)gdt;

    if (limit <= 65536)
        target[6] = 0x40;
    else {
        if ((limit & 0xFFF) != 0xFFF)
            limit = (limit >> 12) - 1;
        else
            limit = limit >> 12;
        target[6] = 0xC0;
    }

    target[0] = limit & 0xFF;
    target[1] = (limit >> 8) & 0xFF;
    target[2] = base & 0xFF;
    target[3] = (base >> 8) & 0xFF;
    target[4] = (base >> 16) & 0xFF;
    target[5] = flags;
    target[6] |= (limit >> 16) & 0xFF;
    target[7] = (base >> 24) & 0xFF;
}

uint32_t getBase(SegmentDescriptor *seg) {
    uint8_t* target = (uint8_t*) seg;
    uint32_t result = target[7];
    result = (result << 8) + target[4];
    result = (result << 8) + target[3];
    result = (result << 8) + target[2];
    return result;
}
uint32_t getLimit(SegmentDescriptor *seg) {
    uint8_t* target = (uint8_t*) seg;
    uint32_t result = target[6] & 0xF;
    result = (result << 8) + target[1];
    result = (result << 8) + target[0];
    
    if ((target[6] & 0xC0) == 0xC0)
       result = (result >> 12) | 0xFFF;

    return result;
}

#endif
