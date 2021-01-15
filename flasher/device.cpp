#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QString>
#include <stdint.h>
#include <QDebug>
#include "config.h"
#include "flasher.h"
#include "device.h"

Device::Device()
{

}

QString Device::getDescription(void)
{
    QString error_ = "Device not connected!";

    if(connectionStatus_)
    {
        return deviceDescription_;
    }

    return error_;
}

portConfig Device::getConfig(void)
{
    return config_;
}

void Device::setupDevice(portConfig config)
{
    config_.baudrate_ = config.baudrate_;
    config_.portName_ = config.portName_;

    device_.setBaudRate(config_.baudrate_);
    device_.setPortName(config_.portName_);
    device_.setDataBits(QSerialPort::Data8);
    device_.setFlowControl(QSerialPort::NoFlowControl);
    device_.setParity(QSerialPort::NoParity);
    device_.setPort(portInfo_);
    device_.setStopBits(QSerialPort::OneStop);
}

void Device::connectDevice(void)
{
    connectionStatus_ = true;
    device_.open(QIODevice::ReadWrite);
}

void Device::disconnectDevice(void)
{
    connectionStatus_ = false;
    device_.close();
}

void Device::sendData(uint8_t* pData, qint64 len)
{
    if(connectionStatus_)
    {
        device_.write((char*) pData, len);
    }
}

QString Device::recieveData(void)
{
    QString data = "Device not connected!";

    if(connectionStatus_)
    {
        data = device_.readAll();
    }

    return data;
}

QString Device::getBLVer(void)
{
    QString bootloaderVer_ = "Device not connected!";
    float version_ = 0;

    if(connectionStatus_)
    {
        /*Get BL version*/

        /*--------------*/
        bootloaderVer_ = "V" + QString::number(version_);
    }

    return bootloaderVer_;
}

QString Device::getBLHelp(void)
{
    if(connectionStatus_)
    {
        /*Get Help*/

        /*--------*/
    }
}

QString Device::getChipID(void)
{
    uint16_t chipID = 0;
    QString result_;

    if(connectionStatus_)
    {
        /*Get Chip ID */

        /*------------*/
        result_ = "Chip ID : " + QString::number(chipID);
    }

    return result_;
}

QString Device::getRDPLevel(void)
{
    rdpLevel level_ = levelOne;
    QString result_;

    if(connectionStatus_)
    {
        /*Get RDP */

        /*--------*/
        switch(level_)
        {
        case levelZero:
            result_ = "RDP level is set to Level-0. No memory protection.";
            break;
        case levelOne:
            result_ = "RDP level is set to Level-1. Memorry readout protection.";
            break;
        case levelTwo:
            result_ = "RDP level is set to Level-2. Chip readout protection.";
            break;
        default:
            result_ = "There was some error in RDP status!";
            break;
        }
    }

    return result_;
}

HAL_FlashStatusTypeDef Device::eraseFlash(void)
{
    HAL_FlashStatusTypeDef status_ = Flash_HAL_OK;

    if(connectionStatus_)
    {
        /*Erase flash */

        /*------------*/
        return status_;
    }
    return Error;
}

HAL_FlashStatusTypeDef Device::flashBin(void)
{
    HAL_FlashStatusTypeDef status_ = Flash_HAL_OK;

    if(connectionStatus_)
    {
        /*Flash binary*/

        /*------------*/
        return status_;
    }

    return Error;
}

void Device::setRDPLevel(rdpLevel level)
{
    if(connectionStatus_)
    {
        /*Set RDP*/

        /*-------*/
    }
}

bool Device::isConnected(void)
{
    return connectionStatus_;
}

bool Device::checkConnection(QString portName)
{
    bool result_ = true;
    if(portName.isEmpty())
    {
        result_ = false;
        return result_;
    }
    if(portInfo_.availablePorts().size() == 0)
    {
        result_ = false;
        return result_;
    }
    if(portName != portInfo_.availablePorts().at(0).portName())
    {
        result_ = false;
        qDebug() << "Available ports " << portInfo_.availablePorts().size();
        qDebug() << "Port name " <<  portInfo_.availablePorts().at(0).portName();
        return result_;
    }
    return result_;
}

uint32_t Device::getCRC(uint8_t* buf, uint32_t len)
{
    uint32_t iter;
    uint32_t crc = 0XFFFFFFFF;

    for(uint32_t n = 0 ; n < len ; n++ )
    {
        uint32_t data = buf[n];

        crc = crc ^ data;
        for(iter=0; iter<32; iter++)
        {

        if (crc & 0x80000000)
            crc = (crc << 1) ^ 0x04C11DB7; // Polynomial used in STM32
        else
            crc = (crc << 1);
        }
    }
  return(crc);
}

uint8_t Device::wordToByte(uint32_t addr, uint8_t index, uint8_t lowerFirst)
{
    uint8_t value = (addr >> ( 8 * ( index -1)) & 0x000000FF );
    Q_UNUSED(lowerFirst);
    return value;
}
