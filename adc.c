#include "adc.h"
#include "stm8l15x_adc.h"

void ADC_Config_Init(void)
{
    /* Enable ADC1 clock */
    CLK_PeripheralClockConfig(CLK_Peripheral_ADC1, ENABLE);
  
    /* Initialise and configure ADC1 */
    ADC_Init(ADC1, ADC_ConversionMode_Single, ADC_Resolution_12Bit, ADC_Prescaler_2);
    ADC_SamplingTimeConfig(ADC1, ADC_Group_SlowChannels, ADC_SamplingTime_384Cycles);

    /* Enable ADC1 */
    ADC_Cmd(ADC1, ENABLE);
}

void ADC_Config_Low_Power_Mode(void)
{
    /* Enable ADC1 clock */
    CLK_PeripheralClockConfig(CLK_Peripheral_ADC1, ENABLE);
  
    /* Initialise and configure ADC1 */
    ADC_Init(ADC1, ADC_ConversionMode_Single, ADC_Resolution_12Bit, ADC_Prescaler_1);
    ADC_SamplingTimeConfig(ADC1, ADC_Group_SlowChannels, ADC_SamplingTime_384Cycles);

    /* Enable ADC1 */
    ADC_Cmd(ADC1, ENABLE);
}

void ADC_Config_DeInit(void)
{
    /* Enable ADC1 clock */
    CLK_PeripheralClockConfig(CLK_Peripheral_ADC1, DISABLE);
    /* Enable ADC1 */
    ADC_Cmd(ADC1, DISABLE);
}

uint16_t ADC_Read(void)
{
    uint16_t tmpreg = 0;
    ADC_ClearFlag(ADC1, ADC_FLAG_EOC);
    /* Enable ADC1 Channel 0 */
    ADC_ChannelCmd(ADC1, ADC_Channel_0, ENABLE);
    /* Start ADC1 Conversion using Software trigger*/
    ADC_SoftwareStartConv(ADC1);
    while((ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET));
    /* Read value */
    /* Get last ADC converted data.*/
    tmpreg = (uint16_t)(ADC1->DRH);
    tmpreg = (uint16_t)((uint16_t)((uint16_t)tmpreg << 8) | ADC1->DRL);
    return (uint16_t)tmpreg;
}

uint16_t ADC17_Read(void)
{
    // uint32_t Timeout = 0xFF;
    uint16_t tmpreg = 0;
    ADC_ClearFlag(ADC1, ADC_FLAG_EOC);
    /* Enable ADC1 Channel 0 */
    ADC_ChannelCmd(ADC1, ADC_Channel_17, ENABLE);
    /* Start ADC1 Conversion using Software trigger*/
    ADC_SoftwareStartConv(ADC1);
    while((ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET));
    /* Read value */
    /* Get last ADC converted data.*/
    tmpreg = (uint16_t)(ADC1->DRH);
    tmpreg = (uint16_t)((uint16_t)((uint16_t)tmpreg << 8) | ADC1->DRL);
    return (uint16_t)tmpreg;
}

