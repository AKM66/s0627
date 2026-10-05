#include "UserTask.h"
#include "dsgshow.h"
#include "BSP_DS18B20.h"
#include "BSP_DigitalTube.h"
#include "LED.h"
#include "BSP_UART.h"
#include "OLED.H"
#include "BSP_Delay.h"
#include "BSP_DebugUART.h"
#include "BSP_ESP8266.h"
#include "esp8266_init.h"

static char LONG_CLIENT_ID[32] = {0};

// char gUsartReciveLineBuf[200] = {0};

/* ESP8266 错误次数初始化 */
static ESP_ERROR_T esp_error_t = {0, 0, 0, 0};

xQueueHandle xQueue_USART3_Cmd;

static void vTaskDigtubeShow(void *pvParameters);
static void vTaskDS18B20(void *pvParameters);
static void vTaskLED(void *pvParameters);
static void vESP8266Task(void *pvParameters);
static void vTaskReceive(void *pvParameters);

void UserTaskCreate(void)
{
  // 创建串口3和串口1消息队列s
  xQueue_USART3_Cmd = xQueueCreate(1, 512);

  xTaskCreate(vTaskDigtubeShow, /* 任务函数  */
              "vTaskDigtube1",  /* 任务名    */
              128,              /* 任务栈大小，单位word，也就是4字节 */
              NULL,             /* 任务参数  */
              3,                /* 任务优先级*/
              NULL);            /* 任务句柄  */

  xTaskCreate(vTaskDS18B20,
              "vTaskDS18B20",
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

  xTaskCreate(vESP8266Task,
              "vESP8266Task",
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

static void vTaskDigtubeShow(void *pvParameters) // 每隔5ms执行一次
{
  while (1)
  {
    // printf("in vTaskLED1\r\n");
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

static void vESP8266Task(void *pvParameter)
{

    while(1)
    {
        if(g_esp_status_t.esp_hw_status_e != ESP_HW_CONNECT_OK || g_esp_status_t.esp_hw_status_e != ESP_HW_RECONFIG)
        {
            g_esp_status_t.esp_hw_status_e = check_esp8266_status();
        }
				BSP_LED_Control(g_esp_status_t.esp_hw_status_e);
				
        switch(g_esp_status_t.esp_hw_status_e)
        {
            case ESP_HW_RESERVE_0:
            case ESP_HW_RESERVE_1:


            /* 获得IP */
            case ESP_HW_GET_IPADDR:
                getTcpConnect();
                break;
						
						case ESP_HW_CONNECT_OK:
								WiFi_Connected();
                break;

            /* 失去连接，断开tcp连接 */
            case ESP_HW_DISCONNECT:
								WiFi_Disconnected();
                esp_error_t.err_esp_disconnect++;
                g_esp_status_t.esp_net_work_e = ESP_NETWORK_FAILED;
                debug_log("TCP CLOSEED\r\n");
                vTaskDelay(3000 / portTICK_RATE_MS);//3秒后重新连接服务器
                /* 重新连接TCP服务器 */
                if(g_esp_status_t.esp_hw_status_e != ESP_HW_RECONFIG)	//如果不是配置状态，则断线重新连接
                {
                    getTcpConnect();
                }
                break;
            case ESP_HW_LOST_WIFI:
                esp_error_t.err_esp_lostwifi++;
                debug_log("ESP8266 Lost\r\n");			//设备丢失
						    ReconfigWIFI();
                break;
            case ESP_HW_BUSY_STUS:
                debug_log("--%s",netDeviceInfo_t.cmd_resp);
                break;
            case ESP_HW_NOT_RESP:
                esp_error_t.err_esp_notresp++;
                debug_log("USART3 send timeOut!");			//设备丢失
                break;
            default:
                break;

        }
		if(g_esp_status_t.esp_net_work_e == 0 )
		{
			esp_error_t.err_esp_network++;
		}
        if(esp_error_t.err_esp_disconnect > ESP_ERROR_CNT || esp_error_t.err_esp_lostwifi >
                ESP_ERROR_CNT || esp_error_t.err_esp_notresp > ESP_ERROR_CNT || esp_error_t.err_esp_network > ESP_ERROR_CNT )
        {
            //RESTART SYSTEM;
//					  debug_log("RESTART SYSTEM!");
//            NVIC_SystemReset(); //软件复位
        }

			debug_log("esp_hw_status_e=%d,esp_net_work_e=%d",g_esp_status_t.esp_hw_status_e,g_esp_status_t.esp_net_work_e);
			vTaskDelay(2000 / portTICK_RATE_MS);
    }

}

static void vTaskReceive(void *pvParameters)
{
  signed portBASE_TYPE pd;
  while (1)
  {
    pd = xQueueReceive(xQueue_USART3_Cmd, gUsartReciveLineBuf, portMAX_DELAY);
    if (pd != pdTRUE)
    {
      printf("pd != pdTRUE\r\n");
      break;
    }
    printf("receive data is %s\r\n", gUsartReciveLineBuf);
    vTaskDelay(20);
  }
}
