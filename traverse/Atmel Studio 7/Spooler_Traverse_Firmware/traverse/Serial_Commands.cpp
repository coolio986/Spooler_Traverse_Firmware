/*
* Serial_Commands.cpp
*
* Created: 3/13/2019 3:35:39 PM
* Author: Anthony
*/


#include "Serial_Commands.h"
#include <Arduino.h>
#include "Serial_Processing.h"
#include "Globals.h"

char* ConcantenateCharandInt(char *s1, uint32_t number);
char* ConcantenateCharandChar(char *s1, char *s2);

void BuildSerialOutput(char *output, int hardwareType, char *command, char *value);
void BuildSerialOutput(char *output, int hardwareType, char *command, uint16_t value);


CMD_STR(home, "");
CMD_STR(getSteps, "");
CMD_STR(traverseRPM, "");
CMD_STR(status, "");
CMD_STR(stop, "");
CMD_STR(runMode, "");
CMD_STR(moveAbsolute, "");
CMD_STR(spoolRPM, "");
CMD_STR(innerOffset, "");
CMD_STR(spoolWidth, "");
CMD_STR(filamentDiameter, "");

sCommand Cmds[] =
{
	COMMAND(home),
	COMMAND(getSteps),
	COMMAND(traverseRPM),
	COMMAND(status),
	COMMAND(stop),
	COMMAND(runMode),
	COMMAND(moveAbsolute),
	COMMAND(spoolRPM),
	COMMAND(innerOffset),
	COMMAND(spoolWidth),
	COMMAND(filamentDiameter),
	{"",0 }

};

static int home_cmd(int argc, char str[MAX_CMD_LENGTH], char arguments[MAX_CMD_LENGTH])
{

	Serial.println("Homing Please Wait...");
	RUN_MODE = MODE_HOME;

	//Serial.println(str);
	//uint32_t i;
	//if (argc>=1)
	//{
	//  i=atol(argv[0]);
	//  SysLogDebug(i);
	//}
}


static int getSteps_cmd(int argc, char str[MAX_CMD_LENGTH], char arguments[MAX_CMD_LENGTH])
{

	//Serial.println("get steps");
	Serial.println(STEPS);
	//Serial.println(str);
	//uint32_t i;
	//if (argc>=1)
	//{
	//  i=atol(argv[0]);
	//  SysLogDebug(i);
	//}
}

static int traverseRPM_cmd(int argc, char str[MAX_CMD_LENGTH], char arguments[MAX_CMD_LENGTH])
{

	if (arguments != 0){

		for (int i=0; arguments[i]!= '\0'; i++)
		{
			//Serial.println(hardwareType[i]);
			if (!isdigit(arguments[i]) != 0)
			{
				Serial.println("Invalid rpm, number is not a digit");
				return 0;
			}
		}
		uint32_t rpm = strtol(arguments, NULL, 10);
		
		


		if (rpm >= 0  && rpm <= MAX_RPM) {TRAVERSE_RPM = rpm;}
		else{TRAVERSE_RPM = MAX_RPM;}
	}
	else
	{
		char output[MAX_CMD_LENGTH] = {0};
		BuildSerialOutput(output, MYHARDWARETYPE, str, TRAVERSE_RPM);
		Serial.println(output);
	}
}

static int status_cmd(int argc, char str[MAX_CMD_LENGTH], char arguments[MAX_CMD_LENGTH])
{
	Serial.print("Motion Controller: ");
	if (RUN_MODE == MODE_RUN_SEMI_AUTO)
	{
		Serial.println("Running in Semi Auto");
	}
	if (RUN_MODE == MODE_RUN_FULL_AUTO)
	{
		Serial.println("Running in Full Auto");
	}
	if (RUN_MODE == MODE_RUN_MANUAL)
	{
		Serial.println("Running in Manual");
	}
	if (RUN_MODE == MODE_HOME)
	{
		Serial.println("Homing...");
	}
	if (RUN_MODE == MODE_STOP)
	{
		Serial.println("Planner Stopped...");
	}
	
}

static int stop_cmd(int argc, char str[MAX_CMD_LENGTH], char arguments[MAX_CMD_LENGTH])
{
	RUN_MODE = MODE_STOP;
}

static int runMode_cmd(int argc, char str[MAX_CMD_LENGTH], char arguments[MAX_CMD_LENGTH])
{
	if (arguments != 0)
	{
		for (int i=0; arguments[i]!= '\0'; i++)
		{
			//Serial.println(hardwareType[i]);
			if (!isdigit(arguments[i]) != 0)
			{
				Serial.println("Invalid run mode, number is not a digit");
				return 0;
			}
		}
		//int32_t desired_position = (strtol(arguments, NULL, 10) * 2);
		uint16_t mode = (strtol(arguments, NULL, 10));
		if (mode <= 4)
		{
			RUN_MODE = (runModes_t)mode;
		}
	}
	else
	{
		char output[MAX_CMD_LENGTH] = {0};
		BuildSerialOutput(output, MYHARDWARETYPE, str, RUN_MODE);
		Serial.println(output);
	}

	
}

static int moveAbsolute_cmd(int argc, char str[MAX_CMD_LENGTH], char arguments[MAX_CMD_LENGTH])
{
	if (arguments != 0)
	{
		for (int i=0; arguments[i]!= '\0'; i++)
		{
			//Serial.println(hardwareType[i]);
			if (!isdigit(arguments[i]) != 0)
			{
				Serial.println("Invalid position, number is not a digit");
				return 0;
			}
		}
		//int32_t desired_position = (strtol(arguments, NULL, 10) * 2);
		int32_t desired_position = (strtol(arguments, NULL, 10));
		desired_position = (desired_position * 1000) / SCREW_PITCH_MM;
		

		if (desired_position >= 0 && desired_position <= MAX_TRAVERSE_DISTANCE){DESIRED_POSITION = desired_position;}

		DESIRED_POSITION = (DESIRED_POSITION * MOTOR_STEPS_PER_REV) / 1000;
		

		DESIRED_POSITION = DESIRED_POSITION * 2;
		//DESIRED_POSITION = (strtol(arguments, NULL, 10) * 2);
	}

}

static int spoolRPM_cmd(int argc, char str[MAX_CMD_LENGTH], char arguments[MAX_CMD_LENGTH])
{
	char output[MAX_CMD_LENGTH] = {0};
	BuildSerialOutput(output, MYHARDWARETYPE, str, SPOOLRPM);
	Serial.println(output);
}

static int innerOffset_cmd(int argc, char str[MAX_CMD_LENGTH], char arguments[MAX_CMD_LENGTH])
{
	if (arguments != 0)
	{
		for (int i=0; arguments[i]!= '\0'; i++)
		{
			//Serial.println(hardwareType[i]);
			if (!isdigit(arguments[i]) != 0)
			{
				Serial.println("Invalid offset, number is not a digit");
				return 0;
			}
		}
		INNER_TRAVERSE_OFFSET = strtol(arguments, NULL, 10);
	}
	else
	{
		char output[MAX_CMD_LENGTH] = {0};
		BuildSerialOutput(output, MYHARDWARETYPE, str, INNER_TRAVERSE_OFFSET);
		Serial.println(output);
	}
}

static int spoolWidth_cmd(int argc, char str[MAX_CMD_LENGTH], char arguments[MAX_CMD_LENGTH])
{
	if (arguments != 0)
	{
		for (int i=0; arguments[i]!= '\0'; i++)
		{
			//Serial.println(hardwareType[i]);
			if (!isdigit(arguments[i]) != 0)
			{
				Serial.println("Invalid width, number is not a digit");
				return 0;
			}
		}
		SPOOL_WIDTH = strtol(arguments, NULL, 10);
	}
	else
	{
		char output[MAX_CMD_LENGTH] = {0};
		BuildSerialOutput(output, MYHARDWARETYPE, str, SPOOL_WIDTH);
		Serial.println(output);
	}
}

static int filamentDiameter_cmd(int argc, char str[MAX_CMD_LENGTH], char arguments[MAX_CMD_LENGTH])
{
	if (arguments != 0)
	{
		for (int i=0; arguments[i]!= '\0'; i++)
		{
			//Serial.println(hardwareType[i]);
			if (!isdigit(arguments[i]) != 0)
			{
				Serial.println("Invalid width, number is not a digit");
				return 0;
			}
		}
		FILAMENT_DIAMETER = strtol(arguments, NULL, 10);
	}
	else
	{
		char output[MAX_CMD_LENGTH] = {0};
		BuildSerialOutput(output, MYHARDWARETYPE, str, FILAMENT_DIAMETER);
		Serial.println(output);
	}
}


int Serial_Commands::commandsProcess(void)
{

	Serial_Processing _Serial_Processing;

	_Serial_Processing.CommandsProcess(Cmds);

	
	
}

char* ConcantenateCharandInt(char *s1, uint32_t number)
{

	char *output;
	output = new char[MAX_CMD_LENGTH + 1];
	output = s1;
	

	sprintf(output + strlen(output),"%d", number);

	return output;
}

void BuildSerialOutput(char *output, int hardwareType, char *command, char *value)
{

	//char *output = {0};
	//output = new char[MAX_CMD_LENGTH + 1];
	
	sprintf(output, "%d;%s;%s", hardwareType, command, value);

	//return output;
}

void BuildSerialOutput(char *output, int hardwareType, char *command, uint16_t value)
{

	//char *output = {0};
	//output = new char[MAX_CMD_LENGTH + 1];
	
	sprintf(output, "%d;%s;%d", hardwareType, command, value);

	//return output;
}


char* ConcantenateCharandChar(char *s1, char *s2)
{
	char *output;
	output = new char[MAX_CMD_LENGTH + 1];
	output = s1;

	sprintf(output + strlen(output),"%s", s2);

	return output;

}



// default constructor
Serial_Commands::Serial_Commands()
{
} //Serial_Commands

// default destructor
Serial_Commands::~Serial_Commands()
{
} //~Serial_Commands
