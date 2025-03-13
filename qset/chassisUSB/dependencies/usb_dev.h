/**
 * @file usb_dev.h
 * @author Jacob Chisholm (jchisholm204.github.io)
 * @brief QSET Drive Control USB Device Definitions
 * @version 0.1
 * @date 2025-02-14
 * 
 * @copyright Copyright (c) 2023
 * 
 * USB Device Definitions:
 *  - Endpoint Definitions
 *  - USB Device IDs
 *
 * This file is used by the driver and the embedded device
 * Both must be recompiled/flashed when changed
 *
 */

 #ifndef _USB_DEV_H_
 #define _USB_DEV_H_
 
 #ifndef UDEV_VERSION
 #define UDEV_VERSION "1.0.0"
 #endif
 
 #define UDEV_INTERFACES  0x06
 
 // Drive Interface
 #define DRVM_RXD_EP      0x01
 #define DRVM_TXD_EP      0x81
 #define DRVM_DATA_SZ     0x40
 #define DRVM_NTF_EP      0x82
 #define DRVM_NTF_SZ      0x08
 #define DRVM_NTF_INUM    0x00
 #define DRVM_DATA_INUM   0x01
 
 // Servo Interface
 #define SRVO_RXD_EP      0x03
 #define SRVO_TXD_EP      0x83
 #define SRVO_DATA_SZ     0x40
 #define SRVO_NTF_EP      0x84
 #define SRVO_NTF_SZ      0x08
 #define SRVO_NTF_INUM    0x02
 #define SRVO_DATA_INUM   0x03
 
 // ADC + Sensor Data Interface
 #define SENS_RXD_EP      0x03
 #define SENS_TXD_EP      0x83
 #define SENS_DATA_SZ     0x40
 #define SENS_NTF_EP      0x84
 #define SENS_NTF_SZ      0x08
 #define SENS_NTF_INUM    0x02
 #define SENS_DATA_INUM   0x03
 
 // USB Device Vendor ID:
 //  Use 0xFFFF or 0xFFFE as designated by the USBIF,
 //  These vendor ID's are reserved for test or hobby devices and
 //  will not conflict with registered vendor drivers
 #define VENDOR_ID 0xFFFE
 // USB Device Product ID:
 //  Used to seperate usb devices from the same vendor
 //  Must be different for each type of device
 #define DEVICE_ID 0xD415
 
 #endif  // INCLUDE_INCLUDE_USB_DEV_H_