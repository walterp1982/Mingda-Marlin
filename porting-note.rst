#####################
porting note.
#####################


todo list
~~~~~~~~~~~~~~~~~~~~~

1. gpio led                 pass
#. gpio key xmin ymin zmin  pass
#. uart                     pass
#. stepper                  pass @ 1mm 10mm move test.
#. spi flash w25qxx         
#. spi sd card reader       pass
#. touch screen xpt2042     pass
#. fsmc lcd display.        pass. use other ui so far.
#. dma 
#. timer pwm buzzer
#. temp sensor              fix max/min temp bug.
#. runout sensor


gpio. f4
~~~~~~~~~~~~~~~~~~~~~


compatible with st parts:
    verified: gpio R/W


use EXTUI-example so far
~~~~~~~~~~~~~~~~~~~~~

123123
~~~~~~~~~~~~~~~~~~~~~


如何通过串口打印debug信息？？
~~~~~~~~~~~~~~~~~~~~~

也许是通过serial == host 之间这个来输出？


code excute sequence
###############################

main -- C:\Users\langgo\.platformio\packages\framework-arduinoststm32\cores\arduino\main.cpp
serialx.begin -- Marlin\src\HAL\STM32\MarlinSerial.h
hw-serial.begin -- C:\Users\langgo\.platformio\packages\framework-arduinoststm32\cores\arduino\HardwareSerial.cpp
uart_init -- C:\Users\langgo\.platformio\packages\framework-arduinoststm32\libraries\SrcWrapper\src\stm32\uart.c
HAL_Init setTimerIntPrio -- C:\Users\langgo\.platformio\packages\framework-arduinoststm32\libraries\SoftwareSerial\src\SoftwareSerial.cpp
tmc_stepper_swserial -- .pio\libdeps\langgo407ve\TMCStepper\src\source\TMC2208Stepper.cpp   setup param for init purpose.
sw_serial -- C:\Users\langgo\.platformio\packages\framework-arduinoststm32\libraries\SoftwareSerial\src\SoftwareSerial.cpp


stepper timer tim5
-------------------------------
tim cfg ARR = 2000. psc = 42,  timck = 84mhz
it freq = 84mhz / 42  = 2mhz  / 2k = 1khz


fsmc tft driver 
-------------------------------
1. fsmc rs cs backlight other data pins define & init.
#. fsmc clock init.
#. lcd rw via fsmc like lcd->ram = xxxx;
#. When debugging today, under the condition of the same code, there is a problem that some memory will suddenly change to 0 in breakpoint debugging. It does not reproduce after power off and restart. Domestic MCUs are sometimes really pitted.。。。

bed level data
-------------------------------------
Bilinear Leveling Grid:
      0      1      2      3
 0 -1.082 -1.332 -1.700 -1.922
 1 -1.090 -1.408 -1.658 -1.905
 2 -1.127 -1.400 -1.652 -1.892
 3 -1.158 -1.441 -1.709 -1.926


Did you compile the app that supports BL?
-------------------------------------
 build_flags       = ${common_stm32.build_flags}
  -DVECT_TAB_OFFSET=0x0     //无loader，调试测试方便 
                            //并且; buildroot/share/PlatformIO/scripts/stm32_bootloader.py
  -DVECT_TAB_OFFSET=0x10000 //支持bootloader




usb serial 原装的CDC QUEUE解读笔记
-------------------------------------------

1. First, initialize two ring queues respectively. For tx and rx respectively
2. The difference between the read pair column and the write queue is that the read can read r==w, that is, the subscripts are equal, but the write can be written to w=r-1 at most, otherwise the first byte will be lost
3. When joining the team, it will be discussed in two cases according to the size of the team (current to the end and whether there is a period after the circle)
4. When reading, call readBlock to read as much as possible, but the read pointer rp has not been processed yet.
5. readblock returns the buffer address and size, you need to read it by yourself memcpy.
6. After reading, call commitRead to move rp, and the read operation is completed.
7. ########Receive queue rx queue ########
8. The dequeue reads 1byte. peek can only read but not queue
9. rx queue read is still divided into 2 cases, the maximum can be read but not circle
10. Use rxq_resvBlock to prepare the location before receiving data. The point is to find a location large enough to put the maxpack size
11. After receiving, still use commintBlock to update the write location