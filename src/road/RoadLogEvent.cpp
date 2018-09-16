/****************************************************************************
**
** Copyright (C) 2008-2018 C.B. Barber. All rights reserved.
** $Id: //main/2005/thid/src/road/RoadLogEvent.cpp#34 $$Change: 2390 $
** $Date: 2018/04/27 $$Author: bbarber $
**
****************************************************************************/

#//! RoadLogEvent -- All exceptions thrown by Thesa are RoadLogEvents

#include "road/RoadLogEvent.h"

#include "road/RoadEvent.h"
#include "road/RoadEventPool.h"
#include "road/RoadEventStack.h"
#include "road/RoadError.h"

#include <QtCore/QDateTime>
#include <QtCore/QString>

//!\todo Handle pragma warning -- sprintf is unsafe
#pragma warning( disable : 4996 )

namespace Thesa {

quint64 RoadLogEvent::
previous_event_milliseconds= 0;

RoadLogEvent::
RoadLogEvent()
: event_id(0)
, event_number(0)
, thread_id(NoIndex)
, event_level(0)
, event_milliseconds(0)
, event_size(0)
, log_event_flags(0)
, int_1(0)
, int_2(0)
, arg64_1()
, arg64_2()
, event_message()
{
}//RoadLogEvent default

RoadLogEvent::
RoadLogEvent(quint32 eventId)
: event_id(eventId)
, event_number(0)
, thread_id(NoIndex)
, event_level(0)
, event_milliseconds(0)
, event_size(0)
, log_event_flags(0|RoadEvent::IsFormatDD)
, int_1(0)
, int_2(0)
, arg64_1()
, arg64_2()
{
}//RoadLogEvent ... fmt

RoadLogEvent::
RoadLogEvent(quint32 eventId, int d)
: event_id(eventId)
, event_number(0)
, thread_id(NoIndex)
, event_level(0)
, event_milliseconds(0)
, event_size(0)
, log_event_flags(0|RoadEvent::IsFormatDD)
, int_1(d)
, int_2(0)
, arg64_1()
, arg64_2()
, event_message()
{
}//RoadLogEvent ... int

RoadLogEvent::
RoadLogEvent(quint32 eventId, int d, int d2)
: event_id(eventId)
, event_number(0)
, thread_id(NoIndex)
, event_level(0)
, event_milliseconds(0)
, event_size(0)
, log_event_flags(0|RoadEvent::IsFormatDD)
, int_1(d)
, int_2(d2)
, arg64_1()
, arg64_2()
, event_message()
{
}//RoadLogEvent ... int2

RoadLogEvent::
RoadLogEvent(quint32 eventId, int d, int d2, RoadArg64 arg)
: event_id(eventId)
, event_number(0)
, thread_id(NoIndex)
, event_level(0)
, event_milliseconds(0)
, event_size(0)
, log_event_flags(0)
, int_1(d)
, int_2(d2)
, arg64_1(arg)
, arg64_2()
, event_message()
{
}//RoadLogEvent ... arg

RoadLogEvent::
RoadLogEvent(quint32 eventId, int d, int d2, RoadArg64 arg, RoadArg64 arg2)
: event_id(eventId)
, event_number(0)
, thread_id(NoIndex)
, event_level(0)
, event_milliseconds(0)
, event_size(0)
, log_event_flags(0|RoadEvent::IsFormatDD)
, int_1(d)
, int_2(d2)
, arg64_1(arg)
, arg64_2(arg2)
{
}//RoadLogEvent ... arg2

RoadLogEvent::
RoadLogEvent(quint32 eventId, quint32 eventFlags)
: event_id(eventId)
, event_number(0)
, thread_id(NoIndex)
, event_level(0)
, event_milliseconds(0)
, event_size(0)
, log_event_flags(eventFlags|RoadEvent::IsFormatDD)
, int_1(0)
, int_2(0)
, arg64_1()
, arg64_2()
, event_message()
{
}//RoadLogEvent eventFlags fmt

RoadLogEvent::
RoadLogEvent(quint32 eventId, quint32 eventFlags, int d)
: event_id(eventId)
, event_number(0)
, thread_id(NoIndex)
, event_level(0)
, event_milliseconds(0)
, event_size(0)
, log_event_flags(eventFlags|RoadEvent::IsFormatDD)
, int_1(d)
, int_2(0)
, arg64_1()
, arg64_2()
, event_message()
{
}//RoadLogEvent eventFlags ... int

RoadLogEvent::
RoadLogEvent(quint32 eventId, quint32 eventFlags, int d, int d2)
: event_id(eventId)
, event_number(0)
, thread_id(NoIndex)
, event_level(0)
, event_milliseconds(0)
, event_size(0)
, log_event_flags(eventFlags|RoadEvent::IsFormatDD)
, int_1(d)
, int_2(d2)
, arg64_1()
, arg64_2()
, event_message()
{
}//RoadLogEvent eventFlags ... int2

RoadLogEvent::
RoadLogEvent(quint32 eventId, quint32 eventFlags, int d, int d2, RoadArg64 arg)
: event_id(eventId)
, event_number(0)
, thread_id(NoIndex)
, event_level(0)
, event_milliseconds(0)
, event_size(0)
, log_event_flags(eventFlags)
, int_1(d)
, int_2(d2)
, arg64_1(arg)
, arg64_2()
, event_message()
{
}//RoadLogEvent eventFlags ... arg

RoadLogEvent::
RoadLogEvent(quint32 eventId, quint32 eventFlags, int d, int d2, RoadArg64 arg, RoadArg64 arg2)
: event_id(eventId)
, event_number(0)
, thread_id(NoIndex)
, event_level(0)
, event_milliseconds(0)
, event_size(0)
, log_event_flags(eventFlags)
, int_1(d)
, int_2(d2)
, arg64_1(arg)
, arg64_2(arg2)
, event_message()
{
}//RoadLogEvent eventFlags ... arg2

//! Usually called on 'throw' but not always, e.g., ThCode::toString...return QStringBuilder
RoadLogEvent::
RoadLogEvent(const RoadLogEvent &other)
: event_id(other.event_id)
, event_number(other.event_number)
, thread_id(other.thread_id)
, event_level(other.event_level)
, event_milliseconds(other.event_milliseconds)
, event_size(other.event_size)
, log_event_flags(other.log_event_flags)
, int_1(other.int_1)
, int_2(other.int_2)
, arg64_1(other.arg64_1)
, arg64_2(other.arg64_2)
, event_message(other.event_message)
{
    maybeSetEventMessage();  // Usually called on 'throw', but not always.  See also RoadError::trapError
}//RoadLogEvent copy (e.g., throw)

RoadLogEvent & RoadLogEvent::
operator=(const RoadLogEvent &other)
{
    event_id= other.event_id;
    event_number= other.event_number;
    thread_id= other.thread_id;
    event_level= other.event_level;
    event_milliseconds= other.event_milliseconds;
    event_size= other.event_size;
    log_event_flags= other.log_event_flags;
    int_1= other.int_1;
    int_2= other.int_2;
    arg64_1= other.arg64_1;
    arg64_2= other.arg64_2;
    event_message= other.event_message;
    return *this;
}//operator=

#//!\name GetSet

const char * RoadLogEvent::
format() const
{
    const char *s= NULL;
    if(eventId()){
        const RoadEvent &e= RoadEvent::event(eventId());
        s= e.eventFormat();
    }else{
        s= "RoadLogEvent: empty RoadLogEvent";
    }
    return s;
}//format

//! Initialize event.  Assumes eventId and args are set.
//! If RoadArg64 not used, set RoadError::IsFormatDD in log_event_flags
void RoadLogEvent::
initializeEvent(const char *fmt)
{
    updateEventNumberTime();
    RoadEventStack *t= RoadEventStack::currentEventStack();
    thread_id= t->threadId();
    event_level= t->eventLevel();
    if(log_event_flags & PreviousEventLevel){
        --event_level;
        log_event_flags &= ~PreviousEventLevel;
    }
    if(!event_id){
        event_id= RoadLogEvent::defaultEventId(fmt);
    }
    RoadEventPool *p= t->eventPool();
    p->addEvent(event_id, log_event_flags, fmt);
}//initializeEvent

// Not used, should set at constructor
void RoadLogEvent::
setEventType(ThFlagsInt i)
{
    RoadEventPool *p= RoadEventPool::globalEventPool();
    p->setEventType(event_id, i);
}//setEventType

#//!\name Methods

//! Set event message if QString arg
//!\todo How to bump QString.ref on use
void RoadLogEvent::
maybeSetEventMessage()
{
    if(arg64_1.hasToUtf8() || arg64_2.hasToUtf8()){
        if(event_message.isEmpty()){
            // on throw ThError, QString's are deallocated
            event_message= toUtf8(RoadError::ROADtag);
        }
    }
}//maybeSetEventMessage

//! Update EventNumber and eventTime, e.g.., for ~ThCall
void RoadLogEvent::
updateEventNumberTime()
{
    event_number=  RoadEventStack::nextEventNumber();
    event_milliseconds= QDateTime::currentMSecsSinceEpoch();
}//updateEventNumberTime

//!\todo FIXUP -- Infinite loop if throws an error
//!\todo Expensive operation when logging ThImporter::importRtf -- largely due to QString.toUtf8 and memory management
QByteArray RoadLogEvent::
toUtf8(const RoadLogEvent &other, const char* tag/*= NULL*/) const
{
    QByteArray result= event_message;
    if(result.isEmpty() || hasReturned()){
        try{
            QDateTime date= QDateTime::fromMSecsSinceEpoch(event_milliseconds);
            RoadEventPool *p= RoadEventPool::globalEventPool();
            RoadEvent e= p->roadEvent(event_id);
            int i= e.eventType()>>1;
            char c= RoadEvent::EventTypeFlags[i];
            int formatOnly= 0;
            QString prefix;
            if(hasReturned()){
                c= RoadEvent::EventTypeFlags[RoadEvent::ReturnEvent>>1];
                const char *t= strchr(e.eventFormat(), RoadEvent::PrefixChar);
                if(t){
                    formatOnly= int(t-e.eventFormat());
                }else{
                    formatOnly= int(strlen(e.eventFormat()));
                }
            }
            qint64 milliDiff= qint64(event_milliseconds)-qint64(previous_event_milliseconds);
            previous_event_milliseconds= event_milliseconds;
            QString msec;
            if(milliDiff==0){
                msec= QStringLiteral(".....");
            }else if(milliDiff<0){
                //!\todo optimize string handling and number conversion
                msec= QStringLiteral("%1").arg(event_milliseconds%1000000, 6, 10, QChar('0'));
            }else if(milliDiff<10){
                msec= QStringLiteral("....%1").arg(milliDiff);
            }else if(milliDiff<100){
                msec= QStringLiteral("...%1").arg(milliDiff);
            }else if(milliDiff<1000){
                msec= QStringLiteral("..%1").arg(milliDiff);
            }else if(milliDiff<10000){
                msec= QStringLiteral(".%1").arg(milliDiff);
            }else if(milliDiff<100000){
                msec= QStringLiteral("%1").arg(milliDiff);
            }else{
                msec= QStringLiteral("%1").arg(event_milliseconds%1000000, 6, 10, QChar('0'));
            }

            //prefix= QString("%1 %2%3 ").arg(date.toString("mmss")).arg(QChar(c)).arg(event_level, 2, 10, QChar('0'));
            prefix= QString("%1 %2 %3%4 ").arg(event_number, MinimumEventNumberWidth, EventNumberBase, QChar('0')).arg(msec).arg(QChar(c)).arg(event_level, 2, 10, QChar('0'));
            if(tag && !formatOnly && !RoadLogEvent::isDefaultEventId(event_id)){
                if(other.isValid() && other.event_id!=event_id){
                    prefix += QString("%1%2 ").arg(QString(tag)).arg(other.event_id);
                }
                prefix += QString("%1%2 ").arg(QString(tag)).arg(event_id);
            }else if(!tag && p->isLogIdentifier() && !RoadLogEvent::isDefaultEventId(event_id)){
                if(other.isValid() && other.event_id!=event_id){
                    prefix += QString("%1%2 ").arg(QString(RoadError::ROADtag)).arg(other.event_id);
                }
                if(e.isSpecial()){
                    prefix += QString("%1%2* ").arg(QString(RoadError::ROADtag)).arg(event_id);
                }else{
                    prefix += QString("%1%2 ").arg(QString(RoadError::ROADtag)).arg(event_id);
                }
            }
            QString message;
            int format= e.argFormat(arg64_1, arg64_2);
            if(formatOnly){
                message= QString(e.eventFormat()).left(formatOnly);
            }else switch(format){
                case RoadEvent::FormatDD:
                    message.sprintf(e.eventFormat(), int_1, int_2);
                    break;
                case RoadEvent::FormatDDAA+RoadEvent::FormatL+RoadEvent::FormatL2:
                    message.sprintf(e.eventFormat(), int_1, int_2, arg64_1.i(), arg64_2.i());
                    break;
                case RoadEvent::FormatDDAA+RoadEvent::FormatL+RoadEvent::FormatP2:
                    message.sprintf(e.eventFormat(), int_1, int_2, arg64_1.i(), e.arg2ToUtf8(arg64_2).constData());
                    break;
                case RoadEvent::FormatDDAA+RoadEvent::FormatP+RoadEvent::FormatL2:
                    message.sprintf(e.eventFormat(), int_1, int_2, e.argToUtf8(arg64_1).constData(), arg64_2.i());
                    break;
                case RoadEvent::FormatDDAA+RoadEvent::FormatP+RoadEvent::FormatP2:
                    message.sprintf(e.eventFormat(), int_1, int_2, e.argToUtf8(arg64_1).constData(), e.arg2ToUtf8(arg64_2).constData());
                    break;
                case RoadEvent::FormatDADA+RoadEvent::FormatL+RoadEvent::FormatL2:
                    message.sprintf(e.eventFormat(), int_1, arg64_1.i(), int_2, arg64_2.i());
                    break;
                case RoadEvent::FormatDADA+RoadEvent::FormatL+RoadEvent::FormatP2:
                    message.sprintf(e.eventFormat(), int_1, arg64_1.i(), int_2, e.arg2ToUtf8(arg64_2).constData());
                    break;
                case RoadEvent::FormatDADA+RoadEvent::FormatP+RoadEvent::FormatL2:
                    message.sprintf(e.eventFormat(), int_1, e.argToUtf8(arg64_1).constData(), int_2, arg64_2.i());
                    break;
                case RoadEvent::FormatDADA+RoadEvent::FormatP+RoadEvent::FormatP2:
                    message.sprintf(e.eventFormat(), int_1, e.argToUtf8(arg64_1).constData(), int_2, e.arg2ToUtf8(arg64_2).constData());
                    break;

                case RoadEvent::FormatDAAD+RoadEvent::FormatL+RoadEvent::FormatL2:
                    message.sprintf(e.eventFormat(), int_1, arg64_1.i(), arg64_2.i(), int_2);
                    break;
                case RoadEvent::FormatDAAD+RoadEvent::FormatL+RoadEvent::FormatP2:
                    message.sprintf(e.eventFormat(), int_1, arg64_1.i(), e.arg2ToUtf8(arg64_2).constData(), int_2);
                    break;
                case RoadEvent::FormatDAAD+RoadEvent::FormatP+RoadEvent::FormatL2:
                    message.sprintf(e.eventFormat(), int_1, e.argToUtf8(arg64_1).constData(), arg64_2.i(), int_2);
                    break;
                case RoadEvent::FormatDAAD+RoadEvent::FormatP+RoadEvent::FormatP2:
                    message.sprintf(e.eventFormat(), int_1, e.argToUtf8(arg64_1).constData(), e.arg2ToUtf8(arg64_2).constData(), int_2);
                    break;

                case RoadEvent::FormatADDA+RoadEvent::FormatL+RoadEvent::FormatL2:
                    message.sprintf(e.eventFormat(), arg64_1.i(), int_1, int_2, arg64_2.i());
                    break;
                case RoadEvent::FormatADDA+RoadEvent::FormatL+RoadEvent::FormatP2:
                    message.sprintf(e.eventFormat(), arg64_1.i(), int_1, int_2, e.arg2ToUtf8(arg64_2).constData());
                    break;
                case RoadEvent::FormatADDA+RoadEvent::FormatP+RoadEvent::FormatL2:
                    message.sprintf(e.eventFormat(), e.argToUtf8(arg64_1).constData(), int_1, int_2, arg64_2.i());
                    break;
                case RoadEvent::FormatADDA+RoadEvent::FormatP+RoadEvent::FormatP2:
                    message.sprintf(e.eventFormat(), e.argToUtf8(arg64_1).constData(), int_1, int_2, e.arg2ToUtf8(arg64_2).constData());
                    break;

                case RoadEvent::FormatADAD+RoadEvent::FormatL+RoadEvent::FormatL2:
                    message.sprintf(e.eventFormat(), arg64_1.i(), int_1, arg64_2.i(), int_2);
                    break;
                case RoadEvent::FormatADAD+RoadEvent::FormatL+RoadEvent::FormatP2:
                    message.sprintf(e.eventFormat(), arg64_1.i(), int_1, e.arg2ToUtf8(arg64_2).constData(), int_2);
                    break;
                case RoadEvent::FormatADAD+RoadEvent::FormatP+RoadEvent::FormatL2:
                    message.sprintf(e.eventFormat(), e.argToUtf8(arg64_1).constData(), int_1, arg64_2.i(), int_2);
                    break;
                case RoadEvent::FormatADAD+RoadEvent::FormatP+RoadEvent::FormatP2:
                    message.sprintf(e.eventFormat(), e.argToUtf8(arg64_1).constData(), int_1, e.arg2ToUtf8(arg64_2).constData(), int_2);
                    break;


                case RoadEvent::FormatAADD+RoadEvent::FormatL+RoadEvent::FormatL2:
                    message.sprintf(e.eventFormat(), arg64_1.i(), arg64_2.i(), int_1, int_2);
                    break;
                case RoadEvent::FormatAADD+RoadEvent::FormatL+RoadEvent::FormatP2:
                    message.sprintf(e.eventFormat(), arg64_1.i(), e.arg2ToUtf8(arg64_2).constData(), int_1, int_2);
                    break;
                case RoadEvent::FormatAADD+RoadEvent::FormatP+RoadEvent::FormatL2:
                    message.sprintf(e.eventFormat(), e.argToUtf8(arg64_1).constData(), arg64_2.i(), int_1, int_2);
                    break;
                case RoadEvent::FormatAADD+RoadEvent::FormatP+RoadEvent::FormatP2:
                    message.sprintf(e.eventFormat(), e.argToUtf8(arg64_1).constData(), e.arg2ToUtf8(arg64_2).constData(), int_1, int_2);
                    break;

                default:
                    /*ThWarn(10935, "unknown...")*/ message.append("%1 \nTH10935: unknown argFormat (0x%2) for log message TH%3)").arg(e.eventFormat()).arg(format, 0, 16).arg(eventId());
            }
            result= (prefix + message).toUtf8();
        }catch(const RoadError &e){
            result= "Error reported in RoadLogEvent::toUtf8"; //!\todo Improved reporting of error while reporting an error.
            QString s= /*ThWarn(10936, "while...")*/ QString("TH10936 while reporting error TH%1, an Unreportable error (TH%2) was thrown by RoadLogEvent::toUtf8()").arg(QString::number(event_id)).arg(QString::number(e.roadLogEvent().eventId()));
            result= s.toUtf8();
        }
        catch(...){
            result= "Error reported in RoadLogEvent::toUtf8";
            QString s= /*ThWarn(10937, "while...")*/ QString("TH10937: while reporting error TH%1, an Unreportable error was thrown by RoadLogEvent::toUtf8()").arg(QString::number(event_id));
            result= s.toUtf8();
        }
    }
    if(!hasReturned()){
        event_message= result;
    }
    if(event_number>1300 && event_id==40378){
        return result; // debug trap -- or in ThLog
    }
    return result;
}//toUtf8

#//!\name Class functions

quint32 RoadLogEvent::
defaultEventId(const char *fmt)
{
    quint32 eventId= qHash(fmt);
    eventId |= IntHighBit;
    return eventId;
}//defaultEventId

}//namespace Thesa

