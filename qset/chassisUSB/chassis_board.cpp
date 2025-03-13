#include "chassis_board.h"

ChassisBoard::ChassisBoard() : sensors(*this), servos(*this), DrvMtr(*this)
{
    device_desc = new libusb_device_descriptor;
}

libusb_error ChassisBoard::initialize(libusb_log_level log_lvl)
{
    int err = 0;
    //Initialize LIBUSB context
    err = libusb_init(&ctx);
    if (err < LIBUSB_SUCCESS) 
        return (libusb_error)err;

    //Set debug log level
    err = libusb_set_option(ctx, LIBUSB_OPTION_LOG_LEVEL, log_lvl);
    if (err < LIBUSB_SUCCESS) 
        return (libusb_error)err;

    //Discover Devices
    ssize_t count = libusb_get_device_list(ctx, &list_of_devices);
    if (count < LIBUSB_SUCCESS) 
        return (libusb_error)err;
        
    //Find required device
    for (ssize_t i = 0; i < count; i++)
    {
        libusb_get_device_descriptor(list_of_devices[i], device_desc);
        if (B_PID == device_desc->idProduct && B_VID == device_desc->idVendor)
        {
            device = list_of_devices[i];
            break;
        }
    }

    //Verify if the board was found
    if (device == nullptr) 
        return (libusb_error)err;
    
    return LIBUSB_SUCCESS;
}

libusb_error ChassisBoard::claimInterfaces()
{
    int err = 0;
    //Handle Device
    err = libusb_open(device, &handle);
    libusb_free_device_list(list_of_devices, 1);
    list_of_devices = nullptr;
    if (err < LIBUSB_SUCCESS) 
        return (libusb_error)err;

    //Detaches kernel driver if applicable
    #ifndef CHASSIS_KDBYPASS_DRVMTR
    if (libusb_kernel_driver_active(handle, DRVM_DATA_INUM))
    {
        err = libusb_detach_kernel_driver(handle, DRVM_DATA_INUM);
        if (err < LIBUSB_SUCCESS) 
            return (libusb_error)err;
    }
    #endif
    #ifndef CHASSIS_KDBYPASS_SERVO
    if (libusb_kernel_driver_active(handle, SRVO_DATA_INUM))
    {
        err = libusb_detach_kernel_driver(handle, SRVO_DATA_INUM);
        if (err < LIBUSB_SUCCESS) 
            return (libusb_error)err;
    }
    #endif
    #ifndef CHASSIS_KDBYPASS_SENSOR
    if (libusb_kernel_driver_active(handle, SENS_DATA_INUM))
    {
        err = libusb_detach_kernel_driver(handle, SENS_DATA_INUM);
        if (err < LIBUSB_SUCCESS) 
            return (libusb_error)err;
    }
    #endif 

    //Claim the Interfaces
    err = libusb_claim_interface(handle, DRVM_DATA_INUM);
    if (err < LIBUSB_SUCCESS) 
        return (libusb_error)err;
    err = libusb_claim_interface(handle, SRVO_DATA_INUM);
    if (err < LIBUSB_SUCCESS) 
        return (libusb_error)err;
    err = libusb_claim_interface(handle, SENS_DATA_INUM);
    if (err < LIBUSB_SUCCESS) 
        return (libusb_error)err;

    return LIBUSB_SUCCESS;
}

int ChassisBoard::get_bytes_recv()
{
    return bytes_recv;
}

int ChassisBoard::get_bytes_sent()
{
    return bytes_sent;
}

ChassisBoard::~ChassisBoard()
{
    if (device_desc) { //This was allocated in the constructor
        delete device_desc;
        device_desc = nullptr;
    }
    libusb_release_interface(handle, DRVM_DATA_INUM);
    libusb_release_interface(handle, SRVO_DATA_INUM);
    libusb_release_interface(handle, SENS_DATA_INUM);
    libusb_free_device_list(list_of_devices, 1);
    libusb_close(handle);
    libusb_exit(ctx);
}

// libusb_error ChassisBoard::CBSensorInterface::write()
// {
//     err = libusb_bulk_transfer(chassis.handle, SENS_RXD_EP, (unsigned char *)&packet_SENSI, sizeof(packet_SENSI), &chassis.bytes_sent, timeout);
//     return (libusb_error)err;
// }

libusb_error ChassisBoard::CBSensorInterface::read()
{
    err = libusb_bulk_transfer(chassis.handle, SENS_TXD_EP, (unsigned char*)&packet_SENSO, sizeof(packet_SENSO), &chassis.bytes_recv, timeout);
    return (libusb_error)err;
}

libusb_error ChassisBoard::CBServoInterface::write()
{
    err = libusb_bulk_transfer(chassis.handle, SRVO_RXD_EP, (unsigned char *)&packet_SI, sizeof(packet_SI), &chassis.bytes_sent, timeout);
    return (libusb_error)err;
}

libusb_error ChassisBoard::CBServoInterface::read()
{
    err = libusb_bulk_transfer(chassis.handle, SRVO_TXD_EP, (unsigned char*)&packet_SO, sizeof(packet_SO), &chassis.bytes_recv, timeout);
    return (libusb_error)err;
}

libusb_error ChassisBoard::CBDriveMotorInterface::write()
{
    err = libusb_bulk_transfer(chassis.handle, DRVM_RXD_EP, (unsigned char *)&packet_MI, sizeof(packet_MI), &chassis.bytes_sent, timeout);    
    return (libusb_error)err;
}

libusb_error ChassisBoard::CBDriveMotorInterface::read()
{
    err = libusb_bulk_transfer(chassis.handle, DRVM_TXD_EP, (unsigned char*)&packet_MO, sizeof(packet_MO), &chassis.bytes_recv, timeout);
    return (libusb_error)err;
}

#ifndef CHASSIS_RMV_EZ_MODE

uint32_t ChassisBoard::CBSensorInterface::get_ADCVals(eChassisADC sensorID)
{
    if (sensorID == eN_DrvADC)
        return 0;
    return packet_SENSO.adc_vals[sensorID];
}

float ChassisBoard::CBSensorInterface::get_ADCVolts(eChassisADC sensorID)
{
    if (sensorID == eN_DrvADC)
        return 0;
    return packet_SENSO.adc_volts[sensorID];    
}

void ChassisBoard::CBServoInterface::set_ID(eChassisServo servoID)
{
    packet_SI.srvo_id = servoID;
}

void ChassisBoard::CBServoInterface::set_CTRL(uint32_t servoCTRL)
{
    packet_SI.srvo_ctrl = servoCTRL;
}

uint8_t ChassisBoard::CBServoInterface::get_Len()
{
    return packet_SO.len;
}

const uint8_t* ChassisBoard::CBServoInterface::get_Buf()
{
    return packet_SO.buf;
}

float ChassisBoard::CBServoInterface::get_Temp()
{
    return packet_SO.core_temp;
}

void ChassisBoard::CBDriveMotorInterface::set_ID(eDrvMotors mtrID)
{
    packet_MI.mtr_id = mtrID;
}

void ChassisBoard::CBDriveMotorInterface::set_LightCTRL(uint8_t bitPat)
{
    packet_MI.light_ctrl = bitPat;
}

void ChassisBoard::CBDriveMotorInterface::set_Pos(float position)
{
    packet_MI.mtr_ctrl.position = position;
}

void ChassisBoard::CBDriveMotorInterface::set_Vel(float velocity)
{
    packet_MI.mtr_ctrl.velocity = velocity;
}

void ChassisBoard::CBDriveMotorInterface::set_PIDctrl(float kP, float kI, float kD, float kF)
{
    packet_MI.mtr_ctrl.kP = kP;
    packet_MI.mtr_ctrl.kI = kI;
    packet_MI.mtr_ctrl.kD = kD;
    packet_MI.mtr_ctrl.kF = kF;
}

void ChassisBoard::CBDriveMotorInterface::set_ActiveStatus(bool toggle)
{
    packet_MI.mtr_ctrl.enable = toggle ? 0x1 : 0x0; 
}

uint8_t ChassisBoard::CBDriveMotorInterface::get_Status_Code()
{
    return packet_MO.status.code;
}

uint8_t ChassisBoard::CBDriveMotorInterface::get_Status_Val()
{
    return packet_MO.status.value;
}

uint8_t ChassisBoard::CBDriveMotorInterface::get_Info_Temp(eDrvMotors motorID)
{
    if (motorID == eN_DrvMotor)
        return 0;
    return packet_MO.mtr_info[motorID].temp;
}   

uint8_t ChassisBoard::CBDriveMotorInterface::get_Info_Current(eDrvMotors motorID)
{
    if (motorID == eN_DrvMotor)
        return 0;
    return packet_MO.mtr_info[motorID].current;
}

float ChassisBoard::CBDriveMotorInterface::get_Info_Pos(eDrvMotors motorID)
{
    if (motorID == eN_DrvMotor)
        return 0;
    return packet_MO.mtr_info[motorID].position;
}

float ChassisBoard::CBDriveMotorInterface::get_Info_Vel(eDrvMotors motorID)
{
    if (motorID == eN_DrvMotor)
        return 0;
    return packet_MO.mtr_info[motorID].velocity;
}
#endif

#ifdef CHASSIS_PRO_MODE

udev_pkt_sens_sts ChassisBoard::CBSensorInterface::get_pro()
{
    return packet_SENSO;
}

void ChassisBoard::CBServoInterface::set_pro(const udev_pkt_srvo_ctrl &send_packet)
{
    packet_SI = send_packet;
}

udev_pkt_srvo_sts ChassisBoard::CBServoInterface::get_pro()
{
    return packet_SO;
}

void ChassisBoard::CBDriveMotorInterface::set_pro(const udev_pkt_drvm_ctrl &send_packet)
{
    packet_MI = send_packet;
}

udev_pkt_drvm_sts ChassisBoard::CBDriveMotorInterface::get_pro()
{
    return packet_MO;
}

#endif