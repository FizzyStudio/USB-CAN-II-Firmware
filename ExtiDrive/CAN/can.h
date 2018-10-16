/****************************************Copyright (c)****************************************************
**                                 http://www.openmcu.com
**--------------File Info---------------------------------------------------------------------------------
** File name:           can.h
** Last modified Date:  2012-07-23
** Last Version:        V1.00
** Descriptions:        
**
**--------------------------------------------------------------------------------------------------------
** Created by:          openmcu
** Created date:        2012-07-23
** Version:             V1.00
** Descriptions:        编写示例代码
**
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#ifndef _CAN_H_
#define _CAN_H_

#include <lpc17xx.h>
/*********************************************************************************************************
**      相关常量定义               
*********************************************************************************************************/
#define CAN1             0                                              /*  线路选择CAN1                */
#define CAN2             1                                              /*  线路选择CAN2                */

#define STANDARD_FORMAT  0                                              /*  标准帧                      */
#define EXTENDED_FORMAT  1                                              /*  扩展帧                      */

#define DATA_FRAME       0                                              /*  数据帧                      */
#define REMOTE_FRAME     1                                              /*  远程帧                      */

#define  PCLK            (100000000UL/4)                                /*  外设时钟频率                */

#define ACCF_OFF				0x01
#define ACCF_BYPASS				0x02
#define ACCF_ON					0x00
#define ACCF_FULLCAN			0x04

/* This number applies to all FULLCAN IDs, explicit STD IDs, group STD IDs, 
explicit EXT IDs, and group EXT IDs. */ 
#define ACCF_IDEN_NUM			1

/* Identifiers for FULLCAN, EXP STD, GRP STD, EXP EXT, GRP EXT */
#define FULLCAN_ID				0x100



/*********************************************************************************************************
**      帧格式声明               
*********************************************************************************************************/
typedef struct  {
  unsigned int   id;                                                    /*  29/11 位标识符              */
  unsigned char  data[8];                                               /*  8字节数据场                 */
  unsigned char  len;                                                   /*  数据场字节数                */
  unsigned char  format;                                                /*  0 - 标准帧, 1- 扩展帧       */
  unsigned char  type;                                                  /*  0 - 数据帧, 1 - 远程帧      */
} CAN_FRAME;

/*********************************************************************************************************
**      函数接口定义               
*********************************************************************************************************/
void CAN_setup         (uint32_t ctrl);                                /*  设置CAN控制器                */
void CAN_start         (uint32_t ctrl);                                /*  启动CAN控制器                */
void CAN_waitReady     (uint32_t ctrl);                                /*  等待发送就绪                 */
void CAN_send          (uint32_t ctrl, CAN_FRAME *msg);                /*  发送CAN报文                  */
void CAN_recive        (uint32_t ctrl, CAN_FRAME *msg);                /*  接收CAN报文                  */
void CAN_SetACCF			 (uint32_t ACCFMode );    

extern CAN_FRAME       CAN_TxMsg[2];                                   /* 发送缓冲区                    */
extern CAN_FRAME       CAN_RxMsg[2];                                   /* 接收缓冲区                    */                                
extern unsigned int    CAN_TxRdy[2];                                   /* 发送可用标识                  */
extern unsigned int    CAN_RxRdy[2];                                   /* 接收完成标识                  */

#endif 
/*********************************************************************************************************
**                                        End Of File
*********************************************************************************************************/
