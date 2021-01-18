#include "dialog.h"

Status::Status()
{

}

void Status::flashStatus(HAL_FlashStatusTypeDef status)
{
    QString message_;

    dialog_.setWindowTitle("Status");
    dialog_.setIcon(QMessageBox::Information);
    switch(status)
    {
    case Error:
        message_ = "Error in flashing!";
        dialog_.setIcon(QMessageBox::Warning);
        dialog_.setText(message_);
        break;
    case Flash_HAL_BUSY:
        message_ = "Flash status returned : Busy";
        dialog_.setText(message_);
        break;
    case Flash_HAL_ERROR:
        message_ = "Flash status returned : Error";
        dialog_.setIcon(QMessageBox::Warning);
        dialog_.setText(message_);
        break;
    case Flash_HAL_INV_ADDR:
        message_ = "Flash status returned : Invalid Address";
        dialog_.setIcon(QMessageBox::Warning);
        dialog_.setText(message_);
        break;
    case Flash_HAL_OK:
        message_ = "Flash status returned : Ok";
        dialog_.setText(message_);
        break;
    case Flash_HAL_TIMEOUT:
        message_ = "Flash status returned : Timeout";
        dialog_.setText(message_);
        break;
    default:
        break;
    }
    dialog_.exec();
}

void Status::status(HAL_StatusTypeDef status)
{
    QString message_;

    dialog_.setWindowTitle("Status");
    dialog_.setIcon(QMessageBox::Information);
    switch(status)
    {
    case HAL_BUSY:
        message_ = "Status : Device busy";
        dialog_.setText(message_);
        break;
    case HAL_ERROR:
        message_ = "Status : Device Error";
        dialog_.setIcon(QMessageBox::Warning);
        dialog_.setText(message_);
        break;
    case HAL_OK:
        message_ = "Status : Ok";
        dialog_.setText(message_);
        break;
    case HAL_TIMEOUT:
        message_ = "Status : Request Timeout";
        dialog_.setText(message_);
        break;
    default:
        break;
    }
    dialog_.exec();
}

void Status::connectStatus(bool connection, QString enteredPort)
{
    QString message_;

    if(enteredPort.isEmpty())
    {
        dialog_.setWindowTitle("Error!");
        dialog_.setIcon(QMessageBox::Warning);
        message_ = "Port name cannot be blank!";
    }

    else if(connection)
    {
        dialog_.setWindowTitle("Connected!");
        dialog_.setIcon(QMessageBox::Information);
        message_ = "Device connected to " + enteredPort + " successfully!";
    }
    else
    {
        dialog_.setWindowTitle("Error!");
        dialog_.setIcon(QMessageBox::Warning);
        message_ = enteredPort + " is not a valid valid COM port!";
    }
    dialog_.setText(message_);
    dialog_.exec();
}

void Status::disconnectStatus()
{
    QString message_;

    message_ = "Device disconnected successfully!";
    dialog_.setIcon(QMessageBox::Information);
    dialog_.setWindowTitle("Disconnected!");
    dialog_.setText(message_);
    dialog_.exec();
}

bool Status::areYouSure(bootloaderCommand command)
{
    QString message_;

    dialog_.setStandardButtons(QMessageBox::Yes);
    dialog_.addButton(QMessageBox::No);
    dialog_.setDefaultButton(QMessageBox::No);
    dialog_.setIcon(QMessageBox::Question);
    dialog_.setWindowTitle("Are you sure?");
    switch(command)
    {
    case getVersionCommand:
        message_ = "Get bootloader version from device?";
        dialog_.setText(message_);
        return dialog_.exec();
        break;
    case getHelpCommand:
        message_ = "Get help from device?";
        dialog_.setText(message_);
        return dialog_.exec();
        break;
    case getChipIDCommand:
        message_ = "Get Chip ID from device?";
        dialog_.setText(message_);
        return dialog_.exec();
        break;
    case getRDPStatusCommand:
        message_ = "Get RDP status from device?";
        dialog_.setText(message_);
        return dialog_.exec();
        break;
    case gotoAddressCommand:
        message_ = "JTA from device?";
        dialog_.setText(message_);
        return dialog_.exec();
        break;
    case eraseFlashCommand:
        message_ = "This will erase flash memory, proceed?";
        dialog_.setText(message_);
        return dialog_.exec();
        break;
    case flashBinaryCommand:
        message_ = "Flash binary file?";
        dialog_.setText(message_);
        return dialog_.exec();
        break;
    case changeRDPCommand:
        return true;
        break;
    default:
        return false;
        break;
    }
}



