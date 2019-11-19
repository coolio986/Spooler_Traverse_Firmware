/* 
* globals.h
*
* Created: 3/14/2019 5:26:20 PM
* Author: Anthony
*/
#include "Arduino.h"

#ifndef __GLOBALS_H__
#define __GLOBALS_H__

#define DELIMITER ";"



extern long STEPS;
extern volatile uint32_t TRAVERSE_RPM;
extern volatile int32_t DESIRED_POSITION;
extern volatile uint16_t SPOOLRPM;
extern volatile uint32_t INNER_TRAVERSE_OFFSET;
extern volatile uint32_t SPOOL_WIDTH;
extern volatile uint32_t SPOOL_OUTER_DIAMETER;
extern volatile uint32_t SPOOL_INNER_DIAMETER;
extern volatile uint32_t FILAMENT_DIAMETER;
extern volatile bool MOVE_TO_END;
extern volatile bool FILAMENT_CAPTURE;





typedef enum {
	MODE_HOME = 0,
	MODE_STOP = 1, 	 
	MODE_RUN_MANUAL= 2, 	 
	MODE_RUN_SEMI_AUTO = 3, 
	MODE_RUN_FULL_AUTO = 4, 

	 
} runModes_t;
extern volatile runModes_t RUN_MODE;


typedef enum {
	DIRECTION_IN = 0,
	DIRECTION_OUT = 1,
	
} traverseDirection_t;

typedef enum {
	BACK = 0,
	MIDDLE = 1,
	FRONT = 2,

} startPosition_t;

extern volatile traverseDirection_t TRAVERSE_DIRECTION;
extern volatile startPosition_t START_POSITION;


#endif //__GLOBALS_H__
