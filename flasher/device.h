#ifndef DEVICE_H
#define DEVICE_H

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <stdint.h>
#include "config.h"

typedef struct
{
    qint32 baudrate_;
    QString portName_;
}portConfig;

class Device:public QSerialPort
{
public:
    QString getDescription(void);
    portConfig getConfig(void);
    void setupDevice(portConfig config);
    void connectDevice(void);
    void disconnectDevice(void);
    void sendData(uint8_t* pData, qint64 len);
    QString recieveData(void);
    QString getBLVer(void);
    QString getBLHelp(void);
    QString getChipID(void);
    QString getRDPLevel(void);
    HAL_FlashStatusTypeDef eraseFlash(void);
    HAL_FlashStatusTypeDef flashBin(void);
    void setRDPLevel(rdpLevel level);
    bool isConnected(void);
    bool checkConnection(QString portName);
public:
    Device();
private:
    QSerialPort device_;
    QSerialPortInfo portInfo_;
    portConfig config_;
    bool connectionStatus_;
    QString deviceDescription_;

    uint8_t wordToByte(uint32_t addr, uint8_t index, uint8_t lowerFirst);
    uint32_t getCRC(uint8_t* buf, uint32_t len);


};

#endif // DEVICE_H
