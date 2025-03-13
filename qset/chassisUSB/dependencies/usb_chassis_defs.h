/**
 * @file usb_drv_defs.h
 * @author Jacob Chisholm (Jchisholm204.github.io)
 * @brief QSET Drive Interface Definitions
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
#ifndef _USB_DRV_DEFS_H_
#define _USB_DRV_DEFS_H_

enum eDrvMotors {
    // Front Left Drive Motor
    eDrvMtrFL,
    // Front Right Drive Motor
    eDrvMtrFR,
    // Back Left Drive Motor
    eDrvMtrBL,
    // Back Right Drive Motor
    eDrvMtrBR,
    // Entry to declare the number of drive motors
    eN_DrvMotor
};

enum eChassisServo {
    eServo1,
    eServo2,
    eServo3,
    eServo4,
    eServo5,
    eServo6,
    eServo7,
    eServo8,
    eN_Servo
};
 
// Bit mappings for the LED Outputs
enum eDrvLights {
    // eDrvLight_R  = 0x01,
    // eDrvLight_G  = 0x02,
    // eDrvLight_B  = 0x04,
    // eDrvLight_ON = 0x07,
    // eDrvLight_OF = 0x00,

    eDrvLight_R1 = 0x01,
    eDrvLight_R0 = 0x02,
    eDrvLight_G1 = 0x04,
    eDrvLight_G0 = 0x08,
    eDrvLight_B0 = 0x10,
    eDrvLight_B1 = 0x20
};

enum eChassisADC{
    eDrvADC1,
    eDrvADC2,
    eDrvADC3,
    eDrvADC4,
    eDrvADC_Temp,
    eN_DrvADC
};

enum eDrvStatus {
    eDrvOK,
    // Board is still waiting for one or more initialization message
    eDrvUnInit,
    // Initialization failed on an interface
    eDrvIniFail,
    // A bus or task has stalled triggering the watchdog (will self resolve in time)
    eDrvStall,
    // A motor has failed to respond in a timely manner
    eDrvMtrFail,
};

#endif