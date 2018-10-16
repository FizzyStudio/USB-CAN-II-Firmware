/****************************************Copyright (c)****************************************************
**                                 
**--------------File Info---------------------------------------------------------------------------------
** File name:           common.h
** Last modified Date:  2018-10-16
** Last Version:        V1.00
** Descriptions:        
**
**--------------------------------------------------------------------------------------------------------
** Created by:          letheascetic
** Created date:        2018-10-16
** Version:             V1.00
** Descriptions:        USB-CAN-II Common Struct Descriptions
**
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/

#ifndef _COMMON_H_
#define _COMMON_H

#define		USBCAN2			0x04


typedef struct _CAN_BOARD_INFO {
	USHORT hw_Version;									//硬件版本号，用 16 进制表示。比如 0x0100 表示 V1.00。
	USHORT fw_Version;
	USHORT dr_Version;
	USHORT in_Version;
	USHORT irq_Num;
	BYTE can_Num;
	CHAR str_Serial_Num[20];
	CHAR str_hw_Type[40];
	USHORT Reserved[4];
} CAN_BOARD_INFO, *PCAN_BOARD_INFO;





#endif