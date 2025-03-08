#include "first_window.h"
#include "beep.h"
#include "word.h"
#include "bmp.h"
#include "lcd.h"
#include "uart.h"
#include "touch.h"
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
void* thread_function(void* arg) {
	
}

int first_page(){
	
	int fd=lcd_init();
	
	//show_bmp(0,0,"main.bmp");
	
	//get_GY39(COM3);
	return 0;
}
