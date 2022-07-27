#include "udiskPrint.h"

#ifdef HAS_UDISK

#include "../module/planner.h"
#include "../module/printcounter.h"


UdiskPrint udisk;
uint64_t UdiskPrint::fsize;  // 文件大小
uint64_t UdiskPrint::psize;  // 已经读取到的文件大小
    int  UdiskPrint::res;    // 存放读取到的文件大小
    bool UdiskPrint::udiskFlag;

void UdiskPrint::fileHasFinish(FIL *fp){
    planner.synchronize();    // 等待队列完成
    f_close(fp);              // 关闭U盘文件
    udiskFlag = false;

    marlin_state = MF_SD_COMPLETE;
}
void UdiskPrint::abortUdiskPrint(FIL *fp){
    udiskFlag = false;
    f_close(fp);              // 关闭U盘文件
}
void UdiskPrint::startUdiskPrint(uint64_t _size){
    udiskFlag = true;psize=0;res=0;fsize = _size;
    
    print_job_timer.start();
}
void UdiskPrint::resumeUdiskPrint(uint64_t _size, uint32_t _psize, millis_t resume_time){
    udiskFlag = true;psize=_psize;res=0;fsize = _size;
    
    print_job_timer.resume(resume_time);
}
uint8_t UdiskPrint::getPrintProgress(void){
    return psize*100/fsize;
}

#endif
