/**
 ****************************************************************************************************
 * @file        delay.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2020-03-12
 * @brief       ʹ��SysTick����ͨ����ģʽ���ӳٽ��й���(֧��ucosii)
 *              �ṩdelay_init��ʼ�������� delay_us��delay_ms����ʱ����
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� STM32H750������
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
 *
 * �޸�˵��
 * V1.0 20200312
 * ��һ�η���
 *
 ****************************************************************************************************
 */
 
#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/delay/delay.h"


/*���ʹ��FreeRTOS,����������ͷ�ļ�����*/
#if SYSTEM_SUPPORT_OS
#include "FreeRTOS.h"       /*FreeRTOS ʹ��*/ 
#include "task.h"
#endif
static uint32_t fac_us=0;             /*us��ʱ������*/

#if SYSTEM_SUPPORT_OS   
static uint16_t fac_ms=0;             /*ms��ʱ������,��os��,����ÿ�����ĵ�ms��*/
#endif


extern void xPortSysTickHandler(void);

/**
 * @brief     systick�жϷ�����,ʹ��OSʱ�õ�
 * @param     ��  
 * @retval    ��
 */  
void SysTick_Handler(void)
{
    if(xTaskGetSchedulerState()!=taskSCHEDULER_NOT_STARTED)   /*ϵͳ�Ѿ�����*/
    {
        xPortSysTickHandler();
    }
    HAL_IncTick();
}


/**
 * @brief     ��ʹ��FreeRTOS��ʱ��,�˺������ʼ��FreeRTOS��ʱ�ӽ���
 * @param     ��  
 * @retval    ��
 */  
void delay_init(uint32_t SYSCLK)
{
    #if SYSTEM_SUPPORT_OS                                     /*�����Ҫ֧��OS.*/
      uint32_t reload;
    #endif
        HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);  /*SysTickƵ��ΪHCLK*/
      fac_us=SYSCLK;                                          /*�����Ƿ�ʹ��OS,fac_us����Ҫʹ��*/
    #if SYSTEM_SUPPORT_OS                                     /*�����Ҫ֧��OS.*/
      reload=SYSCLK;                                          /*ÿ���ӵļ������� ��λΪK*/
      reload*=1000000/configTICK_RATE_HZ;                     /*����delay_ostickspersec�趨���ʱ��*/
                                                              /*reloadΪ24λ�Ĵ���,���ֵ:16777216,��216M��,Լ��77.7ms����*/
      fac_ms=1000/configTICK_RATE_HZ;                         /*����OS������ʱ�����ٵ�λ*/
      SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;                /*����SYSTICK�ж�*/
      SysTick->LOAD=reload;                                   /*ÿ1/OS_TICKS_PER_SEC���ж�һ��*/
      SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;                 /*����SYSTICK*/
    #endif 
} 


/**
 * @brief     Ҫ��ʱ��us��
 * @param     ��  
 * @retval    ��
 */ 
void delay_us(uint32_t nus)
{
    uint32_t ticks;
    uint32_t told,tnow,tcnt=0;
    uint32_t reload=SysTick->LOAD;           /*LOAD��ֵ*/
    ticks=nus*fac_us;                        /*��Ҫ�Ľ����� */
    
    told=SysTick->VAL;                       /*�ս���ʱ�ļ�����ֵ*/
    while(1)
    {
        tnow=SysTick->VAL;  
        if(tnow!=told)
        {       
            if(tnow<told)tcnt+=told-tnow;     /*����ע��һ��SYSTICK��һ���ݼ��ļ������Ϳ�����.*/
            else tcnt+=reload-tnow+told;      
            told=tnow;
            if(tcnt>=ticks)break;             /*ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�.*/
        }  
    };
   
} 

/**
 * @brief     Ҫ��ʱ��xms��
 * @param     ��  
 * @retval    ��
 */ 
void delay_xms(uint32_t nms)
{ 
    if(xTaskGetSchedulerState()!=taskSCHEDULER_NOT_STARTED)   /*ϵͳ�Ѿ�����*/
    {
        if(nms>=fac_ms)                                       /*��ʱ��ʱ�����OS������ʱ������ */
        { 
            vTaskDelay(nms/fac_ms);                           /*FreeRTOS��ʱ*/
        }
        nms%=fac_ms;                                          /*OS�Ѿ��޷��ṩ��ôС����ʱ��,������ͨ��ʽ��ʱ*/
    }
    delay_us((uint32_t)(nms*1000));                           /*��ͨ��ʽ��ʱ*/
}

/**
 * @brief     Ҫ��ʱ��ms��
 * @param     ��  
 * @retval    ��
 */ 
void delay_ms(uint16_t nms)
{
    uint32_t i;
    for(i=0;i<nms;i++) delay_us(1000);
}
