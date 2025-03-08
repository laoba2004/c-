
#include "bmp.h"
#include "touch.h"
#include "lcd.h"
#include "touch.h"
#include <stdio.h>


#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>


//显示bmp图片
int show_bmp( int x0, int y0, char * filename )
{
    //1.打开文件 
    int fd = open( filename , O_RDWR );
    if( fd == -1 )
    {
        perror("open bmp error ");
        return -1;
    }

    //2.读取数据 
        //读取宽度、高度、色深
        int width = 0;
        lseek( fd, 0x12, SEEK_SET );
        read( fd, &width, 4 );

        int height = 0;
        lseek( fd, 0x16, SEEK_SET );
        read( fd, &height, 4 );

        short depth = 0;
        lseek( fd, 0x1C, SEEK_SET );
        read( fd, &depth, 2 );
            
        //读取像素点的颜色数据 
        int line_size = 0;  //一行实际的字节数
        int laizi = 0;      //填充的字节数 
        laizi = 4 - ( abs(width) * (depth/8) ) % 4 ;
        if( laizi == 4 )
        {
            laizi = 0;
        }

        printf("w = %d, h = %d, d = %d, laizi = %d\n", width, height, depth, laizi );

        //一行实际的字节数 = 有效的字节数 + 填充字节数 
        line_size = abs(width) * (depth/8)  + laizi;

        //一次性把整个像素数据全部读取出来 
        unsigned char buf[ line_size * abs(height) ];       //颜色分量值bgr
        lseek( fd, 0x36, SEEK_SET );
        read( fd, buf, line_size * abs(height) );

    //3.显示 
        int i, j;
        int color;
        unsigned char a,r,g,b;
        int num = 0;

        for( j=0; j<abs(height); j++ )
        {
            for( i=0; i<abs(width); i++ )
            {
                //合成颜色值
                b = buf[num++];
                g = buf[num++];
                r = buf[num++];
                if( depth == 32 )
                {
                    a = buf[num++];
                }
                else 
                {
                    a = 0;
                }
                color = (a<<24) | (r<<16) | (g<<8) | b;

                //画点 
                display_point( width>0 ? i+x0 : abs(width)-1-i+x0 , 
                                height>0 ? abs(height)-1-j+y0 : j+y0 , 
                                color );
            }

            //跳过无效填充数据 
            num = num + laizi;

        }

    //4.关闭文件 
    close( fd );
}



//保存图片的名字 



//电子相册
int cool_album(char * bmpname[],int len)
{
	int i=0;
	int *locate;
	printf("%d\n", len);
    while( 1 )
    {
        //显示图片 
        
		if (i>=len)
		{
			i=0;
		}
		if(i<0)
		{
			i=len-1;
		}
		show_bmp_Rectangle( 0,0 , bmpname[i]);
		show_bmp(750,440, "back.bmp");
        //获取手指滑动的方向 
        locate=get_touch();
		if(locate[0]>=940&&locate[0]<=1024&&locate[1]>=550&&locate[1]<=640)
		{
			system("./p10");
		}
		int direct=get_direction();
		if (direct==1||direct==3)
			{
			i++;
		}
		else
			{i--;}
        //根据方向进行切换

    }
	return 0;
}

int music_cast(char * bmpname[],int len)
{
	int i=0;
	show_pure_color(0xFFFFFF);
	show_bmp(0, 240, "main1.bmp");
	int pause=0;
	int *locate;
	printf("%d\n", len);
    while( 1 )
    {   show_bmp(750,440, "back.bmp");
        //显示图片 
        
		if (i>=len)
		{
			i=0;
		}
		if(i<0)
		{
			i=len-1;
		}
		show_bmp( 0,0 , bmpname[i]);
		if(pause==0){
			printf("music number%d",i);
			
		}
		int* touch=get_touch();
		printf("%d,,,,",touch[0]);
		printf("%d",touch[1]);
		if(touch[0]>=409&&touch[0]<=588&&touch[1]>=408&&touch[1]<=518){
			
			if(pause==0){
				pause=1;
				show_bmp(0, 240, "main1.bmp");
				show_bmp(750,440, "back.bmp");
        //获取手指滑动的方向 
        
				printf("start");
				}
			else{ 
				pause=0;
				show_bmp(320, 308, "start.bmp");
				printf("pause");
				}
			
		}
		
		else{
			locate=get_touch();
			if(locate[0]>=940&&locate[0]<=1024&&locate[1]>=550&&locate[1]<=640)
			{
				system("./p10");
			}
	        //获取手指滑动的方向 
			int direct=get_direction();
			
			if (direct==1||direct==3)
				{
				i++;
			}
			else
				{i--;}
	        //根据方向进行切换

			}

    }
	return 0;
}
int show_bmp_Rectangle(int x0,int y0,char * filename )
{
    //1.打开文件 
    int fd = open( filename , O_RDWR );
	if( fd == -1 )
	{
	perror("open file error "); 
	return -1;    
	}
    //2.读取数据 
        //读取宽度、高度、色深
	int width = 0;
	lseek( fd, 0x12, SEEK_SET );
	read( fd, &width, 4 );

	int height = 0;
	lseek(  fd, 0x16, SEEK_SET );
	read(  fd, &height, 4  );

	short  depth = 0;
	lseek( fd,  0x1C , SEEK_SET );
	read(   fd, &depth, 2 );      
        //读取像素点的颜色数据 

	int line_size = 0;  //一行实际的字节数
	int laizi = 0;      //填充的字节数 
	laizi = 4 - ( abs(width) * (depth/8) ) % 4 ;
	if( laizi == 4 )
	{
	laizi = 0;
	}

	//一行实际的字节数 = 有效的字节数 + 填充字节数 
	line_size = abs(width) * (depth/8)  + laizi;


	//一次性把整个像素数据全部读取出来 
	unsigned char buf[ line_size * abs(height) ];       //颜色分量值bgr
	lseek( fd, 0x36, SEEK_SET );
	read( fd, buf, line_size * abs(height) );

    //3.显示 

	int i, j,t;
	int color[800][480];
	unsigned char a,r,g,b;
	int num = 0;

	for( j=0; j<abs(height); j++ )
	{
		for( i=0; i<abs(width); i++ )
		{
		//合成颜色值
		b = buf[num++];
		g = buf[num++];
		r = buf[num++];
			if( depth == 32 )
			{
			    a = buf[num++];
			}
			else 
			{
			    a = 0;
			}
		color[i][j] = (a<<24) | (r<<16) | (g<<8) | b;

		}
		//跳过无效填充数据 
		num = num + laizi;
	}
	    int x1=400,x2=400,y1=240,y2=240;
	    while(1)
	    {
			if(x1<=x2&&y1<=y2)
		    {
				for(int x=x1;x<x2;x++)
				{
					 for(int y=y1;y<y2;y++)
					{
					//画点 
					display_point(width>0 ? x+x0 : abs(width)-1-x+x0,
					height>0 ? abs(height)-1-y+y0 : y+y0,
					color[x][y]);
					}
					
				}
			}
			x1=x1-5;
			x2=x2+5;
			y1=y1-3;
			y2=y2+3;
		    
		    if(x1<0)
		    {
		    break;
		    }
	   }
	 
    //4.关闭文件 
    close(fd);
 }


