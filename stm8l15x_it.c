/**
  ******************************************************************************
  * @file    Project/STM8L15x_StdPeriph_Template/stm8l15x_it.c
  * @author  MCD Application Team
  * @version V1.6.1
  * @date    30-September-2014
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm8l15x_it.h"
#include "stm8l15x_rtc.h"
#include "definepin.h"    
#include "leddisplay.h"
#include "stm8l15x_gpio.h"
#include "stdbool.h"
#include "uart.h"
#include "sleep.h"
#include "ncp.h"
#include "tsm12.h"
#include "adc.h"
#include "define.h"
#include "leddisplay.h"
#include "mainfunctionlist.h"
#include "zb_command.h"
#include "timer.h"
#include "managerpower.h"
#include "fingerprint.h"
#include "delay.h"
#include "drivermotor.h"
#include "mfcv520.h"
#include "audio.h"
#include <stddef.h>

#define Timeout 5000
#define Timeout_Status_Freeze 300000
    
extern volatile uint8_t Check_Motor;
extern bool Check_Status_Success;
volatile uint16_t Timeout_Sound_Count, Start_Count;
volatile uint16_t Count = 0, Count_Motor = 0, Count_Status_Success = 0, Count_Check_Card = 0, Count_Time_Reset = 0;
volatile uint8_t Button_Card_Flag = 0, Finger_Flag = 0, Bell_Flag = 0, Zb_Flag = 0, Switch_Flag = 0, Check_Card_Flag = 0;
volatile uint8_t Timeout_Flag = 0, Sleep_Flag = 1, Timecountcard = 0, Timecountfinger = 0, Fingerflag = 0;
volatile uint16_t Check_Time = 0, Count_Time_Blink = 0, Finger_Flagx = 0, Finger_Timeout = 0;
volatile uint8_t Timeout_Sound_Flag, Temper_Flag = 0, Finger_Timeout_Flag = 0;
volatile uint8_t Enter_Freezen = 0, Start_Flag = 1, Reset_Flag = 0;
volatile uint8_t Check_Faulty_Systm = 0, Smartcard_Flag = 0, Timeout_Baterry = 0;
extern volatile uint8_t DistanceTwiceReadCard, DistanceTwiceReadFingerprint;
extern uint8_t Stateopenstatic;
extern uint8_t zb_flaggetpin, zb_flagsetlock, zb_flagsetpin, zb_flagclearpincode, zb_flaggetallpin;
volatile uint32_t Timecount = 0, Timecount_Freeze = 0, Timedetecterror = 0;
volatile uint8_t Check_Operation_Of_Main = 0, Check_End_Set_Lock = 0;
extern volatile uint8_t Detect_Error_Status;
volatile uint32_t Timeout_CheckBaterry = 0;
volatile uint8_t Motor_Step = 0;

volatile uint16_t Adc_Value = 0;
volatile float Adc_Vol = 0;

// extern uint8_t Fingerprintsendstart[17];

uint8_t Buffer_vtupdatenetworkinfo[] = {0x37, 0xB0, 0x00, 0x00, 0x87};
uint8_t Buffer_vtupdatenetwork[] = {0x36, 0xB0, 0x00, 0x00, 0x86};
uint8_t Buffer_vtupdateallpincode[] = {0x31, 0xB0, 0x00, 0x01, 0x01, 0x81};
uint8_t Buffer_vtupdatesetpincode[] = {0x31, 0xB0, 0x00, 0x01, 0x01, 0x81};
uint8_t Buffer_vtgetpincode[30] = {0x15, 0xB1, 0x00, 0x07, 0x01, 0x31, 0x32, 0x30, 0x38, 0x39, 0x36, 0xA6};
uint8_t Buffer_vtgetpincodeonetime[] = {0x15, 0xB1, 0x00, 0x05, 0x01, 0x24, 0x24, 0x24, 0x24, 0xA0};
uint8_t Buffer_vtgetpincodeonetimedefault[] = {0x15, 0xB1, 0x00, 0x05, 0x01, 0x24, 0x24, 0x24, 0x24, 0xA0};
uint8_t Buffer_vtupdateclearpincode[] = {0x32, 0xB0, 0x00, 0x01, 0x01, 0x82};

uint8_t Buffer_vtlockbyauto[] = {0x30, 0xB0, 0x00, 0x01, 0x1B, 0x9A};
uint8_t Buffer_vtunlockapp[] = {0x30, 0xB0, 0x00, 0x01, 0x19, 0x98};
uint8_t Buffer_vtunlockbyfingerprint[] = {0x57, 0xB0, 0x00, 0x02, 0x02, 0x00, 0xE7};
uint8_t Buffer_vtunlockbypin[] = {0x30, 0xB0, 0x00, 0x02, 0x13, 0x00, 0x92};
uint8_t Buffer_vtunlockbycard[] = {0x30, 0xB0, 0x00, 0x02, 0x16, 0x00, 0x94};

uint8_t Buffer_vtupdatewarningunlock[] = {0x30, 0xB0, 0x00, 0x01, 0xA1, 0x20};
uint8_t Buffer_vtupdatewarninglowbattery[] = {0x30, 0xB0, 0x00, 0x01, 0xA7, 0x26};
uint8_t Buffer_vtupdatepercentbattery[] = {0x1A, 0xB1, 0x00, 0x01, 0x32, 0x98};

// Callback Function

typeIT_HandleIrqExt4CallbackFunc     pv_handleIrqExt4Func = NULL;


void it_handle_irq_ext4_callback_init(
           typeIT_HandleIrqExt4CallbackFunc typeIT_HandleIrqExt4CallbackFuncInit){
   if(typeIT_HandleIrqExt4CallbackFuncInit != NULL){
       pv_handleIrqExt4Func = typeIT_HandleIrqExt4CallbackFuncInit;
   }
}

/** @addtogroup STM8L15x_StdPeriph_Template
  * @{
  */
	
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/

#ifdef _COSMIC_
/**
  * @brief Dummy interrupt routine
  * @par Parameters:
  * None
  * @retval 
  * None
*/
INTERRUPT_HANDLER(NonHandledInterrupt,0)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
#endif

/**
  * @brief TRAP interrupt routine
  * @par Parameters:
  * None
  * @retval 
  * None
*/
INTERRUPT_HANDLER_TRAP(TRAP_IRQHandler)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
/**
  * @brief FLASH Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(FLASH_IRQHandler,1)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
/**
  * @brief DMA1 channel0 and channel1 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(DMA1_CHANNEL0_1_IRQHandler,2)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
/**
  * @brief DMA1 channel2 and channel3 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(DMA1_CHANNEL2_3_IRQHandler,3)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
/**
  * @brief RTC / CSS_LSE Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(RTC_CSSLSE_IRQHandler,4)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
   /* Clear the periodic wakeup unit flag */
    static uint32_t DetectAdcValue = 0;
    static float BaseAdcValue = 0;
    Timeout_CheckBaterry++;
    RTC_ClearITPendingBit(RTC_IT_WUT);

    if(Check_Operation_Of_Main == 0)
    {
        DetectAdcValue++;
        IWDG_ReloadCounter();
        GPIO_Init(GPIOB, GPIO_Pin_1, GPIO_Mode_In_FL_No_IT);
        GPIO_SetBits(MFCVSPI_NRSTPD_GPIO_PORT, MFCVSPI_NRSTPD_PIN);
        
        ADC_Config_Init();
        delay_us(1);

        GPIO_ResetBits(MFCVSPI_NSS_GPIO_PORT, MFCVSPI_NSS_PIN);
        SPI_Write(0x02);
        SPI_Write(0x0F);
        GPIO_SetBits(MFCVSPI_NSS_GPIO_PORT, MFCVSPI_NSS_PIN);

        GPIO_ResetBits(MFCVSPI_NSS_GPIO_PORT, MFCVSPI_NSS_PIN);
        SPI_Write(0x28);
        SPI_Write(0x02);
        GPIO_SetBits(MFCVSPI_NSS_GPIO_PORT, MFCVSPI_NSS_PIN);

        Adc_Value = ADC17_Read();
        ADC_DeInit(ADC1);
        Adc_Vol = Adc_Value*3.3/4096;
    
        if(DetectAdcValue == 1)
        {
            BaseAdcValue = Adc_Vol-0.3; 
        }

        if((Adc_Vol < BaseAdcValue)&&(Adc_Vol > 1.2))
        {
            Check_Operation_Of_Main = 1;
            Smartcard_Flag = 1;
            Timeout_Sound_Flag = 1;
            Timeout_Flag = 1;
            GPIO_ResetBits(MFCVSPI_NRSTPD_GPIO_PORT, MFCVSPI_NRSTPD_PIN);
            delay_us(1);
            GPIO_SetBits(MFCVSPI_NRSTPD_GPIO_PORT, MFCVSPI_NRSTPD_PIN);
            delay_us(1);
            PCD_Config();
            
        }else if(Button_Card_Flag == 1)
        {
            Check_Operation_Of_Main = 1;
            GPIO_ResetBits(MFCVSPI_NRSTPD_GPIO_PORT, MFCVSPI_NRSTPD_PIN);
            delay_us(1);
            GPIO_SetBits(MFCVSPI_NRSTPD_GPIO_PORT, MFCVSPI_NRSTPD_PIN);
            delay_us(1);
        }else
        {
            Check_Operation_Of_Main = 0;
            GPIO_ResetBits(MFCVSPI_NRSTPD_GPIO_PORT, MFCVSPI_NRSTPD_PIN);
            delay_us(1);
        }
        DetectAdcValue = 1;
    }
    if(Timeout_CheckBaterry >= TIMEOUT_BATERRY_DEFAULT)
    {
        // GPIO_ResetBits(GPIOG, TSM12_I2C_enable_pin);
        Timeout_CheckBaterry = 0;
        Check_Operation_Of_Main = 1;
        Timeout_Baterry = 1;
        // TSM12_write_byte(0xE0, 0x00);
        // GPIO_SetBits(GPIOG, TSM12_I2C_enable_pin);

    }
    
}
/**
  * @brief External IT PORTE/F and PVD Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTIE_F_PVD_IRQHandler,5)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
   Switch_Flag = 1; 
   Timeout_Sound_Flag = 1; 
   Timeout_Sound_Count = 0;
   Check_Operation_Of_Main = 1;
   /* Clear It Pending */
   EXTI_ClearITPendingBit(EXTI_IT_PortE);
}

/**
  * @brief External IT PORTB / PORTG Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTIB_G_IRQHandler,6)
{
}

/**
  * @brief External IT PORTD /PORTH Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTID_H_IRQHandler,7)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
  //   Switch_Flag = 1; Timeout_Sound_Flag = 1; Timeout_Sound_Count = 0;
  //  /* Clear It Pending */
  //  EXTI_ClearITPendingBit(EXTI_IT_PortD);
   
}

/**
  * @brief External IT PIN0 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI0_IRQHandler,8)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
    Timeout_Sound_Flag = 1; Zb_Flag = 1;
    Timeout_Flag = 1;
    Check_Operation_Of_Main = 1;
    EXTI_ClearITPendingBit(EXTI_IT_Pin0);
}

/**
  * @brief External IT PIN1 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI1_IRQHandler,9)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
  //  LED_Control_Power_On();
  //  delay_ms(10);
  //  LED_Dispaly(0x40, 0x00); 
  //  EXTI_ClearITPendingBit(EXTI_IT_Pin1);
    // Timeout_Flag = 1; Button_Card_Flag = 1; Timeout_Sound_Flag = 1;
    // Count = 0; Count_Time_Blink = 0; Sleep_Flag = 0;
    // /* Clear It Pending */
    // EXTI_ClearITPendingBit(EXTI_IT_Pin1);

}

/**
  * @brief External IT PIN2 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI2_IRQHandler,10)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
    Switch_Flag = 1; 
    Timeout_Sound_Flag = 1; 
    Timeout_Sound_Count = 0;
    Check_Operation_Of_Main = 1;
    /* Clear It Pending */
    EXTI_ClearITPendingBit(EXTI_IT_Pin2);
}

/**
  * @brief External IT PIN3 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI3_IRQHandler,11)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
    Timeout_Flag = 1; Bell_Flag = 1; Timeout_Sound_Flag = 1;
    Check_Operation_Of_Main = 1;
    /* Clear It Pending */
    EXTI_ClearITPendingBit(EXTI_IT_Pin3);
}

/**
  * @brief External IT PIN4 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI4_IRQHandler,12)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
    Check_Faulty_Systm = 0;
    IWDG_ReloadCounter();
    if(Start_Flag == 0)
    {
        Timeout_Flag = 1; Button_Card_Flag = 1; Timeout_Sound_Flag = 1;
        Count = 0; Count_Time_Blink = 0; Sleep_Flag = 0;
        /* Clear It Pending */
        EXTI_ClearITPendingBit(EXTI_IT_Pin4);
        if(pv_handleIrqExt4Func != NULL) pv_handleIrqExt4Func();  
    }
    else
    {
        /* Clear It Pending */
        EXTI_ClearITPendingBit(EXTI_IT_Pin4);
        if(pv_handleIrqExt4Func != NULL) pv_handleIrqExt4Func();  
    }
    
}

/**
  * @brief External IT PIN5 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI5_IRQHandler,13)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}

/**
  * @brief External IT PIN6 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI6_IRQHandler,14)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
  Temper_Flag = 1;
  Check_Operation_Of_Main = 1;
  /* Clear It Pending */
  EXTI_ClearITPendingBit(EXTI_IT_Pin6);
  
}

/**
  * @brief External IT PIN7 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI7_IRQHandler,15)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
    IWDG_ReloadCounter();
    Finger_Flag = 1;
    Check_Operation_Of_Main = 1;
    Check_Faulty_Systm = 0;
    Timeout_Flag = 1; 
    Timeout_Sound_Flag = 1;
    Count = 0;
    Count_Time_Blink = 0; 
    Sleep_Flag = 0;
    Check_Status_Success = TRUE;
    Count_Status_Success = 0;
    EXTI_ClearITPendingBit(EXTI_IT_Pin7);

}
/**
  * @brief LCD /AES Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(LCD_AES_IRQHandler,16)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
/**
  * @brief CLK switch/CSS/TIM1 break Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(SWITCH_CSS_BREAK_DAC_IRQHandler,17)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}

/**
  * @brief ADC1/Comparator Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(ADC1_COMP_IRQHandler,18)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}

/**
  * @brief TIM2 Update/Overflow/Trigger/Break /USART2 TX Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM2_UPD_OVF_TRG_BRK_USART2_TX_IRQHandler,19)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
    /* Cleat Interrupt Pending bit */
    TIM2_ClearITPendingBit(TIM2_IT_Update);
}

/**
  * @brief Timer2 Capture/Compare / USART2 RX Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM2_CC_USART2_RX_IRQHandler,20)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
    UART_CommonInteruptHandle();
}


/**
  * @brief Timer3 Update/Overflow/Trigger/Break Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM3_UPD_OVF_TRG_BRK_USART3_TX_IRQHandler,21)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
/**
  * @brief Timer3 Capture/Compare /USART3 RX Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM3_CC_USART3_RX_IRQHandler,22)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
/**
  * @brief TIM1 Update/Overflow/Trigger/Commutation Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM1_UPD_OVF_TRG_COM_IRQHandler,23)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
   
    Timecount++;
    if(Timecount >= 600)
    {
        Timecount = 0;
        Check_Faulty_Systm++;
        if(Check_Faulty_Systm >= 100)
        {
            Check_Faulty_Systm = 0;
            if(Check_Operation_Of_Main == 1)
            {
                while(1);
            }else
            {
                IWDG_ReloadCounter();
            }
            
        }else
        {
            IWDG_ReloadCounter();   
        }
    }

    if (Timeout_Flag == 1)
    {
        // Check sleep mode
        Count++;Count_Check_Card++;
        if(Count >= Timeout)
        {
            Count = 0;
            Timeout_Flag = 0;
            Sleep_Flag = 1;
        }
        if(Count_Check_Card == 300)
        {
            Count_Check_Card = 0;
            DistanceTwiceReadCard = 0;
        }

    }

    if(Enter_Freezen == 1)
    {
        Timecount_Freeze++;
        if(Timecount_Freeze >= Timeout_Status_Freeze)
        {
            Timecount_Freeze = 0;
            Enter_Freezen = 0;
        }
    }

    if(Timeout_Sound_Flag == 1)
    {
        if(Start_Count == 1)
            Timeout_Sound_Count++;
    }

    if(Reset_Flag == 1)
    {
        Count_Time_Reset++;
        if(Count_Time_Reset >= 5000)
        {
            Count_Time_Reset = 0;
            Timeout_Flag = 1;
            Count = 0;
            Sleep_Flag = 0;
            Reset_Flag = 0;
        }
    }

    if(Check_Motor == 1)
    {
        if(Stateopenstatic == 0)
        {
            // Check for motor
            Count_Motor++;
            if(zb_flagsetlock == 1)
            {
                if(Count_Motor == 500)
                {
                    Drivermotor_Close_All();
                }else if (Count_Motor == 14500)
                {
                    Drivermotor_Close();
                }else if(Count_Motor == 15000)
                {
                    Check_Motor = 0;
                    Count_Motor = 0;
                    if(Timeout_Flag == 0)
                    {
                        Sleep_Flag = 1;
                    }
                    Drivermotor_Close_All();
                    ZB_Wakeup();
                    NCP_Send(Buffer_vtlockbyauto, sizeof(Buffer_vtlockbyauto));
                    delay_ms(10);
                    Check_End_Set_Lock = 1;
                }

            }else
            {

                if(Count_Motor == 500)
                {
                    Drivermotor_Close_All();
                }else if(Count_Motor == 3500)
                {
                    Drivermotor_Close();
                }else if(Count_Motor == 4000)
                {
                    Check_Motor = 0;
                    Count_Motor = 0;
                    if(Timeout_Flag == 0)
                    {
                        Sleep_Flag = 1;
                    }
                    Drivermotor_Close_All();
                    ZB_Wakeup();
                    NCP_Send(Buffer_vtlockbyauto, sizeof(Buffer_vtlockbyauto));
                    delay_ms(10);
                }

            }
        }else
        {
            // Check for motor
            Count_Motor++;
            if(Count_Motor >= 500)
            {
                Count_Motor = 0;
                Check_Motor = 0;
                Drivermotor_Close_All();
            }
        }
        
    }

    if(Check_Status_Success == false)
    {
        // Check Status Success
        Count_Status_Success++;
        if(Count_Status_Success == 1)
            LED_Dispaly(0x00, 0x00);
        else if(Count_Status_Success == 200)
            LED_Dispaly(0x0F, 0xFF);
        else if(Count_Status_Success == 400)
            LED_Dispaly(0x00, 0x00);
        else if(Count_Status_Success == 600)
            LED_Dispaly(0x0F, 0xFF);
        else if(Count_Status_Success == 800)
            LED_Dispaly(0x00, 0x00);
        else if(Count_Status_Success == 1000)
            LED_Dispaly(0x0F, 0xFF);
        else if(Count_Status_Success == 1200)
        {
            Count_Status_Success = 0;
            Check_Status_Success = true;
            LED_Dispaly(0x00, 0x00);
        }
    }
    
    /* Cleat Interrupt Pending bit */
    TIM1_ClearITPendingBit(TIM1_IT_Update);
}
/**
  * @brief TIM1 Capture/Compare Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM1_CC_IRQHandler,24)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}

/**
  * @brief TIM4 Update/Overflow/Trigger Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM4_UPD_OVF_TRG_IRQHandler,25)
{
    /* In order to detect unexpected events during development,
      it is recommended to set a breakpoint on the following instruction.
    */
  
  TIM_InterruptHandle();
  /* Cleat Interrupt Pending bit */
  TIM4_ClearITPendingBit(TIM4_IT_Update);
}
/**
  * @brief SPI1 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(SPI1_IRQHandler,26)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */		
}

/**
  * @brief USART1 TX / TIM5 Update/Overflow/Trigger/Break Interrupt  routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(USART1_TX_TIM5_UPD_OVF_TRG_BRK_IRQHandler,27)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}

/**
  * @brief USART1 RX / Timer5 Capture/Compare Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(USART1_RX_TIM5_CC_IRQHandler,28)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
    UART_CommonInteruptHandle();
}

/**
  * @brief I2C1 / SPI2 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(I2C1_SPI2_IRQHandler,29)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/