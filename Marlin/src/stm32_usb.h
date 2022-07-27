#pragma once

#ifdef __cplusplus
extern "C"{
#endif

void MX_USB_HOST_Init(void);
void MX_USB_HOST_Process(void);
void MSC_MenuProcess(void);
short mountUdisk(void);
short umountUdisk(void);

extern uint8_t udiskMounted;

#ifdef __cplusplus
}
#endif