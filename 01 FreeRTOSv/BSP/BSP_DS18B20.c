#include "BSP_DS18B20.h"
#include "BSP_Delay.h"
#include "BSP_DebugUART.h"
volatile float result = 0.0f;

//GPIO初始化
uint8_t BSP_DS18B20_Init(void)
{
		
	GPIO_InitTypeDef GPIO_InitStructure; 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC , ENABLE); 
		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ; 
	GPIO_Init(GPIOE, &GPIO_InitStructure); 
	
	GPIO_SetBits(GPIOE,GPIO_Pin_4);
	
	BSP_DS18B20_Rst();
	
	return BSP_DS18B20_Check();

}	



//复位
void BSP_DS18B20_Rst(void)	   
{    	
  DS18B20_IO_OUT(); 	//SET PG11OUTPUT
  DS18B20_DQ_OUT_0(); 	//拉低DQ
  BSP_Delay_us(750);  	//拉低750us
  DS18B20_DQ_OUT_1(); 	//DQ=1 
  BSP_Delay_us(15);   	//15US
}


//检查设备是否在线 无设备返回1 有设备返回0
uint8_t BSP_DS18B20_Check(void) 	   
{   
	uint8_t retry=0;
	DS18B20_IO_IN();	//SET PE4 INPUT	 
  while (GPIO_ReadOutputDataBit(GPIOE,GPIO_Pin_4)&&retry<200)
	{
		retry++;
		BSP_Delay_us(1);
	};	 
	if(retry>=200)return 1;
	else retry=0;
    while (!GPIO_ReadOutputDataBit(GPIOE,GPIO_Pin_4)&&retry<240)
	{
		retry++;
		BSP_Delay_us(1);
	};
	if(retry>=240)return 1;	    
	return 0;
}


//写一个字节
void BSP_DS18B20_Write_Byte(uint8_t data)     
{            
  uint8_t j;
  uint8_t testb;
  DS18B20_IO_OUT();	//SET PE4 OUTPUT;
  for (j=1;j<=8;j++) 
  {
  testb=data&0x01;
  data=data>>1;
  if(testb) 
  {
		DS18B20_DQ_OUT_0();	// Write 1
		BSP_Delay_us(2);                            
		DS18B20_DQ_OUT_1();
		BSP_Delay_us(60); 
	}
  else 
	{
		DS18B20_DQ_OUT_0();	// Write 0
		BSP_Delay_us(60);             
		DS18B20_DQ_OUT_1();
		BSP_Delay_us(2);                          
 	}
  }
}

//读取一个比特位
uint8_t BSP_DS18B20_Read_Bit(void) 	 
{
  uint8_t data;
  DS18B20_IO_OUT();	//设置为输出
  DS18B20_DQ_OUT_0(); 
  BSP_Delay_us(2);	//输出低电平2us
  DS18B20_DQ_OUT_1(); //释放总线
  DS18B20_IO_IN();	//设置为输入
  BSP_Delay_us(12);	//延时12us
  if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4))	//读取总线数据
  {
		data=1;
	}
	else
  {
		data=0;
	}		
  	BSP_Delay_us(50);    //延时50us       
  	return data;
}


//读取一个字节数据
uint8_t BSP_DS18B20_Read_Byte(void)     
{        
  uint8_t i,j,data;
  data=0;
	for (i=1;i<=8;i++) 
	{
		j=BSP_DS18B20_Read_Bit();
    data=(j<<7)|(data>>1);
  }						    
  return data;
}

//开始温度转换
void BSP_DS18B20_Start(void) 
{   					               
		BSP_DS18B20_Rst();	   
		BSP_DS18B20_Check();	 
		BSP_DS18B20_Write_Byte(0xcc);// skip rom
		BSP_DS18B20_Write_Byte(0x44);// convert
}


short BSP_DS18B20_Get_Temp(void)
{
  uint8_t temp;
  uint8_t TL,TH;
  short tem;
  BSP_DS18B20_Start ();  		// ds1820 start convert
  BSP_DS18B20_Rst();
  BSP_DS18B20_Check();	 
  BSP_DS18B20_Write_Byte(0xcc);	// skip rom
  BSP_DS18B20_Write_Byte(0xbe);	// convert	    
  TL=BSP_DS18B20_Read_Byte(); 	// LSB   
  TH=BSP_DS18B20_Read_Byte(); 	// MSB  	    	  
  if(TH>7)
  {
		TH=~TH;
    TL=~TL; 
    temp=0;		//温度为负  
  }else
 {
    temp=1;		//温度为正	  	  
  }
  tem=TH; 		//获得高八位
  tem<<=8;    
  tem+=TL;		//获得底八位
  tem=(float)tem*0.625;	//转换     
  if(temp)
  {
    return tem; 		//返回温度值
  }
  else 
   return -tem;    
}

void SenSorData_Output(void)
{
		short temperature = 0; 
		float integer = 0.0f;
		float decimal = 0.0f;
		//u8 buf[20];
		temperature = BSP_DS18B20_Get_Temp();
		integer = (float)(temperature/10);
		decimal = (float)(temperature%10)/10;
		result = integer + decimal;
		//printf("DSP18B20 Temperature = %.2f\r\n",result);
}
