#include "uart.h"
#include "beep.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>
#include <termios.h> 
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>

#define COM2  "/dev/ttySAC1"

#define COM3  "/dev/ttySAC2"





// 串口初始化函数
int init_serial(const char *file, int baudrate)
{ 
    int fd;
    
    fd = open(file, O_RDWR);
    if (fd == -1)
    {
        perror("open device error:");
        return -1;
    }

    struct termios myserial;
    // 清空结构体
    memset(&myserial, 0, sizeof(myserial));
    // O_RDWR			   
    myserial.c_cflag |= (CLOCAL | CREAD);
    // 设置控制模式状态，本地连接，接受使能
    // 设置 数据位
    myserial.c_cflag &= ~CSIZE;   // 清空数据位
    myserial.c_cflag &= ~CRTSCTS; // 无硬件流控制
    myserial.c_cflag |= CS8;      // 数据位:8

    myserial.c_cflag &= ~CSTOPB;  // 1位停止位
    myserial.c_cflag &= ~PARENB;  // 不要校验
    // myserial.c_iflag |= IGNPAR;  // 不要校验
    // myserial.c_oflag = 0;  // 输入模式
    // myserial.c_lflag = 0;  // 不激活终端模式

    switch (baudrate)
    {
        case 9600:
            cfsetospeed(&myserial, B9600);    // 设置波特率
            cfsetispeed(&myserial, B9600);
            break;
        case 115200:
            cfsetospeed(&myserial, B115200);  // 设置波特率
            cfsetispeed(&myserial, B115200);
            break;
        case 19200:
            cfsetospeed(&myserial, B19200);  // 设置波特率
            cfsetispeed(&myserial, B19200);
            break;
    }
    
    /* 刷新输出队列,清除正接受的数据 */
    tcflush(fd, TCIFLUSH);

    /* 改变配置 */
    tcsetattr(fd, TCSANOW, &myserial);

    return fd;
}


int get_MQ2()
{
    // 1. 打开串口文件，并初始化
    int fd = init_serial(COM2, 9600);
    if (fd == -1) {
        perror("open error");
        return -1;
    }

    // 2. 发送命令
    while(1){
    unsigned char command[9] = {0xFF, 0x01, 0x86, 0, 0, 0, 0, 0, 0x79};
    ssize_t sent_bytes = write(fd, command, sizeof(command));
    if (sent_bytes == -1) {
        perror("write error");
        close(fd);
        return -1;
    }

    // 3. 接收数据
    unsigned char buffer[9];
    ssize_t total_read = 0;
    int retries = 3;

    while (total_read < sizeof(buffer) && retries-- > 0) {
        ssize_t n = read(fd, buffer + total_read, sizeof(buffer) - total_read);
        if (n < 0) {
            perror("read error");
            close(fd);
            return -1;
        } else if (n == 0) {
            printf("Timeout, retrying...\n");
            usleep(500000); 
            continue;
        }
        total_read += n;
        printf("Read %zd bytes, total: %zd bytes\n", n, total_read); 
    }

    if (total_read != sizeof(buffer)) {
        fprintf(stderr, "Incomplete response: %zd bytes\n", total_read);
        close(fd);
        return -1;
    }

    
    int mq2_value = 0;
   
    mq2_value = (buffer[2] << 8) | buffer[3];
    printf("MQ2 Sensor Value: %d\n", mq2_value);

   
    if (mq2_value > 120) {
        printf("Warning: High smoke detected!\n");
		
		beep_led_ctl(LEDall, 1);
		for(int k=0;k<10;k++)
		{
		beep_led_ctl(BEEP, 1);
		beep_led_ctl(BEEP, 0);
		}
    }
	else{beep_led_ctl(LEDall, 0);

	beep_led_ctl(BEEP, 0);}
    	}
    // 5. 关闭串口文件
    close(fd);

    return 0;
}
