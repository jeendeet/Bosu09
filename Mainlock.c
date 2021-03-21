#include "stm8l15x.h"
#include "stm8l15x_it.h"
#include "stm8l15x_gpio.h"
#include "clock.h"
#include "delay.h"
#include "definepin.h"
#include "timer.h"
#include "Softwaretimer.h"
#include "eeprom.h"
#include "adc.h"
#include "rtc.h"
#include "gpio.h"
#include "leddisplay.h"
#include "drivermotor.h"
#include "managerpower.h"
#include "defaultdevicecode.h"
#include "exti.h"
#include "sleep.h"
#include "tsm12.h"
#include "define.h"
#include "uart.h"
#include "ncp.h"
#include "zb_command.h"
#include "fingerprint_app.h"
#include "fingerprint.h" 
#include "mfcv520.h"
#include "reset.h"
#include "audio.h"
#include "mainfunctionlist.h"


void Function_Init(void);

int main(void)

{ 
    Function_Init();
    while(1)
    {
        Function_Read_Adc_Value();
        Powermanager_Init();
        Function_Check_Warning();
        NCP_Proc();
        TIME_Proc();
        Function_Check_All_Flag();
        FingerPrint_Proc();
        Function_Check_Sleep();
    };
}

void Function_Init(void)
{
    RTC_Config();

    CLK_Init(); 

    GPIO_Init_Config();      

    LED_Init(); 

    TIME1_Init(); 

    TIME4_Init(); 

    TSM12_init();

    it_handle_irq_ext4_callback_init(TSM12_handle_digital_output_falling);
    
    TSM12_CallbackInit(app_main_handle_touch_btn_press);

    I2C_Config();

    Drivermotor_Init(); 
    
    EXTI_Init(); 
    
    NCP_Init(); 
    
    FingerPrint_Init(FingerPrint_AppHandle);

    PCD_Init();      

    ADC_Config_Init();

    SLEEP_Mode_Init();     
    
    EXTI_ClearITPendingBit(EXTI_IT_Pin7);

    RESET_CPU_BY_IWDG();
	
	REGISTER_DEVICE(DEFAULT_DEVICE_CODE);

    AUDIO_GPIO_Init(); 

    AUDIO_Send(BIG_SOUND); 

    AUDIO_Send(SOUND_WHEN_TURN_SWITCH);

    TIME_Out(0x2FFFF);

    enableInterrupts();
	
}




