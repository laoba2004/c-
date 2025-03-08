
#ifndef __UART_H__
#define __UART_H__


//6818开发板上对应的串口的路径名 
#define COM2	"/dev/ttySAC1"
#define COM3	"/dev/ttySAC2"
#define COM4	"/dev/ttySAC3"			


int init_serial(const char *file, int baudrate);

int get_MQ2();



#endif

