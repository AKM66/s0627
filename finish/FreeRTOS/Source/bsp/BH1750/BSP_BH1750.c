/*
***************************************************************************
*    模块：BSP_BH1750 
*    描述：板级 BH1750 功能模块驱动
		   BH1750-PG13
*    作者：Huo
*    时间：2017.09.29
*    版本：UP-IOT 1.0.0
***************************************************************************
*/
#include "BSP_BH1750.h"

/* 内部函数声明 */
static void BSP_BH1750_GPIO_Init(void);

/*
***************************************************************************
*	函 数 名: BSP_BH1750_Init
*	功能说明: 板级 BH1750 初始化函数
*	形    参: 无
*	返 回 值: 无
***************************************************************************
*/
void BSP_BH1750_Init(void)
{
	volatile uint32_t Count=80;

	BSP_BH1750_GPIO_Init();
	while(Count--);			//延时 1us
	BH1750_DVI = 1;
	
	BSP_BH1750_SetMode(BH1750_Power_On);
}

/*
***************************************************************************
*	函 数 名: BSP_BH1750_GPIO_Init
*	功能说明: 板级 BH1750 GPIO 内部初始化函数
*	形    参: 无
*	返 回 值: 无
***************************************************************************
*/
static void BSP_BH1750_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	//使能对应时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		//推挽输出
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	BH1750_DVI = 0;
}

uint8_t BSP_BH1750_SetMode(uint8_t BH1750_Mode)
{
	uint16_t m;

	/*　第０步：发停止信号，启动内部写操作　*/
	BSP_MyIIC_Stop();

	/* 通过检查器件应答的方式，判断内部写操作是否完成, 一般小于 10ms
		CLK频率为200KHz时，查询次数为30次左右
	*/
	for (m = 0; m < 1000; m++)
	{
		/* 第1步：发起I2C总线启动信号 */
		BSP_MyIIC_Start();

		/* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
		BSP_MyIIC_SendByte(BH1750_Addr | I2C_WR);	/* 此处是写指令 */

		/* 第3步：发送一个时钟，判断器件是否正确应答 */
		if (BSP_MyIIC_WaitAck() == 0)
		{
			break;
		}
	}
	if (m  == 1000)
	{
		goto cmd_fail;	/* 器件写超时 */
	}

	/* 第4步：开始写入数据 */
	BSP_MyIIC_SendByte(BH1750_Mode);

	/* 第5步：发送ACK */
	if (BSP_MyIIC_WaitAck() != 0)
	{
		goto cmd_fail;	/* 器件无应答 */
	}

	/* 命令执行成功，发送I2C总线停止信号 */
	BSP_MyIIC_Stop();
	return BH1750_SUCCESS;

cmd_fail: /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
	/* 发送I2C总线停止信号 */
	BSP_MyIIC_Stop();
	return BH1750_FAILURE;
}

uint16_t BSP_BH1750_ReadResult(void)
{
	uint16_t Result;

	/* 第1步：发起I2C总线启动信号 */
	BSP_MyIIC_Start();

	/* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
	BSP_MyIIC_SendByte(BH1750_Addr | I2C_RD);	/* 此处是读指令 */

	/* 第3步：发送ACK */
	if (BSP_MyIIC_WaitAck() != 0)
	{
		goto cmd_fail;	/* 器件无应答 */
	}

	/* 第4步：读取数据 */
//	Result = (BSP_MyIIC_ReadByte()<<8);	/* 读高字节 */
//	BSP_MyIIC_Ack();	/* 中间字节读完后，CPU产生ACK信号(驱动SDA = 0) */
//	Result = BSP_MyIIC_ReadByte();	/* 读低字节 */
//	BSP_MyIIC_NAck();	/* 最后1个字节读完后，CPU产生NACK信号(驱动SDA = 1) */
	Result = BSP_MyIIC_ReadByte();	/* 读高字节 */
	BSP_MyIIC_Ack();	/* 中间字节读完后，CPU产生ACK信号(驱动SDA = 0) */
	Result = (Result<<8) | BSP_MyIIC_ReadByte();	/* 读低字节 */
	BSP_MyIIC_NAck();	/* 最后1个字节读完后，CPU产生NACK信号(驱动SDA = 1) */

	/* 发送I2C总线停止信号 */
	BSP_MyIIC_Stop();
	return Result;	/* 执行成功 */

cmd_fail: /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
	/* 发送I2C总线停止信号 */
	BSP_MyIIC_Stop();
	return BH1750_FAILURE;
}
