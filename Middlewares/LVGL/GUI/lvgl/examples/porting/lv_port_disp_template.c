/**
 ****************************************************************************************************
 * @file        lv_port_disp_template.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2020-03-29
 * @brief       LVGL ��ʾ�ӿڳ�ʼ��
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
 
#include "lv_port_disp_template.h"
#include "./BSP/LCD/lcd.h"
#include "./SYSTEM/sys/sys.h"
#include "./MALLOC/malloc.h"


#define USE_SRAM  0 /* ʹ���ⲿsramΪ1������Ϊ0 */

#if USE_SRAM
/* �������� */
#define COLOR_BUF_SIZE   (LV_HOR_RES_MAX*LV_VER_RES_MAX)     /* ȫ���Ĵ�С */
#define color_buf         mymalloc(SRAMEX,COLOR_BUF_SIZE)
#else
#define COLOR_BUF_SIZE   (LV_HOR_RES_MAX*20)       /* ���û�����ɫ�ڴ��С */
static lv_color_t        color_buf[COLOR_BUF_SIZE];/* ��ʾ������,��̬��sram */  
#endif


/* �������� */
static void disp_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p);
#if LV_USE_GPU
static void gpu_blend(lv_color_t * dest, const lv_color_t * src, uint32_t length, lv_opa_t opa);
static void gpu_fill(lv_color_t * dest, uint32_t length, lv_color_t color);
#endif

/**
 * @brief       lvgl��ʾ�ӿڳ�ʼ��
 * @param       ��
 * @retval      ��
 */
void lv_port_disp_init(void)
{ 
    static lv_disp_buf_t disp_buf;

    /* ��ʾ��������ʼ�� */
    lv_disp_buf_init(&disp_buf, color_buf, NULL,COLOR_BUF_SIZE); 

    /* ��ʾ����Ĭ��ֵ��ʼ�� */ 
    lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);

    /* ������Ļ����ʾ��С,��������Ϊ��֧������ԭ�ӵĶ����Ļ,���ö�̬��ȡ�ķ�ʽ */
    /* �����������ʵ����Ŀ�Ļ�,���Բ�������,��ô��Ĭ��ֵ����lv_conf.h��LV_HOR_RES_MAX��LV_VER_RES_MAX�궨���ֵ */
    disp_drv.hor_res = lcddev.width;
    disp_drv.ver_res = lcddev.height;

    /* ע����ʾ�����ص� */
    disp_drv.flush_cb = disp_flush;

    /* ע����ʾ������ */
    disp_drv.buffer = &disp_buf;

#if LV_USE_GPU
    /* ��ѡ��,ֻҪ��ʹ�õ�GPUʱ,����Ҫʵ��gpu_blend��gpu_fill�ӿ� */

    /* ʹ��͸���Ȼ��������ɫ����ʱ��Ҫ�õ�gpu_blend�ӿ� */
    disp_drv.gpu_blend = gpu_blend;

    /* ��һ����ɫ���һ���ڴ�����ʱ��Ҫ�õ�gpu_fill�ӿ� */
    disp_drv.gpu_fill = gpu_fill;
#endif

    /* ע����ʾ������lvgl�� */
    lv_disp_drv_register(&disp_drv);
}

/**
 * @brief       ��ָ���������ʾ����������д�뵽��Ļ��,�����ʹ��DMA����������Ӳ���������ں�̨ȥ����������
                ���������֮��,�����õ���lv_disp_flush_ready()
 * @param       disp_drv : ��ʾ����
 * @param       area     : ��ʾ����
 * @param       color_p  ����ʾ��ɫ
 * @retval      ��
 */
static void disp_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p)
{
    /*��ָ���������ʾ����������д�뵽��Ļ*/
    lcd_color_fill(area->x1,area->y1,area->x2,area->y2,(uint16_t*)color_p);
    /*������õ���,֪ͨlvgl�����Ѿ�flushing���������*/
    lv_disp_flush_ready(disp_drv);
}

/* ��ѡ�� */
#if LV_USE_GPU

/* ������MCU��Ӳ��������(GPU)����ô�����ʹ�ò�͸����������ڴ�
   ֻ���ڻ���ģʽ��ʹ��(LV_VDB_SIZE != 0��lv_conf.h)*/
static void gpu_blend(lv_disp_drv_t * disp_drv, lv_color_t * dest, const lv_color_t * src, uint32_t length, lv_opa_t opa)
{
    /* ���GPU��ɵ�ʾ������ */
    uint32_t i;
    for(i = 0; i < length; i++) {
        dest[i] = lv_color_mix(dest[i], src[i], opa);
    }
}

/* ������MCU��Ӳ��������(GPU)����ô���������������ڴ����ɫ
   ֻ���ڻ���ģʽ��ʹ��(LV_VDB_SIZE != 0��lv_conf.h)*/
static void gpu_fill_cb(lv_disp_drv_t * disp_drv, 
                        lv_color_t * dest_buf, 
                        lv_coord_t dest_width,
                        const lv_area_t * fill_area, 
                        lv_color_t color);
{
    /* ���GPU��ɵ�ʾ������ */
    uint32_t x, y;
    dest_buf += dest_width * fill_area->y1; /*Go to the first line*/

    for(y = fill_area->y1; y < fill_area->y2; y++)
    {
        for(x = fill_area->x1; x < fill_area->x2; x++)
        {
            dest_buf[x] = color;
        }
        
        dest_buf+=dest_width;    /*Go to the next line*/
    }
}

#endif
