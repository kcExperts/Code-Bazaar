/**
 * @file usb_packet.h
 * @author Jacob Chisholm (Jchisholm204.github.io)
 * @brief USB Packet Declarations
 * @version 0.1
 * @date 2025-02-14
 * 
 * @copyright Copyright (c) 2023
 *
 *
 * This file is used by the driver and the embedded device
 * Both must be recompiled/flashed when changed
 *
 */

 #ifndef _USB_PACKET_H_
 #define _USB_PACKET_H_
 
 #include <stdint.h>
 #include <assert.h>
 #include "usb_chassis_defs.h"
 #include "usb_dev.h"
 
 struct udev_status {
     // Status code given by the enum
     // enum eDrvStatus code;
     uint8_t code;
     // Extra information could be:
     //  - Error code from the motor
     //  - number of the motor that failed initialization
     uint8_t value;
 };
 
 // Motor Control Structure (Same as ARM)
 struct udev_mtr_ctrl {
     // Motor Position Command
     float position;
     // Motor Velocity Command
     float velocity;
     // Motor Configuration Data
     float kP, kI, kD, kF;
     // Enable this Motor
     uint8_t enable;
 } __attribute__((packed));
 
 // Motor Control Info Structure (Same as ARM)
 struct udev_mtr_info {
     // Motor Temperature (in C)
     uint8_t temp;
     // Motor Current in A/10
     uint8_t current;
     // Motor Position
     float position;
     // Motor Velocity
     float velocity;
 } __attribute__((packed));
 
 
 // Drive Motor + Auto Lights Packet:
 //  From Host to Device
 //  Must be less than 0x40 in length
 struct udev_pkt_drvm_ctrl {
     uint8_t mtr_id;
     // Lower three bits are RGB
     uint8_t light_ctrl;
     struct udev_mtr_ctrl mtr_ctrl;
 } __attribute__((packed));
 
 // Drive Motor + Auto Lights Packet:
 //  From Device to Host
 //  Must be less than 0x40 in length
 struct udev_pkt_drvm_sts {
     // Device Status
     struct udev_status   status;
     struct udev_mtr_info mtr_info[eN_DrvMotor];
 } __attribute__((packed));
 
 
 // Servo Interface Packet:
 //  From Host to Device
 //  Must be less than 0x40 in length
 struct udev_pkt_srvo_ctrl {
     uint8_t  srvo_id;
     uint32_t srvo_ctrl;
 } __attribute__((packed));
 
 // Servo Interface Packet:
 //  From Device to Host
 //  Must be less than 0x40 in length
 struct udev_pkt_srvo_sts {
     uint8_t len;
     uint8_t buf[0x20];
     float   core_temp;
 } __attribute__((packed));
 
 struct udev_pkt_sens_ctrl {
     // Sensors have no controllable values
 }  __attribute__((packed));
 
 struct udev_pkt_sens_sts {
     uint32_t  adc_vals[eN_DrvADC];
     float     adc_volts[eN_DrvADC];
 }  __attribute__((packed));
 
 
 // USB Packets must be less than 0x40/64 bytes in length
 static_assert(sizeof(struct udev_pkt_drvm_ctrl) <= DRVM_DATA_SZ, "USBD DRVM Packet Oversize");
 static_assert(sizeof(struct udev_pkt_drvm_sts) <= DRVM_DATA_SZ, "USBD DRVM Packet Oversize");
 static_assert(sizeof(struct udev_pkt_srvo_ctrl) <= SRVO_DATA_SZ, "USBD SRVO Packet Oversize");
 static_assert(sizeof(struct udev_pkt_srvo_sts) <= SRVO_DATA_SZ, "USBD SRVO Packet Oversize");
 static_assert(sizeof(struct udev_pkt_sens_ctrl) <= SENS_DATA_SZ, "USBD SENS Packet Oversize");
 static_assert(sizeof(struct udev_pkt_sens_sts) <= SENS_DATA_SZ, "USBD SENS Packet Oversize");
 
 #endif
