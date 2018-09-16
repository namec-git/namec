/****************************************************************************
**
** Copyright (C) 2013-2018 C.B. Barber. All rights reserved.
** $Id: //main/2005/thid/src/thidcore/ThLog.cpp#29 $$Change: 2480 $
** $Date: 2018/09/15 $$Author: bbarber $
**
****************************************************************************/

#//! ThLog -- All exceptions thrown by Thesa are ThLogs
#//! Do not throw ThLog's from destructors.  Use e.logError() instead.

#include "thidcore/thesa.h"

#include "thidcore/ThLog.h"

#include "road/RoadEventStack.h" // logStream()

namespace Thesa {

#//!\name Constructors, etc.

ThLog::ThLog()
: log_event()
{ }

ThLog::ThLog(int code, const char *fmt)
: log_event(code, RoadEvent::Normal|RoadEvent::LogEvent)
{ 
    trapLog(fmt);
}

ThLog::ThLog(int code, const char *fmt, const QString &s)
: log_event(code, RoadEvent::Normal|RoadEvent::LogEvent, 0,0, RoadArg64(s))
{
    trapLog(fmt);
}

ThLog::
ThLog(int code, const char *fmt, int d)
: log_event(code, RoadEvent::Normal|RoadEvent::LogEvent, d)
{ 
    trapLog(fmt);
}

ThLog::ThLog(int code, const char *fmt, int d, int d2)
: log_event(code, RoadEvent::Normal|RoadEvent::LogEvent, d, d2)
{ 
    trapLog(fmt);
}

ThLog::ThLog(int code, const char *fmt, int d, int d2, RoadArg64 arg)
: log_event(code, RoadEvent::Normal|RoadEvent::LogEvent, d, d2, arg)
{ 
    trapLog(fmt);
}

ThLog::ThLog(int code, const char *fmt, int d, int d2, RoadArg64 arg, RoadArg64 arg2)
: log_event(code, RoadEvent::Normal|RoadEvent::LogEvent, d, d2, arg, arg2)
{ 
    trapLog(fmt);
    if(log_event.eventNumber()>1300 && log_event.eventId()==40271){
        int i= 0; ++i;  // debug trap -- or in RoadLogEvent.toUtf8
    }
}

ThLog::ThLog(int code, ThFlagsInt eventFlags, const char *fmt)
: log_event(code, eventFlags|RoadEvent::LogEvent)
{ 
    trapLog(fmt);
}

ThLog::ThLog(int code, ThFlagsInt eventFlags, const char *fmt, const QString &s)
: log_event(code, eventFlags|RoadEvent::LogEvent, 0,0, RoadArg64(s))
{
    trapLog(fmt);
}

ThLog::ThLog(int code, ThFlagsInt eventFlags, const char *fmt, int d)
: log_event(code, eventFlags|RoadEvent::LogEvent, d)
{ 
    trapLog(fmt);
}

ThLog::ThLog(int code, ThFlagsInt eventFlags, const char *fmt, int d, int d2)
: log_event(code, eventFlags|RoadEvent::LogEvent, d, d2)
{ 
    trapLog(fmt);
}

ThLog::ThLog(int code, ThFlagsInt eventFlags, const char *fmt, int d, int d2, RoadArg64 arg)
: log_event(code, eventFlags|RoadEvent::LogEvent, d, d2, arg)
{ 
    trapLog(fmt);
}

ThLog::ThLog(int code, ThFlagsInt eventFlags, const char *fmt, int d, int d2, RoadArg64 arg, RoadArg64 arg2)
: log_event(code, eventFlags|RoadEvent::LogEvent, d, d2, arg, arg2)
{ 
    trapLog(fmt);
}

ThLog::ThLog(const ThLog &other)
: log_event(other.log_event)
{
}//ThLog copy

ThLog & ThLog::
operator=(const ThLog &other)
{
    log_event= other.log_event;
    return *this;
}//operator=

//! Break here for a specific code
void ThLog::
trapLog(const char *fmt)
{
    ThFlagsInt eventPriority= log_event.eventFlags()&RoadEvent::EventPriorityMask;
    ThFlagsInt eventSpecial= log_event.eventFlags()&(RoadEvent::IsSpecial|LogOK|LogDisk); //!<\todo Need to identify special flags
    if(eventSpecial && eventPriority==0){
        log_event.setEventPriority(RoadEvent::Normal);
    }
    if(RoadEvent::isLogging(log_event.eventId(), log_event.eventFlags())){
        log_event.initializeEvent(fmt);
        if(RoadEvent::isOutput()){
            //!\todo Review use of 'endl' vs. '\n' for logging events.  'endl' calls flush().
            //!\todo Replace ErrStrean in ThLog.trapLog, ThWarn.trapWarn, and ThError.warn with log event buffer
            logStream() << log_event.toUtf8().constData() << endl;
        }
        unsigned int n=24528;
        if(log_event.eventNumber()==n || (log_event.eventNumber()>n && log_event.eventNumber()%10==0)){
            int i= 0; 
            i= i;   // Break here, also break in ThLog.trapLog()
        }
    }
}//trapLog

const char * ThLog::
what() const
{
    return log_event.toUtf8(ThError::ROADtag).constData();
}//what

}//namespace Thesa

