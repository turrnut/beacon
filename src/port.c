/**
 * Author: turrnut
 * Copyright © turrnut under the GNU General Public License Version 3
 *
 * port.c
 */

#include "port.h"

void initPort8Bit(Port8Bit* port, uint16_t port_number) {
    port->port_number = port_number;
}
void writePort8Bit(Port8Bit* port, uint8_t data) {
    __asm__ volatile("outb %0, %1" : : "a" (data), "Nd" (port->port_number));
}
uint8_t readPort8Bit(Port8Bit* port) {
    uint8_t result;

    __asm__ volatile("inb %1, %0" : "=a" (result) : "Nd" (port->port_number)) ;

    return result;
}




void initPort8BitSlow(Port8BitSlow* port, uint16_t port_number) {
    port->port_number = port_number;
}
void writePort8BitSlow(Port8BitSlow* port, uint8_t data){
    __asm__ volatile("outb %0, %1\njmp 1f\n1: jmp 1f\n1:" : : "a" (data), "Nd" (port->port_number));

}
// same as 8-bit Port's
uint8_t readPort8BitSlow(Port8BitSlow* port) {
    uint8_t result;

    __asm__ volatile("inb %1, %0" : "=a" (result) : "Nd" (port->port_number)) ;

    return result;
}



void initPort16Bit(Port16Bit* port, uint16_t port_number) {
    port->port_number = port_number;
}
void writePort16Bit(Port16Bit* port, uint16_t data) {
    __asm__ volatile("outw %0, %1" : : "a" (data), "Nd" (port->port_number));
}
uint16_t readPort16Bit(Port16Bit* port) {
    uint16_t result;

    __asm__ volatile("inw %1, %0" : "=a" (result) : "Nd" (port->port_number)) ;

    return result;
}



void initPort32Bit(Port32Bit* port, uint16_t port_number) {
    port->port_number = port_number;
}
void writePort32Bit(Port32Bit* port, uint32_t data) {
    __asm__ volatile("outl %0, %1" : : "a" (data), "Nd" (port->port_number));
}
uint32_t readPort32Bit(Port32Bit* port) {
    uint32_t result;

    __asm__ volatile("inl %1, %0" : "=a" (result) : "Nd" (port->port_number)) ;

    return result;
}
