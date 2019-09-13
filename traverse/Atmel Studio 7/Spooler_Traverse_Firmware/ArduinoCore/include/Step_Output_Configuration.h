/* 
* Step_Output_Configuration.h
*
* Created: 3/12/2019 4:00:50 PM
* Author: Anthony
*/


#ifndef __STEP_OUTPUT_CONFIGURATION_H__
#define __STEP_OUTPUT_CONFIGURATION_H__


class Step_Output_Configuration
{
//variables
public:
protected:
private:

//functions
public:
  Step_Output_Configuration();
  ~Step_Output_Configuration();
  static void SetupStepOutput();
protected:
private:
  Step_Output_Configuration( const Step_Output_Configuration &c );
  Step_Output_Configuration& operator=( const Step_Output_Configuration &c );

}; //Step_Output_Configuration

#endif //__STEP_OUTPUT_CONFIGURATION_H__
