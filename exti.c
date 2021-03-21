#include "exti.h"
#include "definepin.h"
#include "stm8l15x_exti.h"
#include "managerpower.h"
#include "stm8l15x_gpio.h"

/* EXTI Init----------------------------------------------*/
void EXTI_Init(void){
  /* ZB */
  GPIO_Init(GPIOD, GPIO_PIN_ZB_INT, GPIO_Mode_Out_OD_Low_Fast);
  GPIO_Init(GPIOB, GPIO_PIN_ZB_OUT, GPIO_Mode_In_FL_IT);
  EXTI_SetPinSensitivity(EXTI_Pin_0, EXTI_Trigger_Falling);
  
  /* Bell */
  GPIO_Init(GPIOB, GPIO_PIN_BELL_INT, GPIO_Mode_In_FL_IT);
  EXTI_SetPinSensitivity(EXTI_Pin_3, EXTI_Trigger_Rising);

  /* Switch */
  GPIO_Init(GPIOD, GPIO_PIN_SWITCH_INT, GPIO_Mode_In_FL_IT);
  EXTI_SetPinSensitivity(EXTI_Pin_2, EXTI_Trigger_Falling);

  /* Reset */
  GPIO_Init(GPIOE, GPIO_PIN_RESET_INT, GPIO_Mode_In_FL_IT);
  EXTI_SetPortSensitivity(EXTI_Port_E, EXTI_Trigger_Falling);
  EXTI_SetHalfPortSelection(EXTI_HalfPort_E_LSB, ENABLE);

  /* TEMPER  && FINGERPRINT */
  GPIO_Init(GPIOG, GPIO_PIN_FINGER_INT, GPIO_Mode_In_FL_IT);
  GPIO_Init(GPIOG, GPIO_PIN_TEMPER_INT, GPIO_Mode_In_FL_IT);
  EXTI_SetPinSensitivity(EXTI_Pin_6, EXTI_Trigger_Falling);
  EXTI_SetPinSensitivity(EXTI_Pin_7, EXTI_Trigger_Rising);

  /* FINGERPRINT POWER*/
  GPIO_Init(GPIOC, GPIO_PIN_FINGER_CTR_POWER, GPIO_Mode_Out_PP_Low_Fast);

}