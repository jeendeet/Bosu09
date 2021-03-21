#include "clock.h"
#include "stm8l15x_clk.h"

/* CLK Init -------------------------------------------*/
void CLK_Init(void){

    CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);
    
    CLK_SYSCLKSourceSwitchCmd(ENABLE);
    
    CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_HSI);
    
    while (CLK_GetSYSCLKSource() != CLK_SYSCLKSource_HSI);

    CLK_HSICmd(ENABLE);
    
    /* Enable TIM4 CLK */
    CLK_PeripheralClockConfig(CLK_Peripheral_TIM4, ENABLE);
    /* Enable TIM1 CLK */
    CLK_PeripheralClockConfig(CLK_Peripheral_TIM1, ENABLE);
    /* Enable TIM2 CLK */
    CLK_PeripheralClockConfig(CLK_Peripheral_TIM2, ENABLE);
    /* ENable USART1 CLK */
    CLK_PeripheralClockConfig(CLK_Peripheral_USART1, ENABLE);
    /* ENable USART2 CLK */
    CLK_PeripheralClockConfig(CLK_Peripheral_USART2, ENABLE);
    /* ENable I2C CLK */
    CLK_PeripheralClockConfig(CLK_Peripheral_I2C1,ENABLE);
    /* ENable LSI CLK */
    CLK_LSICmd(ENABLE);
}

void UP_Clk(void)
{
    CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);
  
    CLK_SYSCLKSourceSwitchCmd(ENABLE);
    
    CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_HSI);
    
    while (CLK_GetSYSCLKSource() != CLK_SYSCLKSource_HSI);
    CLK_HSICmd(ENABLE);
}

void DOWN_Clk(void)
{
    CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);
  
    CLK_SYSCLKSourceSwitchCmd(ENABLE);
  
    CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_LSI);
  
    while (CLK_GetSYSCLKSource() != CLK_SYSCLKSource_LSI);
    CLK_LSICmd(ENABLE);
}