/*******************************************************************************
 * Copyright (c) 2014 IBM Corp.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * and the Eclipse Distribution License is available at
 *   http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    Ian Craggs - initial API and implementation and/or initial documentation
 *    Sergio R. Caprile - clarifications and/or documentation extension
 *******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "stddef.h"
#include "esp8266_init.h"
#include "esp8266_handler.h"
#include "MQTTPacket.h"
#include "transport.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "timers.h"
#include "mqtt.h"
#include "main.h"
#include "data_handler.h"
#include "cJSON.h"
#include <signal.h>
#include <string.h>
#include "lcd.h"
#include "BSP_Smoke.h"
//#include "BSP_RE200B.h"
#include "BSP_LEDBuzzer.h"
#include "BSP_BH1750.h"
#include "user_info.h"
#include "BSP_DHT11.h"
#include "BSP_DS18B20.h"
#include "BSP_DigitalTube.h"
#include "BSP_Tracking.h"
MqttSta_E mqtt_status;
static uint8_t g_transmit = 0;
MQTT_STATUS_E Mqtt_status_step = MQTT_IDLE;
int toStop = 0;
extern int clientID;
MQTTString pub_topicString = MQTTString_initializer;
static char MQTT_TOPIC[64] = {0};
static char CLIENT_ID[32]= {0};

xQueueHandle xQueue_MQTT_Recv;  /**< MQTT接收消息队列 */
extern xQueueHandle xQueue_USART1_Cmd;                 /**< 串口命令消息队列*/
uint32_t pingRespTickCount;

extern ESP_STATUS_T g_esp_status_t ;
uint8_t show_data;
volatile float shtData_humi,shtData_temp;
uint32_t BH1750_Data;

/**< function code define >*/
typedef enum{
    LED_OnOFF_CTRL = 1,
	  LED_STATUE_READ,
	  AIR_CONDICTION_CTRL
}function_code_t;

extern unsigned char led_state[10];
extern unsigned char buzzer_state[10];
extern unsigned char alarmlamp_state[15];

uint8_t Array[] = {0xAA,0xFF,0x06,0x01,0xA3,0x00,0x00,0x00,0x00,0xFF};
/**
 ******************************************************************************
 * @brief  Sign接收函数
 * @param  iSignNo sign�?
 * @return None.
 ******************************************************************************/
void SignHandler(int iSignNo)
{
    printf("Capture sign no:%d ", iSignNo);
}


//static void BSP_TIME_Dly(void)
//{
//	volatile uint16_t i;
//	for(i = 500; i > 0; i--);
//}
/**
 ******************************************************************************
 * @brief  MQTT内部调用
 * @param  sig �?
 * @return None.
 ******************************************************************************/

void cfinish(int sig)
{
    signal(SIGINT, SignHandler);
    toStop = 1;
}

/**
 ******************************************************************************
 * @brief  MQTT内部调用
 * @return None.
 ******************************************************************************/
void stop_init(void)
{
    signal(SIGINT, cfinish);
    signal(SIGTERM, cfinish);
}

/**
 ******************************************************************************
 * @brief  MQTT初始�?
 * @return None.
 ******************************************************************************/
void MQTT_Init(void)
{
    ReadChipID(CLIENT_ID, sizeof(CLIENT_ID));

    snprintf(MQTT_TOPIC, sizeof(MQTT_TOPIC), "%s%s", MQTT_TOPIC_BASE, CLIENT_ID);
    debug_log("MQTT_TOPIC is: %s", MQTT_TOPIC);
//    MQTT_RB_Init();

    snprintf(MQTT_TOPIC_PUB,sizeof(MQTT_TOPIC_PUB),"%s%s",MQTT_TOPIC_PUBLISH,CLIENT_ID);
    pub_topicString.cstring = MQTT_TOPIC_PUB;
    debug_log("publish topic: %s",pub_topicString.cstring);
    stop_init();
}


/**
 ******************************************************************************
 * @brief  MQTT连接服务�?
* @return < 0,连接错误，其�?返回socket
 ******************************************************************************/
int MQTT_Connect(void)
{
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    unsigned char buf[300];
    int buflen = sizeof(buf);
    int len = 0;
    char *host = MQTT_SERVER_HOST;
    int port = MQTT_SERVER_PORT;

    debug_log("Sending to hostname:%s,port:%d", host, port);


    data.clientID.cstring = MQTT_CLIENTID;//CLIENT_ID;
    data.keepAliveInterval = MQTT_ALIVE_INTERVAL;
    data.cleansession = 1;
    data.username.cstring = MQTT_USER;
    data.password.cstring = MQTT_PASSWORD;

    len = MQTTSerialize_connect(buf, buflen, &data);


    transport_sendPacketBuffer(mqtt_status.socket, buf, len);

    /* wait for connack */
    if (MQTTPacket_read(buf, buflen, transport_getdata) == CONNACK)
    {
        unsigned char sessionPresent, connack_rc;

        if (MQTTDeserialize_connack(&sessionPresent, &connack_rc, buf, buflen) != 1 || connack_rc != 0)
        {
            debug_log("Unable to connect, return code %d", connack_rc);
            goto exit;
        }
    }
    else
        goto exit;

    debug_log("Connected to MQTT server");
    return mqtt_status.socket;

exit:
//    transport_close(mqtt_status.socket);
    return -1;
}

/**
 ******************************************************************************
 * @brief  订阅主题
 * @param  topic_str 主题�?
 * @return the length of the serialized data.  <= 0 indicates error
 ******************************************************************************/
int MQTT_Subscribe(char* topic_str)
{
    MQTTString topicString = MQTTString_initializer;

    int len = 0;
    unsigned char buf[200];
    int buflen = sizeof(buf);
    int msgid = 1;
    int req_qos = 0;
    int rc;

    topicString.cstring = topic_str;
//    debug_log("MQTT_SubscribeTopic:%s",topicString.cstring);
    len = MQTTSerialize_subscribe(buf, buflen, 0, msgid, 1, &topicString, &req_qos);

    transport_sendPacketBuffer(mqtt_status.socket, buf, len);
//    vTaskDelay(2000 / portTICK_RATE_MS);
    if ((rc = MQTTPacket_read(buf, buflen, transport_getdata)) == SUBACK) 	/* wait for suback */
    {
        unsigned short submsgid;
        int subcount;
        int granted_qos;

        rc = MQTTDeserialize_suback(&submsgid, 1, &subcount, &granted_qos, buf, buflen);
        if (granted_qos != 0)
        {
            debug_log("granted qos != 0, %d", granted_qos);
            goto exit;
        }
    }
    else
    {
        debug_log("MQTT_Subscribe Error %d", rc);
        goto exit;
    }
	
exit:
    return rc;
}


/**
 ******************************************************************************
 * @brief 读取主题，阻塞方�?
 * @return the length of the serialized data.  <= 0 indicates error
 ******************************************************************************/
unsigned short MQTT_packid = 0;
int MQTT_Read(void)
{
    int rc;
    static unsigned char buf[200] = {0};
    int buflen = sizeof(buf);

    while (!toStop)
    {
        /* transport_getdata() has a built-in 1 second timeout, your mileage will vary */
        rc = MQTTPacket_read(buf, buflen, transport_getdata);
        debug_log("MQTTPacket_read rc = %d",rc);
        if (rc == PUBLISH)
        {
            
            MQTT_Recv_t mqtt_recv_t;
			
			
			debug_log("----------PUBLISH----------");	
            MQTTDeserialize_publish(&mqtt_recv_t.dup, &mqtt_recv_t.qos, &mqtt_recv_t.retained, &mqtt_recv_t.msgid, &mqtt_recv_t.receivedTopic,
                                    &mqtt_recv_t.payload_in, &mqtt_recv_t.payloadlen_in, buf, buflen);
            xQueueSend(xQueue_MQTT_Recv,  &mqtt_recv_t,  5);
					
        }
        else if(rc == PINGRESP)
        {
            debug_log("Recv MQTT PINGRESP");
            pingRespTickCount = 0;
        }
        else if(rc == DISCONNECT)
        {
            debug_log("MQTT Disconnect");
            goto exit;
        }
        else if(rc == -1)
        {
            debug_log("MQTT Disconnect2");
            goto exit;
        }
        else
        {
            debug_log("MQTT %d", rc);
        }
		
		

		memset(mqttSubscribeData,0,sizeof(mqttSubscribeData));
		
        vTaskDelay(20 / portTICK_RATE_MS);
    }
exit:
    return rc;
}



/**
 ******************************************************************************
 * @brief 发布主题
 * @param dup integer - the MQTT dup flag
 * @param qos integer - the MQTT QoS value
 * @param retained integer - the MQTT retained flag
 * @param packetid integer - the MQTT packet identifier
 * @param topicName MQTTString - the MQTT topic in the publish
 * @param payload byte buffer - the MQTT publish payload
 * @param payloadlen integer - the length of the MQTT payload
 * @return the length of the serialized data.  <= 0 indicates error
 ******************************************************************************/
int MQTT_Publish(unsigned char dup, int qos, unsigned char retained, unsigned short packetid,
                 MQTTString topicName, unsigned char* payload, int payloadlen)
{
    int len, rc;
    unsigned char buf[256];
    int buflen = 256;
//    debug_log("MQTT_Publish");
    if(MQTT_GetConnected())
    {
        len = MQTTSerialize_publish(buf, buflen, dup, qos, retained, packetid, topicName, payload, payloadlen);
		printf("len:%d\r\n",len);
        //len = MQTTSerialize_publish(buf, buflen, 0, 0, 0, 0, topicString, (unsigned char*)payload, payloadlen);
        rc  = transport_sendPacketBuffer(mqtt_status.socket, buf, len);
        return rc;
    }
    else
    {
        debug_log("MQTT Disconneted");
        return -1;
    }
}


/**
 ******************************************************************************
 * @brief  MQTT发送心跳，采用定时触发
 * @param  xTimer
 * @retval None.
 ******************************************************************************/
//void MQTT_PingREQ(xTimerHandle xTimer)
void MQTT_PingREQ(void)
{
    unsigned char buf[200];
    int buflen = sizeof(buf);
    int len;

    if(MQTT_GetConnected())
    {
        printf("PING REQ");

        len = MQTTSerialize_pingreq(buf, buflen);
        transport_sendPacketBuffer(mqtt_status.socket, buf, len);
    }
}

/**
 ******************************************************************************
 * @brief  获取连接状�?
 * @retval 连接状�?
 ******************************************************************************/
uint8_t MQTT_GetConnected(void)
{
    uint8_t i;

    if(g_esp_status_t.esp_net_work_e == ESP_NETWORK_SUCCESS)
    {
        i = 1;
    }
    else
    {
        i = 0;
    }
    return i;
}
/**
******************************************************************************
* @brief  MQTT??????????
* @retval None.
******************************************************************************/
void MqttXqueueGreat(void)
{
    xQueue_MQTT_Recv = xQueueCreate(5, sizeof(MQTT_Recv_t));
    if(xQueue_MQTT_Recv == NULL)
    {
        debug_log("xQueue_MQTT_Recv ERROR");
        while(1);
    }
}

void MqttHandle(void)
{
    MQTT_Init();
    MqttXqueueGreat();
}


void vMQTT_Handler_Task(void *ptr)
{
    int rc = -1;
    while(1)
    {
        switch(Mqtt_status_step)
        {
            case MQTT_IDLE:
            {
                MqttHandle();
                memset(mqttSubscribeData,0,sizeof(mqttSubscribeData));
                Mqtt_status_step = MQTT_CONNECT;
                g_transmit = 0;
//                debug_log("mqtt satus is idle");
            }
            break;
            case MQTT_CONNECT:
            {
                if(g_esp_status_t.esp_hw_status_e ==ESP_HW_CONNECT_OK )
                {
                    lastSendOutTime =  xTaskGetTickCount();
                    rc = MQTT_Connect();
                    if(rc == -1)
                    {
                        vTaskDelay(200 / portTICK_RATE_MS);
                        continue;
                    }
                    else
                    {
                        g_esp_status_t.esp_net_work_e = ESP_NETWORK_SUCCESS;
                        Mqtt_status_step = MQTT_SUBSCRB;
												debug_log("MQTT Enter subscrib!!! ");
//                        memset(mqttSubscribeData,0,sizeof(mqttSubscribeData));
                    }
                }
                vTaskDelay(200 / portTICK_RATE_MS);
            }
            break;
            case MQTT_SUBSCRB:
            {
                lastSendOutTime =  xTaskGetTickCount();
                rc = MQTT_Subscribe(MQTT_TOPIC_BASE);
				debug_log("=====MQTT_Subscribe rc is %d!!!=====",rc);
                if(rc != -1)
                {
                    Mqtt_status_step = MQTT_PUBLSH;
                    memset(mqttSubscribeData,0,sizeof(mqttSubscribeData));
					debug_log("=====Mqtt_status_step switch to publish!!!=====");
                }
                else
                {
                    debug_log("MQTT Subscribe Error");
                }
                vTaskDelay(200 / portTICK_RATE_MS);
            }
            break;
            case MQTT_PUBLSH:
            {
				debug_log("======Start publish data!!!======");
                g_transmit = 1;
                MQTT_Read();
                if(g_esp_status_t.esp_net_work_e != ESP_NETWORK_SUCCESS)
                {
                    debug_log("mqtt connect is failed");
                }
            }
            break;
            default:
                break;
        }
        vTaskDelay(1000 / portTICK_RATE_MS);
    }
}

/**< handler MQTT sub topic message 2020-03-05 Nanford add*/
void mqtt_sub_message_handler(char* payload_data, int data_length)
{
	cJSON* root_json = NULL;
//	cJSON* child_json = NULL;
	cJSON* value_json = NULL;
//	cJSON* string_json = NULL;
	cJSON* led_json = NULL;
	cJSON* buzzer_json = NULL;
	cJSON* alarmlamp_json = NULL;
//	int function_code = 0;
	
	root_json = cJSON_Parse(payload_data);
	if(root_json)
	{
      value_json = cJSON_GetObjectItem(root_json, "desired");	
		  printf("**********\r\n");
		  printf("%s\n",cJSON_Print(value_json));

		
		  led_json = cJSON_GetObjectItem(value_json, "LED");	
			buzzer_json = cJSON_GetObjectItem(value_json, "BUZZER");
		  alarmlamp_json = cJSON_GetObjectItem(value_json, "ALARMLAMP");
	  	printf("%s\n",led_json->valuestring);
		  printf("%s\n",buzzer_json->valuestring);
		  printf("%s\n",alarmlamp_json->valuestring);
		
		  if(strcmp(led_json->valuestring,"off")==0)
			{
				printf("led turn off\r\n");
				BSP_LED_Off(0);
				strcpy((char *)led_state,"ledoff");
			}
			if(strcmp(led_json->valuestring,"on")==0)
			{
				printf("led turn on\r\n");
				BSP_LED_On(0);
				strcpy((char *)led_state,"ledon");
			}
			if(strcmp(buzzer_json->valuestring,"off")==0)
			{
				printf("buzzer turn off\r\n");
				BSP_Buzzer_Off();
				strcpy((char *)buzzer_state,"buzzeroff");		
			}
			if(strcmp(buzzer_json->valuestring,"on")==0)
			{
				printf("buzzer turn on\r\n");
				BSP_Buzzer_On();
				strcpy((char *)buzzer_state,"buzzeron");
			}
			if(strcmp(alarmlamp_json->valuestring,"off")==0)
			{
				printf("alarmlamp turn off\r\n");
				strcpy((char *)alarmlamp_state,"alarmlampoff");				
			}
			if(strcmp(alarmlamp_json->valuestring,"on")==0)
			{
				printf("alarmlamp turn on\r\n");

				strcpy((char *)alarmlamp_state,"alarmlampon");
			}
	}
	cJSON_Delete(root_json);
}	


void vMQTT_Recive_Task(void *ptr)
{
	
    MQTT_Recv_t mqtt_recv_t;
    signed portBASE_TYPE pd;
		cJSON* value_json = NULL;
    while(1)
    {
        pd = xQueueReceive(xQueue_MQTT_Recv, &mqtt_recv_t, 20 / portTICK_RATE_MS);
        if(pd != NULL)
        {
            uint8_t buff[100] = {0};
            printf("mqtt_recv_t:%s\r\n",mqtt_recv_t.payload_in);
            memcpy(buff,mqtt_recv_t.payload_in,mqtt_recv_t.payloadlen_in);
						value_json = cJSON_Parse((const char *)buff);
						printf("%s\n",cJSON_Print(value_json));
        }
		
		
        vTaskDelay(20 / portTICK_PERIOD_MS);
    }
}

/* 串口接收数据缓冲�?*/
char gUsart1ReciveLineBuf[200] = {0};


void MqttTranmitTask(void *ptr)
{
	int rc,len;
  static unsigned char buf[200] = {0};
  int buflen = sizeof(buf);
	char payload[500]; 
	
	unsigned char Temperature[20];
	unsigned char Humidity[20];
	typedef struct{
		char Temperature[20];
		char Humidity[20];
		char ds18b20[20];
		char service_id[20];
		char service_id_data[20];
	}Data_t;
	
	Data_t data;

	memset(&Temperature, 0, sizeof(Temperature));
	memset(&Humidity, 0, sizeof(Humidity));
	strcpy((char *)Temperature,"temperature");
	strcpy((char *)Humidity,"humidity");

	memset(data.Temperature, 0, sizeof(data.Temperature));
	memset(data.Humidity, 0, sizeof(data.Humidity));
	memset(data.ds18b20,0,sizeof(data.ds18b20));
	memset(data.service_id,0,sizeof(data.service_id));
	
	strcpy(data.service_id,"service_id");
	strcpy(data.Temperature,"DHT11_T");
	strcpy(data.Humidity,"DHT11_H");
	strcpy(data.ds18b20,"DS18B20");
	strcpy(data.service_id_data,"stm32");
	MQTTString topicString1 = MQTTString_initializer;
	topicString1.cstring = MQTT_TOPIC_PUBLISH;

	
	while(1)
	{

			if(g_esp_status_t.esp_net_work_e == ESP_NETWORK_SUCCESS)
			{
					if(g_transmit)
					{
						if((result > 0.0f) &&(result < 100.0f))
						{
		//		"{\"services\":[{\"service_id\":\"stm32\",\"properties\":{\"DHT11_T\":27.4,\"DHT11_H\":55.3}}]}"
							sprintf(payload,"{\"services\":[{\"%s\":\"%s\",\"properties\":{\"%s\":%.1f,\"Tracking\":\"%s\"}}]}", \
						data.service_id, data.service_id_data, data.ds18b20, result,trackbuf);	
						}							
//				sprintf(payload,"{\"services\":[{\"%s\":\"%s\",\"properties\":{\"%s\":%.1f,\"%s\":%.1f}}]}", \
//				data.service_id, data.service_id_data, data.Temperature, 35.8f,\
//				data.Humidity, 24.7f);						
						len = MQTTSerialize_publish(buf, buflen, 0, 0, 0, 0, topicString1, (unsigned char *)payload, strlen(payload));
						printf("MQTTserialize_publish len1 = %d\r\n",len);
						rc = transport_sendPacketBuffer(mqtt_status.socket, buf, len);
						printf("rc = %d\r\n",rc);
					}
			}
			vTaskDelay(2000 / portTICK_PERIOD_MS);
	}
}

void vCollectSensorTask(void *ptr)
{
   while (1)
   {
		 SenSorData_Output();
		 Print_Tracking_Status();
		 vTaskDelay(1000/portTICK_RATE_MS);
   }
}

void vShowSenSorDataTask(void *ptr)
{
	while (1)
  {
		Show_Temperature(result);
    vTaskDelay(5/portTICK_RATE_MS);
	}
}

