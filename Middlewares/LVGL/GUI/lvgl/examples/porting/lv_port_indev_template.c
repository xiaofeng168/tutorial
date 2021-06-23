/**
 ****************************************************************************************************
 * @file        lv_port_indev_template.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2020-03-29
 * @brief       LVGL 输入设备初始化
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
 
#include "lv_port_indev_template.h"
#include "./BSP/TOUCH/touch.h"


/*函数申明*/
static bool touchpad_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data);

/**
 * @brief       lvgl的输入设备初始化
 * @param       无
 * @retval      无
 */
void lv_port_indev_init(void)
{
    lv_indev_drv_t indev_drv;

    /*lvgl支持很多种输入设备,但是我们一般常用的就是触摸屏,也就是Touchpad*/
    lv_indev_drv_init(&indev_drv);         /* 初始化输入设备 */
    indev_drv.type = LV_INDEV_TYPE_POINTER;/* 输入设备的类型为指示器类型 */
    indev_drv.read_cb = touchpad_read;     /* 触摸回调函数 */
    lv_indev_drv_register(&indev_drv);     /* 输入设备的登记 */
}

/**
 * @brief       将会被lvgl周期性调用,周期值是通过lv_conf.h中的LV_INDEV_DEF_READ_PERIOD宏来定义的
                此值不要设置的太大,否则会感觉触摸不灵敏,默认值为30ms
 * @param       indev_drv : 输入的设备
 * @retval      data      : 输入设备的数据
 */
static bool touchpad_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data)
{
    static uint16_t last_x = 0;
    static uint16_t last_y = 0;

    if (tp_dev.sta & TP_PRES_DOWN) /*触摸按下了*/
    {
        last_x = tp_dev.x[0];
        last_y = tp_dev.y[0];
        data->point.x = last_x;
        data->point.y = last_y;
        data->state = LV_INDEV_STATE_PR;   /* 触摸按下时，为编辑模式 */
    }
    else  /*触摸松开了*/
    {
        data->point.x = last_x;
        data->point.y = last_y;
        data->state = LV_INDEV_STATE_REL;  /* 触摸释放时，为导航模式 */
    }

    /*返回false代表没有缓冲的数据*/
    return false;
}
