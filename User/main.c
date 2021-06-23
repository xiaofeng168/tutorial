/**
 ****************************************************************************************************
 * @file        main.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2020-03-29
 * @brief       LVGL ���󰴼� ʵ��
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� Mini Pro H750������
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
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
/*FreeRTOS����*/

/* START_TASK ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define START_TASK_PRIO   1            /* �������ȼ� */
#define START_STK_SIZE    128          /* �����ջ��С */
TaskHandle_t StartTask_Handler;        /* ������ */
void start_task(void *pvParameters);   /* ������ */

/* LV_DEMO_TASK ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define LV_DEMO_TASK_PRIO    3         /* �������ȼ� */
#define LV_DEMO_STK_SIZE     2048      /* �����ջ��С */
TaskHandle_t LV_DEMOTask_Handler;      /* ������ */
void lv_demo_task(void *pvParameters); /* ������ */

/* LED_TASK ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define LED_TASK_PRIO   4              /* �������ȼ� */
#define LED_STK_SIZE    128            /* �����ջ��С */
TaskHandle_t LEDTask_Handler;          /* ������ */
void led_task(void *pvParameters);     /* ������ */
/******************************************************************************************************/


int main(void)
{
    sys_cache_enable();                 /* ��L1-Cache */
    HAL_Init();                         /* ��ʼ��HAL�� */
    sys_stm32_clock_init(240, 2, 2, 4); /* ����ʱ��, 480Mhz */
    delay_init(480);                    /* ��ʱ��ʼ�� */
    usart_init(115200);                 /* ���ڳ�ʼ��Ϊ115200 */
    mpu_memory_protection();            /* ������ش洢���� */
    led_init();                         /* ��ʼ��LED */
    lcd_init();                         /* ��ʼ��LCD */
    key_init();                         /* ��ʼ������ */
    tp_dev.init();                      /* ��������ʼ�� */
    btim_timx_int_init(10-1,24000-1);   /* ��ʼ����ʱ�� */
    /* ע�⣺���������������ʾ��XY����������Ҫ�������������������Ҫ���� */
    if ((tp_dev.touchtype & 0X80) == 0)
    {
        tp_adjust();
        tp_save_adjust_data();
    }

    lv_init();                /* lvglϵͳ��ʼ�� */
    lv_port_disp_init();      /* lvgl��ʾ�ӿڳ�ʼ��,����lv_init()�ĺ��� */
    lv_port_indev_init();     /* lvgl����ӿڳ�ʼ��,����lv_init()�ĺ��� */
    lv_port_fs_init();        /* ��ʼ��lvgl�ļ�ϵͳ */

    while (sd_init()) /* ��ⲻ��SD�� */
    {
        lcd_show_string(30,90,200,16,16,"SD Card Failed!", RED);
        delay_ms(200);
        lcd_fill(30,90,200+30,70+16,WHITE);
        delay_ms(200);
    }
    
    xTaskCreate((TaskFunction_t )start_task,            /* ������ */
                (const char*    )"start_task",          /* �������� */
                (uint16_t       )START_STK_SIZE,        /* �����ջ��С */
                (void*          )NULL,                  /* ���ݸ��������Ĳ��� */
                (UBaseType_t    )START_TASK_PRIO,       /* �������ȼ� */
                (TaskHandle_t*  )&StartTask_Handler);   /* ������ */

    vTaskStartScheduler();                              /* ����������� */
    
    while(1);
}

/**
 * @brief       start_task
 * @param       pvParameters : �������(δ�õ�)
 * @retval      ��
 */
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           /* �����ٽ��� */

    /* ����LVGL���� */
    xTaskCreate((TaskFunction_t )lv_demo_task,
                (const char*    )"lv_demo_task",
                (uint16_t       )LV_DEMO_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )LV_DEMO_TASK_PRIO,
                (TaskHandle_t*  )&LV_DEMOTask_Handler);
    /* LED�������� */
    xTaskCreate((TaskFunction_t )led_task,
                (const char*    )"led_task",
                (uint16_t       )LED_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )LED_TASK_PRIO,
                (TaskHandle_t*  )&LEDTask_Handler);

    vTaskDelete(StartTask_Handler); /* ɾ����ʼ���� */
    taskEXIT_CRITICAL();            /* �˳��ٽ��� */
}

/**
 * @brief       lv_demo_task
 * @param       pvParameters : �������(δ�õ�)
 * @retval      ��
 */
void lv_demo_task(void *pvParameters)
{
    pvParameters = pvParameters;  /* ��ֹ���������� */
    lv_mainstart();               /* ���Ե�demo */

    while(1)
    {
        tp_dev.scan(0);
        lv_task_handler();
    }
}

/**
 * @brief       led_task
 * @param       pvParameters : �������(δ�õ�)
 * @retval      ��
 */
void led_task(void *pvParameters)
{
    pvParameters = pvParameters; /* ��ֹ���������� */

    while(1)
    {
        LED0_TOGGLE();
        vTaskDelay(1000);
    }
}