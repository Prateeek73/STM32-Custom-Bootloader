#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>

#define getVersionCommandLength                 6
#define getHelpCommandLength                    6
#define getChipIDCommandLength                  6
#define getRDPStatusCommandLength               6
#define gotoAddressCommandLength                10
#define flashEraseCommandLength                 8
#define flashBinaryCommandLength(x)             (7 + x + 4)
#define changeRDPCommandLength                  8


#define COMMAND_BL_GET_VER                  0x51
#define COMMAND_BL_GET_HELP                 0x52
#define COMMAND_BL_GET_CID                  0x53
#define COMMAND_BL_GET_RDP_STATUS           0x54
#define COMMAND_BL_GO_TO_ADDR               0x55
#define COMMAND_BL_FLASH_ERASE              0x56
#define COMMAND_BL_MEM_WRITE                0x57
#define COMMAND_BL_EN_R_W_PROTECT           0x58
#define COMMAND_BL_MEM_READ                 0x59
#define COMMAND_BL_READ_SECTOR_P_STATUS     0x5A
#define COMMAND_BL_OTP_READ                 0x5B
#define COMMAND_BL_DIS_R_W_PROTECT          0x5C
#define COMMAND_BL_MY_NEW_COMMAND           0x5D

#define applicationFlashBase                0X08008000
#define resetHandeler                       0x080081D8
typedef enum
{
    getVersionCommand = 1,
    getHelpCommand = 2,
    getChipIDCommand = 3,
    getRDPStatusCommand = 4,
    gotoAddressCommand = 5,
    eraseFlashCommand = 6,
    flashBinaryCommand = 7,
    changeRDPCommand = 8,
    debug = 9
} bootloaderCommand;


typedef enum
{
    levelZero = 1,
    levelOne = 2,
    levelTwo = 3
} rdpLevel;

typedef enum
{
  Flash_HAL_OK       = 0x00U,
  Flash_HAL_ERROR    = 0x01U,
  Flash_HAL_BUSY     = 0x02U,
  Flash_HAL_TIMEOUT  = 0x03U,
  Flash_HAL_INV_ADDR = 0x04U,
  Error
} HAL_FlashStatusTypeDef;

typedef enum
{
  HAL_OK       = 0x00U,
  HAL_ERROR    = 0x01U,
  HAL_BUSY     = 0x02U,
  HAL_TIMEOUT  = 0x03U
} HAL_StatusTypeDef;

typedef union
{
    uint16_t flash_sector_status;
    struct
    {
        uint16_t sector0:2;
        uint16_t sector1:2;
        uint16_t sector2:2;
        uint16_t sector3:2;
        uint16_t sector4:2;
        uint16_t sector5:2;
        uint16_t sector6:2;
        uint16_t sector7:2;

    }sectors;

}sectorStatus;
#endif // CONFIG_H
