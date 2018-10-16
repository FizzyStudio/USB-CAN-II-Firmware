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

/*****************************************CAN é”™è¯¯ç ********************************************************/
#define		ERR_CAN_OVERFLOW		0x00000001		//CAN æ§åˆ¶å™¨å†…éƒ¨ FIFOæº¢å‡º
#define		ERR_CAN_ERRALARM		0x00000002		//CAN æ§åˆ¶å™¨é”™è¯¯æŠ¥è­¦
#define		ERR_CAN_PASSIVE			0x00000004		//CAN æ§åˆ¶å™¨æ¶ˆæé”™è¯¯
#define		ERR_CAN_LOSE			0x00000008		//CAN æ§åˆ¶å™¨ä»²è£ä¸¢å¤±
#define		ERR_CAN_BUSERR			0x00000010		//CAN æ§åˆ¶å™¨æ€»çº¿é”™è¯¯
#define		ERR_CAN_BUSOFF			0x00000020		//CAN æ§åˆ¶å™¨æ€»çº¿å…³é—­

/*****************************************Í¨é€šç”¨é”™è¯¯ç *****************************************************/
#define		ERR_DEVICEOPENED		0x00000100		//Éè±¸ÒÑ´ò¿ª
#define		ERR_DEVICEOPEN			0x00000200		//´ò¿ªÉè±¸´íÎó
#define		ERR_DEVICENOTOPEN		0x00000400		//Éè±¸Ã»ÓĞ´ò¿ª
#define		ERR_BUFFEROVERFLOW	0x00000800		//»º³åÇøÒç³ö
#define		ERR_DEVICENOTEXIST	0x00001000		//´ËÉè±¸²»´æÔÚ
#define		ERR_LOADKERNELDLL		0x00002000		//×°ÔØ¶¯Ì¬¿âÊ§°Ü
#define		ERR_CMDFAILED				0x00004000		//Ö´ĞĞÃüÁîÊ§°Ü´íÎóÂë
#define		ERR_BUFFERCREATE		0x00008000		//ÄÚ´æ²»×ã

/****************************************CANET´íÎóÂë*****************************************************/
#define		ERR_CANETE_PORTOPENED		0x00010000		//¶Ë¿ÚÒÑ¾­±»´ò¿ª
#define		ERR_CANETE_INDEXUSED		0x00020000		//Éè±¸Ë÷ÒıºÅÒÑ¾­±»Ê¹ÓÃ
#define		ERR_REF_TYPE_ID					0x00030001		//SetReference»òGetReference´«µİµÄRefType²»´æÔÚ
#define		ERR_CREATE_SOCKET				0x00030002		//´´½¨SocketÊ±Ê§°Ü
#define		ERR_OPEN_CONNECT				0x00030003		//´ò¿ªSocketÁ¬½ÓÊ±Ê§°Ü£¬¿ÉÄÜÉè±¸Á¬½ÓÒÑ¾­´æÔÚ
#define		ERR_NO_STARTUP					0x00030004		//Éè±¸Ã»ÓĞÆô¶¯
#define		ERR_NO_CONNECTED				0x00030005		//Éè±¸ÎŞÁ¬½Ó
#define		ERR_SEND_PARTIAL				0x00030006		//Ö»·¢ËÍÁË²¿·ÖCANÖ¡
#define		ERR_SEND_TOO_FAST				0x00030007		//Êı¾İ·¢ËÍÌ«¿ì Socket»º³åÇøÒÑÂú

#endif 