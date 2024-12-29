/**
 * Author: turrnut
 * Copyright © turrnut under the GNU General Public License Version 3
 *
 * gdt.c
 */
#include <stdint.h>
#include "gdt.h"
#include "stdtypes.h"

void initTable(GlobalDescriptorTable* gdt) {
    gdt->emptySegmentSelector = createSegment(0, 0, 0);
    gdt->unusedSegmentSelector = createSegment(0, 0, 0);
    gdt->codeSegmentSelector = createSegment(0, 0xFFFFF, 0x9A); // code: execute/read
    gdt->dataSegmentSelector = createSegment(0, 0xFFFFF, 0x92); // data: read/write

    uint64_t gdt_ptr[2];
    gdt_ptr[0] = ((uint64_t)(sizeof(GlobalDescriptorTable) - 1)) | ((uint64_t)(uintptr_t)gdt << 16);
    gdt_ptr[1] = (uintptr_t)gdt >> 16;

    asm volatile("lgdt (%0)" : : "r"(gdt_ptr) : "memory");
}

uint16_t getCodeSegmentSelector(GlobalDescriptorTable* gdt) {
    return (uint16_t)((uintptr_t)&gdt->codeSegmentSelector - (uintptr_t)gdt);
}

uint16_t getDataSegmentSelector(GlobalDescriptorTable* gdt) {
    return (uint16_t)((uintptr_t)&gdt->dataSegmentSelector - (uintptr_t)gdt);
}

SegmentDescriptor createSegment(uint32_t base, uint32_t limit, uint8_t flags) {
    SegmentDescriptor seg = {0};

    if (limit > 0xFFFFF) {
        limit >>= 12; // shift for granularity
        seg.upper_flags_limit |= 0xC0; // granularity
    } else {
        seg.upper_flags_limit |= 0x40; // no granularity
    }

    seg.lower_limit = limit & 0xFFFF;
    seg.lower_base = base & 0xFFFF;
    seg.upper_base = (base >> 16) & 0xFF;
    seg.type = flags;
    seg.upper_flags_limit |= (limit >> 16) & 0x0F;
    seg.half_base = (base >> 24) & 0xFF;

    return seg;
}

uint32_t getBase(SegmentDescriptor* seg) {
    return (seg->half_base << 24) | (seg->upper_base << 16) | seg->lower_base;
}

uint32_t getLimit(SegmentDescriptor* seg) {
    uint32_t limit = (seg->upper_flags_limit & 0x0F) << 16 | seg->lower_limit;
    if (seg->upper_flags_limit & 0xC0) {
        limit = (limit << 12) | 0xFFF; // adjust for granularity
    }
    return limit;
}