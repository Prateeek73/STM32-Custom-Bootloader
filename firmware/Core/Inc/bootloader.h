/*
 * bootloader.h
 *
 *  Created on: Dec 18, 2020
 *      Author: Pravesh Narayan
 */

#ifndef INC_BOOTLOADER_H_
#define INC_BOOTLOADER_H_

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "main.h"
#include "usb_device.h"
#include "usbd_cdc_if.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"

#define BL_VER		1.0f
#define uartComm	&huart1
#define infoLED_	GPIO_PIN_13
#define infoPort	GPIOC
#define userButton	GPIO_PIN_1
#define userPort	GPIOA

#define fetchVersion	0x51
#define getHelp			0x52
#define fetchChipID		0x53
#define fetchRDPLevels	0x54
#define gotoAddress		0x55
#define eraseFlash		0x56
#define memoryWrite		0x57
#define changeRDP		0x58
#define ACK				0xA5
#define NACK			0x7F
#define verifySuccess	0
#define verifyFail		1
#define optionBytes		0x1FFFC000
#define invalidSector	0x04
#define flashBaseBegin	0x08000000
#define flashBaseEnd	0x08007FFF
#define invalidAddr		0x01
#define validAddr		0x00


#define commandBufferLen	200

//#define usbBootloader
#define UARTBootloader
//#define F103C8
#define	F411CC


void gotoToApplication(void);

void gotoBootloader(void);

void getCommand(void);

void fetchVersionCommand(uint8_t* command);

void getHelpCommand(uint8_t* command);

void fetchChipIDCommand(uint8_t* command);

void fetchRDPLevelsCommand(uint8_t* command);

void gotoAddressCommand(uint8_t* command);

void eraseFlashCommand(uint8_t* command);

void memoryWriteCommand(uint8_t* command);

void changeRDPCommand(uint8_t* command);

void sendACK(uint8_t commandCode, uint8_t followLen);

void sendNACK();

uint8_t verifyCRC(uint8_t* commandData, uint32_t len, uint32_t crcHost);

uint8_t bootloaderVersion(void);

void writeDataUART(uint8_t* pData, uint8_t len);

uint16_t getChipID(void);

uint8_t flashErase(uint8_t sectorNumber , uint8_t numberOfSectors);

uint32_t getPageAddr(uint8_t sectorNumber);

uint8_t verifyAddr(uint32_t addr_);

uint8_t executeMemoryFlash(uint8_t *command, uint32_t addr_, uint32_t len);

#endif /* INC_BOOTLOADER_H_ */
