#include "main.h"
#include "stm32f10x.h"
#include "esp8266_init.h"
#include "esp8266_handler.h"
#include "mqtt.h"

xQueueHandle xQueue_USART3_Cmd;                 
xQueueHandle xQueue_USART1_Cmd;                

int main ( void )
{
	//硬件初始化
	BSP_Init();
	//WiFi模块复位引脚初始化
	esp8266_init();
	
	
	//创建串口3和串口1消息队列
	xQueue_USART3_Cmd = xQueueCreate(1, 512);
	xQueue_USART1_Cmd = xQueueCreate(1, 512);
	
	//创建串口数据解析任务
	xTaskCreate( vAnalysisUartData,   	"vAnalysisUartData",  	128, NULL, configMAX_PRIORITIES - 2, NULL);
	//创建8266 WiFi模块状态机任务
	xTaskCreate( vEsp8266_Main_Task,   	"vEsp8266_Main_Task",  	128, NULL, configMAX_PRIORITIES - 3, NULL);
	//创建8266 MQTT状态机任务
	xTaskCreate( vMQTT_Handler_Task,   	"vMQTT_Handler_Task",  	256, NULL, configMAX_PRIORITIES - 4, NULL);
	//创建MQTT订阅数据任务
	xTaskCreate( vMQTT_Recive_Task,   	"vMQTT_Recive_Task",  	128, NULL, configMAX_PRIORITIES - 5, NULL);
	//创建MQTT发布数据任务
	xTaskCreate( MqttTranmitTask,		    "MqttTranmitTask",		256, NULL, configMAX_PRIORITIES - 5, NULL);
	//创建传感器数据采集任务
	xTaskCreate( vCollectSensorTask,   	"vCollectSensorTask",  	128, NULL, configMAX_PRIORITIES - 3, NULL);
	//开启任务调度
	vTaskStartScheduler();
	
}
