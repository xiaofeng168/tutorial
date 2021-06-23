/**
 * @file lv_conf.h
 * Configuration file for v7.10.0-dev
 */

/*
 * COPY THIS FILE AS `lv_conf.h` NEXT TO the `lvgl` FOLDER
 */

#if 1 /*Set it to "1" to enable content*/

#ifndef LV_CONF_H
#define LV_CONF_H
/* clang-format off */

#include <stdint.h>





/*********************************************
  ��ʾ������
*********************************************/
/* LVGL֧�ֵ����ˮƽ�ʹ�ֱ�ֱ��� */
#define LV_HOR_RES_MAX          (800)
#define LV_VER_RES_MAX          (480)

/* ��ɫ���:
 * - 1:  1 byte per pixel
 * - 8:  RGB332
 * - 16: RGB565
 * - 32: ARGB8888
 */
#define LV_COLOR_DEPTH     16

/* ����2�ֽڵ�RGB565��ɫ.
 * �����ʾ��8λ�ӿ�(����SPI)*/
#define LV_COLOR_16_SWAP   0

/* 1: ������Ļ͸��.
 * ��OSD���������ص���gui������.
 * Ҫ��' LV_COLOR_DEPTH = 32 '��ɫ����Ļ����ʽӦ�ñ��޸�: `style.body.opa = ...`*/
#define LV_COLOR_SCREEN_TRANSP    0

/*ʹ�ô���ɫ��ͼ�����ز��ᱻ���� */
#define LV_COLOR_TRANSP    LV_COLOR_LIME         /*LV_COLOR_LIME: pure green*/

/* ���÷����(ֱ�ߺͰ뾶����ƽ��) */
#define LV_ANTIALIAS        1

/* Ĭ����ʾˢ������.
 * ��������ʾ��������(' lv_disp_drv_t ')�и���.*/
#define LV_DISP_DEF_REFR_PERIOD      30      /*[ms]*/

/* �ٽ������������� LV_DPI ��ֵ,Ĭ��ֵΪ 130,���ǰ������õ� 60,
   ��������������ڽ����� �ű�����,��ֵԽ��,�ؼ��ֲ��ľ�Խɢ,�ؼ�����ļ��Ҳ���� */
#define LV_DPI              60     /*[px]*/

/* ��ʾ��ʵ�ʿ�ȸı���һЩĬ��ֵ:Ĭ�϶����С��ʾ�����ֵȡ�*/
#define LV_DISP_SMALL_LIMIT  30
#define LV_DISP_MEDIUM_LIMIT 50
#define LV_DISP_LARGE_LIMIT  70

/* ����ṹ�� */
typedef int16_t lv_coord_t;





/*******************************************
   �ڴ����������
*******************************************/
/* LittelvGL���ڲ��ڴ���������ã�ͼ�ζ��������������ݴ洢������. */

/* 1: ʹ���Զ���malloc / free, 0: ��ʹ�����õ�' lv_mem_alloc '��' lv_mem_free ' */
#define LV_MEM_CUSTOM      0
#if LV_MEM_CUSTOM == 0
/* 1: ʹ���Զ���malloc / free, 0: ��ʹ�����õ�' lv_mem_alloc '��' lv_mem_free '
	ע�⣺�������2K*/
#  define LV_MEM_SIZE    (200U * 1024U)

/* �����������ı����� */
#  define LV_MEM_ATTR

/* Ϊ�ڴ������һ����ַ�������ǽ�����Ϊ������䣬Ҳ�������ⲿSRAM�� */
#  define LV_MEM_ADR          0

/* �Զ������ڴ���Ƭ */
#  define LV_MEM_AUTO_DEFRAG  1
#else       /*LV_MEM_CUSTOM*/
#  define LV_MEM_CUSTOM_INCLUDE <stdlib.h>   /*Header for the dynamic memory function*/
#  define LV_MEM_CUSTOM_ALLOC   malloc       /*Wrapper to malloc*/
#  define LV_MEM_CUSTOM_FREE    free         /*Wrapper to free*/
#endif     /*LV_MEM_CUSTOM*/

/* ʹ�ñ�׼��memcpy��memset����LVGL�Լ��ĺ�����
   ��׼�������ܸ�����Ҳ���ܸ��죬��ȡ�������ǵ�ʵ��. */
#define LV_MEMCPY_MEMSET_STD    0

/* ��lvgl�󶨵��߼����Բ����ڴ��ɸ����Թ���ʱʹ�� */
#define LV_ENABLE_GC 0
#if LV_ENABLE_GC != 0
#  define LV_GC_INCLUDE "gc.h"                           /*Include Garbage Collector related things*/
#  define LV_MEM_CUSTOM_REALLOC   your_realloc           /*Wrapper to realloc*/
#  define LV_MEM_CUSTOM_GET_SIZE  your_mem_get_size      /*Wrapper to lv_mem_get_size*/
#endif /* LV_ENABLE_GC */





/*****************************************
   �����豸������
******************************************/
/* �����豸Ĭ�����ã������������豸��������(' lv_indev_drv_t ')�и��� */

/* �����豸��ȡ����(�Ժ���Ϊ��λ) */
#define LV_INDEV_DEF_READ_PERIOD          30

/* ������Ϊ��λ�϶���ֵ */
#define LV_INDEV_DEF_DRAG_LIMIT           10

/* ��[%]���Ϸż��١������ֵ->����ļ��� */
#define LV_INDEV_DEF_DRAG_THROW           10

/* ������ʱ��(����)������' lv_event_long_pressed '��ʱ��) */
#define LV_INDEV_DEF_LONG_PRESS_TIME      400

/* �����ظ���������[ms]����LV_EVENT_LONG_PRESSED_REPEAT���ʱ�� */
#define LV_INDEV_DEF_LONG_PRESS_REP_TIME  100

/* ������ֵ(����) */
#define LV_INDEV_DEF_GESTURE_LIMIT        50

/* ����ǰ�ͷ�ʱ����С�ٶ�(����) */
#define LV_INDEV_DEF_GESTURE_MIN_VELOCITY 3





/*******************************************
 * ���ܵ�ʹ��
 *******************************************/
/*1: ʹ�ܶ��� */
#define LV_USE_ANIMATION        1
#if LV_USE_ANIMATION

/* �����������û����ݵ�����(�����ǡ�`void `�� `int`�� `struct`) */
typedef void * lv_anim_user_data_t;

#endif

/* 1: �ھ�����������Ӱ���� */
#define LV_USE_SHADOW           1
#if LV_USE_SHADOW
/* ������һЩ��Ӱ����
 * LV_SHADOW_CACHE_SIZE�����ֵ��������������Ӱ��С,
 * �������Ӱ��С�� `shadow_width + radius`
 * ������LV_SHADOW_CACHE_SIZE^2���ڴ濪�� */
#define LV_SHADOW_CACHE_SIZE    0
#endif

/*1: �����ھ����ϻ������� */
#define LV_USE_OUTLINE  1

/*1: �����ھ����ϻ���ͼ�� */
#define LV_USE_PATTERN  1

/*1: �����ھ����ϻ���ֵ�ַ��� */
#define LV_USE_VALUE_STR    1

/* 1: ʹ���������ģʽ����������ͨ(' LV_BLEND_MODE_��') */
#define LV_USE_BLEND_MODES      1

/* 1: ʹ��' opa_scale '��ʽ���Կ����������ö������Ӷ���Ĳ�͸���� */
#define LV_USE_OPA_SCALE        1

/* 1: ʹ��ͼ�����ź���ת */
#define LV_USE_IMG_TRANSFORM    1

/* 1: ���ö�����(���ڼ���/����������)  */
#define LV_USE_GROUP            1
#if LV_USE_GROUP
typedef void * lv_group_user_data_t;
#endif  /*LV_USE_GROUP*/




/* �����޸� LV_USE_GPU ��ֵ,Ĭ��ֵ�� 1,���ǰ�������Ϊ 0,����ʹ�� GPU ���� */
/* 1:����GPU�ӿ� */
#define LV_USE_GPU              0   /*Only enables `gpu_fill_cb` and `gpu_blend_cb` in the disp. drv- */
#define LV_USE_GPU_STM32_DMA2D  0
/*�������LV_USE_GPU_STM32_DMA2D, 
  LV_GPU_DMA2D_CMSIS_INCLUDE���붨��Ϊ����Ŀ�괦����CMSISͷ��·����
  ����:��stm32f769xx��h����stm32f429xx.h" */
#define LV_GPU_DMA2D_CMSIS_INCLUDE




/* �������NXPƽ̨�ģ����ǲ���Ҫ��� */
/*1: ��NXP RTxxxƽ̨��ʹ��PXP����CPUж�� */
#define LV_USE_GPU_NXP_PXP      0

/*1: Add default bare metal and FreeRTOS interrupt handling routines for PXP (lv_gpu_nxp_pxp_osa.c)
 *   and call lv_gpu_nxp_pxp_init() automatically during lv_init(). Note that symbol FSL_RTOS_FREE_RTOS
 *   has to be defined in order to use FreeRTOS OSA, otherwise bare-metal implementation is selected.
 *0: lv_gpu_nxp_pxp_init() has to be called manually before lv_init()
 * */
#define LV_USE_GPU_NXP_PXP_AUTO_INIT 0

/*1: ʹ��VG-Lite��NXP RTxxxƽ̨��ж��CPU */
#define LV_USE_GPU_NXP_VG_LITE   0




/* 1:�����ļ�ϵͳ(������Ҫͼ��) */
/* �޸�LV_USE_FILESYSTEM��ֵ,��Ĭ��ֵΪ1,���ǰ�������Ϊ0,����ʹ���ļ�ϵͳ �Ĺ��� */
#define LV_USE_FILESYSTEM       1
#define LV_USE_FS_IF            1
#define LV_FS_IF_FATFS          1
#if LV_USE_FILESYSTEM
/* �����ļ�ϵͳ����������û����ݵ�����(�����ǡ�`void `�� `int`�� `struct`) */
typedef void * lv_fs_drv_user_data_t;
#endif

/*1: ����������Ͷ�����ӡ�user_data�� */
#define LV_USE_USER_DATA        1

/*1: �����½���ʾCPUʹ���ʺ�FPS���� */
#define LV_USE_PERF_MONITOR     0

/* 1:������ܣ�ʹ�þ�API�еĺ���������  */
#define LV_USE_API_EXTENSION_V6  1
#define LV_USE_API_EXTENSION_V7  1





/************************************************
 * ͼ��������ͻ���
 *************************************************/
/* 1: ��������(��ɫ��)ͼ�� */
#define LV_IMG_CF_INDEXED       1

/* 1: ����alpha����ͼ�� */
#define LV_IMG_CF_ALPHA         1

/* Ĭ�ϵ�ͼ�񻺴��С��ͼ�񻺴汣��ͼ��򿪡�
   ���ֻʹ�����õ�ͼ���ʽ�����沢û�����������ơ�
   ���и���ͼ�������(��PNG��JPG)�Ļ�����Ա���ͼ���������/���롣
   ���Ǵ򿪵�ͼƬ���ܻ����Ķ�����ڴ档
   ����Ϊ0�Խ��û���*/
#define LV_IMG_CACHE_DEF_SIZE       1

/* ����ͼ����������û����ݵ�����(������:`void `�� `int`�� `struct`) */
typedef void * lv_img_decoder_user_data_t;





/*****************************************
 *  ����������
 ***************************************/
/* ��������Ϊ1�Ĵ��ϵͳ */
#define LV_BIG_ENDIAN_SYSTEM    0

/* Ϊ' lv_tick_inc '��������һ���Զ������� */
#define LV_ATTRIBUTE_TICK_INC

/* Ϊ' lv_task_handler '��������һ���Զ������� */
#define LV_ATTRIBUTE_TASK_HANDLER

/* Ϊ' lv_disp_flush_ready '��������һ���Զ������� */
#define LV_ATTRIBUTE_FLUSH_READY

/* ����������Ķ����С */
#define LV_ATTRIBUTE_MEM_ALIGN_SIZE

/* ʹ�ô�С�Ż�(-Os)�����������ܲ����������
   4��8�ֽڱ߽硣һЩHW����������Ҫ32��64�ֽڡ�
   �˶��뽫��ʽӦ������Ҫ�ĵط���
   LV_ATTRIBUTE_MEM_ALIGN_SIZEӦ������ָ������Ķ����С��
   ��__attribute__((����(LV_ATTRIBUTE_MEM_ALIGN_SIZE))) */
#define LV_ATTRIBUTE_MEM_ALIGN

/* ��������Ǵ��ͳ������������λͼ */
#define LV_ATTRIBUTE_LARGE_CONST

/* ǰ׺���ܹؼ������������Ƿ��õ�������ڴ���(��RAM) ʹ��15- 20kb������ڴ�*/
#define LV_ATTRIBUTE_FAST_MEM

/* Export integer�������󶨡�
   �ú���LV_<CONST>��ʽ�ĳ���һ��ʹ��
   Ӧ��Ҳ������lvgl��API��Micropython
   Ĭ��ֵֻ�Ƿ�ֹGCC���档
 */
#define LV_EXPORT_CONST_INT(int_value) struct _silence_gcc_warning

/* ��GPU���ٲ�����ʹ�õ�ǰ׺������ͨ����Ҫ������DMA�ɷ��ʵ�RAM���� */
#define LV_ATTRIBUTE_DMA






/*******************************************
 *  HAL ����
 *****************************************/
/* 1: ʹ���Զ�����Դ.
 * ��������ʹ�á�lv_tick_inc���ֶ�����tick����Ҫ) */
#define LV_TICK_CUSTOM     0
#if LV_TICK_CUSTOM == 1
#define LV_TICK_CUSTOM_INCLUDE  "Arduino.h"         /*ϵͳʱ�亯��ͷ�ļ�*/
#define LV_TICK_CUSTOM_SYS_TIME_EXPR (millis())     /*Expression evaluating to current system time in ms*/
#endif   /*LV_TICK_CUSTOM*/

typedef void * lv_disp_drv_user_data_t;             /*Type of user data in the display driver*/
typedef void * lv_indev_drv_user_data_t;            /*Type of user data in the input device driver*/






/********************************
 * ��־����
 ********************************/
/*1: ������־ģʽ*/
#define LV_USE_LOG      0
#if LV_USE_LOG
/* ������Ӷ���Ҫ����־:
 * LV_LOG_LEVEL_TRACE       A lot of logs to give detailed information
 * LV_LOG_LEVEL_INFO        Log important events
 * LV_LOG_LEVEL_WARN        Log if something unwanted happened but didn't cause a problem
 * LV_LOG_LEVEL_ERROR       Only critical issue, when the system may fail
 * LV_LOG_LEVEL_NONE        Do not log anything
 */
#  define LV_LOG_LEVEL    LV_LOG_LEVEL_WARN

/* 1: ʹ�á�printf����ӡ��־;
 * 0: �û���Ҫ�á�lv_log_register_print_cb��ע��ص����� */
#  define LV_LOG_PRINTF   0
#endif  /*LV_USE_LOG*/






/*******************************
 * ��������
 ******************************/
/* ���������Debug, LittelvGL����֤�����Ĳ��������������Ч���������ӡ������־��Ϣ
MCU�ڴ���ʱֹͣ��(' LV_USE_LOG 'Ӧ������)��������ڵ���MCU���������ͣ
���������Բ鿴�����ȷ��λ�á�����ͨ�����������¶�����Ե���Ϊ�����Ƕ��Ե���Ϊ��
����:#define LV_ASSERT_MEM(p) <my_assert_code>
*/
#define LV_USE_DEBUG        1
#if LV_USE_DEBUG

/* �������Ƿ�Ϊ�� */
#define LV_USE_ASSERT_NULL      1

/* ����ڴ��Ƿ����ɹ� */
#define LV_USE_ASSERT_MEM       1

/* ��ִ�йؼ���������' lv_mem '�������� */
#define LV_USE_ASSERT_MEM_INTEGRITY       0

/* ����ַ�����
   ����NULL�������ַ�������Ч�ַ��Ͳ���Ȼ���ظ�
   �������' LV_USE_ASSERT_NULL '����ִ�д��� */
#define LV_USE_ASSERT_STR       0

/* ���NULL����������ͺ��Ƿ����(����û�б�ɾ��)��
   �������' LV_USE_ASSERT_NULL '����ִ�д��� */
#define LV_USE_ASSERT_OBJ       0

/* �����ʽ�Ƿ���ȷ��ʼ�� */
#define LV_USE_ASSERT_STYLE     0

#endif /*LV_USE_DEBUG*/






/*==================
 *    ����ʹ��
 *===================*/
/* �����������ASCII��Χ��һЩÿ����4λ�ķ��š�
 * ���ſ���ͨ��' LV_SYMBOL_���Ķ���
 * ��������ĸ�����Ϣ: https://docs.lvgl.io/v7/en/html/overview/font.html
 * Ҫ���������壬��ת��: https://lvgl.com/ttf-font-to-c-array
 */

/* �������������壬bpp = 4
 * https://fonts.google.com/specimen/Montserrat  */
#define LV_FONT_MONTSERRAT_8     0
#define LV_FONT_MONTSERRAT_10    0
#define LV_FONT_MONTSERRAT_12    0
#define LV_FONT_MONTSERRAT_14    1
#define LV_FONT_MONTSERRAT_16    0
#define LV_FONT_MONTSERRAT_18    0
#define LV_FONT_MONTSERRAT_20    0
#define LV_FONT_MONTSERRAT_22    1
#define LV_FONT_MONTSERRAT_24    0
#define LV_FONT_MONTSERRAT_26    0
#define LV_FONT_MONTSERRAT_28    1
#define LV_FONT_MONTSERRAT_30    0
#define LV_FONT_MONTSERRAT_32    1
#define LV_FONT_MONTSERRAT_34    0
#define LV_FONT_MONTSERRAT_36    1
#define LV_FONT_MONTSERRAT_38    0
#define LV_FONT_MONTSERRAT_40    0
#define LV_FONT_MONTSERRAT_42    0
#define LV_FONT_MONTSERRAT_44    0
#define LV_FONT_MONTSERRAT_46    0
#define LV_FONT_MONTSERRAT_48    0

/* Demonstrate special features */
#define LV_FONT_MONTSERRAT_12_SUBPX      0
#define LV_FONT_MONTSERRAT_28_COMPRESSED 0  /*bpp = 3*/
#define LV_FONT_DEJAVU_16_PERSIAN_HEBREW 0  /*Hebrew, Arabic, PErisan letters and all their forms*/
#define LV_FONT_SIMSUN_16_CJK            0  /*1000 most common CJK radicals*/

/* ���������ĵ������������������ַ
 * http://pelulamu.net/unscii/ */
#define LV_FONT_UNSCII_8     0
#define LV_FONT_UNSCII_16     0

/* �������������������Զ������塣
   ��Ҳ����ʹ����Щ������ΪĬ������
   ���ǽ���ȫ�ַ�Χ���ṩ����
 * #define LV_FONT_CUSTOM_DECLARE LV_FONT_DECLARE(my_font_1) \
 *                                LV_FONT_DECLARE(my_font_2)
 */
#define LV_FONT_CUSTOM_DECLARE    LV_FONT_DECLARE(myFont24)\
                                  LV_FONT_DECLARE(myFont14)\
                                  LV_FONT_DECLARE(myFont36)\
                                  LV_FONT_DECLARE(myFont18)

/* ������кܶ��ַ������壬����������
   �����Ӻ����ʹ�С�������漰�����������
   ����> 10,000�ַ�����������⣬������Ҫ������.*/
#define LV_FONT_FMT_TXT_LARGE   0

/* ����/����֧��ѹ�����塣������ã�ѹ��
   ���Ų��ܱ��⴦�����ᱻ��Ⱦ.*/
#define LV_USE_FONT_COMPRESSED 1

/* ʹ��������Ⱦ */
#define LV_USE_FONT_SUBPX 1
#if LV_USE_FONT_SUBPX
/* ������ʾ������˳��.
 * ����ʹ�á�subpx���塱ʱʹ��.
 * ����normal���������޹ؽ�Ҫ.
 */
#define LV_FONT_SUBPX_BGR    0
#endif

/*����������û����ݵ�����(�����ǡ�`void `�� `int`�� `struct`)*/
typedef void * lv_font_user_data_t;





/************************************
 *  ����ʹ��
 *************************************/
/* ����ʹ��һ������ */

/* û�����⣬���Ը�����ҪӦ�������ʽ
   û�б�־λ������LV_THEME_DEFAULT_FLAG 0 */
#define LV_USE_THEME_EMPTY       1

/* �򵥵ش����������������
   û�б�־λ������LV_THEME_DEFAULT_FLAG 0 */
#define LV_USE_THEME_TEMPLATE    1

/* һ�����ٵ�����..
 * Flags:
 * LV_THEME_MATERIAL_FLAG_LIGHT: light theme
 * LV_THEME_MATERIAL_FLAG_DARK: dark theme
 * LV_THEME_MATERIAL_FLAG_NO_TRANSITION: disable transitions (state change animations)
 * LV_THEME_MATERIAL_FLAG_NO_FOCUS: disable indication of focused state)
 * */
#define LV_USE_THEME_MATERIAL    1

/* Mono-color theme for monochrome displays.
 * If LV_THEME_DEFAULT_COLOR_PRIMARY is LV_COLOR_BLACK the
 * texts and borders will be black and the background will be
 * white. Else the colors are inverted.
 * No flags. Set LV_THEME_DEFAULT_FLAG 0 */
#define LV_USE_THEME_MONO        1

#define LV_THEME_DEFAULT_INCLUDE            <stdint.h>      /*Include a header for the init. function*/
#define LV_THEME_DEFAULT_INIT               lv_theme_material_init
#define LV_THEME_DEFAULT_COLOR_PRIMARY      lv_color_hex(0x01a2b1)
#define LV_THEME_DEFAULT_COLOR_SECONDARY    lv_color_hex(0x44d1b6)
#define LV_THEME_DEFAULT_FLAG               LV_THEME_MATERIAL_FLAG_LIGHT
#define LV_THEME_DEFAULT_FONT_SMALL         &lv_font_montserrat_14
#define LV_THEME_DEFAULT_FONT_NORMAL        &lv_font_montserrat_14
#define LV_THEME_DEFAULT_FONT_SUBTITLE      &lv_font_montserrat_14
#define LV_THEME_DEFAULT_FONT_TITLE         &lv_font_montserrat_14





/**********************************
 *  �ı�����
 ***********************************/
/* Ϊ�ַ���ѡ���ַ�����.
 * IDE��༭��Ӧ�þ�����ͬ���ַ�����
 * 1. - LV_TXT_ENC_UTF8
 * 2. - LV_TXT_ENC_ASCII
 * */
#define LV_TXT_ENC LV_TXT_ENC_UTF8

/* ��������Щ�ַ��ϻ����ı� */
#define LV_TXT_BREAK_CHARS                  " ,.;:-_"

/* ���һ��������������ô�����ͻ��ڡ���Ư�����ĵط��Ͽ���Ҫ���ã�����Ϊֵ<= 0 */
#define LV_TXT_LINE_BREAK_LONG_LEN          0

/* ��һ�������У��ڻ���ǰһ��������ʹ�õ���С�ַ�����ȡ����LV_TXT_LINE_BREAK_LONG_LEN. */
#define LV_TXT_LINE_BREAK_LONG_PRE_MIN_LEN  3

/* ��һ�������У��ڻ��к���һ���з��õ���С�ַ���.ȡ����LV_TXT_LINE_BREAK_LONG_LEN. */
#define LV_TXT_LINE_BREAK_LONG_POST_MIN_LEN 3

/* ����֪ͨ�ı�������ɫ�Ŀ����ַ� */
#define LV_TXT_COLOR_CMD "#"

/* ֧��˫���ı�.
 * �����ϴ����Һʹ��ҵ�����ı�.
 * �÷��򽫸���Unicode˫���㷨���д���:
 * https://www.w3.org/International/articles/inline-bidi-markup/uba-basics*/
#define LV_USE_BIDI     0
#if LV_USE_BIDI
/* ����Ĭ�Ϸ���֧�ֵ�ֵ:
 * 1.`LV_BIDI_DIR_LTR` Left-to-Right ����
 * 2.`LV_BIDI_DIR_RTL` Right-to-Left �ҵ���
 * 3.`LV_BIDI_DIR_AUTO` ����ı��������� */
#define LV_BIDI_BASE_DIR_DEF  LV_BIDI_DIR_AUTO
#endif

/* ֧�ְ�������/��˹���Դ�������Щ�����У��ַ�Ӧ�滻Ϊ��һ����ʽ�������������е�λ�� */
#define LV_USE_ARABIC_PERSIAN_CHARS 0

/* �������õ�(v)snprintf���� */
#define LV_SPRINTF_CUSTOM   0
#if LV_SPRINTF_CUSTOM
#  define LV_SPRINTF_INCLUDE <stdio.h>
#  define lv_snprintf     snprintf
#  define lv_vsnprintf    vsnprintf
#else   /*!LV_SPRINTF_CUSTOM*/
#  define LV_SPRINTF_DISABLE_FLOAT 1
#endif  /*LV_SPRINTF_CUSTOM*/





/*********************************
 *  LVGL��������
 **********************************/
#if LV_USE_USER_DATA
/*����������û����ݵ�����(�����ǡ�`void `�� `int`�� `struct`)*/
typedef void * lv_obj_user_data_t;
/*�ṩһ���������ͷ��û�����*/
#define LV_USE_USER_DATA_FREE 0
#if LV_USE_USER_DATA_FREE
#  define LV_USER_DATA_FREE_INCLUDE  "something.h"  /*Header for user data free function*/
/* Function prototype : void user_data_free(lv_obj_t * obj); */
#  define LV_USER_DATA_FREE  (user_data_free)       /*Invoking for user data free function*/
#endif
#endif

/* 1:����' lv_obj_realign() '����' lv_obj_align() '���� */
#define LV_USE_OBJ_REALIGN          1

/* ʹ��������ڸ���������ϵ��.
 * LV_EXT_CLICK_AREA_OFF ��0:���ø�����
 * LV_EXT_CLICK_AREA_TINY: ������������ˮƽ�ʹ�ֱ����(0..255 px)
 * LV_EXT_CLICK_AREA_FULL: ������������������4�������Ͻ��е���(-32k..+ 32 k px)
 */
#define LV_USE_EXT_CLICK_AREA  LV_EXT_CLICK_AREA_TINY

/************************************
 *  LVGL������÷�
 *************************************/
/*
 * ���ڶ������͵��ĵ�: https://docs.lvgl.com/#Object-types
 */

/*Arc (dependencies: -)*/
#define LV_USE_ARC      1

/*Bar (dependencies: -)*/
#define LV_USE_BAR      1

/*Button (dependencies: lv_cont*/
#define LV_USE_BTN      1

/*Button matrix (dependencies: -)*/
#define LV_USE_BTNMATRIX     1

/*Calendar (dependencies: -)*/
#define LV_USE_CALENDAR 1
#if LV_USE_CALENDAR
#  define LV_CALENDAR_WEEK_STARTS_MONDAY    0
#endif

/*Canvas (dependencies: lv_img)*/
#define LV_USE_CANVAS   1

/*Check box (dependencies: lv_btn, lv_label)*/
#define LV_USE_CHECKBOX       1

/*Chart (dependencies: -)*/
#define LV_USE_CHART    1
#if LV_USE_CHART
#  define LV_CHART_AXIS_TICK_LABEL_MAX_LEN    256
#endif

/*Container (dependencies: -*/
#define LV_USE_CONT     1

/*Color picker (dependencies: -*/
#define LV_USE_CPICKER   1

/*Drop down list (dependencies: lv_page, lv_label, lv_symbol_def.h)*/
#define LV_USE_DROPDOWN    1
#if LV_USE_DROPDOWN != 0
/*Open and close default animation time [ms] (0: no animation)*/
#  define LV_DROPDOWN_DEF_ANIM_TIME     200
#endif

/*Gauge (dependencies:lv_bar, lv_linemeter)*/
#define LV_USE_GAUGE    1

/*Image (dependencies: lv_label*/
#define LV_USE_IMG      1

/*Image Button (dependencies: lv_btn*/
#define LV_USE_IMGBTN   1
#if LV_USE_IMGBTN
/*1: The imgbtn requires left, mid and right parts and the width can be set freely*/
#  define LV_IMGBTN_TILED 0
#endif

/*Keyboard (dependencies: lv_btnm)*/
#define LV_USE_KEYBOARD       1

/*Label (dependencies: -*/
#define LV_USE_LABEL    1
#if LV_USE_LABEL != 0
/*Hor, or ver. scroll speed [px/sec] in 'LV_LABEL_LONG_ROLL/ROLL_CIRC' mode*/
#  define LV_LABEL_DEF_SCROLL_SPEED       25

/* Waiting period at beginning/end of animation cycle */
#  define LV_LABEL_WAIT_CHAR_COUNT        3

/*Enable selecting text of the label */
#  define LV_LABEL_TEXT_SEL               0

/*Store extra some info in labels (12 bytes) to speed up drawing of very long texts*/
#  define LV_LABEL_LONG_TXT_HINT          0
#endif

/*LED (dependencies: -)*/
#define LV_USE_LED      1
#if LV_USE_LED
#  define LV_LED_BRIGHT_MIN  120      /*Minimal brightness*/
#  define LV_LED_BRIGHT_MAX  255     /*Maximal brightness*/
#endif

/*Line (dependencies: -*/
#define LV_USE_LINE     1

/*List (dependencies: lv_page, lv_btn, lv_label, (lv_img optionally for icons ))*/
#define LV_USE_LIST     1
#if LV_USE_LIST != 0
/*Default animation time of focusing to a list element [ms] (0: no animation)  */
#  define LV_LIST_DEF_ANIM_TIME  100
#endif

/*Line meter (dependencies: *;)*/
#define LV_USE_LINEMETER   1
#if LV_USE_LINEMETER
/* Draw line more precisely at cost of performance.
 * Useful if there are lot of lines any minor are visible
 * 0: No extra precision
 * 1: Some extra precision
 * 2: Best precision
 */
#  define LV_LINEMETER_PRECISE    1
#endif

/*Mask (dependencies: -)*/
#define LV_USE_OBJMASK  1

/*Message box (dependencies: lv_rect, lv_btnm, lv_label)*/
#define LV_USE_MSGBOX     1

/*Page (dependencies: lv_cont)*/
#define LV_USE_PAGE     1
#if LV_USE_PAGE != 0
/*Focus default animation time [ms] (0: no animation)*/
#  define LV_PAGE_DEF_ANIM_TIME     400
#endif

/*Preload (dependencies: lv_arc, lv_anim)*/
#define LV_USE_SPINNER      1
#if LV_USE_SPINNER != 0
#  define LV_SPINNER_DEF_ARC_LENGTH   60      /*[deg]*/
#  define LV_SPINNER_DEF_SPIN_TIME    1000    /*[ms]*/
#  define LV_SPINNER_DEF_ANIM         LV_SPINNER_TYPE_SPINNING_ARC
#endif

/*Roller (dependencies: lv_ddlist)*/
#define LV_USE_ROLLER    1
#if LV_USE_ROLLER != 0
/*Focus animation time [ms] (0: no animation)*/
#  define LV_ROLLER_DEF_ANIM_TIME     200

/*Number of extra "pages" when the roller is infinite*/
#  define LV_ROLLER_INF_PAGES         7
#endif

/*Slider (dependencies: lv_bar)*/
#define LV_USE_SLIDER    1

/*Spinbox (dependencies: lv_ta)*/
#define LV_USE_SPINBOX       1

/*Switch (dependencies: lv_slider)*/
#define LV_USE_SWITCH       1

/*Text area (dependencies: lv_label, lv_page)*/
#define LV_USE_TEXTAREA       1
#if LV_USE_TEXTAREA != 0
#  define LV_TEXTAREA_DEF_CURSOR_BLINK_TIME 400     /*ms*/
#  define LV_TEXTAREA_DEF_PWD_SHOW_TIME     1500    /*ms*/
#endif

/*Table (dependencies: lv_label)*/
#define LV_USE_TABLE    1
#if LV_USE_TABLE
#  define LV_TABLE_COL_MAX    12
#  define LV_TABLE_CELL_STYLE_CNT 4
#endif

/*Tab (dependencies: lv_page, lv_btnm)*/
#define LV_USE_TABVIEW      1
#  if LV_USE_TABVIEW != 0
/*Time of slide animation [ms] (0: no animation)*/
#  define LV_TABVIEW_DEF_ANIM_TIME    300
#endif

/*Tileview (dependencies: lv_page) */
#define LV_USE_TILEVIEW     1
#if LV_USE_TILEVIEW
/*Time of slide animation [ms] (0: no animation)*/
#  define LV_TILEVIEW_DEF_ANIM_TIME   300
#endif

/*Window (dependencies: lv_cont, lv_btn, lv_label, lv_img, lv_page)*/
#define LV_USE_WIN      1





/************************************
 * ���û�����
 *************************************/
#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)    /* Disable warnings for Visual Studio*/
#  define _CRT_SECURE_NO_WARNINGS
#endif

/*--END OF LV_CONF_H--*/

#endif /*LV_CONF_H*/

#endif /*End of "Content enable"*/
