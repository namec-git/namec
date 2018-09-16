/****************************************************************************
**
** Copyright (C) 2013-2018 C.B. Barber. All rights reserved.
** $Id: //main/2005/thid/src/road/RoadEventStack.cpp#20 $$Change: 2450 $
** $Date: 2018/07/25 $$Author: bbarber $
**
****************************************************************************/


#include "road/RoadEventStack.h"

#include "road/RoadEvent.h"
#include "road/RoadEventPool.h"
#include "thidcore/ThError.h"

#include <QtCore/QTextStream>

namespace Thesa {

#//!\name Global objects

extern QTextStream
ErrStream(stderr, QIODevice::WriteOnly);

extern QTextStream 
InStream(stdin, QIODevice::ReadOnly);

extern QTextStream 
OutStream(stdout, QIODevice::WriteOnly);

#//!\name Class objects

QThreadStorage<RoadEventStack *> RoadEventStack::
thread_event_stacks;

QAtomicInt RoadEventStack::
next_thread_id(0);

QAtomicInt RoadEventStack::
next_event_number(0);

bool RoadEventStack::
is_log_stdout= false;

#//\name Constructors

RoadEventStack::
RoadEventStack()
: thread_id(NoIndex)
, disk_id(NoIndex)
, event_level(0)
, event_pool(NULL)
, event_stack()
, event_context()
, event_log()
, event_statistics()
, log_priority(RoadEvent::Normal)
, first_log_event(0)
, log_selected(NoIndex)
, log_deselect(NoIndex)
{
    thread_id= RoadEventStack::nextThreadId();
    event_pool= RoadEventPool::globalEventPool();
}//RoadEventStack default

#//!\name Get/Set

int RoadEventStack::
logLevel() const
{
    int result= (log_priority&RoadEvent::EventPriorityMask)>>RoadEvent::EventPriorityShift;
    return result;
}//logLevel

void RoadEventStack::
setLogLevel(int i)
{
    ThFlagsInt priority= i<<RoadEvent::EventPriorityShift;
    if(priority&(~RoadEvent::EventPriorityMask) || priority>RoadEvent::Select){
        throw ThError(10677, "RoadEventStack setLogLevel: expecting Always(0)..Select(7) for logLevel.  Got %d", i);
    }
    setLogPriority(priority);
}//setLogLevel

void RoadEventStack::
setLogPriority(ThFlagsInt i)
{
    if(i&(~RoadEvent::EventPriorityMask) || i>RoadEvent::Select){
        throw ThError(11508, "RoadEventStack setLogPriority: expecting RoadEvent::Always...RoadEvent::Select for maximum logPriority.  Got 0x%X", i);
    }
    log_priority &= ~RoadEvent::EventPriorityMask; 
    
    log_priority |= i;
}//setLogPriority

#//!\name Class functions

//! Return current RoadEventStack [static]
RoadEventStack * RoadEventStack::
currentEventStack() 
{ 
    if(!thread_event_stacks.hasLocalData()){
        RoadEventStack *t= new RoadEventStack();
        thread_event_stacks.setLocalData(t);
        t->eventPool()->addEventStack(t);
    }
    return thread_event_stacks.localData(); 
}//currentEventStack

int RoadEventStack::
currentLogLevel()
{
    RoadEventStack *eventStack= RoadEventStack::currentEventStack();
    return eventStack->logLevel();
}//currentLogLevel

//! Return true if logging RoadEvent::Detail and Detail2 [static]
bool RoadEventStack::
isDetailLogging() 
{ 
    return (RoadEventStack::currentEventStack()->logPriority()>=RoadEvent::Detail); 
}//isDetailLogging

//! Return true if logging RoadEvent::Detail2 [static]
bool RoadEventStack::
isDetail2Logging() 
{ 
   return (RoadEventStack::currentEventStack()->logPriority()>=RoadEvent::Detail2); 
}//isDetail2Logging

//! Set and return next eventLevel for currentEventStack [static]
int RoadEventStack::
nextEventLevel() 
{ 
    RoadEventStack *t= RoadEventStack::currentEventStack();
    if(t->event_level==RoadLogEvent::MaximumEventLevel){
        throw ThError(10409, "RoadEventStack nextEventLevel: overflow of eventLevel 0..%d.  Can not increment event_level %d of thread %i", RoadLogEvent::MaximumEventLevel, t->event_level, qint64(t->thread_id));
    }
    return ++t->event_level;
}//nextEventLevel

//! Set and return previous eventLevel for currentEventStack [static]
int RoadEventStack::
previousEventLevel() 
{ 
    RoadEventStack *t= RoadEventStack::currentEventStack();
    if(t->event_level==0){
        throw ThError(11284, "RoadEventStack previousEventLevel: underflow of eventLevel 0..%d.  Can not decrement event_level %d of thread %i", RoadLogEvent::MaximumEventLevel, t->event_level, qint64(t->thread_id));
    }
    return --t->event_level;
}//previousEventLevel

void RoadEventStack::
setErrStream(QIODevice *device) 
{ 
    Thesa::ErrStream.setDevice(device); 
}//setErrStream

void RoadEventStack::
setInStream(QIODevice *device) 
{ 
    Thesa::InStream.setDevice(device); 
}//setInStream

void RoadEventStack::
setOutStream(QIODevice *device) 
{ 
    Thesa::OutStream.setDevice(device); 
}//setOutStream

#//!\name Global methods

//! Return ErrStream or OutStream depending on 
extern QTextStream &
logStream()
{
    if(RoadEventStack::isLogStdout()){
        return OutStream;
    }else{
        return ErrStream;
    }
}//logStream

}//namespace Thesa

