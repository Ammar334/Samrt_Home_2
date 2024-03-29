/*******************************************************************************************/
/**************		Author 	: Ahmed Amr					************************************/
/**************		SWC 	: TIMER1					************************************/
/**************		Version : Version 1					************************************/
/**************		Date 	: Tue, August 29, 2023	************************************/
/*******************************************************************************************/



#include "TIMER1.h"
#include <avr/io.h>

static void (*Timer_ptrFunc[4])(void) = {NULL};


void Timer1_voidInit(u8 Copy_u8Mode, u8 Copy_u8Scaler, u8 Copy_u8OC1A, u8 Copy_u8OC1B)
{
    //Select Timer1 Mode
    switch (Copy_u8Mode)
    {
    case TIMER1_NORMAL_MODE:
        CLEAR_BIT(TCCR1A, WGM10);
        CLEAR_BIT(TCCR1A, WGM11);
        CLEAR_BIT(TCCR1B, WGM12);
        CLEAR_BIT(TCCR1B, WGM13);
        break;
    case TIMER1_CTC_MODE_TOP_OCR1A:
        CLEAR_BIT(TCCR1A, WGM10);
        CLEAR_BIT(TCCR1A, WGM11);
        SET_BIT(TCCR1B, WGM12);
        CLEAR_BIT(TCCR1B, WGM13);
        break;
    case TIMER1_PHASE_CORRECT_MODE_TOP_OCR1A:
        SET_BIT(TCCR1A, WGM10);
        SET_BIT(TCCR1A, WGM11);
        CLEAR_BIT(TCCR1B, WGM12);
        SET_BIT(TCCR1B, WGM13);
        break;
    case TIMER1_FAST_MODE_TOP_ICR1:
        CLEAR_BIT(TCCR1A, WGM10);
        SET_BIT(TCCR1A, WGM11);
        SET_BIT(TCCR1B, WGM12);
        SET_BIT(TCCR1B, WGM13);
        break;
    case TIMER1_FAST_MODE_TOP_OCR1A:
        SET_BIT(TCCR1A, WGM10);
        SET_BIT(TCCR1A, WGM11);
        SET_BIT(TCCR1B, WGM12);
        SET_BIT(TCCR1B, WGM13);
        break;
    
    default:
        break;
    }
  
    
    //Config Action OC1A PIN
    CLEAR_BITS_IN_REG(TCCR1A, OC1A_CLR_MASK);
    SET_BITS_IN_REG(TCCR1A, OC1A_CLR_MASK);

    //Config Action OC1B PIN
    CLEAR_BITS_IN_REG(TCCR1A, OC1B_CLR_MASK);
    SET_BITS_IN_REG(TCCR1A, Copy_u8OC1B);

    //Select Clock Source
    //Clear CS12:10
    CLEAR_BITS_IN_REG(TCCR1B, OC1B_CLR_MASK);
    //Set Clock source
    SET_BITS_IN_REG(TCCR1B, Copy_u8Scaler);
 
}

void Timer1_voidSetPreloadValue(u16 Copy_u16Value)
{
    TCNT1 = Copy_u16Value;
}

u16  Timer1_voidReadValue(void)
{
    return TCNT1;
}

void Timer1_voidSetOCR1AValue(u16 Copy_u16Value)
{
    OCR1A = Copy_u16Value;
}

void Timer1_voidSetOCR1BValue(u16 Copy_u16Value)
{
    OCR1B = Copy_u16Value;
}

void Timer1_voidOVFInterruptEnable(void)
{
    SET_BIT(TIMSK, TOIE1);
}

void Timer1_voidOVFInterruptDisable(void)
{
    CLEAR_BIT(TIMSK, TOIE1);
}

void Timer1_voidOC1AInterruptEnable(void)
{
    SET_BIT(TIMSK, OCIE1A);
}

void Timer1_voidOC1AInterruptDisable(void)
{
    CLEAR_BIT(TIMSK, OCIE1A);
}

void Timer1_voidOC1BInterruptEnable(void)
{
    SET_BIT(TIMSK, OCIE1B);
}

void Timer1_voidOC1BInterruptDisable(void)
{
    CLEAR_BIT(TIMSK, OCIE1B);
}



void Timer1_voidOVFSetCallBack(void (*Copy_pfNotification)(void))
{
    Timer_ptrFunc[0] = Copy_pfNotification;
}

void Timer1_voidOC1ASetCallBack(void (*Copy_pfNotification)(void))
{
    Timer_ptrFunc[1] = Copy_pfNotification;
}

void Timer1_voidOC1BSetCallBack(void (*Copy_pfNotification)(void))
{
    Timer_ptrFunc[2] = Copy_pfNotification;
}


void Timer1_voidICUSetCallBack(void (*Copy_pfNotification)(void))
{
    Timer_ptrFunc[3] = Copy_pfNotification;
}

void Timer1_voidICUSetTriggerEdge(u8 Copy_u8Edge)
{
    switch (Copy_u8Edge)
    {
    case ICU_FALLING_EDGE:
        CLEAR_BIT(TCCR1B, ICES1);
        break;
    case ICU_RISING_EDGE:
        SET_BIT(TCCR1B, ICES1);
        break;
    
    default:
        break;
    }
}

void Timer1_voidSetICR1Value(u16 Copy_u16Value)
{
    ICR1 = Copy_u16Value;
}

u16  Timer1_u16ReadICR1Value(void)
{
    return ICR1;
}

void Timer1_voidICUInterruptEnable(void)
{
    SET_BIT(TIMSK, TICIE1);
}

void Timer1_voidICUInterruptDisable(void)
{
    CLEAR_BIT(TIMSK, TICIE1);
}


ISR(TIMER1_OVF_vect)
{
    if(Timer_ptrFunc[0] != NULL)
    {
        Timer_ptrFunc[0]();
    }
}

ISR(TIMER1_COMPA_vect)
{
    if(Timer_ptrFunc[1] != NULL)
    {
        Timer_ptrFunc[1]();
    }
}

ISR(TIMER1_COMPB_vect)
{
    if(Timer_ptrFunc[2] != NULL)
    {
        Timer_ptrFunc[2]();
    }
}

ISR(TIMER1_CAPT_vect)
{
    if(Timer_ptrFunc[3] != NULL)
    {
        Timer_ptrFunc[3]();
    }
}
