#include"ultrasonic_sensor.h"
#include"common_macros.h"
/*The ultrasonic sensor use the ICU so, we must include the ICU Library in the source file of the sensor*/
#include"icu.h"
#include"util/delay.h"
/*The ultrasonic sensor use the GPIO so, we must include the GPIO Library in the source file of the sensor*/
#include"gpio.h"
/*Variables :
 * 1-g_distance for ----> distance
 * 2-g_timeHigh for ----> the time of the edge
 * 3-g_count    for ----> count to differentiate of the edges and to change the edge select by its value
 * */
/* Static because these variables belongs to this file only and no file can change there values except this
 * file can.
 * */
static uint16 g_distance;
static uint16 g_time1;
static uint8 g_count=0;

void Ultrasonic_init(void){
	/*First : 1- we use with RISING EDGE
	 *        2- The clock is FCPU/8 as it is required
	 */
	Icu_ConfigType configtype={RISING,F_CPU_8};
	/*initialize the ICU driver by the upper structure so we will pass the structure by address
	 * */
	ICU_init(&configtype);
	/*Set the call back function
	 * why making call back function?
	 * ---> To make the module configurable we must do this because the MCAL can't be depend on the APP
	 * layer nor the HAL
	 */
	ICU_setCallBack(Ultrasonic_edgeProcessing);
	/*SET the trigger pin to be output pin*/
	GPIO_setupPinDirection(ULTRASONIC_SENSOR_TRIGGER_PORT_ID,ULTRASONIC_SENSOR_TRIGGER_PIN_ID,PIN_OUTPUT);
}


void Ultrasonic_Trigger(void){
	/* We need to make a pulse with 10 usec so we will output a logic 1
	 * then we will make a delay of 10 microsecond
	 * when we output a logic 0
	 * */
	GPIO_writePin(ULTRASONIC_SENSOR_TRIGGER_PORT_ID,ULTRASONIC_SENSOR_TRIGGER_PIN_ID,LOGIC_HIGH);
	_delay_us(10);
	GPIO_writePin(ULTRASONIC_SENSOR_TRIGGER_PORT_ID,ULTRASONIC_SENSOR_TRIGGER_PIN_ID,LOGIC_LOW);

}


uint16 Ultrasonic_readDistance(void){
	/*we must make the trigger pin pulse first so we will call the trigger function */
	Ultrasonic_Trigger();
	/*if count =2 that's mean the falling edge is detected and we can calculate the distance so, at this time
	 * we will clear the count to start again with zero and we will make the equation to calculate the
	 * distance and finally return the distance
	 * */
	if(g_count == 2){
		g_count=0;
		/*g_distance =(uint16)(((float)g_timeHigh*17000)/(1000000));*/
	    /*	g_distance =(uint16)((float)g_timeHigh/58.8);*/
		g_distance =(uint16)(((float)(g_time1)*0.017)+1);
	}
	return g_distance;
}

/*This Function is called when the interrupt is happened*/
void Ultrasonic_edgeProcessing(void){
	g_count++;
	if(g_count==1){
		/*clear the timer register to start the timer when the rising edge is detected*/
		ICU_clearTimerValue();
		/*change the edge select to the falling edge to know the time of the pulse*/
		ICU_setEdgeDetectionType(FALLING);
	}
	/*this condition will be true when the falling edge is detected*/
	else if(g_count==2){
		/*we will read the input capture register and save it inside the g_timeHigh*/
		g_time1 = ICU_getInputCaptureValue();

		/*clear the input capture register to start detect again*/
		ICU_clearTimerValue();

		/*To start detecting from the rising edge*/
		ICU_setEdgeDetectionType(RISING);
	}


}
