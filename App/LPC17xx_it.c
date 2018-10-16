#include "lpc17xx.h"                              /* LPC17xx definitions    */
#include "uart.h"
#include "can.h"

volatile unsigned char UART2_BUF; 
void UART2_IRQHandler(void)
{
    uint32_t flag;
    flag  = LPC_UART2->IIR;                                             /* 读取(清除)中断中断状态       */
    flag &= 0x0F;
   
    if((flag == 0x04) && (LPC_UART2->LSR & 0x01)) {                     /* 如果是接收中断，并且有数据   */
        UART2_BUF = LPC_UART2->RBR;                                     /* 读取数据                     */
//        UART2_SendByte (UART2_BUF);                                     /* 串口回显数据                 */           
    }
		CAN_waitReady(CAN1); 
		if (CAN_TxRdy[CAN1]) 
		{                                              /* 使用控制器1发送              */
        CAN_TxRdy[CAN1] = 0; 
				CAN_TxMsg[CAN1].data[0] =  UART2_BUF;                          /* 填充数据                     */
        CAN_send (CAN1, &CAN_TxMsg[CAN1]);                          /* 发送数据                     */
		}
}

/*********************************************************************************************************
** Functoin name:       CAN_IRQHandler
** Descriptions:        CAN中断服务程序
** input paraments:     无
** output paraments:    无    
** Returned values:     无
*********************************************************************************************************/
void CAN_IRQHandler (void)  
{
  volatile uint32_t icr;
                                                                      
  icr = LPC_CAN1->ICR;               	//读取中断信息  读取清零                                                                            
//	icr = LPC_CAN2->ICR;                //读取中断信息  读取清零   
	
  if (icr & (1 << 0))  								//接收中断
	{                          
    CAN_recive(CAN1, &CAN_RxMsg[CAN1]);  //读取数据   
		UART2_SendByte (CAN_RxMsg[CAN1].data[0]); 		
    CAN_RxRdy[CAN1] = 1;              //置位接收标志                           
  }
  if (icr & (1 << 1)) 								//缓冲区1发送完成   
	{                           				                      
		CAN_TxRdy[CAN1] = 1;  						//置位发送标志 
  }
}

