# STM32-Custom-Bootloader
  Custom bootloader firmware for STM32F4, but it is a library so it can be ported to any STM MCU by a change to a header file. I used STMCubeIDE to build the flash file for the     MCU. Also, the bootloader is not complete yet, it works but it needs a host application to communicate with, which I am making with QT developer. Flash the bootloader in the       sector 1 of Flash memory and set the VTOR to 0x08008000 i.e. Sector 2 for application firmware.
# Host Application
  You do need Qt to make the gui application, it is in development right now. 
# Contribute!
  It will be nice to get open contribution to the Project.
# Follow
