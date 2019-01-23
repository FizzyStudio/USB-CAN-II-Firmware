/*******************************************************************************
  * Company: Wang Electronic Technology Co., Ltd.
  ******************************************************************************
  * 文件名称：main.c
  * 功能说明：宝马USB HID例程
  * 版    本：V1.1
	* 作    者：0915	
  * 日    期：2014-04-22
********************************************************************************
  * 文件名称：main.c
  * 功能说明：主函数
  * 版    本：
	* 更新作者:	
  * 日    期：
	* 更新原因：
********************************************************************************/
#include "lpc17xx.h"
#include "main.h"
#include "usb.h"
#include "usbcfg.h"
#include "usbhw.h"

unsigned char InReport;   // 保存了按键的值
unsigned char OutReport;  // 获取到LED的控制值                              
     
/*********************************************************************************************************
** Function name:       void GetInReport (void)
** Descriptions:        输出数据获得函数
** input parameters:    无
** output parameters:   输出的数据:InReport
** Returned value:      无
*********************************************************************************************************/
void GetInReport (void) {
  uint32_t kbd_val;

  kbd_val = (LPC_GPIO1->FIOPIN >> 25) & KBD_MASK; 
    
  InReport = 0x00;
  if ((kbd_val & KBD_UP)     == 0) InReport |= 0x01;  /* up     pressed means 0 */
  if ((kbd_val & KBD_LEFT)   == 0) InReport |= 0x02;  /* left   pressed means 0 */
  if ((kbd_val & KBD_RIGHT)  == 0) InReport |= 0x04;  /* right  pressed means 0 */
  if ((kbd_val & KBD_SELECT) == 0) InReport |= 0x08;  /* select pressed means 0 */
  if ((kbd_val & KBD_DOWN)   == 0) InReport |= 0x10;  /* down   pressed means 0 */
}

/*********************************************************************************************************
** Function name:       void SetOutReport (void)
** Descriptions:        数据接收处理函数
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void SetOutReport (void) {
  static unsigned long led_mask[] = { 1<<0, 1<<1, 1<<2, 1<<3, 1<<4, 1<<5, 1<<6, 1<<7 };
  int i;

  for (i = 0; i < LED_NUM; i++) {
    if (OutReport & (1<<i)) {
			// OutReport表示传进来的值，第i位为1，点亮该位对应的LED
      LPC_GPIO2->FIOPIN |= led_mask[i];
    } else {
			// 第i位为0，熄灭对应的LED
      LPC_GPIO2->FIOPIN &= ~led_mask[i];
    }
  }
}

/*********************************************************************************************************
** Function name:       int main (void)
** Descriptions:        主函数
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
int main (void) {

	// 配置GPIO2.0~7为输出引脚
  LPC_GPIO2->FIODIR   |= ((1UL<< 0)|(1UL<< 1)|
													(1UL<< 2)|(1UL<< 3)|
                          (1UL<< 4)|(1UL<< 5)|
                          (1UL<< 6)|(1UL<< 7)) ; /* P2.0到P2.7为LED灯接口 */

  // 配置GPIO1.25~29为输入引脚
  LPC_GPIO1->FIODIR   &= ~((1<<25)|(1<<26)|
                           (1<<27)|(1<<28)|
                           (1<<29)         ); /* P1.25到P1.29是五向按键的输入 */

  LPC_GPIO0->FIODIR = 0x00200000; 
  LPC_GPIO0->FIOPIN |= 0x00200000; 

  USB_Init();                                 /* USB初始化 */
  USB_Connect(__TRUE);                        /* USB 连接 */

  while (1);                                  
}
/******************************************************************************
**                            End Of File
******************************************************************************/
