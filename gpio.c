#include "gpio.h"
#include "stm8l15x_gpio.h"


void GPIO_Init_Config(void)
{
    GPIO_Init(GPIOA, GPIO_Pin_All, GPIO_Mode_Out_PP_Low_Fast);
    GPIO_Init(GPIOB, GPIO_Pin_All, GPIO_Mode_Out_PP_Low_Fast);
    GPIO_Init(GPIOC, GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7, GPIO_Mode_Out_PP_Low_Fast);
    GPIO_Init(GPIOC, GPIO_Pin_4, GPIO_Mode_Out_PP_Low_Fast);
    GPIO_Init(GPIOD, GPIO_Pin_All, GPIO_Mode_Out_PP_Low_Fast);
    GPIO_Init(GPIOE, GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2, GPIO_Mode_Out_PP_Low_Fast);
    GPIO_Init(GPIOE, GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7, GPIO_Mode_Out_PP_Low_Fast);
    GPIO_Init(GPIOF, GPIO_Pin_All, GPIO_Mode_Out_PP_Low_Fast);
    GPIO_Init(GPIOG, GPIO_Pin_All, GPIO_Mode_Out_PP_Low_Fast);
}