#ifndef __MQTT_H__
#define __MQTT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "FreeRTOS.h"
#include "MQTTPacket.h"
#include "transport.h"

//#define MQTT_SERVER_HOST            "adtwuzp.iot.gz.baidubce.com"					//??????????????????????????
//#define MQTT_SERVER_PORT            1883																	//?????????????????????????
//#define MQTT_ALIVE_INTERVAL         60																		//??????????????????????????
//#define MQTT_CLIENTID								"KD-IOT005"
//#define MQTT_USER										"thingidp@adtwuzp|KD-IOT005|0|MD5"
//#define MQTT_PASSWORD 							"de2aacbdf26c99c96d6a5c8aff532c98"	
//#define MQTT_TOPIC_BASE             "$iot/KD-IOT008/user/send"					  //????????????????????????
//#define MQTT_TOPIC_PUBLISH			    "$iot/KD-IOT008/user/update"				  //????????????????????????	


#define MQTT_SERVER_HOST            "9a52057bbf.st1.iotda-device.cn-north-4.myhuaweicloud.com"
#define MQTT_SERVER_PORT            1883							
#define MQTT_ALIVE_INTERVAL         60								
#define MQTT_USER                  	"6860926432771f177b46e818_stmSF" 				//用户名
#define MQTT_PASSWORD               "c8cbaecaa06df1abd1034e8a925009aae6f64dcdf7974a1e3c22dc1847cd2c3e"		//    //密码			
#define MQTT_TOPIC_BASE             "$oc/devices/6860926432771f177b46e818_stmSF/sys/messages/down"		
#define MQTT_TOPIC_PUBLISH			    "$oc/devices/6860926432771f177b46e818_stmSF/sys/properties/report"		
#define MQTT_CLIENTID								"6860926432771f177b46e818_stmSF_0_0_2025062901"			//设备ID

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

/*Mqtt??????*/
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
