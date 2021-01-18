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
        deviceDescription_ = portInfo_.description();
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

    device_.setBaudRate(config.baudrate_);
    device_.setPortName(config.portName_);
    device_.setDataBits(QSerialPort::Data8);
    device_.setFlowControl(QSerialPort::NoFlowControl);
    device_.setParity(QSerialPort::NoParity);
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
    unsigned char buffer[255];
    uint32_t crc32 = 0;
    //int returns = 0;


    if(connectionStatus_)
    {

        /*Get BL version*/
        buffer[0] = getVersionCommandLength - 1;
        buffer[1] = COMMAND_BL_GET_VER;
        crc32 = getCRC(buffer, getVersionCommandLength - 4);
        buffer[2] = wordToByte(crc32, 1, 1);
        buffer[3] = wordToByte(crc32, 2, 1);
        buffer[4] = wordToByte(crc32, 3, 1);
        buffer[5] = wordToByte(crc32, 4, 1);
        device_.write("5");
        device_.write("0x51");
        device_.write("%d", crc32);
        QString data = device_.readAll();
        qDebug() << data;
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

int Device::readReply(uint8_t commandCode)
{
    QByteArray ack; //MCU sends ack + len field
    ack.resize(2);
    uint32_t lengthToFollow = 0;
    int returns = -2;

    ack = device_.readAll();
    qDebug() << ack;
//    if(ack[0] == 0xA5)
//    {
//        //CRC of last command was good .. received ACK and "len to follow"
//        len_to_follow=ack[1];
//        printf("\n\n   CRC : SUCCESS Len : %d\n",len_to_follow);

//        switch(0x50 | command_code)
//        {
//        case COMMAND_BL_GET_VER:
//             process_COMMAND_BL_GET_VER(len_to_follow);
//            break;
//        case COMMAND_BL_GET_HELP:
//            process_COMMAND_BL_GET_HELP(len_to_follow);
//            break;
//        case COMMAND_BL_GET_CID:
//            process_COMMAND_BL_GET_CID(len_to_follow);
//            break;
//        case COMMAND_BL_GET_RDP_STATUS:
//            process_COMMAND_BL_GET_RDP_STATUS(len_to_follow);
//            break;
//        case COMMAND_BL_GO_TO_ADDR:
//            process_COMMAND_BL_GO_TO_ADDR(len_to_follow);
//            break;
//        case COMMAND_BL_FLASH_ERASE:
//            process_COMMAND_BL_FLASH_ERASE(len_to_follow);
//            break;
//        case COMMAND_BL_MEM_WRITE:
//            process_COMMAND_BL_MEM_WRITE(len_to_follow);
//            break;
//        case COMMAND_BL_READ_SECTOR_P_STATUS:
//            process_COMMAND_BL_READ_SECTOR_STATUS(len_to_follow);
//            break;
//        case COMMAND_BL_EN_R_W_PROTECT:
//            process_COMMAND_BL_EN_R_W_PROTECT(len_to_follow);
//            break;
//        case COMMAND_BL_DIS_R_W_PROTECT:
//            process_COMMAND_BL_DIS_R_W_PROTECT(len_to_follow);
//            break;
//        case COMMAND_BL_MY_NEW_COMMAND:
//            process_COMMAND_BL_MY_NEW_COMMAND(len_to_follow);
//        default:
//            printf("\n  Invalid command code\n");

//        }

//          ret = 0;
//    }
//    else if( ack[0] == 0x7F)
//    {
//
//        printf("\n   CRC: FAIL \n");
//        ret= -1;
//    }

    return returns;
}
