#include "reset.h"
#include "stm8l15x_wwdg.h"
#include "stm8l15x_iwdg.h"

void RESET_CPU(void)
{
    WWDG_Init(WINDOW_VALUE, COUNTER_INIT);
}

void RESET_CPU_BY_IWDG(void)
{
    /* Enable IWDG (the LSI oscillator will be enabled by hardware) */
  IWDG_Enable(); 
  
  /* IWDG timeout equal to 214 ms (the timeout may varies due to LSI frequency
     dispersion) */
  /* Enable write access to IWDG_PR and IWDG_RLR registers */
  IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
  
  /* IWDG configuration: IWDG is clocked by LSI = 38KHz */
  IWDG_SetPrescaler(IWDG_Prescaler_256);
  
  /* IWDG timeout equal to 214.7 ms (the timeout may varies due to LSI frequency dispersion) */
  /* IWDG timeout = (RELOAD_VALUE + 1) * Prescaler / LSI 
                  = (254 + 1) * 256 / 38 000 
                  = 1.7 s */
  IWDG_SetReload((uint8_t)RELOAD_VALUE);
  
  /* Reload IWDG counter */
  IWDG_ReloadCounter();
}