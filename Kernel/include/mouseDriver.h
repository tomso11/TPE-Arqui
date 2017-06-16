#ifndef _INTERRUPTIONS_H_
#define _INTERRUPTIONS_H_

void initialize_Mouse(void);
void mouse_wait(unsigned char type);
void mouse_write(unsigned char a_write);
unsigned char mouse_read();
void mouse_handler(void);
void mouse_Handler();
void mouse_handle();


#endif


#define PACKETS_IN_PIPE 1024
#define DISCARD_POINT 32

#define MOUSE_PORT   0x60
#define MOUSE_STATUS 0x64
#define MOUSE_ABIT   0x02
#define MOUSE_BBIT   0x01
#define MOUSE_WRITE  0xD4
#define MOUSE_F_BIT  0x20
#define MOUSE_V_BIT  0x08
#define MOUSE_MAGIC 0xFEED1234


typedef enum {
	LEFT_CLICK   = 0x01,
	RIGHT_CLICK  = 0x02,
	MIDDLE_CLICK = 0x04
} mouse_click_t;

typedef struct {
	uint32_t magic;
	int8_t x_difference;
	int8_t y_difference;
	mouse_click_t buttons;
} mouse_device_packet_t;
