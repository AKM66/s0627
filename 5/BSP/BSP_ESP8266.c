#include "BSP_ESP8266.h"

TickType_t lastSendOutTime = 0;

NET_DEVICE_INFO_T netDeviceInfo_t = {{0}, NULL};
static char gUsartReciveLineBuf[200] = {0};

/* MCU ID */
static char LONG_CLIENT_ID[32] = {0};

/* ESP8266 ���������ʼ�� */
static ESP_ERROR_T esp_error_t = {0, 0, 0, 0};

ESP_STATUS_T g_esp_status_t = {ESP_HW_RESERVE_0, ESP_NETWORK_FAILED}; // ����esp8266ģ�鹤��״̬ȫ�ֱ���

//==========================================================
//	�������ƣ�	net_device_send_cmd
//
//	�������ܣ�	�������豸����һ��������ȴ���ȷ����Ӧ
//
//	��ڲ�����	cmd����Ҫ���͵�����
//				res����Ҫ��������Ӧ
//
//	���ز�����	�������ӽ��
//
//	˵����		0-�ɹ�		1-ʧ��
//==========================================================
uint8_t net_device_send_cmd(char *cmd, char *res)
{
    uint8_t timeout = SEND_TIMEOUT_TIME;
    USART_printf(USART3, cmd);
    netDeviceInfo_t.cmd_hdl = res;
    if (res == NULL)
    {
        return 0;
    }
    while (netDeviceInfo_t.cmd_hdl != NULL && --timeout != 0)
    {
        vTaskDelay(10 / portTICK_RATE_MS);
    }
    if (timeout > 0)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

//==========================================================
//	�������ƣ�	ReconfigWIFI
//
//	�������ܣ�	��������
//
//	��ڲ�����	��
//
//	���ز�����	��
//
//	˵����		�ɹ�����������		ʧ�ܣ���ʱ����
//==========================================================
void ReconfigWIFI(void)
{
    static uint16_t ConfigCnt = 0;
    printf("---------------Enter ConfigWiFi Function------------------\r\n");
    g_esp_status_t.esp_hw_status_e = ESP_HW_RECONFIG;
    g_esp_status_t.esp_net_work_e = ESP_NETWORK_FAILED;

    if (net_device_send_cmd("AT+CWSMARTSTART=2\r\n", "OK") == 0)
    {
        debug_log("Start smart config configure");
    }

    while (1)
    {

        if (strstr(netDeviceInfo_t.cmd_resp, "SMART SUCCESS") != NULL) // �����ɹ�
        {
            /* ������ɫ�� */

            debug_log("smart config ok");
            /* 5����������ȴ�ESP8266���ֻ������������*/
            vTaskDelay(5000 / portTICK_RATE_MS);
            NVIC_SystemReset(); // ����
        }
        else
        {
            /* 10��û�ɹ� */
            if (ConfigCnt++ >= SMART_CONFIG_TIME)
            {
                /* ��ʱ���� */
                debug_log("config timeout restart");
                vTaskDelay(2000 / portTICK_RATE_MS);
                NVIC_SystemReset();
            }
        }
        vTaskDelay(50 / portTICK_RATE_MS);
    }
}

/**
******************************************************************************
**	�������ƣ�	ConnectTcp
**
**	�������ܣ�	����TCP������
**
**	��ڲ�����	��
**
**	���ز�����	���ӽ�����ɹ�1��ʧ��0
**
**	˵����		������ɹ���ָ����NULL
******************************************************************************
**/
uint8_t ConnectTcp(void)
{
    uint8_t tmp = 0;
    if (net_device_send_cmd("AT+CIPSTART=\"TCP\",\"adtwuzp.iot.gz.baidubce.com\",1883\r\n", "OK") == 0)
    {
        tmp = 1;
        debug_log("connect server tcp ok!");
    }
    else
    {
        debug_log("connect server tcp failed!");
        tmp = 0;
    }
    return tmp;
}

/**
******************************************************************************
**	�������ƣ�	esp8266_cmd_handle
**
**	�������ܣ�	���������Ƿ���ȷ
**
**	��ڲ�����	cmd����Ҫ���͵�����
**
**	���ز�����	��
**
**	˵����		������ɹ���ָ����NULL
******************************************************************************
**/
void esp8266_cmd_handle(char *cmd)
{
    if (strstr(cmd, netDeviceInfo_t.cmd_hdl) != NULL)
    {
        netDeviceInfo_t.cmd_hdl = NULL;
    }
    memcpy(netDeviceInfo_t.cmd_resp, (const char *)cmd, sizeof(netDeviceInfo_t.cmd_resp));
    memset(gUsartReciveLineBuf, 0, sizeof(gUsartReciveLineBuf));
}

/**
 ******************************************************************************
 * @brief  ���ESP8266����״̬
 * @return ״ֵ̬
 ******************************************************************************
 **/
ESP_HW_STATUS_E check_esp8266_status(void)
{
    uint8_t tmp = 255;

    if (net_device_send_cmd("AT+CIPSTATUS\r\n", "OK") == 0) // ����״̬���
    {
        if (strstr(netDeviceInfo_t.cmd_resp, "STATUS:2")) // ���IP
        {
            tmp = 2;
        }
        else if (strstr(netDeviceInfo_t.cmd_resp, "STATUS:3")) // ��������
        {
            tmp = 3;
        }
        else if (strstr(netDeviceInfo_t.cmd_resp, "STATUS:4")) // ʧȥ���ӣ��Ͽ�tcp����
        {
            tmp = 4;
        }
        else if (strstr(netDeviceInfo_t.cmd_resp, "STATUS:5")) // �������ߣ��ر�·������Դ����WIFI�ź�
        {
            tmp = 5;
        }
        else if (strstr(netDeviceInfo_t.cmd_resp, "CLOSE"))
        {
            tmp = 4;
        }
        else
        {
            // nothing;
        }
    }
    else
    {
        if (strstr(netDeviceInfo_t.cmd_resp, "busy s...") != NULL)
            tmp = 6;
        else
            tmp = 7;
    }
    return (ESP_HW_STATUS_E)tmp;
}

/**
 ******************************************************************************
 * @brief  �������緵������
 * @param  Dataptr ������ָ��.
 * @return ״ֵ̬,�ɹ�0��ʧ��1
 ******************************************************************************
 **/
// uint8_t Handle_Internet_Data(char *Dataptr)
//{
//     uint8_t tmp = 255;
//     char* ptrMao = NULL;
//     /* ���������سɹ� */
//     ptrMao = strstr(Dataptr,":");
//     if(ptrMao != NULL)
//     {
//         memset(mqttSubscribeData,0,sizeof(mqttSubscribeData));
//         memcpy(mqttSubscribeData,(ptrMao+1),sizeof(mqttSubscribeData));
//         memset(gUsartReciveLineBuf,0,sizeof(gUsartReciveLineBuf));
//     }
//     return tmp;
// }

void USART3_SendData(const uint8_t *const pData, uint32_t len)
{
    int i;
    for (i = 0; i < len; i++)
    {
        while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
            ;
        USART_SendData(USART3, pData[i]);
    }
}

void SendDataServer(uint8_t *data, int len)
{
    char sendDataCmdBuf[20];
    sprintf(sendDataCmdBuf, (char *)SendDataCmd, len);
    //    if(netDeviceInfo_t.netWork)
    if (g_esp_status_t.esp_net_work_e == ESP_NETWORK_SUCCESS)
    {
        if (net_device_send_cmd(sendDataCmdBuf, ">") == 0)
        {
            debug_log("Send to esp8266 data len = %d", len);
            USART3_SendData(data, len);
            //					USART_printf(USART3,(char *)data);
        }
    }
}

void getTcpConnect(void)
{
    if (ConnectTcp() == 1)
    {
        g_esp_status_t.esp_hw_status_e = ESP_HW_CONNECT_OK;
        esp_error_t.err_esp_disconnect = 0;
        esp_error_t.err_esp_lostwifi = 0;
        esp_error_t.err_esp_notresp = 0;
        debug_log("connect TCP server OK\r\n");
        //        Mqtt_status_step = MQTT_IDLE;
    }
    else
    {
        debug_log("connect TCP server Fail\r\n");
        //				g_esp_status_t.esp_hw_status_e = ESP_HW_DISCONNECT;
    }
}

int Esp8266_Tcp_Send(int socket, uint8_t *data, uint16_t len)
{
    uint8_t ret;
    char cmd[128] = {0};
    sprintf(cmd, "AT+CIPSEND=%d\r\n", len);
    //    debug_log("cmd :%s",cmd);
    if (net_device_send_cmd(cmd, ">") == 0)
    {
        USART3_SendData(data, len);
        //			   USART_printf(USART3,(char *)data);
        ret = 0;
    }
    else
        ret = 1;
    return ret;
}

uint8_t Esp8266_GetTcpStatus(void)
{
    uint8_t status = 0;
    if (g_esp_status_t.esp_hw_status_e == ESP_HW_CONNECT_OK)
        status = 1;
    return status;
}
