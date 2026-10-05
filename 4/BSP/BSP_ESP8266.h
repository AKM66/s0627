#ifndef  __BSP_ESP8266_H
#define	 __BSP_ESP8266_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>  
#include "stm32f10x.h"
#include "BSP_Delay.h"
#include "BSP_UART.h"

#if defined ( __CC_ARM  )
#pragma anon_unions
#endif

extern volatile uint8_t ucTcpClosedFlag;

/*************************** ESP8266 ??????????? ***************************/
typedef enum{
	STA,
	AP,
	STA_AP  
} ENUM_Net_ModeTypeDef;

typedef enum{
	 enumTCP,
	 enumUDP,
} ENUM_NetPro_TypeDef;

typedef enum{
	Multiple_ID_0 = 0,
	Multiple_ID_1 = 1,
	Multiple_ID_2 = 2,
	Multiple_ID_3 = 3,
	Multiple_ID_4 = 4,
	Single_ID_0 = 5,
} ENUM_ID_NO_TypeDef;

typedef enum{
	OPEN = 0,
	WEP = 1,
	WPA_PSK = 2,
	WPA2_PSK = 3,
	WPA_WPA2_PSK = 4,
} ENUM_AP_PsdMode_TypeDef;


/************************ ESP8266 ???????????? ***************************/
#define RX_BUF_MAX_LEN     1024                    //??????????????
extern struct  STRUCT_USARTx_Fram                  //??????????????????
{
	char  Data_RX_BUF [ RX_BUF_MAX_LEN ];
	
	union {
		__IO u16 InfAll;
		struct {
			__IO u16 FramLength       :15;          // 14:0 
			__IO u16 FramFinishFlag   :1;           // 15 
		} InfBit;
	}; 
	
} strEsp8266_Fram_Record;


/************************* ESP8266 ???????? *******************************/
#define  ESP8266_Usart( fmt, ...)    USART_printf ( USART3, fmt, ##__VA_ARGS__ ) 
#define  PC_Usart( fmt, ...)         printf ( fmt, ##__VA_ARGS__ )    


/************************* ESP8266 ???????? *********************************/
void BSP_ESP8266_Init( void );
void BSP_ESP8266_Rst( void );
bool BSP_ESP8266_Cmd( char * cmd, char * reply1, char * reply2, u32 waittime );
void BSP_ESP8266_ATTest( void );
bool BSP_ESP8266_NetModeChoose( ENUM_Net_ModeTypeDef enumMode );
bool BSP_ESP8266_JoinAP( char * pSSID, char * pPassWord );
bool BSP_ESP8266_BuildAP( char * pSSID, char * pPassWord, ENUM_AP_PsdMode_TypeDef enunPsdMode );
bool BSP_ESP8266_MultipleId( FunctionalState enumEnUnvarnishTx );
bool BSP_ESP8266_LinkServer( ENUM_NetPro_TypeDef enumE, char * ip, char * ComNum, ENUM_ID_NO_TypeDef id);
bool BSP_ESP8266_StartOrShutServer( FunctionalState enumMode, char * pPortNum, char * pTimeOver );
uint8_t BSP_ESP8266_GetLinkStatus( void );
uint8_t BSP_ESP8266_GetIdLinkStatus( void );
uint8_t BSP_ESP8266_InquireApIp( char * pApIp, uint8_t ucArrayLength );
bool BSP_ESP8266_UnvarnishSend( void );
void BSP_ESP8266_ExitUnvarnishSend( void );
bool BSP_ESP8266_SendString ( FunctionalState enumEnUnvarnishTx, char * pStr, u32 ulStrLength, ENUM_ID_NO_TypeDef ucId );
char * BSP_ESP8266_ReceiveString ( FunctionalState enumEnUnvarnishTx );

uint8_t BSP_ESP8266_CWLIF( char * pStaIp );
uint8_t BSP_ESP8266_CIPAP( char * pApIp );

#endif
