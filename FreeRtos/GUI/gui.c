#include "gui.h"
#include "lcd.h"
#include "pic.h"
#include "key.h"
#include "delay.h"

//设置
void main_manu_1()
{
	 LCD_Fill(0,0,LCD_W,LCD_H,WHITE);
	 LCD_ShowString(185,40,"Setting",BLACK,WHITE,32,0);
   LCD_ShowPicture(160,80,160,160,gImage_2);
}

//数据
void main_manu_2()
{
	 LCD_Fill(0,0,LCD_W,LCD_H,WHITE);
	 LCD_ShowString(205,40,"Data",BLACK,WHITE,32,0); 
   LCD_ShowPicture(160,80,160,160,gImage_3);
}

//训练
void main_manu_3()
{
	 LCD_Fill(0,0,LCD_W,LCD_H,WHITE);
	 LCD_ShowString(200,40,"Train",BLACK,WHITE,32,0);
   LCD_ShowPicture(160,80,160,160,gImage_4);
}


//训练界面动画设计
void train_move(int i)
{
   LCD_Fill(0,0,99,320,WHITE);
	 Draw_Circle(85,75+i*40,6,BLACK);
}

void adjust_background_light()
{
	 char flag;
	 static int light=500;
   LCD_Fill(0,0,LCD_W,LCD_H,WHITE);
	 LCD_ShowString(200,40,"adjust",BLACK,WHITE,32,0);
   LCD_ShowString(185,80,"light=",BLACK,WHITE,32,0);
   LCD_ShowIntNum(285,80,light/100-1,1,BLACK,WHITE,32);//显示整数变量	
   LCD_DrawRectangle(80, 120, 240, 160,BLACK);
	
   while(1)
   {
			  flag=KEY_Scan(0);
		    delay_ms(1000);
			  if(flag=='1')   //调节光亮
        {    
					   if(light>=900) light=100;
					   light+=100;
			       TIM_SetCompare1(TIM3,light);	//修改比较值，修改占空比	   
             LCD_ShowString(185,80,"light=",BLACK,WHITE,32,0);
             LCD_ShowIntNum(285,80,light/100-1,1,BLACK,WHITE,32);//显示整数变量
           	 LCD_Fill(80,120,480,320,WHITE);			
             LCD_DrawRectangle(80, 120, 120+(light/100-2)*40, 160,BLACK);//在指定位置画一个矩形	40*8				
				}
				else if(flag=='2')  //返回上一级界面
        {
              break;
				}
			 
	 }	 
}

//




