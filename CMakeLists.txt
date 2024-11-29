# 项目名称
project(QUADROTOR C ASM)

set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")
set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")

# 输出 compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
SET(CMAKE_SYSTEM_NAME μCos)
SET(CMAKE_SYSTEM μCos)
SET(CMAKE_SYSTEM_PROCESSOR arm)
SET(CMAKE_CROSSCOMPILING 1)

set(publish_bin_debug 			${LED_SOURCE_DIR}/obj/bin)
set(publish_bin_release 		${LED_SOURCE_DIR}/obj/bin)

set(publish_lib_debug  			${LED_SOURCE_DIR}/obj/lib)
set(publish_lib_release 		${LED_SOURCE_DIR}/obj/lib)

# 指定可执行程序输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG 	${publish_bin_debug})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE 	${publish_bin_release})

# 指定 库文件输出目录
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG 	${publish_lib_debug})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RLEASE 	${publish_lib_release})

# 添加启动文件
set(STARTUP_FILE ${CMAKE_CURRENT_SOURCE_DIR}/Libraries/CMSIS/Device/ST/STM32F4xx/Source/Templates/arm/startup_stm32f401xx.s)
set(OS_CPU_A_FILE ${CMAKE_CURRENT_SOURCE_DIR}/Ports/os_cpu_a.asm)
set(SEGGER_RTT_ASM_FILE ${CMAKE_CURRENT_SOURCE_DIR}/Trace/Systemview_src/SEGGER_RTT_ASM_ARMv7M.S)

# 添加链接器脚本文件
set(LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/STM32F401RE_FLASH.ld)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Cfg)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/RTE/_Project_New)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Ports)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Source/inc)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Libraries/STM32F4xx_StdPeriph_Driver/inc)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Software/Inc)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/System/Inc)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Trace/Systemview_con)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Trace/Systemview_src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/User/Inc)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Hardware/Inc)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Libraries/CMSIS/Include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Libraries/CMSIS/Device/ST/STM32F4xx/Include)

# 添加源文件
file(GLOB SOURCE_FILES 
    "${CMAKE_CURRENT_SOURCE_DIR}/Cfg/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/Hardware/Src/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/Libraries/CMSIS/Device/ST/STM32F4xx/Source/Templates/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/Libraries/STM32F4xx_StdPeriph_Driver/src/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/Ports/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/Ports/*.asm"
    "${CMAKE_CURRENT_SOURCE_DIR}/Software/Src/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/src/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/inc/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/System/Src/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/Trace/Systemview_con/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/Trace/Systemview_src/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/User/Src/*.c")
set(SOURCE_FILES ${SOURCE_FILES})
# AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/STM32F40x_FWLib/src SOURCE_LIB)

# 设置编译器和链接器
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)                                                        
set(CMAKE_LINKER arm-none-eabi-ld)

# 增加宏定义
add_definitions(-DSTM32F401xx -DUSE_STDPERIPH_DRIVER)

# 设置编译选项
set(CMAKE_C_FLAGS "-mthumb -mcpu=cortex-m4 -std=c99 -g -Os \
    -ffunction-sections -fdata-sections -Wall -Wextra \
    -Wno-self-assign -Wno-unused-parameter -Wno-empty-body")
# set(CMAKE_CXX_FLAGS "-mthumb -mcpu=cortex-m4 -std=gnu++17 -g -Os -ffunction-sections -fdata-sections -Wall -Wextra")
set(CMAKE_ASM_FLAGS "-mthumb -mcpu=cortex-m4 -g -x assembler-with-cpp")

# 设置链接选项
set(CMAKE_EXE_LINKER_FLAGS "-mthumb -mabi=aapcs -mcpu=cortex-m4 \
    --specs=nano.specs --specs=nosys.specs -u _printf_float -u _scanf_float \
    -Wl,--gc-sections -Wl,-Map=${PROJECT_NAME}.map -T${LINKER_SCRIPT}")

# 添加可执行文件
add_executable(${PROJECT_NAME}.elf ${SOURCE_FILES} ${STARTUP_FILE} ${OS_CPU_A_FILE} ${SEGGER_RTT_ASM_FILE})
target_link_libraries(${PROJECT_NAME}.elf -Wl,-Map=${PROJECT_NAME}.map -Wl,--gc-sections -Wl,--print-memory-usage -Wl,-lc -lm)

# 添加目标文件
add_custom_target(${PROJECT_NAME}.bin ALL DEPENDS ${PROJECT_NAME}.elf COMMAND arm-none-eabi-objcopy -Obinary ${PROJECT_NAME}.elf ${PROJECT_NAME}.bin)
# 烧录程序
add_custom_target(flash COMMAND st-flash --reset write ${PROJECT_NAME}.bin 0x08000000 WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
# openocd 烧录
# add_custom_target(openocd COMMAND openocd -f ${CMAKE_CURRENT_SOURCE_DIR}/Misc/STM32F401RE.cfg -c init -c "reset halt" -c "flash write_image erase ${PROJECT_NAME}.bin 0x08000000" -c "reset" -c shutdown WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

# ST-Link/DAPLink 烧录
add_custom_target(openocd COMMAND openocd -f interface/stlink-v2.cfg -f target/stm32f4x.cfg -c init -c "reset halt" -c "flash write_image erase ${PROJECT_NAME}.bin 0x08000000" -c "reset" -c shutdown WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})