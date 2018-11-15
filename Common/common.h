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

#define		USBCAN2		0x04

#define		HARDWARE_VERSION	0x0100
#define		FIRMWARE_VERSION	0x0100
#define		DRIVER_VERSION		0x0100
#define		INTERFACE_VERSION	0x0100
#define		IRQ_NUM				25
#define		CAN_NUM				2
#define		BOARD_SERIAL_NO		""
#define		HARDWARE_TYPE		"USB-CAN-II V1.00"

/*********************************************************************************************************
	CAN_BOARD_INFO 结构体包含接口卡的设备信息。结构体将在CAN_ReadBoardInfo函数中被填充。
*********************************************************************************************************/
typedef struct _CAN_BOARD_INFO {
	USHORT hw_Version;				//硬件版本号，用 16 进制表示。比如 0x0100 表示 V1.00。
	USHORT fw_Version;				//固件版本号，用 16 进制表示
	USHORT dr_Version;				//驱动程序版本号，用 16 进制表示
	USHORT in_Version;				//接口库版本号，用 16 进制表示
	USHORT irq_Num;					//板卡所使用的中断号
	BYTE can_Num;					//板卡CAN通道数
	CHAR str_Serial_Num[20];		//板卡序列号
	CHAR str_hw_Type[40];			//硬件类型，比如“USBCAN V1.00”（注意：包括字符串结束符’ \0’）
	USHORT Reserved[4];				//系统保留
} CAN_BOARD_INFO, *PCAN_BOARD_INFO;

/*********************************************************************************************************
	CAN_OBJ 结构体是 CAN 帧结构体，即 1 个结构体表示一个帧的数据结构。
	在发送函数 VCI_Transmit 和接收函数 VCI_Receive 中，被用来传送 CAN 信息帧。
*********************************************************************************************************/
typedef struct _CAN_OBJ {
	UINT ID;						//帧ID。 32 位变量，数据格式为靠右对齐。
	UINT TimeStamp;					//设备接收到某一帧的时间标识。只有智能卡才有时间标示，如 USBCAN 系列与PCI-5010/20。 时间标示从 CAN 卡上电开始计时，计时单位为 0.1ms。
	BYTE TimeFlag;					//是否使用时间标识。为 1 时 TimeStamp 有效， TimeFlag 和 TimeStamp 只在此帧为接收帧时有意义。
	BYTE SendType;					//发送帧类型 0正常发送 1单次发送 2自发自收 3单次自发自收 只在此帧为发送帧时有意义
	BYTE RemoteFlag;				//是否是远程帧 =0 时为为数据帧， =1 时为远程帧
	BYTE ExternFlag;				//是否是扩展帧 =0 时为标准帧（11 位 ID）， =1 时为扩展帧（29 位 ID）
	BYTE DataLen;					//数据长度 DLC (<=8)，即 CAN 帧 Data 有几个字节
	BYTE Data[8];					//CAN 帧的数据
	BYTE Reserved[3];
} CAN_OBJ, *PCAN_OBJ;

/*********************************************************************************************************
	CAN_STATUS 结构体包含 CAN 设备中的 CAN 控制器状态信息(此函数只对使用SJA1000 控制器的设备有效)。
	结构体将在 VCI_ReadCANStatus 函数中调用时，被填充.
*********************************************************************************************************/
typedef struct _CAN_STATUS {
	UCHAR ErrInterrupt;				//中断记录，读操作会清除中断
	UCHAR regMode;					//CAN 控制器模式寄存器值
	UCHAR regStatus;				//CAN 控制器状态寄存器值
	UCHAR regALCapture;				//CAN 控制器仲裁丢失寄存器值
	UCHAR regECCapture;				//CAN 控制器错误寄存器值
	UCHAR regEWLimit;				//CAN 控制器错误警告限制寄存器值。默认为 96
	UCHAR regRECounter;				//CAN 控制器接收错误寄存器值。为 0-127 时，为错误主动状态，为 128-254 为错误被动状态，为 255 时为总线关闭状态
	UCHAR regTECounter;				//CAN 控制器发送错误寄存器值。为 0-127 时，为错误主动状态，为 128-254 为错误被动状态，为 255 时为总线关闭状态。
	DWORD Reserved;
} CAN_STATUS, *PCAN_STATUS;

/*********************************************************************************************************
	CAN_ERR_INFO 结构体用于装载库运行时产生的错误信息 。结构体将在CAN_ReadErrInfo 函数中被填充
*********************************************************************************************************/
typedef struct _ERR_INFO {
	UINT ErrCode;					//错误码
	BYTE Passive_ErrData[3];		//当产生的错误中有消极错误时表示为消极错误的错误标识数据
	BYTE ArLost_ErrData;			//当产生的错误中有仲裁丢失错误时表示为仲裁丢失错误的错误标识数据
} CAN_ERR_INFO, *PCAN_ERR_INFO;

/*********************************************************************************************************
	CAN_INIT_CONFIG 结构体定义了初始化 CAN 的配置。结构体将在 VCI_InitCAN 函数中被填充，即初始化之前，要先填好这个结构体变量
*********************************************************************************************************/
typedef struct _INIT_CONFIG {
	DWORD AccCode;					//验收码。 SJA1000 的帧过滤验收码
	DWORD AccMask;					//屏蔽码。 SJA1000 的帧过滤屏蔽码
	DWORD Reserved;					
	UCHAR Filter;					//滤波方式。 =1 表示单滤波， =0 表示双滤波
	UCHAR Timing0;					//波特率定时器 0（BTR0）
	UCHAR Timing1;					//波特率定时器 1
	UCHAR Mode;						//模式。 =0 表示正常模式（相当于正常节点）， =1 表示只听模式（只接收，不影响总线）
} CAN_INIT_CONFIG, *PCAN_INIT_CONFIG;

/*********************************************************************************************************
	CHGDESIPANDPORT 结构体用于装载更改 CANET_UDP 与 CANET_TCP 的目标 IP 和端口的必要信息。此结构体在 CANETE_UDP 与 CANET_TCP 中使用
*********************************************************************************************************/
typedef struct _tagChgDesIPAndPort {
	char szpwd[10];					//更改目标 IP 和端口所需要的密码，长度小于 10，比如为“11223344”。
	char szdesip[20];				//所要更改的目标 IP，比如为“192.168.0.111”
	int desport;					//所要更改的目标端口，比如为 4000。
	BYTE blisten;					//所要更改的工作模式， 0 表示正常模式， 1 表示只听模式。
} CHGDESIPANDPORT;

/*********************************************************************************************************
	当设备类型为 PCI-5010-U、PCI-5020-U、USBCAN-E-U、USBCAN-2E-U、USBCAN-4E-U时，定义了滤波器的滤波范围 CAN_FILTER_RECORD 结构体。
	结构体在 VCI_SetReference函数中被填充。
*********************************************************************************************************/
typedef struct _CAN_FILTER_RECORD{
	DWORD ExtFrame;					//过滤的帧类型标志，为 1 代表要过滤的为扩展帧，为 0 代表要过滤的为标准帧。
	DWORD Start;					//滤波范围的起始帧 ID
	DWORD End;						//滤波范围的结束帧 ID
}CAN_FILTER_RECORD,*PCAN_FILTER_RECORD;

/*********************************************************************************************************
	当设备类型为 PCI-5010-U、PCI-5020-U、USBCAN-E-U、USBCAN-2E-U、USBCAN-4E-U、USBCAN-8E-U 时，
	设备支持自定义一组硬件层自动定时发送帧列表（列表最大支持 32 帧），
	列表中的发送帧可以按照设置的周期定时发送 CAN 帧，发送过程无需上位机软件干预，
	该发送列表中的发送帧由 VCI_AUTO_SEND_OBJ 设置，该结构体在CAN_SetReference 函数中被填充。
*********************************************************************************************************/
typedef struct _CAN_AUTO_SEND_OBJ{
	BYTE Enable;							//是否使能本条报文。 0：禁能 1：使能。
	BYTE Index;								//报文编号。 最大支持 32 条报文，即取值范围为 0 至 31
	DWORD Interval;							//定时发送时间。 0.5ms 为单位
	AN_OBJ Obj;								//报文
}CAN_AUTO_SEND_OBJ, *PCAN_AUTO_SEND_OBJ;


/*********************************************************************************************************
	当设备类型为 USBCAN-4E-U 时，设置 CAN 通道的状态指示灯，该结构体在CAN_SetReference 函数中被填充。
*********************************************************************************************************/
typedef struct _CAN_INDICATE_LIGHT{
	BYTE Indicate; 							//指示灯编号
	BYTE AttribRedMode:2; 					//Red LED灭/亮/闪烁/自控
	BYTE AttribGreenMode:2; 				//Green LED灭/亮/闪烁/自控
	BYTE AttribReserved:4; 					//保留暂时不用
	BYTE FrequenceRed:2; 					//Red LED闪烁频率
	BYTE FrequenceGreen:2; 					//Green LED闪烁频率
	BYTE FrequenceReserved:4; 				//保留暂时不用
} CAN_INDICATE_LIGHT, *PCAN_INDICATE_LIGHT;


/*********************************************************************************************************
	当设备类型为 CANDTU 时，设置 CANDTU 参数信息，该结构体在 VCI_SetReference函数中被填充。 CANDTU 使用文本配置对设备进行波特率，滤波参数的配置.
*********************************************************************************************************/
typedef struct _DTUCOMCONFIG
{
	DWORD dwLen;					//配置数据的大小。
	BYTE* pData;					//配置内容的地址。
} DTUCOMCONFIG;


#endif