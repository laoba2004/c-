
#ifndef __BEEP_H__
#define __BEEP_H__


#define LED7   "/sys/kernel/gec_ctrl/led_d7"
#define LED8   "/sys/kernel/gec_ctrl/led_d8"
#define LED9   "/sys/kernel/gec_ctrl/led_d9"
#define LED10  "/sys/kernel/gec_ctrl/led_d10"
#define LEDall "/sys/kernel/gec_ctrl/led_all"
#define BEEP   "/sys/kernel/gec_ctrl/beep"



int beep_led_ctl( char * filename , int OnOff );

int get_GY39(char * filename,unsigned char *cmd);

#endif

