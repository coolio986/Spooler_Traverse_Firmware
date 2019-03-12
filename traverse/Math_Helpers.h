/* 
* Math_Helpers.h
*
* Created: 3/12/2019 6:05:16 PM
* Author: Anthony
*/


#ifndef __MATH_HELPERS_H__
#define __MATH_HELPERS_H__


class Math_Helpers
{
//variables
public:
protected:
private:

//functions
public:
  Math_Helpers();
  ~Math_Helpers();

  float fscale( float originalMin, float originalMax, float newBegin, float
  newEnd, float inputValue, float curve);


protected:
private:
  Math_Helpers( const Math_Helpers &c );
  Math_Helpers& operator=( const Math_Helpers &c );

}; //Math_Helpers

#endif //__MATH_HELPERS_H__
