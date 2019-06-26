/*Begining of Auto generated code by Atmel studio */
#include <Arduino.h>
#include <math.h>
#include "ADC_Configuration.h"
#include "Step_Output_Configuration.h"
#include "Serial_Processing.h"
#include "Math_Helpers.h"
#include "Serial_Commands.h"
#include "Globals.h"
#include "EnableInterrupt.h"
//Beginning of Auto generated function prototypes by Atmel Studio
ISR(TIMER1_OVF_vect );
//End of Auto generated function prototypes by Atmel Studio


//WHEN DEBUGGING MAKE SURE TO SELECT GDB UNDER traverse->Properties->Advanced->check GDB

/*End of auto generated code by Atmel studio */

void CheckDirection(); //function prototype
void ToggleStepOutput();
void MoveAbsolutePosition(long position, int speed);
void MoveRelativePosition(long position, int speed);
void home();



const int stepPin = 3;
const int potPin = 0;
const int spoolPinInput = 6;
volatile const int directionInput = 5;
volatile const int directionOutputPin = 4;
volatile const int homingOverLimitMax = 1000;

volatile int traverseSpeed = 0;

int potValue = 0;
volatile long stepDelay = 0;
volatile bool directionInputState = false;
volatile bool directionOutputState = false;
volatile long spoolPulses = 0;
volatile long spoolTime = 0;
volatile unsigned int toneRingDivisions = 10;


uint32_t serialTimerMillis = 1000; //Serial output scheduler delay time
uint32_t serialTimerPreviousMillis = 0; //Current delay time

unsigned long totalspoolTicks = 0;



long STEPS = 0;
bool RUN = false;
bool HOME = false;
volatile int32_t DESIRED_POSITION = 0;
volatile uint32_t STEPRATE = 0;


static unsigned long last_millis = 0;
unsigned long m = millis();


#ifdef USE_POT_FOR_TRAVERSE
volatile bool potEnabled = true;
#else
volatile bool potEnabled = false;
#endif

Serial_Commands _Serial_Commands;
Math_Helpers _Math_Helpers;




ISR (TIMER1_OVF_vect){

	TCNT1 = stepDelay;

	if (DESIRED_POSITION != STEPS)
	{
		ToggleStepOutput();
	}
	
	if (DESIRED_POSITION > STEPS)
	{
		PORTD |= digitalPinToBitMask(directionOutputPin); //dir pin away from home
		STEPS++;
	}
	if (DESIRED_POSITION < STEPS)
	{
		PORTD &= ~digitalPinToBitMask(directionOutputPin); //dir pin toward home
		STEPS--;
	}
	
	
}



void spoolPulse_Vector()
{

	
	m = millis();
	if (m - last_millis < 20)
	{ // ignore interrupt: probably a bounce problem
	}
	else
	{ // do pushbutton stuff
		
		last_millis = m;



		if (spoolPulses == 0)
		{
			spoolTime = millis();
		}
		spoolPulses++;
		totalspoolTicks++;
	}
}



void setup() {
	
	pinMode(stepPin, OUTPUT);
	pinMode(directionOutputPin, OUTPUT);
	pinMode(directionInput, INPUT);
	pinMode(spoolPinInput, INPUT);

	attachPinChangeInterrupt(spoolPinInput, spoolPulse_Vector, FALLING);

	ADC_Configuration _ADC_Configuration ;
	Step_Output_Configuration _Step_Output_Configuration;

	_ADC_Configuration.SetupADC();
	_Step_Output_Configuration.SetupStepOutput();

	Serial.begin(115200);
	Serial.setTimeout(50);

	sei(); // Enable global interrupts
	
}

void loop() {
	
	_Serial_Commands.commandsProcess();

	directionInputState = PIND & 0x20; //0x40 = pin 5 (0010 0000) //read pot
	
	if (RUN){
		stepDelay = STEPRATE;
	}
	
	if (HOME)
	{
		home();
		HOME = false;
		RUN = true;
	}

	if (spoolPulses > 10)
	{
		uint16_t RPM  = 6*1000/(millis() - spoolTime)*spoolPulses;

		//long duration = millis() - spoolTime; //time in milliseconds to complete one full rev
		
		//uint32_t numberOfRevolutions = ((float)spoolPulses / (float)toneRingDivisions) * 1000.0; //divide by 1000 to get actual number of revs, using 1000 to eliminate floats
		////////uint16_t RPM = (1000 / duration) * 60;
		/////////float test = (float)((float)numberOfRevolutions / (float)duration) * (float)1000.0;
		//uint16_t RPM = ((float)numberOfRevolutions / (float)duration) * 60.0;
		//////// ((1 / duration) * 1000) * 60 floating point
		////////(1 / duration) * 60000 //floating point
		////////(1000 / duration) * 60  //doesn't use floating point
		
		spoolPulses = 0; //isr will reset spool time

		Serial.print("RPM: ");
		Serial.println(RPM);

		
	}
	
}

//rpm to stepper tick in micro seconds
//long rpmToTickInterval(long targetRPM){
//// rotation per sec = targetRPM/60
//float stepsPerSecond = (float) targetRPM/60 * MOTORSTEPS;
//long pulseInMicroseconds = (long) (1000000L/stepsPerSecond) / 2;
//
//return pulseInMicroseconds;
//}


void CheckDirection()
{
	if (directionInputState == LOW){
		PORTD &= ~digitalPinToBitMask(directionOutputPin);
	}
	else{
		PORTD |= digitalPinToBitMask(directionOutputPin);
	}

	directionOutputState = directionInputState;
	//positionCounts = 0;
	
}

void ToggleStepOutput()
{
	PORTD ^= !(PIND & 0x08) | 0x08;
}

void MoveAbsolutePosition(long position, int speed)
{
	DESIRED_POSITION = position;
	stepDelay = speed;
}

void MoveRelativePosition(long position, int speed)
{
	DESIRED_POSITION = position + DESIRED_POSITION;
	stepDelay = speed;
}

void home(void)
{
	while(directionInputState == LOW) //coarse home
	{
		directionInputState = PIND & 0x20;
		if (STEPS == DESIRED_POSITION)
		{
			MoveRelativePosition(-1, 65000);
		}
	}
	MoveRelativePosition(-1000, 65000);
	while (STEPS != DESIRED_POSITION)
	{
		;;
	}
	

	while(directionInputState == HIGH) //coarse home move off limit
	{
		directionInputState = PIND & 0x20;
		if (STEPS == DESIRED_POSITION)
		{
			MoveRelativePosition(1, 65000);
		}
	}

	MoveRelativePosition(5000, 65000);

	while (STEPS != DESIRED_POSITION)
	{
		;;
	}


	while(directionInputState == LOW) //fine home
	{
		directionInputState = PIND & 0x20;
		if (STEPS == DESIRED_POSITION)
		{
			MoveRelativePosition(-1, 58000);
		}
	}
	MoveRelativePosition(-1000, 58000);
	while (STEPS != DESIRED_POSITION)
	{
		;;
	}

	while(directionInputState == HIGH) //coarse home move off limit
	{
		directionInputState = PIND & 0x20;
		if (STEPS == DESIRED_POSITION)
		{
			MoveRelativePosition(1, 58000);
		}
	}

	MoveRelativePosition(1000, 58000);

	while (STEPS != DESIRED_POSITION)
	{
		;;
	}



	STEPS = 0;
	DESIRED_POSITION = 0;

}


