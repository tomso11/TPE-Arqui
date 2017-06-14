#ifndef _INTERRUPTIONS_H_
#define _INTERRUPTIONS_H_

void initialize_Mouse(void);
void mouse_wait(unsigned char type);
void mouse_write(unsigned char a_write);
unsigned char mouse_read();
void mouse_handler(void);

#endif