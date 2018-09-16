/****************************************************************************
**
** Copyright (C) 2013-2018 C.B. Barber. All rights reserved.
** $Id: //main/2005/thid/src/thidcore/ThCall.cpp#20 $$Change: 2464 $
** $Date: 2018/08/27 $$Author: bbarber $
**
****************************************************************************/

#//! ThCall -- All exceptions thrown by Thesa are ThCalls
#//! Do not throw ThCall's from destructors.  Use e.logError() instead.

#include "thidcore/ThCall.h"

#include "thidcore/thesa.h"
#include "road/RoadEventStack.h"

namespace Thesa {

#//!\name Constructors, etc.

ThCall::
ThCall()
: log_event()
{ }

ThCall::
ThCall(int code, const char *fmt)
: log_event(code, RoadEvent::Normal|RoadEvent::CallEvent)
{ 
    trapCall(fmt);
}

ThCall::
ThCall(int code, const char *fmt, const QString &s)
: log_event(code, RoadEvent::Normal|RoadEvent::CallEvent, 0,0, RoadArg64(s))
{
    trapCall(fmt);
}

ThCall::
ThCall(int code, const char *fmt, int d)
: log_event(code, RoadEvent::Normal|RoadEvent::CallEvent, d)
{ 
    trapCall(fmt);
}

ThCall::
ThCall(int code, const char *fmt, int d, int d2)
: log_event(code, RoadEvent::Normal|RoadEvent::CallEvent, d, d2)
{ 
    trapCall(fmt);
}

ThCall::
ThCall(int code, const char *fmt, int d, int d2, RoadArg64 arg)
: log_event(code, RoadEvent::Normal|RoadEvent::CallEvent, d, d2, arg)
{ 
    trapCall(fmt);
}

ThCall::
ThCall(int code, const char *fmt, int d, int d2, RoadArg64 arg, RoadArg64 arg2)
: log_event(code, RoadEvent::Normal|RoadEvent::CallEvent, d, d2, arg, arg2)
{ 
    trapCall(fmt);
}

ThCall::
ThCall(int code, ThFlagsInt eventFlags, const char *fmt)
: log_event(code, eventFlags|RoadEvent::CallEvent)
{ 
    trapCall(fmt);
}

ThCall::
ThCall(int code, ThFlagsInt eventFlags, const char *fmt, const QString &s)
: log_event(code, eventFlags|RoadEvent::CallEvent, 0,0, RoadArg64(s))
{
    trapCall(fmt);
}

ThCall::
ThCall(int code, ThFlagsInt eventFlags, const char *fmt, int d)
: log_event(code, eventFlags|RoadEvent::CallEvent, d)
{ 
    trapCall(fmt);
}

ThCall::
ThCall(int code, ThFlagsInt eventFlags, const char *fmt, int d, int d2)
: log_event(code, eventFlags|RoadEvent::CallEvent, d, d2)
{ 
    trapCall(fmt);
}

ThCall::
ThCall(int code, ThFlagsInt eventFlags, const char *fmt, int d, int d2, RoadArg64 arg)
: log_event(code, eventFlags|RoadEvent::CallEvent, d, d2, arg)
{ 
    trapCall(fmt);
}

ThCall::
ThCall(int code, ThFlagsInt eventFlags, const char *fmt, int d, int d2, RoadArg64 arg, RoadArg64 arg2)
: log_event(code, eventFlags|RoadEvent::CallEvent, d, d2, arg, arg2)
{ 
    trapCall(fmt);
}

ThCall::
ThCall(const ThCall &other)
: log_event(other.log_event)
{
}//ThCall copy

ThCall & ThCall::
operator=(const ThCall &other)
{
    log_event= other.log_event;
    return *this;
}//operator=

//! Trap return if returns() not called (programming error or catch..)
//! Note: ThCall() performs an immediate ~ThCall()
ThCall::
~ThCall()
{
    if(log_event.isInitialized() && !log_event.hasReturned()){
        if(log_event.eventNumber()==RoadEventStack::currentEventNumber()-1){
            log_event.setIgnoreReturn();
        }
        trapReturn(&log_event, NULL);
    }
}//~ThCall

#//!\name Methods

void ThCall::
calls(int code, const char *fmt)
{
    log_event= RoadLogEvent(code, RoadEvent::Normal|RoadEvent::CallEvent);
    trapCall(fmt);
}

void ThCall::
calls(int code, const char *fmt, const QString &s)
{
    log_event= RoadLogEvent(code, RoadEvent::Normal|RoadEvent::CallEvent,0,0, s);
    trapCall(fmt);
}

void ThCall::
calls(int code, const char *fmt, int d)
{ 
    log_event= RoadLogEvent(code, RoadEvent::Normal|RoadEvent::CallEvent, d);
    trapCall(fmt);
}

void ThCall::
calls(int code, const char *fmt, int d, int d2)
{ 
    log_event= RoadLogEvent(code, RoadEvent::Normal|RoadEvent::CallEvent, d, d2);
    trapCall(fmt);
}

void ThCall::
calls(int code, const char *fmt, int d, int d2, RoadArg64 arg)
{ 
    if(log_event.millisecondsUtc()!=0){
        throw ThError(11884, "ThCall calls: RoadLogEvent TH%d already initialized at %05d C%i", log_event.eventId(), log_event.eventNumber(), qint64(log_event.eventLevel()));
    }
    log_event= RoadLogEvent(code, RoadEvent::Normal|RoadEvent::CallEvent, d, d2, arg);
    trapCall(fmt);
}

void ThCall::
calls(int code, const char *fmt, int d, int d2, RoadArg64 arg, RoadArg64 arg2)
{ 
    log_event= RoadLogEvent(code, RoadEvent::Normal|RoadEvent::CallEvent, d, d2, arg, arg2);
    trapCall(fmt);
}

void ThCall::
returns(int code, ThFlagsInt eventFlags, const char *fmt)
{ 
    RoadLogEvent e(code, eventFlags|RoadEvent::ReturnEvent|RoadLogEvent::PreviousEventLevel);
    trapReturn(&e, fmt);
}

void ThCall::
returns(int code, ThFlagsInt eventFlags, const char *fmt, int d)
{ 
    RoadLogEvent e(code, eventFlags|RoadEvent::ReturnEvent|RoadLogEvent::PreviousEventLevel, d);
    trapReturn(&e, fmt);
}

void ThCall::
returns(int code, ThFlagsInt eventFlags, const char *fmt, int d, int d2)
{ 
    RoadLogEvent e(code, eventFlags|RoadEvent::ReturnEvent|RoadLogEvent::PreviousEventLevel, d, d2);
    trapReturn(&e, fmt);
}

void ThCall::
returns(int code, ThFlagsInt eventFlags, const char *fmt, int d, int d2, RoadArg64 arg)
{ 
    RoadLogEvent e(code, eventFlags|RoadEvent::ReturnEvent|RoadLogEvent::PreviousEventLevel, d, d2, arg);
    trapReturn(&e, fmt);
}

void ThCall::
returns(int code, ThFlagsInt eventFlags, const char *fmt, int d, int d2, RoadArg64 arg, RoadArg64 arg2)
{ 
    RoadLogEvent e(code, eventFlags|RoadEvent::ReturnEvent|RoadLogEvent::PreviousEventLevel, d, d2, arg, arg2);
    trapReturn(&e, fmt);
}

//! Break here for a specific code
void ThCall::
trapCall(const char *fmt)
{
    ThFlagsInt eventPriority= log_event.eventFlags()&RoadEvent::EventPriorityMask;
    ThFlagsInt eventSpecial= log_event.eventFlags()&(RoadEvent::IsSpecial|LogOK|LogDisk); //!<\todo Need to identify special flags
    if(eventSpecial && eventPriority==0){
        log_event.setEventPriority(RoadEvent::Normal);
    }
    if(RoadEvent::isLogging(log_event.eventId(), log_event.eventFlags())){
        log_event.initializeEvent(fmt);
        int level= RoadEventStack::nextEventLevel();
        if(log_event.eventId()==30100){
            int i= log_event.eventId(); 
            i= i;   // Break here
        }
        if(RoadEvent::isOutput()){
            logStream() << log_event.toUtf8() << "\n";
        }
        if(level != log_event.eventLevel()+1){
            ThWarn(10946, "ThCall inconsistent: switched RoadEventStacks.  Expecting eventLevel %d.  Got %d", log_event.eventLevel()+1, level);
        }
    }
}//trapCall

void ThCall::
trapReturn(RoadLogEvent *e, const char *fmt)
{
    log_event.setHasReturned();
    if(log_event.isInitialized()){ // the corresponding trapCall
        if(fmt){
            e->initializeEvent(fmt);
        }else{ //~ThCall
            e->updateEventNumberTime();
            e->setHasReturned(); // Abbreviated message for toUtf8()
        }
        int level= RoadEventStack::previousEventLevel();
        if(level != log_event.eventLevel()){
            ThWarn(10947, "ThCall trapReturn: eventLevel %d for current RoadEventStack is not eventLevel %d of previous call %lli", level, log_event.eventLevel(), (qint64)log_event.eventNumber());
        }
        if(e->eventLevel() != log_event.eventLevel()){
            ThWarn(10948, "ThCall trapReturn: returned eventLevel %d is not eventLevel %d of previous call %lli", e->eventLevel(), log_event.eventLevel(), (qint64)log_event.eventNumber());
        }
        //!\todo Log return only if intervening events on RoadEventStack. RoadEventStack::events is currently not maintained 
        if(RoadEvent::isOutput()){
            if(!log_event.ignoreReturn()){
                logStream() << e->toUtf8(log_event) << "\n";
            }
        }
    }else if(RoadEvent::isSelected(e->eventId()) && fmt){
        e->initializeEvent(fmt);
        e->setEventType(RoadEvent::LogEvent); //!\todo Should selecting a return event turn it into a LogEvent if the corresponding call is inactive?
        if(RoadEvent::isOutput()){
            logStream() << e->toUtf8(log_event) << "\n";
        }
    }
}//trapReturn

const char * ThCall::
what() const
{
    return log_event.toUtf8(ThError::ROADtag).constData();
}//what


}//namespace Thesa

