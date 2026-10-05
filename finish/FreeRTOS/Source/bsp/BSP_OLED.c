#include "BSP_OLED.h"
#include "OLEDFont.h"  	 
#include "BSP_Delay.h"
#include <stdbool.h>		   
//向SSD1106写入一个字节。
//dat:要写入的数据/命令
//cmd:数据/命令标志 0,表示命令;1,表示数据;
void OLED_WR_Byte(u8 dat,u8 cmd)
{	
	u8 i;			  
	if(cmd)
	  OLED_DC_Set();
	else 
	  OLED_DC_Clr();		  
	
	for(i=0;i<8;i++)
	{			  
		OLED_SCLK_Clr();
		if(dat&0x80)
		   OLED_SDIN_Set();
		else 
		   OLED_SDIN_Clr();
		OLED_SCLK_Set();
		dat<<=1;   
	}				 		  
	OLED_DC_Set();   	  
} 


	void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 
	OLED_WR_Byte(0xb0+y,OLED_CMD);
	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
	OLED_WR_Byte((x&0x0f)|0x01,OLED_CMD); 
}   	  
//开启OLED显示    
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}
//关闭OLED显示     
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}		   			 
//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!	  
void OLED_Clear(void)  
{  
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
		OLED_WR_Byte (0x00,OLED_CMD);      //设置显示位置—列低地址
		OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)OLED_WR_Byte(0,OLED_DATA); 
	} //更新显示
}


//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~7 每格数据代表8行
//mode:0,反白显示;1,正常显示				 
//size:选择字体 16/12 
void OLED_ShowChar(u8 x,u8 y,u8 chr)
{      	
	unsigned char c=0,i=0;	
		c=chr-' ';//得到偏移后的值			
		if(x>Max_Column-1){x=0;y=y+2;}
		if(SIZE ==16)
			{
			OLED_Set_Pos(x,y);	
			for(i=0;i<8;i++)
			OLED_WR_Byte(F8X16[c*16+i],OLED_DATA);
			OLED_Set_Pos(x,y+1);
			for(i=0;i<8;i++)
			OLED_WR_Byte(F8X16[c*16+i+8],OLED_DATA);
			}			
}

//显示一个字符号串
void OLED_ShowString(u8 x,u8 y,u8 *chr)
{
	unsigned char j=0;
	while (chr[j]!='\0')
	{		OLED_ShowChar(x,y,chr[j]);
			x+=8;
		if(x>120){x=0;y+=2;}
			j++;
	}
}
//显示汉字
void OLED_ShowCHinese(u8 x,u8 y,u8 no)
{      			    
	u8 t,adder=0;
	OLED_Set_Pos(x,y);	
    for(t=0;t<16;t++)
		{
				OLED_WR_Byte(Hzk[2*no][t],OLED_DATA);
				adder+=1;
     }	
		OLED_Set_Pos(x,y+1);	
    for(t=0;t<16;t++)
			{	
				OLED_WR_Byte(Hzk[2*no+1][t],OLED_DATA);
				adder+=1;
      }					
}

//显示32X32点阵汉字
void OLED_Show_CHinese32X32(u8 x,u8 y,u8 no) 
{              
  u8 t; 
  OLED_Set_Pos(x,y);  
     for(t=0;t<32;t++) 
   { 
     OLED_WR_Byte(Hzk32[4*no][t],OLED_DATA); 
      }  
   OLED_Set_Pos(x,y+1);  
     for(t=0;t<32;t++) 
    {  
     OLED_WR_Byte(Hzk32[4*no+1][t],OLED_DATA); 
        }     
    
     OLED_Set_Pos(x,y+2);  
     for(t=0;t<32;t++)    { 
     OLED_WR_Byte(Hzk32[4*no+2][t],OLED_DATA); 
      }  
   OLED_Set_Pos(x,y+3);  
     for(t=0;t<32;t++) 
    {  
     OLED_WR_Byte(Hzk32[4*no+3][t],OLED_DATA); 
   
       }  
} 
 

//初始化SSD1306					    
void OLED_Init(void)
{ 		 
 	GPIO_InitTypeDef  GPIO_InitStructure;
 	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);	 //使能PD端口时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;	 //PD4~PD7推挽输出  
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
 	GPIO_Init(GPIOD, &GPIO_InitStructure);	  //初始化GPIOD3,6
 	GPIO_SetBits(GPIOD,GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_3|GPIO_Pin_8);	//PD3,PD6 输出高

  OLED_RST_Set();
	BSP_Delay_ms(100);
	OLED_RST_Clr();
	BSP_Delay_ms(100);
	OLED_RST_Set(); 
					  
	OLED_WR_Byte(0xAE,OLED_CMD);//--turn off oled panel
	OLED_WR_Byte(0x00,OLED_CMD);//---set low column address
	OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
	OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_WR_Byte(0x81,OLED_CMD);//--set contrast control register
	OLED_WR_Byte(0xCF,OLED_CMD); // Set SEG Output Current Brightness
	OLED_WR_Byte(0xA1,OLED_CMD);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
	OLED_WR_Byte(0xC8,OLED_CMD);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
	OLED_WR_Byte(0xA6,OLED_CMD);//--set normal display
	OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
	OLED_WR_Byte(0x3f,OLED_CMD);//--1/64 duty
	OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_WR_Byte(0x00,OLED_CMD);//-not offset
	OLED_WR_Byte(0xd5,OLED_CMD);//--set display clock divide ratio/oscillator frequency
	OLED_WR_Byte(0x80,OLED_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
	OLED_WR_Byte(0xD9,OLED_CMD);//--set pre-charge period
	OLED_WR_Byte(0xF1,OLED_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_WR_Byte(0xDA,OLED_CMD);//--set com pins hardware configuration
	OLED_WR_Byte(0x12,OLED_CMD);
	OLED_WR_Byte(0xDB,OLED_CMD);//--set vcomh
	OLED_WR_Byte(0x40,OLED_CMD);//Set VCOM Deselect Level
	OLED_WR_Byte(0x20,OLED_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_WR_Byte(0x02,OLED_CMD);//
	OLED_WR_Byte(0x8D,OLED_CMD);//--set Charge Pump enable/disable
	OLED_WR_Byte(0x14,OLED_CMD);//--set(0x10) disable
	OLED_WR_Byte(0xA4,OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
	OLED_WR_Byte(0xA6,OLED_CMD);// Disable Inverse Display On (0xa6/a7) 
	OLED_WR_Byte(0xAF,OLED_CMD);//--turn on oled panel
	
	OLED_WR_Byte(0xAF,OLED_CMD); /*display ON*/ 
	OLED_Clear();
	OLED_Set_Pos(0,0); 	
}  

void show_WiFi_Connected(void)
{
	OLED_ShowCHinese(5,0,8);
	OLED_ShowCHinese(25,0,9);
	OLED_ShowCHinese(45,0,10);
	OLED_ShowCHinese(65,0,11);
	OLED_ShowCHinese(85,0,12);
	OLED_ShowCHinese(105,0,13);
}

void show_WiFi_disConnect(void)
{
	OLED_ShowCHinese(5,0,8);
	OLED_ShowCHinese(25,0,9);
	OLED_ShowCHinese(45,0,14);
	OLED_ShowCHinese(65,0,15);
	OLED_ShowCHinese(85,0,10);
	OLED_ShowCHinese(105,0,11);
}

void show_WiFi_Lost(void)
{
//	LCD_ShowChinese(30,50,"网络连接丢失",BLUE,LGRAY,32,0);
	OLED_ShowCHinese(5,0,8);
	OLED_ShowCHinese(25,0,9);
	OLED_ShowCHinese(45,0,10);
	OLED_ShowCHinese(65,0,11);
	OLED_ShowCHinese(45,0,16);
	OLED_ShowCHinese(65,0,17);	
}

void show_WiFi_busy(void)
{
//	LCD_ShowChinese(30,50,"通信模块忙碌",BLUE,LGRAY,32,0);
	OLED_ShowCHinese(5,0,32);
	OLED_ShowCHinese(25,0,33);
	OLED_ShowCHinese(45,0,18);
	OLED_ShowCHinese(65,0,19);
	OLED_ShowCHinese(45,0,32);
	OLED_ShowCHinese(65,0,33);	
}

void show_WiFi_noResp(void)
{
//	LCD_ShowChinese(30,50,"网络没有响应",BLUE,LGRAY,32,0);
	OLED_ShowCHinese(5,0,8);
	OLED_ShowCHinese(25,0,9);
	OLED_ShowCHinese(45,0,22);
	OLED_ShowCHinese(65,0,23);
	OLED_ShowCHinese(45,0,24);
	OLED_ShowCHinese(65,0,25);
}

void show_user_configWiFi(void)
{
//  LCD_ShowChinese(40,80,"请按下按键",BLUE,LGRAY,32,0);
//	LCD_ShowChinese(55,112,"配置网络",BLUE,LGRAY,32,0);
	OLED_ShowCHinese(5,0,26);
	OLED_ShowCHinese(25,0,27);
	OLED_ShowCHinese(45,0,18);
	OLED_ShowCHinese(65,0,19);
	OLED_ShowCHinese(45,0,8);
	OLED_ShowCHinese(65,0,9);
}


void show_MQTT_Connect_result(bool flag)
{
	OLED_ShowString(25,3,"MQTT");
	OLED_ShowCHinese(60,3,29);
	OLED_ShowCHinese(80,3,30);
	OLED_ShowCHinese(100,3,31);
  if(flag == true)
	{

//		OLED_ShowCHinese(0,5,29);
//		OLED_ShowCHinese(15,5,30);
//		OLED_ShowCHinese(35,5,31);
		OLED_ShowCHinese(30,6,10);
		OLED_ShowCHinese(50,6,11);
		OLED_ShowCHinese(70,6,12);
		OLED_ShowCHinese(90,6,13);	
	}
	else
	{

		OLED_ShowCHinese(30,6,14);
		OLED_ShowCHinese(50,6,15);
		OLED_ShowCHinese(70,6,10);
		OLED_ShowCHinese(90,6,11);	
	}
}
