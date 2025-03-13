#ifndef CHASSIS_BOARD_H
#define CHASSIS_BOARD_H

#include "usb_packet.h"
#include <libusb-1.0/libusb.h>

/**
 * @file chassis_board.h
 * @author Kian Cossettini 
 * @brief QSET Chassis Board Communication
 * @version 0.1
 * @date 2025-03-12
 * 
 * @copyright Copyright (c) 2025
 *
 */


/**
 * @class ChassisBoard
 * @brief Provides interfaces to communicate with sensors, servos, and drive motors.
 *
 * The `ChassisBoard` class encapsulates three main hardware interfaces:
 * 
 * - Sensors (`CBSensor`): Used for reading sensor data.
 * 
 * - Servos (`CBServo`): Controls servo motors.
 * 
 * - Drive Motors (`CBDriveMotor`): Controls drive motors and LED strips.
 * 
 * 
 *
 * The class supports advanced configuration through preprocessor flags, which must be
 * defined before including this header file.
 *
 * ### Configuration Flags
 * 
 * The following `#define` flags can be used to customize the behavior of the class:
 * 
 * - `CHASSIS_RMV_EZ_MODE`: Removes individual packet setting and getting functions. Combine with `CHASSIS_PRO_MODE` for advanced usage.
 * 
 * - `CHASSIS_PRO_MODE`: Enables PRO MODE, allowing manual packet struct manipulation.
 * 
 * - `CHASSIS_KDBYPASS_DRVMTR`: Disables the DRVMTR kernel detaching check.
 * 
 * - `CHASSIS_KDBYPASS_SERVO`: Disables the SERVO kernel detaching check.
 * 
 * - `CHASSIS_KDBYPASS_SENSOR`: Disables the SENSOR kernel detaching check.
 * 
 *
 * ### PRO MODE
 * 
 * When `CHASSIS_PRO_MODE` is enabled, users can manually fill packet structs and pass
 * them to functions. This mode also allows retrieving data in the form of structs.
 *
 * @note Ensure the appropriate flags are defined before including this header to enable
 *       or disable specific features.
 *
 * @warning Incorrect use of configuration flags or manual packet manipulation in PRO MODE
 *          may result in undefined behavior.
 */
class ChassisBoard
{
    private:
        static constexpr uint16_t B_VID = 0xFFFE;
        static constexpr uint16_t B_PID = 0xD415;
        int bytes_recv = 0;
        int bytes_sent = 0;
        static constexpr int timeout = 100; //ms
        libusb_context *ctx = nullptr;
        libusb_device **list_of_devices = nullptr;
        libusb_device *device = nullptr; 
        libusb_device_descriptor *device_desc = nullptr; 
        libusb_device_handle *handle = nullptr;

        class CBSensorInterface
        {
            private:
                ChassisBoard& chassis;
                int err;
                //udev_pkt_sens_ctrl packet_SENSI; //Sensor in packet (useless)
                udev_pkt_sens_sts packet_SENSO; //Sensor out packet
            public:
                CBSensorInterface(ChassisBoard& chassis_ref) : chassis(chassis_ref) {}
                //libusb_error write(); //This is useless as the packet cannot send anything
                libusb_error read();
                #ifndef CHASSIS_RMV_EZ_MODE
                //Get packet EZ MODE
                uint32_t get_ADCVals(eChassisADC sensorID);
                float get_ADCVolts(eChassisADC sensorID);
                #endif
                #ifdef CHASSIS_PRO_MODE
                udev_pkt_sens_sts get_pro();
                #endif
        };
        class CBServoInterface
        {
            private:
                ChassisBoard& chassis;
                int err;
                udev_pkt_srvo_ctrl packet_SI; //Servo in packet
                udev_pkt_srvo_sts packet_SO; //Servo out packet
            public:
                CBServoInterface(ChassisBoard& chassis_ref) : chassis(chassis_ref) {}
                libusb_error write();
                libusb_error read();
                #ifndef CHASSIS_RMV_EZ_MODE
                //Set packet EZ MODE
                void set_ID(eChassisServo servoID);
                void set_CTRL(uint32_t servoCTRL);
                //Get packet EZ MODE
                uint8_t get_Len();
                const uint8_t* get_Buf();
                float get_Temp();
                #endif
                #ifdef CHASSIS_PRO_MODE
                void set_pro(const udev_pkt_srvo_ctrl &send_packet);
                udev_pkt_srvo_sts get_pro();
                #endif
        };
        class CBDriveMotorInterface
        {
            private:
                ChassisBoard& chassis;
                int err;
                udev_pkt_drvm_ctrl packet_MI; //Motor in packet
                udev_pkt_drvm_sts packet_MO; //Motor out packet
            public:
                CBDriveMotorInterface(ChassisBoard& chassis_ref) : chassis(chassis_ref) {}
                libusb_error write();
                libusb_error read();
                #ifndef CHASSIS_RMV_EZ_MODE
                //Set packet EZ MODE
                void set_ID(eDrvMotors mtrID);
                /**
                 * @brief Control for the LED strip.
                 *
                 * This function determines the state of the LED's on the strip. Multiple colors can be combined using
                 * bit operators.
                 *
                 * @param bitPat An 8-bit unsigned integer representing the motor control pattern.  
                 *               Each bit in the pattern corresponds to a specific motor or LED function.
                 *
                 * ## Bit Pattern Mapping:
                 * 
                 * - eDrvLight_R1 (0x01) → Red LED ON  
                 * 
                 * - eDrvLight_R0 (0x02) → Red LED OFF  
                 * 
                 * - eDrvLight_G1 (0x04) → Green LED ON  
                 * 
                 * - eDrvLight_G0 (0x08) → Green LED OFF  
                 * 
                 * - eDrvLight_B0 (0x10) → Blue LED OFF  
                 * 
                 * - eDrvLight_B1 (0x20) → Blue LED ON  
                 * 
                 *
                 * @warning Incorrect bit patterns may result in unexpected behavior.
                 */
                void set_LightCTRL(uint8_t bitPat);
                void set_Pos(float position);
                void set_Vel(float velocity);
                void set_PIDctrl(float kP, float kI, float kD, float kF);
                //Determines whether or not to activate the motor
                void set_ActiveStatus(bool toggle);
                //Get packet EZ MODE
                uint8_t get_Status_Code();
                uint8_t get_Status_Val();
                uint8_t get_Info_Temp(eDrvMotors motorID);
                uint8_t get_Info_Current(eDrvMotors motorID);
                float get_Info_Pos(eDrvMotors motorID);
                float get_Info_Vel(eDrvMotors motorID);
                #endif
                #ifdef CHASSIS_PRO_MODE
                void set_pro(const udev_pkt_drvm_ctrl &send_packet);
                udev_pkt_drvm_sts get_pro();
                #endif
        };

    public:
        CBSensorInterface sensors;
        CBServoInterface servos;
        CBDriveMotorInterface DrvMtr;
        ChassisBoard();
        /**
         * @brief Initializes the libusb context and detects the chassis board.
         *
         * This function sets up the libusb library, initializes the USB context,  
         * and attempts to locate the chassis board.  
         * The function should be called before any USB communication occurs (along with `claimInterfaces()`).
         *
         * @param log_lvl The desired libusb logging level (`LIBUSB_LOG_LEVEL_NONE`, 
         * `LIBUSB_LOG_LEVEL_ERROR`, `LIBUSB_LOG_LEVEL_WARNING`, `LIBUSB_LOG_LEVEL_INFO`,  
         * or `LIBUSB_LOG_LEVEL_DEBUG`).
         *
         * @return `LIBUSB_SUCCESS` if initialization is successful.  
         *         Otherwise, returns a libusb error code (e.g., `LIBUSB_ERROR_NO_DEVICE`  
         *         if the board is not found, or `LIBUSB_ERROR_OTHER` for general failures).
         *
         * @note If initialization fails, USB communication with the board will not be possible.
         *       Ensure libusb is properly installed and that the board is connected.
         * 
         */
        libusb_error initialize(libusb_log_level log_lvl);

        /**
         * @brief Claims USB interfaces for communication with the chassis board.
         *
         * This function claims the necessary USB interfaces for device communication.  
         * If the corresponding preprocessor flag (`CHASSIS_KDBYPASS_*`) is not defined,  
         * it will attempt to detach the kernel driver before claiming the interface.
         * This step is required before performing any USB communication with the board.
         *
         * @return `LIBUSB_SUCCESS` on success.  
         *         Otherwise, returns a libusb error code.
         *
         * @note If kernel driver detachment is disabled via `CHASSIS_KDBYPASS_*` flags,  
         *       ensure that no other process is using the USB device.
         * 
         */
        libusb_error claimInterfaces();
        //Returns the number of bytes received after a write operation.
        int get_bytes_recv();
        //Returns the number of bytes send after a read operation
        int get_bytes_sent();
        ~ChassisBoard();
};

#endif