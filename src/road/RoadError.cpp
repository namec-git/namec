/****************************************************************************
**
** Copyright (C) 2008-2018 C.B. Barber. All rights reserved.
** $Id: //main/2005/thid/src/road/RoadError.cpp#24 $$Change: 2464 $
** $Date: 2018/08/27 $$Author: bbarber $
**
****************************************************************************/

#//! RoadError -- All exceptions thrown by Thesa are RoadErrors
#//! Do not throw RoadError's from destructors.  Use e.logError() instead.

#include "road/RoadError.h"

#include "road/RoadEvent.h"

using std::endl;

namespace Thesa {

#//Class fields

//! Identifies error messages from Qhull and Road for web searches.
//! See QhullError.h#QHULLlastError and user.h#MSG_ERROR
const char * RoadError::
ROADtag= "TH";  //!\todo -- As a virtual function or somehow inherited?

RoadError::ErrorSetting RoadError::
s_error_setting= RoadError::None;

std::ostringstream RoadError::
global_log;

#//!\name Constructors, etc.

RoadError::
RoadError()
: log_event()
{ }

RoadError::
RoadError(int code, const std::string &message)
: log_event(code, RoadEvent::ErrorEvent)
{
    trapError(message.c_str());
    log_event.toUtf8(ROADtag); // Sets event_message since c_str may disappear
}

RoadError::
RoadError(int code, const char *fmt)
: log_event(code, RoadEvent::ErrorEvent)
{ 
    trapError(fmt);
}

RoadError::
RoadError(int code, const char *fmt, int d)
: log_event(code, RoadEvent::ErrorEvent, d)
{ 
    trapError(fmt);
}

RoadError::
RoadError(int code, const char *fmt, int d, int d2)
: log_event(code, RoadEvent::ErrorEvent, d, d2)
{ 
    trapError(fmt);
}

RoadError::
RoadError(int code, const char *fmt, int d, int d2, RoadArg64 arg)
: log_event(code, RoadEvent::ErrorEvent, d, d2, arg)
{ 
    trapError(fmt);
}

RoadError::
RoadError(int code, const char *fmt, int d, int d2, RoadArg64 arg, RoadArg64 arg2)
: log_event(code, RoadEvent::ErrorEvent, d, d2, arg, arg2)
{ 
    trapError(fmt);
}

RoadError::
RoadError(int code, ThFlagsInt eventFlags, const char *fmt)
: log_event(code, eventFlags|RoadEvent::ErrorEvent)
{ 
    trapError(fmt);
}

RoadError::
RoadError(int code, ThFlagsInt eventFlags, const char *fmt, int d)
: log_event(code, eventFlags|RoadEvent::ErrorEvent, d)
{ 
    trapError(fmt);
}

RoadError::
RoadError(int code, ThFlagsInt eventFlags, const char *fmt, int d, int d2)
: log_event(code, eventFlags|RoadEvent::ErrorEvent, d, d2)
{ 
    trapError(fmt);
}

RoadError::
RoadError(int code, ThFlagsInt eventFlags, const char *fmt, int d, int d2, RoadArg64 arg)
: log_event(code, eventFlags|RoadEvent::ErrorEvent, d, d2, arg)
{ 
    trapError(fmt);
}

RoadError::
RoadError(int code, ThFlagsInt eventFlags, const char *fmt, int d, int d2, RoadArg64 arg, RoadArg64 arg2)
: log_event(code, eventFlags|RoadEvent::ErrorEvent, d, d2, arg, arg2)
{ 
    trapError(fmt);
}

RoadError::
RoadError(const RoadError &other)
: log_event(other.log_event)
{
}//RoadError copy

RoadError & RoadError::
operator=(const RoadError &other)
{
    log_event= other.log_event;
    return *this;
}//operator=

#//!\name Virtual methods
const char * RoadError::
what() const throw()
{
    return log_event.toUtf8(ROADtag).constData();
}//what

#//!\name Methods

//! Log error instead of throwing it.
void RoadError::
logError() const
{
    global_log << what() << endl;
}//logError

//! 
void RoadError::
trapError(const char *fmt)
{
    log_event.initializeEvent(fmt);
    log_event.maybeSetEventMessage();
    if(s_error_setting!=ErrorExpected && log_event.eventId()!=0 && (log_event.eventFlags()&LogOK)==0){
        //!\todo Add user list of required errors and ignored errors -- same for warnings and logs
        //!\todo Eventually, invoke error dialog box and write log events to file as done for thesa.com
        const char *err= what();
        // see RoadLogEvent::toUtf8 for source of error
        err= "";   // Break here, also break in ThLog.trapLog()
    }
}//trapError


}//namespace Thesa

