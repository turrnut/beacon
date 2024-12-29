#include <stdint.h>
#include "pic.h"
// PIC ports
#define PIC1         0x20 // Master PIC
#define PIC2         0xA0 // Slave PIC
#define PIC1_COMMAND PIC1
#define PIC1_DATA    (PIC1 + 1)
#define PIC2_COMMAND PIC2
#define PIC2_DATA    (PIC2 + 1)

#define ICW1_INIT    0x10 // Initialization Control Word 1
#define ICW1_ICW4    0x01 // ICW4 needed
#define ICW4_8086    0x01 // 8086/88 mode

// Write a byte to an I/O port
static inline void outb(uint16_t port, uint8_t value) {
    asm volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

// Read a byte from an I/O port
static inline uint8_t inb(uint16_t port) {
    uint8_t value;
    asm volatile ("inb %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}

void pic_remap() {
    uint8_t mask1 = inb(0x21);
    uint8_t mask2 = inb(0xA1);

    outb(0x20, 0x11); // Start initialization sequence (ICW1)
    outb(0xA0, 0x11);
    outb(0x21, 0x20); // Master PIC vector offset
    outb(0xA1, 0x28); // Slave PIC vector offset
    outb(0x21, 0x04); // Tell Master PIC about Slave PIC at IRQ2
    outb(0xA1, 0x02); // Tell Slave PIC its cascade identity
    outb(0x21, 0x01); // Set PICs to 8086/88 mode
    outb(0xA1, 0x01);

    outb(0x21, mask1); // Restore saved masks
    outb(0xA1, mask2);
}