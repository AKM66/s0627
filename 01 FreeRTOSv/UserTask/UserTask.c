#include "stm32f10x.h"
#include "UserTask.h"
#include "dsgshow.h"
#include "BSP_DS18B20.h"
#include "BSP_DebugUART.h"
#include "LED.h"
#include "BSP_Delay.h"

static void  vTaskDigtubeShow1(void *pvParameters);
static void  vTaskDS18B20(void *pvParameters);
static void  vTaskLED(void *pvParameters);
//����
 void UserTaskCreate (void)
{
	xTaskCreate(  vTaskDigtubeShow1,     		/* ������  */
                 "vTaskDigtube1",   		/* ������    */
                 128,             		/* ����ջ��С����λword��Ҳ����4�ֽ� */
                 NULL,           		/* �������  */
                 3,               		/* �������ȼ�*/
                 NULL);  /* ������  */
	
	xTaskCreate( vTaskDS18B20,     		/* ������  */
                 "vTaskDigtube2",   		/* ������    */
                 128,             		/* ����ջ��С����λword��Ҳ����4�ֽ� */
                 NULL,           		/* �������  */
                 4,               		/* �������ȼ�*/
                 NULL);  /* ������  */

	
	xTaskCreate( vTaskLED,     		/* ������  */
                 "vTaskLED",   		/* ������    */
                 128,             		/* ����ջ��С����λword��Ҳ����4�ֽ� */
                 NULL,           		/* �������  */
                 4,               		/* �������ȼ�*/
                 NULL);  /* ������  */
								 
  /* �������ȣ���ʼִ������ */
  vTaskStartScheduler();
}

static void  vTaskDigtubeShow1(void *pvParameters)    //ÿ�������ִ��һ��
{
    while(1)
    {
//			printf("in vTaskLED1\r\n");
			Show_Temperature(result); //��ʾ���������   
			vTaskDelay(5);
    }
}

static void  vTaskDS18B20(void *pvParameters)      //ÿ��2000����ִ��һ��
{
    while(1)
    {
			printf("in vTaskLED2\r\n");
			SenSorData_Output();  //�ɼ�����������
			vTaskDelay(1000);
    }
}

static void  vTaskLED(void *pvParameters)      //ÿ��2000����ִ��һ��
{
  int i;  
	while(1)
    {
      for(i=1;i<8;i++)
			{
				GPIO_ResetBits(GPIOC,LED_Pin[i] );
			}
			BSP_Delay_ms(1000);
			 for(i=1;i<8;i++)
			{
				GPIO_SetBits(GPIOC,LED_Pin[i] );
			}
			vTaskDelay(1500);
    }
}
