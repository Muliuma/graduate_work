#ifndef __SPI_H
#define __SPI_H
#include "sys.h"

//��������
//PB3:CLK
//PB4:MISO
//PB5:MOSI

void SPI1_Init(void);			 //��ʼ��SPI1��
void SPI1_SetSpeed(u8 SpeedSet); //����SPI1�ٶ�   
u8 SPI1_ReadWriteByte(u8 TxData);//SPI1���߶�дһ���ֽ�

		 
#endif

