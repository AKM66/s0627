/*
***************************************************************************
*    模块：BSP_ESP8266 
*    描述：板级 ESP8266 功能模块驱动
          使用 USART3:TX3-PB10 RX3-PB11
*    作者：Huo
*    时间：2017.10.20
*    版本：UP-Magic-Version 1.0.0
***************************************************************************
*/
#include "BSP_ESP8266.h"

struct  STRUCT_USARTx_Fram strEsp8266_Fram_Record = { 0 };

volatile uint8_t ucTcpClosedFlag = 0;

/*
***************************************************************************
*	函 数 名: BSP_ESP8266_Init
*	功能说明: 板载 ESP8266 初始化函数
*	形    参: 无
*	返 回 值: 无
***************************************************************************
*/
void BSP_ESP8266_Init( void )
{
	BSP_UART_Init(115200); 
}

/*
***************************************************************************
*	函 数 名: BSP_ESP8266_Rst
*	功能说明: 板载 ESP8266 复位功能函数
*	形    参: 无
*	返 回 值: 无
***************************************************************************
*/
void BSP_ESP8266_Rst( void )
{
	BSP_ESP8266_Cmd ( "AT+RST", "OK", "ready", 2500 );   	
	BSP_Delay_ms ( 500 ); 
}

/*
***************************************************************************
*	函 数 名: BSP_ESP8266_Cmd
*	功能说明: 对 ESP8266 发送 AT指令 功能函数
*	形    参: cmd，待发送的指令
*             reply1，reply2，期待的响应，为NULL表不需响应，两者为或逻辑关系
*             waittime，等待响应的时间
*	返 回 值: 1，指令发送成功
*             0，指令发送失败
***************************************************************************
*/
bool BSP_ESP8266_Cmd(char * cmd,char * reply1,char * reply2,u32 waittime )
{    
	strEsp8266_Fram_Record .InfBit .FramLength = 0; //重新开始接收新的数据包

	ESP8266_Usart ( "%s\r\n", cmd );

	if ( ( reply1 == 0 ) && ( reply2 == 0 ) )       //不需要接收数据
		return true;
	
	BSP_Delay_ms ( waittime );                 //延时
	
	strEsp8266_Fram_Record .Data_RX_BUF [ strEsp8266_Fram_Record .InfBit .FramLength ]  = '\0';

	PC_Usart ( "%s", strEsp8266_Fram_Record .Data_RX_BUF );
  
	if ( ( reply1 != 0 ) && ( reply2 != 0 ) )
		return ( ( bool ) strstr ( strEsp8266_Fram_Record .Data_RX_BUF, reply1 ) || 
						 ( bool ) strstr ( strEsp8266_Fram_Record .Data_RX_BUF, reply2 ) ); 
 	
	else if ( reply1 != 0 )
		return ( ( bool ) strstr ( strEsp8266_Fram_Record .Data_RX_BUF, reply1 ) );
	
	else
		return ( ( bool ) strstr ( strEsp8266_Fram_Record .Data_RX_BUF, reply2 ) );
}

/*
***************************************************************************
*	函 数 名: BSP_ESP8266_ATTest
*	功能说明: 板载 ESP8266 AT测试 功能函数
*	形    参: 无
*	返 回 值: 无
***************************************************************************
*/
void BSP_ESP8266_ATTest ( void )
{
	char count=0;
	
	BSP_ESP8266_ExitUnvarnishSend ();
	while ( count < 10 )
	{
		if( BSP_ESP8266_Cmd ( "AT", "OK", NULL, 500 ) ) return;
		BSP_ESP8266_Rst();
		++ count;
	}
}

/*
***************************************************************************
*	函 数 名: BSP_ESP8266_NetModeChoose
*	功能说明: 板载 ESP8266 工作模式选择 功能函数
*	形    参: enumMode，工作模式
*	返 回 值: 1，选择成功
*             0，选择失败
***************************************************************************
*/
bool BSP_ESP8266_NetModeChoose ( ENUM_Net_ModeTypeDef enumMode )
{
	switch ( enumMode )
	{
		case STA:
			return BSP_ESP8266_Cmd ( "AT+CWMODE=1", "OK", "no change", 2500 ); 

		case AP:
		  return BSP_ESP8266_Cmd ( "AT+CWMODE=2", "OK", "no change", 2500 ); 

		case STA_AP:
		  return BSP_ESP8266_Cmd ( "AT+CWMODE=3", "OK", "no change", 2500 ); 

		default:
		  return false;
  }
}

/*
***************************************************************************
*	函 数 名: BSP_ESP8266_JoinAP
*	功能说明: 板载 ESP8266 连接路由器 功能函数
*	形    参: pSSID，WiFi名称字符串
*             pPassWord，WiFi密码字符串
*	返 回 值: 1，选择成功
*             0，选择失败
***************************************************************************
*/
bool BSP_ESP8266_JoinAP ( char * pSSID, char * pPassWord )
{
	char cCmd [120];

	sprintf ( cCmd, "AT+CWJAP=\"%s\",\"%s\"", pSSID, pPassWord );
	
	return BSP_ESP8266_Cmd ( cCmd, "OK", NULL, 5000 );
}

/*
***************************************************************************
*	函 数 名: BSP_ESP8266_BuildAP
*	功能说明: 板载 ESP8266 创建路由器热点 功能函数
*	形    参: pSSID，WiFi名称字符串
*             pPassWord，WiFi密码字符串
*             enunPsdMode，WiFi加密方式代号字符串
*	返 回 值: 1，选择成功
*             0，选择失败
***************************************************************************
*/
bool BSP_ESP8266_BuildAP ( char * pSSID, char * pPassWord, ENUM_AP_PsdMode_TypeDef enunPsdMode )
{
	char cCmd [120];

	sprintf ( cCmd, "AT+CWSAP=\"%s\",\"%s\",1,%d", pSSID, pPassWord, enunPsdMode );
	
	return BSP_ESP8266_Cmd ( cCmd, "OK", 0, 1000 );
}

/*
***************************************************************************
*	函 数 名: BSP_ESP8266_MultipleId
*	功能说明: 板载 ESP8266 启动多连接 功能函数
*	形    参: enumEnUnvarnishTx，配置是否多连接
*	返 回 值: 1，选择成功
*             0，选择失败
***************************************************************************
*/
bool BSP_ESP8266_MultipleId ( FunctionalState enumEnUnvarnishTx )
{
	char cStr [20];
	
	sprintf ( cStr, "AT+CIPMUX=%d", ( enumEnUnvarnishTx ? 1 : 0 ) );
	
	return BSP_ESP8266_Cmd ( cStr, "OK", 0, 500 );
}

/*
***************************************************************************
*	函 数 名: BSP_ESP8266_LinkServer
*	功能说明: 板载 ESP8266 连接外部服务器 功能函数
*	形    参: enumE，网络协议
*             ip，服务器IP字符串
*             ComNum，服务器端口字符串
*             id，模块连接服务器的ID
*	返 回 值: 1，选择成功
*             0，选择失败
***************************************************************************
*/
bool BSP_ESP8266_LinkServer ( ENUM_NetPro_TypeDef enumE, char * ip, char * ComNum, ENUM_ID_NO_TypeDef id)
{
	char cStr [100] = { 0 }, cCmd [120];

	switch (  enumE )
	{
		case enumTCP:
		  sprintf ( cStr, "\"%s\",\"%s\",%s", "TCP", ip, ComNum );
		  break;
		
		case enumUDP:
		  sprintf ( cStr, "\"%s\",\"%s\",%s", "UDP", ip, ComNum );
		  break;
		
		default:
			break;
	}

	if ( id < 5 )
		sprintf ( cCmd, "AT+CIPSTART=%d,%s", id, cStr);
	else
		sprintf ( cCmd, "AT+CIPSTART=%s", cStr );

	return BSP_ESP8266_Cmd ( cCmd, "OK", "ALREAY CONNECT", 4000 );
}

/*
***************************************************************************
*	函 数 名: BSP_ESP8266_StartOrShutServer
*	功能说明: 板载 ESP8266 开启或关闭服务器 功能函数
*	形    参: enumMode，开启/关闭
*             pPortNum，服务器端口号字符串
*             pTimeOver，服务器超时时间字符串，单位：秒
*	返 回 值: 1，选择成功
*             0，选择失败
***************************************************************************
*/
bool BSP_ESP8266_StartOrShutServer ( FunctionalState enumMode, char * pPortNum, char * pTimeOver )
{
	char cCmd1 [120], cCmd2 [120];

	if ( enumMode )
	{
		sprintf ( cCmd1, "AT+CIPSERVER=%d,%s", 1, pPortNum );
		sprintf ( cCmd2, "AT+CIPSTO=%s", pTimeOver );

		return ( BSP_ESP8266_Cmd ( cCmd1, "OK", 0, 500 ) &&
						 BSP_ESP8266_Cmd ( cCmd2, "OK", 0, 500 ) );
	}
	else
	{
		sprintf ( cCmd1, "AT+CIPSERVER=%d,%s", 0, pPortNum );

		return BSP_ESP8266_Cmd ( cCmd1, "OK", 0, 500 );
	}
}

/*
***************************************************************************
*	函 数 名: BSP_ESP8266_GetLinkStatus
*	功能说明: 板载 ESP8266 获取连接状态 功能函数
*	形    参: 无
*	返 回 值: 2，获得ip
*             3，建立连接
*             4，失去连接
*             0，获取状态失败
***************************************************************************
*/
uint8_t BSP_ESP8266_GetLinkStatus ( void )
{
	if ( BSP_ESP8266_Cmd ( "AT+CIPSTATUS", "OK", 0, 500 ) )
	{
		if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "STATUS:2\r\n" ) )
			return 2;
		
		else if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "STATUS:3\r\n" ) )
			return 3;
		
		else if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "STATUS:4\r\n" ) )
			return 4;		
	}
	
	return 0;
}

/*
***************************************************************************
*	函 数 名: BSP_ESP8266_GetIdLinkStatus
*	功能说明: 板载 ESP8266 获取端口（Id）连接状态 功能函数
*	形    参: 无
*	返 回 值: 端口（Id）的连接状态，低5位为有效位，分别对应Id5~0，某位若置1表该Id建立了连接，若被清0表该Id未建立连接
***************************************************************************
*/
uint8_t BSP_ESP8266_GetIdLinkStatus ( void )
{
	uint8_t ucIdLinkStatus = 0x00;
	
	if ( BSP_ESP8266_Cmd ( "AT+CIPSTATUS", "OK", 0, 500 ) )
	{
		if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "+CIPSTATUS:0," ) )
			ucIdLinkStatus |= 0x01;
		else 
			ucIdLinkStatus &= ~ 0x01;
		
		if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "+CIPSTATUS:1," ) )
			ucIdLinkStatus |= 0x02;
		else 
			ucIdLinkStatus &= ~ 0x02;
		
		if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "+CIPSTATUS:2," ) )
			ucIdLinkStatus |= 0x04;
		else 
			ucIdLinkStatus &= ~ 0x04;
		
		if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "+CIPSTATUS:3," ) )
			ucIdLinkStatus |= 0x08;
		else 
			ucIdLinkStatus &= ~ 0x08;
		
		if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "+CIPSTATUS:4," ) )
			ucIdLinkStatus |= 0x10;
		else 
			ucIdLinkStatus &= ~ 0x10;	
	}
	
	return ucIdLinkStatus;
}

/*
***************************************************************************
*	函 数 名: BSP_ESP8266_InquireApIp
*	功能说明: 板载 ESP8266 的 AP IP 功能函数
*	形    参: pApIp，存放 AP IP 的数组的首地址
*             ucArrayLength，存放 AP IP 的数组的长度
*	返 回 值: 0，获取失败
*             1，获取成功
***************************************************************************
*/
uint8_t BSP_ESP8266_InquireApIp ( char * pApIp, uint8_t ucArrayLength )
{
	char uc;
	char * pCh;
	
	BSP_ESP8266_Cmd ( "AT+CIFSR", "OK", 0, 500 );
	
	pCh = strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "APIP,\"" );
	
	if ( pCh )
		pCh += 6;
	else
		return 0;
	
	for ( uc = 0; uc < ucArrayLength; uc ++ )
	{
		pApIp [ uc ] = * ( pCh + uc);
		
		if ( pApIp [ uc ] == '\"' )
		{
			pApIp [ uc ] = '\0';
			break;
		}
	}
	
	return 1;
}

/*
***************************************************************************
*	函 数 名: BSP_ESP8266_UnvarnishSend
*	功能说明: 板载 ESP8266 进入透传模式 功能函数
*	形    参: 无
*	返 回 值: 0，获取失败
*             1，获取成功
***************************************************************************
*/
bool BSP_ESP8266_UnvarnishSend ( void )
{
	if ( ! BSP_ESP8266_Cmd ( "AT+CIPMODE=1", "OK", 0, 500 ) )
		return false;
	
	return 
	  BSP_ESP8266_Cmd ( "AT+CIPSEND", "OK", ">", 500 );
}

/*
***************************************************************************
*	函 数 名: BSP_ESP8266_ExitUnvarnishSend
*	功能说明: 板载 ESP8266 退出透传模式 功能函数
*	形    参: 无
*	返 回 值: 无
***************************************************************************
*/
void BSP_ESP8266_ExitUnvarnishSend ( void )
{
	BSP_Delay_ms ( 1000 );
	
	ESP8266_Usart ( "+++" );
	
	BSP_Delay_ms ( 500 ); 
	
}

/*
***************************************************************************
*	函 数 名: BSP_ESP8266_SendString
*	功能说明: 板载 ESP8266 发送字符串 功能函数
*	形    参: enumEnUnvarnishTx，声明是否已使能了透传模式
*             pStr，要发送的字符串
*             ulStrLength，要发送的字符串的字节数
*             ucId，哪个ID发送的字符串
*	返 回 值: 0，获取失败
*             1，获取成功
***************************************************************************
*/
bool BSP_ESP8266_SendString ( FunctionalState enumEnUnvarnishTx, char * pStr, u32 ulStrLength, ENUM_ID_NO_TypeDef ucId )
{
	char cStr [20];
	bool bRet = false;
		
	if ( enumEnUnvarnishTx )
	{
		ESP8266_Usart ( "%s", pStr );
		
		bRet = true;
	}
	else
	{
		if ( ucId < 5 )
			sprintf ( cStr, "AT+CIPSEND=%d,%d", ucId, ulStrLength + 2 );
		else
			sprintf ( cStr, "AT+CIPSEND=%d", ulStrLength + 2 );
		
		BSP_ESP8266_Cmd ( cStr, "> ", 0, 100 );

		bRet = BSP_ESP8266_Cmd ( pStr, "SEND OK", 0, 500 );
  }
	
	return bRet;
}

/*
***************************************************************************
*	函 数 名: BSP_ESP8266_ReceiveString
*	功能说明: 板载 ESP8266 接受字符串 功能函数
*	形    参: enumEnUnvarnishTx，声明是否已使能了透传模式
*	返 回 值: 接收到的字符串首地址
***************************************************************************
*/
char * BSP_ESP8266_ReceiveString ( FunctionalState enumEnUnvarnishTx )
{
	char * pRecStr = 0;
	
	strEsp8266_Fram_Record .InfBit .FramLength = 0;
	strEsp8266_Fram_Record .InfBit .FramFinishFlag = 0;
	
	while ( ! strEsp8266_Fram_Record .InfBit .FramFinishFlag );
	strEsp8266_Fram_Record .Data_RX_BUF [ strEsp8266_Fram_Record .InfBit .FramLength ] = '\0';
	
	if ( enumEnUnvarnishTx )
		pRecStr = strEsp8266_Fram_Record .Data_RX_BUF;
	else 
	{
		if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "+IPD" ) )
			pRecStr = strEsp8266_Fram_Record .Data_RX_BUF;
	}

	return pRecStr;
}

/*
***************************************************************************
*	函 数 名: BSP_ESP8266_CWLIF
*	功能说明: 板载 ESP8266 查询已接入设备的IP 功能函数
*	形    参: pStaIp，存放已接入设备的IP
*	返 回 值: 0，获取失败
*             1，获取成功
***************************************************************************
*/
uint8_t BSP_ESP8266_CWLIF ( char * pStaIp )
{
	uint8_t uc, ucLen;
	char * pCh, * pCh1;
	
	BSP_ESP8266_Cmd ( "AT+CWLIF", "OK", 0, 100 );
	
	pCh = strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "," );
	
	if ( pCh )
	{
		pCh1 = strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "AT+CWLIF\r\r\n" ) + 11;
		ucLen = pCh - pCh1;
	}
	else
		return 0;
	
	for ( uc = 0; uc < ucLen; uc ++ )
		pStaIp [ uc ] = * ( pCh1 + uc);
	
	pStaIp [ ucLen ] = '\0';
	
	return 1;
}

/*
***************************************************************************
*	函 数 名: BSP_ESP8266_CIPAP
*	功能说明: 板载 ESP8266 设置模块的 AP IP 功能函数
*	形    参: pApIp，模块的 AP IP
*	返 回 值: 0，获取失败
*             1，获取成功
***************************************************************************
*/
uint8_t BSP_ESP8266_CIPAP ( char * pApIp )
{
	char cCmd [ 30 ];
	
	sprintf ( cCmd, "AT+CIPAP=\"%s\"", pApIp );
	
	if ( BSP_ESP8266_Cmd ( cCmd, "OK", 0, 5000 ) )
		return 1;
	else 
		return 0;
}
