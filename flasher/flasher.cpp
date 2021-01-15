#include "flasher.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include "ui_flasher.h"
#include "config.h"
#include "device.h"
#include "dialog.h"



Flasher::Flasher(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Flasher)
{
    ui->setupUi(this);
    this->setStyleSheet("background-color: black;");
    setFixedSize(width(), height());
    setDefault();
}

Flasher::~Flasher()
{
    delete ui;
}

bool Flasher::checkConnection(QString portName)
{
    return device_.checkConnection(portName);
}

void Flasher::on_connectButton_released()
{
    QString portName = ui->comPortValue->text();
    bool isConnected = checkConnection(portName);

    if(isConnected)
    {
        device_.connectDevice();
        ui->disconnectButon->setEnabled(true);
        ui->disconnectButon->setVisible(true);
    }
    status_.connectStatus(isConnected, portName);
}

void Flasher::setDefault()
{
    ui->disconnectButon->setDisabled(true);
    ui->disconnectButon->setVisible(false);
    resetChoice();
    ui->commandList->addItem("Select a command...");
    ui->commandList->addItem("Bootloader version");
    ui->commandList->addItem("Get help");
    ui->commandList->addItem("Get chip ID");
    ui->commandList->addItem("Get RDP status");
    ui->commandList->addItem("Jump to address");
    ui->commandList->addItem("Erase flash");
    ui->commandList->addItem("Flash binary");
    ui->commandList->addItem("Change RDP level");
    ui->commandList->addItem("Debug");

    ui->rdpList->addItem("Level 0(default)");
    ui->rdpList->addItem("Level 1(Read protect)");
    ui->rdpList->addItem("Level 2(Write protect)");
}

void Flasher::resetChoice()
{
    ui->rdpList->setEnabled(false);
    ui->writeRDPbutton->setEnabled(false);
    ui->writeButton->setEnabled(false);
    ui->writeButton->setStyleSheet("QPushButton{ background-color : #cca300; color : black; }");
    ui->writeRDPbutton->setStyleSheet("QPushButton{ background-color : #cca300; color : black; }");
}

void Flasher::getVersion()
{

}

void Flasher::getHelp()
{

}

void Flasher::getChipID()
{

}

void Flasher::getRDPStatus()
{

}

void Flasher::gotoAddress()
{

}


void Flasher::eraseFlash()
{

}

void Flasher::flashBinary()
{
    QFileDialog openFile_;
    QString filename_ = openFile_.getOpenFileName(this,
                              "Open Binary",
                              QDir::currentPath(),
                              "Binary files (*.bin)");
}

void Flasher::changeRDP()
{
    int index_ = ui->rdpList->currentIndex();

    switch(index_)
    {
    case levelZero:
        break;
    case levelOne:
        break;
    case levelTwo:
        break;
    }
}

void Flasher::noCommand()
{

}
void Flasher::on_commandList_currentIndexChanged(int index)
{
    switch(index)
    {
        case getVersionCommand:
            resetChoice();
            ui->writeButton->setEnabled(true);
            ui->writeButton->setStyleSheet("QPushButton{ background-color : #ffcc00; color : black; }");
            break;
        case getHelpCommand:
            resetChoice();
            ui->writeButton->setEnabled(true);
            ui->writeButton->setStyleSheet("QPushButton{ background-color : #ffcc00; color : black; }");
            break;
        case getChipIDCommand:
            resetChoice();
            ui->writeButton->setEnabled(true);
            ui->writeButton->setStyleSheet("QPushButton{ background-color : #ffcc00; color : black; }");
            break;
        case getRDPStatusCommand:
            resetChoice();
            ui->writeButton->setEnabled(true);
            ui->writeButton->setStyleSheet("QPushButton{ background-color : #ffcc00; color : black; }");
            break;
        case gotoAddressCommand:
            resetChoice();
            ui->writeButton->setEnabled(true);
            ui->writeButton->setStyleSheet("QPushButton{ background-color : #ffcc00; color : black; }");
            break;
        case eraseFlashCommand:
            resetChoice();
            ui->writeButton->setEnabled(true);
            ui->writeButton->setStyleSheet("QPushButton{ background-color : #ffcc00; color : black; }");
            break;
        case flashBinaryCommand:
            resetChoice();
            ui->writeButton->setEnabled(true);
            ui->writeButton->setStyleSheet("QPushButton{ background-color : #ffcc00; color : black; }");
            break;
        case changeRDPCommand:
            resetChoice();
            ui->writeButton->setEnabled(false);
            ui->rdpList->setEnabled(true);
            ui->writeRDPbutton->setEnabled(true);
            ui->writeRDPbutton->setStyleSheet("QPushButton{ background-color : #ffcc00; color : black; }");
            break;
        case debug:
            resetChoice();
            ui->writeButton->setEnabled(true);
            ui->writeButton->setStyleSheet("QPushButton{ background-color : #ffcc00; color : black; }");
            break;
        default:
            noCommand();
            break;
    }
}


void Flasher::on_writeButton_released()
{
    int index = ui->commandList->currentIndex();
    uint8_t dataBuffer[255];
    uint32_t crc32 = 0;
    int returnVal = 0;

    switch(index)
    {
        case getVersionCommand:
            getVersion(dataBuffer, crc32, returnVal);
            break;
        case getHelpCommand:
            getHelp();
            break;
        case getChipIDCommand:
            getChipID();
            break;
        case getRDPStatusCommand:
            getRDPStatus();
            break;
        case gotoAddressCommand:
            gotoAddress();
            break;
        case eraseFlashCommand:
            eraseFlash();
            break;
        case flashBinaryCommand:
            flashBinary();
            break;
        case debug:

        default:
            noCommand();
            break;
    }
}

void Flasher::on_writeRDPbutton_released()
{
    changeRDP();
}

void Flasher::on_disconnectButon_released()
{
    device_.disconnectDevice();
    status_.disconnectStatus();
    ui->disconnectButon->setDisabled(true);
    ui->disconnectButon->setVisible(false);
}



