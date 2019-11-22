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
//ISR(TIMER1_COMPA_vect);
//End of Auto generated function prototypes by Atmel Studio


//WHEN DEBUGGING MAKE SURE TO SELECT GDB UNDER traverse->Properties->Advanced->check GDB

/*End of auto generated code by Atmel studio */

void CheckDirection(); //function prototype
void ToggleStepOutput();
void MoveAbsolutePosition(uint32_t position, uint32_t speed);
void MoveRelativePosition(long position, long speed);
void home();
void Set_Traverse_RPM(long rpm);
uint32_t StepsToMM(uint32_t steps);
uint32_t MMToSteps(uint32_t mm);



const int stepPin = 3;
const int potPin = 0;
const int spoolPinInput = 6;
volatile const int directionInput = 5;
volatile const int directionOutputPin = 4;
volatile const int homingOverLimitMax = 1000;

volatile int traverseSpeed = 0;

int potValue = 0;
volatile long traverse_rpm = 0;
volatile long stepDelay = 5000;
volatile bool directionInputState = false;
volatile bool directionOutputState = false;
volatile long rpmSpoolTicks = 0;
volatile long spoolTime = 0;



int spoolUpdateInterval = 2000;
int fullAutoUpdateInterval = 50;
long previousFullAutoTime = fullAutoUpdateInterval;
long previousSpoolTime = spoolUpdateInterval;
uint32_t serialTimerMillis = 1000; //Serial output scheduler delay time
uint32_t serialTimerPreviousMillis = 0; //Current delay time
volatile uint32_t fullAutoSpoolTicks = 0;
volatile uint32_t previousSpoolTicks = 0;
static unsigned long last_millis = 0;
unsigned long m = millis();


//*** GLOBAL DEFINES ***//
long STEPS = 0;

volatile int32_t DESIRED_POSITION = 0;
volatile uint32_t TRAVERSE_RPM = 0;
volatile uint16_t SPOOLRPM = 0;
volatile uint32_t INNER_TRAVERSE_OFFSET = 100;
volatile uint32_t SPOOL_WIDTH = 60000;
volatile uint32_t FILAMENT_DIAMETER = 1750;
volatile runModes_t RUN_MODE = MODE_STOP;
volatile traverseDirection_t TRAVERSE_DIRECTION = DIRECTION_OUT;
volatile startPosition_t START_POSITION = BACK;
volatile bool MOVE_TO_END = false;
volatile bool FILAMENT_CAPTURE = false;
volatile uint32_t SPOOL_TICKS = 0;
//*** GLOBAL DEFINES ***//



volatile bool tickFlag = false;
bool previousCaptureState = false;
startPosition_t previousStartPosition = BACK;

#ifdef USE_POT_FOR_TRAVERSE
volatile bool potEnabled = true;
#else
volatile bool potEnabled = false;
#endif

Serial_Commands _Serial_Commands;
Math_Helpers _Math_Helpers;


ISR (TIMER1_OVF_vect)
{

	TCNT1 = 65535 - stepDelay;

	if (RUN_MODE != MODE_STOP){

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
	
	
}



void spoolPulse_Vector()
{

	
	m = millis();
	if (m - last_millis < 1)
	{ // ignore interrupt: probably a bounce problem
	}
	else
	{ // do pushbutton stuff
		
		last_millis = m;



		if (rpmSpoolTicks == 0)
		{
			spoolTime = millis();
		}
		rpmSpoolTicks++;

		if (tickFlag == false && rpmSpoolTicks >= 10){tickFlag = true;}

		fullAutoSpoolTicks++;

		SPOOL_TICKS++;
		
	}
	
}



void setup() {
	//cli();

	pinMode(stepPin, OUTPUT);
	pinMode(directionOutputPin, OUTPUT);
	pinMode(directionInput, INPUT);
	pinMode(spoolPinInput, INPUT);

	attachPinChangeInterrupt(spoolPinInput, spoolPulse_Vector, CHANGE);  //rising and falling

	ADC_Configuration _ADC_Configuration ;
	Step_Output_Configuration _Step_Output_Configuration;

	_ADC_Configuration.SetupADC();
	_Step_Output_Configuration.SetupStepOutput();

	Serial.begin(115200);
	Serial.setTimeout(50);
	
	//sei(); // Enable global interrupts

	//home();
	RUN_MODE = MODE_HOME;

	
	
}

void loop() {


	_Serial_Commands.commandsProcess();

	
	directionInputState = PIND & 0x20; //0x20 = pin 5 (0010 0000) //read traverse optical switch
	
	
	if (RUN_MODE == MODE_RUN_MANUAL){
		
		if (TRAVERSE_RPM != traverse_rpm){
			Set_Traverse_RPM(TRAVERSE_RPM);
		}
	}
	
	if (RUN_MODE == MODE_RUN_SEMI_AUTO)
	{
		if (TRAVERSE_DIRECTION == DIRECTION_OUT)
		{
			MoveAbsolutePosition(MMToSteps(SPOOL_WIDTH + INNER_TRAVERSE_OFFSET), TRAVERSE_RPM);
		}
		if (TRAVERSE_DIRECTION == DIRECTION_IN)
		{
			MoveAbsolutePosition(MMToSteps(INNER_TRAVERSE_OFFSET), TRAVERSE_RPM);
		}
		
		if (STEPS == DESIRED_POSITION)
		{
			TRAVERSE_DIRECTION = TRAVERSE_DIRECTION == DIRECTION_OUT ? DIRECTION_IN : DIRECTION_OUT;
		}
		
	}

	if (RUN_MODE == MODE_RUN_FULL_AUTO)
	{
		if ((previousCaptureState != FILAMENT_CAPTURE) ||  (previousStartPosition != START_POSITION ))
		{
			if (!FILAMENT_CAPTURE)
			{
				switch (START_POSITION)
				{
					case BACK:
						TRAVERSE_DIRECTION = DIRECTION_OUT;
						MoveAbsolutePosition(MMToSteps(INNER_TRAVERSE_OFFSET ), MAX_RPM);
						break;
					case MIDDLE:
						TRAVERSE_DIRECTION = DIRECTION_IN;
						MoveAbsolutePosition(MMToSteps(abs((SPOOL_WIDTH - INNER_TRAVERSE_OFFSET) / 2)), MAX_RPM);
						break;
					case FRONT:
						TRAVERSE_DIRECTION = DIRECTION_IN;
						MoveAbsolutePosition(MMToSteps(INNER_TRAVERSE_OFFSET + SPOOL_WIDTH ), MAX_RPM);
						break;
					default:
						break;
				}
				
			}
			
			previousCaptureState = FILAMENT_CAPTURE;
		}

		if (millis() > previousFullAutoTime + fullAutoUpdateInterval && FILAMENT_CAPTURE == true)
		{
			if (!MOVE_TO_END)
			{
				if (STEPS < (int32_t)MMToSteps(INNER_TRAVERSE_OFFSET))
				{
					MoveAbsolutePosition(MMToSteps(INNER_TRAVERSE_OFFSET ), MAX_RPM);
					TRAVERSE_DIRECTION = DIRECTION_OUT;
					previousSpoolTicks = 0; //prevent rollover
					fullAutoSpoolTicks = 0; //prevent rollover
				}
				else
				{
					uint32_t filamentStepsPerTick = (MMToSteps(FILAMENT_DIAMETER) / TONE_RING_DIVISIONS) / 2;
					uint32_t spoolTickDelta = fullAutoSpoolTicks - previousSpoolTicks;

					if (TRAVERSE_DIRECTION == DIRECTION_OUT)
					{
						MoveRelativePosition((uint32_t)(spoolTickDelta * filamentStepsPerTick), SPOOLRPM * 1.75);
					}
					if (TRAVERSE_DIRECTION == DIRECTION_IN)
					{
						MoveRelativePosition((int32_t)-(spoolTickDelta * filamentStepsPerTick), SPOOLRPM * 1.75);
					}

					if (STEPS >= MMToSteps(INNER_TRAVERSE_OFFSET) + MMToSteps(SPOOL_WIDTH))
					{
						TRAVERSE_DIRECTION = DIRECTION_IN;
					}
					

					previousSpoolTicks = fullAutoSpoolTicks;
				}
			}
			else
			{
			
				MoveAbsolutePosition(MMToSteps(SPOOL_WIDTH + INNER_TRAVERSE_OFFSET), HOME_SPEED);
				while(STEPS != DESIRED_POSITION)
				{
					MoveAbsolutePosition(MMToSteps(SPOOL_WIDTH + INNER_TRAVERSE_OFFSET), HOME_SPEED);
				}
				MOVE_TO_END = false;
			}
			
			previousFullAutoTime = millis();
		}
		
	}
	
	if (RUN_MODE == MODE_HOME)
	{
		Set_Traverse_RPM(HOME_SPEED);
		home();
	}

	if (!FILAMENT_CAPTURE) { rpmSpoolTicks = 0;}

	if (tickFlag || millis() > previousSpoolTime + spoolUpdateInterval)
	{
		uint32_t millisDelta = millis() - previousSpoolTime;
		noInterrupts();
		if (tickFlag) { SPOOLRPM = 60000*rpmSpoolTicks/millisDelta/TONE_RING_DIVISIONS/2; } //2 since ISR is on rising and falling (CHANGE)
		else { SPOOLRPM = 0; }
		rpmSpoolTicks = 0; //isr will reset spool time
		tickFlag = false;
		interrupts();
		
		previousSpoolTime = millis();
		//Serial.println(SPOOLRPM);
	}
	

	if (SPOOL_TICKS >= 4294967290) {SPOOL_TICKS = 0;} //prevent rollover
	
	
}


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

void MoveAbsolutePosition(uint32_t position, uint32_t speed)
{
	Set_Traverse_RPM(speed);
	
	DESIRED_POSITION = position;
}

void MoveRelativePosition(long position, long speed)
{
	Set_Traverse_RPM(speed);

	DESIRED_POSITION = position + DESIRED_POSITION;
	
}

void home(void)
{

	DESIRED_POSITION = 0;
	while(directionInputState == LOW) //coarse home
	{
		directionInputState = PIND & 0x20;
		if (STEPS == DESIRED_POSITION)
		{
			MoveRelativePosition(-100, HOME_SPEED);
		}
	}
	MoveRelativePosition(-1000, HOME_SPEED);
	while (STEPS != DESIRED_POSITION)
	{
		;;
	}
	

	while(directionInputState == HIGH) //coarse home move off limit
	{
		directionInputState = PIND & 0x20;
		if (STEPS == DESIRED_POSITION)
		{
			MoveRelativePosition(100, HOME_SPEED);
		}
	}

	MoveRelativePosition(5000, HOME_SPEED);

	while (STEPS != DESIRED_POSITION)
	{
		;;
	}


	while(directionInputState == LOW) //fine home
	{
		directionInputState = PIND & 0x20;
		if (STEPS == DESIRED_POSITION)
		{
			MoveRelativePosition(-100, HOME_SPEED / 4);
		}
	}
	MoveRelativePosition(-1000, HOME_SPEED / 4);
	while (STEPS != DESIRED_POSITION)
	{
		;;
	}

	while(directionInputState == HIGH) //coarse home move off limit
	{
		directionInputState = PIND & 0x20;
		if (STEPS == DESIRED_POSITION)
		{
			MoveRelativePosition(100, HOME_SPEED / 4);
		}
	}

	MoveRelativePosition(1000, HOME_SPEED / 4);

	while (STEPS != DESIRED_POSITION)
	{
		;;
	}



	STEPS = 0;
	DESIRED_POSITION = 0;
	RUN_MODE = MODE_STOP;

}



uint32_t StepsToMM(uint32_t steps)
{

	uint32_t position = (uint32_t)(steps * 1000) / 2;
	position = position / MOTOR_STEPS_PER_REV;
	position = position * SCREW_PITCH_MM;
	position = (float)(position / 1000) + 1;

	return position;
}

uint32_t MMToSteps(uint32_t mm)
{
	uint32_t position = (mm * 1000) / SCREW_PITCH_MM;
	position = (position * MOTOR_STEPS_PER_REV) / 1000;
	position = position * 2;

	return position;
}



void Set_Traverse_RPM(long rpm)
{
	
	uint32_t pps = (rpm * MOTOR_STEPS_PER_REV) / 60; // pps is also the frequency, 60 is minutes to seconds
	pps = pps * 2; //* 2 to get two half period waves
	stepDelay = F_CPU / (1 * pps) - 1;
	}

	


