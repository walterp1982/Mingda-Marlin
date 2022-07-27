/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file            : usb_host.c
  * @version         : v1.0_Cube
  * @brief           : This file implements the USB Host
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/

// #include "usb_host.h"
#include "usbh_core.h"
#include "usbh_msc.h"

/* USER CODE BEGIN Includes */
// #include "usb_printf.h"
// #include "ffconf.h"
#include "ff.h"
#include "ff_gen_drv.h"
// #include "usbh_diskio.h"
#include "explorer.h"
#include "log.h"
#include "fatfs.h"
/* USER CODE END Includes */

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USB Host core handle declaration */
USBH_HandleTypeDef hUsbHostFS;
// ApplicationTypeDef Appli_state = APPLICATION_IDLE;

/*
 * -- Insert your variables declaration here --
 */
/* USER CODE BEGIN 0 */
FATFS USBH_fatfs;
// char usbh_disk_path[4]; /* USB Host logical drive path */
BYTE working_buffer[FF_MAX_SS]; /* Work area (larger is better for process time) */
/* USER CODE END 0 */

/*
 * user callback declaration
 */
static void USBH_UserProcess(USBH_HandleTypeDef *phost, uint8_t id);

/*
 * -- Insert your external function declaration here --
 */
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/**
  * Init USB host library, add supported class and start the library
  * @retval None
  */
void MX_USB_HOST_Init(void)
{
  /* USER CODE BEGIN USB_HOST_Init_PreTreatment */
  
  /* USER CODE END USB_HOST_Init_PreTreatment */
  
  /* Init host Library, add supported class and start the library. */
  if (USBH_Init(&hUsbHostFS, USBH_UserProcess, HOST_HS) != USBH_OK)
  {
    Error_Handler();
  }
  if (USBH_RegisterClass(&hUsbHostFS, USBH_MSC_CLASS) != USBH_OK)
  {
    Error_Handler();
  }
  if (USBH_Start(&hUsbHostFS) != USBH_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USB_HOST_Init_PostTreatment */
  
  /* USER CODE END USB_HOST_Init_PostTreatment */
}

/*
 * Background task
 */
void MX_USB_HOST_Process(void)
{
  /* USB Host Background task */
  USBH_Process(&hUsbHostFS);
}
/*
 * user callback definition
 */
static void USBH_UserProcess  (USBH_HandleTypeDef *phost, uint8_t id)
{
  /* USER CODE BEGIN CALL_BACK_1 */
  switch(id)
  {
  case HOST_USER_SELECT_CONFIGURATION:
  // usb_printf("usb device select config. \r\n");
  break;

  case HOST_USER_DISCONNECTION:
  Appli_state = APPLICATION_DISCONNECT;
  if (f_mount(NULL, "", 0) != FR_OK) {
    // usb_printf("ERROR: deinit fatfs fail.");
    LOGI("mount deinit fail.");
  }
  FATFS_UnLinkDriver(USBHPath);
  
  // usb_printf("usb device disconnected. \r\n");
  break;

  case HOST_USER_CLASS_ACTIVE:
  Appli_state = APPLICATION_READY;
  // usb_printf("usb device active. \r\n");
  break;

  case HOST_USER_CONNECTION:
  Appli_state = APPLICATION_START;
  LOGI("usbh connect");
  // extern const Diskio_drvTypeDef  USBH_Driver;
  FATFS_LinkDriver(&USBH_Driver, USBHPath);
  LOGI("usbh udisk path:%s", USBHPath);
  // FRESULT res = f_mkfs(usbh_disk_path, FM_FAT32, 4096, 
  //                     working_buffer, 
  //                     sizeof(working_buffer));

  // LOGI("format fs result:%d", res);

  if (f_mount(&USBH_fatfs, "", 0) != FR_OK) {
    // usb_printf("ERROR: mount usb fatfs fail");
    LOGE("usbh connect mount fail");
  }

  //DONT just operate fs here!! usbh fsm should run to switch 
  //correct state!!!
  // extern FRESULT Explore_Disk(char *path, uint8_t recu_level);
  // Explore_Disk(USBHPath, 1);
  break;

  default:
  break;
  }
  /* USER CODE END CALL_BACK_1 */
}

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
