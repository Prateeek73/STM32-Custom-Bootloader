#ifndef DIALOG_H
#define DIALOG_H

#include <QObject>
#include <QWidget>
#include <QMessageBox>
#include <stdint.h>
#include "config.h"





class Status:public QMessageBox
{
public:
    void flashStatus(HAL_FlashStatusTypeDef status);
    void status(HAL_StatusTypeDef status);
    void connectStatus(bool connection, QString enteredPort);
    void disconnectStatus(void);
    bool areYouSure(bootloaderCommand command);

private:
    QMessageBox dialog_;
public:
    Status();

};

#endif // DIALOG_H
