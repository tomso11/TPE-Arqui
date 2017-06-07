#ifndef _INTERRUPTIONS_H_
#define _INTERRUPTIONS_H_

#include <stdint.h>

void iSetHandler(int index, uint64_t handler);
void sti();
void irq0Handler();
void irq1Handler();
void irq12Handler();
void setPicMaster(uint16_t);
void setPicSlave(uint16_t);
void keyboard_init(void);
char read_port(unsigned short port);
void write_port(unsigned short port, unsigned char data);
void keyboard_handler(void);
void mouse_handler(void);
void initialize_Mouse(void);

#endif