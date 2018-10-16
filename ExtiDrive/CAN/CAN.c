/****************************************Copyright (c)****************************************************
**                                 http://www.openmcu.com
**--------------File Info---------------------------------------------------------------------------------
** File name:           can.c
** Last modified Date:  2014-04-18
** Last Version:        V1.1
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
#include "CAN.h"                                 

CAN_FRAME        CAN_TxMsg[2];                                          /* 发送缓冲区                   */
CAN_FRAME        CAN_RxMsg[2];                                          /* 接收缓冲区                   */
unsigned int     CAN_TxRdy[2] = {0,0};                                  /* 发送可用标识                 */
unsigned int     CAN_RxRdy[2] = {0,0};                                  /* 接收完成标识                 */

/*********************************************************************************************************
**      波特率设置  
**      NT = Nominal bit time = TSEG1 + TSEG2 + 3
**      SP = Sample point     = ((TSEG2 +1) / (TSEG1 + TSEG2 + 3)) * 100%
**      SAM,  SJW, TSEG1, TSEG2, NT,  SP             
*********************************************************************************************************/
const uint32_t CAN_BIT_TIME[] = {          0,                           /*  not used                    */
                                           0,                           /*  not used                    */
                                           0,                           /*  not used                    */
                                           0,                           /*  not used                    */
                                  0x0001C000,                           /*  0+1,  3+1, 1+1, 0+1, 4, 75% */
                                           0,                           /*  not used                    */
                                  0x0012C000,                           /*  0+1,  3+1, 2+1,1+1, 6, 67%  */
                                           0,                           /*  not used             */
                                  0x0023C000,                           /*  0+1,  3+1, 3+1,2+1,  8, 63% */
                                           0,                           /*  not used             */
                                  0x0025C000,                           /*  0+1,  3+1, 5+1, 2+1, 10,70% */
                                           0,                           /*  not used             */
                                  0x0036C000,                           /*  0+1,  3+1, 6+1, 3+1,12, 67% */
                                           0,                           /*  not used                    */
                                           0,                           /*  not used                    */
                                  0x0048C000,                           /*  0+1,  3+1, 8+1,4+1, 15, 67% */
                                  0x0049C000,                           /*  0+1,  3+1, 9+1,4+1, 16, 69% */
                                };

/*********************************************************************************************************
** Functoin name:       CAN_cfgBaudrate
** Descriptions:        设置波特率
** input paraments:     ctrl:CAN控制器号，可选项 CAN1/CAN2
**                      baudrate: 波特率     
** output paraments:    无    
** Returned values:     无
*********************************************************************************************************/
static void CAN_cfgBaudrate (uint32_t ctrl, uint32_t baudrate)  {
    LPC_CAN_TypeDef *pCAN = (ctrl == 0) ? LPC_CAN1 : LPC_CAN2;
    uint32_t result = 0;
    uint32_t nominal_time;

    if (((PCLK / 1000000) % 6) == 0) 
		{
      nominal_time = 12;                                                /* CCLK 为72MHz                 */
    } 
		else 
		{
      nominal_time = 10;                                                /* CCLK 为100MHz                */
    }

    result  = (PCLK / nominal_time) / baudrate - 1;				//波特率预分频值 BRP=2.5M/baudrate-1
    result &= 0x000003FF;
    result |= CAN_BIT_TIME[nominal_time];

    pCAN->BTR  = result;                                  //设置波特率 25M/(BRP+1)/(3+5+2)
}

/*********************************************************************************************************
** Functoin name:       CAN_setup
** Descriptions:        设置控制器
** input paraments:     ctrl:CAN控制器号，可选项 CAN1/CAN2    
** output paraments:    无    
** Returned values:     无
*********************************************************************************************************/
void CAN_setup (uint32_t ctrl)  
{
  LPC_CAN_TypeDef *pCAN = (ctrl == 0) ? LPC_CAN1 : LPC_CAN2;

  if (ctrl == 0) {
      LPC_SC->PCONP       |=  (1 << 13);                                //CAN控制器1上电               */
      LPC_PINCON->PINSEL0 |=  (1 <<  0);                                //P0.0 用作 RD1 (CAN1)         */
      LPC_PINCON->PINSEL0 |=  (1 <<  2);                                //P0.1 用作 TD1 (CAN1)         */
  } else {
      LPC_SC->PCONP       |=  (1 << 14);                                //CAN控制器2上电               */
      LPC_PINCON->PINSEL0 |=  (1 <<  9);                                //P0.4 用作 RD2 (CAN2)        */
      LPC_PINCON->PINSEL0 |=  (1 << 11);                                //P0.5 用作 TD2 (CAN2)        */
  }
	
	NVIC_EnableIRQ(CAN_IRQn);                                           //允许CAN中断                  */

	LPC_CANAF->AFMR = 1;                                                /* 进入旁路滤波模式             */
	pCAN->MOD   = 1;                                                    /* 进入复位模式                 */
	pCAN->IER   = 0;                                                    /* 关闭接收发送中断             */
	pCAN->GSR   = 0;                                                    /* 清除状态寄存器               */
	CAN_cfgBaudrate(ctrl, 500000);                                      /* 设置波特率                   */
	pCAN->IER   = 0x0003;                                               /* 允许接收发送中断             */

	pCAN->MOD   = 0;
	LPC_CANAF->AFMR = 0x02;  
}

/*********************************************************************************************************
** Functoin name:       CAN_start
** Descriptions:        启动控制器
** input paraments:     ctrl:CAN控制器号，可选项 CAN1/CAN2    
** output paraments:    无    
** Returned values:     无
*********************************************************************************************************/
void CAN_start (uint32_t ctrl)  
{
  LPC_CAN_TypeDef *pCAN = (ctrl == 0) ? LPC_CAN1 : LPC_CAN2;
  pCAN->MOD = 0;                                 //进入正常工作模式
}

/*********************************************************************************************************
** Functoin name:       CAN_waitReady
** Descriptions:        等待发送可用
** input paraments:     ctrl:CAN控制器号，可选项 CAN1/CAN2    
** output paraments:    无    
** Returned values:     无
*********************************************************************************************************/
void CAN_waitReady (uint32_t ctrl)  
{
  LPC_CAN_TypeDef *pCAN = (ctrl == 0) ? LPC_CAN1 : LPC_CAN2;

  while ((pCAN->SR & (1<<2)) == 0);           //等待发送缓冲区1释放可用
	CAN_TxRdy[ctrl] = 1;												//置位发送标志 
}

/*********************************************************************************************************
** Functoin name:       CAN_send
** Descriptions:        发送程序
** input paraments:     ctrl:CAN控制器号，可选项 CAN1/CAN2
**                      msg: 待发送的帧    
** output paraments:    无    
** Returned values:     无
*********************************************************************************************************/
void CAN_send (uint32_t ctrl, CAN_FRAME *msg)  
{
  LPC_CAN_TypeDef *pCAN = (ctrl == 0) ? LPC_CAN1 : LPC_CAN2;
  uint32_t CANData;

  CANData = (((uint32_t) msg->len) << 16)     & 0x000F0000 | 			//数据长度
            (msg->format == EXTENDED_FORMAT ) * 0x80000000 |			//0标准帧与1扩展帧
            (msg->type   == REMOTE_FRAME)     * 0x40000000;				//0数据帧与1远程帧

  if (pCAN->SR & (1<<2))  
	{                                 
    pCAN->TFI1  = CANData;                                        //填充帧信息 
    pCAN->TID1 = msg->id;                                         //填充帧id                     
    pCAN->TDA1 = *(uint32_t *) &msg->data[0];                     //填充数据场前4字节 
    pCAN->TDB1 = *(uint32_t *) &msg->data[4];                     //填充数据场后4字节   
    //pCAN->CMR  = 0x31;                                          //使用自发自收模式      
    pCAN->CMR  = 0x21;                                            //发送                         
  }
}

/*********************************************************************************************************
** Functoin name:       CAN_recive
** Descriptions:        接收程序
** input paraments:     ctrl:CAN控制器号，可选项 CAN1/CAN2
**                      msg:  存储缓冲区     
** output paraments:    无    
** Returned values:     无
*********************************************************************************************************/
void CAN_recive  (uint32_t ctrl, CAN_FRAME *msg)  
{
  LPC_CAN_TypeDef *pCAN = (ctrl == 0) ? LPC_CAN1 : LPC_CAN2;
  uint32_t CANData;
                                                                        
  CANData = pCAN->RFS;																				//读取帧信息 
  msg->format   = (CANData & 0x80000000) == 0x80000000;				//数据帧 或扩展帧
  msg->type     = (CANData & 0x40000000) == 0x40000000;				//标准帧 或远程帧
  msg->len      = ((uint8_t)(CANData >> 16)) & 0x0F;					//数据长度
  msg->id = pCAN->RID;                                        //读取CAN标识符                

  if (msg->type == DATA_FRAME)  
	{                             //读取数据场                   
    *(uint32_t *) &msg->data[0] = pCAN->RDA;
    *(uint32_t *) &msg->data[4] = pCAN->RDB;
  }
  pCAN->CMR = (1 << 2);                                       //释放接收缓冲区
}
unsigned int EXP_STD_ID[2]={0x0001,0x0002};			//接收标识符为0x0001和0x0002的标准标识符帧
unsigned int GRP_STD_ID[2]={0x200,0x210};				//接收标识符在0x200到0x210之间的标准帧标识符
unsigned int EXP_EXT_ID[2]={0x100000,0x100001};	//接收标识符为0x100000和0x100001的扩展标识符帧
unsigned int GRP_EXT_ID[2]={0x200000,0x200010};	//接收标识符在0x200000到0x200010之间的扩展帧标识符
/*********************************************************************************************************
** Functoin name:       CAN_SetACCF_Lookup
** Descriptions:        设置滤波器
** input paraments:     无
** output paraments:    无    
** Returned values:     无
*********************************************************************************************************/
void CAN_SetACCF_Lookup( void )
{
  uint32_t address = 0;
  uint32_t ID_high, ID_low;

  LPC_CANAF->SFF_sa = address;				//设置单个标准帧标识符表格地址
	ID_low = (0 << 29) | (EXP_STD_ID[0] << 16);
	ID_high = ((0) << 13) | (EXP_STD_ID[1] << 0);
	*((volatile uint32_t *)(LPC_CANAF_RAM_BASE + address)) = ID_low | ID_high;
	address += 4; 
		
  LPC_CANAF->SFF_GRP_sa = address;		//设置标准帧组标识符表格地址
	ID_low = (0 << 29) | (GRP_STD_ID[0] << 16);
	ID_high = ((0) << 13) | (GRP_STD_ID[1] << 0);
	*((volatile uint32_t *)(LPC_CANAF_RAM_BASE + address)) = ID_low | ID_high;
	address += 4; 
 
  LPC_CANAF->EFF_sa = address;				//设置单个扩展帧标识符表格地址
	ID_low = (0 << 29) | (EXP_EXT_ID[0] << 0);
	*((volatile uint32_t *)(LPC_CANAF_RAM_BASE + address)) = ID_low;
	address += 4; 
	ID_low = (0 << 29) | (EXP_EXT_ID[1] << 0);
	*((volatile uint32_t *)(LPC_CANAF_RAM_BASE + address)) = ID_low;
	address += 4; 

  LPC_CANAF->EFF_GRP_sa = address;		//设置扩展帧组标识符表格地址
	ID_low = (0 << 29) | (GRP_EXT_ID[0] << 0);
	*((volatile uint32_t *)(LPC_CANAF_RAM_BASE + address)) = ID_low;
	address += 4; 
	ID_low = (0 << 29) | (GRP_EXT_ID[1] << 0);
	*((volatile uint32_t *)(LPC_CANAF_RAM_BASE + address)) = ID_low;
	address += 4; 

  LPC_CANAF->ENDofTable = address;		//设置AF表结束地址
  return;
}

/*********************************************************************************************************
** Functoin name:       CAN_SetACCF
** Descriptions:        设置CAN模式
** input paraments:     ACCFMode 模式设置
** output paraments:    无    
** Returned values:     无
*********************************************************************************************************/
void CAN_SetACCF( uint32_t ACCFMode )
{
  switch ( ACCFMode )
  {
		case ACCF_OFF:		//不接收数据
			LPC_CANAF->AFMR = ACCFMode;
			LPC_CAN1->MOD = LPC_CAN2->MOD = 1;	//复位CAN
			LPC_CAN1->IER = LPC_CAN2->IER = 0;	//禁止接收中断
			LPC_CAN1->GSR = LPC_CAN2->GSR = 0;	//复位错误计数器
		break;

		case ACCF_BYPASS:		//不使用滤波正常接收
			LPC_CANAF->AFMR = ACCFMode;
		break;

		case ACCF_ON:				//使用滤波
		case ACCF_FULLCAN:
			LPC_CANAF->AFMR = ACCF_OFF;
			CAN_SetACCF_Lookup();
			LPC_CANAF->AFMR = ACCFMode;
		break;

		default:
		break;
  }
  return;
}
/*********************************************************************************************************
**                                        End Of File
*********************************************************************************************************/
