/**
  ******************************************************************************
  * @file    bsp_led.c
  * @Author  Simic
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   led应用函数接口
  ******************************************************************************
  * @attention
  ******************************************************************************
  */

#include "./led/bsp_led.h"
//#include "bsp.h"

uint16_t LED_PIN_GROUP[4] = {GPIO_Pin_5, GPIO_Pin_8, GPIO_Pin_10, GPIO_Pin_12};

/**
 * @brief  LED GPIO 配置初始化
 * @retval None
 */
void LED_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd( LED0_GPIO_CLK/*|LED1_GPIO_CLK|LED2_GPIO_CLK|LED3_GPIO_CLK*/, ENABLE);

    GPIO_InitStructure.GPIO_Pin = LED0_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LED0_GPIO_PORT, &GPIO_InitStructure);

    /** 关闭所有led灯	*/
    GPIO_SetBits(LED0_GPIO_PORT, LED0_GPIO_PIN);

}

void SetLedConnectNet(LEDNetworkStatus status)
{
	switch(status)
	{
		case LED_NETWORK_OK:
			LED0_ON;
//			LED1_OFF;
		break;
		case LED_NETWORK_NOK:
		LED0_OFF;
//		LED1_ON;
		break;
		default:
		break;
	}
}

/**< get the led status  */
//static int led_status_get(GPIO_TypeDef* GPIOx, uint8_t pin_index)
//{
//    return GPIO_ReadOutputDataBit(GPIOx, LED_PIN_GROUP[pin_index]);
//}

void LED_OnOFF_Control(int led_number, uint8_t led_mode)
{
//    int led_status = 0;
	  switch(led_number)
		{
			case RED:
				if(led_mode == 1)
				{
             LED0_ON;
				}
				else
				{
					   LED0_OFF;
				}

				break;
			
			case GREEN:
				if(led_mode == 1)
				{
//             LED1_ON;
				}
				else
				{
//					   LED1_OFF;
				}
				break;
			
			case YELLOW:
				if(led_mode == 1)
				{
//             LED2_ON;
				}
				else
				{
//					   LED2_OFF;
				}
				break;
			
			case BLUE:
				if(led_mode == 1)
				{
//             LED3_ON;
				}
				else
				{
//					   LED3_OFF;
				}
				break;
				
			default:
				
				break;
		}

}

/*********************************************END OF FILE**********************/
