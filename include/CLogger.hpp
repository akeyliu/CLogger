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


// Usage: TRACELOG(...)/DEBUGLOG(...)/INFOLOG(...)/WARNLOG()/ERRORLOG(...)/FATALLOG(...)
// Example: WARNLOG( "Warn for check condition:", iAge, ">", iStandardAge, "!" );  
// Requirement: 
//      1.  #define USE_LOG before using boost::log and this CLogger Function.
//      2.  include boost library, suggest > 1.58
//      3.  include boost head files.
/*
#include <boost/log/core.hpp>
#include <boost/log/core/record_view.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/expressions/attr_fwd.hpp>
#include <boost/log/expressions/attr.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/severity_feature.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/formatter_parser.hpp>
#include <boost/log/attributes/named_scope.hpp>
#include <boost/log/attributes/scoped_attribute.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/detail/format.hpp>
#include <boost/log/detail/thread_id.hpp>
*/
//		4.  Add follow code in your source code within main(), which define the gLogger.
/*
//Define for Global DEBUG variable.
#ifndef _LOGGER_INIT
#define _LOGGER_INIT
log::sources::severity_logger< log::trivial::severity_level > gLogger;
#endif
*/
//		5.  Add follow code in your source code within main(), which intialize the Logger referenced by the follow macro code.
/*
        INIT_LOG(filedir,filename,filesize,maxsize,maxfile,flag);
        SET_LOG_LEVEL(level);
*/
//		6.  Using TRACELOG(...)/DEBUGLOG(...)/INFOLOG(...)/WARNLOG()/ERRORLOG(...)/FATALLOG(...) to your code for output the log info.
//		7.  Remmber TRACELOG() only for DEBUG, and all variable should support output to ostream.

#ifdef USE_LOG

#ifndef LOG_DEFINE
#define LOG_DEFINE

using namespace boost;

extern log::sources::severity_logger< log::trivial::severity_level > gLogger;

//Define Boost Log Attribute of Source File Name which is used for filter.
//BOOST_LOG_ATTRIBUTE_KEYWORD(source_file_name, "SourceFileName", string);

// filedir:  LOG file directory.
// filename: LOG and ERROR file Name without extension.
// filesize: LOG and ERROR file size, unit of MB.
// maxsize:  LOG and ERROR max total size, unit of MB.
// maxfile:  LOG and ERROR max file number, unit of file.
//  Usage: Since 1.61          log::keywords::max_files        = maxfile,                                  \
// flag:     file flush flag, true/false
#define INIT_LOG(filedir,filename,filesize,maxsize,maxfile,flag)                    \
{                                                                                   \
    string strTmpLog;                                                               \
    strTmpLog  = filedir;                                                           \
    strTmpLog += filename;                                                          \
    strTmpLog += "_%N.log";                                                         \
                                                                                    \
    log::add_file_log(                                                              \
        log::keywords::file_name        = strTmpLog,                                \
        log::keywords::target           = filedir,                                  \
        log::keywords::open_mode        = ios::app,                                 \
        log::keywords::rotation_size    = filesize * 1024 * 1024,                   \
        log::keywords::max_size         = maxsize * 1024 * 1024,                    \
        log::keywords::auto_flush       = flag,                                     \
        log::keywords::scan_method      = log::sinks::file::scan_matching,          \
        log::keywords::format = (                                                   \
                log::expressions::stream                                            \
                << "[" << log::expressions::format_date_time< posix_time::ptime >   \
                        ("TimeStamp", "%Y%m%d%H%M%S")                               \
                << "][" << log::expressions::attr<log::aux::thread::id>("ThreadID") \
                << "][" << log::expressions::attr<unsigned int>("LineID")           \
                << "][" << log::trivial::severity                                   \
                << "]" << log::expressions::smessage                                \
			)                                                                       \
        );                                                                          \
    log::add_common_attributes();                                                   \
}

#define MYLOG(level, ...)   myLog(log::trivial::level, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define MYSTR(...)          myString(__VA_ARGS__)

#ifdef DEBUG
#define TRACELOG(...)       MYLOG(trace,  __VA_ARGS__)
#else
#define TRACELOG(...)
#endif
#define DEBUGLOG(...)       MYLOG(debug,  __VA_ARGS__)
#define INFOLOG(...)        MYLOG(info,   __VA_ARGS__)
#define WARNLOG(...)        MYLOG(warning,__VA_ARGS__)
#define ERRORLOG(...)       MYLOG(error,  __VA_ARGS__)
#define FATALLOG(...)       MYLOG(fatal,  __VA_ARGS__)

#define MYFILTER(level)    { log::core::get()->set_filter( log::trivial::severity >= log::trivial::level ); }

#define SET_LOG_LEVEL(level)  {                                     \
    if(      level == string("FATAL") )     { MYFILTER(fatal); }    \
    else if( level == string("ERROR") )     { MYFILTER(error); }    \
    else if( level == string("WARNING") )   { MYFILTER(warning); }  \
    else if( level == string("INFO") )      { MYFILTER(info); }     \
    else if( level == string("DEBUG") )     { MYFILTER(debug); }    \
    else if( level == string("TRACE") )     { MYFILTER(trace); }    \
    else                                    { MYFILTER(debug); }    \
}

#endif // LOG_DEFINE

#endif // USE_LOG

#endif //_CLOGGER_H