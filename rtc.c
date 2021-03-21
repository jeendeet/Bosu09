#include "rtc.h"
#include "stm8l15x_tim4.h"
#include "stm8l15x_rtc.h"
#include "stm8l15x_clk.h"

void RTC_Config(void)
{
    /* ENable LSI CLK */
    CLK_LSICmd(ENABLE);
    
    /* Enable RTC clock */
    CLK_RTCClockConfig(CLK_RTCCLKSource_LSI, CLK_RTCCLKDiv_1);
    /* Wait for LSI clock to be ready */
    while (CLK_GetFlagStatus(CLK_FLAG_LSIRDY) == RESET);
    /* wait for 1 second for the LSE Stabilisation */
    LSI_StabTime();
    
    CLK_PeripheralClockConfig(CLK_Peripheral_RTC, ENABLE);

    /* Configures the RTC wakeup timer_step = RTCCLK/16 = LSI/16 = 421.052631 us */
    RTC_WakeUpClockConfig(RTC_WakeUpClock_RTCCLK_Div16);

    /* Enable wake up unit Interrupt */
    RTC_ITConfig(RTC_IT_WUT, ENABLE);

    RTC_SetWakeUpCounter(855);
    
    RTC_WakeUpCmd(ENABLE);
}

void LSI_StabTime(void)
{
    CLK_PeripheralClockConfig(CLK_Peripheral_TIM4, ENABLE);

    /* Configure TIM4 to generate an update event each 1 s */
    TIM4_TimeBaseInit(TIM4_Prescaler_16384, 50);
    /* Clear update flag */
    TIM4_ClearFlag(TIM4_FLAG_Update);

    /* Enable TIM4 */
    TIM4_Cmd(ENABLE);

    /* Wait 1 sec */
    while ( TIM4_GetFlagStatus(TIM4_FLAG_Update) == RESET );

    TIM4_ClearFlag(TIM4_FLAG_Update);

    /* Disable TIM4 */
    TIM4_Cmd(DISABLE);

    CLK_PeripheralClockConfig(CLK_Peripheral_TIM4, DISABLE);
}