/**
 * Author: turrnut
 * Copyright © turrnut under the GNU General Public License Version 3
 *
 * gdt.h
 */

#pragma once
#ifndef global_descriptor_table_header
#define global_descriptor_table_header

#include "stdtypes.h"

typedef struct {
    uint16_t lower_limit;
    uint16_t lower_base;
    uint8_t upper_base;
    uint8_t type;
    uint8_t upper_flags_limit;
    uint8_t half_base;
}  __attribute__((packed)) SegmentDescriptor;

typedef struct {
    SegmentDescriptor emptySegmentSelector;
    SegmentDescriptor unusedSegmentSelector;
    SegmentDescriptor codeSegmentSelector;
    SegmentDescriptor dataSegmentSelector;
} GlobalDescriptorTable;

// GlobalDescriptorTable functions

void initTable(GlobalDescriptorTable* gdt);
uint16_t getCodeSegmentSelector(GlobalDescriptorTable* gdt);
uint16_t getDataSegmentSelector(GlobalDescriptorTable* gdt);

// SegmentDescriptor functions

SegmentDescriptor createSegment(GlobalDescriptorTable* gdt, uint32_t base, uint32_t limit, uint8_t flags);
uint32_t getBase(SegmentDescriptor *seg);
uint32_t getLimit(SegmentDescriptor *seg);

#endif
