/*
***************************************************************************
*    ģ�飺BSP_BH1750 
*    �������弶 BH1750 ����ģ������
		   BH1750-PG13
*    ���ߣ�Huo
*    ʱ�䣺2017.09.29
*    �汾��UP-IOT 1.0.0
***************************************************************************
*/
#include "BSP_BH1750.h"

/* �ڲ��������� */
static void BSP_BH1750_GPIO_Init(void);

/*
***************************************************************************
*	�� �� ��: BSP_BH1750_Init
*	����˵��: �弶 BH1750 ��ʼ������
*	��    ��: ��
*	�� �� ֵ: ��
***************************************************************************
*/
void BSP_BH1750_Init(void)
{
	volatile uint32_t Count=80;

	BSP_BH1750_GPIO_Init();
	while(Count--);			//��ʱ 1us
	//BH1750_DVI /* = 1 */;
	GPIO_WriteBit(GPIOC, GPIO_Pin_1, Bit_SET);   // 置高
	
	BSP_BH1750_SetMode(BH1750_Power_On);
}

/*
***************************************************************************
*	�� �� ��: BSP_BH1750_GPIO_Init
*	����˵��: �弶 BH1750 GPIO �ڲ���ʼ������
*	��    ��: ��
*	�� �� ֵ: ��
***************************************************************************
*/
static void BSP_BH1750_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	//ʹ�ܶ�Ӧʱ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		//�������
	GPIO_Init(GPIOC, &GPIO_InitStructure);

/* 	BH1750_DVI = 0; */

GPIO_WriteBit(GPIOC, GPIO_Pin_1, Bit_RESET); // 置低
}

uint8_t BSP_BH1750_SetMode(uint8_t BH1750_Mode)
{
	uint16_t m;

	/*���ڣ�������ֹͣ�źţ������ڲ�д������*/
	BSP_MyIIC_Stop();

	/* ͨ���������Ӧ��ķ�ʽ���ж��ڲ�д�����Ƿ����, һ��С�� 10ms
		CLKƵ��Ϊ200KHzʱ����ѯ����Ϊ30������
	*/
	for (m = 0; m < 1000; m++)
	{
		/* ��1��������I2C���������ź� */
		BSP_MyIIC_Start();

		/* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
		BSP_MyIIC_SendByte(BH1750_Addr | I2C_WR);	/* �˴���дָ�� */

		/* ��3��������һ��ʱ�ӣ��ж������Ƿ���ȷӦ�� */
		if (BSP_MyIIC_WaitAck() == 0)
		{
			break;
		}
	}
	if (m  == 1000)
	{
		goto cmd_fail;	/* ����д��ʱ */
	}

	/* ��4������ʼд������ */
	BSP_MyIIC_SendByte(BH1750_Mode);

	/* ��5��������ACK */
	if (BSP_MyIIC_WaitAck() != 0)
	{
		goto cmd_fail;	/* ������Ӧ�� */
	}

	/* ����ִ�гɹ�������I2C����ֹͣ�ź� */
	BSP_MyIIC_Stop();
	return BH1750_SUCCESS;

cmd_fail: /* ����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 */
	/* ����I2C����ֹͣ�ź� */
	BSP_MyIIC_Stop();
	return BH1750_FAILURE;
}

uint16_t BSP_BH1750_ReadResult(void)
{
	uint16_t Result;

	/* ��1��������I2C���������ź� */
	BSP_MyIIC_Start();

	/* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
	BSP_MyIIC_SendByte(BH1750_Addr | I2C_RD);	/* �˴��Ƕ�ָ�� */

	/* ��3��������ACK */
	if (BSP_MyIIC_WaitAck() != 0)
	{
		goto cmd_fail;	/* ������Ӧ�� */
	}

	/* ��4������ȡ���� */
//	Result = (BSP_MyIIC_ReadByte()<<8);	/* �����ֽ� */
//	BSP_MyIIC_Ack();	/* �м��ֽڶ����CPU����ACK�ź�(����SDA = 0) */
//	Result = BSP_MyIIC_ReadByte();	/* �����ֽ� */
//	BSP_MyIIC_NAck();	/* ���1���ֽڶ����CPU����NACK�ź�(����SDA = 1) */
	Result = BSP_MyIIC_ReadByte();	/* �����ֽ� */
	BSP_MyIIC_Ack();	/* �м��ֽڶ����CPU����ACK�ź�(����SDA = 0) */
	Result = (Result<<8) | BSP_MyIIC_ReadByte();	/* �����ֽ� */
	BSP_MyIIC_NAck();	/* ���1���ֽڶ����CPU����NACK�ź�(����SDA = 1) */

	/* ����I2C����ֹͣ�ź� */
	BSP_MyIIC_Stop();
	return Result;	/* ִ�гɹ� */

cmd_fail: /* ����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 */
	/* ����I2C����ֹͣ�ź� */
	BSP_MyIIC_Stop();
	return BH1750_FAILURE;
}
