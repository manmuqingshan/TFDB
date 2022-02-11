#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "lcd.h"
#include "sdram.h"
#include "usmart.h"
#include "stmflash.h"
#include "tinyflashdb.h"
/************************************************
 ALIENTEK ������STM32F429������ʵ��36
 FLASHģ��EEPROMʵ��-HAL�⺯����
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com  
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 �������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/

//Ҫд�뵽STM32 FLASH���ַ�������
const u8 TEXT_Buffer[]={"STM32 FLASH TEST"};
#define TEXT_LENTH sizeof(TEXT_Buffer)	 		  	//���鳤��	
#define SIZE TEXT_LENTH/4+((TEXT_LENTH%4)?1:0)

#define FLASH_SAVE_ADDR  0x08010000 	//����FLASH �����ַ(����Ϊ4�ı���������������,Ҫ���ڱ�������ռ�õ�������.
										//����,д������ʱ��,���ܻᵼ�²�����������,�Ӷ����𲿷ֳ���ʧ.��������.

const tfdb_index_t test_index = {
		.end_byte = 0x00,
		.flash_addr = FLASH_SAVE_ADDR,
		.flash_size = 16*1024,
		.value_length = 4,
};

tfdb_addr_t addr = 0; /*addr cache*/

uint32_t test_buf[2]; /*aligned_value_size*/

uint8_t test_value[4]={"hi0\0"};


int main(void)
{
	u8 key=0;
	u16 i=0;
	TFDB_Err_Code result;
	u8 datatemp[SIZE];
	HAL_Init();                     //��ʼ��HAL��   
	Stm32_Clock_Init(360,25,2,8);   //����ʱ��,180Mhz
	delay_init(180);                //��ʼ����ʱ����
	uart_init(115200);              //��ʼ��USART
	usmart_dev.init(90); 		    //��ʼ��USMART	
	LED_Init();                     //��ʼ��LED 
	KEY_Init();                     //��ʼ������
	SDRAM_Init();                   //��ʼ��SDRAM
	LCD_Init();                     //��ʼ��LCD
	POINT_COLOR=RED;
	LCD_ShowString(30,50,200,16,16,"Apollo STM32F4/F7"); 
	LCD_ShowString(30,70,200,16,16,"FLASH EEPROM TEST");	
	LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
	LCD_ShowString(30,110,200,16,16,"2016/1/16");	 		
	LCD_ShowString(30,130,200,16,16,"KEY1:Write  KEY0:Read");
	while(1)
	{
		key=KEY_Scan(0);
		if(key==KEY1_PRES)	//KEY1����,д��STM32 FLASH
		{
			LCD_Fill(0,170,239,319,WHITE);//�������    
 			LCD_ShowString(30,170,200,16,16,"Start Write FLASH....");
			test_value[2]++;
			if(test_value[2]>('9')){ 
				test_value[2] = '0';
			}
			result = tfdb_set(&test_index, (uint8_t *)test_buf, &addr, &test_value);
			if(result == TFDB_NO_ERR){
					printf("set ok, addr:%x\n", addr);
					LCD_ShowString(30,170,200,16,16,"FLASH Write Finished!");//��ʾ�������
			} else {
					LCD_ShowString(30,170,200,16,16,"FLASH Write error!");//��ʾ�������
			}
			
		}
		if(key==KEY0_PRES)	//KEY0����,��ȡ�ַ�������ʾ
		{
 			LCD_ShowString(30,170,200,16,16,"Start Read FLASH.... ");
			result = tfdb_get(&test_index, (uint8_t *)test_buf, NULL, &test_value);
			if(result == TFDB_NO_ERR){
					printf("get ok\n");
			}
			LCD_ShowString(30,170,200,16,16,"The Data Readed Is:  ");//��ʾ�������
			LCD_ShowString(30,190,200,16,16,test_value);//��ʾ�������ַ���
		}
		i++;
		delay_ms(10);  
		if(i==20)
		{
			LED0=!LED0;//��ʾϵͳ��������	
			i=0;
		}		   
	}    
}