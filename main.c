#include "bmp.h"// p3/main.c
#include "first_window.h"
#include "beep.h"
#include <pthread.h>

#include "lcd.h"
#include "uart.h"
#include "touch.h"
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
pthread_t thread_1,thread_2,thread_3;

void go_where();

void* thread_function1(void* arg){
	unsigned char cmd1[3] = { 0xA5, 0x83, 0x26 };

	get_GY39(COM3,cmd1);
	return NULL;
	
}


void* thread_function2(void* arg){
	go_where();
	return NULL;
	
}
void* thread_function3(void* arg){
	get_MQ2();
	return NULL;
	
}
void go_where(){
	int light=0;
	
	while(1)
	{
		int *locate=get_touch();
		if(light==0)beep_led_ctl(LED8, 1);
		if(locate[0]>=691&&locate[0]<=781&&locate[1]>=0&&locate[1]<=110)
		{	printf("%d",light);
			if(light==0)
				{beep_led_ctl(LED8, 1);light=1;}
			else
				{beep_led_ctl(LED8, 0);light=0;}
		}
		if(locate[0]>=691&&locate[0]<=781&&locate[1]>=111&&locate[1]<=195)
		{
			char * bmpname1[] = { "music1.bmp","1.bmp","2.bmp","3.bmp"  };
			int len1=sizeof(bmpname1)/sizeof(bmpname1[0]);
			
			pthread_cancel(thread_1);  // 取消线程 1
            pthread_cancel(thread_3); 
			
			cool_album(bmpname1,len1);
		}
		if(locate[0]>=691&&locate[0]<=781&&locate[1]>=196&&locate[1]<=293)
		{
			char * bmpname2[] = { "music1.bmp","music2.bmp"  };
			int len2=sizeof(bmpname2)/sizeof(bmpname2[0]);
			pthread_cancel(thread_1);  // 取消线程 1
            pthread_cancel(thread_3); 
			music_cast(bmpname2, len2);
		}
	}
}

int main()      //程序的入口
{	

	if (lcd_init() == -1) 
		{
			return -1;
		}
	
	
	int result1,result2,result3;
	show_bmp(0,0,"main.bmp");
	//get_MQ2();
	int fd=lcd_init();
	result1 = pthread_create(&thread_1, NULL, thread_function1, NULL);
    if (result1 != 0) {
        perror("pthread_create");
        return 1;
    }

	result2 = pthread_create(&thread_2, NULL, thread_function2, NULL);
    if (result2 != 0) {
        perror("pthread_create");
        return 1;
    }
	result3 = pthread_create(&thread_3, NULL, thread_function3, NULL);
    if (result3 != 0) {
        perror("pthread_create");
        return 1;
    }

	result1 = pthread_join(thread_1, NULL);
   if (result1 != 0) {
	   perror("pthread_join");
	   return 1;
   }

	   result2 = pthread_join(thread_2, NULL);
   if (result2 != 0) {
	   perror("pthread_join");
	   return 1;
   }

	   result3 = pthread_join(thread_3, NULL);
   if (result3 != 0) {
	   perror("pthread_join");
	   return 1;
   }




	lcd_close();
    return 0;
}
