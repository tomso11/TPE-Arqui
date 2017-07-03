#include <interruptions.h>
#include <driverVideo.h>
#include <driverKeyboard.h>
#include "mouseDriver.h"
#include "naiveConsole.h"


uint8_t mouse_cycle = 0;
int8_t  mouse_byte[3];

static int x=10;
static int y=10;
static int xo=0;
static int yo=0;
static int xf=0;
static int yf=0;
static int select_flag=0;

void mouse_wait(unsigned char type){
  unsigned int time_out=100000;
  if(type==0)
  {
    while(time_out--) //Data
    {
      if((read_port(0x64) & 1)==1)
      {
        return;
      }
    }
    return;
  }
  else
  {
    while(time_out--) //Signal
    {
      if((read_port(0x64) & 2)==0)
      {
        return;
      }
    }
    return;
  }
}

void mouse_write(unsigned char a_write) {
	//Wait to be able to send a command
	mouse_wait(1);
	//Tell the mouse we are sending a command
	write_port(0x64, 0xD4);
	//Wait for the final part
	mouse_wait(1);
	//Finally write
	write_port(0x60, a_write);
}

unsigned char mouse_read() {
	//Get response from mouse
	mouse_wait(0);
	return read_port(0x60);
}

void initialize_Mouse() {
	//cuando prendo la computadora, el mouse esta desactivado
	//y la informacion que proviene de el no va a generar interrupciones
	//entonces debo activarlo
	char status;

	mouse_wait(1); //espera hasta que el mouse pueda enviar/recibir comandos/data
	write_port(0x64,0xA8); //activo el mouse

	mouse_wait(1);
	write_port(0x64,0x20); //modifico el byte del compaq status para habilitar las interrupciones

	mouse_wait(0);
	status=(read_port(0x60) | 2); //sets bit number 1 (value=2) que es responsable de activar la IRQ12

	mouse_wait(1);
	write_port(0x64, 0x60);

	mouse_wait(1);
	write_port(0x60, status); ////mando el byte modificado al port 0x60

	//hago el setup del mouse
	mouse_write(0xF6); //le digo al mouse que use los default settings
	mouse_read();

	mouse_write(0xF4); //habilito el mouse
	mouse_read();
}








void mouse_handle() {
  uint8_t status = read_port(MOUSE_STATUS);
  while (status & MOUSE_BBIT) {
    int8_t mouse_in = read_port(MOUSE_PORT);
    if (status & MOUSE_F_BIT) {
      switch (mouse_cycle) {
        case 0:
          mouse_byte[0] = mouse_in;
          if (!(mouse_in & MOUSE_V_BIT)) return;
          ++mouse_cycle;
          break;
        case 1:
          mouse_byte[1] = mouse_in;
          ++mouse_cycle;
          break;
        case 2:
          mouse_byte[2] = mouse_in;
          /* We now have a full mouse packet ready to use */
          if (mouse_byte[0] & 0x80 || mouse_byte[0] & 0x40) {
            /* x/y overflow? bad packet! */
            mouse_cycle=4;
            break;
          }
          mouse_device_packet_t packet;
          packet.magic = MOUSE_MAGIC;
          packet.x_difference = cap_movement(mouse_byte[1]);
          packet.y_difference = cap_movement(mouse_byte[2]);
          // ncPrintDec(packet.x_difference);
          // printChar('\n');
          // ncPrintDec(packet.y_difference);
          // printChar('\n');
          packet.buttons = 0;
          if (mouse_byte[0] & 0x01) {
            packet.buttons |= LEFT_CLICK;
            if(select_flag){
                if( yf<yo || ((yf==yo) && (xf<xo)) ){
                  undoSelection( yf, xf,yo, xo);

                }else{
                  undoSelection(yo, xo, yf, xf);
                }
                xf=x;
                yf=y;
            }else{
                select_flag=1;
                xo=x;
                yo=y;
                xf=x;
                yf=y;
            }
            if(yf<yo ||( (yf==yo) &&(xf<xo))){ // solo se puede seleccionar hacia la izq y hacia arriba
                selection( yf, xf,yo, xo);  
            }else{
                selection(yo, xo, yf, xf);
            }}else{
              if(select_flag){
                if(yf<yo || ((yf==yo) &&(xf<xo))){
                  undoSelection( yf, xf,yo, xo);
                }else{
                  undoSelection(yo, xo, yf, xf);
                }
                select_flag=0;
                //updateScreen();
              }
            }
            if (mouse_byte[0] & 0x02) {
              packet.buttons |= RIGHT_CLICK;
            }
            if (mouse_byte[0] & 0x04) {
            packet.buttons |= MIDDLE_CLICK;
            }
            if ((mouse_byte[0] & 0x20)==0x20)
              packet.y_difference=-1*(packet.y_difference); //delta-y is a negative value
            if ((mouse_byte[0] & 0x10)==0x10)
              packet.x_difference =-1*(packet.x_difference); //delta-x is a negative value
            mouse_packet_handler(packet);

            // //udrawMouse(y,x);
            // if(x+packet.x_difference/10 < 80 && x+packet.x_difference/10>=0)
            //   x+=packet.x_difference/10;
            // if(y-packet.y_difference/10 < 25 && y-packet.y_difference/10>=0)
            //   y-=packet.y_difference/10;
            // //drawMouse(y,x);
            // update_cursor(x,y);
            mouse_cycle = 0;
        }
      }
    
    
    status = read_port(MOUSE_STATUS);
  }
}

void mouse_packet_handler(mouse_device_packet_t packet){
  update_cursor(packet.x_difference,packet.y_difference);
  if(packet.buttons == LEFT_CLICK){
    //printString("Left Button");
  }
  if(packet.buttons == RIGHT_CLICK){
    //printString("Right Button");
  }
  if(packet.buttons == MIDDLE_CLICK){
    //printString("Middle Button");
  }
}

int cap_movement(int number){
  int n=0;
  if(number > 100){
    n++;
    number=number-100;
  }
  return n;
}