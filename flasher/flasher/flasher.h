#ifndef FLASHER_H
#define FLASHER_H


#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include "config.h"
#include "device.h"
#include "dialog.h"


QT_BEGIN_NAMESPACE
namespace Ui { class Flasher; }
QT_END_NAMESPACE

class Flasher : public QMainWindow
{
    Q_OBJECT

public:
    bool checkConnection(QString portName);
    Flasher(QWidget *parent = nullptr);
    ~Flasher();

private slots:
    void on_connectButton_released();

    void on_commandList_currentIndexChanged(int index);

    void on_writeButton_released();

    void on_writeRDPbutton_released();

    void on_disconnectButon_released();

private:
    Ui::Flasher *ui;
    Device myDevice;
    Status status_;


    void setDefault(void);
    void getVersion(void);
    void getHelp(void);
    void getChipID(void);
    void getRDPStatus(void);
    void gotoAddress(void);
    void eraseFlash(void);
    void flashBinary(void);
    void changeRDP(void);
    void noCommand(void);
    void resetChoice(void);
};
#endif // FLASHER_H
