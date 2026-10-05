/*
***************************************************************************
*    ģ�飺BSP_ESP8266 
*    �������弶 ESP8266 ����ģ������
          ʹ�� USART3:TX3-PB10 RX3-PB11
*    ���ߣ�Huo
*    ʱ�䣺2017.10.20
*    �汾��UP-Magic-Version 1.0.0
***************************************************************************
*/
#include "BSP_ESP8266.h"

struct  STRUCT_USARTx_Fram strEsp8266_Fram_Record = { 0 };

volatile uint8_t ucTcpClosedFlag = 0;

/*
***************************************************************************
*	�� �� ��: BSP_ESP8266_Init
*	����˵��: ���� ESP8266 ��ʼ������
*	��    ��: ��
*	�� �� ֵ: ��
***************************************************************************
*/
void BSP_ESP8266_Init( void )
{
	BSP_USART_Init(115200); 
}

/*
***************************************************************************
*	�� �� ��: BSP_ESP8266_Rst
*	����˵��: ���� ESP8266 ��λ���ܺ���
*	��    ��: ��
*	�� �� ֵ: ��
***************************************************************************
*/
void BSP_ESP8266_Rst( void )
{
	BSP_ESP8266_Cmd ( "AT+RST", "OK", "ready", 2500 );   	
	BSP_Delay_ms ( 500 ); 
}

/*
***************************************************************************
*	�� �� ��: BSP_ESP8266_Cmd
*	����˵��: �� ESP8266 ���� ATָ�� ���ܺ���
*	��    ��: cmd�������͵�ָ��
*             reply1��reply2���ڴ�����Ӧ��ΪNULL��������Ӧ������Ϊ���߼���ϵ
*             waittime���ȴ���Ӧ��ʱ��
*	�� �� ֵ: 1��ָ��ͳɹ�
*             0��ָ���ʧ��
***************************************************************************
*/
bool BSP_ESP8266_Cmd(char * cmd,char * reply1,char * reply2,u32 waittime )
{    
	strEsp8266_Fram_Record .InfBit .FramLength = 0; //���¿�ʼ�����µ����ݰ�

	ESP8266_Usart ( "%s\r\n", cmd );

	if ( ( reply1 == 0 ) && ( reply2 == 0 ) )       //����Ҫ��������
		return true;
	
	BSP_Delay_ms ( waittime );                 //��ʱ
	
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
*	�� �� ��: BSP_ESP8266_ATTest
*	����˵��: ���� ESP8266 AT���� ���ܺ���
*	��    ��: ��
*	�� �� ֵ: ��
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
*	�� �� ��: BSP_ESP8266_NetModeChoose
*	����˵��: ���� ESP8266 ����ģʽѡ�� ���ܺ���
*	��    ��: enumMode������ģʽ
*	�� �� ֵ: 1��ѡ��ɹ�
*             0��ѡ��ʧ��
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
*	�� �� ��: BSP_ESP8266_JoinAP
*	����˵��: ���� ESP8266 ����·���� ���ܺ���
*	��    ��: pSSID��WiFi�����ַ���
*             pPassWord��WiFi�����ַ���
*	�� �� ֵ: 1��ѡ��ɹ�
*             0��ѡ��ʧ��
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
*	�� �� ��: BSP_ESP8266_BuildAP
*	����˵��: ���� ESP8266 ����·�����ȵ� ���ܺ���
*	��    ��: pSSID��WiFi�����ַ���
*             pPassWord��WiFi�����ַ���
*             enunPsdMode��WiFi���ܷ�ʽ�����ַ���
*	�� �� ֵ: 1��ѡ��ɹ�
*             0��ѡ��ʧ��
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
*	�� �� ��: BSP_ESP8266_MultipleId
*	����˵��: ���� ESP8266 ���������� ���ܺ���
*	��    ��: enumEnUnvarnishTx�������Ƿ������
*	�� �� ֵ: 1��ѡ��ɹ�
*             0��ѡ��ʧ��
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
*	�� �� ��: BSP_ESP8266_LinkServer
*	����˵��: ���� ESP8266 �����ⲿ������ ���ܺ���
*	��    ��: enumE������Э��
*             ip��������IP�ַ���
*             ComNum���������˿��ַ���
*             id��ģ�����ӷ�������ID
*	�� �� ֵ: 1��ѡ��ɹ�
*             0��ѡ��ʧ��
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
*	�� �� ��: BSP_ESP8266_StartOrShutServer
*	����˵��: ���� ESP8266 ������رշ����� ���ܺ���
*	��    ��: enumMode������/�ر�
*             pPortNum���������˿ں��ַ���
*             pTimeOver����������ʱʱ���ַ�������λ����
*	�� �� ֵ: 1��ѡ��ɹ�
*             0��ѡ��ʧ��
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
*	�� �� ��: BSP_ESP8266_GetLinkStatus
*	����˵��: ���� ESP8266 ��ȡ����״̬ ���ܺ���
*	��    ��: ��
*	�� �� ֵ: 2�����ip
*             3����������
*             4��ʧȥ����
*             0����ȡ״̬ʧ��
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
*	�� �� ��: BSP_ESP8266_GetIdLinkStatus
*	����˵��: ���� ESP8266 ��ȡ�˿ڣ�Id������״̬ ���ܺ���
*	��    ��: ��
*	�� �� ֵ: �˿ڣ�Id��������״̬����5λΪ��Чλ���ֱ��ӦId5~0��ĳλ����1����Id���������ӣ�������0����Idδ��������
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
*	�� �� ��: BSP_ESP8266_InquireApIp
*	����˵��: ���� ESP8266 �� AP IP ���ܺ���
*	��    ��: pApIp����� AP IP ��������׵�ַ
*             ucArrayLength����� AP IP ������ĳ���
*	�� �� ֵ: 0����ȡʧ��
*             1����ȡ�ɹ�
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
*	�� �� ��: BSP_ESP8266_UnvarnishSend
*	����˵��: ���� ESP8266 ����͸��ģʽ ���ܺ���
*	��    ��: ��
*	�� �� ֵ: 0����ȡʧ��
*             1����ȡ�ɹ�
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
*	�� �� ��: BSP_ESP8266_ExitUnvarnishSend
*	����˵��: ���� ESP8266 �˳�͸��ģʽ ���ܺ���
*	��    ��: ��
*	�� �� ֵ: ��
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
*	�� �� ��: BSP_ESP8266_SendString
*	����˵��: ���� ESP8266 �����ַ��� ���ܺ���
*	��    ��: enumEnUnvarnishTx�������Ƿ���ʹ����͸��ģʽ
*             pStr��Ҫ���͵��ַ���
*             ulStrLength��Ҫ���͵��ַ������ֽ���
*             ucId���ĸ�ID���͵��ַ���
*	�� �� ֵ: 0����ȡʧ��
*             1����ȡ�ɹ�
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
*	�� �� ��: BSP_ESP8266_ReceiveString
*	����˵��: ���� ESP8266 �����ַ��� ���ܺ���
*	��    ��: enumEnUnvarnishTx�������Ƿ���ʹ����͸��ģʽ
*	�� �� ֵ: ���յ����ַ����׵�ַ
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
*	�� �� ��: BSP_ESP8266_CWLIF
*	����˵��: ���� ESP8266 ��ѯ�ѽ����豸��IP ���ܺ���
*	��    ��: pStaIp������ѽ����豸��IP
*	�� �� ֵ: 0����ȡʧ��
*             1����ȡ�ɹ�
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
*	�� �� ��: BSP_ESP8266_CIPAP
*	����˵��: ���� ESP8266 ����ģ��� AP IP ���ܺ���
*	��    ��: pApIp��ģ��� AP IP
*	�� �� ֵ: 0����ȡʧ��
*             1����ȡ�ɹ�
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
