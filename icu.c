#include<avr/interrupt.h>
#include"icu.h"
#include<avr/io.h>
/*
 * Static   because : it is for the ICU function only and no one can use it but icu.c can
 * Volatile because : it is used in function and the ISR
 * NULL_PTR because : it is just an initialization
 * */
static volatile void (*g_callBackPtr)(void)=NULL_PTR;

ISR(TIMER1_CAPT_vect){
	/*This check is important and without it the code will crash in the run time if we didn't initialize the g_callBackPtr
	 * */
	if( g_callBackPtr != NULL_PTR){
		(*g_callBackPtr)();
	}
}

void ICU_setCallBack(void (*a_ptr)(void)){
	/*a_ptr is local variable so it will be deleted after quiting the function and we need to save the value of function address
	 * and that can be done by putting its value inside global pointer to function
	 * */
	g_callBackPtr=a_ptr;
}

void ICU_init(const Icu_ConfigType *ConfigType){
	/*To make the input capture pin an input pin*/
	DDRD&=~(1<<PD6);
    /*  1 -We are non-pwm mode so the FOC1A & FOC1B must be both 1
	*   2 -The instruction below will choose the Normal mode operation because of the ORING operation so
	*   the WGM11 AND WGM10 will be zero
	*   2*-why normal mode ?
	*   --> normal mode is more logically than the compare mode because we can measure more time with the
	*    normal mode than the compare mode .. the goal is to measure the time not to make overflow
	*    EXAMPLE : if we want to detect a signal with frequency of 500 Hz.. that means a time of 1/500 which
	*    is 2 milliseconds.. if the F_CPU = 1 MHz so the Ttimer is = 1 microseconds
	*    that mean the timer should count to 2000 counts and that can be done by timer 1 because it is a
	*    16-bit timer so, if we made it by the compare mode we should adjust the compare value each time the
	*    signal frequency changes.
	*/
	TCCR1A = (1<<FOC1A)|(1<<FOC1B);
	/*(ConfigType->edge<<ICES1) to select the edge type based on the structure given and shift it to put the
	 * value on the specified bit of the edge type.
	 * (ConfigType->clock) to select the clock based on the structure given
	 * */
	TCCR1B = (TCCR1B & 0xBF) | ((ConfigType->edge)<<ICES1);
	TCCR1B = (TCCR1B & 0xF8) | (ConfigType->clock);
	/*CLEAR THE TCNT1 AT THE FIRST TIME*/
	TCNT1=0;
	/*CLEAR THE ICR1 TOO IN THE FIRST TIME */
	ICR1=0;
	/*TO enable the module interrupt of the input capture unit so when the I-BIT is 1 and input capture flag
	 * is 1 the interrupt will happen and implement the ISR CODE*/
	TIMSK|=(1<<TICIE1);
}

void ICU_clearTimerValue(void){
	/*clearing the timer register*/
	TCNT1=0;
}

void ICU_setEdgeDetectionType(const ICU_edgeType a_edge){
	/*to adjust the edge type */
	TCCR1B = (TCCR1B & 0xBF) | (a_edge<<ICES1);
}

uint16 ICU_getInputCaptureValue(void){
	/*reading the input capture register*/
	return ICR1;
}

void ICU_DeInit(void)
{
	/*de-initializing the ICU init*/
	TCCR1A = 0;
	TCCR1B = 0;
	TCNT1 = 0;
	ICR1 = 0;
	TIMSK &= ~(1<<TICIE1);
}
