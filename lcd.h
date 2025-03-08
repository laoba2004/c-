
#ifndef __LCD_H__
#define __LCD_H__ 


//包含宏定义、函数声明等内容 

int lcd_init();
void lcd_close();
void display_point( int x, int y, int color );
void show_pure_color( int color );

void display_Rectangle( int x0, int y0, int w, int h, int color );
void display_Circle( int x0, int y0, int r, int color );
void display_pentastar( int x0, int y0,int r,int color);



#endif


