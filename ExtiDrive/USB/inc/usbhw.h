/*----------------------------------------------------------------------------
 * U S B  -  K e r n e l
 *----------------------------------------------------------------------------
 * Name:    usbhw.h
 * Purpose: USB Hardware Layer Definitions
 * Version: V1.20
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2009 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------
 * History:
 *          V1.20 Added USB_ClearEPBuf 
 *          V1.00 Initial Version
 *----------------------------------------------------------------------------*/

#ifndef __USBHW_H__
#define __USBHW_H__
#include "type.h"

/* USB RAM Definitions */
#define USB_RAM_ADR     0x7FD00000  /* USB RAM Start Address */
#define USB_RAM_SZ      0x00002000  /* USB RAM Size (8kB) */

/* DMA Endpoint Descriptors */
#define DD_NISO_CNT             16  /* Non-Iso EP DMA Descr. Count (max. 32) */
#define DD_ISO_CNT               8  /* Iso EP DMA Descriptor Count (max. 32) */
#define DD_NISO_SZ    (DD_NISO_CNT * 16)    /* Non-Iso DMA Descr. Size */
#define DD_ISO_SZ     (DD_ISO_CNT  * 20)    /* Iso DMA Descriptor Size */
#define DD_NISO_ADR   (USB_RAM_ADR + 128)   /* Non-Iso DMA Descr. Address */
#define DD_ISO_ADR    (DD_NISO_ADR + DD_NISO_SZ) /* Iso DMA Descr. Address */
#define DD_SZ                 (128 + DD_NISO_SZ + DD_ISO_SZ) /* Descr. Size */

/* DMA Buffer Memory Definitions */
#define DMA_BUF_ADR   (USB_RAM_ADR + DD_SZ) /* DMA Buffer Start Address */
#define DMA_BUF_SZ    (USB_RAM_SZ  - DD_SZ) /* DMA Buffer Size */

/* USB Error Codes */
#define USB_ERR_PID         0x0001  /* PID Error */
#define USB_ERR_UEPKT       0x0002  /* Unexpected Packet */
#define USB_ERR_DCRC        0x0004  /* Data CRC Error */
#define USB_ERR_TIMOUT      0x0008  /* Bus Time-out Error */
#define USB_ERR_EOP         0x0010  /* End of Packet Error */
#define USB_ERR_B_OVRN      0x0020  /* Buffer Overrun */
#define USB_ERR_BTSTF       0x0040  /* Bit Stuff Error */
#define USB_ERR_TGL         0x0080  /* Toggle Bit Error */

/* USB DMA Status Codes */
#define USB_DMA_INVALID     0x0000  /* DMA Invalid - Not Configured */
#define USB_DMA_IDLE        0x0001  /* DMA Idle - Waiting for Trigger */
#define USB_DMA_BUSY        0x0002  /* DMA Busy - Transfer in progress */
#define USB_DMA_DONE        0x0003  /* DMA Transfer Done (no Errors)*/
#define USB_DMA_OVER_RUN    0x0004  /* Data Over Run */
#define USB_DMA_UNDER_RUN   0x0005  /* Data Under Run (Short Packet) */
#define USB_DMA_ERROR       0x0006  /* Error */
#define USB_DMA_UNKNOWN     0xFFFF  /* Unknown State */

/* USB DMA Descriptor */
typedef struct _USB_DMA_DESCRIPTOR {
  unsigned int BufAdr;                       /* DMA Buffer Address */
  unsigned short BufLen;                       /* DMA Buffer Length */
  unsigned short MaxSize;                      /* Maximum Packet Size */
  unsigned int InfoAdr;                      /* Packet Info Memory Address */
  union {                           /* DMA Configuration */
    struct {
      unsigned int Link   : 1;               /* Link to existing Descriptors */
      unsigned int IsoEP  : 1;               /* Isonchronous Endpoint */
      unsigned int ATLE   : 1;               /* ATLE (Auto Transfer Length Extract) */
      unsigned int Rsrvd  : 5;               /* Reserved */
      unsigned int LenPos : 8;               /* Length Position (ATLE) */
    } Type;
    unsigned int Val;
  } Cfg;
} USB_DMA_DESCRIPTOR;

/* USB Hardware Functions */
extern void USB_Init        (void);
extern void USB_Connect     (unsigned int con);
extern void USB_Reset       (void);
extern void USB_Suspend     (void);
extern void USB_Resume      (void);
extern void USB_WakeUp      (void);
extern void USB_WakeUpCfg   (unsigned int cfg);
extern void USB_SetAddress  (unsigned int  adr);
extern void USB_Configure   (unsigned int cfg);
extern void USB_ConfigEP    (USB_ENDPOINT_DESCRIPTOR *pEPD);
extern void USB_DirCtrlEP   (unsigned int  dir);
extern void USB_EnableEP    (unsigned int  EPNum);
extern void USB_DisableEP   (unsigned int  EPNum);
extern void USB_ResetEP     (unsigned int  EPNum);
extern void USB_SetStallEP  (unsigned int  EPNum);
extern void USB_ClrStallEP  (unsigned int  EPNum);
extern void USB_ClearEPBuf  (unsigned int  EPNum);
extern unsigned int  USB_ReadEP      (unsigned int  EPNum, unsigned char *pData);
extern unsigned int  USB_WriteEP     (unsigned int  EPNum, unsigned char *pData, unsigned int cnt);
extern unsigned int USB_DMA_Setup   (unsigned int  EPNum, USB_DMA_DESCRIPTOR *pDD);
extern void USB_DMA_Enable  (unsigned int  EPNum);
extern void USB_DMA_Disable (unsigned int  EPNum);
extern unsigned int  USB_DMA_Status  (unsigned int  EPNum);
extern unsigned int  USB_DMA_BufAdr  (unsigned int  EPNum);
extern unsigned int  USB_DMA_BufCnt  (unsigned int  EPNum);
extern unsigned int  USB_GetFrame    (void);


#endif  /* __USBHW_H__ */
