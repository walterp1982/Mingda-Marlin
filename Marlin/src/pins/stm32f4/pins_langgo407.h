/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#pragma once

/**
 * STM32F407VET6 with RAMPS-like shield
 * 'Black' STM32F407VET6 board - https://www.stm32duino.com/viewtopic.php?t=485
 * Shield - https://github.com/jmz52/Hardware
 */


#if NOT_TARGET(STM32F4, STM32F4xx)
  #error "Oops! Select an STM32F4 board in 'Tools > Board.'"
#elif HOTENDS > 2 || E_STEPPERS > 2
  #error "Black STM32F4VET6 supports up to 2 hotends / E-steppers."
#endif

#ifndef BOARD_INFO_NAME
  #define BOARD_INFO_NAME "BC.DZ.PC000007"

  #if ENABLED(ST32_SHIP)
    #define CHIP_NAME "(S)"
  #elif ENABLED(USE_GD32)
    #define CHIP_NAME "(G)"
  #endif

  #if ENABLED(R3_PRO)
    #define ROBOT_NAME "-MAX"
  #elif ENABLED(R4_PRO)
    #define ROBOT_NAME "-PRO"
  #else
    #define ROBOT_NAME ""
  #endif
#endif

#define DEFAULT_MACHINE_NAME "HP STM32F407VE"

//#define I2C_EEPROM
#if 0
    #define SRAM_EEPROM_EMULATION
    #define MARLIN_EEPROM_SIZE                0x2000  // 8KB
#else
    #define W25QXX_SPI_EEPROM
#endif
//
// Servos - THIS IS ABL PROBE
//
// #define SERVO0_PIN                          PC6
// #define SERVO1_PIN                          PC7
#define CALIB_PIN          PB1

//
// Z Probe must be this pins 
//
#define Z_MIN_PROBE_PIN    PB0

//
// Limit Switches
//
#if 0
  #define X_MAX_PIN                           PF12
#else
#define X_MIN_PIN                           PF12
// #define X_MAX_PIN                           PA15
#endif

#define Y_MIN_PIN                           PF14
// #define Y_MAX_PIN                           PD12

#ifdef Z_MIN_PROBE_USES_Z_MIN_ENDSTOP_PIN
  #define Z_MIN_PIN          PB1
#else
  #define Z_MIN_PIN          PG0
  // #define Z_MAX_PIN 
#endif
// #define Z_MIN_PIN                           PG0
// #define Z_MAX_PIN                           PD15

#define LED_RED_PIN               PG9

//
// Steppers
//
#define X_ENABLE_PIN       PD3
#define X_STEP_PIN         PD2
#define X_DIR_PIN          PC10

#define Y_ENABLE_PIN       PC12
#define Y_STEP_PIN         PA10
#define Y_DIR_PIN          PA9

#define Z_ENABLE_PIN       PC9
#define Z_STEP_PIN         PG8
#define Z_DIR_PIN          PG7

#define E0_ENABLE_PIN      PD13
#define E0_STEP_PIN        PD12
#define E0_DIR_PIN         PD11

#define Z2_ENABLE_PIN      PG5
#define Z2_STEP_PIN        PG4
#define Z2_DIR_PIN         PG3

/**
 * TMC2208/TMC2209 stepper drivers
 */
#if HAS_TMC220x
  //
  // Software serial ASYNC half duplex?
  //
  #define X_SERIAL_TX_PIN  PD6
  #define X_SERIAL_RX_PIN  PD6

  #define Y_SERIAL_TX_PIN  PC11
  #define Y_SERIAL_RX_PIN  PC11

  #define Z_SERIAL_TX_PIN  PA8
  #define Z_SERIAL_RX_PIN  PA8

  #define E0_SERIAL_TX_PIN PG2
  #define E0_SERIAL_RX_PIN PG2

  #define Z2_SERIAL_TX_PIN PG6
  #define Z2_SERIAL_RX_PIN PG6

  // Reduce baud rate to improve software serial reliability
  #define TMC_BAUD_RATE 19200
#endif

//
// Filament Sensor
//
#ifndef FIL_RUNOUT_PIN
  #define FIL_RUNOUT_PIN   PG1
#endif

//
// Power Supply Control
//
#ifndef PS_ON_PIN
  #define PS_ON_PIN        PA1
#endif

//
// Temperature Sensors
//
#define TEMP_0_PIN                          PC4   // T0
#define TEMP_BED_PIN                        PC5   // TB

#ifndef TEMP_CHAMBER_PIN
  #define TEMP_CHAMBER_PIN                  PC3   // TC
#endif

//
// Heaters / Fans
//
#define HEATER_0_PIN       PC6
#define HEATER_BED_PIN     PC8

#define FAN_PIN            PC7

#define STEP_TIMER_NUM        5  // Timer Index for Stepper
#define TEMP_TIMER_NUM        14  // Timer Index for Temperature

//
// USB connect control
//
#define USB_CONNECT_PIN    PA15
#define USB_CONNECT_INVERTING false

#define SD_DETECT_PIN      PE3
#define BEEPER_PIN         PG11
#define LED_PIN            PG9

#define NEOPIXEL_PIN       PB9


//
// Misc. Functions
//
// #define KILL_PIN                            PB1

//
// LCD / Controller
//
// #define FSMC_GRAPHICAL_TFT           //use offical tft lcd controller.?

// #if HAS_FSMC_TFT
//   /**
//    * Note: MKS Robin TFT screens use various TFT controllers
//    * Supported screens are based on the ILI9341, ST7789V and ILI9328 (320x240)
//    * ILI9488 is not supported
//    * Define init sequences for other screens in u8g_dev_tft_320x240_upscale_from_128x64.cpp
//    *
//    * If the screen stays white, disable 'LCD_RESET_PIN'
//    * to let the bootloader init the screen.
//    *
//    * Setting an 'LCD_RESET_PIN' may cause a flicker when entering the LCD menu
//    * because Marlin uses the reset as a failsafe to revive a glitchy LCD.
//    */
//   //#define LCD_RESET_PIN                   PF6
//   #define LCD_BACKLIGHT_PIN                 PG10
//   #define FSMC_CS_PIN                       PD7  // NE1
//   #define FSMC_RS_PIN                       PE2  // A23
//   #define TFT_CS_PIN                 FSMC_CS_PIN
//   #define TFT_RS_PIN                 FSMC_RS_PIN

//   #define LCD_USE_DMA_FSMC                        // Use DMA transfers to send data to the TFT
//   #define FSMC_DMA_DEV                      DMA2
//   #define FSMC_DMA_CHANNEL               DMA_CH5
// #endif

// #if NEED_TOUCH_PINS
//   #define TOUCH_CS_PIN                      PB1   // SPI2_NSS
//   #define TOUCH_SCK_PIN                     PB13  // SPI2_SCK
//   #define TOUCH_MISO_PIN                    PB14  // SPI2_MISO
//   #define TOUCH_MOSI_PIN                    PB15  // SPI2_MOSI
//   #define TOUCH_INT_PIN                     -1
// #endif

//#define SD_DETECT_PIN                     PC5
//#define SD_DETECT_PIN                     PA8   // SDIO SD_DETECT_PIN, external SDIO card reader only


/**
 * Note: Alfawise screens use various TFT controllers. Supported screens
 * are based on the ILI9341, ILI9328 and ST7798V. Define init sequences for
 * other screens in u8g_dev_tft_320x240_upscale_from_128x64.cpp
 *
 * If the screen stays white, disable 'LCD_RESET_PIN' to let the bootloader
 * init the screen.
 *
 * Setting an 'LCD_RESET_PIN' may cause a flicker when entering the LCD menu
 * because Marlin uses the reset as a failsafe to revive a glitchy LCD.
 */

#define LCD_BACKLIGHT_PIN   PG10
#define FSMC_CS_PIN         PD7   // FSMC_NE1
#define FSMC_RS_PIN         PE2   // A23 Register. Only one address needed
#define TFT_CS_PIN          FSMC_CS_PIN
#define TFT_RS_PIN          FSMC_RS_PIN
#define TFT_BACKLIGHT_PIN   LCD_BACKLIGHT_PIN

#define LCD_USE_DMA_FSMC   // Use DMA transfers to send data to the TFT
#define FSMC_DMA_DEV       DMA1
#define FSMC_DMA_CHANNEL   DMA_CH4  //fixme in F4

#if ENABLED(TOUCH_SCREEN)
  #define TOUCH_CS_PIN     PB6
  #define TOUCH_SCK_PIN    PB3
  #define TOUCH_MOSI_PIN   PB5
  #define TOUCH_MISO_PIN   PB4 
  #define TOUCH_INT_PIN    PB7 // PenIRQ coming from XPT2046
#endif

#define DOGLCD_MOSI        -1  // Prevent auto-define by Conditionals_post.h
#define DOGLCD_SCK         -1

// #define LCD_PINS_RS                         PE15
// #define LCD_PINS_ENABLE                     PD8
// #define LCD_PINS_D4                         PE10
// #define LCD_PINS_D5                         PE12
// #define LCD_PINS_D6                         PD1
// #define LCD_PINS_D7                         PE8
// #define BTN_ENC                             PD9
// #define BTN_EN1                             PD4
// #define BTN_EN2                             PD13



//
// Onboard SD support
//

#define HAS_ONBOARD_SD

#define SDIO_D0_PIN                         PA6
#define SDIO_D1_PIN                         PC9
#define SDIO_D2_PIN                         PC10
#define SDIO_D3_PIN                         PA4
#define SDIO_CK_PIN                         PA5
#define SDIO_CMD_PIN                        PA7

#ifndef SDCARD_CONNECTION
  #define SDCARD_CONNECTION              ONBOARD
#endif

#if SD_CONNECTION_IS(ONBOARD)
  // #define SDIO_SUPPORT                            // Use SDIO for onboard SD

  #ifndef SDIO_SUPPORT
    #define SOFTWARE_SPI                          // Use soft SPI for onboard SD
    #define SDSS                     SDIO_D3_PIN  //A4
    #define SCK_PIN                  SDIO_CK_PIN  //A5
    #define MISO_PIN                 SDIO_D0_PIN  //A6
    #define MOSI_PIN                SDIO_CMD_PIN  //A7
  #endif
#endif


//
// WIFI
//

/**
 *          -----
 *      TX | 1 2 | GND      Enable PG1   // Must be high for module to run
 *  Enable | 3 4 | GPIO2    Reset  PG0   // active low, probably OK to leave floating
 *   Reset | 5 6 | GPIO0    GPIO2  PF15  // must be high (ESP3D software configures this with a pullup so OK to leave as floating)
 *    3.3V | 7 8 | RX       GPIO0  PF14  // Leave as unused (ESP3D software configures this with a pullup so OK to leave as floating)
 *          -----
 *            W1
 */
#define ESP_WIFI_MODULE_COM                    6  // Must also set either SERIAL_PORT or SERIAL_PORT_2 to this
#define ESP_WIFI_MODULE_BAUDRATE        BAUDRATE  // Must use same BAUDRATE as SERIAL_PORT & SERIAL_PORT_2
#define ESP_WIFI_MODULE_RESET_PIN           PE5
#define ESP_WIFI_MODULE_ENABLE_PIN          PE4
#define ESP_WIFI_MODULE_GPIO0_PIN           ESP_WIFI_MODULE_ENABLE_PIN
// #define ESP_WIFI_MODULE_GPIO2_PIN           PF15

