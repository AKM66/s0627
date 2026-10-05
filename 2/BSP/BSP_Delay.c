/*
***************************************************************************
*    模块：BSP_Delay 
*    描述：板级 延时功能模块驱动
*    作者：Shao
*    时间：2018.06.12
*    版本：UP-Magic-Version 2.0.0
***************************************************************************
*/
#include "BSP_Delay.h"

/*
***************************************************************************
*                         Cortex-M3/4 寄存器
***************************************************************************
*/
#define  DWT_CYCCNT  *(volatile unsigned int *)0xE0001004
#define  DWT_CR      *(volatile unsigned int *)0xE0001000
#define  DEM_CR      *(volatile unsigned int *)0xE000EDFC

#define  DEM_CR_TRCENA               (1 << 24)
#define  DWT_CR_CYCCNTENA            (1 <<  0)
	
/*
***************************************************************************
*	函 数 名: BSP_Delay_Init
*	功能说明: 板载 Delay 初始化函数
*	形    参: 无
*	返 回 值: 无
***************************************************************************
*/
 void BSP_Delay_Init(void)
{
	DEM_CR     |= (unsigned int)DEM_CR_TRCENA;  //Enable Cortex-M3/4's DWT CYCCNT reg.
	DWT_CYCCNT  = (unsigned int)0u;
	DWT_CR     |= (unsigned int)DWT_CR_CYCCNTENA;
}								    

/*
***************************************************************************
*	函 数 名: BSP_Delay_us
*	功能说明: 板载 Delay 微秒功能函数
*	形    参: nus 要延时的us数
*	返 回 值: 无
***************************************************************************
*/
void BSP_Delay_us(uint32_t nus)
{		
#if 1
    uint32_t tCnt, tDelayCnt;
	uint32_t tStart;
		
	tStart = DWT_CYCCNT;                             /* 刚进入时的计数器值 */
	tCnt = 0;
	tDelayCnt = nus * (SystemCoreClock / 1000000);	 /* 需要的节拍数 */ 		      

	while(tCnt < tDelayCnt)
	{
		tCnt = DWT_CYCCNT - tStart; /* 求减过程中，如果发生第一次32位计数器重新计数，依然可以正确计算 */	
	}
#else
    uint32_t st,et,ts;

    st = DWT_CYCCNT;
    ts =  nus * (SystemCoreClock /(1000000));
    et = st + ts;
    if(et < st)
    {
        //溢出，需要转动一周
        while(DWT_CYCCNT > et);      //等待 DWT_CYCCNT 溢出 返回0
    }

    while(DWT_CYCCNT < et);      //等待 DWT_CYCCNT 到底计数值
#endif
}

/*
***************************************************************************
*	函 数 名: BSP_Delay_ms
*	功能说明: 板载 Delay 毫秒功能函数
*	形    参: nms 要延时的ms数，nms<=1864
*	返 回 值: 无
***************************************************************************
*/
void BSP_Delay_ms(uint16_t nms)
{	 		  	  
#if 1
	BSP_Delay_us(1000*nms);
#else
    while(nms--)
    {
        BSP_Delay_us(1000);
    }
#endif
} 
