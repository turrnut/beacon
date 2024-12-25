/**
 * Author: turrnut
 * Copyrighted © turrnut under the Apache 2.0 license
 *
 * We hope that you will utilize this open-source software
 * responsibly. Please read the LICENSE for comprehensive
 * information on how you can employ it. You have the
 * freedom to distribute and incorporate this file into your
 * project. However, you must indicate any modifications
 * you've made and provide credit to the original author of
 * this file.
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
