/****************************************************************************
**
** Copyright (C) 2008-2018 C.B. Barber. All rights reserved.
** $Id: //main/2005/thid/src/road/RoadError.h#22 $$Change: 2464 $
** $Date: 2018/08/27 $$Author: bbarber $
**
****************************************************************************/

#ifndef RoadError_H
#define RoadError_H

// Do not use thesa.h to avoid circular references
#include "road/RoadLogEvent.h"

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

namespace Thesa {

#//!\name Defined here
    //! RoadError -- Report and log errors
    //!  See discussion in Saylan, G., "Practical C++ error handling in hybrid environments," Dr. Dobb's Journal, p. 50-55, March 2007.
    //!   He uses an auto_ptr to track a stringstream.  It constructs a string on the fly.  RoadError uses the copy constructor to transform RoadLogEvent into a string
    class RoadError;

class RoadError : public std::exception {
#//!\name Defined here
public:
    enum ErrorSetting;
    class Setting;

enum ErrorSetting : int {
    None=               0,
    ErrorExpected=      1,
    ErrorNotExpected=   2,  //! Errors expected and ignored by trapError()
};

class Setting {
private:
    ErrorSetting        previous_setting;
public:

                        Setting(ErrorSetting i) : previous_setting(s_error_setting) { s_error_setting= i; }
                        ~Setting() { s_error_setting= previous_setting; }
};//class Setting

#//!\name Fields
private:
    RoadLogEvent        log_event;   //! Format string w/ arguments
                                     //! printf codes %d and %X mean int/int2
                                     //! %s means ptr to C-string or ArgString (count+bytes) 

#//Class fields
    static std::ostringstream  global_log; //! May be replaced with any ostream object
    static ErrorSetting s_error_setting; 
public:
    static const char  *ROADtag; //!<\todo Change to a function?

#//!\name Constructors, etc.
    RoadError();
    RoadError(const RoadError &other);  //! Called on throw, sets event_message?
    RoadError(int code, const std::string &message);
    RoadError(int code, const char *fmt);
    RoadError(int code, const char *fmt, int d);
    RoadError(int code, const char *fmt, int d, int d2);
    RoadError(int code, const char *fmt, int d, int d2, RoadArg64 arg);
    RoadError(int code, const char *fmt, int d, int d2, RoadArg64 arg, RoadArg64 arg2);
    RoadError(int code, ThFlagsInt eventFlags, const char *fmt);
    RoadError(int code, ThFlagsInt eventFlags, const char *fmt, int d);
    RoadError(int code, ThFlagsInt eventFlags, const char *fmt, int d, int d2);
    RoadError(int code, ThFlagsInt eventFlags, const char *fmt, int d, int d2, RoadArg64 arg);
    RoadError(int code, ThFlagsInt eventFlags, const char *fmt, int d, int d2, RoadArg64 arg, RoadArg64 arg2);
    
    RoadError          &operator=(const RoadError &other);
                       ~RoadError() throw() { }

#//!\name GetSet
    bool                isDefined() const { return log_event.isInitialized(); }
    int                 errorCode() const { return log_event.eventId(); };
   //!\todo QH11021 should RoadError provide errorMessage.  Currently \ref what
    RoadLogEvent        roadLogEvent() const { return log_event; };

#//!\name Virtual methods
    virtual const char *what() const throw();

#//!\name Methods
    void                logError() const;
    void                trapError(const char *fmt);

#//Class functions
    static void         clearGlobalLog() { global_log.seekp(0); }
    static bool         emptyGlobalLog() { return global_log.tellp()<=0; }
    static int          errorCount();
    static const char  *stringGlobalLog() { return global_log.str().c_str(); }

};//class RoadError

}//namespace Thesa

#//!\name Global functions

inline std::ostream    &operator<<(std::ostream &os, const Thesa::RoadError &e) { return os << e.what(); }

#endif RoadError_H
