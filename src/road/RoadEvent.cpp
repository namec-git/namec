/****************************************************************************
**
** Copyright (C) 2013-2018 C.B. Barber. All rights reserved.
** $Id: //main/2005/thid/src/road/RoadEvent.cpp#19 $$Change: 2390 $
** $Date: 2018/04/27 $$Author: bbarber $
**
****************************************************************************/


#include "road/RoadEvent.h"

#include "road/RoadEventPool.h"
#include "road/RoadEventStack.h"
#include "thidcore/ThError.h"


namespace Thesa {

#//\name Class constants

const char * RoadEvent::
EventTypeFlags= "XLCEWRYZ";

#//\name Constructors

RoadEvent::
RoadEvent(const RoadEvent &other)
: event_format(other.event_format)
, arg_to_utf8(other.arg_to_utf8)
, arg2_to_utf8(other.arg2_to_utf8)
, event_priority(other.event_priority)
, event_flags(other.event_flags)
{
}//RoadEvent copy

RoadEvent & RoadEvent::
operator=(const RoadEvent &other)
{
    event_format= other.event_format;
    arg_to_utf8= other.arg_to_utf8;
    arg2_to_utf8= other.arg2_to_utf8;
    event_priority= other.event_priority;
    event_flags= other.event_flags;
    return *this;
}//operator=

#//!\name Methods

quint32 RoadEvent::
argFormat(const RoadArg64 &arg, const RoadArg64 &arg2) const
{
    quint32 args;
    if(isFormatDD()){
        return FormatDD;
    }
    args= FormatL+FormatL2;  // Default args unless %s && hasToUtf8
    const char *endFormat= event_format+MaxFormatLength;
    int argCount= 0;
    int firstInt= 0;
    int secondInt= 0;
    int firstArg= 0;
    int secondArg= 0;
    bool inArg= false;
    for(const char *s= event_format; *s && s<endFormat; ++s){
        char c= *s;
        bool hasToUtf8= false;
        int argsUpdate= 0;
        if(!inArg){
            if(c==ArgSprintf){
                inArg= true;
            }
        }else switch(c){
            case '-':   // Flag directives
            case '+':
            case '#':
            case ' ':
                break;
            case '0':   // Flag, Width, and precision
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case '.':
                break;
            case 'l':  // 'll' indicates a long long parameter (ignored by QString expect for '%lc' and '%lC'
            case 'h':  // other size prefixes
            case 'I':
                break;
            case 'c':   // int type
            case 'C':
            case 'd':
            case 'o':
            case 't':
            case 'u':
            case 'X':
                ++argCount;
                inArg= false;
                if(argCount>4){
                    throw ThError(10930, "RoadEvents: More than four arguments (%d) in '%s'", argCount, 0, event_format);
                    // Cannot ThError::warn since argFormat is called by warn::toUtf8
                    break;
                }
                if(secondInt){
                    throw ThError(10929, "RoadEvent: More than two int specifiers (%%c, %%C, %%d, %%o, %%t, %%u, %%X) in format '%s'", 0,0, event_format);
                    // Cannot ThError::warn since argFormat is called by warn::toUtf8
                    break;
                }else if(firstInt){
                    secondInt= argCount;
                }else{
                    firstInt= argCount;
                }
                break;
            case 'a':   // non-int type
            case 'A':
            case 'e':
            case 'E':
            case 'f':
            case 'g':
            case 'G':
            case 'i':
            case 'p':
            case 's':
            case 'S':
            case 'x':
                ++argCount;
                inArg= false;
                if(secondArg){
                    throw ThError(10932, "RoadEvent: More than two int64 or pointer specifiers (%%a, %%e, %%E, %%f, %%g, %%lli, %%p, %%s, %%llx) in format '%s'", 0,0, event_format);
                    // Cannot ThError::warn since argFormat is called by warn::toUtf8
                    break;
                }else if(firstArg){
                    secondArg= argCount;
                    hasToUtf8= arg2.hasToUtf8();
                    argsUpdate= FormatP2-FormatL2; // if hasToUtf8 and '%s'
                }else{
                    firstArg= argCount;
                    hasToUtf8= arg.hasToUtf8();
                    argsUpdate= FormatP-FormatL; // if hasToUtf8 and '%s'
                }
                if(c=='s' || c=='S'){
                    if(!hasToUtf8){
                        throw ThError(10933, "RoadEvent: '%%s' arg without a string parameter for '%s'", 0,0, event_format);
                        //!\todo Cannot ThError::warn or throw to a ThWarn since argFormat is called by warn::toUtf8
                    }
                    args += argsUpdate;
                }
                break;
            default:
                inArg= false;
                break;
        }
    }
    if(firstArg==0){
        return FormatDD;
    }
    switch(firstInt+4*secondInt){
        case 0+4*0:
            return FormatAADD+args;
        case 1+4*0:
            return FormatDAAD+args;
        case 1+4*2:
            return FormatDDAA+args;
        case 1+4*3:
            return FormatDADA+args;
        case 1+4*4:
            return FormatDAAD+args;
        case 2+4*0:
            return FormatADAD+args;
        case 2+4*3:
            return FormatADDA+args;
        case 2+4*4:
            return FormatADAD+args;
        case 3+4*0:
        case 3+4*4:
            return FormatAADD+args;
        default:
            throw ThError(10931, "RoadEvent inconsistent: Unknown case for FirstInt %d SecondInt %d in format '%s'", firstInt, secondInt, event_format);
    }
    return FormatDDAA+args;
}//argFormat

QByteArray RoadEvent::
argToUtf8(const RoadArg64 &arg) const
{
    if(arg_to_utf8){
        return (*arg_to_utf8)(arg);
    }
    return arg.toUtf8();
}//argToUtf8

QByteArray RoadEvent::
arg2ToUtf8(const RoadArg64 &arg2) const
{
    if(arg2_to_utf8){
        return (*arg2_to_utf8)(arg2);
    }
    return arg2.toUtf8();
}//arg2ToUtf8

//! Returen RoadEvent for eventId [static]
const RoadEvent & RoadEvent::
event(ThLocalInt eventId)
{
    RoadEventStack *t= RoadEventStack::currentEventStack();
    RoadEventPool *p= t->eventPool();
    return p->roadEvent(eventId);
}//event

//! Return true if logging this event [static]
bool RoadEvent::
isDeselect(ThLocalInt eventId) 
{ 
    RoadEventStack *t= RoadEventStack::currentEventStack();
    ThLocalInt deselectId= t->logDeselect();
    RoadEventPool *p= t->eventPool();
    if(deselectId!=NoIndex && p->isDeselectEvent(deselectId, eventId)){
        return true;
    }
    return false;
}//isDeselect

//! Return true if logging this eventid [static]
bool RoadEvent::
isLogging(ThLocalInt eventId, quint32 eventFlags) 
{ 
    RoadEventStack *t= RoadEventStack::currentEventStack();
    ThLocalInt selectedId= t->logSelected();
    ThLocalInt deselectId= t->logDeselect();
    RoadEventPool *p= t->eventPool(); // The corresponding RoadEvent may not be created yet
    ThFlagsInt priority= eventFlags&EventPriorityMask;
    bool isLog= false;
    if(priority<=t->logPriority()){
        isLog= true;
    }else if(selectedId!=NoIndex && p->isSelectedEvent(selectedId, eventId)){
        isLog= true;
    }
    if(isLog && deselectId!=NoIndex && p->isDeselectEvent(deselectId, eventId)){
        isLog= false;
    }
    return isLog;
}//isLogging

//! Return true if logging this eventid [static]
bool RoadEvent::
isOutput() 
{ 
    RoadEventStack *t= RoadEventStack::currentEventStack();
    if(t->firstLogEvent()!=0){
        if(t->firstLogEvent()<=RoadEventStack::currentEventNumber()){  // nextEvent already called
            t->setFirstLogEvent(0);
        }
        return false;
    }
    return true;
}//isOutput

//! Return true if logging this event for currentEventStack [static]
bool RoadEvent::
isSelected(ThLocalInt eventId) 
{ 
    RoadEventStack *t= RoadEventStack::currentEventStack();
    ThLocalInt selectedId= t->logSelected();
    ThLocalInt deselectId= t->logDeselect();
    RoadEventPool *p= t->eventPool();
    if(selectedId!=NoIndex && p->isSelectedEvent(selectedId, eventId)){
        if(deselectId==NoIndex || !p->isDeselectEvent(deselectId, eventId)){
            return true;
        }
    }
    return false;
}//isSelected

//! Set isDeselect for eventId in deselectId of currentEventStack [static]
void RoadEvent::
setIsDeselect(ThLocalInt eventId) 
{ 
    RoadEventStack *t= RoadEventStack::currentEventStack();
    ThLocalInt deselectId= t->logDeselect();
    RoadEventPool *p= t->eventPool();
    p->addDeselectEvent(deselectId, eventId);
}//setIsDeselect

//! Set isSelected for eventId in selectedId of currentEventStack [static]
void RoadEvent::
setIsSelected(ThLocalInt eventId) 
{ 
    RoadEventStack *t= RoadEventStack::currentEventStack();
    ThLocalInt selectedId= t->logSelected();
    RoadEventPool *p= t->eventPool();
    p->addSelectedEvent(selectedId, eventId);
}//setIsSelected

}//namespace Thesa

