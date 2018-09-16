/****************************************************************************
**
** Copyright (C) 2013-2018 C.B. Barber. All rights reserved.
** $Id: //main/2005/thid/src/thidcore/ThWarn.cpp#11 $$Change: 2450 $
** $Date: 2018/07/25 $$Author: bbarber $
**
****************************************************************************/

#//! ThWarn -- Log a warning, may lead to a thrown ThError

#include "thidcore/ThWarn.h"
#include "thidcore/thesa.h"

#include "road/RoadEventStack.h" // logStream()

namespace Thesa {

#//!\name Constructors, etc.

ThWarn::
ThWarn()
: log_event()
{ }

ThWarn::
ThWarn(int code, const char *fmt)
: log_event(code, RoadEvent::WarnEvent)
{ 
    trapWarn(fmt);
}

ThWarn::
ThWarn(int code, const char *fmt, const QString &s)
: log_event(code, RoadEvent::WarnEvent, 0,0, RoadArg64(s))
{
    trapWarn(fmt);
}

ThWarn::
ThWarn(int code, const char *fmt, int d)
: log_event(code, RoadEvent::WarnEvent, d)
{ 
    trapWarn(fmt);
}

ThWarn::
ThWarn(int code, const char *fmt, int d, int d2)
: log_event(code, RoadEvent::WarnEvent, d, d2)
{ 
    trapWarn(fmt);
}

ThWarn::
ThWarn(int code, const char *fmt, int d, int d2, RoadArg64 arg)
: log_event(code, RoadEvent::WarnEvent, d, d2, arg)
{ 
    trapWarn(fmt);
}

ThWarn::
ThWarn(int code, const char *fmt, int d, int d2, RoadArg64 arg, RoadArg64 arg2)
: log_event(code, RoadEvent::WarnEvent, d, d2, arg, arg2)
{ 
    trapWarn(fmt);
}

ThWarn::
ThWarn(int code, ThFlagsInt eventFlags, const char *fmt)
: log_event(code, eventFlags|RoadEvent::WarnEvent)
{ 
    trapWarn(fmt);
}

ThWarn::
ThWarn(int code, ThFlagsInt eventFlags, const char *fmt, const QString &s)
: log_event(code, eventFlags|RoadEvent::WarnEvent, 0,0, RoadArg64(s))
{
    trapWarn(fmt);
}

ThWarn::
ThWarn(int code, ThFlagsInt eventFlags, const char *fmt, int d)
: log_event(code, eventFlags|RoadEvent::WarnEvent, d)
{ 
    trapWarn(fmt);
}

ThWarn::
ThWarn(int code, ThFlagsInt eventFlags, const char *fmt, int d, int d2)
: log_event(code, eventFlags|RoadEvent::WarnEvent, d, d2)
{ 
    trapWarn(fmt);
}

ThWarn::
ThWarn(int code, ThFlagsInt eventFlags, const char *fmt, int d, int d2, RoadArg64 arg)
: log_event(code, eventFlags|RoadEvent::WarnEvent, d, d2, arg)
{ 
    trapWarn(fmt);
}

ThWarn::
ThWarn(int code, ThFlagsInt eventFlags, const char *fmt, int d, int d2, RoadArg64 arg, RoadArg64 arg2)
: log_event(code, eventFlags|RoadEvent::WarnEvent, d, d2, arg, arg2)
{ 
    trapWarn(fmt);
}

ThWarn::
ThWarn(const ThWarn &other)
: log_event(other.log_event)
{
}//ThWarn copy

ThWarn & ThWarn::
operator=(const ThWarn &other)
{
    log_event= other.log_event;
    return *this;
}//operator=

//! Break here for a specific code
void ThWarn::
trapWarn(const char *fmt)
{
    log_event.initializeEvent(fmt);
    if(log_event.eventId()==30100){
        int i= 0; 
        i= i;   // Break here
    }
    //!\todo Replace logStream() in ThLog.trapLog, ThWarn.trapWarn, and ThError.warn with log event buffer
    logStream() << log_event.toUtf8(ThError::ROADtag).constData() << endl;
}//trapWarn

const char * ThWarn::
what() const
{
    return log_event.toUtf8(ThError::ROADtag).constData();
}//what


}//namespace Thesa

