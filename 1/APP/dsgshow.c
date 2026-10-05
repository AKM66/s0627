/****************************************************************
                   Source file of dsgshow.c
****************************************************************/
#include "dsgshow.h"
//declare three extern variable
extern u8 hour,minute,second;
u8 smgduan[11]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
u16 smgwei[6]={0x7fff,0xbfff,0xdfff,0xefff,0xf7ff,0xfbff};

/****************************************************************
* �� �� ��         : DsgShowInit
* ��������		   : �������ʾ��ʼ��
* ��    ��         : ��
* ��    ��         : ��
****************************************************************/
void DsgShowInit()
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	//��GPIOE��GPIOGʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOG , ENABLE); 
	//����GPIOEΪ�������ģʽ, Ϊ��������жϲ�������, ֻ��ʼ����8λ	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15|GPIO_Pin_14|GPIO_Pin_13|
	GPIO_Pin_12|GPIO_Pin_11|GPIO_Pin_10|GPIO_Pin_9|GPIO_Pin_8; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ; 
	GPIO_Init(GPIOE, &GPIO_InitStructure); 
	//����GPIOGΪ�������ģʽ
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ; 
	GPIO_Init(GPIOG, &GPIO_InitStructure); 
}

/****************************************************************
* �� �� ��         : DsgShowNum
* ��������		   : �������ʾ��λѧ��
* ��    ��         : ��
* ��    ��         : ��
****************************************************************/
void DsgShowNum()
{
	u16 i;
	while(1)
	{
		GPIO_Write(GPIOE,0x7FFF);//ѡ�е�һ�������
		GPIO_Write(GPIOG,0xc0); //�͵�һ���ֵĶ���
		for(i=0;i<400;i++)  ;//��ʱ�Ƚ϶̵�ʱ��
		GPIO_Write(GPIOE,0xBFFF);
		GPIO_Write(GPIOG,0xf9); 
		for(i=0;i<400;i++)  ;
		GPIO_Write(GPIOE,0xDFFF);
		GPIO_Write(GPIOG,0xa4); 
		for(i=0;i<400;i++)  ;
		GPIO_Write(GPIOE,0xEFFF);
		GPIO_Write(GPIOG,0xb0); 
		for(i=0;i<400;i++)  ;
		GPIO_Write(GPIOE,0xF7FF);
		GPIO_Write(GPIOG,0x99);		
		for(i=0;i<400;i++)  ;
		GPIO_Write(GPIOE,0xFBFF);
		GPIO_Write(GPIOG,0x92);		
		for(i=0;i<400;i++)  ;
	}
}

/****************************************************************
* �� �� ��         : DsgShowNum
* ��������		   : �������ʾʱ��
* ��    ��         : ��
* ��    ��         : ��
****************************************************************/
void DsgShowTime()
{
	u16 j;
	while(1)
	{	
		GPIO_Write(GPIOE,smgwei[0]);	
		GPIO_Write(GPIOG,smgduan[hour/10]);
		for(j=0;j<400;j++);
		GPIO_Write(GPIOE,smgwei[1]);	
		GPIO_Write(GPIOG,(smgduan[hour%10])&0xff7f);
		for(j=0;j<400;j++);
		
		GPIO_Write(GPIOE,smgwei[2]);	
		GPIO_Write(GPIOG,smgduan[minute/10]);
		for(j=0;j<400;j++);
		GPIO_Write(GPIOE,smgwei[3]);	
		GPIO_Write(GPIOG,(smgduan[minute%10])&0xff7f);
		for(j=0;j<400;j++);
		
		GPIO_Write(GPIOE,smgwei[4]);	
		GPIO_Write(GPIOG,smgduan[second/10]);
		for(j=0;j<400;j++);
		GPIO_Write(GPIOE,smgwei[5]);	
		GPIO_Write(GPIOG,smgduan[second%10]);
		for(j=0;j<400;j++);
	}
}
