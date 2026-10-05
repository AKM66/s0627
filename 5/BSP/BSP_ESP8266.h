#ifndef __BSP_ESP8266_H__
#define __BSP_ESP8266_H__

#include "FreeRTOS.h"
#include "queue.h"

#include "BSP_UART.h"
#include "BSP_DebugUART.h"

#include "UserTask.h"


#define ESP_ERROR_CNT			10

/* SmartConfigrationTime(MS) */
#define SMART_CONFIG_TIME		10000

/* ���ڷ��ͳ�ʱ(MS) */
#define SEND_TIMEOUT_TIME		200

/* ���ڷ��ͼ��(MS) */
#define USART_SEND_INTERVAL		1000

/* ������������ */
#define SendDataCmd						("AT+CIPSEND=%d\r\n")

#define USER_PRINT_BASE( format, args... )  printf( format, ##args )
#define debug_log( format, args... ) USER_PRINT_BASE( "%s_%d:"format"\r\n",__FILE__,__LINE__, ##args )

#define USER_PRINT_BASE( format, args... )  printf( format, ##args )
#define debug_log( format, args... ) USER_PRINT_BASE( "%s_%d:"format"\r\n",__FILE__,__LINE__, ##args )

/* ESP8266Ӳ��״̬���� */
typedef enum
{
	ESP_HW_RESERVE_0 = 0,
	ESP_HW_RESERVE_1,
	ESP_HW_GET_IPADDR,
	ESP_HW_CONNECT_OK,
	ESP_HW_DISCONNECT,
	ESP_HW_LOST_WIFI,
	ESP_HW_BUSY_STUS,
	ESP_HW_NOT_RESP,
	ESP_HW_RECONFIG
}ESP_HW_STATUS_E;

/* ESP8266����״̬���� */
typedef enum
{
	ESP_NETWORK_FAILED = 0,
	ESP_NETWORK_SUCCESS,
}ESP_NET_WORK_E;

/* ESP8266״̬���� */
typedef struct
{
	ESP_HW_STATUS_E esp_hw_status_e;	//Ӳ������״̬
	ESP_NET_WORK_E esp_net_work_e;		//MQTT����������״̬
}ESP_STATUS_T;

typedef struct
{
	uint8_t err_esp_disconnect;			//TCP����
	uint8_t err_esp_lostwifi;			//WIFI�źŹر�
	uint8_t err_esp_notresp;			//esp8266��������Ӧ
	uint8_t err_esp_network;
}ESP_ERROR_T;

typedef enum
{
	MQTT_IDLE = 0,
	MQTT_CONNECT,
	MQTT_SUBSCRB,
	MQTT_PUBLSH,
}MQTT_STATUS_E;

typedef struct
{
	char cmd_resp[100];					//����ظ�ָ�롣�����ȡ����ص����ݣ�������ȡ��ָ���ڵ�����
    char *cmd_hdl;					//��������ڷ�������󣬻��ڷ��������������û�ָ���ķ�������
} NET_DEVICE_INFO_T;

extern NET_DEVICE_INFO_T netDeviceInfo_t;

extern ESP_STATUS_T g_esp_status_t;

uint8_t net_device_send_cmd(char *cmd, char *res);

void ReconfigWIFI(void);

uint8_t ConnectTcp(void);

void esp8266_cmd_handle(char *cmd);

ESP_HW_STATUS_E check_esp8266_status(void);

void getTcpConnect(void);

int Esp8266_Tcp_Send(int socket, uint8_t *data, uint16_t len);

uint8_t Esp8266_GetTcpStatus(void);

void USART3_SendData(const uint8_t* const pData, uint32_t len);

#endif
