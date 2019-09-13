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

#define HOMING_SWITCH_USES_SPRING_RETURN false
#define HOMING_USES_DOUBLE_BUMP false
#define USE_POT_FOR_TRAVERSE  //uncomment to disable external speed pot

#define ADC_MIN_VALUE 0
#define ADC_MAX_VALUE 1023 //zero based

#endif //__DEVICE_CONFIGURATION_H__
