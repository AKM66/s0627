#ifndef __MQTT_H__
#define __MQTT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "FreeRTOS.h"
#include "e:\progect_private\arm32\shixun\s0627\6\MQTTPacket\src\MQTTPacket.h"
#include "transport.h"

//#define MQTT_SERVER_HOST            "adtwuzp.iot.gz.baidubce.com"					//ʵ�ʴ������޸ĳ��Լ��ķ���������
//#define MQTT_SERVER_PORT            1883																	//ʵ�ʴ������޸ĳ��Լ��ķ������˿�
//#define MQTT_ALIVE_INTERVAL         60																		//ʵ�ʴ������޸ĳ��Լ�������ʱ�䣨�룩
//#define MQTT_CLIENTID								"KD-IOT005"
//#define MQTT_USER										"thingidp@adtwuzp|KD-IOT005|0|MD5"
//#define MQTT_PASSWORD 							"de2aacbdf26c99c96d6a5c8aff532c98"	
//#define MQTT_TOPIC_BASE             "$iot/KD-IOT008/user/send"					  //ʵ�ʴ������޸ĳ��Լ��Ķ�������
//#define MQTT_TOPIC_PUBLISH			    "$iot/KD-IOT008/user/update"				  //ʵ�ʴ������޸ĳ��Լ��ķ�������	


#define MQTT_SERVER_HOST            "9a52057bbf.st1.iotda-device.cn-north-4.myhuaweicloud.com"
#define MQTT_SERVER_PORT            1883							
#define MQTT_ALIVE_INTERVAL         60								
#define MQTT_USER                  	"686096dfd582f20018362af4_StarFire" 				
#define MQTT_PASSWORD               "b1154741460537a3285ce93c5f8e01fe619d3d4adb0f2683177ce0ab30a72c69"					
#define MQTT_TOPIC_BASE             "$oc/devices/686096dfd582f20018362af4_StarFire/sys/messages/down"		
#define MQTT_TOPIC_PUBLISH			    "$oc/devices/686096dfd582f20018362af4_StarFire/sys/properties/report"		
#define MQTT_CLIENTID								"686096dfd582f20018362af4_StarFire_0_0_2025062901"			

static char MQTT_TOPIC_PUB[64] = {0};
typedef struct
{
    unsigned char dup;
    int qos;
    unsigned char retained; 
    unsigned short msgid;  
    int payloadlen_in;
    unsigned char* payload_in;
    MQTTString receivedTopic;
} MQTT_Recv_t;

extern xQueueHandle xQueue_MQTT_Recv;

/*Mqtt״̬�ṹ��*/
typedef struct
{
	uint8_t connect;
	int socket;
}MqttSta_E;

extern MqttSta_E mqtt_status;

extern uint32_t pingRespTickCount;

void MQTT_Init(void);
int MQTT_Connect(void);
void vMQTT_Handler_Task(void *ptr);
void vMQTT_Recive_Task(void *ptr);
void MqttTranmitTask(void *ptr);
void myMQTT_Publish(char *pPubTopic, char *pMessage);

#ifdef __cplusplus
}
#endif

#endif /*__MQTT_CONFIG_H__*/
