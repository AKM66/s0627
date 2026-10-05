#ifndef __USER_INFO_H__
#define __USER_INFO_H__
#include "stdint.h"
//char MQTT_CLIENTID[10];
//char MQTT_USER[35];
//char MQTT_PASSWORD[35]; 
//char MQTT_TOPIC_BASE[25]="$iot/KD-IOT008/user/send";											//实际代码请修改成自己的订阅主题
//char MQTT_TOPIC_PUBLISH[27]="$iot/KD-IOT008/user/update";

//#define MQTT_SERVER_HOST            "adtwuzp.iot.gz.baidubce.com"					//实际代码请修改成自己的服务器域名
//#define MQTT_SERVER_PORT            1883																	//实际代码请修改成自己的服务器端口
//#define MQTT_ALIVE_INTERVAL         60																		//实际代码请修改成自己的心跳时间（秒）
//#define MQTT_TOPIC_BASE             "$iot/KD-IOT008/user/send"					//实际代码请修改成自己的订阅主题
//#define MQTT_TOPIC_PUBLISH			    "$iot/KD-IOT008/user/update"				//实际代码请修改成自己的发布主题


void Set_Mqtt_Info(uint32_t id);
#endif
