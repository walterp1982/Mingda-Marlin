#ifndef __UDISKP_
#define __UDISKP_

#include "../inc/MarlinConfig.h"

#ifdef HAS_UDISK

#include "ff.h"
class UdiskPrint{

public:
    static bool eof(void){return psize>=fsize;}
    static int get(TCHAR *rbuf, int len, FIL *fp){
        res = f_gets_p(rbuf, len, fp);
        psize += res;
        return res;
    }
    static uint64_t getPrintSize(){return psize;}
    static uint64_t getFileSize(){return fsize;}
    static void init(){fsize=0;psize=0;res=0;}
    static void fileHasFinish(FIL *fp);
    static bool isUdiskPrint(){return udiskFlag;}
    static void startUdiskPrint(uint64_t _size);
    static void resumeUdiskPrint(uint64_t _size, uint32_t _psize, millis_t resume_time);
    static void setFileSize(uint64_t _size){}
    static uint8_t getPrintProgress(void);
    static void abortUdiskPrint(FIL *fp);
  #ifdef USB_CONNECT_PIN
    static void InitUdiskPin(){OUT_WRITE(USB_CONNECT_PIN,HIGH);}
    static void ResetUdisk(){WRITE(USB_CONNECT_PIN, LOW); delay(10); WRITE(USB_CONNECT_PIN, HIGH);}
  #endif
private:
    static uint64_t fsize;  // 文件大小
    static uint64_t psize;  // 已经读取到的文件大小
    static int res;    // 存放读取到的文件大小
    // static bool noRead = false;
    static bool udiskFlag;
};

extern UdiskPrint udisk;

#endif

#endif //__UDISKP
