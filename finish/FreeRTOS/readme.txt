SenSor GPIO Setting
**********************************************
DHT11温湿度传感器 ->	DHT11温湿度传感器DATA连接J14 A01，GND连接J7的GND，5V连接J15的5V。

光照强度传感器	 -> J1口靠近'O'一端最近的引脚为GND，相反的一端为5V。
				    光照强度传感器模块J1的GND连接J1_2任意GND。
					光照强度传感器模块J1的5V连接J1_2任意5V。
					光照强度传感器模块J1的SCL引脚连接J1_2的U2T。
					光照强度传感器模块J1的SDA引脚连接J1_2的U2R。
					光照强度传感器模块J2的DVI引脚连接J1的C01。
				 
广谱气体传感器	 ->	红外对射传感器J1的INT连接物联网口袋机J4的A07。
				 ->	红外对射传感器J1的GND连接物联网口袋机J7的GND。
				 ->	红外对射传感器J1的5V连接物联网口袋机J15的5V。
（替代烟雾传感器）

ESP8266 WiFi模块	PB10 tx
					PB11 rx	
					
发送数据指令    "AT+CIPSEND=%d\r\n"
连接服务器指令 “AT+CIPSTART=\"TCP\",\"adtwuzp.iot.gz.baidubce.com\",1883\r\n”
微信配网指令	  “AT+CWSMARTSTART=2”
状态查询指令	  “AT+CIPSTATUS”
**********************************************