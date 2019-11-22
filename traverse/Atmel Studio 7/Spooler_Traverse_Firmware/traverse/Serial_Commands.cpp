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
void BuildSerialOutput(char *output, int hardwareType, char *command, uint32_t value);


CMD_STR(Home, "");
CMD_STR(GetSteps, "");
CMD_STR(TraverseRPM, "");
CMD_STR(Status, "");
CMD_STR(Stop, "");
CMD_STR(RunMode, "");
CMD_STR(MoveAbsolute, "");
CMD_STR(SpoolRPM, "");
CMD_STR(InnerOffset, "");
CMD_STR(SpoolWidth, "");
CMD_STR(FilamentDiameter, "");
CMD_STR(MoveToEnd, "");
CMD_STR(StartPosition, "");
CMD_STR(FilamentCapture, "");
CMD_STR(SpoolTicks, "");

sCommand Cmds[] =
{
	COMMAND(Home),
	COMMAND(GetSteps),
	COMMAND(TraverseRPM),
	COMMAND(Status),
	COMMAND(Stop),
	COMMAND(RunMode),
	COMMAND(MoveAbsolute),
	COMMAND(SpoolRPM),
	COMMAND(InnerOffset),
	COMMAND(SpoolWidth),
	COMMAND(FilamentDiameter),
	COMMAND(MoveToEnd),
	COMMAND(StartPosition),
	COMMAND(FilamentCapture),
	COMMAND(SpoolTicks),
	{"",0 }

};

static int Home_cmd(int argc, char str[MAX_CMD_LENGTH], char arguments[MAX_CMD_LENGTH])
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


static int GetSteps_cmd(int argc, char str[MAX_CMD_LENGTH], char arguments[MAX_CMD_LENGTH])
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

static int TraverseRPM_cmd(int argc, char str[MAX_CMD_LENGTH], char arguments[MAX_CMD_LENGTH])
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

static int Status_cmd(int argc, char str[MAX_CMD_LENGTH], char arguments[MAX_CMD_LENGTH])
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

static int Stop_cmd(int argc, char str[MAX_CMD_LENGTH], char arguments[MAX_CMD_LENGTH])
{
	RUN_MODE = MODE_STOP;
}

static int RunMode_cmd(int argc, char str[MAX_CMD_LENGTH], char arguments[MAX_CMD_LENGTH])
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

static int MoveAbsolute_cmd(int argc, char str[MAX_CMD_LENGTH], char arguments[MAX_CMD_LENGTH])
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

static int SpoolRPM_cmd(int argc, char str[MAX_CMD_LENGTH], char arguments[MAX_CMD_LENGTH])
{
	char output[MAX_CMD_LENGTH] = {0};
	BuildSerialOutput(output, MYHARDWARETYPE, str, SPOOLRPM);
	Serial.println(output);
}

static int InnerOffset_cmd(int argc, char str[MAX_CMD_LENGTH], char arguments[MAX_CMD_LENGTH])
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

static int SpoolWidth_cmd(int argc, char str[MAX_CMD_LENGTH], char arguments[MAX_CMD_LENGTH])
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

static int FilamentDiameter_cmd(int argc, char str[MAX_CMD_LENGTH], char arguments[MAX_CMD_LENGTH])
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

static int MoveToEnd_cmd(int argc, char str[MAX_CMD_LENGTH], char arguments[MAX_CMD_LENGTH])
{
	
	if (RUN_MODE == MODE_RUN_FULL_AUTO || RUN_MODE == MODE_RUN_SEMI_AUTO)
	{
		//uint32_t desired_position = ((INNER_TRAVERSE_OFFSET + SPOOL_WIDTH) * 1000) / SCREW_PITCH_MM;
		//desired_position = (desired_position * MOTOR_STEPS_PER_REV) / 1000;
		//desired_position = desired_position * 2;
		//
		//DESIRED_POSITION = desired_position;
		MOVE_TO_END = true;
	}
	

}

static int StartPosition_cmd(int argc, char str[MAX_CMD_LENGTH], char arguments[MAX_CMD_LENGTH])
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
		uint16_t startposition = (strtol(arguments, NULL, 10));
		if (startposition <= 3)
		{
			START_POSITION = (startPosition_t)startposition;
		}
	}
	else
	{
		char output[MAX_CMD_LENGTH] = {0};
		BuildSerialOutput(output, MYHARDWARETYPE, str, START_POSITION);
		Serial.println(output);
	}
}

static int FilamentCapture_cmd(int argc, char str[MAX_CMD_LENGTH], char arguments[MAX_CMD_LENGTH])
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
		uint16_t captureState = (strtol(arguments, NULL, 10));
		FILAMENT_CAPTURE = captureState == 1 ? true : false;
		if (FILAMENT_CAPTURE) {SPOOL_TICKS = 0;}

	}
	else
	{
		char output[MAX_CMD_LENGTH] = {0};
		BuildSerialOutput(output, MYHARDWARETYPE, str, FILAMENT_CAPTURE);
		Serial.println(output);
	}
}

static int SpoolTicks_cmd(int argc, char str[MAX_CMD_LENGTH], char arguments[MAX_CMD_LENGTH])
{
	char output[MAX_CMD_LENGTH] = {0};
	BuildSerialOutput(output, MYHARDWARETYPE, str, SPOOL_TICKS);
	Serial.println(output);
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

void BuildSerialOutput(char *output, int hardwareType, char *command, uint32_t value)
{

	//char *output = {0};
	//output = new char[MAX_CMD_LENGTH + 1];
	
	sprintf(output, "%d;%s;%lu", hardwareType, command, value);

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


