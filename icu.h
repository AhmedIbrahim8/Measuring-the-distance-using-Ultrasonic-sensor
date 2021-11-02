
#ifndef ICU_H_
#define ICU_H_
#include"std_types.h"
typedef enum{
	FALLING,RISING
}ICU_edgeType;

typedef enum{
	NO_CLOCK,F_CPU_CLOCK,F_CPU_8,F_CPU_64,F_CPU_256,F_CPU_1024,EXT_CLOCK_FALLING,EXT_CLOCK_RISING
}ICU_clock;

typedef struct{
	ICU_edgeType edge;
	ICU_clock clock;
}Icu_ConfigType;

/*
 * Description :
 *    function responsible for putting the address of the code that will implement if the Interrupt happens
 *    in the global variable.. to set the call back function
 *    This function receives : address of a function so we need to put it inside pointer to function
 */
void ICU_setCallBack(void (*a_ptr)(void));
/*
 * Description :
 *    function responsible for initializing the ICU module
 */
void ICU_init(const Icu_ConfigType *ConfigType);
/*
 * Description :
 *    function responsible for clearing the timer register
 */
void ICU_clearTimerValue(void);
/*
 * Description :
 *    function responsible for adjusting the edge type (falling or rising)
 */
void ICU_setEdgeDetectionType(const ICU_edgeType edge);
/*
 * Description :
 *    function responsible for de-initializing the init fucntion
 *    why ?
 *     --> to set a new initialization of the ICU module with a new structure
 */
void ICU_DeInit(void);
/*
 * Description :
 *    function responsible for reading the input capture register to get the time
 */
uint16 ICU_getInputCaptureValue(void);
#endif /* ICU_H_ */
