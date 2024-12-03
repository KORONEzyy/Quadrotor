# Quadrotor for Linux 
&emsp;&emsp;基于STM32的四轴无人机项目  

## tips

- <a href="https://github.com/asIsinkintowest/Drone" target="_blank">Keil工程版本</a>
- tips:此项目为电子科技大学软院综合课程设计的项目，目前并未完善，并非一个完整的合格的四轴无人机驱动程序。————2024/11/29.

## 目录结构

.
├── build
├── Cfg
│   ├── app_cfg.h
│   ├── app_hooks.c
│   └── os_cfg.h
├── CMakeLists.txt
├── Hardware //硬件驱动
│   ├── Inc
│   │   ├── GY86.h
│   │   ├── HMC5883LAdress.h
│   │   ├── HMC5883L.h
│   │   ├── Motor.h
│   │   ├── MPU6050Adress.h
│   │   ├── MPU.h
│   │   ├── MyI2C.h
│   │   ├── OLED_Font.h
│   │   ├── OLED.h
│   │   ├── Receiver.h
│   │   └── Serial.h
│   └── Src
│       ├── GY86.c
│       ├── HMC5883L.c
│       ├── Motor.c
│       ├── MPU.c
│       ├── MyI2C.c
│       ├── OLED.c
│       ├── Receiver.c
│       └── Serial.c
├── Libraries
│   ├── CMSIS	//STM32内核相关
│   │   ├── Device
│   │   │   └── ST
│   │   │       └── STM32F4xx
│   │   │           ├── Include
│   │   │           └── Source
│   │   │               └── Templates
│   │   │                   ├── arm
│   │   │                   │   └── startup_stm32f401xx.s
│   │   │                   └── system_stm32f4xx.c
│   │   └── Include
│   └── STM32F4xx_StdPeriph_Driver 	//STM32标准库
│       ├── inc
│       └── src
├── log.md
├── Ports //μCos移植接口
│   ├── os_core.h
│   ├── os_cpu_a.asm
│   ├── os_cpu_c.c
│   └── os_cpu.h
├── RTE
│   └── _Project_New
│       └── RTE_Components.h
├── Software // 软件驱动
│   ├── Inc
│   │   ├── Acc.h
│   │   ├── Anto.h
│   │   ├── Madgwick.h
│   │   ├── PID.h
│   │   ├── PWM.h
│   │   ├── Quaternion.h
│   │   └── USART.h
│   └── Src
│       ├── Acc.c
│       ├── Anto.c
│       ├── Madgwick.c
│       ├── PID.c
│       ├── PWM.c
│       ├── Quaternion.c
│       └── USART.c
├── Source//μCos内核
│   ├── inc
│   │   ├── os.h
│   │   ├── os_trace.h
│   │   └── ucos_ii.h
│   └── src
│       ├── os_core.c
│       ├── os_dbg_r.c
│       ├── os_flag.c
│       ├── os_mbox.c
│       ├── os_mem.c
│       ├── os_mutex.c
│       ├── os_q.c
│       ├── os_sem.c
│       ├── os_task.c
│       ├── os_time.c
│       └── os_tmr.c
├── STM32F401RE_FLASH.ld
├── System
│   ├── Inc
│   │   └── Delay.h
│   └── Src
│       └── Delay.c
├── Trace //Systemview
│   ├── Systemview_con
│   └── Systemview_src
└── User
    ├── Inc
    │   ├── stm32f4xx_conf.h
    │   └── stm32f4xx_it.h
    └── Src
        ├── main .c
        └── stm32f4xx_it.c
