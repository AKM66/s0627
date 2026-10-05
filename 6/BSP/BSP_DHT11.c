/*
***************************************************************************
*    ģ�飺BSP_DHT11 
*    �������弶 DHT11 ����ģ������
*    ���ߣ�Shao
*    ʱ�䣺2018.06.12
*    �汾��UP-IOT 2.0.0
***************************************************************************
*/
#include "BSP_DHT11.h"

/*
*******************************************************************************
*	�� �� ��: _DelayUs
*	����˵��: �Ǿ�׼us������ʱ����
*	��    ��: uint16_t xUs
*	�� �� ֵ: ��
*******************************************************************************
*/
static void _DelayUs(uint16_t xUs)
{
	while(xUs != 0){
		xUs--;
	}
}

/*
*******************************************************************************
*	�� �� ��: _DelayMs
*	����˵��: �Ǿ�׼ms������ʱ����
*	��    ��: uint16_t count
*	�� �� ֵ: ��
*******************************************************************************
*/
/* �Ǿ�ȷ��ʱ���� */
static void _DelayMs(uint16_t xMs)
{
	volatile uint32_t Count=8000;
	while(xMs--)
	{
		Count=8000;
		while(Count--);
	}
}

/*
***************************************************************************
*	�� �� ��: BSP_DHT11_Init
*	����˵��: DHT11��ʼ������
*	��    ��: ��
*	�� �� ֵ: ��
***************************************************************************
*/
void BSP_DHT11_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;        
	RCC_APB2PeriphClockCmd(DHT11_RCC_CLK,ENABLE);
			
	GPIO_InitStructure.GPIO_Pin = DHT11_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(DHT11_PORT, &GPIO_InitStructure);

	BSP_DHT11_Reset();        //��λͨѶ
}

/*
***************************************************************************
*	�� �� ��: BSP_DHT11_Reset
*	����˵��: DHT11��λ����
*	��    ��: ��
*	�� �� ֵ: ��
***************************************************************************
*/
void BSP_DHT11_Reset(void)
{
	DHT11_IO_OUT(); 			//�������ģʽ
  //DHT11_OUT=0; 					//����DQ		
GPIO_WriteBit(GPIOC, GPIO_Pin_1, Bit_RESET); // 置低
//  _DelayMs(18);					//��������18ms
	BSP_Delay_ms(20);
	//DHT11_OUT=1; 					//DQ=1 
	GPIO_WriteBit(GPIOC, GPIO_Pin_1, Bit_SET);   // 置高
//	_DelayUs(320);				//��������20~40us     35
	BSP_Delay_us(35);
}

/*
***************************************************************************
*	�� �� ��: BSP_DHT11_CheckDevice
*	����˵��: ���DHT11�豸�Ƿ���ں���
*	��    ��: ��
*	�� �� ֵ: ����0������⵽DHT11�豸������1����û�м�⵽DHT11�豸
***************************************************************************
*/
uint16_t BSP_DHT11_CheckDevice(void){
	uint8_t delay_cnt = 0;
	DHT11_IO_IN();													//��������ģʽ
	while(DHT11_IN&&delay_cnt<100)					//DHT11������40~80us
	{
		delay_cnt++;
		BSP_Delay_us(10);
//		_DelayUs(80);
	};	 
	if(delay_cnt >= 100){										//��ʱ����1
		return 1;
	}
	else {
		delay_cnt = 0;
	}
  while((!DHT11_IN) && (delay_cnt < 100))	//DHT11���ͺ���ٴ�����40~80us
	{
		delay_cnt++;
		BSP_Delay_us(10);
//		_DelayUs(80);
	};
	if(delay_cnt >= 100){
		return 1;
	}		
	return 0;
}


/*
***************************************************************************
*	�� �� ��: DHT11_Read_Bit
*	����˵��: ��ȡ1Bit����
*	��    ��: ��
*	�� �� ֵ: ���ض�����������0����1
***************************************************************************
*/
uint8_t DHT11_Read_Bit(void) 			 
{
 	uint8_t delay_cnt=0;
	while(DHT11_IN&&delay_cnt<100)	//�ȴ���Ϊ�͵�ƽ
	{
		delay_cnt++;
		BSP_Delay_us(10);
//		_DelayUs(80);
	}
	delay_cnt = 0;
	while(!DHT11_IN&&delay_cnt<100)	//�ȴ���ߵ�ƽ
	{
		delay_cnt++;
		BSP_Delay_us(10);
//		_DelayUs(80);
	}
		BSP_Delay_us(35);
//	_DelayUs(320);									//�ȴ�40us	����1�źŵ���ʱ������0�źŵ���ʱ����Լ40us.
	if(DHT11_IN){										//�ж�����״̬
		return 1;
	}
	else 
		return 0;	
}

/*
***************************************************************************
*	�� �� ��: DHT11_Read_Byte
*	����˵��: ��ȡ1�ֽں���
*	��    ��: ��
*	�� �� ֵ: ���ض���������
***************************************************************************
*/
uint8_t DHT11_Read_Byte(void)    
{        
  uint8_t i;
  uint8_t data=0;
	
	for (i=0;i<8;i++) 
	{
   		data <<= 1; 
	    data |= DHT11_Read_Bit();
    }						    
    return data;
}

/*
***************************************************************************
*	�� �� ��: BSP_DHT11_Read_Data
*	����˵��: ��ȡ��ʪ�ȴ���������
*	��    ��: uint8_t *temp_int:����¶�ֵ��������
						uint8_t *humi_int:���ʪ��ֵ��������
						uint8_t *temp_float:����¶�ֵС������
						uint8_t *humi_float:���ʪ��ֵС������
*	�� �� ֵ: ���ض���������
***************************************************************************
*/
uint8_t BSP_DHT11_Read_Data(uint8_t *temp_int,uint8_t *humi_int,uint8_t *temp_float,uint8_t *humi_float)    
{        
 	uint8_t buf[5];
	uint8_t i;
	BSP_DHT11_Reset();
	if(BSP_DHT11_CheckDevice() == 0)
	{
		for(i=0;i<5;i++)//��ȡ40λ����
		{
			buf[i]=DHT11_Read_Byte();
		}
		if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4])
		{
			*humi_int = buf[0];
			*humi_float = buf[1];
			*temp_int = buf[2];
			*temp_float = buf[3];
		}
	}
	else 
		return 1;
	return 0;	    
}

/*
***************************************************************************
*	�� �� ��: BSP_DHT11_CalcuDewPoint
*	����˵��: ����¶��
*	��    ��: h-ʵ�ʵ�ʪ�ȣ�t-ʵ�ʵ��¶� 
*	�� �� ֵ: dew_point-¶��
***************************************************************************
*/
float BSP_DHT11_CalcuDewPoint(float t, float h)
{
	float logEx, dew_point;

	logEx = 0.66077 + 7.5 * t / (237.3 + t) + (log10(h) - 2);
	dew_point = ((0.66077 - logEx) * 237.3) / (logEx - 8.16077);

	return dew_point; 
}
