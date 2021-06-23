/**
 ****************************************************************************************************
 * @file        main.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2020-03-29
 * @brief       LVGL 矩阵按键 实验
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 Mini Pro H750开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 *
 ****************************************************************************************************
 */

#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./USMART/usmart.h"
#include "./BSP/MPU/mpu.h"
#include "./BSP/LED/led.h"
#include "./BSP/LCD/lcd.h"
#include "./BSP/KEY/key.h"
#include "./BSP/TOUCH/touch.h"
#include "./MALLOC/malloc.h"
#include "./BSP/TIMER/btim.h"
#include "lvgl.h"
#include "lv_port_indev_template.h"
#include "lv_port_disp_template.h"
#include "lv_demo_printer_theme.h"
#include "./BSP/SDMMC/sdmmc_sdcard.h"
#include "./FATFS/exfuns/exfuns.h"
#include "lv_port_fs.h"
#include "lv_mainstart.h"
#include "FreeRTOS.h"
#include "task.h"


/******************************************************************************************************/
/*FreeRTOS配置*/

/* START_TASK 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define START_TASK_PRIO   1            /* 任务优先级 */
#define START_STK_SIZE    128          /* 任务堆栈大小 */
TaskHandle_t StartTask_Handler;        /* 任务句柄 */
void start_task(void *pvParameters);   /* 任务函数 */

/* LV_DEMO_TASK 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define LV_DEMO_TASK_PRIO    3         /* 任务优先级 */
#define LV_DEMO_STK_SIZE     2048      /* 任务堆栈大小 */
TaskHandle_t LV_DEMOTask_Handler;      /* 任务句柄 */
void lv_demo_task(void *pvParameters); /* 任务函数 */

/* LED_TASK 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define LED_TASK_PRIO   4              /* 任务优先级 */
#define LED_STK_SIZE    128            /* 任务堆栈大小 */
TaskHandle_t LEDTask_Handler;          /* 任务句柄 */
void led_task(void *pvParameters);     /* 任务函数 */
/******************************************************************************************************/


int main(void)
{
    sys_cache_enable();                 /* 打开L1-Cache */
    HAL_Init();                         /* 初始化HAL库 */
    sys_stm32_clock_init(240, 2, 2, 4); /* 设置时钟, 480Mhz */
    delay_init(480);                    /* 延时初始化 */
    usart_init(115200);                 /* 串口初始化为115200 */
    mpu_memory_protection();            /* 保护相关存储区域 */
    led_init();                         /* 初始化LED */
    lcd_init();                         /* 初始化LCD */
    key_init();                         /* 初始化按键 */
    tp_dev.init();                      /* 触摸屏初始化 */
    btim_timx_int_init(10-1,24000-1);   /* 初始化定时器 */
    /* 注意：电阻屏如果发现显示屏XY镜像现象，需要坐标矫正，电容屏不需要矫正 */
    if ((tp_dev.touchtype & 0X80) == 0)
    {
        tp_adjust();
        tp_save_adjust_data();
    }

    lv_init();                /* lvgl系统初始化 */
    lv_port_disp_init();      /* lvgl显示接口初始化,放在lv_init()的后面 */
    lv_port_indev_init();     /* lvgl输入接口初始化,放在lv_init()的后面 */
    lv_port_fs_init();        /* 初始化lvgl文件系统 */

    while (sd_init()) /* 检测不到SD卡 */
    {
        lcd_show_string(30,90,200,16,16,"SD Card Failed!", RED);
        delay_ms(200);
        lcd_fill(30,90,200+30,70+16,WHITE);
        delay_ms(200);
    }
    
    xTaskCreate((TaskFunction_t )start_task,            /* 任务函数 */
                (const char*    )"start_task",          /* 任务名称 */
                (uint16_t       )START_STK_SIZE,        /* 任务堆栈大小 */
                (void*          )NULL,                  /* 传递给任务函数的参数 */
                (UBaseType_t    )START_TASK_PRIO,       /* 任务优先级 */
                (TaskHandle_t*  )&StartTask_Handler);   /* 任务句柄 */

    vTaskStartScheduler();                              /* 开启任务调度 */
    
    while(1);
}

/**
 * @brief       start_task
 * @param       pvParameters : 传入参数(未用到)
 * @retval      无
 */
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           /* 进入临界区 */

    /* 创建LVGL任务 */
    xTaskCreate((TaskFunction_t )lv_demo_task,
                (const char*    )"lv_demo_task",
                (uint16_t       )LV_DEMO_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )LV_DEMO_TASK_PRIO,
                (TaskHandle_t*  )&LV_DEMOTask_Handler);
    /* LED测试任务 */
    xTaskCreate((TaskFunction_t )led_task,
                (const char*    )"led_task",
                (uint16_t       )LED_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )LED_TASK_PRIO,
                (TaskHandle_t*  )&LEDTask_Handler);

    vTaskDelete(StartTask_Handler); /* 删除开始任务 */
    taskEXIT_CRITICAL();            /* 退出临界区 */
}

/**
 * @brief       lv_demo_task
 * @param       pvParameters : 传入参数(未用到)
 * @retval      无
 */
void lv_demo_task(void *pvParameters)
{
    pvParameters = pvParameters;  /* 防止编译器错误 */
    lv_mainstart();               /* 测试的demo */

    while(1)
    {
        tp_dev.scan(0);
        lv_task_handler();
    }
}

/**
 * @brief       led_task
 * @param       pvParameters : 传入参数(未用到)
 * @retval      无
 */
void led_task(void *pvParameters)
{
    pvParameters = pvParameters; /* 防止编译器错误 */

    while(1)
    {
        LED0_TOGGLE();
        vTaskDelay(1000);
    }
}