/*
 * Copyright (c) 2022, mx1117 - smartmx@qq.com
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * This file is part of the Tiny Flash DataBase Library.
 *
 */
#include "tfdb_port.h"

/**
 * Read data from flash.
 * @note This operation's units is refer to TFDB_WRITE_UNIT_BYTES.
 *
 * @param addr flash address.
 * @param buf buffer to store read data.
 * @param size read bytes size.
 *
 * @return TFDB_Err_Code
 */
TFDB_Err_Code tfdb_port_read(tfdb_addr_t addr, uint8_t *buf, size_t size)
{
    TFDB_Err_Code result = TFDB_NO_ERR;
		uint8_t *buf_8 = (uint8_t *)buf;
		size_t i;

    /* You can add your code under here. */
		for (i = 0; i < size; i++, addr ++, buf_8++) {
        *buf_8 = *(uint8_t *) addr;
    }
    return result;
}

/**
 * Erase flash.
 * @param addr flash address.
 * @param size erase bytes size.
 *
 * @return TFDB_Err_Code
 */
TFDB_Err_Code tfdb_port_erase(tfdb_addr_t addr, size_t size)
{
    TFDB_Err_Code result = TFDB_NO_ERR;
		FLASH_EraseInitTypeDef FlashEraseInit;
		HAL_StatusTypeDef FlashStatus=HAL_OK;
		u32 SectorError=0;
    /* You can add your code under here. */
		HAL_FLASH_Unlock();             //����	
		FlashEraseInit.TypeErase=FLASH_TYPEERASE_SECTORS;       //�������ͣ��������� 
		FlashEraseInit.Sector=STMFLASH_GetFlashSector(addr);    //Ҫ����������
		FlashEraseInit.NbSectors=STMFLASH_GetFlashSector(addr + size) - FlashEraseInit.Sector + 1;   //�����������
		FlashEraseInit.VoltageRange=FLASH_VOLTAGE_RANGE_3;      //��ѹ��Χ��VCC=2.7~3.6V֮��!!
		if(HAL_FLASHEx_Erase(&FlashEraseInit,&SectorError)!=HAL_OK) 
		{
			result = TFDB_ERASE_ERR;//����������	
		} else {
			FlashStatus=FLASH_WaitForLastOperation(FLASH_WAITETIME * FlashEraseInit.NbSectors);            //�ȴ��ϴβ������
			if(FlashStatus!=HAL_OK)
			{
				result = TFDB_ERASE_ERR;//����������	
			}
		}
		
		HAL_FLASH_Lock();           //����
    return result;
}

/**
 * Write data to flash.
 * @note This operation's units is refer to TFDB_WRITE_UNIT_BYTES.
 *
 * @param addr flash address.
 * @param buf the write data buffer.
 * @param size write bytes size.
 *
 * @return result
 */
TFDB_Err_Code tfdb_port_write(tfdb_addr_t addr, const uint8_t *buf, size_t size)
{
    TFDB_Err_Code result = TFDB_NO_ERR;
		uint32_t *pBuffer = (uint32_t *)buf;
		uint32_t WriteAddr = addr;
		uint32_t endaddr = addr + size;
    /* You can add your code under here. */
		HAL_FLASH_Unlock();             //����	
		while(WriteAddr<endaddr)//д����
		{
			if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,WriteAddr,*pBuffer)!=HAL_OK)//д������
			{ 
				break;	//д���쳣
			}
			WriteAddr+=4;
			pBuffer++;
		}  
	
		HAL_FLASH_Lock();           //����
    return result;
}


