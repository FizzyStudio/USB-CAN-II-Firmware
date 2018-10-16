/*******************************************************************************
  * Company: Wang Electronic Technology Co., Ltd.
  ******************************************************************************
  * 文件名称：lcd.c
  * 功能说明：宝马3.2寸LCD屏的驱动
  * 版    本：V1.1
	* 作    者：0915	
  * 日    期：2014-04-09
********************************************************************************
  * 文件名称：lcd.c
  * 功能说明：宝马3.2寸LCD屏的驱动
  * 版    本：
	* 更新作者:	
  * 日    期：
	* 更新原因：
********************************************************************************/


#include <lpc17xx.h>
#include "LCD.h"
#include "font.h"

//static unsigned short DeviceCode;
#define DeviceCode 0x8999
/*********************** Hardware specific configuration **********************/

/* 8bit to 16bit LCD Interface 
   
   PINS: 
   - EN      = P0.19
   - LE      = P0.20
   - DIR     = P0.21
   - CS      = P0.22
   - RS      = P0.23 
   - WR      = P0.24 
   - RD      = P0.25
   - DB[0.7] = P2.0...P2.7
   - DB[8.15]= P2.0...P2.7                                                     */

#define PIN_EN		(1 << 19)
#define PIN_LE		(1 << 20)
#define PIN_DIR		(1 << 21)
#define PIN_CS    (1 << 22)
#define PIN_RS		(1 << 23)
#define PIN_WR		(1 << 24)
#define PIN_RD		(1 << 25)   

/*------------------------- Speed dependant settings -------------------------*/

/* If processor works on high frequency delay has to be increased, it can be 
   increased by factor 2^N by this constant                                   */
#define DELAY_2N    18

/*---------------------- Graphic LCD size definitions ------------------------*/

#define WIDTH       320                 /* Screen Width (in pixels)           */
#define HEIGHT      240                 /* Screen Hight (in pixels)           */
#define BPP         16                  /* Bits per pixel                     */
#define BYPP        ((BPP+7)/8)         /* Bytes per pixel                    */

#define Font_Size 16                 		/* 默认字体为16*8 */
#define XY 1														/* 横竖屏切换，竖屏：1；横屏：0 */

/*--------------- Graphic LCD interface hardware definitions -----------------*/

/* Pin EN setting to 0 or 1                                                   */
#define LCD_EN(x)   ((x) ? (LPC_GPIO0->FIOSET = PIN_EN) : (LPC_GPIO0->FIOCLR = PIN_EN));
/* Pin LE setting to 0 or 1                                                   */
#define LCD_LE(x)   ((x) ? (LPC_GPIO0->FIOSET = PIN_LE) : (LPC_GPIO0->FIOCLR = PIN_LE));
/* Pin DIR setting to 0 or 1                                                   */
#define LCD_DIR(x)   ((x) ? (LPC_GPIO0->FIOSET = PIN_DIR) : (LPC_GPIO0->FIOCLR = PIN_DIR));
/* Pin CS setting to 0 or 1                                                   */
#define LCD_CS(x)   ((x) ? (LPC_GPIO0->FIOSET = PIN_CS) : (LPC_GPIO0->FIOCLR = PIN_CS));
/* Pin RS setting to 0 or 1                                                   */
#define LCD_RS(x)   ((x) ? (LPC_GPIO0->FIOSET = PIN_RS) : (LPC_GPIO0->FIOCLR = PIN_RS));
/* Pin WR setting to 0 or 1                                                   */
#define LCD_WR(x)   ((x) ? (LPC_GPIO0->FIOSET = PIN_WR) : (LPC_GPIO0->FIOCLR = PIN_WR));
/* Pin RD setting to 0 or 1                                                   */
#define LCD_RD(x)   ((x) ? (LPC_GPIO0->FIOSET = PIN_RD) : (LPC_GPIO0->FIOCLR = PIN_RD));
 
/*---------------------------- Global variables ------------------------------*/

/******************************************************************************/
static volatile unsigned short TextColor = Black, BackColor = White;


/************************ Local auxiliary functions ***************************/

/*********************************************************************************************************
** Function name:       delay
** Descriptions:        软件延时
** input parameters:    cnt
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/

static void delay (int cnt) {

  cnt <<= DELAY_2N;
  while (cnt--);
}

__asm void wait()
{
    nop
    BX lr
}

void wait_delay(int count)
{
  while(count--);
}

/*********************************************************************************************************
** Function name:       unsigned char lcd_send (unsigned short byte)
** Descriptions:        向lcd发送数据
** input parameters:    要发送的数据：unsigned short byte
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
static __inline unsigned char lcd_send (unsigned short byte) 
{
  LPC_GPIO2->FIODIR |= 0x000000ff;  //P2.0...P2.7 Output
  LCD_DIR(1)		   				//Interface A->B
  LCD_EN(0)	                        //Enable 2A->2B
  LPC_GPIO2->FIOPIN =  byte;        //Write D0..D7
  LCD_LE(1)
  wait(); 
	wait();	
  LCD_LE(0)							//latch D0..D7
  LPC_GPIO2->FIOPIN =  byte >> 8;   //Write D8..D15 
  return(1);
}

/*********************************************************************************************************
** Function name:       unsigned short lcd_read (void)
** Descriptions:        向lcd读取数据
** input parameters:    无
** output parameters:   无
** Returned value:      从LCD返回的值
*********************************************************************************************************/
static __inline unsigned short lcd_read (void) 
{
  unsigned short id;
  LPC_GPIO2->FIODIR &= 0xffffff00;                //P2.0...P2.7 Input
  LCD_DIR(0)		   				              //Interface B->A
  LCD_EN(0)	                                      //Enable 2B->2A
  wait_delay(20);							      //delay some times
  id = LPC_GPIO2->FIOPIN & 0x00ff;                //Read D8..D15                         
  LCD_EN(1)	                                      //Enable 1B->1A
  wait_delay(20);							      //delay some times
  id = (id << 8) | (LPC_GPIO2->FIOPIN & 0x00ff);  //Read D0..D7                         
  LCD_DIR(1)
  return(id); 
}

/*********************************************************************************************************
** Function name:       void wr_cmd (unsigned char c) 
** Descriptions:        向lcd写命令
** input parameters:    命令 c
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
static __inline void wr_cmd (unsigned char c) 
{
  LCD_CS(0)
	LCD_RS(0)
  LCD_RD(1)
	lcd_send(c);  
  LCD_WR(0)
  wait();
  LCD_WR(1)
	LCD_CS(1)
}

/*********************************************************************************************************
** Function name:       void wr_dat (unsigned short c) 
** Descriptions:        向lcd写数据
** input parameters:    数据：c
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
static __inline void wr_dat (unsigned short c) 
{
  LCD_CS(0)
	LCD_RS(1)
  LCD_RD(1)
  lcd_send(c);
  LCD_WR(0)
  wait();
  LCD_WR(1)
	LCD_CS(1)
}

/*********************************************************************************************************
** Function name:       unsigned short rd_dat (void)
** Descriptions:        读取LCD的数据
** input parameters:    无
** output parameters:   无
** Returned value:      返回LCD的数据
*********************************************************************************************************/
static __inline unsigned short rd_dat (void) 
{
  unsigned short val = 0;

  LCD_CS(0)
	LCD_RS(1)
  LCD_WR(1)
  LCD_RD(0)
  val = lcd_read();
  LCD_RD(1)
	LCD_CS(1)
  return val;
}

/*********************************************************************************************************
** Function name:       void wr_dat_only (unsigned short c)
** Descriptions:        写入数据
** input parameters:    数据 c
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
static __inline void wr_dat_only (unsigned short c) 
{
  LCD_CS(0)
	LCD_RS(1)
	lcd_send(c);
  LCD_WR(0)
  wait();
  LCD_WR(1)
	LCD_CS(1)
}

/*********************************************************************************************************
** Function name:       void wr_reg (unsigned char reg, unsigned short val)
** Descriptions:        配置寄存器
** input parameters:    寄存器地址：unsigned char reg ； 数据：unsigned short val
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
static __inline void wr_reg (unsigned char reg, unsigned short val) 
{
  wr_cmd(reg);
  wr_dat(val);
}

/*********************************************************************************************************
** Function name:       unsigned short rd_reg (unsigned short reg) 
** Descriptions:        读寄存器的值
** input parameters:    寄存器地址：unsigned char reg 
** output parameters:   无
** Returned value:      寄存器的值
*********************************************************************************************************/

static unsigned short rd_reg (unsigned short reg) 
{
  unsigned short val = 0;

  wr_cmd(reg);
  val = rd_dat(); 
  return (val);
}

/************************ Exported functions **********************************/

/*********************************************************************************************************
** Function name:       void LCD_Init (void) 
** Descriptions:        LCD初始化
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void LCD_Init (void) 
{ 
  /* Configure the LCD Control pins */

  LPC_GPIO2->FIODIR   |= 0x000000FF;
  LPC_GPIO0->FIODIR   |= 0x03f80000;					/*P0.19--P0.25           		*/
  LPC_GPIO0->FIOSET    = 0x03f80000;					/*设置P0.19--P0.25 为输出		*/

  delay(5);                            				 	/* Delay 50 ms           		*/

//  DeviceCode = rd_reg(0x00);			/* 读取屏的ID */

	if(DeviceCode==0x9325||DeviceCode==0x9328)			/*ILI9325	  大屏9320 小屏9325*/
	{
		wr_reg(0x00e7,0x0010);      
		wr_reg(0x0000,0x0001);
		wr_reg(0x0001,0x0100);     
		wr_reg(0x0002,0x0700);                   
		wr_reg(0x0003,(1<<12)|(3<<4)|(0<<3) );    
		wr_reg(0x0004,0x0000);                                   
		wr_reg(0x0008,0x0207);	           
		wr_reg(0x0009,0x0000);         
		wr_reg(0x000a,0x0000);         
		wr_reg(0x000c,0x0001);         
		wr_reg(0x000d,0x0000);          
		wr_reg(0x000f,0x0000);

		wr_reg(0x0010,0x0000);   
		wr_reg(0x0011,0x0007);
		wr_reg(0x0012,0x0000);                                                                 
		wr_reg(0x0013,0x0000);                 
		delay(5); 
		wr_reg(0x0010,0x1590);   
		wr_reg(0x0011,0x0227);
		delay(5); 
		wr_reg(0x0012,0x009c);                  
		delay(5); 
		wr_reg(0x0013,0x1900);   
		wr_reg(0x0029,0x0023);
		wr_reg(0x002b,0x000e);
		delay(5); 
		wr_reg(0x0020,0x0000);                                                            
		wr_reg(0x0021,0x013f);           
		delay(5); 
		wr_reg(0x0030,0x0007); 
		wr_reg(0x0031,0x0707);   
		wr_reg(0x0032,0x0006);
		wr_reg(0x0035,0x0704);
		wr_reg(0x0036,0x1f04); 
		wr_reg(0x0037,0x0004);
		wr_reg(0x0038,0x0000);        
		wr_reg(0x0039,0x0706);     
		wr_reg(0x003c,0x0701);
		wr_reg(0x003d,0x000f);
		delay(5); 

		wr_reg(0x0050,0x0000);
		wr_reg(0x0051,0x00ef);                   
		wr_reg(0x0052,0x0000);                   
		wr_reg(0x0053,0x013f); 
		
		wr_reg(0x0060,0xa700);        
		wr_reg(0x0061,0x0001); 
		wr_reg(0x006a,0x0000);
		wr_reg(0x0080,0x0000);
		wr_reg(0x0081,0x0000);
		wr_reg(0x0082,0x0000);
		wr_reg(0x0083,0x0000);
		wr_reg(0x0084,0x0000);
		wr_reg(0x0085,0x0000);
	
		wr_reg(0x0090,0x0010);     
		wr_reg(0x0092,0x0000);  
		wr_reg(0x0093,0x0003);
		wr_reg(0x0095,0x0110);
		wr_reg(0x0097,0x0000);        
		wr_reg(0x0098,0x0000);  
	 
		wr_reg(0x0007,0x0133);   
		wr_reg(0x0020,0x0000);                                                            
		wr_reg(0x0021,0x013f); 
	}else if(DeviceCode==0x9320||DeviceCode==0x9300)
	{
    /* Start Initial Sequence --------------------------------------------------*/
		wr_reg(0x00, 0x0001);                 /* Start internal OSC                 */
		wr_reg(0x01, 0x0100);                 /* Set SS and SM bit                  */
		wr_reg(0x02, 0x0700);                 /* Set 1 line inversion               */
		wr_reg(0x03, 0x1038);                 /* Set GRAM write direction and BGR=1 */
		wr_reg(0x04, 0x0000);                 /* Resize register                    */
		wr_reg(0x08, 0x0202);                 /* 2 lines each, back and front porch */
		wr_reg(0x09, 0x0000);                 /* Set non-disp area refresh cyc ISC  */
		wr_reg(0x0A, 0x0000);                 /* FMARK function                     */
		wr_reg(0x0C, 0x0000);                 /* RGB interface setting              */
		wr_reg(0x0D, 0x0000);                 /* Frame marker Position              */
		wr_reg(0x0F, 0x0000);                 /* RGB interface polarity             */
		
		/* Power On sequence -------------------------------------------------------*/
		wr_reg(0x10, 0x0000);                 /* Reset Power Control 1              */
		wr_reg(0x11, 0x0000);                 /* Reset Power Control 2              */
		wr_reg(0x12, 0x0000);                 /* Reset Power Control 3              */
		wr_reg(0x13, 0x0000);                 /* Reset Power Control 4              */
		delay(20);                            /* Discharge cap power voltage (200ms)*/
		wr_reg(0x10, 0x17B0);                 /* SAP, BT[3:0], AP, DSTB, SLP, STB   */
		wr_reg(0x11, 0x0137);                 /* DC1[2:0], DC0[2:0], VC[2:0]        */
		delay(5);                             /* Delay 50 ms                        */
		wr_reg(0x12, 0x0139);                 /* VREG1OUT voltage                   */
		delay(5);                             /* Delay 50 ms                        */
		wr_reg(0x13, 0x1D00);                 /* VDV[4:0] for VCOM amplitude        */
		wr_reg(0x29, 0x0013);                 /* VCM[4:0] for VCOMH                 */
		delay(5);                             /* Delay 50 ms                        */
		wr_reg(0x20, 0x0000);                 /* GRAM horizontal Address            */
		wr_reg(0x21, 0x0000);                 /* GRAM Vertical Address              */
		
		/* Adjust the Gamma Curve --------------------------------------------------*/
		wr_reg(0x30, 0x0006);
		wr_reg(0x31, 0x0101);
		wr_reg(0x32, 0x0003);
		wr_reg(0x35, 0x0106);
		wr_reg(0x36, 0x0B02);
		wr_reg(0x37, 0x0302);
		wr_reg(0x38, 0x0707);
		wr_reg(0x39, 0x0007);
		wr_reg(0x3C, 0x0600);
		wr_reg(0x3D, 0x020B);
			
		/* Set GRAM area -----------------------------------------------------------*/
		wr_reg(0x50, 0x0000);                 /* Horizontal GRAM Start Address      */
		wr_reg(0x51, (HEIGHT-1));             /* Horizontal GRAM End   Address      */
		wr_reg(0x52, 0x0000);                 /* Vertical   GRAM Start Address      */
		wr_reg(0x53, (WIDTH-1));              /* Vertical   GRAM End   Address      */
		wr_reg(0x60, 0x2700);                 /* Gate Scan Line                     */
		wr_reg(0x61, 0x0001);                 /* NDL,VLE, REV                       */
		wr_reg(0x6A, 0x0000);                 /* Set scrolling line                 */
		
		/* Partial Display Control -------------------------------------------------*/
		wr_reg(0x80, 0x0000);
		wr_reg(0x81, 0x0000);
		wr_reg(0x82, 0x0000);
		wr_reg(0x83, 0x0000);
		wr_reg(0x84, 0x0000);
		wr_reg(0x85, 0x0000);
		
		/* Panel Control -----------------------------------------------------------*/
		wr_reg(0x90, 0x0010);
		wr_reg(0x92, 0x0000);
		wr_reg(0x93, 0x0003);
		wr_reg(0x95, 0x0110);
		wr_reg(0x97, 0x0000);
		wr_reg(0x98, 0x0000);	   
		wr_reg(0x07, 0x0137);                 /* 262K color and display ON          */ 
	}
	else if(DeviceCode==0x8999)	   //对应的驱动IC为SSD1289
	{
		//************* Start Initial Sequence **********//
		wr_reg(0x00, 0x0001); // Start internal OSC.
		wr_reg(0x01, 0x3B3F); // Driver output control, RL=0;REV=1;GD=1;BGR=0;SM=0;TB=1
		wr_reg(0x02, 0x0600); // set 1 line inversion
		//************* Power control setup ************/
		wr_reg(0x0C, 0x0007); // Adjust VCIX2 output voltage
		wr_reg(0x0D, 0x0006); // Set amplitude magnification of VLCD63
		wr_reg(0x0E, 0x3200); // Set alternating amplitude of VCOM
		wr_reg(0x1E, 0x00BB); // Set VcomH voltage
		wr_reg(0x03, 0x6A64); // Step-up factor/cycle setting  7b30
		//************ RAM position control **********/
		wr_reg(0x0F, 0x0000); // Gate scan position start at G0.
		wr_reg(0x44, 0xEF00); // Horizontal RAM address position
		wr_reg(0x45, 0x0000); // Vertical RAM address start position
		wr_reg(0x46, 0x013F); // Vertical RAM address end position
		// ----------- Adjust the Gamma Curve ----------//
		wr_reg(0x30, 0x0000);
		wr_reg(0x31, 0x0706);
		wr_reg(0x32, 0x0206);
		wr_reg(0x33, 0x0300);
		wr_reg(0x34, 0x0002);
		wr_reg(0x35, 0x0000);
		wr_reg(0x36, 0x0707);
		wr_reg(0x37, 0x0200);
		wr_reg(0x3A, 0x0908);
		wr_reg(0x3B, 0x0F0D);
		//************* Special command **************/
		wr_reg(0x28, 0x0006); // Enable test command
		wr_reg(0x2F, 0x12EB); // RAM speed tuning
		wr_reg(0x26, 0x7000); // Internal Bandgap strength
		wr_reg(0x20, 0xB0E3); // Internal Vcom strength
		wr_reg(0x27, 0x0044); // Internal Vcomh/VcomL timing
		wr_reg(0x2E, 0x7E45); // VCOM charge sharing time  
		//************* Turn On display ******************/
		wr_reg(0x10, 0x0000); // Sleep mode off.
		delay(8); // Wait 30mS
		wr_reg(0x11, 0x6870); // Entry mode setup. 262K type B, take care on the data bus with 16it only 
		wr_reg(0x07, 0x0033); // Display ON	*/
	}
}

/*********************************************************************************************************
** Function name:       void GLCD_WindowMax (void) 
** Descriptions:        设置窗口最大化240*320
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void GLCD_WindowMax (void) 
{
	if(DeviceCode==0x8999)
	{
		wr_reg(0x44,0|((HEIGHT-1)<<8));
		wr_reg(0x45,0);
		wr_reg(0x46,WIDTH-1);
	}
	else
	{	
		wr_reg(0x50, 0);                      /* Horizontal GRAM Start Address      */
		wr_reg(0x51, HEIGHT-1);               /* Horizontal GRAM End   Address (-1) */
		wr_reg(0x52, 0);                      /* Vertical   GRAM Start Address      */
		wr_reg(0x53, WIDTH-1);                /* Vertical   GRAM End   Address (-1) */
	}
}

/*********************************************************************************************************
** Function name:       void LCD_WindowMax (void) 
** Descriptions:        设置窗口
** input parameters:    x轴起始点：unsigned int x
**											x轴终  点：unsigned int x_end
**											y轴起始点：unsigned int y
**											y轴终  点：unsigned int y_end
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void LCD_WindowMax (unsigned int x,unsigned int y,unsigned int x_end,unsigned int y_end) 
{
	if(DeviceCode==0x8999)
	{
		wr_reg(0x44,x|((x_end-1)<<8));
		wr_reg(0x45,y);
		wr_reg(0x46,y_end-1);
	}
	else
	{
		wr_reg(0x50, x);                      /* Horizontal GRAM Start Address      */
		wr_reg(0x51, x_end-1);               /* Horizontal GRAM End   Address (-1) */
		wr_reg(0x52, y);                      /* Vertical   GRAM Start Address      */
		wr_reg(0x53, y_end-1);                /* Vertical   GRAM End   Address (-1) */
	}
}

/*********************************************************************************************************
** Function name:       void LCD_PutPixel (unsigned int x, unsigned int y) 
** Descriptions:        打点
** input parameters:    x轴起始点：unsigned int x
**											y轴起始点：unsigned int y
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void LCD_PutPixel (unsigned int x, unsigned int y) 
{
  wr_reg(0x20, y);
  wr_reg(0x21, WIDTH-1-x);
  wr_cmd(0x22);
  wr_dat(TextColor);
}

/*********************************************************************************************************
** Function name:       void LCD_SetTextColor (unsigned short color) 
** Descriptions:        设置字体颜色
** input parameters:    字体颜色值：unsigned short color
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void LCD_SetTextColor (unsigned short color) 
{
  TextColor = color;
}

/*********************************************************************************************************
** Function name:       void LCD_SetBackColor (unsigned short color) 
** Descriptions:        设置背景颜色
** input parameters:  	背景颜色值：unsigned short color
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void LCD_SetBackColor (unsigned short color) 
{
  BackColor = color;
}

/*********************************************************************************************************
** Function name:       void LCD_SetCursor(uint8_t Xpos, uint16_t Ypos) 
** Descriptions:        设置光标位置
** input parameters:  	x轴坐标：uint8_t Xpos  ；  y轴坐标：uint16_t Ypos
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void LCD_SetCursor(uint8_t Xpos, uint16_t Ypos)
{
 	if(DeviceCode==0x8999||DeviceCode==0x9919)
	{
		wr_reg(0x004E, Xpos);
		wr_reg(0X004F, Ypos);
	}
	else 
	{
		wr_reg(0x20, Xpos);
		wr_reg(0x21, Ypos);
	}
}

/*********************************************************************************************************
** Function name:       void LCD_Clear (unsigned short color) 
** Descriptions:        刷屏函数
** input parameters:  	颜色值
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void LCD_Clear (unsigned short color) 
{
  unsigned int   i;

  GLCD_WindowMax();
  LCD_SetCursor(0,0);
  wr_cmd(0x22);
  for(i = 0; i < (WIDTH*HEIGHT); i++)
	{
    wr_dat_only(color);
	}
}

/*********************************************************************************************************
** Function name:       void LCD_DrawChar (unsigned int x, unsigned int y, unsigned short *c)
** Descriptions:        写字符
** input parameters:  	x轴起始点：unsigned int x
**											y轴起始点：unsigned int y
**											字符偏移量：unsigned char c
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void LCD_DrawChar (unsigned int x, unsigned int y, unsigned char c) 
{	
  uint8_t temp;
  uint8_t pos,t;	
	if(XY)
	{
		LCD_WindowMax(x,y,x+Font_Size/2,y+Font_Size);	   /*设置窗口	*/
		wr_reg(0x11, 0x6870);
		LCD_SetCursor(x, y); 
	}
	else
	{
		LCD_WindowMax(x,y,x+Font_Size,y+Font_Size/2);	   /*设置窗口	*/
		wr_reg(0x11, 0x6858);
		LCD_SetCursor(x, y);                  /*设置光标位置 */
	}  
	wr_cmd(0x22);               /*开始写入GRAM  */   
	for(pos=0;pos<Font_Size;pos++)
	{
		temp=asc2_1608[c][pos];		 /*调用1608字体	*/
		for(t=0;t<Font_Size/2;t++)
	  {                 
	    if(temp&0x01)			   /*从低位开始*/
			{
				wr_dat_only(TextColor);  /*画字体颜色 一个点*/
			}
			else
			{
				wr_dat_only(BackColor);	   /*画背景颜色 一个点*/
			}				
	    temp>>=1; 
	  }
	}
	wr_reg(0x11, 0x6870); 	
	GLCD_WindowMax();;	/*恢复窗体大小*/	
}

/*********************************************************************************************************
** Function name:       void LCD_DrawChar (unsigned int x, unsigned int y, unsigned short *c)
** Descriptions:        写字符
** input parameters:  	X起始：unsigned int x
**											Y起始：unsigned int y
**											字符：unsigned char c
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void LCD_DisplayChar (unsigned int x, unsigned int y, unsigned char c) 
{
  c -= ' ';
	if(Font_Size==16)
		LCD_DrawChar(x, y, c);
}

/*********************************************************************************************************
** Function name:       unsigned int mypow(unsigned int m,unsigned int n)
** Descriptions:        m的n次方 用于取出数值每一位
** input parameters:  	m进制：unsigned int m
**											n次方：unsigned int n						
** output parameters:   m^n
** Returned value:      无
*********************************************************************************************************/
unsigned int mypow(unsigned int m,unsigned int n)
{
	unsigned int result=1;	 
	while(n--)result*=m;    
	return result;
}	

/*********************************************************************************************************
** Function name:       void LCD_DisplayNum(unsigned int x,unsigned int y,unsigned int num,unsigned int len)
** Descriptions:        显示数值
** input parameters:  	X起始：unsigned int x
**											Y起始：unsigned int y
**											数值 ：unsigned int num
**  										有效长度 ：unsigned int len
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void LCD_DisplayNum(unsigned int x,unsigned int y,unsigned int num,unsigned int len)
{         	
	unsigned int  t,temp;
	unsigned int  enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_DisplayChar(x+(Font_Size/2)*t,y,' ');
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_DisplayChar(x+(Font_Size/2)*t,y,temp+'0'); 
	}
} 

/*********************************************************************************************************
** Function name:       void LCD_DisplayString (unsigned int ln, unsigned int col, unsigned char *s)
** Descriptions:        写字符串,自动换行
** input parameters:  	X起始：unsigned int x
**											Y起始：unsigned int y
**											字符串地址：unsigned char *s
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void LCD_DisplayString (unsigned int x, unsigned int y, unsigned char *s) 
{
  while (*s) 
	{    
		if(XY)
		{
			LCD_DisplayChar(x,y, *s++);
			x+=Font_Size/2;
			if(x>=232)
			{
				x=0;
				y+=Font_Size;
				if(y>=304)y=0;
			}
		}
		else
		{
			LCD_DisplayChar(x,320-y, *s++);
			y+=Font_Size/2;
			if(y>=312)
			{
				y=0;
				x+=Font_Size;
				if(x>=224)y=0;
			}
		}
  }
}

/*********************************************************************************************************
** Function name:       void LCD_ClearLn (unsigned int ln) 
** Descriptions:        清除一行
** input parameters:    行号：unsigned int ln
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void LCD_ClearLn (unsigned int ln) 
{
  LCD_DisplayString(ln, 0, "                    ");
}

/*********************************************************************************************************
** Function name:       void LCD_Bmp (unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned char *bmp)  
** Descriptions:        显示BMP图片
** input parameters:    x轴起始点：unsigned int x
**											y轴起始点：unsigned int y
**											图片宽度：unsigned int w
**											图片高度：unsigned int h
**											图片数组地址：unsigned char *bmp
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void LCD_Bmp (unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned char *bmp) 
{
  unsigned int    i, j;
  unsigned short *bitmap_ptr = (unsigned short *)bmp;

  x = WIDTH-x-w;
	wr_reg(0x11, 0x6868);
  LCD_WindowMax(y,x,y+h,x+w);

  LCD_SetCursor(y,x);
  wr_cmd(0x22);
  
  bitmap_ptr += (h*w)-1;
  for (j = 0; j < h; j++) {
    for (i = 0; i < w; i++) {
      wr_dat_only(*bitmap_ptr--);
    }
  }
	wr_reg(0x11, 0x6870);
}

/******************************************************************************/
