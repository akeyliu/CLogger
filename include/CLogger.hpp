#ifndef _CLOGGER_H
#define _CLOGGER_H

#include <string>
#include <iostream>

// Usage: MYSTR(...)
// Example: MYSTR( "My age:", iAge, "!" );  
// Requirement: 
//      1.  Compile with c++ 11;  i.e. g++ -std=c++11
//      2.  Return string for future process.
//      3.  Parameters can be any type of variable, but all should can be output to stream with operator '<<';
//		4.  Add some functions to convert for output to string if some variable can't director output to stream. 

#define MYSTR(...)          myString(__VA_ARGS__)

template<typename T>
string myString( string& strOutput,  T value )
{
    ostringstream oss;
    oss << strOutput << "[" << value << "]";
    return( oss.str() );
}

template<typename First, typename ...Rest>
string myString( string& strOutput, First first, Rest ...rest )
{
    ostringstream oss;
    oss << strOutput << "[" << first << "]";
    strOutput = oss.str();
    return( myString( strOutput, rest... ) );
}

template<typename First, typename ...Rest>
string myString( First first, Rest ...rest )
{
    string strOutput;
    return( myString( strOutput, first, rest... ) );
}

#endif