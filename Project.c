#include"lcd.h"
#include"ultrasonic_sensor.h"
#include<avr/io.h>
int main(void){
	/* Distance is from 2-cm to 400-cm so uint16 as type is used */
	uint16 distance;
	/*Enable the global interrupt */
	SREG|=(1<<7);
	/*To initialize the ultrasonic sensor therefore initializing the icu module */
	Ultrasonic_init();
	LCD_init();
	LCD_displayString("Distance = ");
	LCD_moveCursor(0,14);
	LCD_displayString("cm");
	while(1){
	LCD_moveCursor(0,11);
	/*getting the distance from the ultrasonic sensor */
	distance = Ultrasonic_readDistance();
	/*to display the distance on the lcd on a specific cursor location*/
	LCD_intgerToString(distance);
	/*This condition is to display correctly on the LCD */
	if(distance < 100){
		LCD_moveCursor(0,13);
		LCD_displayCharacter(' ');
		if(distance < 10){
			LCD_moveCursor(0,12);
			LCD_displayCharacter(' ');
	}
	}
	}
}
