#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "stm32f4xx.h"
#include "led.h"
#include "FreeRTOS.h"
#include "task.h"
#include "lcd.h"
#include "gui.h"
#include "key.h"
#include "pwm.h"


static TaskHandle_t app_task1_handle1 = NULL;
static TaskHandle_t app_task1_handle2 = NULL;
static TaskHandle_t app_task1_handle3 = NULL;
static TaskHandle_t app_task1_handle4 = NULL;


/* 任务1 */ 
static void app_task1(void* pvParameters);  //主界面进程

static void app_task2(void* pvParameters);  //设置进程

static void app_task3(void* pvParameters);  //数据进程

static void app_task4(void* pvParameters);  //训练进程




int main(void)
{
	
	int j,i;
	delay_init(168);
	LED_Init();//LED初始化
	LCD_Init();//LCD初始化
	KEY_Init();//按键初始化
	LED_Init();//初始化LED端口
	TIM3_PWM_Init(1000,840-1);//pwm控制背光大小
	TIM_SetCompare1(TIM3,500);	
	
	/* 创建app_task1任务 */
	xTaskCreate((TaskFunction_t )app_task1,  		/* 任务入口函数 */
			  (const char*    )"app_task1",			/* 任务名字 */
			  (uint16_t       )512,  				/* 任务栈大小 */
			  (void*          )NULL,				/* 任务入口函数参数 */
			  (UBaseType_t    )4, 					/* 任务的优先级 */
			  (TaskHandle_t*  )&app_task1_handle1);	/* 任务控制块指针 */ 
	

	/* 创建app_task2任务 */
	xTaskCreate((TaskFunction_t )app_task2,  		/* 任务入口函数 */
			  (const char*    )"app_task2",			/* 任务名字 */
			  (uint16_t       )512,  				/* 任务栈大小 */
			  (void*          )NULL,				/* 任务入口函数参数 */
			  (UBaseType_t    )3, 					/* 任务的优先级 */
			  (TaskHandle_t*  )&app_task1_handle2);	/* 任务控制块指针 */ 
	
	/* 创建app_task3任务 */
	xTaskCreate((TaskFunction_t )app_task3,  		/* 任务入口函数 */
			  (const char*    )"app_task3",			/* 任务名字 */
			  (uint16_t       )512,  				/* 任务栈大小 */
			  (void*          )NULL,				/* 任务入口函数参数 */
			  (UBaseType_t    )3, 					/* 任务的优先级 */
			  (TaskHandle_t*  )&app_task1_handle3);	/* 任务控制块指针 */ 
	
	/* 创建app_task4任务 */
	xTaskCreate((TaskFunction_t )app_task4,  		/* 任务入口函数 */
			  (const char*    )"app_task4",			/* 任务名字 */
			  (uint16_t       )512,  				/* 任务栈大小 */
			  (void*          )NULL,				/* 任务入口函数参数 */
			  (UBaseType_t    )3, 					/* 任务的优先级 */
			  (TaskHandle_t*  )&app_task1_handle4);	/* 任务控制块指针 */ 
	
  vTaskSuspend(app_task1_handle2);    
  vTaskSuspend(app_task1_handle3);    
  vTaskSuspend(app_task1_handle4);    				
	/* 开启任务调度 */
	vTaskStartScheduler(); 
	
}


static char flag;  //用户操作
static int fuc;   //当前界面

void  init_main()
{
	LCD_Fill(0,0,LCD_W,LCD_H,WHITE);
	fuc=1;
  main_manu_1();
}


//主界面任务——负责完成控制终端的主界面显示
static void app_task1(void* pvParameters)           
{

	init_main();
	flag='0';
	fuc=1;

  //循环判断用户操作
	while(1)
  {
		 LED0=!LED0;
		 delay_ms(1000);
	   flag=KEY_Scan(0);
		
	   if(flag=='1')             //移动图标
     {
			  fuc++;
			  if(fuc>=4) fuc=1;
			  switch(fuc)
        {
					case 1: main_manu_1();break;
					case 2: main_manu_2();break;
					case 3: main_manu_3();break;
				}
			 
		      
		 }else if(flag=='2')        //选择功能
		 {
		      if(fuc==1)
					{
						    vTaskResume(app_task1_handle2);
							  vTaskSuspend(app_task1_handle1);     
						
						    vTaskSuspend(app_task1_handle2);  
						    init_main();
					}else if(fuc==2){
						    vTaskResume(app_task1_handle3);
							  vTaskSuspend(app_task1_handle1);
						
						    vTaskSuspend(app_task1_handle3);
                init_main();						
					}else if(fuc==3){
						    vTaskResume(app_task1_handle4);
							  vTaskSuspend(app_task1_handle1);
						
						    vTaskSuspend(app_task1_handle4);  
						    init_main();
		      }//图像大小为160*160
	    }
  }
	
} 


void  init_setting()
{
    LCD_Fill(0,0,LCD_W,LCD_H,WHITE);
	  
	  LCD_ShowString(100,60,"Background Light",BLACK,WHITE,32,0);//显示字符串
	  LCD_ShowString(100,100,"WIFI",BLACK,WHITE,32,0);//显示字符串
	  LCD_ShowString(100,140,".....",BLACK,WHITE,32,0);//显示字符串
	  LCD_ShowString(100,180,".....",BLACK,WHITE,32,0);//显示字符串
	  LCD_ShowString(100,220,"back",BLACK,WHITE,32,0);//显示字符串
	  train_move(0);
}

//设置
//1、设置背光亮度
//2、设置wifi
static void app_task2(void* pvParameters)  
{
	
	
	  static int fuc=1;//记录当前的选项
	  int i;
    init_setting();
	
	  while(1)
    {
			  flag=KEY_Scan(0);
			  if(flag=='1')
        {
			       fuc++;
			       if(fuc>=6) fuc=1;
					   train_move(fuc-1);			    
				}
				else if(flag=='2')
        {
				     switch(fuc)
             {
							    case 1:adjust_background_light();init_setting();break;//执行相关动作1
							    case 2:break;//执行相关动作2
							    case 3:break;//执行相关动作3
							    case 4:break;//执行相关动作4
							    case 5:vTaskResume(app_task1_handle1);init_setting();break;//返回主页面
						 }
				}
			 
		}
}


//数据
static void app_task3(void* pvParameters)  
{
	  while(1)
    {
        LCD_Fill(0,0,LCD_W,LCD_H,WHITE);
	      LCD_ShowString(100,100,"GUI2",BLACK,WHITE,16,0);//显示字符串
	      delay_ms(1000);
	      flag='3';  
	      vTaskResume(app_task1_handle1);		
		}

}

void  init_train()
{
    LCD_Fill(0,0,LCD_W,LCD_H,WHITE);
	  
	  LCD_ShowString(100,60,"Gesture1",BLACK,WHITE,32,0);//显示字符串
	  LCD_ShowString(100,100,"Gesture2",BLACK,WHITE,32,0);//显示字符串
	  LCD_ShowString(100,140,"Gesture3",BLACK,WHITE,32,0);//显示字符串
	  LCD_ShowString(100,180,"Gesture4",BLACK,WHITE,32,0);//显示字符串
	  LCD_ShowString(100,220,"back",BLACK,WHITE,32,0);//显示字符串
	  train_move(0);
}

//训练
static void app_task4(void* pvParameters)  
{
	
	
	  static int fuc=1;//记录当前的选项
	  int i;
    init_train();
	
	
	  while(1)
    {
			  flag=KEY_Scan(0);
			  if(flag=='1')
        {
			       fuc++;
			       if(fuc>=6) fuc=1;
					   train_move(fuc-1);			    
				}
				else if(flag=='2')
        {
				     switch(fuc)
             {
							    case 1:break;//执行相关动作1
							    case 2:break;//执行相关动作2
							    case 3:break;//执行相关动作3
							    case 4:break;//执行相关动作4
							    case 5:vTaskResume(app_task1_handle1);init_train();break;//返回主页面
						 }
				}
			 
		}
}


/**
*******************下面注释掉的代码是通过 位带 操作实现IO口控制**************************************
	
int main(void)
{ 
 
	delay_init(168);		  //初始化延时函数
	LED_Init();		        //初始化LED端口
  while(1)
	{
     LED0=0;			  //LED0亮
	   LED1=1;				//LED1灭
		 delay_ms(500);
		 LED0=1;				//LED0灭
		 LED1=0;				//LED1亮
		 delay_ms(500);
	 }
}
**************************************************************************************************
 **/	
	
/**
*******************下面注释掉的代码是通过 直接操作寄存器 方式实现IO口控制**************************************
int main(void)
{ 
 
	delay_init(168);		  //初始化延时函数
	LED_Init();		        //初始化LED端口
	while(1)
	{
     GPIOF->BSRRH=GPIO_Pin_9;//LED0亮
	   GPIOF->BSRRL=GPIO_Pin_10;//LED1灭
		 delay_ms(500);
     GPIOF->BSRRL=GPIO_Pin_9;//LED0灭
	   GPIOF->BSRRH=GPIO_Pin_10;//LED1亮
		 delay_ms(500);

	 }
 }	 
**************************************************************************************************
**/	
 



