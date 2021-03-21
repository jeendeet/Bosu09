#include "managerpower.h"
#include "mainfunctionlist.h"
#include "stm8l15x_rtc.h"
#include "definepin.h"
#include "mfcv520.h"
#include "tsm12.h"
#include "audio.h"
#include "define.h"
#include "eeprom.h"
#include "adc.h"
#include "leddisplay.h"
#include "delay.h"

extern uint8_t Check_Flag_Volume, Check_Flag_Volume_MID_SMALL;
extern uint8_t Use_Left_Lock_State, Check_Language, Check_Flag_Volume;
extern uint8_t Bell_Flag, Temper_Flag;
extern volatile uint8_t Check_Operation_Of_Main;

void LED_Control_Power_On(void)
{
    GPIO_SetBits(GPIOA, GPIO_PIN_LED_CTR_POWER); 
    GPIO_SetBits(GPIOG, GPIO_PIN_CTR_LED); 
    GPIO_ResetBits(GPIOG, TSM12_I2C_enable_pin);
}

void LED_Control_Power_Off(void)
{
    GPIO_ResetBits(GPIOA, GPIO_PIN_LED_CTR_POWER); 
    GPIO_ResetBits(GPIOG, GPIO_PIN_CTR_LED); 
    GPIO_SetBits(GPIOG, TSM12_I2C_enable_pin);
}


void FINGER_Control_Power_On(uint32_t timeout)
{
    GPIO_SetBits(GPIOC, GPIO_PIN_FINGER_CTR_POWER); // Cap nguon cho fingerprint
    while(timeout--);
}

void FINGER_Control_Power_Off(void)
{
    GPIO_ResetBits(GPIOC, GPIO_PIN_FINGER_CTR_POWER); // Cap nguon cho fingerprint
}

void CARD_Control_Power_On(void)
{
    // PCD_SoftPowerUp();
    GPIO_SetBits(MFCVSPI_NRSTPD_GPIO_PORT, MFCVSPI_NRSTPD_PIN);
    delay_us(2);
}

void CARD_Control_Power_Off(void)
{ 
    // PCD_SoftPowerDown();
    GPIO_ResetBits(MFCVSPI_NRSTPD_GPIO_PORT, MFCVSPI_NRSTPD_PIN);
    delay_us(2);
}


void Powermanager_DeInit(void)
{
    // Deinit led
    GPIO_Init(GPIOG, GPIO_PIN_SH, GPIO_Mode_In_FL_No_IT);
	GPIO_Init(GPIOG, GPIO_PIN_DS, GPIO_Mode_In_FL_No_IT);
	GPIO_Init(GPIOG, GPIO_PIN_ST, GPIO_Mode_In_FL_No_IT);

    // Deinit Audio
	GPIO_Init(GPIOA, AUDIO_PIN_CLK, GPIO_Mode_In_FL_No_IT);
	GPIO_Init(GPIOA, AUDIO_PIN_CS, GPIO_Mode_In_FL_No_IT);
	GPIO_Init(GPIOA, AUDIO_PIN_DATA, GPIO_Mode_In_FL_No_IT);

    GPIO_Init(GPIOB, GPIO_Pin_1, GPIO_Mode_Out_PP_Low_Fast);
    GPIO_Init(GPIOA, GPIO_Pin_6, GPIO_Mode_Out_PP_Low_Fast);
    
}

void Powermanager_Init(void)
{

    /* Init power on board*/
    LED_Control_Power_On();

    /* Init Pin Led*/
    GPIO_Init(GPIOG, GPIO_PIN_SH, GPIO_Mode_Out_PP_Low_Fast);
	GPIO_Init(GPIOG, GPIO_PIN_DS, GPIO_Mode_Out_PP_Low_Fast);
	GPIO_Init(GPIOG, GPIO_PIN_ST, GPIO_Mode_Out_PP_Low_Fast);

    /*fast mode selection*/
    TSM12_write_byte(TSM12_CTRL1, 0x80);

    Use_Left_Lock_State = READ_Byte_From_Eeprom(ADDRESS_LEFT_LOCK_STATE);
    if(Use_Left_Lock_State != 1)
        Use_Left_Lock_State = 0;

    Check_Language = READ_Byte_From_Eeprom(ADDRESS_LANGUAGE);
    if(Check_Language != 1)
        Check_Language = 0;

    Check_Flag_Volume = READ_Byte_From_Eeprom(ADDRESS_VOLUME);    
    if(Check_Flag_Volume == 0xFF)
    {
        Check_Flag_Volume = 0;
        Check_Flag_Volume_MID_SMALL = 0;
    }else if(Check_Flag_Volume == 0x00)
    {
        Check_Flag_Volume = 0;
        Check_Flag_Volume_MID_SMALL = 1;
    }else if(Check_Flag_Volume == 0x01)
    {
        Check_Flag_Volume = 1;
        Check_Flag_Volume_MID_SMALL = 0;
    }

    if(Check_Flag_Volume_MID_SMALL == 1)
    {
        if((Bell_Flag != 1)&&(Temper_Flag != 1))
            AUDIO_Send(NORMAL_SOUND);
        else
        {
            AUDIO_Send(BIG_SOUND);
        }
    }

    if(Check_Flag_Volume == 1)
    {
        if((Bell_Flag != 1)&&(Temper_Flag != 1))
            AUDIO_Send(NOT_SOUND);
        else
        {
            AUDIO_Send(BIG_SOUND);
        }
        
    }
    
}


