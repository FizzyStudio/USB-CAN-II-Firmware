/****************************************Copyright (c)****************************************************
**                                 
**--------------File Info---------------------------------------------------------------------------------
** File name:           error.h
** Last modified Date:  2018-10-16
** Last Version:        V1.00
** Descriptions:        
**
**--------------------------------------------------------------------------------------------------------
** Created by:          letheascetic
** Created date:        2018-10-16
** Version:             V1.00
** Descriptions:        USB-CAN-II Error Descriptions
**
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#ifndef _ERROR_H_
#define _ERROR_H_

/*****************************************CAN错误********************************************************/
#define		ERR_CAN_OVERFLOW		0x00000001		//CAN控制器FIFO溢出
#define		ERR_CAN_ERRALARM		0x00000002		//CAN控制器错误报警
#define		ERR_CAN_PASSIVE			0x00000004		//CAN控制器消极错误
#define		ERR_CAN_LOSE				0x00000008		//CAN控制器仲裁丢失
#define		ERR_CAN_BUSERR			0x00000010		//CAN控制器总线错误
#define		ERR_CAN_BUSOFF			0x00000020		//CAN控制器总线关闭

/*****************************************通用错误码*****************************************************/
#define		ERR_DEVICEOPENED		0x00000100		//设备已打开
#define		ERR_DEVICEOPEN			0x00000200		//打开设备错误
#define		ERR_DEVICENOTOPEN		0x00000400		//设备没有打开
#define		ERR_BUFFEROVERFLOW	0x00000800		//缓冲区溢出
#define		ERR_DEVICENOTEXIST	0x00001000		//此设备不存在
#define		ERR_LOADKERNELDLL		0x00002000		//装载动态库失败
#define		ERR_CMDFAILED				0x00004000		//执行命令失败错误码
#define		ERR_BUFFERCREATE		0x00008000		//内存不足

/****************************************CANET错误码*****************************************************/
#define		ERR_CANETE_PORTOPENED		0x00010000		//端口已经被打开
#define		ERR_CANETE_INDEXUSED		0x00020000		//设备索引号已经被使用
#define		ERR_REF_TYPE_ID					0x00030001		//SetReference或GetReference传递的RefType不存在
#define		ERR_CREATE_SOCKET				0x00030002		//创建Socket时失败
#define		ERR_OPEN_CONNECT				0x00030003		//打开Socket连接时失败，可能设备连接已经存在
#define		ERR_NO_STARTUP					0x00030004		//设备没有启动
#define		ERR_NO_CONNECTED				0x00030005		//设备无连接
#define		ERR_SEND_PARTIAL				0x00030006		//只发送了部分CAN帧
#define		ERR_SEND_TOO_FAST				0x00030007		//数据发送太快 Socket缓冲区已满

