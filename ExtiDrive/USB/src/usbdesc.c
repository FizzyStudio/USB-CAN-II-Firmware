/*----------------------------------------------------------------------------
 * U S B  -  K e r n e l
 *----------------------------------------------------------------------------
 * Name:    usbdesc.c
 * Purpose: USB Descriptors
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
 *          V1.20 Changed string descriptor handling
 *                Changed HID Report Descriptor
 *          V1.00 Initial Version
 *----------------------------------------------------------------------------*/

#include "type.h"

#include "usb.h"
#include "usbcfg.h"
#include "usbdesc.h"

#include "vendor.h"
#include "hid.h"

/* USB Standard Device Descriptor */
const U8 USB_DeviceDescriptor[] = {
  USB_DEVICE_DESC_SIZE,              /* bLength */
  USB_DEVICE_DESCRIPTOR_TYPE,        /* bDescriptorType */
  WBVAL(0x0200), /* 2.00 */          /* bcdUSB */
  USB_DEVICE_CLASS_VENDOR_SPECIFIC,  /* bDeviceClass 厂商自定义设备*/
  VENDOR_SUBCLASS_NONE,              /* bDeviceSubClass */
  VENDOR_PROTOCOL_NONE,              /* bDeviceProtocol */
  USB_MAX_PACKET0,                   /* bMaxPacketSize0 控制端点0的最大传输长度*/
  WBVAL(0x0319),                     /* idVendor 厂商ID*/
  WBVAL(0x0ACE),                     /* idProduct 产品ID*/
  WBVAL(0x0100), /* 1.00 */          /* bcdDevice 产品版本*/
  0x01,                              /* iManufacturer */
  0x02,                              /* iProduct */
  0x03,                              /* iSerialNumber */
  0x01                               /* bNumConfigurations: one possible configuration*/
};

/* USB Configuration Descriptor */
/*   All Descriptors (Configuration, Interface, Endpoint, Class, Vendor) */
const U8 USB_ConfigDescriptor[] = {
/* Configuration 1 */
  USB_CONFIGUARTION_DESC_SIZE,       /* bLength */
  USB_CONFIGURATION_DESCRIPTOR_TYPE, /* bDescriptorType */
  WBVAL(                             /* wTotalLength */
    USB_CONFIGUARTION_DESC_SIZE +
    USB_INTERFACE_DESC_SIZE     +
    HID_DESC_SIZE               +
    USB_ENDPOINT_DESC_SIZE
  ),
  0x01,                              /* bNumInterfaces 总接口数*/
  0x01,                              /* bConfigurationValue: 0x01 is used to select this configuration 配置描述符值*/
  0x00,                              /* iConfiguration: no string to describe this configuration */
  USB_CONFIG_BUS_POWERED /*|*/       /* bmAttributes 总线供电*/
/*USB_CONFIG_REMOTE_WAKEUP*/,
  USB_CONFIG_POWER_MA(100),          /* bMaxPower, device power consumption is 100 mA */

/* Interface 0, Alternate Setting 0, HID Class */
  USB_INTERFACE_DESC_SIZE,           /* bLength */
  USB_INTERFACE_DESCRIPTOR_TYPE,     /* bDescriptorType */
  0x00,                              /* bInterfaceNumber 接口号为0*/
  0x00,                              /* bAlternateSetting 接口可选值*/
  0x03,                              /* bNumEndpoints 包含3个端点*/
  USB_DEVICE_CLASS_VENDOR_SPECIFIC,  /* bInterfaceClass 接口类型*/
  VENDOR_SUBCLASS_NONE,              /* bInterfaceSubClass */
  VENDOR_PROTOCOL_NONE,              /* bInterfaceProtocol */
  0x04,                              /* iInterface 接口描述符*/

/* Endpoint, EP1 Interrupt In */
  USB_ENDPOINT_DESC_SIZE,            /* bLength */
  USB_ENDPOINT_DESCRIPTOR_TYPE,      /* bDescriptorType */
  USB_ENDPOINT_IN(1),                /* bEndpointAddress 端点地址为0x01，输入端点*/
  USB_ENDPOINT_TYPE_INTERRUPT,       /* bmAttributes 端点类型，中断端点*/
  WBVAL(0x0004),                     /* wMaxPacketSize 最大包长度*/
  0x01,                 						 /* bInterval 中断端点的巡检间隔，1ms*/
	
/* Endpoint, EP2 Bulk IN */
  USB_ENDPOINT_DESC_SIZE,            /* bLength */
  USB_ENDPOINT_DESCRIPTOR_TYPE,      /* bDescriptorType */
  USB_ENDPOINT_IN(2),                /* bEndpointAddress */
  USB_ENDPOINT_TYPE_BULK,            /* bmAttributes */
  WBVAL(0x0040),                     /* wMaxPacketSize */
  0x00,                              /* bInterval: ignore for Bulk transfer */

/* Endpoint, EP2 Bulk OUT */
  USB_ENDPOINT_DESC_SIZE,            /* bLength */
  USB_ENDPOINT_DESCRIPTOR_TYPE,      /* bDescriptorType */
  USB_ENDPOINT_OUT(2),               /* bEndpointAddress */
  USB_ENDPOINT_TYPE_BULK,            /* bmAttributes */
  WBVAL(0x0040),                     /* wMaxPacketSize */
  0x00,                              /* bInterval: ignore for Bulk transfer */

/* Terminator */
  0                                  /* bLength */
};


/* USB String Descriptor (optional) */
const U8 USB_StringDescriptor[] = {
/* Index 0x00: LANGID Codes */
  0x04,                              /* bLength */
  USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
  WBVAL(0x0409), /* US English */    /* wLANGID */
/* Index 0x01: Manufacturer */
  (7*2 + 2),                        /* bLength (13 Char + Type + lenght) */
  USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
  'J',0,
  'X',0,
  ' ',0,
  'T',0,
  'e',0,
  'c',0,
  'h',0,
/* Index 0x02: Product */
  (10*2 + 2),                        /* bLength (16 Char + Type + lenght) */
  USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
  'U',0,
  'S',0,
  'B',0,
  ' ',0,
  'C',0,
  'A',0,
  'N',0,
  ' ',0,
  'I',0,
  'I',0,
/* Index 0x03: Serial Number */
  (12*2 + 2),                        /* bLength (12 Char + Type + lenght) */
  USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
  '0',0,                             /* allowed characters are       */
  '0',0,                             /*   0x0030 - 0x0039 ('0'..'9') */
  '0',0,                             /*   0x0041 - 0x0046 ('A'..'F') */
  '1',0,                             /*   length >= 26               */
  'A',0,
  '0',0,
  '0',0,
  '0',0,
  '0',0,
  '0',0,
  '0',0,
  '0',0,
/* Index 0x04: Interface 0, Alternate Setting 0 */
  (10*2 + 2),                        /* bLength (3 Char + Type + lenght) */
  USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
  'U',0,
  'S',0,
  'B',0,
  ' ',0,
  'C',0,
  'A',0,
  'N',0,
  ' ',0,
  'I',0,
  'I',0,
};
