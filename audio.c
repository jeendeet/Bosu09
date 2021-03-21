#include "audio.h"
#include "delay.h"
#include "define.h"
#include "definepin.h"
#include "getpassword.h"
#include "tsm12.h"

extern volatile uint16_t Timeout_Sound_Count, Start_Count;
extern uint8_t Check_Language, Check_Key;
extern volatile uint8_t Finger_Flag, Button_Card_Flag, Bell_Flag, Switch_Flag, zb_flagsetlock;
extern volatile uint8_t Temper_Flag, Timeout_Sound_Flag, Check_Faulty_Systm;
extern uint8_t Password_Array[16];
uint8_t Firsttime = 1;

void AUDIO_GPIO_Init(void)
{
    GPIO_Init(GPIOA, AUDIO_PIN_COMMON, GPIO_Mode_Out_PP_High_Fast);
    GPIO_Init(GPIOA, AUDIO_PIN_PLAY, GPIO_Mode_In_FL_No_IT);
}

void AUDIO_Send(uint8_t Address)
{
    delay_ms(20);
    GPIO_ResetBits(GPIOA, AUDIO_PIN_CS);
    Check_Faulty_Systm = 0;
    delay_ms(5);
    for(uint8_t i = 0; i < 8; i++)
    {
        GPIO_ResetBits(GPIOA, AUDIO_PIN_CLK);
        if(Address & 0x01)
        {
            GPIO_SetBits(GPIOA, AUDIO_PIN_DATA);
        }
        else
        {
            GPIO_ResetBits(GPIOA, AUDIO_PIN_DATA);
        }
        Address >>= 1;
        delay_us(100);
        GPIO_SetBits(GPIOA, AUDIO_PIN_CLK);
        delay_us(100);
        
    }
    GPIO_SetBits(GPIOA, AUDIO_PIN_CS);
}

void AUDIO_Handle_Send_Speak(uint8_t Type_Command)
{
    AUDIO_Send(Type_Command);
    delay_ms(2);
    while (!GPIO_ReadInputDataBit(GPIOA, AUDIO_PIN_PLAY));
    
}

uint8_t AUDIO_Handle_Send_2(uint8_t Type_Command1, uint8_t Type_Command1_1, uint8_t Type_Command1_2, uint16_t Time_Sound, uint8_t Language)
{
    uint8_t Key = 0;
    AUDIO_Send(Type_Command1+Language);
    Start_Count = 1; Timeout_Sound_Count = 0;
    do
    {
        Key = TSM12_key_pad_scan();
        switch (Key)
        {
        case '0':
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
        case '8':
            Key = 0;
            break;
        case '9':
            Key = 0;
            break;
        default:
            break;
        }
    } while ((Key == 0)&&(Timeout_Sound_Count < Time_Sound));
    if(Key != 0) 
    {
        Start_Count = 0;
        return Key;
    }else
    {
        if((Type_Command1_1 == 0) || (Type_Command1_2 == 0)) 
        {
            return 0;
        } else
        {
            AUDIO_Send(Type_Command1_1+Language);
            Start_Count = 1; Timeout_Sound_Count = 0;
            do
            {
                Key = TSM12_key_pad_scan();
                switch (Key)
                {
                case '0':
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
                case '8':
                    Key = 0;
                    break;
                case '9':
                    Key = 0;
                    break;
                default:
                    break;
                }
            } while ((Key == 0)&&(Timeout_Sound_Count < MIN_TIME_SOUND));
            if(Key != 0) 
            {
                Start_Count = 0;
                return Key;
            }else
            {
                AUDIO_Send(Type_Command1_2+Language);
                Start_Count = 1; Timeout_Sound_Count = 0;
                do
                {
                    Key = TSM12_key_pad_scan();
                    switch (Key)
                    {
                    case '0':
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
                    case '8':
                        Key = 0;
                        break;
                    case '9':
                        Key = 0;
                        break;
                    default:
                        break;
                    }
                } while ((Key == 0)&&(Timeout_Sound_Count < MIN_TIME_SOUND));
                if(Key != 0)
                {
                    Start_Count = 0;
                    return Key;
                }else
                {
                    return 0;
                }
                
            }
        }
    }   
}

uint8_t AUDIO_Handle_Send_3(uint8_t Type_Command1, uint8_t Type_Command1_1, uint8_t Type_Command1_2, uint16_t Time_Sound, uint8_t Language)
{
    uint8_t Key = 0;
    AUDIO_Send(Type_Command1+Language);
    Start_Count = 1; Timeout_Sound_Count = 0;
    do
    {
        Key = TSM12_key_pad_scan();
        switch (Key)
        {
        case '0':
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
        case '8':
            Key = 0;
            break;
        case '9':
            Key = 0;
            break;
        default:
            break;
        }
    } while ((Key == 0)&&(Timeout_Sound_Count < Time_Sound));
    if(Key != 0) 
    {
        Start_Count = 0;
        return Key;
    }else
    {
        if((Type_Command1_1 == 0) || (Type_Command1_2 == 0)) 
        {
            return 0;
        } else
        {
            AUDIO_Send(Type_Command1_1+Language);
            Start_Count = 1; Timeout_Sound_Count = 0;
            do
            {
                Key = TSM12_key_pad_scan();
                switch (Key)
                {
                case '0':
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
                case '8':
                    Key = 0;
                    break;
                case '9':
                    Key = 0;
                    break;
                default:
                    break;
                }
            } while ((Key == 0)&&(Timeout_Sound_Count < MIN_TIME_SOUND));
            if(Key != 0) 
            {
                Start_Count = 0;
                return Key;
            }else
            {
                AUDIO_Send(Type_Command1_2+Language);
                Start_Count = 1; Timeout_Sound_Count = 0;
                do
                {
                    Key = TSM12_key_pad_scan();
                    switch (Key)
                    {
                    case '0':
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
                    case '8':
                        Key = 0;
                        break;
                    case '9':
                        Key = 0;
                        break;
                    default:
                        break;
                    }
                } while ((Key == 0)&&(Timeout_Sound_Count < MIN_TIME_SOUND));
                if(Key != 0)
                {
                    Start_Count = 0;
                    return Key;
                }else
                {
                    return 0;
                }
                
            }
        }
    }   
}

uint8_t AUDIO_Handle_Send_4(uint8_t Type_Command1, uint8_t Type_Command1_1, uint8_t Type_Command1_2, uint16_t Time_Sound, uint8_t Language)
{
    uint8_t Key = 0;
    AUDIO_Send(Type_Command1+Language);
    Start_Count = 1; Timeout_Sound_Count = 0;
    do
    {
        Key = TSM12_key_pad_scan();
        switch (Key)
        {
        case '0':
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
        case '8':
            Key = 0;
            break;
        case '9':
            Key = 0;
            break;
        default:
            break;
        }
    } while ((Key == 0)&&(Timeout_Sound_Count < Time_Sound));
    if(Key != 0) 
    {
        Start_Count = 0;
        return Key;
    }else
    {
        if((Type_Command1_1 == 0) || (Type_Command1_2 == 0)) 
        {
            return 0;
        } else
        {
            AUDIO_Send(Type_Command1_1+Language);
            Start_Count = 1; Timeout_Sound_Count = 0;
            do
            {
                Key = TSM12_key_pad_scan();
                switch (Key)
                {
                case '0':
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
                case '8':
                    Key = 0;
                    break;
                case '9':
                    Key = 0;
                    break;
                default:
                    break;
                }
            } while ((Key == 0)&&(Timeout_Sound_Count < MIN_TIME_SOUND));
            if(Key != 0) 
            {
                Start_Count = 0;
                return Key;
            }else
            {
                AUDIO_Send(Type_Command1_2+Language);
                Start_Count = 1; Timeout_Sound_Count = 0;
                do
                {
                    Key = TSM12_key_pad_scan();
                    switch (Key)
                    {
                    case '0':
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
                    case '8':
                        Key = 0;
                        break;
                    case '9':
                        Key = 0;
                        break;
                    default:
                        break;
                    }
                } while ((Key == 0)&&(Timeout_Sound_Count < MIN_TIME_SOUND));
                if(Key != 0)
                {
                    Start_Count = 0;
                    return Key;
                }else
                {
                    return 0;
                }
                
            }
        }
    }   
}

uint8_t AUDIO_Handle_Send_5(uint8_t Type_Command1, uint8_t Type_Command1_1, uint8_t Type_Command1_2, uint16_t Time_Sound, uint8_t Language)
{
    uint8_t Key = 0;
    AUDIO_Send(Type_Command1+Language);
    Start_Count = 1; Timeout_Sound_Count = 0;
    do
    {
        Key = TSM12_key_pad_scan();
        switch (Key)
        {
        case '0':
            Key = 0;
            break;
        case '6':
            Key = 0;
            break;
        case '7':
            Key = 0;
            break;
        case '8':
            Key = 0;
            break;
        case '9':
            Key = 0;
            break;
        default:
            break;
        }
    } while ((Key == 0)&&(Timeout_Sound_Count < Time_Sound));
    if(Key != 0) 
    {
        Start_Count = 0;
        return Key;
    }else
    {
        if((Type_Command1_1 == 0) || (Type_Command1_2 == 0)) 
        {
            return 0;
        } else
        {
            AUDIO_Send(Type_Command1_1+Language);
            Start_Count = 1; Timeout_Sound_Count = 0;
            do
            {
                Key = TSM12_key_pad_scan();
                switch (Key)
                {
                case '0':
                    Key = 0;
                    break;
                case '6':
                    Key = 0;
                    break;
                case '7':
                    Key = 0;
                    break;
                case '8':
                    Key = 0;
                    break;
                case '9':
                    Key = 0;
                    break;
                default:
                    break;
                }
            } while ((Key == 0)&&(Timeout_Sound_Count < MIN_TIME_SOUND));
            if(Key != 0) 
            {
                Start_Count = 0;
                return Key;
            }else
            {
                AUDIO_Send(Type_Command1_2+Language);
                Start_Count = 1; Timeout_Sound_Count = 0;
                do
                {
                    Key = TSM12_key_pad_scan();
                    switch (Key)
                    {
                    case '0':
                        Key = 0;
                        break;
                    case '6':
                        Key = 0;
                        break;
                    case '7':
                        Key = 0;
                        break;
                    case '8':
                        Key = 0;
                        break;
                    case '9':
                        Key = 0;
                        break;
                    default:
                        break;
                    }
                } while ((Key == 0)&&(Timeout_Sound_Count < MIN_TIME_SOUND));
                if(Key != 0)
                {
                    Start_Count = 0;
                    return Key;
                }else
                {
                    return 0;
                }
                
            }
        }
    }   
}

uint8_t AUDIO_Handle_Send_6(uint8_t Type_Command1, uint8_t Type_Command1_1, uint8_t Type_Command1_2, uint16_t Time_Sound, uint8_t Language)
{
    uint8_t Key = 0;
    AUDIO_Send(Type_Command1+Language);
    Start_Count = 1; Timeout_Sound_Count = 0;
    do
    {
        Key = TSM12_key_pad_scan();
        switch (Key)
        {
        case '0':
            Key = 0;
            break;
        case '7':
            Key = 0;
            break;
        case '8':
            Key = 0;
            break;
        case '9':
            Key = 0;
            break;
        default:
            break;
        }
    } while ((Key == 0)&&(Timeout_Sound_Count < Time_Sound));
    if(Key != 0) 
    {
        Start_Count = 0;
        return Key;
    }else
    {
        if((Type_Command1_1 == 0) || (Type_Command1_2 == 0)) 
        {
            return 0;
        } else
        {
            AUDIO_Send(Type_Command1_1+Language);
            Start_Count = 1; Timeout_Sound_Count = 0;
            do
            {
                Key = TSM12_key_pad_scan();
                switch (Key)
                {
                case '0':
                    Key = 0;
                    break;
                case '7':
                    Key = 0;
                    break;
                case '8':
                    Key = 0;
                    break;
                case '9':
                    Key = 0;
                    break;
                default:
                    break;
                }
            } while ((Key == 0)&&(Timeout_Sound_Count < MIN_TIME_SOUND));
            if(Key != 0) 
            {
                Start_Count = 0;
                return Key;
            }else
            {
                AUDIO_Send(Type_Command1_2+Language);
                Start_Count = 1; Timeout_Sound_Count = 0;
                do
                {
                    Key = TSM12_key_pad_scan();
                    switch (Key)
                    {
                    case '0':
                        Key = 0;
                        break;
                    case '7':
                        Key = 0;
                        break;
                    case '8':
                        Key = 0;
                        break;
                    case '9':
                        Key = 0;
                        break;
                    default:
                        break;
                    }
                } while ((Key == 0)&&(Timeout_Sound_Count < MIN_TIME_SOUND));
                if(Key != 0)
                {
                    Start_Count = 0;
                    return Key;
                }else
                {
                    return 0;
                }
                
            }
        }
    }   
}

uint8_t AUDIO_Handle_Send_0(uint8_t Type_Command1, uint8_t Type_Command1_1, uint8_t Type_Command1_2, uint16_t Time_Sound, uint8_t Language)
{
    uint8_t Key = 0;
    AUDIO_Send(Type_Command1+Language);
    Start_Count = 1; Timeout_Sound_Count = 0;
    do
    {
        Key = TSM12_key_pad_scan();
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
        case '8':
            Key = 0;
            break;
        case '9':
            Key = 0;
            break;
        default:
            break;
        }
    } while ((Key == 0)&&(Timeout_Sound_Count < Time_Sound));
    if(Key != 0) 
    {
        Start_Count = 0;
        return Key;
    }else
    {
        if((Type_Command1_1 == 0) || (Type_Command1_2 == 0)) 
        {
            return 0;
        } else
        {
            AUDIO_Send(Type_Command1_1+Language);
            Start_Count = 1; Timeout_Sound_Count = 0;
            do
            {
                Key = TSM12_key_pad_scan();
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
                case '8':
                    Key = 0;
                    break;
                case '9':
                    Key = 0;
                    break;
                default:
                    break;
                }
            } while ((Key == 0)&&(Timeout_Sound_Count < MIN_TIME_SOUND));
            if(Key != 0) 
            {
                Start_Count = 0;
                return Key;
            }else
            {
                AUDIO_Send(Type_Command1_2+Language);
                Start_Count = 1; Timeout_Sound_Count = 0;
                do
                {
                    Key = TSM12_key_pad_scan();
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
                    case '8':
                        Key = 0;
                        break;
                    case '9':
                        Key = 0;
                        break;
                    default:
                        break;
                    }
                } while ((Key == 0)&&(Timeout_Sound_Count < MIN_TIME_SOUND));
                if(Key != 0)
                {
                    Start_Count = 0;
                    return Key;
                }else
                {
                    return 0;
                }
                
            }
        }
    }   
}

uint8_t AUDIO_Handle_Send_0_6(uint8_t Type_Command1, uint8_t Type_Command1_1, uint8_t Type_Command1_2, uint16_t Time_Sound, uint8_t Language)
{
    uint8_t Key = 0;
    AUDIO_Send(Type_Command1+Language);
    Start_Count = 1; Timeout_Sound_Count = 0;
    do
    {
        Key = TSM12_key_pad_scan();
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
        case '7':
            Key = 0;
            break;
        case '8':
            Key = 0;
            break;
        case '9':
            Key = 0;
            break;
        default:
            break;
        }
    } while ((Key == 0)&&(Timeout_Sound_Count < Time_Sound));
    if(Key != 0) 
    {
        Start_Count = 0;
        return Key;
    }else
    {
        if((Type_Command1_1 == 0) || (Type_Command1_2 == 0)) 
        {
            return 0;
        } else
        {
            AUDIO_Send(Type_Command1_1+Language);
            Start_Count = 1; Timeout_Sound_Count = 0;
            do
            {
                Key = TSM12_key_pad_scan();
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
                case '7':
                    Key = 0;
                    break;
                case '8':
                    Key = 0;
                    break;
                case '9':
                    Key = 0;
                    break;
                default:
                    break;
                }
            } while ((Key == 0)&&(Timeout_Sound_Count < MIN_TIME_SOUND));
            if(Key != 0) 
            {
                Start_Count = 0;
                return Key;
            }else
            {
                AUDIO_Send(Type_Command1_2+Language);
                Start_Count = 1; Timeout_Sound_Count = 0;
                do
                {
                    Key = TSM12_key_pad_scan();
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
                    case '7':
                        Key = 0;
                        break;
                    case '8':
                        Key = 0;
                        break;
                    case '9':
                        Key = 0;
                        break;
                    default:
                        break;
                    }
                } while ((Key == 0)&&(Timeout_Sound_Count < MIN_TIME_SOUND));
                if(Key != 0)
                {
                    Start_Count = 0;
                    return Key;
                }else
                {
                    return 0;
                }
                
            }
        }
    }   
}


uint8_t AUDIO_Handle_Send_0_8(uint8_t Type_Command1, uint8_t Type_Command1_1, uint8_t Type_Command1_2, uint16_t Time_Sound, uint8_t Language)
{
    uint8_t Key = 0;
    AUDIO_Send(Type_Command1+Language);
    Start_Count = 1; Timeout_Sound_Count = 0;
    do
    {
        Key = TSM12_key_pad_scan();
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
    } while ((Key == 0)&&(Timeout_Sound_Count < Time_Sound));
    if(Key != 0) 
    {
        Start_Count = 0;
        return Key;
    }else
    {
        if((Type_Command1_1 == 0) || (Type_Command1_2 == 0)) 
        {
            return 0;
        } else
        {
            AUDIO_Send(Type_Command1_1+Language);
            Start_Count = 1; Timeout_Sound_Count = 0;
            do
            {
                Key = TSM12_key_pad_scan();
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
            } while ((Key == 0)&&(Timeout_Sound_Count < MIN_TIME_SOUND));
            if(Key != 0) 
            {
                Start_Count = 0;
                return Key;
            }else
            {
                AUDIO_Send(Type_Command1_2+Language);
                Start_Count = 1; Timeout_Sound_Count = 0;
                do
                {
                    Key = TSM12_key_pad_scan();
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
                } while ((Key == 0)&&(Timeout_Sound_Count < MIN_TIME_SOUND));
                if(Key != 0)
                {
                    Start_Count = 0;
                    return Key;
                }else
                {
                    return 0;
                }
                
            }
        }
    }   
}

uint8_t AUDIO_Handle_Send_0_9(uint8_t Type_Command1, uint16_t Time_Sound, uint8_t Language)
{
    uint8_t Key = 0;
    AUDIO_Send(Type_Command1+Language);
    Start_Count = 1; Timeout_Sound_Count = 0;
    do
    {
        Key = TSM12_key_pad_scan();
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
        case '#':
            Key = 0;
            break;
        default:
            break;
        }
    } while ((Key == 0)&&(Timeout_Sound_Count < Time_Sound)&&(Finger_Flag == 0));
    if(Key != 0) 
    {
        Start_Count = 0;
        return Key;
    }else
    {
        Start_Count = 0;
        return 0;
    }
}

uint8_t AUDIO_Handle_Send_0_9_0(uint8_t Type_Command1, uint16_t Time_Sound, uint8_t Language)
{
    uint8_t Key = 0;
    AUDIO_Send(Type_Command1+Language);
    Start_Count = 1; Timeout_Sound_Count = 0;
    do
    {
        Key = TSM12_key_pad_scan();
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
        case '*':
            Key = 0;
            break;
        default:
            break;
        }
    } while ((Key == 0)&&(Timeout_Sound_Count < Time_Sound));
    if(Key != 0) 
    {
        Start_Count = 0;
        return Key;
    }else
    {
        Start_Count = 0;
        return 0;
    }
}

uint8_t AUDIO_Handle_Send_Not_Press(uint8_t Type_Command1, uint8_t Type_Command1_1, uint8_t Type_Command1_2, uint16_t Time_Sound, uint8_t Language)
{
    uint8_t Key = 0;
    AUDIO_Send(Type_Command1+Language);
    Start_Count = 1; Timeout_Sound_Count = 0;
    do
    {
        Key = TSM12_key_pad_scan();
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
        case '8':
            Key = 0;
            break;
        case '9':
            Key = 0;
            break;
        case '*':
            Key = 0;
            break;
        case '#':
            Key = 0;
            break;
        default:
            break;
        }
    } while ((Key == 0)&&(Timeout_Sound_Count < Time_Sound));
    if(Key != 0) 
    {
        Start_Count = 0;
        return Key;
    }else
    {
        if((Type_Command1_1 == 0) || (Type_Command1_2 == 0)) 
        {
            return 0;
        } else
        {
            AUDIO_Send(Type_Command1_1+Language);
            Start_Count = 1; Timeout_Sound_Count = 0;
            do
            {
                Key = TSM12_key_pad_scan();
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
                case '8':
                    Key = 0;
                    break;
                case '9':
                    Key = 0;
                    break;
                case '*':
                    Key = 0;
                    break;
                case '#':
                    Key = 0;
                    break;
                default:
                    break;
                }
            } while ((Key == 0)&&(Timeout_Sound_Count < MIN_TIME_SOUND));
            if(Key != 0) 
            {
                Start_Count = 0;
                return Key;
            }else
            {
                AUDIO_Send(Type_Command1_2+Language);
                Start_Count = 1; Timeout_Sound_Count = 0;
                do
                {
                    Key = TSM12_key_pad_scan();
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
                    case '8':
                        Key = 0;
                        break;
                    case '9':
                        Key = 0;
                        break;
                    case '*':
                        Key = 0;
                        break;
                    case '#':
                        Key = 0;
                        break;
                    default:
                        break;
                    }
                } while ((Key == 0)&&(Timeout_Sound_Count < MIN_TIME_SOUND));
                if(Key != 0)
                {
                    Start_Count = 0;
                    return Key;
                }else
                {
                    return 0;
                }
                
            }
        }
    }   
}

uint8_t AUDIO_Handle_Send(uint8_t Type_Command1, uint8_t Type_Command1_1, uint8_t Type_Command1_2, uint16_t Time_Sound)
{
    uint8_t Key = 0;
    if(Check_Language == 0)
    {
        AUDIO_Send(Type_Command1);
        Start_Count = 1; Timeout_Sound_Count = 0;
        do
        {
            Key = TSM12_key_pad_polling();
        } while ((Key == 0) && (Timeout_Sound_Count < Time_Sound));
        if(Key != 0) 
        {
            Start_Count = 0;
            return Key;
        }
        else
        {
            if((Type_Command1_1 == 0) || (Type_Command1_2 == 0)) 
            {
                return 0;
            }
            else
            {
                AUDIO_Send(Type_Command1_1);
                Start_Count = 1; Timeout_Sound_Count = 0;
                do
                {
                    Key = TSM12_key_pad_polling();
                } while ((Key == 0) && (Timeout_Sound_Count < MIN_TIME_SOUND));
                if(Key != 0) 
                {
                    Start_Count = 0;
                    return Key;
                }
                else
                {
                    AUDIO_Send(Type_Command1_2);
                    Start_Count = 1; Timeout_Sound_Count = 0;
                    do
                    {
                        Key = TSM12_key_pad_polling();
                    } while ((Key == 0) && (Timeout_Sound_Count < MIN_TIME_SOUND));
                    if(Key != 0) 
                    {
                        Start_Count = 0;
                        return Key;
                    }
                    else
                    {
                        return 0;
                    }
                    
                }
            }
            
        }
    }
    else if(Check_Language == 1)
    {
        AUDIO_Send(Type_Command1+1);
        Start_Count = 1; Timeout_Sound_Count = 0;
        do
        {
            Key = TSM12_key_pad_polling();
        } while ((Key == 0) && (Timeout_Sound_Count < Time_Sound));
        if(Key != 0) 
        {
            Start_Count = 0;
            return Key;
        }
        else
        {
            if((Type_Command1_1 == 0) || (Type_Command1_2 == 0)) 
            {
                return 0;
            }
            else
            {
                AUDIO_Send(Type_Command1_1+1);
                Start_Count = 1; Timeout_Sound_Count = 0;
                do
                {
                    Key = TSM12_key_pad_polling();
                } while ((Key == 0) && (Timeout_Sound_Count < MIN_TIME_SOUND));
                if(Key != 0) 
                {
                    Start_Count = 0;
                    return Key;
                }
                else
                {
                    AUDIO_Send(Type_Command1_2+1);
                    Start_Count = 1; Timeout_Sound_Count = 0;
                    do
                    {
                        Key = TSM12_key_pad_polling();
                    } while ((Key == 0) && (Timeout_Sound_Count < MIN_TIME_SOUND));
                    if(Key != 0) 
                    {
                        Start_Count = 0;
                        return Key;
                    }
                    else
                    {
                        return 0;
                    }
                    
                }
            }
            
        }
    }
    return 0;
    
}

uint8_t AUDIO_Press_Send(uint8_t Type_Command1, uint16_t Time_Sound)
{
    AUDIO_Send(Type_Command1);
    Start_Count = 1; Timeout_Sound_Count = 0;
    do
    {
    } while ((Timeout_Sound_Count < Time_Sound));
    Start_Count = 0;
    return 0;
}


uint8_t AUDIO_Finger_Send(uint8_t Type_Command1, uint16_t Time_Sound, uint8_t Language)
{
    AUDIO_Send(Type_Command1+Language);
    Start_Count = 1; Timeout_Sound_Count = 0;
    do
    {

    } while ((Finger_Flag == 0) && (Timeout_Sound_Count < Time_Sound));
    Start_Count = 0; Timeout_Sound_Count = 0;
    return 0;
}

uint8_t AUDIO_Finger_First_Send(uint8_t Type_Command1, uint16_t Time_Sound, uint8_t Language)
{
    AUDIO_Send(Type_Command1+Language);
    Start_Count = 1; Timeout_Sound_Count = 0;
    do
    {
        Check_Key = TSM12_key_pad_polling();
    } while ((Finger_Flag == 0) && (Timeout_Sound_Count < Time_Sound)&& (Check_Key != '*'));
    Start_Count = 0;
    Timeout_Sound_Count = 0;
    if(Finger_Flag == 1)
        return 1;
    else if(Check_Key == '*')
        return 2;
    else
        return 0;
}

uint8_t AUDIO_Get_Finger_Send(uint8_t Type_Command1, uint16_t Time_Sound, uint8_t Language)
{
    AUDIO_Send(Type_Command1+Language);
    Start_Count = 1; Timeout_Sound_Count = 0;
    do
    {
    } while (Timeout_Sound_Count < Time_Sound);
    Start_Count = 0; 
    Timeout_Sound_Count = 0;
    return 0;
}

uint8_t AUDIO_Common_Send(uint8_t Type_Command1, uint16_t Time_Sound, uint8_t Language)
{
    AUDIO_Send(Type_Command1+Language);
    Start_Count = 1; Timeout_Sound_Count = 0;
    do
    {
        Check_Key = TSM12_key_pad_polling();
    } while ((Check_Key != '#')&&(Check_Key != '*')&&(Timeout_Sound_Count < Time_Sound));
    return Check_Key;
}


uint8_t AUDIO_Card_Send(uint8_t Type_Command1, uint16_t Time_Sound, uint8_t Language)
{
    AUDIO_Send(Type_Command1+Language);
    Start_Count = 1; Timeout_Sound_Count = 0; Button_Card_Flag = 0;
    do
    {
        Check_Key = TSM12_key_pad_polling();
    } while ((Button_Card_Flag == 0) && (Timeout_Sound_Count < Time_Sound));
    if(Button_Card_Flag == 1) 
    {
        Start_Count = 0;
        return Check_Key;
    }
    return 0;
}

void AUDIO_Card_User_Manager_Send(uint8_t Type_Command1, uint16_t Time_Sound, uint8_t Language)
{
    AUDIO_Send(Type_Command1+Language);
    Start_Count = 1; Timeout_Sound_Count = 0; Button_Card_Flag = 0;
    do
    {
    } while (Timeout_Sound_Count < Time_Sound);
    if(Button_Card_Flag == 1) 
    {
        Start_Count = 0;
    }
}
