/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/main.c
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * 风扇继电器 -- PA4
  * 人体红外 -- PB3
  * 舵机 -- PA6
  * 温湿度
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
//USE_STDPERIPH_DRIVER, STM32F10X_HD, USE_STM3210B_EVAL

#include "stm32f10x.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "usart1.h"
#include "usart2.h"
#include "utils.h"
#include "sht20.h"
#include "hal_i2c.h"
#include "esp8266.h"
#include "fengshan.h"
#include "rthongwai.h"
#include "led.h"
#include "duoji.h"

//#define API_KEY     "3L4OaBc8pb4iuTosCJ33IWZznkg="		//需要定义为用户自己的参数
//#define DEV_ID      "4093185"							//需要定义为用户自己的参数

/**n
  * @brief      利用I2C接口，采集温湿度传感器的值，使用HTTP协议上传到OneNET
  * @attention  使用UART2连接ESP8266模块，使用透传模式发送和接收数据
  *	            使用UART1作为调试打印串口，使用printf将从该接口打印消息
  *			
  */
uint32_t HTTP_NewPostPacket(char *pkt, char *val1, char *val2, char *val3, char *val4);
int main(void)
{
    uint16_t temp, humi;    //温湿度
    char HTTP_Buf[400];     //HTTP报文缓存区
    char tempStr[5];       //字符串格式温度
    char humiStr[5];       //字符串格式湿度
		char indoor[1];
		char feng[1];
		char status[5] = "open";
    int len;

    USART1_Config();        //USART1作为调试串口
    USART2_Config();        //USART2用于连接ESP8266模块
    Hal_I2C_Init();			    //I2C初始化，用于连接温湿度传感器
		JDQ_Init();
		Duoji_Init(9999,143);	 	//舵机初始化
		Body_Init();
    ESP8266_Init();         //ESP8266初始化
		printf("ESP8266 init over\r\n");
    //SHT20_loop();         //
	
    while(1)
    {
        /* 获取温湿度 */
        SHT2x_MeasureHM(SHT20_Measurement_T_HM, &temp);
        mDelay(500);
        SHT2x_MeasureHM(SHT20_Measurement_RH_HM, &humi);

        /* 转化为字符串形式 */
        sprintf(tempStr, "%d", temp);
        sprintf(humiStr, "%d", humi);
				
				if(temp > 35){
					JDQ_Switch(J_ON,JDQ_1);
				}else{
					JDQ_Switch(J_OFF,JDQ_1);
				}
				
				if(GPIO_ReadInputDataBit(Body_GPIO_PORT,Body_GPIO_PIN)){
					indoor[0] = '1';
				}else{
					indoor[0] = '0';
				}
				
				if(fengStatus.FengSta == 1){
					feng[0] = '1';
				}else{
					feng[0] = '0';
				}
        //printf("%s   %s\r\n", tempStr, humiStr);

        USART2_Clear();
        len = HTTP_NewPostPacket(HTTP_Buf, tempStr, humiStr, feng, indoor); //HTTP组包
        USART2_Write(USART2, (unsigned char *)(HTTP_Buf), len);			//报文发送
        printf("send HTTP msg:\r\n%s\r\n", HTTP_Buf);

        mDelay(1000);
        printf("rcv response:\r\n%s\r\n", usart2_rcv_buf);
				if(usart2_rcv_buf[0] == status[0]){
						TIM3->CCR1= 300;//open
						mDelay(1000);
						TIM3->CCR1= 735;
						LED_Switch(LED_ON,LED_R);
				}else{
					  LED_Switch(LED_OFF,LED_B);
				}

//        USART2_Clear();
//        len = HTTP_NewPostPacket(HTTP_Buf,  "humi", humiStr); //HTTP组包
//        USART2_Write(USART2, (unsigned char *)(HTTP_Buf), len);			//报文发送
//        printf("send HTTP msg:\r\n%s\r\n", HTTP_Buf);

//        mDelay(1000);
//        printf("rcv response:\r\n%s\r\n", usart2_rcv_buf);

        mDelay(5000);

    }
}

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
