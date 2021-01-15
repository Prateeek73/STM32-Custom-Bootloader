/*
 * bootloader.c
 *
 *  Created on: Dec 18, 2020
 *      Author: Pravesh Narayan
 */

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "main.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "usb_device.h"
#include "usbd_cdc_if.h"
#include "bootloader.h"
#include "userHW.h"

/*	TypeDef declarations for UART1, UART2, CRC
 */
extern UART_HandleTypeDef huart1;
extern CRC_HandleTypeDef  hcrc;


/*	Declaration of the command packet recieved by the bootloader
 * 	user, and the list of supported commands for GetHelp command.
 */
uint8_t commandBuffer[commandBufferLen];
uint8_t supportedCommands[] = {fetchVersion,
								getHelp,
								fetchChipID,
								fetchRDPLevels,
								gotoAddress,
								eraseFlash,
								memoryWrite};




/*	@brief getCommand function
 * 		Recieves command packet from the command UART peripheral.
 * 	@params None
 * 	@returns None
 */
void getCommand(void)
{
#ifdef UARTBootloader
	uint8_t recieveLen = 0;

	HAL_UART_Receive(uartComm, commandBuffer, 1, HAL_MAX_DELAY);
	recieveLen = commandBuffer[0];
	HAL_UART_Receive(uartComm, &commandBuffer[1], recieveLen, HAL_MAX_DELAY);
#endif
}

/*	@brief sendACK function
 * 		Sends positive acknowledgement on CRC success.
 * 	@params (Command code, following length)
 * 	@returns None
 */
void sendACK(uint8_t commandCode, uint8_t followLen)
{
	uint8_t ackBuffer[2];

	ackBuffer[0] = ACK;
	ackBuffer[1] = followLen;
	HAL_UART_Transmit(uartComm, ackBuffer, 2, HAL_MAX_DELAY);
}

/*	@brief sendNACK function
 * 		Sends Negative acknowledgement on CRC fail.
 * 	@params None
 * 	@returns None
 */
void sendNACK(void)
{
	uint8_t nack = NACK;

	HAL_UART_Transmit(uartComm, &nack, 1, HAL_MAX_DELAY);
}

/*	@brief verifyCRC function
 * 		Verifies CRC for the command packet.
 * 	@params (Command code, length, Host CRC value)
 * 	@returns (8-bit unsigned int)
 */
uint8_t verifyCRC(uint8_t* commandData, uint32_t len, uint32_t crcHost)
{
	uint32_t CRCvalue = 0xff;

	for(uint32_t iter = 0; iter < len; iter++)
	{
		uint32_t iterData = commandData[iter];
		CRCvalue = HAL_CRC_Accumulate(&hcrc, &iterData, 1);
	}

	if(CRCvalue == crcHost)
		return verifySuccess;
	else
		return verifyFail;
}

/*	@brief bootloaderVersion function
 * 		Returns the value of Bootloader version
 * 	@params None
 * 	@returns (8-bit unsigned int)
 */
uint8_t bootloaderVersion(void)
{
	return BL_VER;
}

/*	@brief writeDataUART function
 * 		Sends buffer to the command UART peripheral
 * 	@params (Buffer pointer, length)
 * 	@returns None
 */
void writeDataUART(uint8_t* pData, uint8_t len)
{
	HAL_UART_Transmit(uartComm, pData, len, HAL_MAX_DELAY);
}

/*	@brief getChipID function
 * 		Returns the Chip identification number
 * 	@params None
 * 	@returns (16-bit unsigned int)
 */
uint16_t getChipID(void)
{
	uint16_t chipID_;

	chipID_ = (uint16_t)(DBGMCU->IDCODE) & 0x0FFF;
	return chipID_;
}

/*	@brief getRDP function
 * 		Returns the Read data protection value(0xAA-Level1)(0xCC-Level3)
 * 	@params None
 * 	@returns (8-bit unsigned int)
 */
uint8_t getRDP(void)
{
	uint8_t rdpLevel_ = 0;
	volatile uint32_t *pOptionByteAddr = (uint32_t*) optionBytes;

	rdpLevel_ = (uint8_t)(*pOptionByteAddr >> 8);
	return rdpLevel_;
}

uint32_t getPageAddr(uint8_t sectorNumber)
{
#ifdef F103C8
	uint32_t baseAddr = 0x08000000;
	for(uint8_t iter = 0; iter < sectorNumber; iter++)
	{
		baseAddr = baseAddr | 0x00000400;
	}
	return baseAddr;
#endif
#ifdef F411CC
	return 0x08000000;
#endif
}

/*	@brief flashErase function
 * 		Erases flash
 * 	@params (Sector number(0-7), number of sectors)
 * 	@returns (8-bit unsigned int)
 */
uint8_t flashErase(uint8_t sectorNumber , uint8_t numberOfSectors)
{
	FLASH_EraseInitTypeDef flashEraseHandler_;
	uint32_t sectorError;
	HAL_StatusTypeDef status_;

	if(numberOfSectors > 8)
	{
		return invalidSector;
	}

	if((sectorNumber == 0xff) || (sectorNumber <= 31))
	{
		if(sectorNumber == (uint8_t) 0xff)
		{
			flashEraseHandler_.TypeErase = FLASH_TYPEERASE_MASSERASE;
		}
		else
		{
			uint8_t remainingSectors_ = 8 - sectorNumber;
			if(numberOfSectors > remainingSectors_)
			{
				numberOfSectors = remainingSectors_;
			}
			flashEraseHandler_.TypeErase = FLASH_TYPEERASE_SECTORS;
			flashEraseHandler_.NbSectors = numberOfSectors;

			flashEraseHandler_.Sector = sectorNumber;
		}
		flashEraseHandler_.Banks = FLASH_BANK_1;
		HAL_FLASH_Unlock();
		status_ = (uint8_t)HAL_FLASHEx_Erase(&flashEraseHandler_, &sectorError);
		HAL_FLASH_Lock();

		return status_;
	}
	return invalidSector;
}

/*	@brief verifyAddr function
 * 		Verifies if the given address is valid or not
 * 	@params Address
 * 	@returns Status
 */
uint8_t verifyAddr(uint32_t addr_)
{
    if(addr_ >= flashBaseBegin && addr_ <= flashBaseEnd)
		return validAddr;
	else
		return invalidAddr;
}

/*	@brief executeMemoryFlash function
 * 		Writes the given binary packet onto the specified address
 * 	@params (command pointer, address, length of packet)
 * 	@returns (8-bit unsigned int)
 */
uint8_t executeMemoryFlash(uint8_t *command, uint32_t addr_, uint32_t len)
{
    uint8_t status_ = HAL_OK;

    HAL_FLASH_Unlock();
    for(uint32_t iter = 0 ; iter <len ; iter++)
    {
        status_ = HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, addr_ + iter, command[iter]);
    }
    HAL_FLASH_Lock();

    return status_;
}



/*	@brief fetchVersionCommand function
 * 		Writes the version of the Bootloader in the Command UART peripheral
 * 	@params Command pointer
 * 	@returns None
 */
void fetchVersionCommand(uint8_t* command)
{
	uint8_t bootloaderVer_;
	uint32_t packetLen = command[0] + 1;
	uint32_t hostCRC = *((uint32_t *) (command + packetLen - 4));

	sendMessage("Verifying checksum...\n");

	if(!verifyCRC(&command[0], packetLen - 4, hostCRC))
	{
		sendMessage("Checksum success!\n");
		sendACK(command[0], 1);
		bootloaderVer_ = bootloaderVersion();
		sendMessage("Bootloader version : %d %#x\n", bootloaderVer_, bootloaderVer_);
		writeDataUART(&bootloaderVer_, 1);
		infoLED(1, 100);
	}
	else
	{
		sendMessage("Checksum failed!\n");
		sendNACK();
	}
}

/*	@brief getHelpCommand function
 * 		Displays the supported commands list in the command UART peripheral
 * 	@params Command pointer
 * 	@returns None
 */
void getHelpCommand(uint8_t* command)
{
	uint32_t packetLen = command[0] + 1;
	uint32_t hostCRC = *((uint32_t *) (command + packetLen - 4));

	sendMessage("Verifying checksum...\n");

	if(!verifyCRC(&command[0], packetLen - 4, hostCRC))
	{
		sendMessage("Checksum success!\n");
		sendACK(command[0], 1);
		writeDataUART(supportedCommands, sizeof(supportedCommands));
		infoLED(1, 100);
	}
	else
	{
		sendMessage("Checksum failed!\n");
		sendNACK();
	}
}

/*	@brief fetchChipIDCommand function
 * 		Displays the Chip identification number in the command UART peripheral
 * 	@params Command pointer
 * 	@returns None
 */
void fetchChipIDCommand(uint8_t* command)
{
	uint16_t chipID_ = getChipID();
	uint32_t packetLen = command[0] + 1;
	uint32_t hostCRC = *((uint32_t *) (command + packetLen - 4));

	sendMessage("Verifying checksum...\n");

	if(!verifyCRC(&command[0], packetLen - 4, hostCRC))
	{
		sendMessage("Checksum success!\n");
		sendACK(command[0], 1);
		writeDataUART((uint8_t *)&chipID_, 2);
		infoLED(1, 100);
	}
	else
	{
		sendMessage("Checksum failed!\n");
		sendNACK();
	}
}

/*	@brief fetchRDPLevelsCommand function
 * 		Displays the RDP level number in the command UART peripheral
 * 	@params Command pointer
 * 	@returns None
 */
void fetchRDPLevelsCommand(uint8_t* command)
{
	uint8_t rdpStatus_ = getRDP();
	uint32_t packetLen = command[0] + 1;
	uint32_t hostCRC = *((uint32_t *) (command + packetLen - 4));

	sendMessage("Verifying checksum...\n");

	if(!verifyCRC(&command[0], packetLen - 4, hostCRC))
	{
		sendMessage("Checksum success!\n");
		sendACK(command[0], 1);
		writeDataUART(&rdpStatus_, 1);
		infoLED(1, 100);
	}
	else
	{
		sendMessage("Checksum failed!\n");
		sendNACK();
	}
}

/*	@brief gotoAddressCommand function
 * 		Jumps to the given address in memory map
 * 	@params Command pointer
 * 	@returns None
 */
void gotoAddressCommand(uint8_t* command)
{
	//Will be added later.
}

/*	@brief eraseFlashCommand function
 * 		Jumps to the given address in memory map
 * 	@params Command pointer
 * 	@returns None
 */
void eraseFlashCommand(uint8_t* command)
{
	uint8_t eraseStatus = 0x00;
	uint32_t packetLen = command[0] + 1;
	uint32_t hostCRC = *((uint32_t *) (command + packetLen - 4));

	sendMessage("Verifying checksum..\n.");
	if(!verifyCRC(&command[0], packetLen - 4, hostCRC))
	{
		sendMessage("Checksum success!\n");
		sendMessage("Clearing flash memory...\n");
		sendACK(command[0], 1);

		infoLED(5, 100);
		eraseStatus = flashErase(command[2], command[3]);
		sendMessage("Flash erase status :  %#x\n", eraseStatus);
		writeDataUART(&eraseStatus, 1);
	}
	else
	{
		sendMessage("Checksum failed!\n");
		sendNACK();
	}
}

/*	@brief memoryWriteCommand function
 * 		Jumps to the given address in memory map
 * 	@params Command pointer
 * 	@returns None
 */
void memoryWriteCommand(uint8_t* command)
{
	uint8_t validAddr_ = validAddr;
	uint8_t writeStatus = 0x00;
	uint8_t payloadLength = command[6];
	uint32_t memAddr_ = *((uint32_t *)(&command[2]));
	uint32_t packetLen = command[0] + 1;
	uint32_t hostCRC = *((uint32_t *) (command + packetLen - 4));

	sendMessage("Verifying checksum..\n.");
	if(!verifyCRC(&command[0], packetLen - 4, hostCRC))
	{
		sendMessage("Checksum success!\n");
		sendMessage("Initiating flash...\n");
		sendACK(command[0], 1);
		infoLED(5, 100);
		if(verifyAddr(memAddr_) == validAddr_)
		{
            sendMessage("Address is valid!\n");
    		infoLED(1, 100);
    		writeStatus = executeMemoryFlash(&command[7],memAddr_, payloadLength);
    		infoLED(1, 100);
    		writeDataUART(&writeStatus,1);
		}
		else
		{
            sendMessage("Address is invalid!\n");
            writeStatus = validAddr_;
            writeDataUART(&writeStatus,1);
		}
	}
	else
	{
		sendMessage("Checksum failed!\n");
		sendNACK();
	}

}

/*	@brief changeRDPCommand function
 * 		Jumps to the given address in memory map
 * 	@params Command pointer
 * 	@returns None
 */
void changeRDPCommand(uint8_t* command)
{
	//Will be added later
}

/* 	@brief gotoToApplication Function
 * 		Jumps to user application.
 *  @param None
 *  @retval None
 */
void gotoToApplication(void)
{
	//Switching to user Application
	infoLED(2, 200);

	void(*appResetHandler)(void);
	sendMessage("Initializing user application...\n\r");

	uint32_t mspValue = *(volatile uint32_t *) FLASH_SECTOR2_BASE_ADDRESS;
	sendMessage("MSP - %#x\n", mspValue);

	__set_MSP(mspValue);

	uint32_t resetAddress = *(volatile uint32_t *) (FLASH_SECTOR2_BASE_ADDRESS + 4);
	appResetHandler = (void* ) resetAddress;
	sendMessage("Reset address - %#x\n", appResetHandler);

	sendMessage("Done!\n");
	appResetHandler();
}

/* 	@brief gotoBootloader Function
 * 		Jumps to Bootloader.
 *  @param None
 *  @retval None
 */
void gotoBootloader(void)
{
	//Switched to Bootloader
	sendMessage("Initializing Bootloader...");
	infoLED(4, 200);

	while(1)
	{
		memset(commandBuffer, 0, commandBufferLen);
		getCommand();
		switch(commandBuffer[1])
		{
			case fetchVersion:
				fetchVersionCommand(commandBuffer);
				break;
			case getHelp:
				getHelpCommand(commandBuffer);
				break;
			case fetchChipID:
				fetchChipIDCommand(commandBuffer);
				break;
			case fetchRDPLevels:
				fetchRDPLevelsCommand(commandBuffer);
				break;
			case gotoAddress:
				gotoAddressCommand(commandBuffer);
				break;
			case eraseFlash:
				eraseFlashCommand(commandBuffer);
				break;
			case memoryWrite:
				memoryWriteCommand(commandBuffer);
				break;
			case changeRDP:
				changeRDPCommand(commandBuffer);
				break;
			default:
				sendMessage("Error, no such command!\n");
				break;
		}
	}
}

