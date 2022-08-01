# Building Mingda Firmware
- The original 1.6.6.5 source code is Mingda official code, and has been compiled and tested successfully.
- Further revisions will incrementally implement additional Marlin options and update code with the goal of eventually modernizing the code to reflect the latest Marlin stock releases. This will be a difficult and long process, as the original source code is heavily modified and unique.
- This code is capable of compiling firmware for various Mingda models using the following 32-bit Mingda board and its iterations.
  - Board Model Numbers: BC.DZ.PC000007.03, 04
  - Board Name/Aliases: langgo4007, MD d301
  - Microcontrollers: 
    - (S) = STM32 
        - [STM32F407ZET6 (192k RAM. 512k Flash)] https://www.st.com/en/microcontrollers-microprocessors/stm32f407ze.html
    - (G) = GD32
  - Features:
    - Integrated 2208 stepper drivers

  *NOTE: Both (S) and (G) Mingda Magician X firmwares can be found in the "PRECOMPILED" folder in the root directory.

- Compiling Process
  1) In Configuration.h, ensure only one of these options are defined, and comment the other out:
  - #define ST32_SHIP //this should be used for STM32 (S) boards.
  - #define USE_GD32  //this should be used for GD32 (G) boards.
  2) In platformio.ini, ensure only one of these options are defined, and comment the other out:
  - default_envs = langgo407ve_st //this should be used for STM32 (S) boards.
  - default_envs = langgo407ve_gd //this should be used for GD32 (G) boards.
  3) Compile: Using VS Code, click the check mark at the bottom for "PlatformIO: Build".
  4) Browse to: ".pio\build\langgo407ve_[st (S Board) or gd (G Board)], and retreive "firmware.bin".
  5) Place "firmware.bin" on an SD card and flash the firmware as usual. Include the Mingda "TFT35" folder alongside firmware file if you suspect the need to update the icon/font pack for the LCD.

# Mingda Firmware Changelog
1.6.6.5
 - Updated both microcontroller configuration comments for clarity.
 - Updated deprecated src_filter commands in platformio.ini
 - Compiled and populated PRECOMPILED folder with (S/G) Magician X firmware and TFT folder.

 Marlin\src\pins\stm32f4\pins_langgo407.h
 pins file

 Marlin\src\lcd\extui\tsc
 HOME OF GUI

buildroot\share\PlatformIO\variants\MARLIN_F407VE\variant.h:
// Default pin used for 'Serial' instance (ex: ST-Link)
// UNIQUE
// Mandatory for Firmata
// #define PIN_SERIAL_RX           PA10
// #define PIN_SERIAL_TX           PA9

// Timer Definitions
// Do not use timer used by PWM pins when possible. See PinMap_PWM in PeripheralPins.c
// UNIQUE
#define TIMER_TONE              TIM3  // TIMER_TONE must be defined in this file

// On-board user button
// UNIQUE
//#define BTN_K0                  PE4
//#define BTN_K1                  PE3
//#define USER_BTN                BTN_K0

// ADC resolution is 12bits
#define ADC_RESOLUTION          10

buildroot\share\PlatformIO\variants\MARLIN_F407VE\hal_conf_custom.h
  * @brief This is the list of modules to be used in the HAL driver
  */
#define HAL_MODULE_ENABLED
#define HAL_ADC_MODULE_ENABLED
#define HAL_CRC_MODULE_ENABLED
#define HAL_DAC_MODULE_ENABLED
#define HAL_DMA_MODULE_ENABLED
//#define HAL_EXTI_MODULE_ENABLED   // Needed for Endstop (and other external) Interrupts
#define HAL_FLASH_MODULE_ENABLED
#define HAL_GPIO_MODULE_ENABLED
#define HAL_I2C_MODULE_ENABLED
#define HAL_PWR_MODULE_ENABLED
#define HAL_RCC_MODULE_ENABLED
#define HAL_RTC_MODULE_ENABLED
#define HAL_SD_MODULE_ENABLED
#define HAL_SPI_MODULE_ENABLED
#define HAL_TIM_MODULE_ENABLED
#define HAL_CORTEX_MODULE_ENABLED
//#define HAL_PCD_MODULE_ENABLED // Automatically added if any type of USB is enabled, as in Arduino IDE. (STM32 v3.10700.191028)

//#define HAL_CAN_MODULE_ENABLED
//#define HAL_CAN_LEGACY_MODULE_ENABLED
//#define HAL_CEC_MODULE_ENABLED
//#define HAL_CRYP_MODULE_ENABLED
//#define HAL_DCMI_MODULE_ENABLED
//#define HAL_DMA2D_MODULE_ENABLED
//#define HAL_ETH_MODULE_ENABLED
//#define HAL_NAND_MODULE_ENABLED
//#define HAL_NOR_MODULE_ENABLED
//#define HAL_PCCARD_MODULE_ENABLED
#define HAL_SRAM_MODULE_ENABLED
//#define HAL_SDRAM_MODULE_ENABLED
//#define HAL_HASH_MODULE_ENABLED
//#define HAL_SMBUS_MODULE_ENABLED
//#define HAL_I2S_MODULE_ENABLED
//#define HAL_IWDG_MODULE_ENABLED
//#define HAL_LTDC_MODULE_ENABLED
//#define HAL_DSI_MODULE_ENABLED
//#define HAL_QSPI_MODULE_ENABLED
//#define HAL_RNG_MODULE_ENABLED
//#define HAL_SAI_MODULE_ENABLED
//#define HAL_UART_MODULE_ENABLED
//#define HAL_USART_MODULE_ENABLED
//#define HAL_IRDA_MODULE_ENABLED
//#define HAL_SMARTCARD_MODULE_ENABLED
//#define HAL_WWDG_MODULE_ENABLED
#define HAL_HCD_MODULE_ENABLED
//#define HAL_FMPI2C_MODULE_ENABLED
//#define HAL_SPDIFRX_MODULE_ENABLED
//#define HAL_DFSDM_MODULE_ENABLED
//#define HAL_LPTIM_MODULE_ENABLED
//#define HAL_MMC_MODULE_ENABLED

Marlin\src\HAL\shared\eeprom_if.h
#include "../../lcd/extui/tsc/Hal/w25qxx.h"

THESE 4 REQUIRED FOR USB SUPPORT ON MAINBOARD:
Marlin\lib\fat_fs
Marlin\lib\rtt
Marlin\lib\GD32F4xx_usb_library
Marlin\lib\STM32_USB_Host_Library




<p align="center"><img src="buildroot/share/pixmaps/logo/marlin-outrun-nf-500.png" height="250" alt="MarlinFirmware's logo" /></p>

<h1 align="center">Marlin 3D Printer Firmware</h1>

<p align="center">
    <a href="/LICENSE"><img alt="GPL-V3.0 License" src="https://img.shields.io/github/license/marlinfirmware/marlin.svg"></a>
    <a href="https://github.com/MarlinFirmware/Marlin/graphs/contributors"><img alt="Contributors" src="https://img.shields.io/github/contributors/marlinfirmware/marlin.svg"></a>
    <a href="https://github.com/MarlinFirmware/Marlin/releases"><img alt="Last Release Date" src="https://img.shields.io/github/release-date/MarlinFirmware/Marlin"></a>
    <a href="https://github.com/MarlinFirmware/Marlin/actions"><img alt="CI Status" src="https://github.com/MarlinFirmware/Marlin/actions/workflows/test-builds.yml/badge.svg"></a>
    <a href="https://github.com/sponsors/thinkyhead"><img alt="GitHub Sponsors" src="https://img.shields.io/github/sponsors/thinkyhead?color=db61a2"></a>
    <br />
    <a href="https://twitter.com/MarlinFirmware"><img alt="Follow MarlinFirmware on Twitter" src="https://img.shields.io/twitter/follow/MarlinFirmware?style=social&logo=twitter"></a>
</p>

Additional documentation can be found at the [Marlin Home Page](https://marlinfw.org/).
Please test this firmware and let us know if it misbehaves in any way. Volunteers are standing by!

## Marlin 2.1

Marlin 2.1 continues to support both 32-bit ARM and 8-bit AVR boards while adding support for up to 9 coordinated axes and to up to 8 extruders.

Download earlier versions of Marlin on the [Releases page](https://github.com/MarlinFirmware/Marlin/releases).

## Example Configurations

Before building Marlin you'll need to configure it for your specific hardware. Your vendor should have already provided source code with configurations for the installed firmware, but if you ever decide to upgrade you'll need updated configuration files. Marlin users have contributed dozens of tested example configurations to get you started. Visit the [MarlinFirmware/Configurations](https://github.com/MarlinFirmware/Configurations) repository to find the right configuration for your hardware.

## Building Marlin 2.1

To build Marlin 2.1 you'll need [Arduino IDE 1.8.8 or newer](https://www.arduino.cc/en/main/software) or [PlatformIO](http://docs.platformio.org/en/latest/ide.html#platformio-ide). Detailed build and install instructions are posted at:

  - [Installing Marlin (Arduino)](http://marlinfw.org/docs/basics/install_arduino.html)
  - [Installing Marlin (VSCode)](http://marlinfw.org/docs/basics/install_platformio_vscode.html).

### Supported Platforms

  Platform|MCU|Example Boards
  --------|---|-------
  [Arduino AVR](https://www.arduino.cc/)|ATmega|RAMPS, Melzi, RAMBo
  [Teensy++ 2.0](https://www.microchip.com/en-us/product/AT90USB1286)|AT90USB1286|Printrboard
  [Arduino Due](https://www.arduino.cc/en/Guide/ArduinoDue)|SAM3X8E|RAMPS-FD, RADDS, RAMPS4DUE
  [ESP32](https://github.com/espressif/arduino-esp32)|ESP32|FYSETC E4, E4d@BOX, MRR
  [LPC1768](https://www.nxp.com/products/processors-and-microcontrollers/arm-microcontrollers/general-purpose-mcus/lpc1700-cortex-m3/512-kb-flash-64-kb-sram-ethernet-usb-lqfp100-package:LPC1768FBD100)|ARM® Cortex-M3|MKS SBASE, Re-ARM, Selena Compact
  [LPC1769](https://www.nxp.com/products/processors-and-microcontrollers/arm-microcontrollers/general-purpose-mcus/lpc1700-cortex-m3/512-kb-flash-64-kb-sram-ethernet-usb-lqfp100-package:LPC1769FBD100)|ARM® Cortex-M3|Smoothieboard, Azteeg X5 mini, TH3D EZBoard
  [STM32F103](https://www.st.com/en/microcontrollers-microprocessors/stm32f103.html)|ARM® Cortex-M3|Malyan M200, GTM32 Pro, MKS Robin, BTT SKR Mini
  [STM32F401](https://www.st.com/en/microcontrollers-microprocessors/stm32f401.html)|ARM® Cortex-M4|ARMED, Rumba32, SKR Pro, Lerdge, FYSETC S6
  [STM32F7x6](https://www.st.com/en/microcontrollers-microprocessors/stm32f7x6.html)|ARM® Cortex-M7|The Borg, RemRam V1
  [SAMD51P20A](https://www.adafruit.com/product/4064)|ARM® Cortex-M4|Adafruit Grand Central M4
  [Teensy 3.5](https://www.pjrc.com/store/teensy35.html)|ARM® Cortex-M4|
  [Teensy 3.6](https://www.pjrc.com/store/teensy36.html)|ARM® Cortex-M4|
  [Teensy 4.0](https://www.pjrc.com/store/teensy40.html)|ARM® Cortex-M7|
  [Teensy 4.1](https://www.pjrc.com/store/teensy41.html)|ARM® Cortex-M7|
  Linux Native|x86/ARM/etc.|Raspberry Pi

## Submitting Patches

- Submit **Bug Fixes** as Pull Requests to the ([bugfix-2.1.x](https://github.com/MarlinFirmware/Marlin/tree/bugfix-2.1.x)) branch.
- Follow the [Coding Standards](http://marlinfw.org/docs/development/coding_standards.html) to gain points with the maintainers.
- Please submit your questions and concerns to the [Issue Queue](https://github.com/MarlinFirmware/Marlin/issues).

## Marlin Support

The Issue Queue is reserved for Bug Reports and Feature Requests. To get help with configuration and troubleshooting, please use the following resources:

- [Marlin Documentation](https://marlinfw.org) - Official Marlin documentation
- [Marlin Discord](https://discord.gg/n5NJ59y) - Discuss issues with Marlin users and developers
- Facebook Group ["Marlin Firmware"](https://www.facebook.com/groups/1049718498464482/)
- RepRap.org [Marlin Forum](https://forums.reprap.org/list.php?415)
- Facebook Group ["Marlin Firmware for 3D Printers"](https://www.facebook.com/groups/3Dtechtalk/)
- [Marlin Configuration](https://www.youtube.com/results?search_query=marlin+configuration) on YouTube

## Contributors

Marlin is constantly improving thanks to a huge number of contributors from all over the world bringing their specialties and talents. Huge thanks are due to [all the contributors](https://github.com/MarlinFirmware/Marlin/graphs/contributors) who regularly patch up bugs, help direct traffic, and basically keep Marlin from falling apart. Marlin's continued existence would not be possible without them.

## Administration

Regular users can open and close their own issues, but only the administrators can do project-related things like add labels, merge changes, set milestones, and kick trolls. The current Marlin admin team consists of:

 - Scott Lahteine [[@thinkyhead](https://github.com/thinkyhead)] - USA - Project Maintainer &nbsp; [💸 Donate](https://www.thinkyhead.com/donate-to-marlin)
 - Roxanne Neufeld [[@Roxy-3D](https://github.com/Roxy-3D)] - USA
 - Keith Bennett [[@thisiskeithb](https://github.com/thisiskeithb)] - USA &nbsp; [💸 Donate](https://github.com/sponsors/thisiskeithb)
 - Peter Ellens [[@ellensp](https://github.com/ellensp)] - New Zealand
 - Victor Oliveira [[@rhapsodyv](https://github.com/rhapsodyv)] - Brazil
 - Chris Pepper [[@p3p](https://github.com/p3p)] - UK
 - Jason Smith [[@sjasonsmith](https://github.com/sjasonsmith)] - USA
 - Luu Lac [[@shitcreek](https://github.com/shitcreek)] - USA
 - Bob Kuhn [[@Bob-the-Kuhn](https://github.com/Bob-the-Kuhn)] - USA
 - Erik van der Zalm [[@ErikZalm](https://github.com/ErikZalm)] - Netherlands &nbsp; [💸 Donate](https://flattr.com/submit/auto?user_id=ErikZalm&url=https://github.com/MarlinFirmware/Marlin&title=Marlin&language=&tags=github&category=software)

## License

Marlin is published under the [GPL license](/LICENSE) because we believe in open development. The GPL comes with both rights and obligations. Whether you use Marlin firmware as the driver for your open or closed-source product, you must keep Marlin open, and you must provide your compatible Marlin source code to end users upon request. The most straightforward way to comply with the Marlin license is to make a fork of Marlin on Github, perform your modifications, and direct users to your modified fork.

While we can't prevent the use of this code in products (3D printers, CNC, etc.) that are closed source or crippled by a patent, we would prefer that you choose another firmware or, better yet, make your own.
