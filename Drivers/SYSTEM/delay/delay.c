/**
 ****************************************************************************************************
 * @file        delay.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2020-03-12
 * @brief       使用SysTick的普通计数模式对延迟进行管理(支持ucosii)
 *              提供delay_init初始化函数， delay_us和delay_ms等延时函数
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 STM32H750开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 *
 * 修改说明
 * V1.0 20200312
 * 第一次发布
 *
 ****************************************************************************************************
 */
 
#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/delay/delay.h"


/*如果使用FreeRTOS,则包括下面的头文件即可*/
#if SYSTEM_SUPPORT_OS
#include "FreeRTOS.h"       /*FreeRTOS 使用*/ 
#include "task.h"
#endif
static uint32_t fac_us=0;             /*us延时倍乘数*/

#if SYSTEM_SUPPORT_OS   
static uint16_t fac_ms=0;             /*ms延时倍乘数,在os下,代表每个节拍的ms数*/
#endif


extern void xPortSysTickHandler(void);

/**
 * @brief     systick中断服务函数,使用OS时用到
 * @param     无  
 * @retval    无
 */  
void SysTick_Handler(void)
{
    if(xTaskGetSchedulerState()!=taskSCHEDULER_NOT_STARTED)   /*系统已经运行*/
    {
        xPortSysTickHandler();
    }
    HAL_IncTick();
}


/**
 * @brief     当使用FreeRTOS的时候,此函数会初始化FreeRTOS的时钟节拍
 * @param     无  
 * @retval    无
 */  
void delay_init(uint32_t SYSCLK)
{
    #if SYSTEM_SUPPORT_OS                                     /*如果需要支持OS.*/
      uint32_t reload;
    #endif
        HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);  /*SysTick频率为HCLK*/
      fac_us=SYSCLK;                                          /*不论是否使用OS,fac_us都需要使用*/
    #if SYSTEM_SUPPORT_OS                                     /*如果需要支持OS.*/
      reload=SYSCLK;                                          /*每秒钟的计数次数 单位为K*/
      reload*=1000000/configTICK_RATE_HZ;                     /*根据delay_ostickspersec设定溢出时间*/
                                                              /*reload为24位寄存器,最大值:16777216,在216M下,约合77.7ms左右*/
      fac_ms=1000/configTICK_RATE_HZ;                         /*代表OS可以延时的最少单位*/
      SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;                /*开启SYSTICK中断*/
      SysTick->LOAD=reload;                                   /*每1/OS_TICKS_PER_SEC秒中断一次*/
      SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;                 /*开启SYSTICK*/
    #endif 
} 


/**
 * @brief     要延时的us数
 * @param     无  
 * @retval    无
 */ 
void delay_us(uint32_t nus)
{
    uint32_t ticks;
    uint32_t told,tnow,tcnt=0;
    uint32_t reload=SysTick->LOAD;           /*LOAD的值*/
    ticks=nus*fac_us;                        /*需要的节拍数 */
    
    told=SysTick->VAL;                       /*刚进入时的计数器值*/
    while(1)
    {
        tnow=SysTick->VAL;  
        if(tnow!=told)
        {       
            if(tnow<told)tcnt+=told-tnow;     /*这里注意一下SYSTICK是一个递减的计数器就可以了.*/
            else tcnt+=reload-tnow+told;      
            told=tnow;
            if(tcnt>=ticks)break;             /*时间超过/等于要延迟的时间,则退出.*/
        }  
    };
   
} 

/**
 * @brief     要延时的xms数
 * @param     无  
 * @retval    无
 */ 
void delay_xms(uint32_t nms)
{ 
    if(xTaskGetSchedulerState()!=taskSCHEDULER_NOT_STARTED)   /*系统已经运行*/
    {
        if(nms>=fac_ms)                                       /*延时的时间大于OS的最少时间周期 */
        { 
            vTaskDelay(nms/fac_ms);                           /*FreeRTOS延时*/
        }
        nms%=fac_ms;                                          /*OS已经无法提供这么小的延时了,采用普通方式延时*/
    }
    delay_us((uint32_t)(nms*1000));                           /*普通方式延时*/
}

/**
 * @brief     要延时的ms数
 * @param     无  
 * @retval    无
 */ 
void delay_ms(uint16_t nms)
{
    uint32_t i;
    for(i=0;i<nms;i++) delay_us(1000);
}
