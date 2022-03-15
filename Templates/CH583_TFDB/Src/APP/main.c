/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : WCH
 * Version            : V1.1
 * Date               : 2020/08/06
 * Description        : ����ӻ�Ӧ��������������ϵͳ��ʼ��
 *******************************************************************************/

/******************************************************************************/
/* ͷ�ļ����� */
#include "CONFIG.h"
#include "CH58x_common.h"
#include "HAL.h"
#include "tinyflashdb.h"

/*********************************************************************
 * GLOBAL TYPEDEFS
 */
__attribute__((aligned(4))) u32 MEM_BUF[BLE_MEMHEAP_SIZE / 4];
__attribute__((aligned(4))) uint8_t tfkvtest[128];

#if(defined(BLE_MAC)) && (BLE_MAC == TRUE)
u8C MacAddr[6] = {0x84, 0xC2, 0xE4, 0x03, 0x02, 0x02};
#endif

/*******************************************************************************
 * Function Name  : Main_Circulation
 * Description    : ��ѭ��
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/

tfdb_index_t tetttt =
{
    .end_byte = 0x00,
    .flash_addr = 0x4000,
    .flash_size = 256,
    .value_length = 1,
};

uint8_t buf[256];
/*******************************************************************************
 * Function Name  : main
 * Description    : ������
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
int main(void)
{
#if(defined(DCDC_ENABLE)) && (DCDC_ENABLE == TRUE)
    PWR_DCDCCfg(ENABLE);
#endif
    SetSysClock(CLK_SOURCE_PLL_60MHz);
#if(defined(HAL_SLEEP)) && (HAL_SLEEP == TRUE)
    GPIOA_ModeCfg(GPIO_Pin_All, GPIO_ModeIN_PU);
    GPIOB_ModeCfg(GPIO_Pin_All, GPIO_ModeIN_PU);
#endif
#ifdef DEBUG
    GPIOA_SetBits(bTXD1);
    GPIOA_ModeCfg(bTXD1, GPIO_ModeOut_PP_5mA);
    UART1_DefInit();
#endif
    PRINT("%s\n", VER_LIB);
    tfdb_addr_t maddr = 0; //��ַ����
    uint8_t likii[8] = {'3', '2', '1', '0', '7', '6', '5', '4'}; //����д��������ݺ��ܷ��ҵ���ȷ��ַ
    uint16_t tests = 16; //���ݻ���
    TFDB_Err_Code result;
    result = tfdb_set(&tetttt, tfkvtest, &maddr, &tests);
    if (result == TFDB_NO_ERR)
    {
        PRINT("set ok\n");
    }
    PRINT("addr cache:%x\n", maddr); //��ӡ�����ַ

    EEPROM_READ(tetttt.flash_addr, buf, 256);//��ȡflash��������
    PRINT("DATA:");//�����ݴ�ӡ����
    for (uint16_t i = 0; i < 256; i++)
    {
        PRINT(" %02x", buf[i]);
    }
    PRINT("\n");
#if 1
    /* test read */
    EEPROM_WRITE(maddr + 8, likii, 8);//д���������
    maddr = 0;//���õ�ַ�����������ܷ��ҵ���ַ
#endif
    tests = 0;//���������㣬���´�flash��ȡ�����Ƿ�д��ɹ�
    result = tfdb_get(&tetttt, tfkvtest, &maddr, &tests);
    if (result == TFDB_NO_ERR)
    {
        PRINT("addr cache:%x\n", maddr); //��ӡ�����ַ
        PRINT("tests:%d\n", tests);
    }
    while (1) {}//hold

}

/******************************** endfile @ main ******************************/
