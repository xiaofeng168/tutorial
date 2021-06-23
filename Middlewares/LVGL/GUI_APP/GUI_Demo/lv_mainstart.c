/**
 ****************************************************************************************************
 * @file        lv_mainstart.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2020-03-29
 * @brief       LVGL 实验
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
 
#include "lvgl/lvgl.h"
#include "stdio.h"
#include "stdlib.h"


static const char * lv_kb_map[23] = {"7", "8", "9", "DEL","#FF0000 AC#", "\n",
                                      "4", "5", "6", "+","-", "\n",
                                      "1", "2", "3", "*","/", "\n",
                                      "0", "+/-", ".", "=", ""};
lv_obj_t * lv_ta_input;               /* 账号文字区域控件 */
lv_obj_t * lv_btnm_label;
int lv_calc_x1 = 0;
int lv_calc_pos = 0;
int calc_ec =0;
double cbp_and_negative = 0;
int lv_cbp_and_negative = 0;
int lv_cbp_and_negativee = 0;
int lv_calc_math = 0;
uint8_t lv_math_flag = 0;
double lv_math_x1 = 0;
double lv_math_x2 = 0;
double lv_math_result = 0;
char lv_chx_buf[100];

/* π值 */
#define CALC_PI 3.1415926535897932384626433832795

uint8_t lv_calc_label(uint8_t ctype)
{
    char *chx;
    memset(lv_chx_buf, 0, sizeof(lv_chx_buf));
    switch(ctype)
    {
        case 0:/* 加法计算 */
          chx = "+";
          break;
       case 1:/* 减法计算 */
          chx = "-";
          break;
       case 2:/* 乘法计算 */
          chx = "*";
          break;
       case 3:/* 除法计算 */
          chx = "/";
          break;
    }

    strcat((char *)lv_chx_buf, chx);
    lv_label_set_text(lv_btnm_label,lv_chx_buf);
    return  ctype;
}

double lv_math_calc(double x1,double x2,uint8_t ctype)
{
    switch(ctype)
    {
        case 0:/* 加法 */
          x1=x1+x2;
          break;
        case 1:/* 减法 */
          x1=x1-x2;
          break;
        case 2:/* 乘法 */
          x1=x1*(x2);
          break;
        case 3:/* 除法 */
          x1=x1/(x2);
          break;
        case 5:/* 没有任何运算符 */
          x1=x2;
          break;
    }
    return x1;
}

/**
  * @brief  回调函数
  * @param  obj  :对象
  * @param  event:事件
  * @retval 无
  */
static void lv_btnm_handler(lv_obj_t * obj, lv_event_t event)
{
    char str[25];
    char chx_buf[100];
    memset(chx_buf, 0, sizeof(chx_buf));
  
    if(event == LV_EVENT_VALUE_CHANGED)
    {
        const char * txt = lv_btnmatrix_get_active_btn_text(obj);
        uint32_t cur = lv_textarea_get_cursor_pos(lv_ta_input);

        if (atof(lv_textarea_get_text(lv_ta_input)) == 0&&lv_calc_pos == 0&&lv_cbp_and_negative ==0&&lv_calc_math == 0)
        {
            lv_textarea_set_text(lv_ta_input, "");
        }

        if (txt == lv_kb_map[4]) /* AC */
        {
            lv_textarea_set_text(lv_ta_input, "0");
            lv_calc_pos = 0;
            lv_cbp_and_negativee = 0;
            lv_cbp_and_negative = 0;
        }
        else if(txt == lv_kb_map[3]) /* 删除一个字符 */
        {
            if (calc_ec == 0)
            {
                lv_textarea_del_char(lv_ta_input);

                if (atof(lv_textarea_get_text(lv_ta_input)) == 0)
                {
                    lv_textarea_set_text(lv_ta_input, "0");
                }
            }
            else
            {
                lv_textarea_set_text(lv_ta_input, "0");
                lv_calc_math = 0;
                lv_calc_pos = 0;
                lv_cbp_and_negativee = 0;
                lv_cbp_and_negative = 0;
            }
        }
        else if (txt == lv_kb_map[0]||txt == lv_kb_map[1]||txt == lv_kb_map[2]
                 ||txt == lv_kb_map[6]||txt == lv_kb_map[7]||txt == lv_kb_map[8]
                 ||txt == lv_kb_map[12]||txt == lv_kb_map[13]||txt == lv_kb_map[14]
                 ||txt == lv_kb_map[18]||txt == lv_kb_map[20])
        {
            if (calc_ec == 1)
            {
                lv_textarea_set_text(lv_ta_input, "");
                calc_ec = 0; 
                lv_calc_pos = 0;
                lv_cbp_and_negativee = 0;
                lv_cbp_and_negative = 0;
            }
            
            if (lv_math_x1!=0&&atof(lv_textarea_get_text(lv_ta_input)) == 0&&!strstr(lv_textarea_get_text(lv_ta_input), ".")&&!strstr(lv_textarea_get_text(lv_ta_input), "-"))
            {
                lv_textarea_set_text(lv_ta_input, "");
            }
            
            if (lv_cbp_and_negative == 1&&strstr(lv_textarea_get_text(lv_ta_input), "-")&&lv_cbp_and_negativee == 0)
            {
                lv_textarea_del_char(lv_ta_input);
                lv_cbp_and_negative = 0;
                lv_cbp_and_negativee = 1;
            }

            lv_textarea_set_cursor_pos(lv_ta_input, (int32_t)cur);

            if (atof(lv_textarea_get_text(lv_ta_input)) == 0&&txt == lv_kb_map[20]&&lv_calc_pos != 1)
            {
                lv_calc_pos = 1;
                strcat((char *)chx_buf, "0.");
                lv_textarea_add_text(lv_ta_input,chx_buf);
            }
            else
            {
                if (txt == lv_kb_map[20]&&strstr(lv_textarea_get_text(lv_ta_input), "."))
                {
                    
                }
                else
                {
                    lv_textarea_add_text(lv_ta_input,txt);
                }
            }

        }
       else if (txt == lv_kb_map[19]) /* +/- */
       {
           cbp_and_negative = atof((const char *)lv_textarea_get_text(lv_ta_input)); /* 转换为数字 */
           cbp_and_negative = - cbp_and_negative;
           memset(str, 0, sizeof(str));
           sprintf(str,"%g",cbp_and_negative);
           lv_textarea_set_text(lv_ta_input,(const char *)str);
           lv_cbp_and_negative = 1;
       }
       else if (txt == lv_kb_map[9]) /* + */
       {
            lv_calc_math = 1;
            if (lv_math_x1 == 0)
            {
                lv_math_x1 = atof((const char *)lv_textarea_get_text(lv_ta_input));
                lv_textarea_set_text(lv_ta_input, "0");
            }
            else
            {
                lv_calc_math = 0;
            }
            lv_math_flag = lv_calc_label(0);

       }
       else if (txt == lv_kb_map[10]) /* - */
       {
            lv_calc_math = 1;
            if (lv_math_x1 == 0)
            {
                lv_math_x1 = atof((const char *)lv_textarea_get_text(lv_ta_input));
                lv_textarea_set_text(lv_ta_input, "0");
            }
            else
            {
                lv_calc_math = 0;
            }
            lv_math_flag = lv_calc_label(1);
       }
       else if (txt == lv_kb_map[15]) /* X */
       {
            lv_calc_math = 1;
            if (lv_math_x1 == 0)
            {
                lv_math_x1 = atof((const char *)lv_textarea_get_text(lv_ta_input));
                lv_textarea_set_text(lv_ta_input, "0");
            }
            else
            {
                lv_calc_math = 0;
            }
            lv_math_flag = lv_calc_label(2);
       }
       else if (txt == lv_kb_map[16]) /* / */
       {
            lv_calc_math = 1;
            if (lv_math_x1 == 0)
            {
                lv_math_x1 = atof((const char *)lv_textarea_get_text(lv_ta_input));
                lv_textarea_set_text(lv_ta_input, "0");
            }
            else
            {
                lv_calc_math = 0;
            }
            lv_math_flag = lv_calc_label(3);
       }
       else if (txt == lv_kb_map[21]) /* = */
       {
            lv_math_x2 = atof((const char *)lv_textarea_get_text(lv_ta_input));
            lv_math_result = lv_math_calc(lv_math_x1,lv_math_x2,lv_math_flag);
            memset(str, 0, sizeof(str));
            calc_ec = 1;
            lv_math_x1 = 0;
            sprintf(str,"%g",lv_math_result);
            lv_textarea_set_text(lv_ta_input,(const char *)str);
       }
    }
}

/**
  * @brief  测试
  * @param  无
  * @retval 无
  */
void lv_mainstart(void)
{
    lv_obj_t * lv_btnm_key = lv_btnmatrix_create(lv_scr_act(), NULL);
    lv_btnmatrix_set_map(lv_btnm_key, lv_kb_map);
    lv_btnmatrix_set_recolor(lv_btnm_key, true);
    lv_obj_set_size(lv_btnm_key,LV_HOR_RES_MAX,LV_VER_RES_MAX/2);
    lv_btnmatrix_set_align(lv_btnm_key,LV_LABEL_ALIGN_CENTER);
    lv_obj_align(lv_btnm_key, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, 0);
    lv_obj_set_event_cb(lv_btnm_key, lv_btnm_handler);
    lv_obj_set_style_local_text_font(lv_btnm_key,LV_OBJ_PART_MAIN,LV_STATE_DEFAULT,&lv_font_montserrat_22);

    /* 创建输入框 */
    lv_ta_input = lv_textarea_create(lv_scr_act(), NULL);
    lv_textarea_set_text(lv_ta_input, "0");
    lv_textarea_set_one_line(lv_ta_input, true);      /* 将文本区域配置为一行或恢复正常 */
    lv_textarea_set_cursor_hidden(lv_ta_input,true);  /* 隐藏光标 */
    lv_obj_set_click(lv_ta_input,false);
    lv_obj_set_size(lv_ta_input,LV_HOR_RES_MAX - 20,(LV_VER_RES_MAX/3 - 30)/4);
    lv_obj_align(lv_ta_input, lv_btnm_key, LV_ALIGN_OUT_TOP_MID, 0, -50);
    lv_obj_set_style_local_text_font(lv_ta_input,LV_OBJ_PART_MAIN,LV_STATE_DEFAULT,&lv_font_montserrat_36);

    lv_btnm_label = lv_label_create(lv_scr_act(),NULL);
    lv_label_set_text(lv_btnm_label,"");
    lv_obj_set_style_local_text_font(lv_btnm_label,LV_OBJ_PART_MAIN,LV_STATE_DEFAULT,&lv_font_montserrat_22);
    lv_obj_align(lv_btnm_label,lv_ta_input,LV_ALIGN_IN_BOTTOM_RIGHT,-50,-10);
}
