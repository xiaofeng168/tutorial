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
  显示屏设置
*********************************************/
/* LVGL支持的最大水平和垂直分辨率 */
#define LV_HOR_RES_MAX          (800)
#define LV_VER_RES_MAX          (480)

/* 颜色深度:
 * - 1:  1 byte per pixel
 * - 8:  RGB332
 * - 16: RGB565
 * - 32: ARGB8888
 */
#define LV_COLOR_DEPTH     16

/* 互换2字节的RGB565颜色.
 * 如果显示有8位接口(例如SPI)*/
#define LV_COLOR_16_SWAP   0

/* 1: 启用屏幕透明.
 * 对OSD或其他有重叠的gui很有用.
 * 要求' LV_COLOR_DEPTH = 32 '颜色和屏幕的样式应该被修改: `style.body.opa = ...`*/
#define LV_COLOR_SCREEN_TRANSP    0

/*使用此颜色的图像像素不会被绘制 */
#define LV_COLOR_TRANSP    LV_COLOR_LIME         /*LV_COLOR_LIME: pure green*/

/* 启用反锯齿(直线和半径将被平滑) */
#define LV_ANTIALIAS        1

/* 默认显示刷新周期.
 * 可以在显示驱动程序(' lv_disp_drv_t ')中更改.*/
#define LV_DISP_DEF_REFR_PERIOD      30      /*[ms]*/

/* 再接着我们来设置 LV_DPI 的值,默认值为 130,我们把他设置到 60,
   这个宏是用来调节界面缩 放比例的,此值越大,控件分布的就越散,控件自身的间隔也会变大 */
#define LV_DPI              60     /*[px]*/

/* 显示的实际宽度改变了一些默认值:默认对象大小、示例布局等。*/
#define LV_DISP_SMALL_LIMIT  30
#define LV_DISP_MEDIUM_LIMIT 50
#define LV_DISP_LARGE_LIMIT  70

/* 坐标结构体 */
typedef int16_t lv_coord_t;





/*******************************************
   内存管理器设置
*******************************************/
/* LittelvGL的内部内存管理器设置，图形对象和其他相关数据存储在这里. */

/* 1: 使用自定义malloc / free, 0: ：使用内置的' lv_mem_alloc '和' lv_mem_free ' */
#define LV_MEM_CUSTOM      0
#if LV_MEM_CUSTOM == 0
/* 1: 使用自定义malloc / free, 0: ：使用内置的' lv_mem_alloc '和' lv_mem_free '
	注意：必须大于2K*/
#  define LV_MEM_SIZE    (200U * 1024U)

/* 大数组声明的编译器 */
#  define LV_MEM_ATTR

/* 为内存池设置一个地址，而不是将其作为数组分配，也可以在外部SRAM中 */
#  define LV_MEM_ADR          0

/* 自动整理内存碎片 */
#  define LV_MEM_AUTO_DEFRAG  1
#else       /*LV_MEM_CUSTOM*/
#  define LV_MEM_CUSTOM_INCLUDE <stdlib.h>   /*Header for the dynamic memory function*/
#  define LV_MEM_CUSTOM_ALLOC   malloc       /*Wrapper to malloc*/
#  define LV_MEM_CUSTOM_FREE    free         /*Wrapper to free*/
#endif     /*LV_MEM_CUSTOM*/

/* 使用标准的memcpy和memset代替LVGL自己的函数。
   标准函数可能更慢，也可能更快，这取决于它们的实现. */
#define LV_MEMCPY_MEMSET_STD    0

/* 当lvgl绑定到高级语言并且内存由该语言管理时使用 */
#define LV_ENABLE_GC 0
#if LV_ENABLE_GC != 0
#  define LV_GC_INCLUDE "gc.h"                           /*Include Garbage Collector related things*/
#  define LV_MEM_CUSTOM_REALLOC   your_realloc           /*Wrapper to realloc*/
#  define LV_MEM_CUSTOM_GET_SIZE  your_mem_get_size      /*Wrapper to lv_mem_get_size*/
#endif /* LV_ENABLE_GC */





/*****************************************
   输入设备的设置
******************************************/
/* 输入设备默认设置，可以在输入设备驱动程序(' lv_indev_drv_t ')中更改 */

/* 输入设备读取周期(以毫秒为单位) */
#define LV_INDEV_DEF_READ_PERIOD          30

/* 以像素为单位拖动阈值 */
#define LV_INDEV_DEF_DRAG_LIMIT           10

/* 在[%]中拖放减速。更大的值->更快的减速 */
#define LV_INDEV_DEF_DRAG_THROW           10

/* 长按下时间(毫秒)，发送' lv_event_long_pressed '的时间) */
#define LV_INDEV_DEF_LONG_PRESS_TIME      400

/* 长按重复触发周期[ms]，“LV_EVENT_LONG_PRESSED_REPEAT间隔时间 */
#define LV_INDEV_DEF_LONG_PRESS_REP_TIME  100

/* 手势阈值(像素) */
#define LV_INDEV_DEF_GESTURE_LIMIT        50

/* 滑动前释放时的最小速度(像素) */
#define LV_INDEV_DEF_GESTURE_MIN_VELOCITY 3





/*******************************************
 * 功能的使用
 *******************************************/
/*1: 使能动画 */
#define LV_USE_ANIMATION        1
#if LV_USE_ANIMATION

/* 声明动画的用户数据的类型(可以是。`void `， `int`， `struct`) */
typedef void * lv_anim_user_data_t;

#endif

/* 1: 在矩形上启用阴影绘制 */
#define LV_USE_SHADOW           1
#if LV_USE_SHADOW
/* 允许缓冲一些阴影计算
 * LV_SHADOW_CACHE_SIZE是最大值。到缓冲区的阴影大小,
 * 这里的阴影大小是 `shadow_width + radius`
 * 缓存有LV_SHADOW_CACHE_SIZE^2的内存开销 */
#define LV_SHADOW_CACHE_SIZE    0
#endif

/*1: 允许在矩形上绘制轮廓 */
#define LV_USE_OUTLINE  1

/*1: 允许在矩形上绘制图案 */
#define LV_USE_PATTERN  1

/*1: 启用在矩形上绘制值字符串 */
#define LV_USE_VALUE_STR    1

/* 1: 使用其他混合模式，而不是普通(' LV_BLEND_MODE_…') */
#define LV_USE_BLEND_MODES      1

/* 1: 使用' opa_scale '样式属性可以立即设置对象及其子对象的不透明度 */
#define LV_USE_OPA_SCALE        1

/* 1: 使用图像缩放和旋转 */
#define LV_USE_IMG_TRANSFORM    1

/* 1: 启用对象组(用于键盘/编码器导航)  */
#define LV_USE_GROUP            1
#if LV_USE_GROUP
typedef void * lv_group_user_data_t;
#endif  /*LV_USE_GROUP*/




/* 接着修改 LV_USE_GPU 的值,默认值是 1,我们把它设置为 0,即不使能 GPU 功能 */
/* 1:开启GPU接口 */
#define LV_USE_GPU              0   /*Only enables `gpu_fill_cb` and `gpu_blend_cb` in the disp. drv- */
#define LV_USE_GPU_STM32_DMA2D  0
/*如果启用LV_USE_GPU_STM32_DMA2D, 
  LV_GPU_DMA2D_CMSIS_INCLUDE必须定义为包含目标处理器CMSIS头的路径，
  例如:“stm32f769xx。h”或“stm32f429xx.h" */
#define LV_GPU_DMA2D_CMSIS_INCLUDE




/* 下面关于NXP平台的，我们不需要理会 */
/*1: 在NXP RTxxx平台上使用PXP进行CPU卸载 */
#define LV_USE_GPU_NXP_PXP      0

/*1: Add default bare metal and FreeRTOS interrupt handling routines for PXP (lv_gpu_nxp_pxp_osa.c)
 *   and call lv_gpu_nxp_pxp_init() automatically during lv_init(). Note that symbol FSL_RTOS_FREE_RTOS
 *   has to be defined in order to use FreeRTOS OSA, otherwise bare-metal implementation is selected.
 *0: lv_gpu_nxp_pxp_init() has to be called manually before lv_init()
 * */
#define LV_USE_GPU_NXP_PXP_AUTO_INIT 0

/*1: 使用VG-Lite在NXP RTxxx平台上卸载CPU */
#define LV_USE_GPU_NXP_VG_LITE   0




/* 1:启用文件系统(可能需要图像) */
/* 修改LV_USE_FILESYSTEM的值,其默认值为1,我们把他设置为0,即不使能文件系统 的功能 */
#define LV_USE_FILESYSTEM       1
#define LV_USE_FS_IF            1
#define LV_FS_IF_FATFS          1
#if LV_USE_FILESYSTEM
/* 声明文件系统驱动程序的用户数据的类型(可以是。`void `， `int`， `struct`) */
typedef void * lv_fs_drv_user_data_t;
#endif

/*1: 向驱动程序和对象添加“user_data” */
#define LV_USE_USER_DATA        1

/*1: 在右下角显示CPU使用率和FPS计数 */
#define LV_USE_PERF_MONITOR     0

/* 1:如果可能，使用旧API中的函数和类型  */
#define LV_USE_API_EXTENSION_V6  1
#define LV_USE_API_EXTENSION_V7  1





/************************************************
 * 图像解码器和缓存
 *************************************************/
/* 1: 启用索引(调色板)图像 */
#define LV_IMG_CF_INDEXED       1

/* 1: 启用alpha索引图像 */
#define LV_IMG_CF_ALPHA         1

/* 默认的图像缓存大小。图像缓存保持图像打开。
   如果只使用内置的图像格式，缓存并没有真正的优势。
   具有复杂图像解码器(如PNG或JPG)的缓存可以保存图像的连续打开/解码。
   但是打开的图片可能会消耗额外的内存。
   设置为0以禁用缓存*/
#define LV_IMG_CACHE_DEF_SIZE       1

/* 声明图像解码器的用户数据的类型(可以是:`void `， `int`， `struct`) */
typedef void * lv_img_decoder_user_data_t;





/*****************************************
 *  编译器设置
 ***************************************/
/* 对于设置为1的大端系统 */
#define LV_BIG_ENDIAN_SYSTEM    0

/* 为' lv_tick_inc '函数定义一个自定义属性 */
#define LV_ATTRIBUTE_TICK_INC

/* 为' lv_task_handler '函数定义一个自定义属性 */
#define LV_ATTRIBUTE_TASK_HANDLER

/* 为' lv_disp_flush_ready '函数定义一个自定义属性 */
#define LV_ATTRIBUTE_FLUSH_READY

/* 缓冲区所需的对齐大小 */
#define LV_ATTRIBUTE_MEM_ALIGN_SIZE

/* 使用大小优化(-Os)，编译器可能不会对齐数据
   4或8字节边界。一些HW可能甚至需要32或64字节。
   此对齐将显式应用在需要的地方。
   LV_ATTRIBUTE_MEM_ALIGN_SIZE应该用于指定所需的对齐大小。
   如__attribute__((对齐(LV_ATTRIBUTE_MEM_ALIGN_SIZE))) */
#define LV_ATTRIBUTE_MEM_ALIGN

/* 属性来标记大型常量数组字体的位图 */
#define LV_ATTRIBUTE_LARGE_CONST

/* 前缀性能关键函数，将它们放置到更快的内存中(如RAM) 使用15- 20kb额外的内存*/
#define LV_ATTRIBUTE_FAST_MEM

/* Export integer常量到绑定。
   该宏与LV_<CONST>形式的常量一起使用
   应该也出现在lvgl绑定API如Micropython
   默认值只是防止GCC警告。
 */
#define LV_EXPORT_CONST_INT(int_value) struct _silence_gcc_warning

/* 在GPU加速操作中使用的前缀变量，通常需要放置在DMA可访问的RAM段中 */
#define LV_ATTRIBUTE_DMA






/*******************************************
 *  HAL 设置
 *****************************************/
/* 1: 使用自定义标记源.
 * 它消除了使用“lv_tick_inc”手动更新tick的需要) */
#define LV_TICK_CUSTOM     0
#if LV_TICK_CUSTOM == 1
#define LV_TICK_CUSTOM_INCLUDE  "Arduino.h"         /*系统时间函数头文件*/
#define LV_TICK_CUSTOM_SYS_TIME_EXPR (millis())     /*Expression evaluating to current system time in ms*/
#endif   /*LV_TICK_CUSTOM*/

typedef void * lv_disp_drv_user_data_t;             /*Type of user data in the display driver*/
typedef void * lv_indev_drv_user_data_t;            /*Type of user data in the input device driver*/






/********************************
 * 日志设置
 ********************************/
/*1: 开启日志模式*/
#define LV_USE_LOG      0
#if LV_USE_LOG
/* 可以添加多重要的日志:
 * LV_LOG_LEVEL_TRACE       A lot of logs to give detailed information
 * LV_LOG_LEVEL_INFO        Log important events
 * LV_LOG_LEVEL_WARN        Log if something unwanted happened but didn't cause a problem
 * LV_LOG_LEVEL_ERROR       Only critical issue, when the system may fail
 * LV_LOG_LEVEL_NONE        Do not log anything
 */
#  define LV_LOG_LEVEL    LV_LOG_LEVEL_WARN

/* 1: 使用“printf”打印日志;
 * 0: 用户需要用“lv_log_register_print_cb”注册回调函数 */
#  define LV_LOG_PRINTF   0
#endif  /*LV_USE_LOG*/






/*******************************
 * 调试设置
 ******************************/
/* 如果启用了Debug, LittelvGL将验证函数的参数。如果发现无效参数，则打印错误日志信息
MCU在错误时停止。(' LV_USE_LOG '应该启用)如果你正在调试MCU，你可以暂停
调试器，以查看问题的确切位置。可以通过在这里重新定义断言的行为来覆盖断言的行为。
例如:#define LV_ASSERT_MEM(p) <my_assert_code>
*/
#define LV_USE_DEBUG        1
#if LV_USE_DEBUG

/* 检查参数是否为空 */
#define LV_USE_ASSERT_NULL      1

/* 检查内存是否分配成功 */
#define LV_USE_ASSERT_MEM       1

/* 在执行关键操作后检查' lv_mem '的完整性 */
#define LV_USE_ASSERT_MEM_INTEGRITY       0

/* 检查字符串。
   搜索NULL、超长字符串、无效字符和不自然的重复
   如果禁用' LV_USE_ASSERT_NULL '将被执行代替 */
#define LV_USE_ASSERT_STR       0

/* 检查NULL，对象的类型和是否存在(例如没有被删除)。
   如果禁用' LV_USE_ASSERT_NULL '将被执行代替 */
#define LV_USE_ASSERT_OBJ       0

/* 检查样式是否正确初始化 */
#define LV_USE_ASSERT_STYLE     0

#endif /*LV_USE_DEBUG*/






/*==================
 *    字体使用
 *===================*/
/* 内置字体包含ASCII范围和一些每像素4位的符号。
 * 符号可以通过' LV_SYMBOL_…的定义
 * 关于字体的更多信息: https://docs.lvgl.io/v7/en/html/overview/font.html
 * 要创建新字体，请转到: https://lvgl.com/ttf-font-to-c-array
 */

/* 蒙特塞拉特字体，bpp = 4
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

/* 像素完美的单间距字体请打开下面的网址
 * http://pelulamu.net/unscii/ */
#define LV_FONT_UNSCII_8     0
#define LV_FONT_UNSCII_16     0

/* 可以在这里声明您的自定义字体。
   你也可以使用这些字体作为默认字体
   它们将在全局范围内提供。如
 * #define LV_FONT_CUSTOM_DECLARE LV_FONT_DECLARE(my_font_1) \
 *                                LV_FONT_DECLARE(my_font_2)
 */
#define LV_FONT_CUSTOM_DECLARE    LV_FONT_DECLARE(myFont24)\
                                  LV_FONT_DECLARE(myFont14)\
                                  LV_FONT_DECLARE(myFont36)\
                                  LV_FONT_DECLARE(myFont18)

/* 如果你有很多字符的字体，请启用它。
   限制视乎字型大小、字型面及基本字体而定
   但它> 10,000字符，如果有问题，可能需要启用它.*/
#define LV_FONT_FMT_TXT_LARGE   0

/* 启用/禁用支持压缩字体。如果禁用，压缩
   符号不能被库处理，不会被渲染.*/
#define LV_USE_FONT_COMPRESSED 1

/* 使亚像素渲染 */
#define LV_USE_FONT_SUBPX 1
#if LV_USE_FONT_SUBPX
/* 设置显示的像素顺序.
 * 仅在使用“subpx字体”时使用.
 * 而“normal”字体则无关紧要.
 */
#define LV_FONT_SUBPX_BGR    0
#endif

/*声明字体的用户数据的类型(可以是。`void `， `int`， `struct`)*/
typedef void * lv_font_user_data_t;





/************************************
 *  主题使用
 *************************************/
/* 至少使能一个主题 */

/* 没有主题，可以根据需要应用你的样式
   没有标志位。设置LV_THEME_DEFAULT_FLAG 0 */
#define LV_USE_THEME_EMPTY       1

/* 简单地创建您的主题基于它
   没有标志位。设置LV_THEME_DEFAULT_FLAG 0 */
#define LV_USE_THEME_TEMPLATE    1

/* 一个快速的主题..
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
 *  文本设置
 ***********************************/
/* 为字符串选择字符编码.
 * IDE或编辑器应该具有相同的字符编码
 * 1. - LV_TXT_ENC_UTF8
 * 2. - LV_TXT_ENC_ASCII
 * */
#define LV_TXT_ENC LV_TXT_ENC_UTF8

/* 可以在这些字符上换行文本 */
#define LV_TXT_BREAK_CHARS                  " ,.;:-_"

/* 如果一个单词至少有这么长，就会在“最漂亮”的地方断开，要禁用，设置为值<= 0 */
#define LV_TXT_LINE_BREAK_LONG_LEN          0

/* 在一个长字中，在换行前一行中所能使用的最小字符数，取决于LV_TXT_LINE_BREAK_LONG_LEN. */
#define LV_TXT_LINE_BREAK_LONG_PRE_MIN_LEN  3

/* 在一个长字中，在换行后在一行中放置的最小字符数.取决于LV_TXT_LINE_BREAK_LONG_LEN. */
#define LV_TXT_LINE_BREAK_LONG_POST_MIN_LEN 3

/* 用于通知文本重新上色的控制字符 */
#define LV_TXT_COLOR_CMD "#"

/* 支持双向文本.
 * 允许混合从左到右和从右到左的文本.
 * 该方向将根据Unicode双向算法进行处理:
 * https://www.w3.org/International/articles/inline-bidi-markup/uba-basics*/
#define LV_USE_BIDI     0
#if LV_USE_BIDI
/* 设置默认方向。支持的值:
 * 1.`LV_BIDI_DIR_LTR` Left-to-Right 左到右
 * 2.`LV_BIDI_DIR_RTL` Right-to-Left 右到左
 * 3.`LV_BIDI_DIR_AUTO` 检测文本基本方向 */
#define LV_BIDI_BASE_DIR_DEF  LV_BIDI_DIR_AUTO
#endif

/* 支持阿拉伯语/波斯语言处理，在这些语言中，字符应替换为另一种形式基于他们在文中的位置 */
#define LV_USE_ARABIC_PERSIAN_CHARS 0

/* 更改内置的(v)snprintf函数 */
#define LV_SPRINTF_CUSTOM   0
#if LV_SPRINTF_CUSTOM
#  define LV_SPRINTF_INCLUDE <stdio.h>
#  define lv_snprintf     snprintf
#  define lv_vsnprintf    vsnprintf
#else   /*!LV_SPRINTF_CUSTOM*/
#  define LV_SPRINTF_DISABLE_FLOAT 1
#endif  /*LV_SPRINTF_CUSTOM*/





/*********************************
 *  LVGL对象设置
 **********************************/
#if LV_USE_USER_DATA
/*声明对象的用户数据的类型(可以是。`void `， `int`， `struct`)*/
typedef void * lv_obj_user_data_t;
/*提供一个函数来释放用户数据*/
#define LV_USE_USER_DATA_FREE 0
#if LV_USE_USER_DATA_FREE
#  define LV_USER_DATA_FREE_INCLUDE  "something.h"  /*Header for user data free function*/
/* Function prototype : void user_data_free(lv_obj_t * obj); */
#  define LV_USER_DATA_FREE  (user_data_free)       /*Invoking for user data free function*/
#endif
#endif

/* 1:启用' lv_obj_realign() '基于' lv_obj_align() '参数 */
#define LV_USE_OBJ_REALIGN          1

/* 使对象可以在更大的区域上点击.
 * LV_EXT_CLICK_AREA_OFF 或0:禁用该特性
 * LV_EXT_CLICK_AREA_TINY: 额外的区域可以水平和垂直调整(0..255 px)
 * LV_EXT_CLICK_AREA_FULL: 额外的区域可以在所有4个方向上进行调整(-32k..+ 32 k px)
 */
#define LV_USE_EXT_CLICK_AREA  LV_EXT_CLICK_AREA_TINY

/************************************
 *  LVGL对象的用法
 *************************************/
/*
 * 关于对象类型的文档: https://docs.lvgl.com/#Object-types
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
 * 非用户部分
 *************************************/
#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)    /* Disable warnings for Visual Studio*/
#  define _CRT_SECURE_NO_WARNINGS
#endif

/*--END OF LV_CONF_H--*/

#endif /*LV_CONF_H*/

#endif /*End of "Content enable"*/
