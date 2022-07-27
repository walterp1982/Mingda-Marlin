#pragma once

#ifdef __cplusplus
extern "C"{
#endif

#include "ff.h"

FRESULT Explore_Disk(char *path, uint8_t recu_level);

#ifdef __cplusplus
}
#endif

