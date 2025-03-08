#include "bmp.h"
#include <errno.h>
#include "uart.h"
#include "beep.h"
#include "word.h"
#include "lcd.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>

//蜂鸣器和led灯的控制 
int beep_led_ctl( char * filename , int OnOff )
{
    //1.打开文件 
    int fd = open( filename, O_RDWR );
    if( fd == -1 )
    {
        perror("open led or bepp error ");
        return -1;
    }

    //2.操作 
        //写1   点亮
        //写0   熄灭
    write( fd, &OnOff, 4 );

    //3.关闭文件 
    close( fd );
    return 0;
}

int get_GY39(char * filename ,unsigned char *cmd)
{
    //1.打开串口文件，并初始化 
    int fd = init_serial(filename,9600 );
    if (fd == -1) {
        return -1;
    }
	 
    // 计算校验和
    cmd[2] = (cmd[0] + cmd[1]) & 0xFF;

    unsigned char buf[256] = {0};  // 扩大缓冲区
    int index = 0;

    //2.发送命令 
    ssize_t bytes_written = write(fd, cmd, sizeof(cmd));
    if (bytes_written != sizeof(cmd)) {
        perror("write error");
        close(fd);
        return -1;
    }
    printf("Command sent successfully\n");

    while (1)
    { 
        //3.接收数据 
        size_t size = read(fd, buf + index, sizeof(buf) - index);
        if (size == -1)
        {
            perror("read error");
            close(fd);
            return -1;
        }
        index += size;

      
        int i = 0;
        while (i < index - 1)
        {
            if (buf[i] == 0x5A && buf[i + 1] == 0x5A)
            {
                int frame_len = 0;
                if (buf[i + 2] == 0x15) {
                    frame_len = 9;
                } else if (buf[i + 2] == 0x45) {
                    frame_len = 15;
                }

                if (i + frame_len <= index)
                {
                   
                    printf("test\n");

                  
                    if (buf[i + 2] == 0x15)
                    {show_bmp(0,0,"main.bmp");
                      
                        unsigned int lux = (buf[i + 4] << 24) | (buf[i + 5] << 16) | (buf[i + 6] << 8) | buf[i + 7];
                        lux /= 100;
                        printf("light: %d lux\n", (int)lux);
						display_Rectangle(490, 440, 20, 20, 0xFFFFFF);
                        show_number(510,440, (int)lux, 0xFF00F0);
                    }

                  
                    if (buf[i + 2] == 0x45)
                    {
                        // 温度
                        unsigned short temperature = (buf[i + 4] << 8) | buf[i + 5];
                        temperature /= 100;
                        printf("temperature: %d°C\n", (int)temperature);
						display_Rectangle(190, 310, 20, 20, 0xFFFFFF);
                        show_number(210,310, (int)temperature, 0xFF00F0);

                        // 气压
                        unsigned int pressure = (buf[i + 6] << 24) | (buf[i + 7] << 16) | (buf[i + 8] << 8) | buf[i + 9];
                        pressure /= 100;
                        printf("press: %dpa\n", (int)pressure);
						display_Rectangle(210, 435, 20, 20, 0xFFFFFF);
                        show_number(230,435, (int)(pressure/1000), 0xFF00F0);

                        // 湿度
                        unsigned short humidity = (buf[i + 10] << 8) | buf[i + 11];
                        humidity /= 100;
                        printf("dirt: %d\n", (int)humidity);
						display_Rectangle(450, 310, 20, 20, 0xFFFFFF);
                        show_number(470,310, (int)humidity, 0xFF00F0);

                        // 海拔
                        unsigned short altitude = (buf[i + 12] << 8) | buf[i + 13];
                        printf("altitute: %d m\n", (int)(altitude/1000));
						display_Rectangle(730, 310, 20,20, 0xFFFFFF);
                        show_number(750,310, (int)(altitude/1000), 0xFF00F0);
                    }

                    
                    i += frame_len;
                } else {
                    
                    break;
                }
            } else {
                i++;
            }
        }

        // 移除已经处理的数据
        if (i > 0) {
            for (int j = 0; j < index - i; j++) {
                buf[j] = buf[i + j];
            }
            index -= i;
        }
    }

    
    close(fd);
    return 0;
}
