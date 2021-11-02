#ifndef ULTRASONIC_SENSOR_H_
#define ULTRASONIC_SENSOR_H_

#include"std_types.h"

#define ULTRASONIC_SENSOR_TRIGGER_PORT_ID   PORTB_ID
#define ULTRASONIC_SENSOR_TRIGGER_PIN_ID    PIN5_ID

/*
 * Description :
 *    function responsible for INITIALIZING the ultrasonic sensor and by initialize the sensor we will
 *    initialize the ICU
 */
void Ultrasonic_init(void);
/*
 * Description :
 *    function responsible for making the trigger pulse to start the ultra-sound pulses with 40KHz
 */
void Ultrasonic_Trigger(void);
/*
 * Description :
 *    function responsible for calculating the distance and return it
 */
uint16 Ultrasonic_readDistance(void);
/*
 * Description :
 *    function responsible for making our task which will be called in the ISR and this code can't be inside
 *    the ISR CODE to make the driver configurable
 */
void Ultrasonic_edgeProcessing(void);

#endif /* ULTRASONIC_SENSOR_H_ */
