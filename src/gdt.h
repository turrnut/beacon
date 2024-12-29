#pragma once
#include "stdtypes.h"

typedef struct {
    uint16_t lower_limit;
    uint16_t lower_base;
    uint8_t upper_base;
    uint8_t type; // access flags
    uint8_t upper_flags_limit; // granularity + upper limit bits
    uint8_t half_base; // top 8 bits of base address
} __attribute__((packed)) SegmentDescriptor;

typedef struct {
    SegmentDescriptor emptySegmentSelector;
    SegmentDescriptor unusedSegmentSelector;
    SegmentDescriptor codeSegmentSelector;
    SegmentDescriptor dataSegmentSelector;
} __attribute__((packed)) GlobalDescriptorTable;

void initTable(GlobalDescriptorTable* gdt);
uint16_t getCodeSegmentSelector(GlobalDescriptorTable* gdt);
uint16_t getDataSegmentSelector(GlobalDescriptorTable* gdt);
SegmentDescriptor createSegment(uint32_t base, uint32_t limit, uint8_t flags);
uint32_t getBase(SegmentDescriptor* seg);
uint32_t getLimit(SegmentDescriptor* seg);