#include "mainfunctionlist.h"
#include "stm8l15x_it.h"
#include "stm8l15x_gpio.h"
#include "clock.h"
#include "define.h"
#include "definepin.h"
#include "managerpower.h"
#include "delay.h"
#include "Softwaretimer.h"
#include "drivermotor.h"
#include "eeprom.h"
#include "defaultdevicecode.h"
#include "exti.h"
#include "getpassword.h"
#include "leddisplay.h"
#include "sleep.h"
#include "tsm12.h"
#include "audio.h"
#include "adc.h"
#include "uart.h"
#include "ncp.h"
#include "zb_command.h"
#include "fingerprint_app.h"
#include "fingerprint.h" 
#include "timer.h"
#include "reset.h"
#include "mfcv520.h"
#include <stddef.h>
#include <string.h>


/* Private variables -------------------------------------------*/
bool Check_Status_Success = true, Check_Byte_In_Eeprom = FALSE, Check_Password = FALSE;
bool Check_Type_Password = FALSE, Check_Password_Enroll = TRUE, Check_Card_Enroll = TRUE;
bool Check_Card = FALSE, Check_Finger = FALSE, Check_Enroll = FALSE;
bool Check_Hand_Enroll = FALSE, Check_Hand_Search = FALSE, Check_Compare_Finger = FALSE;
bool Check_Finger_Enroll = TRUE;
uint8_t Password_Array[24], Password_Save[24], ID_Clear[5], Password_Array_Again[24];
uint8_t Password_Temp_Array[50];
uint8_t Finger_Default = 1, Numbertesteeprom = 0, Password_Finger = 0, Finger_Temp = 2;
uint8_t Check_Key, Check_Number_Of_Array, Check_Number_Of_Array_Again, Type_Setting, Check_Number_Of_Temp_Array;
uint8_t Temp_ID = 1, Key = 0, ID_Value = 0, Check_Password_False = 0, Finger_Value = 0, Check_Flag_Volume = 0;
uint8_t Numberofpassword = 0, Check_Id = 0, Use_Left_Lock_State = 0, State = 0, Check_Language = 0;
uint8_t Warning_Flag = 0, Stateopenstatic = 0, Eeprom_Flag = 0, Start_First_Flag = 0, Finger_Value_Temp = 0;
uint8_t OpenLock = 0, SettingSystemFlag = 0, Card_Flag = 0, Numberoffnotcorrect = 0, Check_Flag_Volume_MID_SMALL = 0;
uint8_t Check_All_Finger = 0, Check_All_Pin = 0, Check_All_Card = 0, Check_Eeprom_Manager_Or_User = 1, Check_Button = 0, Check_Battery = 0;
uint16_t Temp_ADDRESS = 24, Zb_Flag_Count = 0;
static uint32_t Battery_Value;
uint8_t Readbattery = 0;
volatile uint8_t Check_Motor, Detect_Error_Status;
volatile uint8_t Finger_FlagError = 0, DistanceTwiceReadCard = 0, DistanceTwiceReadFingerprint = 0;

extern NCP_Command_t ncpCommandBuff[NCP_COMMAND_QUEUE_SIZE];
extern uint8_t Fingerprintsendclear[12], Fingerprintsendclearonefinger[16];
extern u8 fingerPrintState;
extern volatile uint8_t Check_Operation_Of_Main, Check_End_Set_Lock;
extern volatile uint8_t zb_flagnetworkinfo, networkstatus, Enter_Freezen, zb_getid_again, Smartcard_Flag;
extern volatile uint8_t Button_Card_Flag, Finger_Flag, Bell_Flag, zb_flagsetlock, Switch_Flag;
extern volatile uint8_t Timeout_Flag, Sleep_Flag, Fingerflag, Temper_Flag, Reset_Flag, Check_Faulty_Systm;
extern volatile uint8_t zb_flagtogglenetwork, zb_flagchangelockstate, zb_flagjoinnetwork, Start_Flag;
extern volatile uint8_t Finger_Timeout_Flag, Zb_Flag, zb_flagsetpin, zb_flagupdatepincode;
extern volatile uint8_t zb_flaggetpin, zb_getid, zb_flagclearpincode, zb_flagchangepin, zb_flaggetallpin;
extern volatile uint8_t Timeout_Baterry, Motor_Step, Timeout_Sound_Flag;
extern uint8_t Buffer_vtupdatenetworkinfo[5];
extern uint8_t Buffer_vtupdatenetwork[5];
extern uint8_t Buffer_vtlockbyauto[6];
extern uint8_t Buffer_vtunlockapp[6];
extern uint8_t Buffer_vtupdatesetpincode[6];
extern uint8_t Buffer_vtunlockbyfingerprint[7];
extern uint8_t Buffer_vtupdateclearpincode[6];
extern uint8_t Buffer_vtunlockbypin[7];
extern uint8_t Buffer_vtunlockbycard[7];
extern uint8_t Buffer_vtgetpincodeonetime[10];
extern uint8_t Buffer_vtgetpincodeonetimedefault[10];
extern uint8_t Buffer_vtupdatewarninglowbattery[6];
extern uint8_t Buffer_vtupdatepercentbattery[6];
extern uint8_t Buffer_vtupdatewarningunlock[7];
extern uint8_t Buffer_vtupdatefingerprint[7];
extern uint8_t Fingerprintenableinterrputstep2[12];

extern uint8_t Buffer_vtgetpincode[30];
extern volatile uint16_t Timeout_Sound_Count, Start_Count, Finger_Timeout, Count_Time_Reset, Count, Count_Motor;
extern volatile uint32_t Timedetecterror;

void Function_Check_All_Flag(void)
{
    if((Button_Card_Flag == 1) || (Finger_Flag == 1) || (zb_flagsetlock == 1) ||\
       (Bell_Flag == 1) || (Switch_Flag == 1) || (zb_flagsetpin == 1) || (zb_flaggetpin == 1)||\
       (zb_flagclearpincode == 1) || (zb_flaggetallpin == 1) || (Smartcard_Flag == 1))  
    {
        //Check All Flag
        if(Smartcard_Flag == 1)
        {
            Function_Check_Card();
        }
        else if (Button_Card_Flag == 1) 
        {   
            Function_Check_Button();
        }
        else if(Finger_Flag == 1)
        {   
            Function_Check_Finger();
        }
        else if(zb_flagsetlock == 1)
        {
            Function_Handsetlock();
            Zb_Flag_Count = 0;
            Sleep_Flag = 1;
        }
        else if(zb_flagsetpin == 1)
        {   
            Function_Handsetpin();
            Zb_Flag_Count = 0;
            Sleep_Flag = 1;
        }
        else if(zb_flaggetpin == 1)
        {
            Function_Handgetpin();
            Zb_Flag_Count = 0;
            Sleep_Flag = 1;
        }
        else if(zb_flagclearpincode == 1)
        {
            Function_Handclearpin();
            Zb_Flag_Count = 0;
            Sleep_Flag = 1;
        }
        else if(zb_flaggetallpin == 1)
        {
            Function_Handgetallpin();
            LED_Notifi_Init_Success(20);
            Function_Hand_Low_Battery();
            Zb_Flag_Count = 0;
            Sleep_Flag = 1;
        }
        else if(Bell_Flag == 1)
        {
            Function_Handbell();
            Sleep_Flag = 1;
        }
        else if(Switch_Flag == 1)
        {
            // Check Switch
            TIME_Out(0x3FFFF);
            if(GPIO_ReadInputDataBit(GPIOE, GPIO_PIN_RESET_INT) == 0)
            {
                Function_Handresetlock();
            }else if(GPIO_ReadInputDataBit(GPIOD, GPIO_PIN_SWITCH_INT) == 0)
            {
                Function_Handswitchlock();
                Sleep_Flag = 1;
            }
        }
    }

    if(Temper_Flag == 1)
    {
        if(GPIO_ReadInputDataBit(GPIOG, GPIO_PIN_TEMPER_INT) == RESET)
        {
            Function_Handtemper();
        }
    }

    if(Timeout_Baterry == 1)
    {
        Timeout_Baterry = 0;
        Timeout_Sound_Flag = 1;
        Function_Hand_Low_Battery();
        Sleep_Flag = 1;
    }
}

void Function_Handtemper(void)
{
    Warning_Flag = 1;
    ZB_Wakeup();
    NCP_Send(Buffer_vtupdatewarningunlock, sizeof(Buffer_vtupdatewarningunlock));
    disableInterrupts();
    while (1)
    {
        AUDIO_Send(SOUND_WHEN_WANING);
        for(uint8_t i = 0; i < 8; i++)
        {
            TIME_Out(0x3FFFF);
            IWDG_ReloadCounter();
        }
    }
}

void Function_Handbell(void)
{
    LED_Dispaly(0x40, 0x00); 
    AUDIO_Press_Send(SOUND_WHEN_PRESS_BELL, MID_MIN_TIME_SOUND);
}

void Function_Handresetlock(void)
{
    Reset_Flag = 1;
    AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER,300);
    while (Reset_Flag == 1);
    LED_Dispaly(0x02, 0x01); // Bat led * 1
    AUDIO_Handle_Send(SUGGEST_ENTER_USER_ORDER_NUMBER, 0, 0, MID_TIME_SOUND);
    Count = 0;
    do
    {
        Key = GET_Key();
        if(Key == '*')
        {
            AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
            Sleep_Flag = 1;
            break;
        }
        else if(Key == '1')
        {
            LED_Dispaly(0x02, 0x02); // Bat led * 2
            AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
            Key = GET_Key();
            if(Key == '*')
            {
                AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
                Sleep_Flag = 1;
                break;
            }
            else if(Key == '2')
            {
                AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
                LED_Dispaly(0x0A, 0x00); // Bat led * #
                Function_Handle_Package_Sound_Step4();
                Timeout_Flag = 1; Count = 0; Sleep_Flag = 0;
                do
                {
                    Key = GET_Key();
                    if(Key == '*')
                    {
                        AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
                        Sleep_Flag = 1;
                        break;
                    }
                    else if(Key == '#')
                    {
                        AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
                        LED_Dispaly(0x00, 0x00);
                        cleanmemory(Password_Array, 0xFF, SIZEOFFRAME);
                        
                        ZB_Wakeup();
                        NCP_Send(Buffer_vtlockbyauto, sizeof(Buffer_vtlockbyauto));
                        delay_ms(10);

                        for(uint16_t i = 0; i < ALL_FRAME_USE_OF_KEY; i++)
                        {
                            Buffer_vtupdateclearpincode[4] = i+1;
                            Buffer_vtupdateclearpincode[5] = 0;
                            for (uint8_t j = 0; j < 5; j++)
                            {
                                Buffer_vtupdateclearpincode[5] ^= Buffer_vtupdateclearpincode[j];
                            }
                            ZB_Wakeup();
                            NCP_Send(Buffer_vtupdateclearpincode, sizeof(Buffer_vtupdateclearpincode));
                            delay_ms(10);
                        }

                        for(uint16_t i = 0; i < ALL_PAGE_USE_COMMON; i++)
                        {
                            WRITER_Page_To_Eeprom(i, 0xFF);
                            delay_ms(5);
                        }

                        WRITER_Byte_To_Eeprom(ADDRESS_SAVE_FINGER, 0xFF);
                        delay_ms(5);
                        WRITER_Byte_To_Eeprom(ADDRESS_SAVE_PIN, 0xFF);
                        delay_ms(5);
                        WRITER_Byte_To_Eeprom(ADDRESS_SAVE_CARD, 0xFF);
                        delay_ms(5);
                        WRITER_Byte_To_Eeprom(ADDRESS_SAVE_STATE_EEPROM, 0xFF);
                        delay_ms(5);
                        WRITER_Byte_To_Eeprom(ADDRESS_LEFT_LOCK_STATE, 0xFF);
                        delay_ms(5);
                        WRITER_Byte_To_Eeprom(ADDRESS_LANGUAGE, 0xFF);
                        delay_ms(5);
                        WRITER_Byte_To_Eeprom(ADDRESS_VOLUME, 0xFF);
                        delay_ms(5);

                        FingerPrint_Send_Clear_All_User();
                        AUDIO_Get_Finger_Send(SUCCESSFULLY_RECOVERY_SYSTEM, MID_TIME_SOUND, Check_Language);
                        Check_Faulty_Systm = 110;
                        while(1);
                    }

                } while ((Key != '#') && (Sleep_Flag == 0));

            }   

        }
    } while ((Key != '*') && (Sleep_Flag == 0));
    
}

void Function_Handswitchlock(void)
{
    ZB_Wakeup(); 
    NCP_Send(Buffer_vtlockbyauto, sizeof(Buffer_vtlockbyauto));
    if(Check_Language == 0)
    {
        AUDIO_Handle_Send(SOUND_WHEN_TURN_SWITCH, 0, 0, MIN_TIME_SOUND);
    }else
    {
        AUDIO_Handle_Send(SOUND_WHEN_TURN_SWITCH-1, 0, 0, MIN_TIME_SOUND);
    }
    Zb_Flag = 0;
    if(Stateopenstatic == 1)
    {
        Drivermotor_Close();
        Check_Motor = 1;
        while (Check_Motor == 1);
        AUDIO_Handle_Send(HAS_REMOVE_MOVED_TO_FIXED_STATE, 0, 0, MID_TIME_SOUND);
        Stateopenstatic = 0;
    }
    Sleep_Flag = 1;
    
}

void Function_Handsetlock(void)
{
    Numberoffnotcorrect = 0;
    if(Stateopenstatic == 0)
    {
        ZB_Wakeup();
        NCP_Send(Buffer_vtunlockapp, sizeof(Buffer_vtunlockapp));
        FingerPrint_Send_Success_Led();
        Drivermotor_Open(); 
        Check_Motor = 1;

        GPIO_Init(GPIOG, GPIO_PIN_FINGER_INT, GPIO_Mode_In_FL_No_IT);

        AUDIO_Get_Finger_Send(VERIFY_SUCCESSFUL, 16000, Check_Language);

        LED_Dispaly(0x00, 0x00);
        FingerPrint_Send_Off_Led();
        
        GPIO_Init(GPIOG, GPIO_PIN_FINGER_INT, GPIO_Mode_In_FL_IT);
        Zb_Flag = 0;
    }else
    {
        Function_Hand_Low_Battery();
        Sleep_Flag = 1;
    }

}

void Function_Handsetpin(void)
{
    if(zb_flagchangepin == 0)
    {
        Check_Number_Of_Array = Function_Check_Number_Of_Array(Password_Array);
        if(Check_Number_Of_Array != 0)
        {
            Key = NULL;
            Check_Password_Enroll = Function_Check_Password_In_Eeprom(Password_Array);
            if(Check_Password_Enroll == FALSE)
            {
                if(zb_getid > 25)
                {
                    AUDIO_Press_Send(PASSWORD_WAS_FULL, NORMAL_TIME_SOUND);
                    Sleep_Flag = 1;
                }else
                {
                    Buffer_vtupdatesetpincode[4] = zb_getid;
                    Buffer_vtupdatesetpincode[5] = 0;
                    for(uint8_t i = 0; i < 5; i++)
                    {
                        
                        Buffer_vtupdatesetpincode[5] ^= Buffer_vtupdatesetpincode[i];
                    }
                    ZB_Wakeup();
                    NCP_Send(Buffer_vtupdatesetpincode, sizeof(Buffer_vtupdatesetpincode));
                    FingerPrint_Send_Success_Led();

                    Temp_ADDRESS = ADDRESS_KEY_FIRST + SIZEOFFRAME*(zb_getid - 1);
                    Temp_ID = zb_getid;

                    if(Temp_ID == 25)
                    {
                        for(uint8_t i = 4; i < SIZEOFFRAME; i++)
                        {
                            Password_Array[i] = 0x00;
                        }
                    }

//                    Function_Save_Password_User_Into_Eeprom(Password_Array);
					if(Check_Eeprom_Manager_Or_User == 1)
					{
						Function_Save_Password_User_Into_Eeprom(Password_Array);
					}else if(Check_Eeprom_Manager_Or_User == 2)
					{
						Function_Save_Password_Manager_Into_Eeprom(Password_Array);
						Check_Eeprom_Manager_Or_User = 1;
					}
                	cleanmemory(Password_Array, NULL, SIZEOFFRAME);  
                }

            }else
            {
                Sleep_Flag = 1;
            }
        }
    }else
    {
        uint16_t Address_Read_Password = 0;
        zb_flagchangepin = 0;
        Address_Read_Password = ADDRESS_KEY_FIRST + SIZEOFFRAME*(zb_getid -1);
        Check_Number_Of_Array = Function_Check_Number_Of_Array(Password_Array);
        if(Check_Number_Of_Array != 0)
        {
            Key = NULL;
            Check_Password_Enroll = Function_Check_Password_In_Eeprom_Again(Password_Array);
            if(Check_Password_Enroll == FALSE)
            {
                Buffer_vtupdatesetpincode[4] = zb_getid;
                Buffer_vtupdatesetpincode[5] = 0;
                for(uint8_t i = 0; i < 5; i++)
                {
                    Buffer_vtupdatesetpincode[5] ^= Buffer_vtupdatesetpincode[i];
                }
                ZB_Wakeup();
                NCP_Send(Buffer_vtupdatesetpincode, sizeof(Buffer_vtupdatesetpincode));
                FingerPrint_Send_Success_Led();

                cleanmemory(Password_Save, 0xFF, SIZEOFFRAME);
                WRITER_Frame_To_Eeprom(Address_Read_Password, Password_Save, SIZEOFFRAME);
                cleanmemory(Password_Save, NULL, SIZEOFFRAME); 

                Temp_ID = zb_getid;
                Temp_ADDRESS = Address_Read_Password;

                if(Temp_ID == 25)
                {
                    for(uint8_t i = 4; i < SIZEOFFRAME; i++)
                    {
                        Password_Array[i] = 0x00;
                    }
                }

                if(Check_Eeprom_Manager_Or_User == 1)
                {
                    Function_Save_Password_User_Into_Eeprom(Password_Array);
                }else if(Check_Eeprom_Manager_Or_User == 2)
                {
                    Function_Save_Password_Manager_Into_Eeprom(Password_Array);
                    Check_Eeprom_Manager_Or_User = 1;
                }
                
                cleanmemory(Password_Array, NULL, SIZEOFFRAME);
            }else
            {
                Sleep_Flag = 1;
            }
        }
    }
}

void Function_Handgetallpin(void)
{
    for(uint16_t i = 0; i < ALL_FRAME_USE_OF_KEY; i++)
    {
        Temp_ID =  Function_Read_Id(ADDRESS_KEY_FIRST+SIZEOFFRAME*i, SIZEOFFRAME);
        if(Temp_ID == 0)
        {
            continue;
        }
        else
        {
            Buffer_vtupdatesetpincode[4] = Temp_ID;
            Buffer_vtupdatesetpincode[5] = 0;
            for(uint8_t j = 0; j < 5; j++)
            {
                Buffer_vtupdatesetpincode[5] ^= Buffer_vtupdatesetpincode[j];
            }
            ZB_Wakeup();
            NCP_Send(Buffer_vtupdatesetpincode, sizeof(Buffer_vtupdatesetpincode));   
        }
        
    }


}

uint8_t Function_Read_Id(uint16_t ReadAddr, uint8_t Size_Of_Frame)
{
    cleanmemory(Password_Save, NULL, SIZEOFFRAME);
    Numberofpassword =  READ_Frame_From_Eeprom(Password_Save, ReadAddr, SIZEOFFRAME);
    if(Password_Save[0] == 0xFF)
    {
        return 0;
    }
    else
    {
        return Password_Save[Numberofpassword];
    }
}

void Function_Handgetpin(void)
{
    uint16_t Address_Read_Password = 0;
    uint8_t Lenght_Password = 0;
    if(zb_getid < 25)
    {
        Address_Read_Password = ADDRESS_KEY_FIRST + SIZEOFFRAME*(zb_getid -1);
        Numberofpassword = READ_Frame_From_Eeprom(Password_Array, Address_Read_Password, SIZEOFFRAME);
        
        Check_Eeprom_Manager_Or_User = Password_Array[Numberofpassword+1];

        Buffer_vtgetpincode[3] = Numberofpassword+1;
        Buffer_vtgetpincode[4] = 0x01;
        Lenght_Password = Numberofpassword + 6;
        Buffer_vtgetpincode[Lenght_Password-1] = 0;

        for(uint8_t i = 5; i < Lenght_Password-1; i++)
        {
            Buffer_vtgetpincode[i] = Password_Array[i-5];
        }
        for (uint8_t i = 0; i < Lenght_Password-1; i++)
        {
            Buffer_vtgetpincode[Lenght_Password-1] ^= Buffer_vtgetpincode[i];
        }
        
        ZB_Wakeup();
        NCP_Send(Buffer_vtgetpincode, Lenght_Password);
        cleanmemory(Password_Array, NULL, SIZEOFFRAME); 
        cleanmemory(Buffer_vtgetpincode+3, NULL, 27);
    }else
    {
        Address_Read_Password = ADDRESS_KEY_FIRST + SIZEOFFRAME*24;
        Numberofpassword = READ_Frame_From_Eeprom(Password_Array, Address_Read_Password, SIZEOFFRAME);
        Lenght_Password = sizeof(Buffer_vtgetpincodeonetime);
        if(Password_Array[0] == 0xFF)
        {
            ZB_Wakeup();
            NCP_Send(Buffer_vtgetpincodeonetimedefault, sizeof(Buffer_vtgetpincodeonetimedefault));
        }else
        {
            Buffer_vtgetpincodeonetime[Lenght_Password-1] = 0;
            for(uint8_t i = 0; i < 4; i++)
            {
                Buffer_vtgetpincodeonetime[5+i] = Password_Array[i];
            }
            for(uint8_t i = 0; i < Lenght_Password-1; i++)
            {
                Buffer_vtgetpincodeonetime[Lenght_Password-1] ^= Buffer_vtgetpincodeonetime[i]; 
            }
            ZB_Wakeup();
            NCP_Send(Buffer_vtgetpincodeonetime, Lenght_Password);
        }
        
    }
    

}

void Function_Handclearpin(void)
{
    uint16_t Address_Read_Password = 0;
    if(zb_getid == 0xF0)
    {
        zb_getid = 25;
    }
    Address_Read_Password = ADDRESS_KEY_FIRST + SIZEOFFRAME*(zb_getid -1);
    cleanmemory(Password_Array, 0xFF, SIZEOFFRAME);
    WRITER_Frame_To_Eeprom(Address_Read_Password, Password_Array, SIZEOFFRAME);
    cleanmemory(Password_Array, NULL, SIZEOFFRAME); // Xoa tat ca key da nhap
    AUDIO_Handle_Send(OPERATION_SUCCESSFUL, 0, 0, NORMAL_TIME_SOUND);
    
    Buffer_vtupdateclearpincode[4] = zb_getid;
    Buffer_vtupdateclearpincode[5] = 0;
    for (uint8_t i = 0; i < 5; i++)
    {
        Buffer_vtupdateclearpincode[5] ^= Buffer_vtupdateclearpincode[i];
    }
    
    ZB_Wakeup();
    NCP_Send(Buffer_vtupdateclearpincode, sizeof(Buffer_vtupdateclearpincode));

}

bool Function_Check_Card(void)
{
    memset(Password_Array, 0, sizeof(Password_Array));
    READ_Card(Password_Array);
    Check_Number_Of_Array = Function_Check_Number_Of_Array(Password_Array);
    Check_Byte_In_Eeprom = Function_Check_AllByte_Use_In_Eeprom();
    if(Password_Array[0] == NULL)
    {
        Sleep_Flag = 1;
        Timeout_Flag = 0;
        Count = 0;
        return FALSE;
    }else
    {
        if(Check_Byte_In_Eeprom == FALSE)
        {
            // eeprom khong co data
            Eeprom_Flag = 1;
            LED_Dispaly(0x08, 0x00);
            FingerPrint_Send_Success_Led();
            Drivermotor_Open(); Check_Motor = 1;
            Function_Handle_Package_Sound_Step0_0();
            if(Key == '#')
            {
                Key = 0;
                AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
                FingerPrint_Send_Off_Led();
                Function_Handle_Setting_Resgister();
            }else
            {
                Eeprom_Flag = 0;
                Sleep_Flag = 1;
            }
            return TRUE;
        }else
        {
            // eeprom co data
            Check_Card = Function_Handle_Card(Password_Array);
            if(Check_Card == FALSE)
            {
                FingerPrint_Send_Fail_Led();
                Check_Status_Success = false;
            }
            Function_Hand_Low_Battery();
            if(Check_Card == TRUE)
            {   
                // Card chinh xac
                Function_Handle_Card_Correct();
                Button_Card_Flag = 0;
                return TRUE;

            }else
            {
                // Card khong chinh xac
                Function_Handle_Card_Not_Correct();
                return TRUE;
            }   
        }
    }
        
}

bool Function_Handle_Card(uint8_t* Password_Array)
{
    for(uint16_t i = 0; i < ALL_FRAME_USE_OF_CARD; i++)
    {
        READ_Frame_From_Eeprom(Password_Save, ADDRESS_CARD_FIRST+SIZEOFFRAME*i, SIZEOFFRAME);
        if(memcmp(Password_Array, Password_Save, 4) == 0)
        {
            Check_Id = Password_Save[4];
            Buffer_vtunlockbycard[5] = Check_Id;
            Buffer_vtunlockbycard[6] = 0;
            for(uint8_t i = 0; i < 6; i++)
            {
                Buffer_vtunlockbycard[6] ^= Buffer_vtunlockbycard[i];
            }
            cleanmemory(Password_Array, NULL, SIZEOFFRAME);
            return TRUE;
        }
    }
    return FALSE;
}

void Function_Handle_Card_Correct(void)
{
    Card_Flag = 1; Numberoffnotcorrect = 0;
    FingerPrint_Send_Success_Led();
    Check_Password_False = 0;
    Check_Type_Password = Function_Check_Card_Manager_Or_User(Check_Number_Of_Array);
    
    if(Check_Type_Password == TRUE)
    {
        ZB_Wakeup();
        NCP_Send(Buffer_vtunlockbycard, sizeof(Buffer_vtunlockbycard));
    }else
    {
        if(Use_Left_Lock_State == 1)
        {
            if(GPIOD->IDR & GPIO_PIN_SWITCH_INT)
            {
                ZB_Wakeup();
                NCP_Send(Buffer_vtunlockbycard, sizeof(Buffer_vtunlockbycard));
            }
    
        }else
        {
            ZB_Wakeup();
            NCP_Send(Buffer_vtunlockbycard, sizeof(Buffer_vtunlockbycard));
        }
        
    }

    Function_Notification_Status();
    if(Check_Type_Password == TRUE)
    {
        // Card is manager
        Function_Handle_Card_Manager();
    }
    else
    {
        // Card is user
        Function_Handle_Card_User();
    }
}

bool Function_Check_Card_Manager_Or_User(uint8_t Check_Number_Of_Array)
{
    if(Password_Save[Check_Number_Of_Array + 1] == MANAGER) // Kiem tra co phai la Manager hay khong
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

void Function_Handle_Card_Manager(void)
{
    do
    {
        if(Key == '*')
        {
            Stateopenstatic = 0;
            FingerPrint_Send_Off_Led();
            AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
            LED_Dispaly(0x00, 0x00);
            Function_Notification_Id();
            Sleep_Flag = 1;
            break;
        }
        else if(Key == '#')
        {
            FingerPrint_Send_Off_Led();
            AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
            Function_Handle_Setting();
            break;
        }
        else if(Key == '6')
        {
            AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
            LED_Dispaly(0x0A, 0x80); // Bat led * # 8
            FingerPrint_Send_Off_Led();
            do
            {
                Key = GET_Key_8();
                if(Key == '*')
                {
                    Stateopenstatic = 0;
                    AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
                    Function_Notification_Id();
                    Sleep_Flag = 1;
                    break;
                }
                else if(Key == '#')
                {
                    AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
                    Function_Handle_Setting();
                    break;
                }
                else if(Key == '8')
                {
                    Stateopenstatic = 1;
                    AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
                    LED_Dispaly(0x00, 0x00);
                    Check_Motor = 0; Count_Motor = 0;
                    AUDIO_Handle_Send(HAS_MOVED_TO_FIXED_STATE, 0, 0, MID_TIME_SOUND);
                    Sleep_Flag = 1;
                }
                else
                {
                    LED_Dispaly(0x00, 0x00);
                    Sleep_Flag = 1;
                }
                
            } while ((Key != '*') && (Sleep_Flag == 0) && (Key != '#'));
            Sleep_Flag = 1;
        }
        else
        {
            Key = GET_Key();
        }
    } while ((Key != '*') && (Sleep_Flag == 0) && (Key != '#'));
    
}

void Function_Handle_Card_User(void)
{
    do
    {
        Key = GET_Key();
        if(Key == '*')
        {
            FingerPrint_Send_Off_Led();
            AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
            LED_Dispaly(0x00, 0x00);
            Function_Notification_Id();
            Sleep_Flag = 1;
        }

    } while ((Key != '*') && (Sleep_Flag == 0));
    
}

void Function_Handle_Card_Not_Correct(void)
{
    Check_Password_False++;
    if(Check_Password_False == 3)
    {
        Check_Password_False = 0;
        Enter_Freezen = 1;
        ZB_Wakeup();
        NCP_Send(Buffer_vtupdatewarningunlock, sizeof(Buffer_vtupdatewarningunlock));
        LED_Dispaly(0x00, 0x00);
        FingerPrint_Send_Off_Led();
        AUDIO_Handle_Send(KEYBOARD_IS_LOCKED, 0, 0, NORMAL_TIME_SOUND);
        Button_Card_Flag = 0;
        LED_Control_Power_Off();
        while(Enter_Freezen == 1)
        {
            Check_Faulty_Systm = 0;
            if(GPIO_ReadInputDataBit(GPIOG, GPIO_PIN_TEMPER_INT) == RESET)
            {
                break;
            }
        }
        LED_Control_Power_On();
        if(Enter_Freezen == 0)
        {
            Sleep_Flag = 1;
        }else
        {
            Sleep_Flag = 0;
        }
        
    }
    else
    {
        FingerPrint_Send_Fail_Led();
        AUDIO_Get_Finger_Send(VERIFY_NOT_SUCCESSFUL, MID_TIME_SOUND, Check_Language);
        Button_Card_Flag = 0;
    }
}

void Function_Check_Button(void)
{
    Check_Button++;
    TIME_Out(0x7FFF);
    LED_Dispaly(0x0F, 0xFF); // Bat tat ca led ban phim
    if(Check_Button == 1)
    {
        Function_Hand_Low_Battery(); 
    }
    if((Check_Flag_Volume == 0)&&(Battery_Value > LOW_BATTERY_VALUE))
        AUDIO_Press_Send(SOUND_WHEN_TOUCHING_KEYBOARD_FIRST_TIME, 500);
    Check_Key = GET_Password(Password_Array); // Doc key
    LED_Dispaly(0x0F, 0xFF); // Bat tat ca led ban phim
    Check_Number_Of_Array = Function_Check_Number_Of_Array(Password_Array); //Kiem tra so phan tu nhap vao
    Check_Number_Of_Temp_Array = Function_Check_Number_Of_Temp_Array(Password_Temp_Array);
    if(Check_Key == REPEAT)// Kiem tra xem # co duoc nhan hay khong
    {
        Check_Byte_In_Eeprom = Function_Check_AllByte_Use_In_Eeprom();
        if(Check_Byte_In_Eeprom == FALSE)// eeprom khong co data
        {
            Eeprom_Flag = 1;
            if(Password_Array[0] == 0)
            {
                LED_Dispaly(0x02, 0x00);
                FingerPrint_Send_Check_Led();
                Key = AUDIO_Finger_First_Send(SUGGEST_PLACING_FINGERPRINT, HIGHT_TIME_SOUND, Check_Language);
                if(Key == 0)
                {
                    Eeprom_Flag = 0;
                    Sleep_Flag = 1;
                }else if(Key == 2)
                {
                    AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
                    Key = 0;
                    Eeprom_Flag = 0;
                    Sleep_Flag = 1;
                }else if(Key == 1)
                {
                    FingerPrint_Send_Success_Led();
                    Drivermotor_Open(); Check_Motor = 1;

                    LED_Dispaly(0x08, 0x00);
                    Function_Handle_Package_Sound_Step0_0();
                    if(Key == '#')
                    {
                        Key = 0;
                        AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
                        FingerPrint_Send_Off_Led();
                        Function_Handle_Setting_Resgister();
                    }else
                    {
                        Eeprom_Flag = 0;
                        Sleep_Flag = 1;
                    }  
                }
                

            }else
            {
                LED_Dispaly(0x08, 0x00);
                FingerPrint_Send_Success_Led();
                Drivermotor_Open(); Check_Motor = 1;
                Function_Handle_Package_Sound_Step0_0();
                if(Key == '#')
                {
                    Key = 0;
                    AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
                    FingerPrint_Send_Off_Led();
                    Function_Handle_Setting_Resgister();
                }else
                {
                    Eeprom_Flag = 0;
                    Sleep_Flag = 1;
                }   
            }
            
        }else// eeprom co data
        {
            Check_Password = Function_Check_Password(Password_Array); // So sanh password
            if(Check_Password == TRUE)// password chinh xac
            {
                Function_Handle_Password_Correct();
            }
            else// Password khong chinh xac
            {
                Function_Handle_Password_Not_Correct();
            }       
        }
    }else if(Check_Key == BACK)  // Kiem tra xem * co duoc nhan hay khong
    {
        LED_Dispaly(0x00, 0x00); 
        Sleep_Flag = 1; // vao sleep mode
    }
    Button_Card_Flag = 0;    
}

uint8_t Function_Check_Number_Of_Array(uint8_t* array)
{
    for(uint8_t i = 0; i < SIZEOFFRAME; i++) // Tim so phan tu co trong mang
    {
        if(array[i] == NULL)
        {
            return i;
        }
    }
    return 0;
}

uint8_t Function_Check_Number_Of_Temp_Array(uint8_t* array)
{
    for(uint8_t i = 0; i < 50; i++) // Tim so phan tu co trong mang
    {
        if(array[i] == NULL)
        {
            return i;
        }
    }
    return 0;
}

bool Function_Check_AllByte_Use_In_Eeprom(void)
{
    static uint8_t Check_Eeprom;
    Check_Eeprom = READ_Byte_From_Eeprom(ADDRESS_SAVE_STATE_EEPROM);
    if(Check_Eeprom == 0xFF) return FALSE;
    else return TRUE;
}

void Function_Save_Password_into_Eepprom(uint8_t* Password_Array)
{
    for(uint8_t i = 0; i < SIZEOFFRAME; i++)
    {
        if(Password_Array[i] == NULL)
        {
            Password_Array[i] = ID; Password_Array[i+1] = MANAGER; Password_Array[i+2] = END_FRAME;
            for(uint8_t j = i+3; j < SIZEOFFRAME; j++)
            {
                Password_Array[j] = 0xFF;
            }
            WRITER_Frame_To_Eeprom(ADDRESS_KEY_FIRST, Password_Array, Check_Number_Of_Array+3); // Luu password vao eeprom
            cleanmemory(Password_Array, NULL, SIZEOFFRAME); // Xoa tat ca key da nhap
            FingerPrint_Send_Success_Led();
            AUDIO_Handle_Send(VERIFY_SUCCESSFUL, 0, 0, NORMAL_TIME_SOUND);
            Sleep_Flag = 1;
            break;
        }
    }
}

bool Function_Check_Password(uint8_t* Password_Array)
{
    for(uint16_t i = 0; i < ALL_FRAME_USE_OF_KEY; i++)
    {
        Numberofpassword = READ_Frame_From_Eeprom(Password_Save, ADDRESS_KEY_FIRST+SIZEOFFRAME*i, SIZEOFFRAME);
        if(Function_Compare_Password(Password_Save, Password_Array))
        {
            Check_Id = Password_Save[Numberofpassword];
            Buffer_vtunlockbypin[5] = Check_Id;
            Buffer_vtunlockbypin[6] = 0;
            for(uint8_t i = 0; i < 6; i++)
            {
                Buffer_vtunlockbypin[6] ^= Buffer_vtunlockbypin[i];
            }
            cleanmemory(Password_Array, NULL, SIZEOFFRAME); // Xoa tat ca key da nhap
            if(Check_Id == 25)
            {
                Buffer_vtupdateclearpincode[4] = Check_Id;
                Buffer_vtupdateclearpincode[5] = 0;
                for (uint8_t i = 0; i < 5; i++)
                {
                    Buffer_vtupdateclearpincode[5] ^= Buffer_vtupdateclearpincode[i];
                }
                ZB_Wakeup();
                NCP_Send(Buffer_vtupdateclearpincode, sizeof(Buffer_vtupdateclearpincode));

                Temp_ADDRESS = ADDRESS_KEY_FIRST+SIZEOFFRAME*24;
                cleanmemory(Password_Array, 0xFF, SIZEOFFRAME);
                WRITER_Frame_To_Eeprom(Temp_ADDRESS, Password_Array, SIZEOFFRAME);
                cleanmemory(Password_Array, NULL, SIZEOFFRAME); 
                Check_Id = 0;

                return TRUE;
            }
            else
            {
                return TRUE;   
            }
        }
    }
    return FALSE;
    
}

bool Function_Compare_Password(uint8_t *Password_Save, uint8_t *Password_Enter)
{
    uint8_t Count = 0, Temp = 0;
	uint8_t Sizeofpasswordsave = Numberofpassword;
	uint8_t Sizeofpasswordenter = Check_Number_Of_Array;
	for (uint8_t i = 0; i < Sizeofpasswordenter; i++)
	{
		if (Password_Enter[i] == Password_Save[Temp])
		{
			Count++; Temp = Temp + 1;
			if(Count == Sizeofpasswordsave)
			{
				return TRUE;
			}
		}
		else
		{
            if(Count != 0)
			{
                i = i - Count;
			}
			Count = 0; Temp = 0; 
		}
	}
	return FALSE;
}


bool Function_Check_Password_Manager_Or_User(uint8_t Check_Number_Of_Array)
{
    if(Password_Save[Numberofpassword + 1] == MANAGER) // Kiem tra co phai la Manager hay khong
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
    
}

void Function_Handle_Password_Not_Correct(void)
{
    Check_Password_False++;
    if(Check_Password_False == 3)
    {
        Check_Password_False = 0;
        Enter_Freezen = 1;
        ZB_Wakeup();
        NCP_Send(Buffer_vtupdatewarningunlock, sizeof(Buffer_vtupdatewarningunlock));
        LED_Dispaly(0x00, 0x00);
        FingerPrint_Send_Off_Led();
        AUDIO_Handle_Send(KEYBOARD_IS_LOCKED, 0, 0, NORMAL_TIME_SOUND);
        LED_Control_Power_Off();
        while(Enter_Freezen == 1)
        {
            Check_Faulty_Systm = 0;
            if(GPIO_ReadInputDataBit(GPIOG, GPIO_PIN_TEMPER_INT) == RESET)
            {
                break;
            }
        }
        LED_Control_Power_On();
        if(Enter_Freezen == 0)
        {
            Sleep_Flag = 1;
        }
        else
        {
            Sleep_Flag = 0;
        }
        
    }
    else
    {
        FingerPrint_Send_Fail_Led();
        Check_Status_Success = false;
        AUDIO_Get_Finger_Send(VERIFY_NOT_SUCCESSFUL, MID_TIME_SOUND, Check_Language);
    }
}

void Function_Handle_Password_Correct(void)
{
    Numberoffnotcorrect = 0;
    FingerPrint_Send_Success_Led();
    Check_Password_False = 0;
    Check_Type_Password = Function_Check_Password_Manager_Or_User(Check_Number_Of_Array);
    
    if(Check_Type_Password == TRUE)
    {
        ZB_Wakeup();
        NCP_Send(Buffer_vtunlockbypin, sizeof(Buffer_vtunlockbypin));
    }else
    {
        if(Use_Left_Lock_State == 1)
        {
            if(GPIOD->IDR & GPIO_PIN_SWITCH_INT)
            {
                ZB_Wakeup();
                NCP_Send(Buffer_vtunlockbypin, sizeof(Buffer_vtunlockbypin));
            }
    
        }else
        {
            ZB_Wakeup();
            NCP_Send(Buffer_vtunlockbypin, sizeof(Buffer_vtunlockbypin));
        }
        
    }
    
    Function_Notification_Status();
    if(Check_Type_Password == TRUE)
    {
        // Password is manager
        Function_Handle_Password_Manager();
    }
    else
    {
        // Password is user
        Function_Handle_Password_User();
    }
    
        
}

void Function_Handle_Password_User(void)
{
    do
    {   
        Key = GET_Key();
        if(Key == '*')
        {
            FingerPrint_Send_Off_Led();
            AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER,300);
            LED_Dispaly(0x00, 0x00);
            Function_Notification_Id();
            Sleep_Flag = 1;
        }

    } while ((Key != '*') && (Sleep_Flag == 0));
    
}

void Function_Notification_Id(void)
{
    uint8_t temp;
    uint8_t odr1 = 0, odr2 = 0;
    temp = Check_Id;
    odr1 = temp/10;
    odr2 = temp%10;
    AUDIO_Get_Finger_Send(USER_ORDER_NUMBER, MID_MIN_TIME_SOUND, Check_Language);

    switch(odr1)
    {
    case 0:
        AUDIO_Get_Finger_Send(SPEAK_0, MIN_TIME_SOUND, Check_Language);
        break;
    case 1:
        AUDIO_Get_Finger_Send(SPEAK_1, MIN_TIME_SOUND, Check_Language);
        break;
    case 2:
        AUDIO_Get_Finger_Send(SPEAK_2, MIN_TIME_SOUND, Check_Language);
        break;
    case 3:
        AUDIO_Get_Finger_Send(SPEAK_3, MIN_TIME_SOUND, Check_Language);
        break;
    case 4:
        AUDIO_Get_Finger_Send(SPEAK_4, MIN_TIME_SOUND, Check_Language);
        break;
    case 5:
        AUDIO_Get_Finger_Send(SPEAK_5, MIN_TIME_SOUND, Check_Language);
        break;
    case 6:
        AUDIO_Get_Finger_Send(SPEAK_6, MIN_TIME_SOUND, Check_Language);
        break;
    case 7:
        AUDIO_Get_Finger_Send(SPEAK_7, MIN_TIME_SOUND, Check_Language);
        break;
    case 8:
        AUDIO_Get_Finger_Send(SPEAK_8, MIN_TIME_SOUND, Check_Language);
        break;
    case 9:
        AUDIO_Get_Finger_Send(SPEAK_9, MIN_TIME_SOUND, Check_Language);
        break;
    default:
        break;
    }

    switch(odr2)
    {
    case 0:
        AUDIO_Get_Finger_Send(SPEAK_0, MIN_TIME_SOUND, Check_Language);
        break;
    case 1:
        AUDIO_Get_Finger_Send(SPEAK_1, MIN_TIME_SOUND, Check_Language);
        break;
    case 2:
        AUDIO_Get_Finger_Send(SPEAK_2, MIN_TIME_SOUND, Check_Language);
        break;
    case 3:
        AUDIO_Get_Finger_Send(SPEAK_3, MIN_TIME_SOUND, Check_Language);
        break;
    case 4:
        AUDIO_Get_Finger_Send(SPEAK_4, MIN_TIME_SOUND, Check_Language);
        break;
    case 5:
        AUDIO_Get_Finger_Send(SPEAK_5, MIN_TIME_SOUND, Check_Language);
        break;
    case 6:
        AUDIO_Get_Finger_Send(SPEAK_6, MIN_TIME_SOUND, Check_Language);
        break;
    case 7:
        AUDIO_Get_Finger_Send(SPEAK_7, MIN_TIME_SOUND, Check_Language);
        break;
    case 8:
        AUDIO_Get_Finger_Send(SPEAK_8, MIN_TIME_SOUND, Check_Language);
        break;
    case 9:
        AUDIO_Get_Finger_Send(SPEAK_9, MIN_TIME_SOUND, Check_Language);
        break;
    default:
        break;
    }
    
}

void Function_Handle_Password_Manager(void)
{
    do
    {
        if(Key == '*')
        {
            Stateopenstatic = 0;
            FingerPrint_Send_Off_Led();
            AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
            LED_Dispaly(0x00, 0x00);
            Function_Notification_Id();
            Sleep_Flag = 1;
            break;
        }
        else if(Key == '#')
        {
            FingerPrint_Send_Off_Led();
            AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
            Function_Handle_Setting();
            break;
        }
        else if(Key == '6')
        {
            Key = 0;
            AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
            LED_Dispaly(0x0A, 0x80); // Bat led * # 8
            FingerPrint_Send_Off_Led();
            do
            {
                Key = GET_Key_8();
                switch (Key)
                {
                case '0':
                    Key = 0;
                    break;
                case '1':
                    Key = 0;
                    break;
                case '2':
                    Key = 0;
                    break;
                case '3':
                    Key = 0;
                    break;
                case '4':
                    Key = 0;
                    break;
                case '5':
                    Key = 0;
                    break;
                case '6':
                    Key = 0;
                    break;
                case '7':
                    Key = 0;
                    break;
                case '9':
                    Key = 0;
                    break;
                default:
                    break;
                }
            }
            while((Key == 0) && (Sleep_Flag == 0) && (Count_Motor != 0));
            if(Key == '*')
            {
                Stateopenstatic = 0;
                AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
                Function_Notification_Id();
                Sleep_Flag = 1;
                break;
            }
            else if(Key == '#')
            {
                AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
                Function_Handle_Setting();
                break;
            }
            else if(Key == '8')
            {
                Stateopenstatic = 1;
                AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
                LED_Dispaly(0x00, 0x00);
                Check_Motor = 0; Count_Motor = 0;
                AUDIO_Handle_Send(HAS_MOVED_TO_FIXED_STATE, 0, 0, MID_TIME_SOUND);
                Sleep_Flag = 1;
            }
            else
            {
                LED_Dispaly(0x00, 0x00);
                Sleep_Flag = 1;
            }
            
        }
    } while ((Key != '*') && (Sleep_Flag == 0) && (Key != '#'));
        
}

uint8_t Function_Handle_Setting(void)
{
    LED_Dispaly(0x0A, 0x1F); // Bat led * #, 1, 2, 3, 4, 5
    if(Check_Flag_Volume == 1)
        AUDIO_Send(BIG_SOUND);
    do
    {
        Function_Handle_Package_Sound_Step1();
        switch (Key)
        {
        case '*':
            AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
            LED_Dispaly(0x00, 0x00);
            if(Check_Motor == 1)
                while (Sleep_Flag == 0);
            else
                Sleep_Flag = 1;
            
            break;
        case '#':
            AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
            break;
        case '1':
            AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
            Function_Handle_Setting_Resgister();
            break;
        case '2':
            AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
            Function_Handle_Setting_Clear();
            break;
        case '3':
            AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
            Function_Handle_Setting_System();
            break;
        case '4':
            AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
            Function_Handle_Setting_Introduce();
            break;
        case '5':
            AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
            Function_Handle_Join_Network();
            break;
        default:
            Key = '#';
            break;
        }
    }while((Key == '#') && (Sleep_Flag == 0));
    return 0;
}

void Function_Handle_Package_Sound_Step1(void)
{
    uint16_t i = 0;
    if(Check_Language == 0)
    {
        i = 0;
    }
    else
    {
        i = 1000;
    }
    
    do
    {
        Key = AUDIO_Handle_Send_5(REGISTER_USER, PRESS_KEY, SPEAK_1, MID_MIN_TIME_SOUND-i, Check_Language);
        if(Key != 0) break;
        else
        {
            Key = AUDIO_Handle_Send_5(DELETE_USER, PRESS_KEY, SPEAK_2, MID_MIN_TIME_SOUND-i, Check_Language);
            if(Key != 0) break;
            else 
            {
                Key = AUDIO_Handle_Send_5(INSTALL_SYSTEM, PRESS_KEY, SPEAK_3, NORMAL_TIME_SOUND-i, Check_Language);
                if(Key != 0) break;
                else
                {
                    if(Check_Language == 0)
                    {
                        Key = AUDIO_Handle_Send_5(INTRUDUCTION_EQUIPMENT, PRESS_KEY, SPEAK_4, NORMAL_TIME_SOUND-i, Check_Language);
                    }
                    else
                    {
                        Key = AUDIO_Handle_Send_5(INTRUDUCTION_EQUIPMENT, PRESS_KEY, SPEAK_4, MID_TIME_SOUND-i, Check_Language);
                    }
                    
                    if(Key != 0) break;
                    else
                    {
                        Key = AUDIO_Handle_Send_5(SETTING_NETWORK, PRESS_KEY, SPEAK_5, NORMAL_TIME_SOUND-i, Check_Language);
                        
                        if(Key != 0) break;
                        else
                        {
                            Key = AUDIO_Handle_Send_5(TO_BACK_PRESS_SAO, 0, 0, MID_TIME_SOUND, Check_Language);
                            if(Key != 0) 
                            {
                                Start_Count = 0;
                                break;
                            }
                            else
                            {
                                Key = AUDIO_Handle_Send_5(LISTEN_AGAIN_PRESS_THANG, 0, 0,  MID_TIME_SOUND, Check_Language);
                                if(Key != 0) 
                                {
                                    Start_Count = 0;
                                    break;
                                }
                                else
                                {
                                    Start_Count = 0;
                                    break;
                                }
                                
                            }   
                        }
                            
                
                        
                    }
                    
                }
                
            }
        }
        
    } while ((Key == 0) && (Start_Count == 1));
}

void Function_Handle_Package_Sound_Step0_0(void)
{
    do
    {
        Key = AUDIO_Handle_Send_0_9_0(VERIFY_SUCCESSFUL, NORMAL_TIME_SOUND, Check_Language);
        if(Key != 0) break;
        else
        {
            Key = AUDIO_Handle_Send_0_9_0(SUGGEST_SETTING_MANAGER, MID_TIME_SOUND, Check_Language);
            if(Key != 0) break;
            else
            {
                Key = AUDIO_Handle_Send_0_9_0(PRESS_THANG_KEY_TO_ENTER_SETTING_LIST, HIGHT_TIME_SOUND, Check_Language);
                if(Key != 0) break;
                else
                {
                    Start_Count = 0;
                    break;
                }
                
            }
            
        }
        
    } while ((Key == 0)&&(Start_Count == 1));
}

void Function_Handle_Package_Sound_Step1_3(void)
{
    do
    {
        Key = AUDIO_Handle_Send(SUGGEST_ENTER_NEW_PASSWORD, 0, 0, MID_MIN_TIME_SOUND);
        if(Key != 0) break;
        else
        {

            Key = AUDIO_Handle_Send(TO_CONFIRM_PRESS_THANG, 0, 0, MID_TIME_SOUND);
            if(Key != 0) 
            {
                Start_Count = 0;
                break;
            }
            else
            {
                Key = AUDIO_Handle_Send(TO_BACK_PRESS_SAO, 0, 0,  HIGHT_TIME_SOUND);
                if(Key != 0) 
                {
                    Start_Count = 0;
                    break;
                }
                else
                {
                    Start_Count = 0;
                    break;
                }
                
            }
                            
                        
        }
        
    } while ((Key == 0) && (Start_Count == 1));
}

void Function_Handle_Package_Sound_Step1_4(void)
{
    do
    {
        Key = AUDIO_Handle_Send(SUGGEST_ENTER_NEW_PASSWORD_AGAIN, 0, 0, MID_TIME_SOUND);
        if(Key != 0) break;
        else
        {

            Key = AUDIO_Handle_Send(TO_CONFIRM_PRESS_THANG, 0, 0, MID_TIME_SOUND);
            if(Key != 0) 
            {
                Start_Count = 0;
                break;
            }
            else
            {
                Key = AUDIO_Handle_Send(TO_BACK_PRESS_SAO, 0, 0,  HIGHT_TIME_SOUND);
                if(Key != 0) 
                {
                    Start_Count = 0;
                    break;
                }
                else
                {
                    Start_Count = 0;
                    break;
                }
                
            }
                            
                        
        }
        
    } while ((Key == 0) && (Start_Count == 1));
}

void Function_Handle_Setting_Resgister(void)
{
    LED_Dispaly(0x0A, 0x07); // Bat led * #, 1, 2, 3
    do
    {
        Function_Handle_Package_Sound_Step1_1();
        Key = GET_Key();
        switch (Key)
        {
        case '*':
            AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
            if(Eeprom_Flag == 0)
                Function_Handle_Setting();
            else
                Sleep_Flag = 1;
            break;
        case '#':
            AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
            break;
        case '1':
            AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
            Function_Handle_Setting_Fingerprint();
            break;
        case '2':
            AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
            Function_Handle_Setting_Password();
            break;
        case '3':
            AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
            Function_Handle_Setting_Card();
            break;
        default:
            Key = '#';
            break;
        }
    }while((Key == '#')&&(Sleep_Flag == 0));
}

void Function_Handle_Package_Sound_Step1_1(void)
{
    do
    {
        Key = AUDIO_Handle_Send_3(AND_FINGERPRINT, PRESS_KEY, SPEAK_1, MID_MIN_MIN_TIME_SOUND, Check_Language);
        if(Key != 0) break;
        else
        {
            Key = AUDIO_Handle_Send_3(AND_PASSWORD, PRESS_KEY, SPEAK_2, NORMAL_TIME_SOUND, Check_Language);
            if(Key != 0) break;
            else 
            {
                Key = AUDIO_Handle_Send_3(AND_CARD, PRESS_KEY, SPEAK_3, NORMAL_TIME_SOUND, Check_Language);
                if(Key != 0) break;
                else
                {
                    Key = AUDIO_Handle_Send_3(TO_BACK_PRESS_SAO, 0, 0, MID_TIME_SOUND, Check_Language);
                    if(Key != 0) 
                    {
                        Start_Count = 0;
                        break;
                    }
                    else
                    {
                        Key = AUDIO_Handle_Send_3(LISTEN_AGAIN_PRESS_THANG, 0, 0, MID_TIME_SOUND, Check_Language);
                        if(Key != 0) 
                        {
                            Start_Count = 0;
                            break;
                        }
                        else
                        {
                            Start_Count = 0;
                            break;
                        }
                        
                    }
                                
                }
                
            }
        }
        
    } while ((Key == 0) && (Start_Count == 1));
}

void Function_Handle_Setting_Password(void)
{
    if(Eeprom_Flag == 0)
    {
        LED_Dispaly(0x0A, 0x03); // Bat led * #, 1, 2
        do
        {
            Function_Handle_Package_Sound_Step1_2();
            Key = GET_Key();
            switch (Key)
            {
            case '*':
                AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
                Function_Handle_Setting_Resgister();
                break;
            case '#':
                AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
                break;
            case '1':
                AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
                Function_Handle_Setting_Password_User();
                break;
            case '2':
                AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
                Function_Handle_Setting_Password_Manager();
                break;
            default:
                Key = '#';
                break;
            }
        }while((Key == '#')&&(Sleep_Flag == 0));
    }
    else
    {
        // Eeprom_Flag = 0;
        Function_Handle_Setting_Password_Manager();
    }
    
    
}

void Function_Handle_Package_Sound_Step1_2(void)
{
    do
    {
        Key = AUDIO_Handle_Send_2(NOTIFICATION_USER, PRESS_KEY, SPEAK_1, MIN_TIME_SOUND, Check_Language);
        if(Key != 0) break;
        else
        {
            Key = AUDIO_Handle_Send_2(NOTIFICATION_MANAGER, PRESS_KEY, SPEAK_2, NORMAL_TIME_SOUND, Check_Language);
            if(Key != 0) break;
            else 
            {
                Key = AUDIO_Handle_Send_2(TO_BACK_PRESS_SAO, 0, 0, MID_TIME_SOUND, Check_Language);
                if(Key != 0) 
                {
                    Start_Count = 0;
                    break;
                }
                else
                {
                    Key = AUDIO_Handle_Send_2(LISTEN_AGAIN_PRESS_THANG, 0, 0, MID_TIME_SOUND, Check_Language);
                    if(Key != 0) 
                    {
                        Start_Count = 0;
                        break;
                    }
                    else
                    {
                        Start_Count = 0;
                        break;
                    }
                    
                }
                                
                
            }
        }
        
    } while ((Key == 0) && (Start_Count == 1));
}

void Function_Handle_Setting_Password_User(void)
{
    uint8_t Check_Enter_Password = FALSE; 
    LED_Dispaly(0x0F, 0xFF); // Bat tat ca led ban phim
    cleanmemory(Password_Array, NULL, SIZEOFFRAME); // Xoa tat ca key da nhap
    Check_Key = NULL;
    Function_Handle_Package_Sound_Step1_3();
    if((Key != '*') && (Key != '#') && (Key != 0))
    {
        Password_Array[0] = Key;
        Check_Key = GET_Pin(Password_Array);
        Key = '1';
    }else
    {
        if(Key == '*')
        {   
            AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
            Check_Key = BACK;
        }
        else if(Key == '#')
        {
            AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
            Check_Key = REPEAT;
        }
        else
        {
            Sleep_Flag = 1;
        }
        
        
    }
    
    if(Check_Key == REPEAT)// Kiem tra xem # co duoc nhan hay khong
    {
        LED_Dispaly(0x0F, 0xFF);
        Check_Number_Of_Array = Function_Check_Number_Of_Array(Password_Array);
        if(Check_Number_Of_Array < 4)
        {
            AUDIO_Handle_Send(OPERATION_NOT_SUCCESSFUL, 0, 0, MID_TIME_SOUND);
            Function_Handle_Setting_Password_User();
        }else
        {
            Check_Enter_Password = Function_Enter_Password_Again();
            Key = '1';
            if(Check_Enter_Password == VERIFY_TRUE)
            {
                Check_Password_Enroll = Function_Check_Password_In_Eeprom(Password_Array);
                if(Check_Password_Enroll == FALSE)
                {
                    if(Temp_ID > 25)
                    {
                        AUDIO_Press_Send(PASSWORD_WAS_FULL, NORMAL_TIME_SOUND);
                        Function_Handle_Setting_Password_User();
                    }else
                    {
                        Buffer_vtupdatesetpincode[4] = Temp_ID;
                        Buffer_vtupdatesetpincode[5] = 0;
                        for(uint8_t i = 0; i < 5; i++)
                        {
                            Buffer_vtupdatesetpincode[5] ^= Buffer_vtupdatesetpincode[i];
                        }
                        ZB_Wakeup();
                        NCP_Send(Buffer_vtupdatesetpincode, sizeof(Buffer_vtupdatesetpincode));
                        Function_Save_Password_User_Into_Eeprom(Password_Array);
                        WRITER_Byte_To_Eeprom(ADDRESS_SAVE_PIN, 0x01);
                        delay_ms(5);
                        Check_Id = Temp_ID;
                        Function_Notification_Id();
                        FingerPrint_Send_Off_Led();
                        Function_Handle_Setting_Password();   
                    }
                }
            }else if(Check_Enter_Password == VERIFY_FALSE)
            {
                AUDIO_Handle_Send(OPERATION_NOT_SUCCESSFUL, 0, 0, MID_TIME_SOUND);
                Function_Handle_Setting_Password_User();
            }else
            {
                Sleep_Flag = 1;
            }
        }
         
    }else if(Check_Key == BACK)  // Kiem tra xem * co duoc nhan hay khong
    {
        Function_Handle_Setting_Password();
    }else
    {
        Sleep_Flag = 1;
    }
    
}

uint8_t Function_Enter_Password_Again(void)
{
    delay_ms(100);
    LED_Dispaly(0x0F, 0xFF);
    cleanmemory(Password_Array_Again, NULL, SIZEOFFRAME);
    Check_Key = NULL;
    Function_Handle_Package_Sound_Step1_4();
    if((Key != '*') && (Key != '#') && (Key != 0))
    {
        Password_Array[0] = Key;
        Check_Key = GET_Pin(Password_Array_Again);
        if(Check_Key == REPEAT)
        {
            Check_Number_Of_Array_Again = Function_Check_Number_Of_Array(Password_Array_Again);
            if(Check_Number_Of_Array != Check_Number_Of_Array_Again)
            {
                // TIME_Out(0xFFFF);
                delay_ms(100);
                return VERIFY_FALSE;
            }
            else
            {
                if(memcmp(Password_Array_Again, Password_Array, Check_Number_Of_Array) == 0)
                {
                    LED_Dispaly(0x00, 0x00);
                    return VERIFY_TRUE;
                }
                else
                {
                    // TIME_Out(0xFFFF);
                    delay_ms(100);
                    return VERIFY_FALSE;
                }   
            }
            
        }
        else if(Check_Key == BACK)
        {
            Function_Enter_Password_Again();
        }
        else
        {
            Sleep_Flag = 1;
            return SLEEP;
        }
        
    }
    else
    {
        if(Key == '*')
        {   
            AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
            return VERIFY_FALSE;
        }
        else if(Key == '#')
        {
            AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
            Function_Enter_Password_Again();
        }
        else
        {
            Sleep_Flag = 1;
            return SLEEP;
        }
    }
    return SLEEP;
}

bool Function_Check_Password_In_Eeprom(uint8_t* Password_Array)
{
    uint8_t sizeofpassword = 0;
    for(uint16_t i = 0; i < ALL_FRAME_USE_OF_KEY; i++)
    {
        sizeofpassword = READ_Frame_From_Eeprom(Password_Save, ADDRESS_KEY_FIRST+SIZEOFFRAME*i, SIZEOFFRAME);
        if(memcmp(Password_Array, Password_Save, sizeofpassword) == 0)
        {
            AUDIO_Handle_Send(PASSWORD_WAS_USED, 0, 0, MID_TIME_SOUND);
            cleanmemory(Password_Array, NULL, SIZEOFFRAME);
            if(Key == '1')
            {
                Function_Handle_Setting_Password_User();
                return TRUE;
            }else if(Key == '2')
            {
                Function_Handle_Setting_Password_Manager();
                return TRUE;
            }else
            {
                return TRUE;
            }
        }

        if(Password_Save[0] == 0xFF)
        {
            Temp_ADDRESS = ADDRESS_KEY_FIRST+SIZEOFFRAME*i;
            Temp_ID = i+1;
            if(zb_getid == 25)
                Temp_ID = 25;
            if(zb_getid > 25)
            {
                if(zb_getid_again == 0xF0)
                {
                    zb_getid_again = 1;
                    zb_getid = 25;

                }else
                {
                    // AUDIO_Press_Send(PASSWORD_WAS_FULL, NORMAL_TIME_SOUND);
                    // return TRUE;   
                    return FALSE;
                    
                }
            }
            return FALSE;
        }
    }
    if(zb_getid > 25)
    {
        AUDIO_Press_Send(PASSWORD_WAS_FULL, NORMAL_TIME_SOUND);
        return TRUE;
    }
    Temp_ID = 26;
    return FALSE;
}

bool Function_Check_Password_In_Eeprom_Again(uint8_t* Password_Array)
{
    for(uint16_t i = 0; i < ALL_FRAME_USE_OF_KEY; i++)
    {
        READ_Frame_From_Eeprom(Password_Save, ADDRESS_KEY_FIRST+SIZEOFFRAME*i, SIZEOFFRAME);
        if(memcmp(Password_Array, Password_Save, Check_Number_Of_Array) == 0)
        {
            AUDIO_Handle_Send(PASSWORD_WAS_USED, 0, 0, MID_TIME_SOUND);
            cleanmemory(Password_Array, NULL, SIZEOFFRAME);
            return TRUE;
        }

    }
    return FALSE;
}

void Function_Save_Password_User_Into_Eeprom(uint8_t* Password_Array)
{
    for(uint8_t i = 0; i < SIZEOFFRAME; i++)
    {
        if(Password_Array[i] == NULL)
        {
            Password_Array[i] = Temp_ID; Password_Array[i+1] = USER; Password_Array[i+2] = END_FRAME;
            for(uint8_t j = i+3; j < SIZEOFFRAME; j++)
            {
                Password_Array[j] = 0xFF;
            }
            WRITER_Frame_To_Eeprom(Temp_ADDRESS, Password_Array, SIZEOFFRAME); // Luu password vao eeprom
            cleanmemory(Password_Array, NULL, SIZEOFFRAME); // Xoa tat ca key da nhap
            FingerPrint_Send_Success_Led();
            AUDIO_Handle_Send(OPERATION_SUCCESSFUL, 0, 0, NORMAL_TIME_SOUND);
            Sleep_Flag = 1;
            break;
        }
    }
}

void Function_Handle_Setting_Password_Manager(void)
{
    uint8_t Check_Enter_Password = FALSE; 
    LED_Dispaly(0x0F, 0xFF); // Bat tat ca led ban phim
    cleanmemory(Password_Array, NULL, SIZEOFFRAME); // Xoa tat ca key da nhap
    Check_Key = NULL;
    Function_Handle_Package_Sound_Step1_3();
    if((Key != '*') && (Key != '#') &&  (Key != 0))
    {
        Password_Array[0] = Key;
        Check_Key = GET_Pin(Password_Array);
        Key = '2';
    }
    else
    {
        if(Key == '*')
        {   
            AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
            Check_Key = BACK;
        }
        else if(Key == '#')
        {
            AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
            Check_Key = REPEAT;
        }
        else
        {
            Sleep_Flag = 1;
        }
        
        
    }

    if(Check_Key == REPEAT)// Kiem tra xem # co duoc nhan hay khong
    {
        LED_Dispaly(0x0F, 0xFF);
        Check_Number_Of_Array = Function_Check_Number_Of_Array(Password_Array);
        if(Check_Number_Of_Array < 4)
        {
            AUDIO_Handle_Send(OPERATION_NOT_SUCCESSFUL, 0, 0, MID_TIME_SOUND);
            Function_Handle_Setting_Password_Manager();
        }else
        {
            Check_Enter_Password = Function_Enter_Password_Again();
            Key = '2';
            if(Check_Enter_Password == VERIFY_TRUE)
            {
                Check_Password_Enroll = Function_Check_Password_In_Eeprom(Password_Array);
                if(Check_Password_Enroll == FALSE)
                {
                    if(Temp_ID > 25)
                    {
                        AUDIO_Press_Send(PASSWORD_WAS_FULL, NORMAL_TIME_SOUND);
                        Function_Handle_Setting_Password_User();
                    }else
                    {
                        Buffer_vtupdatesetpincode[4] = Temp_ID;
                        Buffer_vtupdatesetpincode[5] = 0;
                        for(uint8_t i = 0; i < 5; i++)
                        {
                            Buffer_vtupdatesetpincode[5] ^= Buffer_vtupdatesetpincode[i];
                        }
                        ZB_Wakeup();
                        NCP_Send(Buffer_vtupdatesetpincode, sizeof(Buffer_vtupdatesetpincode));
                        Function_Save_Password_Manager_Into_Eeprom(Password_Array);
                        WRITER_Byte_To_Eeprom(ADDRESS_SAVE_STATE_EEPROM, 0x00);
                        delay_ms(5);
                        WRITER_Byte_To_Eeprom(ADDRESS_SAVE_PIN, 0x01);
                        delay_ms(5);
                        Check_Id = Temp_ID;
                        Function_Notification_Id();
                        FingerPrint_Send_Off_Led();
                        if(Eeprom_Flag == 0)
                            Function_Handle_Setting_Password();   
                    }
                }
            }else if(Check_Enter_Password == VERIFY_FALSE)
            {
                AUDIO_Handle_Send(OPERATION_NOT_SUCCESSFUL, 0, 0, MID_TIME_SOUND);
                Function_Handle_Setting_Password_Manager();
            }else
            {
                Sleep_Flag = 1;
            }
        }
        
    }
    else if(Check_Key == BACK)  // Kiem tra xem * co duoc nhan hay khong
    {
        if(Eeprom_Flag == 0)
            Function_Handle_Setting_Password();
        else
            Function_Handle_Setting_Resgister();
    }
    else
    {
        Sleep_Flag = 1;
    }
}

void Function_Save_Password_Manager_Into_Eeprom(uint8_t* Password_Array)
{
    for(uint8_t i = 0; i < SIZEOFFRAME; i++)
    {
        if(Password_Array[i] == NULL)
        {
            Password_Array[i] = Temp_ID; Password_Array[i+1] = MANAGER; Password_Array[i+2] = END_FRAME;
            for(uint8_t j = i+3; j < SIZEOFFRAME; j++)
            {
                Password_Array[j] = 0xFF;
            }
            WRITER_Frame_To_Eeprom(Temp_ADDRESS, Password_Array, SIZEOFFRAME); // Luu password vao eeprom
            cleanmemory(Password_Array, NULL, SIZEOFFRAME); // Xoa tat ca key da nhap
            FingerPrint_Send_Success_Led();
            AUDIO_Handle_Send(OPERATION_SUCCESSFUL, 0, 0, NORMAL_TIME_SOUND);
            Sleep_Flag = 1;
            break;
        }
    }
}

void Function_Handle_Setting_Card(void)
{
    CARD_Control_Power_On();
    PCD_Config();
    delay_us(2);
    if(Eeprom_Flag == 0)
    {
        LED_Dispaly(0x0A, 0x03); // Bat led * #, 1, 2
        do
        {
            Function_Handle_Package_Sound_Step1_2();
            Key = GET_Key();
            switch (Key)
            {
            case '*':
                AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
                Function_Handle_Setting_Resgister();
                break;
            case '#':
                AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
                break;
            case '1':
                AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);;
                Function_Handle_Setting_Card_User();
                break;
            case '2':
                AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
                Function_Handle_Setting_Card_Manager();
                break;
            default:
                Key = '#';
                break;
            }
        }while((Key == '#')&&(Sleep_Flag == 0));
    }
    else
    {
        //Eeprom_Flag = 0;
        Function_Handle_Setting_Card_Manager();
    }
    
}

void Function_Handle_Setting_Card_User(void)
{
    LED_Dispaly(0x02, 0x00); // Bat led * 
    do
    {
        Check_Key = GET_Card(Password_Array);
        switch (Check_Key)
        {
        case '*':
            AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
            Function_Handle_Setting_Card();
            break;
        case 1:
            LED_Dispaly(0x00, 0x00);
            Check_Card_Enroll = Function_Check_Card_In_Eeprom(Password_Array);
            if(Check_Card_Enroll == FALSE)
            {
                if(Temp_ID > MAXCARD)
                {
                    AUDIO_Get_Finger_Send(CARD_WAS_FULL, NORMAL_TIME_SOUND, Check_Language);
                    Function_Handle_Setting_Card();
                }else
                {
                    Function_Save_Card_User_Into_Eeprom(Password_Array);
                    WRITER_Byte_To_Eeprom(ADDRESS_SAVE_CARD, 0x01);
                    delay_ms(5);
                    Check_Id = Temp_ID;
                    Function_Notification_Id();
                    FingerPrint_Send_Off_Led();
                    Function_Handle_Setting_Card();
                }
            }
            break;
        case 2:
            break;
        default:
            break;
        }
    } while (Check_Key == '#');
    
}

bool Function_Check_Card_In_Eeprom(uint8_t* Password_Array)
{
    for(uint16_t i = 0; i < ALL_FRAME_USE_OF_CARD; i++)
    {
        READ_Frame_From_Eeprom(Password_Save, ADDRESS_CARD_FIRST+SIZEOFFRAME*i, SIZEOFFRAME);
        if(memcmp(Password_Array, Password_Save, 4) == 0)
        {
            Finger_Flag = 0;
            AUDIO_Finger_Send(CARD_WAS_USED, MID_TIME_SOUND, Check_Language);
            cleanmemory(Password_Array, NULL, SIZEOFFRAME); // Xoa tat ca key da nhap
            if(Key == '1')
            {
                Timeout_Flag = 1;
                Count = 0; 
                Sleep_Flag = 0;
                Function_Handle_Setting_Card_User();
                return TRUE;
            }
            else if(Key == '2')
            {
                Timeout_Flag = 1;
                Count = 0; 
                Sleep_Flag = 0;
                Function_Handle_Setting_Card_Manager();
                return TRUE;
            }
        }
        if(Password_Save[0] == 0xFF)
        {
            Temp_ADDRESS = ADDRESS_CARD_FIRST+SIZEOFFRAME*i;
            Temp_ID = i+1;
            return FALSE;
        }
    }
    return FALSE;
}

void Function_Save_Card_User_Into_Eeprom(uint8_t* Password_Array)
{
    for(uint8_t i = 0; i < SIZEOFFRAME; i++)
    {
        if(Password_Array[i] == NULL)
        {
            Password_Array[i] = Temp_ID; Password_Array[i+1] = USER; Password_Array[i+2] = END_FRAME;
            for(uint8_t j = i+3; j < SIZEOFFRAME; j++)
            {
                Password_Array[j] = 0xFF;
            }
            WRITER_Frame_To_Eeprom(Temp_ADDRESS, Password_Array, SIZEOFFRAME);
            cleanmemory(Password_Array, NULL, SIZEOFFRAME); 
            LED_Dispaly(0x00, 0x00);
            FingerPrint_Send_Success_Led();
            AUDIO_Card_User_Manager_Send(OPERATION_SUCCESSFUL, NORMAL_TIME_SOUND, Check_Language);
            Sleep_Flag = 1;
            break;
        }
    }
}

void Function_Handle_Setting_Card_Manager(void)
{
    LED_Dispaly(0x02, 0x00); // Bat led * 
    do
    {
        Check_Key = GET_Card(Password_Array);
        switch (Check_Key)
        {
        case '*':
            AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
            Function_Handle_Setting_Card();
            break;
        case 1:
            LED_Dispaly(0x00, 0x00);
            Check_Card_Enroll = Function_Check_Card_In_Eeprom(Password_Array);
            if(Check_Card_Enroll == FALSE)
            {
                if(Temp_ID > MAXCARD)
                {
                    AUDIO_Press_Send(CARD_WAS_FULL, NORMAL_TIME_SOUND);
                    Function_Handle_Setting_Card();
                }else
                {
                    Function_Save_Card_Manager_Into_Eeprom(Password_Array);
                    WRITER_Byte_To_Eeprom(ADDRESS_SAVE_STATE_EEPROM, 0x00);
                    delay_ms(5);
                    WRITER_Byte_To_Eeprom(ADDRESS_SAVE_CARD, 0x01);
                    delay_ms(5);
                    Check_Id = Temp_ID;
                    Function_Notification_Id();
                    FingerPrint_Send_Off_Led();
                    if(Eeprom_Flag == 0)
                        Function_Handle_Setting_Card();   
                }
            }
            break;
        case 2:
            break;
        default:
            break;
        }
    } while (Check_Key == '#');
    
}

void Function_Save_Card_Manager_Into_Eeprom(uint8_t* Password_Array)
{
    for(uint8_t i = 0; i < SIZEOFFRAME; i++)
    {
        if(Password_Array[i] == NULL)
        {
            Password_Array[i] = Temp_ID; Password_Array[i+1] = MANAGER; Password_Array[i+2] = END_FRAME;
            for(uint8_t j = i+3; j < SIZEOFFRAME; j++)
            {
                Password_Array[j] = 0xFF;
            }
            WRITER_Frame_To_Eeprom(Temp_ADDRESS, Password_Array, SIZEOFFRAME); 
            cleanmemory(Password_Array, NULL, SIZEOFFRAME); 
            LED_Dispaly(0x00, 0x00);
            FingerPrint_Send_Success_Led();
            AUDIO_Card_User_Manager_Send(OPERATION_SUCCESSFUL, NORMAL_TIME_SOUND, Check_Language);
            Sleep_Flag = 1;
            break;
        }
    }
}

void Function_Handle_Setting_Fingerprint(void)
{
    if(Eeprom_Flag == 0)
    {
        LED_Dispaly(0x0A, 0x03); // Bat led * #, 1, 2
        do
        {
            Function_Handle_Package_Sound_Step1_2();
            Key = GET_Key();
            switch (Key)
            {
            case '*':
                AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
                Function_Handle_Setting_Resgister();
                break;
            case '#':
                AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
                break;
            case '1':
                AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
                Function_Handle_Setting_Fingerprint_User();
                break;
            case '2':
                AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
                Function_Handle_Setting_Fingerprint_Manager();
                break;
            default:
                Key = '#';
                break;
            }
        }while((Key == '#')&&(Sleep_Flag == 0));
    }
    else
    {
        Function_Handle_Setting_Fingerprint_Manager();
    }
    
}

void Function_Handle_Setting_Fingerprint_User(void)
{
    static uint8_t Count = 0;
    Check_Faulty_Systm = 0;
    LED_Dispaly(0x02, 0x00);
    FingerPrint_Send_Check_Led();
    
    FingerPrint_Enable_Int();
    // UART_SendData(UART1, Fingerprintenableinterrputstep2, sizeof(Fingerprintenableinterrputstep2));
    // EXTI_ClearITPendingBit(EXTI_IT_Pin7);
    Finger_Flag  = 0;

    cleanmemory(Password_Array, NULL, SIZEOFFRAME);
    Finger_Value =  Function_Return_Finger_Not_Use();
    Key = AUDIO_Handle_Send_0_9(USE_SOLID_FINGER_AT_DIFFERENT_ANGLES, 2*HIGHT_TIME_SOUND, Check_Language);
    if(Key == '*')
    {
        Key = 0;
        AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
        FingerPrint_Send_Off_Led();
        Function_Handle_Setting_Fingerprint();
    }else
    {
        
        if(SEARCH_Get_Finger() == TRUE)
        {
            AUDIO_Handle_Send(FINGERPRINT_WAS_USED, 0, 0, MID_MIN_TIME_SOUND);
            Function_Handle_Setting_Fingerprint_User();
        }else
        {
            if(Finger_Flag == 1)
            {
                Finger_Flag = 0;
                if(Function_Check_Finger_In_Eeprom())
                {
                    FingerPrint_Send_Off_Led();
                    AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
                    FingerPrint_Send_Check_Led();
                    AUDIO_Get_Finger_Send(CHANGE_ANGLE_FINGERPRINT, 1, Check_Language);
                    delay_ms(300);
                    Check_Hand_Enroll = GET_Finger();
                    if(Check_Hand_Enroll == TRUE)
                    {
                        if(Key == 0xFF)
                        {
                            Count = 0;
                            Sleep_Flag = 1;
                        }else
                        {
                            Check_Hand_Enroll = FALSE;
                            Function_Save_Finger_User_Into_Eeprom(Password_Array);
                            WRITER_Byte_To_Eeprom(ADDRESS_SAVE_FINGER, 0x01);
                            delay_ms(5);
                            Check_Id = Temp_ID;
                            Function_Notification_Id();
                            FingerPrint_Send_Off_Led();
                            Function_Handle_Setting_Fingerprint();
                        }
                    }else
                    {
                        Count++;
                        AUDIO_Finger_Send(OPERATION_NOT_SUCCESSFUL, MID_TIME_SOUND, Check_Language);
                        if(Count < 2)
                            Function_Handle_Setting_Fingerprint_User();
                        else
                        {
                            Count = 0;
                            Sleep_Flag = 1;
                        }
                        
                    } 
                }else
                {
                    AUDIO_Get_Finger_Send(FINGERPRINT_WAS_FULL, NORMAL_TIME_SOUND, Check_Language);
                    Function_Handle_Setting_Fingerprint();
                }
            }else
            {
                if(Function_Check_Finger_In_Eeprom())
                {
                    Check_Hand_Enroll = GET_Finger();
                    if(Check_Hand_Enroll == TRUE)
                    {
                        if(Key == 0xFF)
                        {
                            Count = 0;
                            Sleep_Flag = 1;
                        }else
                        {
                            Check_Hand_Enroll = FALSE;
                            Function_Save_Finger_User_Into_Eeprom(Password_Array);
                            WRITER_Byte_To_Eeprom(ADDRESS_SAVE_FINGER, 0x01);
                            delay_ms(5);
                            Check_Id = Temp_ID;
                            Function_Notification_Id();
                            FingerPrint_Send_Off_Led();
                            Function_Handle_Setting_Fingerprint();
                        }
                    }else
                    {
                        Count++;
                        AUDIO_Finger_Send(OPERATION_NOT_SUCCESSFUL, MID_TIME_SOUND, Check_Language);
                        if(Count < 2)
                            Function_Handle_Setting_Fingerprint_User();
                        else
                        {
                            Count = 0;
                            Sleep_Flag = 1;
                        }
                        
                    } 
                }else
                {
                    AUDIO_Get_Finger_Send(FINGERPRINT_WAS_FULL, NORMAL_TIME_SOUND, Check_Language);
                    Function_Handle_Setting_Fingerprint();
                }
            }
              
        }
            
    }
}

uint8_t Function_Return_Finger_Not_Use(void)
{
    for(uint16_t i = 0; i < ALL_FRAME_USE_OF_FINGERPRINT; i++)
    {
        READ_Frame_From_Eeprom(Password_Save, ADDRESS_FINGERPRINT_FIRST+SIZEOFFRAME*i, SIZEOFFRAME);
        if(Password_Save[0] == 0xFF)
        {
            Finger_Temp = Finger_Default + 5*i;
            return Finger_Temp;
        }
    }
    return Finger_Temp;
}

void Function_Save_Finger_User_Into_Eeprom(uint8_t* Password_Array)
{
    for(uint8_t i = 0; i < SIZEOFFRAME; i++)
    {
        if(Password_Array[i] == NULL)
        {
            Password_Array[i] = Temp_ID; Password_Array[i+1] = USER; Password_Array[i+2] = END_FRAME;
            for(uint8_t j = i+3; j < SIZEOFFRAME; j++)
            {
                Password_Array[j] = 0xFF;
            }
            WRITER_Frame_To_Eeprom(Temp_ADDRESS, Password_Array, SIZEOFFRAME);
			memcpy(Password_Save, Password_Array, 24);
            cleanmemory(Password_Array, NULL, SIZEOFFRAME); 
            AUDIO_Handle_Send(OPERATION_SUCCESSFUL, 0, 0, NORMAL_TIME_SOUND);
            Sleep_Flag = 1;
            break;
        }
    }
}

bool Function_Check_Finger_In_Eeprom(void)
{
    for(uint16_t i = 0; i < ALL_FRAME_USE_OF_FINGERPRINT; i++)
    {
        READ_Frame_From_Eeprom(Password_Save, ADDRESS_FINGERPRINT_FIRST+SIZEOFFRAME*i, SIZEOFFRAME);
        if(Password_Save[0] == 0xFF)
        {
            Temp_ADDRESS = ADDRESS_FINGERPRINT_FIRST + SIZEOFFRAME*i;
            Temp_ID = i + 1;
            return TRUE;
        }
    }
	return FALSE; //Finger isn't use
}

void Function_Handle_Setting_Fingerprint_Manager(void)
{
    static uint8_t Count = 0;
    Check_Faulty_Systm = 0;
    LED_Dispaly(0x02, 0x00);
    FingerPrint_Send_Check_Led();

    FingerPrint_Enable_Int();
    // UART_SendData(UART1, Fingerprintenableinterrputstep2, sizeof(Fingerprintenableinterrputstep2));
    // EXTI_ClearITPendingBit(EXTI_IT_Pin7);
    Finger_Flag  = 0;
    
    cleanmemory(Password_Array, NULL, SIZEOFFRAME);
    Finger_Value =  Function_Return_Finger_Not_Use();
    Key = AUDIO_Handle_Send_0_9(USE_SOLID_FINGER_AT_DIFFERENT_ANGLES, 2*HIGHT_TIME_SOUND, Check_Language);
    if(Key == '*')
    {
        Key = 0;
        AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
        FingerPrint_Send_Off_Led();
        if(Eeprom_Flag == 1)
        {
            Function_Handle_Setting_Resgister();
        }else
        {
            Function_Handle_Setting_Fingerprint();   
        }
    }else
    {
        if(SEARCH_Get_Finger() == TRUE)
        {
            AUDIO_Handle_Send(FINGERPRINT_WAS_USED, 0, 0, MID_MIN_TIME_SOUND);
            Function_Handle_Setting_Fingerprint_Manager();
        }else
        {
            if(Finger_Flag == 1)
            {
                Finger_Flag = 0;
                if(Function_Check_Finger_In_Eeprom())
                {
                    FingerPrint_Send_Off_Led();
                    AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
                    FingerPrint_Send_Check_Led();
                    AUDIO_Get_Finger_Send(CHANGE_ANGLE_FINGERPRINT, 1, Check_Language);
                    delay_ms(300);
                    Check_Hand_Enroll = GET_Finger();
                    if(Check_Hand_Enroll == TRUE)
                    {
                        if(Key == 0xFF)
                        {
                            Count = 0;
                            Sleep_Flag = 1;
                        }else
                        {
                            Check_Hand_Enroll = FALSE;
                            Function_Save_Finger_Manager_Into_Eeprom(Password_Array);
                            WRITER_Byte_To_Eeprom(ADDRESS_SAVE_STATE_EEPROM, 0x00);
                            delay_ms(5);
                            WRITER_Byte_To_Eeprom(ADDRESS_SAVE_FINGER, 0x01);
                            delay_ms(5);
                            Check_Id = Temp_ID;
                            Function_Notification_Id();
                            FingerPrint_Send_Off_Led();
                            if(Eeprom_Flag == 0)
                                Function_Handle_Setting_Fingerprint();
                        }
                    }else
                    {
                        Count++;
                        AUDIO_Handle_Send(OPERATION_NOT_SUCCESSFUL, 0, 0, MID_MIN_TIME_SOUND);
                        if(Count < 2)
                            Function_Handle_Setting_Fingerprint_Manager();
                        else
                        {
                            Count = 0;
                            Sleep_Flag = 1;
                        }
                        
                    }
                }else
                {
                    AUDIO_Get_Finger_Send(FINGERPRINT_WAS_FULL, NORMAL_TIME_SOUND, Check_Language);
                    Function_Handle_Setting_Fingerprint();
                }

            }else
            {
                if(Function_Check_Finger_In_Eeprom())
                {
                    Check_Hand_Enroll = GET_Finger();
                    if(Check_Hand_Enroll == TRUE)
                    {
                        if(Key == 0xFF)
                        {
                            Count = 0;
                            Sleep_Flag = 1;
                        }else
                        {
                            Check_Hand_Enroll = FALSE;
                            Function_Save_Finger_Manager_Into_Eeprom(Password_Array);
                            WRITER_Byte_To_Eeprom(ADDRESS_SAVE_STATE_EEPROM, 0x00);
                            delay_ms(5);
                            WRITER_Byte_To_Eeprom(ADDRESS_SAVE_FINGER, 0x01);
                            delay_ms(5);
                            Check_Id = Temp_ID;
                            Function_Notification_Id();
                            FingerPrint_Send_Off_Led();
                            if(Eeprom_Flag == 0)
                                Function_Handle_Setting_Fingerprint();
                        }
                    }else
                    {
                        Count++;
                        AUDIO_Handle_Send(OPERATION_NOT_SUCCESSFUL, 0, 0, MID_MIN_TIME_SOUND);
                        if(Count < 2)
                            Function_Handle_Setting_Fingerprint_Manager();
                        else
                        {
                            Count = 0;
                            Sleep_Flag = 1;
                        }
                        
                    }
                }else
                {
                    AUDIO_Get_Finger_Send(FINGERPRINT_WAS_FULL, NORMAL_TIME_SOUND, Check_Language);
                    Function_Handle_Setting_Fingerprint();
                }   
            }
            
        }
            
    }

}

void Function_Save_Finger_Manager_Into_Eeprom(uint8_t* Password_Array)
{
    for(uint8_t i = 0; i < SIZEOFFRAME; i++)
    {
        if(Password_Array[i] == NULL)
        {
            Password_Array[i] = Temp_ID; Password_Array[i+1] = MANAGER; Password_Array[i+2] = END_FRAME;
            for(uint8_t j = i+3; j < SIZEOFFRAME; j++)
            {
                Password_Array[j] = 0xFF;
            }
            WRITER_Frame_To_Eeprom(Temp_ADDRESS, Password_Array, SIZEOFFRAME);
            cleanmemory(Password_Array, NULL, SIZEOFFRAME); 
            AUDIO_Handle_Send(OPERATION_SUCCESSFUL, 0, 0, NORMAL_TIME_SOUND);
            Sleep_Flag = 1;
            break;
        }
    }
}

void Function_Handle_Setting_Clear(void)
{
    LED_Dispaly(0x0A, 0x3F); // Bat led * #, 1, 2, 3, 4, 5, 6
    do
    {
        Function_Handle_Package_Sound_Step2_1();
        Key = GET_Key();
        switch (Key)
        {
        case '*':
            AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
            Function_Handle_Setting();
            break;
        case '#':
            AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
            break;
        case '1':
            AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
            Function_Clear_One_Fingerprint();
            break;
        case '2':
            AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
            Function_Clear_One_Key();
            break;
        case '3':
            AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
            Function_Clear_One_Card();
            break;
        case '4':
            AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
            Function_Clear_All_Fingerprint();
            break;
        case '5':
            AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
            Function_Clear_All_Key();
            break;
        case '6':
            AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
            Function_Clear_All_Card();
            break;
        default:
            Key = '#';
            break;
        }
    }while((Key == '#')&&(Sleep_Flag == 0));
}

void Function_Handle_Package_Sound_Step2_1(void)
{
    do
    {
        Key = AUDIO_Handle_Send_6(REMOVE_ONE_FINGERPRINT, PRESS_KEY, SPEAK_1, NORMAL_TIME_SOUND, Check_Language);
        if(Key != 0) break;
        else
        {
            Key = AUDIO_Handle_Send_6(REMOVE_ONE_PASSWORD, PRESS_KEY, SPEAK_2, NORMAL_TIME_SOUND, Check_Language);
            if(Key != 0) break;
            else 
            {
                Key = AUDIO_Handle_Send_6(REMOVE_ONE_CARD, PRESS_KEY, SPEAK_3, NORMAL_TIME_SOUND, Check_Language);
                if(Key != 0) break;
                else
                {
                   Key = AUDIO_Handle_Send_6(REMOVE_ALL_FINGERPRINT, PRESS_KEY, SPEAK_4, MID_MIN_TIME_SOUND, Check_Language);
                   if(Key != 0) break;
                   else
                   {
                        Key = AUDIO_Handle_Send_6(REMOVE_ALL_PASSWORD, PRESS_KEY, SPEAK_5, MID_MIN_TIME_SOUND, Check_Language);
                        if(Key != 0) break;
                        else
                        {
                            Key = AUDIO_Handle_Send_6(REMOVE_ALL_CARD, PRESS_KEY, SPEAK_6, MID_MIN_TIME_SOUND, Check_Language);
                            if(Key != 0) break;
                            else
                            {
                                Key = AUDIO_Handle_Send_6(TO_BACK_PRESS_SAO, 0, 0, MID_TIME_SOUND, Check_Language);
                                if(Key != 0) 
                                {
                                    Start_Count = 0;
                                    break;
                                }
                                else
                                {
                                    Key = AUDIO_Handle_Send_6(LISTEN_AGAIN_PRESS_THANG, 0, 0, MID_TIME_SOUND, Check_Language);
                                    if(Key != 0) 
                                    {
                                        Start_Count = 0;
                                        break;
                                    }
                                    else
                                    {
                                        Start_Count = 0;
                                        break;
                                    }
                                    
                                }
                                            
                            }
                        }    
                   }
                   
                                
                }
                
            }
        }
        
    } while ((Key == 0) && (Start_Count == 1));
}

void Function_Clear_One_Key(void)
{
    
    Key = 0;
    for(uint8_t i = 0; i < 25; i++)
    {
        if(READ_Byte_From_Eeprom(ADDRESS_KEY_FIRST + 24*i) == 0xFF)
        {
            Key++;
        }else
        {
            Key = 0;
            break;
        }
        delay_ms(5);
    }
    if(Key == 0)
    {
        LED_Dispaly(0x0F, 0xFF); // Bat all led
        do
        {
            
            Function_Handle_Package_Sound_Step2_2();
            ID_Value = Function_Covert_Character_To_ID(ID_Clear);
            if(Key == BACK)
            {
                delay_ms(200);
                Function_Handle_Setting_Clear();
                break;
            }
            else if((Key == REPEAT) && (ID_Value != 0))
            {
                LED_Dispaly(0x00, 0x00);
                Function_Handle_Clear_One_Key(ID_Value);
                break;
            }

        }while(Key == REPEAT);
    }
    else
    {
        LED_Dispaly(0x00, 0x00);
        AUDIO_Handle_Send(NUMBER_USE_IS_EMPTY, 0, 0, NORMAL_TIME_SOUND);
        Function_Handle_Setting_Clear();
    }
}

void Function_Handle_Package_Sound_Step2_2(void)
{
    do
    {
        Key = AUDIO_Handle_Send(SUGGEST_ENTER_USER_ORDER_NUMBER, 0, 0, MID_TIME_SOUND);
        if(Key != 0) break;
        else
        {
            
            Key = AUDIO_Handle_Send(TO_CONFIRM_PRESS_THANG, 0, 0, MID_TIME_SOUND);
            if(Key != 0) 
            {
                Start_Count = 0;
                break;
            }
            else
            {
                Key = AUDIO_Handle_Send(TO_BACK_PRESS_SAO, 0, 0, MID_TIME_SOUND);
                if(Key != 0) 
                {
                    Start_Count = 0;
                    break;
                }
                else
                {
                    Start_Count = 0;
                    Sleep_Flag = 0;
                    Count = 0;
                    Timeout_Flag = 1;
                    break;
                }
                
            }                
                
        }
        
    } while ((Key == 0) && (Start_Count == 1));
}

uint8_t Function_Covert_Character_To_ID(uint8_t* ID_Clear)
{
    int returnvalue = 0, number = 0;
    Key = GET_Pin(ID_Clear);
    number = strlen((char const*)ID_Clear);

    for(int i = 0; i < number; i++)
    {
        ID_Clear[i] = (uint8_t)ID_Clear[i]-0x30;
    }
    if(number == 1)
    {
        returnvalue =  ID_Clear[0];
    }
    else if(number == 2)
    {
        returnvalue =  ID_Clear[0]*10 + ID_Clear[1];
    }
    else if(number == 3)
    {
        returnvalue =  ID_Clear[0]*100 + ID_Clear[1]*10 + ID_Clear[2];
    }
    return returnvalue;

}

void Function_Handle_Clear_One_Key(uint8_t ID_Number)
{
    if(ID_Number != 0)
    {
        Temp_ADDRESS = ADDRESS_KEY_FIRST+SIZEOFFRAME*(ID_Number-1);
        if(READ_Byte_From_Eeprom(Temp_ADDRESS) == 0xFF)
        {
            LED_Dispaly(0x00, 0x00);
            AUDIO_Handle_Send(OPERATION_NOT_SUCCESSFUL, 0, 0, NORMAL_TIME_SOUND);
            Function_Clear_One_Key();

        }else
        {
            Buffer_vtupdateclearpincode[4] = ID_Number;
            Buffer_vtupdateclearpincode[5] = 0;
            for (uint8_t i = 0; i < 5; i++)
            {
                Buffer_vtupdateclearpincode[5] ^= Buffer_vtupdateclearpincode[i];
            }
            ZB_Wakeup();
            NCP_Send(Buffer_vtupdateclearpincode, sizeof(Buffer_vtupdateclearpincode));
            cleanmemory(Password_Array, 0xFF, SIZEOFFRAME);
            WRITER_Frame_To_Eeprom(Temp_ADDRESS, Password_Array, SIZEOFFRAME);
            cleanmemory(Password_Array, NULL, SIZEOFFRAME); 
            AUDIO_Handle_Send(OPERATION_SUCCESSFUL, 0, 0, NORMAL_TIME_SOUND);
            if(Function_Check_User_Exist() == FALSE)
            {
                WRITER_Byte_To_Eeprom(ADDRESS_SAVE_STATE_EEPROM, 0xFF);
                delay_ms(5);
                AUDIO_Handle_Send(NUMBER_USE_IS_EMPTY, 0, 0, NORMAL_TIME_SOUND);
            }
            Function_Handle_Setting_Clear();   
        }
    }else
    {   
        AUDIO_Handle_Send(OPERATION_NOT_SUCCESSFUL, 0, 0, NORMAL_TIME_SOUND);
        Function_Clear_One_Key();
    }
    
}

void Function_Clear_One_Card(void)
{
    Key = 0;
    for(uint8_t i = 0; i < 25; i++)
    {
        if(READ_Byte_From_Eeprom(ADDRESS_CARD_FIRST + 24*i) == 0xFF)
        {
            Key++;
        }else
        {
            Key = 0;
            break;
        }
        delay_ms(5);
    }
    if(Key == 0)
    {
        LED_Dispaly(0x0F, 0xFF); // Bat all led
        do
        {
            Function_Handle_Package_Sound_Step2_2();
            ID_Value = Function_Covert_Character_To_ID(ID_Clear);
            if(Key == BACK)
            {
                delay_ms(200);
                Function_Handle_Setting_Clear();
                break;
            }
            else if((Key == REPEAT) && (ID_Value != 0))
            {
                LED_Dispaly(0x00, 0x00);
                Function_Handle_Clear_One_Card(ID_Value);
                break;
            }

        }while(Key == REPEAT);
    }
    else
    {
        LED_Dispaly(0x00, 0x00);
        AUDIO_Handle_Send(NUMBER_USE_IS_EMPTY, 0, 0, NORMAL_TIME_SOUND);
        Function_Handle_Setting_Clear();
    }
    
}

void Function_Handle_Clear_One_Card(uint8_t ID_Number)
{
    if(ID_Number != 0)
    {
        Temp_ADDRESS = ADDRESS_CARD_FIRST+SIZEOFFRAME*(ID_Number-1);
        if(READ_Byte_From_Eeprom(Temp_ADDRESS) == 0xFF)
        {
            LED_Dispaly(0x00, 0x00);
            AUDIO_Handle_Send(OPERATION_NOT_SUCCESSFUL, 0, 0, NORMAL_TIME_SOUND);
            Function_Clear_One_Card();
        }else
        {
            cleanmemory(Password_Array, 0xFF, SIZEOFFRAME);
            WRITER_Frame_To_Eeprom(Temp_ADDRESS, Password_Array, SIZEOFFRAME);
            cleanmemory(Password_Array, NULL, SIZEOFFRAME); 
            AUDIO_Handle_Send(OPERATION_SUCCESSFUL, 0, 0, NORMAL_TIME_SOUND);
            if(Function_Check_User_Exist() == FALSE)
            {
                WRITER_Byte_To_Eeprom(ADDRESS_SAVE_STATE_EEPROM, 0xFF);
                delay_ms(5);
                AUDIO_Handle_Send(NUMBER_USE_IS_EMPTY, 0, 0, NORMAL_TIME_SOUND);
            }
            Function_Handle_Setting_Clear();
        }
    }
    else
    {
        AUDIO_Handle_Send(OPERATION_NOT_SUCCESSFUL, 0, 0, NORMAL_TIME_SOUND);
        Function_Clear_One_Card();
    }
}

void Function_Clear_One_Fingerprint(void)
{
    Key = 0;
    for(uint8_t i = 0; i < 25; i++)
    {
        if(READ_Byte_From_Eeprom(ADDRESS_FINGERPRINT_FIRST + 24*i) == 0xFF)
        {
            Key++;
        }else
        {
            Key = 0;
            break;
        }
        delay_ms(5);
    }
    if(Key == 0)
    {
        LED_Dispaly(0x0F, 0xFF); // Bat all led
        do
        {
            Function_Handle_Package_Sound_Step2_2();
            ID_Value = Function_Covert_Character_To_ID(ID_Clear);
            if(Key == BACK)
            {
                delay_ms(200);
                Function_Handle_Setting_Clear();
                break;
            }
            else if((Key == REPEAT) && (ID_Value != 0))
            {
                LED_Dispaly(0x00, 0x00);
                Function_Handle_Clear_One_Fingerprint(ID_Value);
                break;
            }

        }while(Key == REPEAT);
    }
    else
    {
        LED_Dispaly(0x00, 0x00);
        AUDIO_Handle_Send(NUMBER_USE_IS_EMPTY, 0, 0, NORMAL_TIME_SOUND);
        Function_Handle_Setting_Clear();
    }
    
}


void Function_Handle_Clear_One_Fingerprint(uint8_t ID_Number)
{
    if(ID_Number != 0)
    {
        Temp_ADDRESS = ADDRESS_FINGERPRINT_FIRST+SIZEOFFRAME*(ID_Number-1);
        if(READ_Byte_From_Eeprom(Temp_ADDRESS) == 0xFF)
        {
            LED_Dispaly(0x00, 0x00);
            AUDIO_Handle_Send(OPERATION_NOT_SUCCESSFUL, 0, 0, NORMAL_TIME_SOUND);
            Function_Clear_One_Fingerprint();
        }else
        {
            cleanmemory(Password_Array, 0xFF, SIZEOFFRAME);
            WRITER_Frame_To_Eeprom(Temp_ADDRESS, Password_Array, SIZEOFFRAME);
            cleanmemory(Password_Array, NULL, SIZEOFFRAME); 
            for(uint8_t i = 0; i < 5; i++)
            {
                Function_Remove_One_Fingerprint(5*ID_Number - i);
                delay_ms(5);
            }
            AUDIO_Handle_Send(OPERATION_SUCCESSFUL, 0, 0, NORMAL_TIME_SOUND);
            if(Function_Check_User_Exist() == FALSE)
            {
                WRITER_Byte_To_Eeprom(ADDRESS_SAVE_STATE_EEPROM, 0xFF);
                delay_ms(5);
                AUDIO_Handle_Send(NUMBER_USE_IS_EMPTY, 0, 0, NORMAL_TIME_SOUND);
            }
            Function_Handle_Setting_Clear();   
        }
    }else
    {
        AUDIO_Handle_Send(OPERATION_NOT_SUCCESSFUL, 0, 0, NORMAL_TIME_SOUND);
        Function_Clear_One_Fingerprint();
    }
}

bool Function_Check_User_Exist(void)
{
    uint8_t Temp = 0;
    for (uint8_t i = 0; i < 5; i++)
    {
        Temp_ADDRESS = ADDRESS_KEY_FIRST+SIZEOFFRAME*i;
        Temp = READ_Byte_From_Eeprom(Temp_ADDRESS);
        delay_ms(5);
        if(Temp != 0xFF)
        {
            return TRUE;
        } 
    }
    for (uint8_t i = 0; i < 5; i++)
    {
        Temp_ADDRESS = ADDRESS_CARD_FIRST+SIZEOFFRAME*i;
        Temp = READ_Byte_From_Eeprom(Temp_ADDRESS);
        delay_ms(5);
        if(Temp != 0xFF)
        {
            return TRUE;
        }     
    }
    for (uint8_t i = 0; i < 5; i++)
    {
        Temp_ADDRESS = ADDRESS_FINGERPRINT_FIRST+SIZEOFFRAME*i;
        Temp = READ_Byte_From_Eeprom(Temp_ADDRESS);
        delay_ms(5);
        if(Temp != 0xFF)
        {
            return TRUE;
        }     
    }
    return FALSE;
}

void Function_Remove_One_Fingerprint(uint8_t ID_Number)
{
    uint8_t Check_Sum = 0;
    Fingerprintsendclearonefinger[11] = ID_Number;
    for(uint8_t i = 6; i < 15; i++)
    {
        Check_Sum = Check_Sum + Fingerprintsendclearonefinger[i];
    }
    Fingerprintsendclearonefinger[15] = Check_Sum;
    disableInterrupts();
    // TIME_Out(0xFFFF);
    delay_ms(100);
    UART_SendData(UART1, Fingerprintsendclearonefinger, sizeof(Fingerprintsendclearonefinger));
    EXTI_ClearITPendingBit(EXTI_IT_Pin7);
    enableInterrupts();
}

void Function_Clear_All_Key(void)
{
    Key = 0;
    for(uint8_t i = 0; i < 25; i++)
    {
        if(READ_Byte_From_Eeprom(ADDRESS_KEY_FIRST + 24*i) == 0xFF)
        {
            Key++;
        }else
        {
            Key = 0;
            break;
        }
        delay_ms(5);
    }
    if(Key == 0)
    {
        LED_Dispaly(0x0A, 0x00); // Bat LED * #
        do
        {
            Function_Handle_Package_Sound_Step2_3();
            if(Key == '*')
            {
                AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
                Function_Handle_Setting_Clear();
                break;
            }
            else if(Key == '#')
            {
                AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
                Function_Handle_Clear_All_Key();
                break;
            }

        }while((Key != '#') && (Sleep_Flag == 0));
    }else
    {
        LED_Dispaly(0x00, 0x00);
        WRITER_Byte_To_Eeprom(ADDRESS_SAVE_PIN, 0xFF);
        delay_ms(5);
        AUDIO_Handle_Send(NUMBER_USE_IS_EMPTY, 0, 0, NORMAL_TIME_SOUND);
        Function_Handle_Setting_Clear();
    }
}

void Function_Handle_Package_Sound_Step2_3(void)
{
    do
    {
        Key = AUDIO_Handle_Send_0(TO_CONFIRM_PRESS_THANG, 0, 0, MID_TIME_SOUND, Check_Language);
        if(Key != 0) break;
        else
        {
            
            Key = AUDIO_Handle_Send_0(TO_BACK_PRESS_SAO, 0, 0, HIGHT_TIME_SOUND, Check_Language);
            if(Key != 0) 
            {
                Start_Count = 0;
                break;
            }
            else
            {
                Start_Count = 0;
                break;
            }                
                
        }
        
    } while ((Key == 0) && (Start_Count == 1));
}

void Function_Handle_Clear_All_Key(void)
{
    LED_Dispaly(0x00, 0x00); // Off led
    for(uint16_t i = 0; i < ALL_FRAME_USE_OF_KEY; i++)
    {
        READ_Frame_From_Eeprom(Password_Save, ADDRESS_KEY_FIRST+SIZEOFFRAME*i, SIZEOFFRAME);
        if(Password_Save[0] != 0xFF)
        {
            Buffer_vtupdateclearpincode[4] = i+1;
            Buffer_vtupdateclearpincode[5] = 0;
            for (uint8_t j = 0; j < 5; j++)
            {
                Buffer_vtupdateclearpincode[5] ^= Buffer_vtupdateclearpincode[j];
            }
            ZB_Wakeup();
            NCP_Send(Buffer_vtupdateclearpincode, sizeof(Buffer_vtupdateclearpincode));
            Temp_ADDRESS = ADDRESS_KEY_FIRST+SIZEOFFRAME*i;
            cleanmemory(Password_Array, 0xFF, SIZEOFFRAME);
            WRITER_Frame_To_Eeprom(Temp_ADDRESS, Password_Array, SIZEOFFRAME);
        }
    
    }
    WRITER_Byte_To_Eeprom(ADDRESS_SAVE_PIN, 0xFF);
    delay_ms(5);
    Check_All_Finger =  READ_Byte_From_Eeprom(ADDRESS_SAVE_FINGER);
    delay_ms(5);
    Check_All_Card = READ_Byte_From_Eeprom(ADDRESS_SAVE_CARD);
    delay_ms(5);
    if((Check_All_Finger == 0xFF) && (Check_All_Card == 0xFF))
    {
        WRITER_Byte_To_Eeprom(ADDRESS_SAVE_STATE_EEPROM, 0xFF);
        delay_ms(5);
    }
    cleanmemory(Password_Array, NULL, SIZEOFFRAME); 
    AUDIO_Handle_Send(OPERATION_SUCCESSFUL, 0, 0, NORMAL_TIME_SOUND);
    Function_Handle_Setting_Clear();
}

void Function_Clear_All_Card(void)
{
    Key = 0;
    for(uint8_t i = 0; i < 25; i++)
    {
        if(READ_Byte_From_Eeprom(ADDRESS_CARD_FIRST + 24*i) == 0xFF)
        {
            Key++;
        }else
        {
            Key = 0;
            break;
        }
        delay_ms(5);
    }
    if(Key == 0)
    {
        LED_Dispaly(0x0A, 0x00); // Bat LED * #
        do
        {
            Function_Handle_Package_Sound_Step2_3();
            Key = GET_Key();
            if(Key == '*')
            {
                AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
                Function_Handle_Setting_Clear();
                break;
            }
            else if(Key == '#')
            {
                AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
                Function_Handle_Clear_All_Card();
                break;
            }  

        }while((Key != '#') && (Sleep_Flag == 0));
    }else
    {
        LED_Dispaly(0x00, 0x00);
        WRITER_Byte_To_Eeprom(ADDRESS_SAVE_CARD, 0xFF);
        delay_ms(5);
        AUDIO_Handle_Send(NUMBER_USE_IS_EMPTY, 0, 0, NORMAL_TIME_SOUND);
        Function_Handle_Setting_Clear();
    }
}

void Function_Handle_Clear_All_Card(void)
{
    LED_Dispaly(0x00, 0x00); // off led

    for(uint16_t i = 0; i < 40; i++)
    {
        Temp_ADDRESS = ADDRESS_CARD_FIRST + i;
        WRITER_Byte_To_Eeprom(Temp_ADDRESS, 0xFF);
        delay_ms(5);
    }
    for(uint16_t i = 10; i < 46; i++)
    {
        Temp_ADDRESS = i;
        WRITER_Page_To_Eeprom(Temp_ADDRESS, 0xFF);
        delay_ms(5);
    }
    for(uint16_t i = 0; i < 56; i++)
    {
        Temp_ADDRESS = 2944 + i;
        WRITER_Byte_To_Eeprom(Temp_ADDRESS, 0xFF);
        delay_ms(5);
    }

    WRITER_Byte_To_Eeprom(ADDRESS_SAVE_CARD, 0xFF);
    delay_ms(5);
    Check_All_Finger =  READ_Byte_From_Eeprom(ADDRESS_SAVE_FINGER);
    delay_ms(5);
    Check_All_Pin = READ_Byte_From_Eeprom(ADDRESS_SAVE_PIN);
    delay_ms(5);
    if((Check_All_Finger == 0xFF) && (Check_All_Pin == 0xFF))
    {
        WRITER_Byte_To_Eeprom(ADDRESS_SAVE_STATE_EEPROM, 0xFF);
        delay_ms(5);
    }
    cleanmemory(Password_Array, NULL, SIZEOFFRAME); 
    AUDIO_Handle_Send(OPERATION_SUCCESSFUL, 0, 0, NORMAL_TIME_SOUND);
    Function_Handle_Setting_Clear();
}

void Function_Clear_All_Fingerprint(void)
{
    Key = 0;
    for(uint8_t i = 0; i < 25; i++)
    {
        if(READ_Byte_From_Eeprom(ADDRESS_FINGERPRINT_FIRST + 24*i) == 0xFF)
        {
            Key++;
        }else
        {
            Key = 0;
            break;
        }
        delay_ms(5);
    }
    if(Key == 0)
    {
        LED_Dispaly(0x0A, 0x00); // Bat LED * #
        do
        {
            Function_Handle_Package_Sound_Step2_3();
            Key = GET_Key();
            if(Key == '*')
            {
                AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
                Function_Handle_Setting_Clear();
                break;
            }else if(Key == '#')
            {
                AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
                Function_Handle_Clear_All_Fingerprint();
                break;
            }

        }while((Key != '#') && (Sleep_Flag == 0));
    }else
    {
        LED_Dispaly(0x00, 0x00);
        WRITER_Byte_To_Eeprom(ADDRESS_SAVE_FINGER, 0xFF);
        delay_ms(5);
        AUDIO_Handle_Send(NUMBER_USE_IS_EMPTY, 0, 0, NORMAL_TIME_SOUND);
        Function_Handle_Setting_Clear();
    }
    
}

void Function_Handle_Clear_All_Fingerprint(void)
{
    LED_Dispaly(0x00, 0x00); // off led

    for(uint16_t i = 0; i < 8; i++)
    {
        Temp_ADDRESS = ADDRESS_FINGERPRINT_FIRST + i;
        WRITER_Byte_To_Eeprom(Temp_ADDRESS, 0xFF);
        delay_ms(5);
    }
    for(uint16_t i = 47; i < 84; i++)
    {
        Temp_ADDRESS = i;
        WRITER_Page_To_Eeprom(Temp_ADDRESS, 0xFF);
        delay_ms(5);
    }
    for(uint16_t i = 0; i < 24; i++)
    {
        Temp_ADDRESS = 5376 + i;
        WRITER_Byte_To_Eeprom(Temp_ADDRESS, 0xFF);
        delay_ms(5);
    }

    WRITER_Byte_To_Eeprom(ADDRESS_SAVE_FINGER, 0xFF);
    delay_ms(5);
    Check_All_Pin =  READ_Byte_From_Eeprom(ADDRESS_SAVE_PIN);
    delay_ms(5);
    Check_All_Card = READ_Byte_From_Eeprom(ADDRESS_SAVE_CARD);
    delay_ms(5);
    if((Check_All_Pin == 0xFF) && (Check_All_Card == 0xFF))
    {
        WRITER_Byte_To_Eeprom(ADDRESS_SAVE_STATE_EEPROM, 0xFF);
        delay_ms(5);
    }
    FingerPrint_Send_Clear_All_User();
    cleanmemory(Password_Array, NULL, SIZEOFFRAME);
    AUDIO_Handle_Send(OPERATION_SUCCESSFUL, 0, 0, NORMAL_TIME_SOUND);
    Function_Handle_Setting_Clear();
}

void Function_Handle_Setting_System(void)
{
    LED_Dispaly(0x0A, 0x0F); // Bat led * #, 1, 2, 3, 4
    SettingSystemFlag = 1;
    do
    {
        Function_Handle_Package_Sound_Step3_1();
        Key = GET_Key();
        switch (Key)
        {
        case '*':
            AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
            Function_Handle_Setting();
            break;
        case '#':
            AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
            break;
        case '1':
            AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
            Function_Setting_Volume();
            break;
        case '2':
            AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
            Function_Setting_Language();
            break;
        case '3':
            AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
            Function_Setting_Status_Left_Lock();
            break;
        case '4':
            AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
            Function_Recover_Setting_Original();
            break;
        default:
            Key = '#';
            break;
        }
    }while((Key == '#')&&(Sleep_Flag == 0));
}

void Function_Handle_Package_Sound_Step3_1(void)
{
    do
    {
        Key = AUDIO_Handle_Send_4(ADJ_SOUND, PRESS_KEY, SPEAK_1, NORMAL_TIME_SOUND, Check_Language);
        if(Key != 0) break;
        else
        {
            Key = AUDIO_Handle_Send_4(SETTING_LANGUAGE, PRESS_KEY, SPEAK_2, NORMAL_TIME_SOUND, Check_Language);
            if(Key != 0) break;
            else 
            {
                Key = AUDIO_Handle_Send_4(SETTING_LOCK_LEFT, PRESS_KEY, SPEAK_3, MID_MIN_TIME_SOUND, Check_Language);
                if(Key != 0) break;
                else
                {
                    Key = AUDIO_Handle_Send_4(SYSTEM_RECOVERY, PRESS_KEY, SPEAK_4, MID_MIN_TIME_SOUND, Check_Language);
                    if(Key != 0) break;
                    else
                    {
                        Key = AUDIO_Handle_Send_4(TO_BACK_PRESS_SAO, 0, 0, MID_TIME_SOUND, Check_Language);
                        if(Key != 0) 
                        {
                            Start_Count = 0;
                            break;
                        }
                        else
                        {
                            Key = AUDIO_Handle_Send_4(LISTEN_AGAIN_PRESS_THANG, 0, 0, MID_TIME_SOUND, Check_Language);
                            if(Key != 0) 
                            {
                                Start_Count = 0;
                                break;
                            }
                            else
                            {
                                Start_Count = 0;
                                break;
                            }
                            
                        }   
                    }
                                
                }
                
            }
        }
        
    } while ((Key == 0) && (Start_Count == 1));
}

void Function_Setting_Volume()
{
    LED_Dispaly(0x0A, 0x0F); // Bat led * #, 1, 2, 3, 4
    do
    {
        Function_Handle_Package_Sound_Step3_1_1();
        Key = GET_Key();
        switch (Key)
        {
        case '*':
            AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
            Function_Handle_Setting_System();
            break;
        case '#':
            AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
            break;
        case '1':
            AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
            Function_Setting_Loud_Volume();
            break;
        case '2':
            AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
            Function_Setting_Mid_Volume();
            break;
        case '3':
            AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
            Function_Setting_Small_Volume();
            break;
        case '4':
            AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
            Function_Setting_Clam_Volume();
            break;
        default:
            Key = '#';
            break;
        }
    }while((Key == '#')&&(Sleep_Flag == 0));
}

void Function_Handle_Package_Sound_Step3_1_1(void)
{
    do
    {
        Key = AUDIO_Handle_Send_4(MAX_SOUND, PRESS_KEY, SPEAK_1, MID_MIN_MIN_TIME_SOUND, Check_Language);
        if(Key != 0) break;
        else
        {
            Key = AUDIO_Handle_Send_4(MIND_SOUND, PRESS_KEY, SPEAK_2, MID_MIN_MIN_TIME_SOUND, Check_Language);
            if(Key != 0) break;
            else 
            {
                Key = AUDIO_Handle_Send_4(MIN_SOUND, PRESS_KEY, SPEAK_3, MID_MIN_MIN_TIME_SOUND, Check_Language);
                if(Key != 0) break;
                else
                {
                    Key = AUDIO_Handle_Send_4(CLAM_SOUND, PRESS_KEY, SPEAK_4, MID_MIN_MIN_TIME_SOUND, Check_Language);
                    if(Key != 0) break;
                    else
                    {
                        Key = AUDIO_Handle_Send_4(TO_BACK_PRESS_SAO, 0, 0, MID_TIME_SOUND, Check_Language);
                        if(Key != 0) 
                        {
                            Start_Count = 0;
                            break;
                        }
                        else
                        {
                            Key = AUDIO_Handle_Send_4(LISTEN_AGAIN_PRESS_THANG, 0, 0, MID_TIME_SOUND, Check_Language);
                            if(Key != 0) 
                            {
                                Start_Count = 0;
                                break;
                            }
                            else
                            {
                                Start_Count = 0;
                                break;
                            }
                            
                        }   
                    }
                                
                }
                
            }
        }
        
    } while ((Key == 0) && (Start_Count == 1));
}

void Function_Setting_Loud_Volume()
{
    Check_Flag_Volume = 0;
    Check_Flag_Volume_MID_SMALL = 0;
    LED_Dispaly(0x00, 0x00);
    WRITER_Byte_To_Eeprom(ADDRESS_VOLUME, 0xFF);
    AUDIO_Send(BIG_SOUND);
    AUDIO_Handle_Send(OPERATION_SUCCESSFUL, 0, 0, MID_MIN_TIME_SOUND);
    Function_Handle_Setting_System();
}

void Function_Setting_Mid_Volume()
{
    Check_Flag_Volume = 0;
    Check_Flag_Volume_MID_SMALL = 1;
    LED_Dispaly(0x00, 0x00);
    WRITER_Byte_To_Eeprom(ADDRESS_VOLUME, 0x00);
    AUDIO_Send(NORMAL_SOUND);
    AUDIO_Handle_Send(OPERATION_SUCCESSFUL, 0, 0, MID_MIN_TIME_SOUND);
    Function_Handle_Setting_System();
}

void Function_Setting_Small_Volume()
{
    Check_Flag_Volume = 0;
    Check_Flag_Volume_MID_SMALL = 1;
    LED_Dispaly(0x00, 0x00);
    WRITER_Byte_To_Eeprom(ADDRESS_VOLUME, 0x00);
    AUDIO_Send(NORMAL_SOUND);
    AUDIO_Handle_Send(OPERATION_SUCCESSFUL, 0, 0, MID_MIN_TIME_SOUND);
    Function_Handle_Setting_System();
}

void Function_Setting_Clam_Volume()
{
    Check_Flag_Volume = 1;
    Check_Flag_Volume_MID_SMALL = 0;
    WRITER_Byte_To_Eeprom(ADDRESS_VOLUME, 0x01);
    AUDIO_Handle_Send(OPERATION_SUCCESSFUL, 0, 0, MID_MIN_TIME_SOUND);
    Function_Handle_Setting_System();
}

void Function_Setting_Language()
{
    LED_Dispaly(0x0A, 0x03); // Bat led * #, 1, 2
    do
    {
        Function_Handle_Package_Sound_Step3_2();
        Key = GET_Key();
        switch (Key)
        {
        case '*':
            AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
            Function_Handle_Setting_System();
            break;
        case '#':
            AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
            break;
        case '1':
            AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
            Function_Setting_Vietnamese_Language();
            break;
        case '2':
            AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
            Function_Setting_English_Language();
            break;
        default:
            Key = '#';
            break;
        }
    }while((Key == '#')&&(Sleep_Flag == 0));
}

void Function_Handle_Package_Sound_Step3_2(void)
{
    do
    {
        if(Check_Language == 0)
        {
            Key = AUDIO_Handle_Send_2(VIETNAMESE, PRESS_KEY, SPEAK_1, MIN_TIME_SOUND, Check_Language);
            if(Key != 0) break;
            else
            {
                Key = AUDIO_Handle_Send_2(ENGLISH, PRESS_KEY, SPEAK_2, MIN_TIME_SOUND, Check_Language);
                if(Key != 0) break;
                else 
                {
                    Key = AUDIO_Handle_Send_2(TO_BACK_PRESS_SAO, 0, 0, MID_TIME_SOUND, Check_Language);
                    if(Key != 0) 
                    {
                        Start_Count = 0;
                        break;
                    }
                    else
                    {
                        Key = AUDIO_Handle_Send_2(LISTEN_AGAIN_PRESS_THANG, 0, 0, MID_TIME_SOUND, Check_Language);
                        if(Key != 0) 
                        {
                            Start_Count = 0;
                            break;
                        }
                        else
                        {
                            Start_Count = 0;
                            break;
                        }
                        
                    }
                                    
                    
                }
            }
        }
        else
        {
            Key = AUDIO_Handle_Send(VIETNAMESE-1, PRESS_KEY, SPEAK_1, MIN_TIME_SOUND);
            if(Key != 0) break;
            else
            {
                Key = AUDIO_Handle_Send(ENGLISH-1, PRESS_KEY, SPEAK_2, MIN_TIME_SOUND);
                if(Key != 0) break;
                else 
                {
                    Key = AUDIO_Handle_Send(TO_BACK_PRESS_SAO, 0, 0, MID_TIME_SOUND);
                    if(Key != 0) 
                    {
                        Start_Count = 0;
                        break;
                    }
                    else
                    {
                        Key = AUDIO_Handle_Send(LISTEN_AGAIN_PRESS_THANG, 0, 0, MID_TIME_SOUND);
                        if(Key != 0) 
                        {
                            Start_Count = 0;
                            break;
                        }
                        else
                        {
                            Start_Count = 0;
                            break;
                        }
                        
                    }
                                    
                    
                }
            }
        }
        
        
    } while ((Key == 0) && (Start_Count == 1));
}

void Function_Setting_Vietnamese_Language(void)
{
    Check_Language = 0;
    WRITER_Byte_To_Eeprom(ADDRESS_LANGUAGE, 0x00);
    AUDIO_Handle_Send(OPERATION_SUCCESSFUL, 0, 0, MID_MIN_TIME_SOUND);
    Function_Handle_Setting_System();
}

void Function_Setting_English_Language(void)
{
    Check_Language = 1;
    WRITER_Byte_To_Eeprom(ADDRESS_LANGUAGE, 0x01);
    AUDIO_Handle_Send(OPERATION_SUCCESSFUL, 0, 0, MID_MIN_TIME_SOUND);
    Function_Handle_Setting_System();
}

void Function_Setting_Status_Left_Lock()
{
    LED_Dispaly(0x0A, 0x03); // Bat led * #, 1, 2
    do
    {
        Function_Handle_Package_Sound_Step3_3();
        Key = GET_Key();
        switch (Key)
        {
        case '*':
            AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
            Function_Handle_Setting_System();
            break;
        case '#':
            AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
            break;
        case '1':
            AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
            Function_Setting_Left_Lock();
            break;
        case '2':
            AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
            Function_Setting_Normal();
            break;
        default:
            Key = '#';
            break;
        }
    }while((Key == '#')&&(Sleep_Flag == 0));
}

void Function_Handle_Package_Sound_Step3_3(void)
{
    do
    {
        Key = AUDIO_Handle_Send_2(USE_LOCK_LEFT, PRESS_KEY, SPEAK_1, MID_MIN_TIME_SOUND, Check_Language);
        if(Key != 0) break;
        else
        {
            Key = AUDIO_Handle_Send_2(UNUSER_LOCK_LEFT, PRESS_KEY, SPEAK_2, MID_MIN_TIME_SOUND, Check_Language);
            if(Key != 0) break;
            else 
            {
                Key = AUDIO_Handle_Send_2(TO_BACK_PRESS_SAO, 0, 0, MID_TIME_SOUND, Check_Language);
                if(Key != 0) 
                {
                    Start_Count = 0;
                    break;
                }
                else
                {
                    Key = AUDIO_Handle_Send_2(LISTEN_AGAIN_PRESS_THANG, 0, 0, MID_TIME_SOUND, Check_Language);
                    if(Key != 0) 
                    {
                        Start_Count = 0;
                        break;
                    }
                    else
                    {
                        Start_Count = 0;
                        break;
                    }
                    
                }
                                
                
            }
        }
        
    } while ((Key == 0) && (Start_Count == 1));
}

void Function_Setting_Left_Lock(void)
{
    // function enable leftlock
    LED_Dispaly(0x00, 0x00);
    AUDIO_Handle_Send(OPERATION_SUCCESSFUL, 0, 0, NORMAL_TIME_SOUND);
    Use_Left_Lock_State = 1;
    WRITER_Byte_To_Eeprom(ADDRESS_LEFT_LOCK_STATE, 0x01);
    Function_Handle_Setting_System();
    Sleep_Flag = 1;
}

void Function_Setting_Normal(void)
{
    // function disable leftlock
    LED_Dispaly(0x00, 0x00);
    AUDIO_Handle_Send(OPERATION_SUCCESSFUL, 0, 0, NORMAL_TIME_SOUND);
    Use_Left_Lock_State = 0;
    WRITER_Byte_To_Eeprom(ADDRESS_LEFT_LOCK_STATE, 0xFF);
    Function_Handle_Setting_System();
    Sleep_Flag = 1;
}

void Function_Recover_Setting_Original()
{
    LED_Dispaly(0x0A, 0x00); // Bat led * #
    do
    {
        Function_Handle_Package_Sound_Step4();
        Key = GET_Key();
        if(Key == '*')
        {
            AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
            Function_Handle_Setting_System();
            break;
        }
        else if(Key == '#')
        {
            AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
            LED_Dispaly(0x00, 0x00);
            cleanmemory(Password_Array, 0xFF, SIZEOFFRAME);

            ZB_Wakeup();
            NCP_Send(Buffer_vtlockbyauto, sizeof(Buffer_vtlockbyauto));
            delay_ms(10);

            for(uint16_t i = 0; i < ALL_FRAME_USE_OF_KEY; i++)
            {
                Buffer_vtupdateclearpincode[4] = i+1;
                Buffer_vtupdateclearpincode[5] = 0;
                for (uint8_t j = 0; j < 5; j++)
                {
                    Buffer_vtupdateclearpincode[5] ^= Buffer_vtupdateclearpincode[j];
                }
                ZB_Wakeup();
                NCP_Send(Buffer_vtupdateclearpincode, sizeof(Buffer_vtupdateclearpincode));
                delay_ms(10);
            
            }

            for(uint16_t i = 0; i < ALL_PAGE_USE_COMMON; i++)
            {
                WRITER_Page_To_Eeprom(i, 0xFF);
                delay_ms(5);
            }

            WRITER_Byte_To_Eeprom(ADDRESS_SAVE_FINGER, 0xFF);
            delay_ms(5);
            WRITER_Byte_To_Eeprom(ADDRESS_SAVE_PIN, 0xFF);
            delay_ms(5);
            WRITER_Byte_To_Eeprom(ADDRESS_SAVE_CARD, 0xFF);
            delay_ms(5);
            WRITER_Byte_To_Eeprom(ADDRESS_SAVE_STATE_EEPROM, 0xFF);
            delay_ms(5);
            WRITER_Byte_To_Eeprom(ADDRESS_LEFT_LOCK_STATE, 0xFF);
            delay_ms(5);
            WRITER_Byte_To_Eeprom(ADDRESS_LANGUAGE, 0xFF);
            delay_ms(5);
            WRITER_Byte_To_Eeprom(ADDRESS_VOLUME, 0xFF);
            delay_ms(5);
    

            FingerPrint_Send_Clear_All_User();
            AUDIO_Get_Finger_Send(SUCCESSFULLY_RECOVERY_SYSTEM, MID_TIME_SOUND, Check_Language);
            Check_Faulty_Systm = 110;
            while(1);
        }

    } while ((Key != '#') && (Sleep_Flag == 0));
    Key = NULL;
}

void Function_Handle_Package_Sound_Step4(void)
{
    do
    {
        Key = AUDIO_Handle_Send_0(TO_CONFIRM_PRESS_THANG, 0, 0, MID_MIN_TIME_SOUND, Check_Language);
        if(Key != 0) 
        {
            Start_Count = 0;
            break;
        }
        else
        {
            Key = AUDIO_Handle_Send_0(TO_BACK_PRESS_SAO, 0, 0, NORMAL_TIME_SOUND, Check_Language);
            if(Key != 0) 
            {
                Start_Count = 0;
                break;
            }
            else
            {
                Start_Count = 0;
                break;
            }
            
        }                
                
        
    } while ((Key == 0) && (Start_Count == 1));
}

void Function_Handle_Setting_Introduce(void)
{
    uint8_t code = 0, count = 0;
    LED_Dispaly(0x0A, 0x00);
    do
    {
        Key = AUDIO_Handle_Send_0_8(NUMBER_BOARD, 0, 0, NORMAL_TIME_SOUND, Check_Language);
        if(Key == '*')
        {
            AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
            Sleep_Flag = 1;
            break;
        }else if(Key == 0)
        {
            for(uint8_t i = 4; i < 9; i ++)
            {
                code = READ_Byte_From_Eeprom(ADDRESS_DEVICE_CODE_FIRST+i);
                switch(code)
                {
                case '0':
                    Key = AUDIO_Handle_Send_0_8(SPEAK_0, 0, 0, MIN_TIME_SOUND-200, Check_Language);
                    break;
                case '1':
                    Key = AUDIO_Handle_Send_0_8(SPEAK_1, 0, 0, MIN_TIME_SOUND-200, Check_Language);
                    break;
                case '2':
                    Key = AUDIO_Handle_Send_0_8(SPEAK_2, 0, 0, MIN_TIME_SOUND-200, Check_Language);
                    break;
                case '3':
                    Key = AUDIO_Handle_Send_0_8(SPEAK_3, 0, 0, MIN_TIME_SOUND-200, Check_Language);
                    break;
                case '4':
                    Key = AUDIO_Handle_Send_0_8(SPEAK_4, 0, 0, MIN_TIME_SOUND-200, Check_Language);
                    break;
                case '5':
                    Key = AUDIO_Handle_Send_0_8(SPEAK_5, 0, 0, MIN_TIME_SOUND-200, Check_Language);
                    break;
                case '6':
                    Key = AUDIO_Handle_Send_0_8(SPEAK_6, 0, 0, MIN_TIME_SOUND-200, Check_Language);
                    break;
                case '7':
                    Key = AUDIO_Handle_Send_0_8(SPEAK_7, 0, 0, MIN_TIME_SOUND-200, Check_Language);
                    break;
                case '8':
                    Key = AUDIO_Handle_Send_0_8(SPEAK_8, 0, 0, MIN_TIME_SOUND-200, Check_Language);
                    break;
                case '9':
                    Key = AUDIO_Handle_Send_0_8(SPEAK_9, 0, 0, MIN_TIME_SOUND-200, Check_Language);
                    break;
                default:
                    Key = 0;
                    break;
                }

                if(Key == '*')
                {
                    AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
                    Sleep_Flag = 1;
                    break;
                }else if(Key == '#')
                {
                    AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
                    Start_Count = 1;
                    break;
                }else if(Key == 0)
                {
                    count++;
                    if(count == 5)
                    {
                        count = 0;
                        Sleep_Flag = 1;
                        Key = '*';
                        break;
                    }
                }
            }
        }else if(Key == '#')
        {
            AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
            Start_Count = 1;
        }
    } while ((Key != '*')&&(Start_Count == 1));
}

bool Function_Check_Finger(void)
{
    FingerPrint_Send_Check_Led();
    Check_Byte_In_Eeprom = Function_Check_AllByte_Use_In_Eeprom();
    FINGER_Control_Power_On(TIMEOUT_FINGER);
    if(Check_Byte_In_Eeprom == FALSE)
    {
        // eeprom khong co data
        Eeprom_Flag = 1;
        LED_Dispaly(0x02, 0x00);
        FingerPrint_Send_Success_Led();
        Drivermotor_Open(); Check_Motor = 1;
        delay_ms(500);
        FingerPrint_Send_Success_Led();
        LED_Dispaly(0x08, 0x00);
        Function_Handle_Package_Sound_Step0_0();
        if(Key == '#')
        {
            Key = 0;
            AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
            FingerPrint_Send_Off_Led();
            Function_Handle_Setting_Resgister();
        }else
        {
            Eeprom_Flag = 0;
            Sleep_Flag = 1;
        }
        
    }else
    {
        // eeprom co data
        LED_Dispaly(0x02, 0x00);
        Check_Compare_Finger = Function_Read_Fingerprint(Password_Array);
        
        if(Check_Compare_Finger == TRUE)
        {
            Function_Handle_Fingerprint_Correct();
            LED_Dispaly(0x00, 0x00);
            FingerPrint_Send_Off_Led();
            return TRUE;
        }else
        {
            Function_Handle_Fingerprint_Not_Correct();
            LED_Dispaly(0x00, 0x00);
            FingerPrint_Send_Off_Led();
            Sleep_Flag = 1;
            return FALSE;
        }
        
    }
    return FALSE;
}

bool Function_Read_Fingerprint(uint8_t* Password_Array)
{
    static uint8_t OriginalFingerAddress;
    Finger_Timeout = 0;
    Finger_Timeout_Flag = 0;
    Check_Hand_Search = SEARCH_Finger();
    Finger_Timeout_Flag = 0;
    if(Check_Hand_Search == FALSE)
    {
        FingerPrint_Send_Fail_Led();
        Check_Status_Success = false;
    }
    Function_Hand_Low_Battery();
    if(Check_Hand_Search == TRUE)
    {
        Check_Hand_Search = FALSE;
        OriginalFingerAddress = SearchFingerAddress(Password_Finger);
        Numberofpassword = READ_Frame_From_Eeprom(Password_Save,
                        ADDRESS_FINGERPRINT_FIRST+SIZEOFFRAME*(OriginalFingerAddress-1), 
                        SIZEOFFRAME);
        for(uint8_t i = 0; i < 5; i++)
        {
            if(Password_Save[i] == Password_Finger)
            {
                Check_Id = Password_Save[Numberofpassword];
                Buffer_vtunlockbyfingerprint[5] = Check_Id;
                Buffer_vtunlockbyfingerprint[6] = 0;
                for(uint8_t i = 0; i < 6; i++)
                {
                    Buffer_vtunlockbyfingerprint[6] ^= Buffer_vtunlockbyfingerprint[i]; 
                }
                cleanmemory(Password_Array, NULL, SIZEOFFRAME); 
                return TRUE;
            }
        }
        return FALSE;
    }else
    {
        return FALSE; 
    }
}

void Function_Handle_Fingerprint_Correct(void)
{
    Numberoffnotcorrect = 0;
    FingerPrint_Send_Success_Led();
    Check_Type_Password = Function_Check_Finger_Manager_Or_User();

    if(Check_Type_Password == TRUE)
    {
        ZB_Wakeup();
        NCP_Send(Buffer_vtunlockbyfingerprint, sizeof(Buffer_vtunlockbyfingerprint));
    }else
    {
        if(Use_Left_Lock_State == 1)
        {
            if(GPIOD->IDR & GPIO_PIN_SWITCH_INT)
            {
                ZB_Wakeup();
                NCP_Send(Buffer_vtunlockbyfingerprint, sizeof(Buffer_vtunlockbyfingerprint));
            }
    
        }else
        {
            ZB_Wakeup();
            NCP_Send(Buffer_vtunlockbyfingerprint, sizeof(Buffer_vtunlockbyfingerprint));
        }
        
    }

    Function_Notification_Status();
    Check_Password_False = 0;
    if(Check_Type_Password == TRUE)
    {
        // Finger is manager
        Function_Handle_Finger_Manager();
    }
    else
    {
        // Finger is user
        Function_Handle_Finger_User();
    }
}

void Function_Notification_Status(void)
{
    if(Check_Type_Password == TRUE)
    {
        if(!(GPIOD->IDR & GPIO_PIN_SWITCH_INT))
        {
            LED_Dispaly(0x0A, 0x00); // Bat led * #
        }else
        {
            LED_Dispaly(0x0A, 0x20); // Bat led * # 6
        }
        
        if(Stateopenstatic == 0)
        {
            Drivermotor_Open(); 
            Check_Motor = 1;
        }else
        {
            LED_Dispaly(0x02, 0x00); // Bat led *
            Drivermotor_Close();
            Check_Motor = 1;
        }

		if(Smartcard_Flag == 0)
		{
		 	Key = AUDIO_Handle_Send_0_6(VERIFY_SUCCESSFUL, 0, 0, NORMAL_TIME_SOUND, Check_Language); 
		}else
		{
		  	if(Use_Left_Lock_State == 1)
			{
			  	if(GPIOD->IDR & GPIO_PIN_SWITCH_INT)
            	{
			  		Key = AUDIO_Handle_Send_0_6(VERIFY_SUCCESSFUL, 0, 0, NORMAL_TIME_SOUND, Check_Language);
				}else
				{
				  	Key = AUDIO_Handle_Send_0(VERIFY_SUCCESSFUL, 0, 0, NORMAL_TIME_SOUND, Check_Language);
				}
			}else
			{
			  	Key = AUDIO_Handle_Send_0_6(VERIFY_SUCCESSFUL, 0, 0, NORMAL_TIME_SOUND, Check_Language);
			}
		}
		
        if((Key != '*') && (Key != '#') && (Key != '6'))
        {
            if(Stateopenstatic == 0)
            {
                LED_Dispaly(0x0A, 0x00);
                Key = AUDIO_Handle_Send_0(PRESS_THANG_KEY_TO_ENTER_SETTING_LIST, 0, 0, HIGHT_TIME_SOUND, Check_Language);
            }else
            {
                Stateopenstatic = 0;
                ZB_Wakeup();
                NCP_Send(Buffer_vtlockbyauto, sizeof(Buffer_vtlockbyauto));
                AUDIO_Handle_Send(HAS_REMOVE_MOVED_TO_FIXED_STATE, 0, 0, MID_TIME_SOUND);
            }
        }
    }
    else
    {
        LED_Dispaly(0x02, 0x00); // Bat led *
        if(Use_Left_Lock_State == 1)
        {
            if(GPIOD->IDR & GPIO_PIN_SWITCH_INT)
            {
                if(Stateopenstatic == 0)
                {
                    Drivermotor_Open(); 
                    Check_Motor = 1;
                }else
                {
                    Drivermotor_Close();
                    Check_Motor = 1;
                }
                AUDIO_Handle_Send(VERIFY_SUCCESSFUL, 0, 0, MID_MIN_TIME_SOUND); 
                if(Stateopenstatic == 1)
                {
                    ZB_Wakeup();
                    NCP_Send(Buffer_vtlockbyauto, sizeof(Buffer_vtlockbyauto));
                    Stateopenstatic = 0;
                    AUDIO_Handle_Send(HAS_REMOVE_MOVED_TO_FIXED_STATE, 0, 0, MID_TIME_SOUND);
                }
            }else
            {
                // Key = 0;
                Key = AUDIO_Handle_Send(VERIFY_SUCCESSFUL, 0, 0, MID_MIN_TIME_SOUND);
                if(Key != '*')
                {
                    AUDIO_Handle_Send(INSIDE_LOCKED_DOOR, 0, 0, MID_MIN_TIME_SOUND);
                }
                Key = 0;
                
            }
            
        }else
        {
            if(Stateopenstatic == 0)
            {
                Drivermotor_Open(); 
                Check_Motor = 1;
            }else
            {
                Drivermotor_Close();
                Check_Motor = 1;
            }
            
            AUDIO_Handle_Send(VERIFY_SUCCESSFUL, 0, 0, MID_MIN_TIME_SOUND);
            if(Stateopenstatic == 1)
            {
                ZB_Wakeup();
                NCP_Send(Buffer_vtlockbyauto, sizeof(Buffer_vtlockbyauto));
                Stateopenstatic = 0;
                AUDIO_Handle_Send(HAS_REMOVE_MOVED_TO_FIXED_STATE, 0, 0, MID_TIME_SOUND);
            }
        }
        
    }
    
}

void Function_Handle_Finger_User(void)
{
    do
    {
        Key = GET_Key();
        if(Key == '*')
        {
            FingerPrint_Send_Off_Led();
            AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
            LED_Dispaly(0x00, 0x00);
            Function_Notification_Id();
            Sleep_Flag = 1;
        }

    } while ((Key != '*') && (Sleep_Flag == 0));
}

void Function_Handle_Finger_Manager(void)
{
    do
    {
        if(Key == '*')
        {
            Stateopenstatic = 0;
            FingerPrint_Send_Off_Led();
            AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
            LED_Dispaly(0x00, 0x00);
            Function_Notification_Id();
            Sleep_Flag = 1;
            break;
        }
        else if(Key == '#')
        {
            FingerPrint_Send_Off_Led();
            AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
            Function_Handle_Setting();
            break;
        }
        else if(Key == '6')
        {
            Key = 0;
            AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
            LED_Dispaly(0x0A, 0x80); // Bat led * # 8
            FingerPrint_Send_Off_Led();
            do
            {
                Key = GET_Key_8();
                switch (Key)
                {
                case '0':
                    Key = 0;
                    break;
                case '1':
                    Key = 0;
                    break;
                case '2':
                    Key = 0;
                    break;
                case '3':
                    Key = 0;
                    break;
                case '4':
                    Key = 0;
                    break;
                case '5':
                    Key = 0;
                    break;
                case '6':
                    Key = 0;
                    break;
                case '7':
                    Key = 0;
                    break;
                case '9':
                    Key = 0;
                    break;
                default:
                    break;
                }
            }
            while((Key == 0) && (Sleep_Flag == 0) && (Count_Motor != 0));
            if(Key == '*')
            {
                Stateopenstatic = 0;
                AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
                Function_Notification_Id();
                Sleep_Flag = 1;
                break;
            }
            else if(Key == '#')
            {
                AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
                Function_Handle_Setting();
                break;
            }
            else if(Key == '8')
            {
                Stateopenstatic = 1;
                AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
                LED_Dispaly(0x00, 0x00);
                Check_Motor = 0; Count_Motor = 0;
                AUDIO_Handle_Send(HAS_MOVED_TO_FIXED_STATE, 0, 0, MID_TIME_SOUND);
                Sleep_Flag = 1;
            }else
            {
                LED_Dispaly(0x00, 0x00);
                Sleep_Flag = 1;
            }
            
        }
    } while ((Key != '*') && (Sleep_Flag == 0) && (Key != '#'));
}

bool Function_Check_Finger_Manager_Or_User(void)
{
    if(Password_Save[6] == MANAGER)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

void Function_Handle_Fingerprint_Not_Correct(void)
{
    Numberoffnotcorrect++;
    if(Numberoffnotcorrect == 5)
    {
        Numberoffnotcorrect = 0;
        FINGER_Control_Power_Off();
        delay_ms(100);
        RESET_CPU();
        delay_ms(10000);
    }else
    {
        UART_SendData(UART1, Fingerprintenableinterrputstep2, sizeof(Fingerprintenableinterrputstep2));
        EXTI_ClearITPendingBit(EXTI_IT_Pin7);
        Count = 0; Finger_Flag = 0;
        AUDIO_Finger_Send(VERIFY_NOT_SUCCESSFUL, NORMAL_TIME_SOUND, Check_Language);   
    }
}

void Function_Save_Fingerprint_Into_Eeprom(uint8_t* Password_Array)
{
    Check_Hand_Enroll = Function_HandEnroll(Finger_Default);
    if(Check_Hand_Enroll == TRUE)
    {
        Check_Hand_Enroll = FALSE;
        for(uint8_t i = 0; i < SIZEOFFRAME; i++)
        {
            if(Password_Array[i] == NULL)
            {
                Password_Array[i] = ID; Password_Array[i+1] = MANAGER; Password_Array[i+2] = END_FRAME;
                WRITER_Frame_To_Eeprom(ADDRESS_FINGERPRINT_FIRST, Password_Array, 4); 
                cleanmemory(Password_Array, NULL, SIZEOFFRAME);
                FingerPrint_Send_Success_Led();
                AUDIO_Finger_Send(VERIFY_SUCCESSFUL, MID_MIN_TIME_SOUND, Check_Language);
                Sleep_Flag = 1; 
                break;
            }
        }
    }
        
}

bool Function_HandEnroll(uint8_t Password_Finger)
{   
    if(Check_Enroll == FALSE)
    {
        if(fingerPrintState == STATE_IDLE)
        {
            if(State == 0)
            {
                FingerPrint_HandShakeCmd();
                State = 1;
            }else if(State == 1)
            { 
                FingerPrint_EnrollFingercmd(Password_Finger);
            }
        }
    }else
    {
        Check_Enroll = FALSE;
        State = 0;
        return TRUE;
    }
    return FALSE;
}

bool Function_HandSearch(void)
{
    if(Check_Finger == FALSE)
    {
        if(fingerPrintState == STATE_IDLE)
        {
            if(State == 0)
            {
                FingerPrint_HandShakeCmd();
                State = 1;
            }else if(State == 1)
            { 
                FingerPrint_Search();
            }
        }
    }else
    {
        Check_Finger = FALSE;
        State = 0;
        return TRUE;
    }
    return FALSE;
}

void Function_Handle_Join_Network(void)
{
    Function_Join_Network();
    for(uint8_t i = 0; i < 5; i++)
    {
        NCP_Proc();
        // TIME_Out(0xFFFF);  
        delay_ms(100);
        if(zb_flagjoinnetwork == 1)
        {
            break;
        }
    }

    if(zb_flagjoinnetwork == 1)
    {
        AUDIO_Handle_Send(OPERATION_SUCCESSFUL, 0, 0, NORMAL_TIME_SOUND);
        zb_flagjoinnetwork = 0;
        Sleep_Flag = 1;
    }
}

void Function_Read_Adc_Value(void)
{
    Check_Operation_Of_Main = 1;

    // Init Audio, Adc pin
    GPIO_Init(GPIOA, AUDIO_PIN_COMMON, GPIO_Mode_Out_PP_High_Fast);
    GPIO_Init(GPIOA, GPIO_Pin_6, GPIO_Mode_In_FL_No_IT);
    delay_ms(20);

    if(Readbattery == 0)
    {
        Readbattery = 1;
        ADC_Config_Init();
        Battery_Value = (uint32_t)ADC_Read();
    }
}

void Function_Handle_Setting_State_Door(void)
{
    LED_Dispaly(0x00, 0x00);
    while (Check_Motor == 1);
    Drivermotor_Open();
    Check_Motor = 1;
    Stateopenstatic = 1;
    while (Check_Motor == 1);
    AUDIO_Handle_Send(HAS_MOVED_TO_FIXED_STATE, 0, 0, MID_TIME_SOUND);
    Sleep_Flag = 1;
}

void Function_Join_Network(void)
{
    ZB_Wakeup();
    NCP_Send(Buffer_vtupdatenetwork, sizeof(Buffer_vtupdatenetwork));
    while(Sleep_Flag == 0)
    {
        LED_Notifi_Init_Success(10);
    }
    
}

void Function_Hand_Low_Battery(void)
{
    uint32_t Tempbattery = 0;
    if(Battery_Value > MID_BATTERY_VALUE)
    {
        Tempbattery = 13750*Battery_Value/100000 - 230; // 13293
        if(Tempbattery > 100)
        {
            Tempbattery = 100;
        }
        
    }else if(Battery_Value > LOW_BATTERY_VALUE)
    {
        Tempbattery = 1000*Battery_Value/10000 - 160;   
    }else
    {
        Tempbattery = 1;
    }
    Buffer_vtupdatepercentbattery[4] = (uint8_t)Tempbattery;
    


    Buffer_vtupdatepercentbattery[5] = 0;
    for(uint8_t i = 0; i < 5; i++)
    {
        Buffer_vtupdatepercentbattery[5] ^= Buffer_vtupdatepercentbattery[i];
    }
    ZB_Wakeup();
    NCP_Send(Buffer_vtupdatepercentbattery, sizeof(Buffer_vtupdatepercentbattery));
    if(Tempbattery < 15)
    {
        ZB_Wakeup();
        NCP_Send(Buffer_vtupdatewarninglowbattery, sizeof(Buffer_vtupdatewarninglowbattery));
        if(Check_Flag_Volume == 1)
        {
            AUDIO_Send(BIG_SOUND);
        }
        AUDIO_Card_User_Manager_Send(LOW_BATTERY_WARNING, MID_MIN_TIME_SOUND, Check_Language);
        Timeout_Flag = 1;
        Count = 0;
        Sleep_Flag = 0;
        if(Check_Flag_Volume == 1)
        {
            AUDIO_Send(NOT_SOUND);
        }
    }
    
}

void Function_Check_Warning(void)
{
    if(GPIO_ReadInputDataBit(GPIOG, GPIO_PIN_TEMPER_INT) == RESET)
    {
        Temper_Flag = 1; 
    }
}

void Function_Check_Sleep(void)
{
    if(Sleep_Flag == 1)
    {   
        //Vao sleep mode
        if(Bell_Flag == 1)
        {
            Bell_Flag = 0;
        }
        else if(Switch_Flag == 1)
        {
            Switch_Flag = 0;
        
        }
        else if(zb_flaggetpin == 1)
        {   
            zb_flaggetpin = 0;
        }
        else if((SettingSystemFlag == 1) || (zb_flaggetallpin == 1) || (Eeprom_Flag == 1) \
                || (zb_flagsetpin == 1)||(zb_flagclearpincode == 1))
        { 
            LED_Dispaly(0x00, 0x00);
            AUDIO_Common_Send(SYSTEM_HAS_EXITED, MID_TIME_SOUND, Check_Language); 
        }
        SLEEP_Mode();
    }
}

