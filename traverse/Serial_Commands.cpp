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


CMD_STR(home, "");
CMD_STR(getSteps, "");
CMD_STR(stepRate, "");
CMD_STR(status, "");
CMD_STR(stop, "");
CMD_STR(run, "");
CMD_STR(moveAbsolute, "");

sCommand Cmds[] =
{
	COMMAND(home),
	COMMAND(getSteps),
	COMMAND(stepRate),
	COMMAND(status),
	COMMAND(stop),
	COMMAND(run),
	COMMAND(moveAbsolute),
	{"",0 }

};

static int home_cmd(int argc, char str[MAX_CMD_LENGTH], char arguments[MAX_CMD_LENGTH])
{

	Serial.println("Homing Please Wait...");
	RUN = false;
	HOME = true;

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

static int stepRate_cmd(int argc, char str[MAX_CMD_LENGTH], char arguments[MAX_CMD_LENGTH])
{

	if (arguments != 0){

		for (int i=0; arguments[i]!= '\0'; i++)
		{
			//Serial.println(hardwareType[i]);
			if (!isdigit(arguments[i]) != 0)
			{
				Serial.println("Invalid Steprate, number is not a digit");
				return 0;
			}
		}
		uint32_t step_rate = strtol(arguments, NULL, 10);

		if (step_rate >= 0  && step_rate <= MAX_STEP_RATE) {STEPRATE = step_rate;}
		else{STEPRATE = MAX_STEP_RATE;}
	}
	else
	{
		char output[MAX_CMD_LENGTH] = {"stepRate: "};
		
		//sprintf(output + strlen(output),"%d", STEPRATE);

		Serial.println(ConcantenateCharandInt(output, STEPRATE));
		
	}


	
	
	//Serial.println("get steps");
	//Serial.println(STEPS);
	//Serial.println(str);
	//uint32_t i;
	//if (argc>=1)
	//{
	//  i=atol(argv[0]);
	//  SysLogDebug(i);
	//}
}

static int status_cmd(int argc, char str[MAX_CMD_LENGTH], char arguments[MAX_CMD_LENGTH])
{
	Serial.print("Motion Controller: ");
	if (RUN)
	{
		Serial.println("Running");
	}
	else
	{
		Serial.println("Stopped");
	}
	
}

static int stop_cmd(int argc, char str[MAX_CMD_LENGTH], char arguments[MAX_CMD_LENGTH])
{
	RUN =false;
}

static int run_cmd(int argc, char str[MAX_CMD_LENGTH], char arguments[MAX_CMD_LENGTH])
{
	RUN =true;
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
		int32_t desired_position = (strtol(arguments, NULL, 10) * 2);

		if (desired_position >= 0 && desired_position <= MAX_TRAVERSE_DISTANCE){DESIRED_POSITION = desired_position;}
		
		//DESIRED_POSITION = (strtol(arguments, NULL, 10) * 2);
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



// default constructor
Serial_Commands::Serial_Commands()
{
} //Serial_Commands

// default destructor
Serial_Commands::~Serial_Commands()
{
} //~Serial_Commands
