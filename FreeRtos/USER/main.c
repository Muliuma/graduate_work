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


/* ����1 */ 
static void app_task1(void* pvParameters);  //���������

static void app_task2(void* pvParameters);  //���ý���

static void app_task3(void* pvParameters);  //���ݽ���

static void app_task4(void* pvParameters);  //ѵ������




int main(void)
{
	
	int j,i;
	delay_init(168);
	LED_Init();//LED��ʼ��
	LCD_Init();//LCD��ʼ��
	KEY_Init();//������ʼ��
	LED_Init();//��ʼ��LED�˿�
	TIM3_PWM_Init(1000,840-1);//pwm���Ʊ����С
	TIM_SetCompare1(TIM3,500);	
	
	/* ����app_task1���� */
	xTaskCreate((TaskFunction_t )app_task1,  		/* ������ں��� */
			  (const char*    )"app_task1",			/* �������� */
			  (uint16_t       )512,  				/* ����ջ��С */
			  (void*          )NULL,				/* ������ں������� */
			  (UBaseType_t    )4, 					/* ��������ȼ� */
			  (TaskHandle_t*  )&app_task1_handle1);	/* ������ƿ�ָ�� */ 
	

	/* ����app_task2���� */
	xTaskCreate((TaskFunction_t )app_task2,  		/* ������ں��� */
			  (const char*    )"app_task2",			/* �������� */
			  (uint16_t       )512,  				/* ����ջ��С */
			  (void*          )NULL,				/* ������ں������� */
			  (UBaseType_t    )3, 					/* ��������ȼ� */
			  (TaskHandle_t*  )&app_task1_handle2);	/* ������ƿ�ָ�� */ 
	
	/* ����app_task3���� */
	xTaskCreate((TaskFunction_t )app_task3,  		/* ������ں��� */
			  (const char*    )"app_task3",			/* �������� */
			  (uint16_t       )512,  				/* ����ջ��С */
			  (void*          )NULL,				/* ������ں������� */
			  (UBaseType_t    )3, 					/* ��������ȼ� */
			  (TaskHandle_t*  )&app_task1_handle3);	/* ������ƿ�ָ�� */ 
	
	/* ����app_task4���� */
	xTaskCreate((TaskFunction_t )app_task4,  		/* ������ں��� */
			  (const char*    )"app_task4",			/* �������� */
			  (uint16_t       )512,  				/* ����ջ��С */
			  (void*          )NULL,				/* ������ں������� */
			  (UBaseType_t    )3, 					/* ��������ȼ� */
			  (TaskHandle_t*  )&app_task1_handle4);	/* ������ƿ�ָ�� */ 
	
  vTaskSuspend(app_task1_handle2);    
  vTaskSuspend(app_task1_handle3);    
  vTaskSuspend(app_task1_handle4);    				
	/* ����������� */
	vTaskStartScheduler(); 
	
}


static char flag;  //�û�����
static int fuc;   //��ǰ����

void  init_main()
{
	LCD_Fill(0,0,LCD_W,LCD_H,WHITE);
	fuc=1;
  main_manu_1();
}


//���������񡪡�������ɿ����ն˵���������ʾ
static void app_task1(void* pvParameters)           
{

	init_main();
	flag='0';
	fuc=1;

  //ѭ���ж��û�����
	while(1)
  {
		 LED0=!LED0;
		 delay_ms(1000);
	   flag=KEY_Scan(0);
		
	   if(flag=='1')             //�ƶ�ͼ��
     {
			  fuc++;
			  if(fuc>=4) fuc=1;
			  switch(fuc)
        {
					case 1: main_manu_1();break;
					case 2: main_manu_2();break;
					case 3: main_manu_3();break;
				}
			 
		      
		 }else if(flag=='2')        //ѡ����
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
		      }//ͼ���СΪ160*160
	    }
  }
	
} 


void  init_setting()
{
    LCD_Fill(0,0,LCD_W,LCD_H,WHITE);
	  
	  LCD_ShowString(100,60,"Background Light",BLACK,WHITE,32,0);//��ʾ�ַ���
	  LCD_ShowString(100,100,"WIFI",BLACK,WHITE,32,0);//��ʾ�ַ���
	  LCD_ShowString(100,140,".....",BLACK,WHITE,32,0);//��ʾ�ַ���
	  LCD_ShowString(100,180,".....",BLACK,WHITE,32,0);//��ʾ�ַ���
	  LCD_ShowString(100,220,"back",BLACK,WHITE,32,0);//��ʾ�ַ���
	  train_move(0);
}

//����
//1�����ñ�������
//2������wifi
static void app_task2(void* pvParameters)  
{
	
	
	  static int fuc=1;//��¼��ǰ��ѡ��
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
							    case 1:adjust_background_light();init_setting();break;//ִ����ض���1
							    case 2:break;//ִ����ض���2
							    case 3:break;//ִ����ض���3
							    case 4:break;//ִ����ض���4
							    case 5:vTaskResume(app_task1_handle1);init_setting();break;//������ҳ��
						 }
				}
			 
		}
}


//����
static void app_task3(void* pvParameters)  
{
	  while(1)
    {
        LCD_Fill(0,0,LCD_W,LCD_H,WHITE);
	      LCD_ShowString(100,100,"GUI2",BLACK,WHITE,16,0);//��ʾ�ַ���
	      delay_ms(1000);
	      flag='3';  
	      vTaskResume(app_task1_handle1);		
		}

}

void  init_train()
{
    LCD_Fill(0,0,LCD_W,LCD_H,WHITE);
	  
	  LCD_ShowString(100,60,"Gesture1",BLACK,WHITE,32,0);//��ʾ�ַ���
	  LCD_ShowString(100,100,"Gesture2",BLACK,WHITE,32,0);//��ʾ�ַ���
	  LCD_ShowString(100,140,"Gesture3",BLACK,WHITE,32,0);//��ʾ�ַ���
	  LCD_ShowString(100,180,"Gesture4",BLACK,WHITE,32,0);//��ʾ�ַ���
	  LCD_ShowString(100,220,"back",BLACK,WHITE,32,0);//��ʾ�ַ���
	  train_move(0);
}

//ѵ��
static void app_task4(void* pvParameters)  
{
	
	
	  static int fuc=1;//��¼��ǰ��ѡ��
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
							    case 1:break;//ִ����ض���1
							    case 2:break;//ִ����ض���2
							    case 3:break;//ִ����ض���3
							    case 4:break;//ִ����ض���4
							    case 5:vTaskResume(app_task1_handle1);init_train();break;//������ҳ��
						 }
				}
			 
		}
}


/**
*******************����ע�͵��Ĵ�����ͨ�� λ�� ����ʵ��IO�ڿ���**************************************
	
int main(void)
{ 
 
	delay_init(168);		  //��ʼ����ʱ����
	LED_Init();		        //��ʼ��LED�˿�
  while(1)
	{
     LED0=0;			  //LED0��
	   LED1=1;				//LED1��
		 delay_ms(500);
		 LED0=1;				//LED0��
		 LED1=0;				//LED1��
		 delay_ms(500);
	 }
}
**************************************************************************************************
 **/	
	
/**
*******************����ע�͵��Ĵ�����ͨ�� ֱ�Ӳ����Ĵ��� ��ʽʵ��IO�ڿ���**************************************
int main(void)
{ 
 
	delay_init(168);		  //��ʼ����ʱ����
	LED_Init();		        //��ʼ��LED�˿�
	while(1)
	{
     GPIOF->BSRRH=GPIO_Pin_9;//LED0��
	   GPIOF->BSRRL=GPIO_Pin_10;//LED1��
		 delay_ms(500);
     GPIOF->BSRRL=GPIO_Pin_9;//LED0��
	   GPIOF->BSRRH=GPIO_Pin_10;//LED1��
		 delay_ms(500);

	 }
 }	 
**************************************************************************************************
**/	
 



