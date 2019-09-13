/*
* Device_Configuration.h
*
* Created: 3/12/2019 5:42:01 PM
* Author: Anthony
*/


#ifndef __DEVICE_CONFIGURATION_H__
#define __DEVICE_CONFIGURATION_H__

#include "HardwareTypes.h"

#define MYHARDWARETYPE TRAVERSE

#define MAX_CMD_LENGTH 60

#define HOMING_SWITCH_USES_SPRING_RETURN true
#define HOMING_USES_DOUBLE_BUMP true
//#define USE_POT_FOR_TRAVERSE  //uncomment to disable external speed pot

#define ADC_MIN_VALUE 0
#define ADC_MAX_VALUE 1023 //zero based
#define MAX_RPM 600 //600 is the max the timer can tolerate;
#define MOTOR_STEPS_PER_REV  3200 //16 microsteps (200 * 16)
#define MAX_TRAVERSE_DISTANCE 74437 //Divide by 1000 to get 74.437 mm
#define SCREW_PITCH_MM 1588 //Divide by 1000 to get 1.588
#define TONE_RING_DIVISIONS 50
#define HOME_SPEED 600

#endif //__DEVICE_CONFIGURATION_H__
