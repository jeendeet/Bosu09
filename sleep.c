#include "sleep.h"
#include "stm8l15x_pwr.h"
#include "stm8l15x_wwdg.h"
#include "stm8l15x_iwdg.h"
#include "stm8l15x_rtc.h"
#include "leddisplay.h"
#include "stm8l15x_gpio.h"
#include "definepin.h"
#include "define.h"
#include "clock.h"
#include "audio.h"
#include "timer.h"
#include "eeprom.h"
#include "ncp.h"
#include "reset.h"
#include "tsm12.h"
#include "Softwaretimer.h"
#include "adc.h"
#include "delay.h"  
#include "stm8l15x_adc.h"
#include "fingerprint.h"
#include "mfcv520.h"
#include "managerpower.h"

extern uint8_t SettingSystemFlag, Eeprom_Flag, Temp_ID, Check_Button, Readbattery;
extern volatile uint8_t Button_Card_Flag, Finger_Flag, Bell_Flag, Switch_Flag, Smartcard_Flag;
extern volatile uint8_t Timeout_Flag, Sleep_Flag, Finger_Timeout_Flag, Start_Flag, Temper_Flag;
extern volatile uint8_t DistanceTwiceReadCard, fingerPrintState, zb_flagsetlock, Timeout_Sound_Flag;
extern volatile uint8_t zb_flagtogglenetwork, zb_flagchangelockstate, zb_flagjoinnetwork;
extern volatile uint8_t Zb_Flag, zb_flagsetpin, zb_flaggetpin, zb_flagchangepin, zb_getid_again;
extern volatile uint8_t zb_flagclearpincode, zb_flaggetallpin, Check_Faulty_Systm, Check_Operation_Of_Main;
extern volatile uint8_t Detect_Error_Status, Check_End_Set_Lock;
extern volatile uint16_t Count, Count_Motor;
extern uint16_t Zb_Flag_Count;

extern bool Check_Hand_Search, Check_Hand_Enroll;
uint8_t Check_Sleep = 0;


/* Sleep mode */
void SLEEP_Mode_Init(void){
    PWR_UltraLowPowerCmd(ENABLE);
    PWR_FastWakeUpCmd(ENABLE);
}

void SLEEP_Mode_WFE_Init(void)
{
	PWR_UltraLowPowerCmd(ENABLE);
	PWR_FastWakeUpCmd(ENABLE);
}

void SLEEP_Mode(void)
{
	Button_Card_Flag = 0; Finger_Flag = 0; zb_flagjoinnetwork = 0; Finger_Timeout_Flag = 0;
	Bell_Flag = 0; Timeout_Flag = 0; Sleep_Flag = 0; Switch_Flag = 0, zb_flagsetlock = 0;
	DistanceTwiceReadCard = 0, Check_Sleep = 1, fingerPrintState = 0, Readbattery = 0;
	Zb_Flag = 0, Start_Flag = 0, zb_flagsetpin = 0; zb_flaggetpin = 0; zb_flagchangepin = 0;
	zb_flagclearpincode = 0, zb_flaggetallpin = 0, Check_Button = 0; Smartcard_Flag = 0;
	Check_Hand_Search = FALSE,  Check_Hand_Enroll = FALSE, Detect_Error_Status = 0;
	SettingSystemFlag = 0, Eeprom_Flag = 0; Temp_ID = 0, Check_Faulty_Systm = 0;
    Check_End_Set_Lock = 0; Count = 0; Count_Motor = 0;

    TSM12_write_byte(TSM12_CTRL1, 0x00);
    LED_Dispaly(0x00, 0x00); 
    FingerPrint_Send_Off_Led();
    FingerPrint_Enable_Int();  
    FingerPrint_Send_Sleep_Led();
	CARD_Control_Power_Off();
	FINGER_Control_Power_Off();
	ADC_DeInit(ADC1);
	LED_Control_Power_Off();
    Powermanager_DeInit();
    Check_Operation_Of_Main = 0;
	HALT: halt();
    if(Zb_Flag == 1)
    {
        Zb_Flag_Count++;
        Timeout_Sound_Flag = 1;
        Timeout_Flag = 1;
        if(Zb_Flag_Count > 2)
        {
            Zb_Flag_Count = 0;
            delay_ms(100);
        	RESET_CPU();
        	delay_ms(10000);
        }
    }else if(Check_Operation_Of_Main == 0)
    {
        goto HALT;
    }
    
}