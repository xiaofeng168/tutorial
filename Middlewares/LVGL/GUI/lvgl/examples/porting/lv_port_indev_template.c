/**
 ****************************************************************************************************
 * @file        lv_port_indev_template.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2020-03-29
 * @brief       LVGL �����豸��ʼ��
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
 
#include "lv_port_indev_template.h"
#include "./BSP/TOUCH/touch.h"


/*��������*/
static bool touchpad_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data);

/**
 * @brief       lvgl�������豸��ʼ��
 * @param       ��
 * @retval      ��
 */
void lv_port_indev_init(void)
{
    lv_indev_drv_t indev_drv;

    /*lvgl֧�ֺܶ��������豸,��������һ�㳣�õľ��Ǵ�����,Ҳ����Touchpad*/
    lv_indev_drv_init(&indev_drv);         /* ��ʼ�������豸 */
    indev_drv.type = LV_INDEV_TYPE_POINTER;/* �����豸������Ϊָʾ������ */
    indev_drv.read_cb = touchpad_read;     /* �����ص����� */
    lv_indev_drv_register(&indev_drv);     /* �����豸�ĵǼ� */
}

/**
 * @brief       ���ᱻlvgl�����Ե���,����ֵ��ͨ��lv_conf.h�е�LV_INDEV_DEF_READ_PERIOD���������
                ��ֵ��Ҫ���õ�̫��,�����о�����������,Ĭ��ֵΪ30ms
 * @param       indev_drv : ������豸
 * @retval      data      : �����豸������
 */
static bool touchpad_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data)
{
    static uint16_t last_x = 0;
    static uint16_t last_y = 0;

    if (tp_dev.sta & TP_PRES_DOWN) /*����������*/
    {
        last_x = tp_dev.x[0];
        last_y = tp_dev.y[0];
        data->point.x = last_x;
        data->point.y = last_y;
        data->state = LV_INDEV_STATE_PR;   /* ��������ʱ��Ϊ�༭ģʽ */
    }
    else  /*�����ɿ���*/
    {
        data->point.x = last_x;
        data->point.y = last_y;
        data->state = LV_INDEV_STATE_REL;  /* �����ͷ�ʱ��Ϊ����ģʽ */
    }

    /*����false����û�л��������*/
    return false;
}
