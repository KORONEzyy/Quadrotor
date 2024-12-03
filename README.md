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
│   └── Src  
├── Libraries  
│   ├── CMSIS	//STM32内核相关  
│   │   ├── Device  
│   │   └── Include  
│   └── STM32F4xx_StdPeriph_Driver 	//STM32标准库  
│       ├── inc  
│       └── src  
├── log.md  
├── Ports //μCos移植接口  
├── RTE  
│   └── _Project_New  
│       └── RTE_Components.h  
├── Software // 软件驱动  
│   ├── Inc  
│   └── Src  
├── Source//μCos内核  
│   ├── inc  
│   └── src  
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
