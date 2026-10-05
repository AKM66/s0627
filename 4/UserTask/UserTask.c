#include "stm32f10x.h"
#include "UserTask.h"
#include "dsgshow.h"
#include "BSP_DS18B20.h"
#include "BSP_DigitalTube.h"
#include "LED.h"
#include "BSP_UART.h"


char gUsartReciveLineBuf[200] = {0};
xQueueHandle xQueue_USART3_Cmd;

static void vTaskDigtubeShow1(void *pvParameters);
static void vTaskDS18B20(void *pvParameters);
static void vTaskLED(void *pvParameters);
static void vTaskReceive(void *pvParameters);

void UserTaskCreate(void)
{

  // 创建串口3和串口1消息队列s
  xQueue_USART3_Cmd = xQueueCreate(1, 512);

  xTaskCreate(vTaskDigtubeShow1, /* 任务函数  */
              "vTaskDigtube1",   /* 任务名    */
              128,               /* 任务栈大小，单位word，也就是4字节 */
              NULL,              /* 任务参数  */
              3,                 /* 任务优先级*/
              NULL);             /* 任务句柄  */

  xTaskCreate(vTaskDS18B20,
              "vTaskDigtube2",
              128,
              NULL,
              4,
              NULL);

  xTaskCreate(vTaskLED,
              "vTaskLED",
              128,
              NULL,
              4,
              NULL);
  xTaskCreate(vTaskReceive,
              "vTaskReceive",
              256,
              NULL,
              5,
              NULL);


  /* 启动调度，开始执行任务 */
  vTaskStartScheduler();
}

static void vTaskDigtubeShow1(void *pvParameters) // 每隔5ms执行一次
{
  while (1)
  {
    printf("in vTaskLED1\r\n");
    Show_Temperature(result); // 显示数码管数据
    vTaskDelay(8);
  }
}

static void vTaskDS18B20(void *pvParameters) // 每隔2000ms执行一次
{
  while (1)
  {
    //			printf("in vTaskLED2\r\n");
    SenSorData_Output(); // 采集传感器数据
    vTaskDelay(1011);
  }
}

static void vTaskLED(void *pvParameters)
{
  static int cnt = 0;
  static int cnt2 = 7;
  while (1)
  {
    GPIOC->ODR ^= LED_Pin[cnt++];
    GPIOC->ODR ^= LED_Pin[cnt2--];

    if (cnt == 8)
      cnt = 0;
    if (cnt == 0)
      cnt2 = 7;
    vTaskDelay(450);
  }
}

static void vTaskReceive(void *pvParameters)
{
  signed portBASE_TYPE pd;
  while (1)
  {
    pd =  xQueueReceive(xQueue_USART3_Cmd, gUsartReciveLineBuf, portMAX_DELAY);
    if(pd != pdTRUE)
    {
         printf("pd != pdTRUE\r\n");
         break;
    }
		printf("receive data is %s\r\n", gUsartReciveLineBuf);
    vTaskDelay(20);
  }
}

