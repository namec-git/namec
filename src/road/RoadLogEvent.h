/****************************************************************************
**
** Copyright (C) 2008-2018 C.B. Barber. All rights reserved.
** $Id: //main/2005/thid/src/road/RoadLogEvent.h#30 $$Change: 2434 $
** $Date: 2018/07/08 $$Author: bbarber $
**
****************************************************************************/

#ifndef RoadLogEvent_H
#define RoadLogEvent_H

// Do not use thesa.h to avoid circular references
#include "thidcore/tnamespace.h"
#include "thidcore/tglobal.h"

#include "road/RoadArg64.h"
#include "road/RoadEvent.h"

#include <QtCore/QGlobalStatic>

#include <ostream>
#include <stdexcept>
#include <string>

namespace Thesa {

#//!\name Defined here
    //! RoadLogEvent -- Record an event for the RoadLog
    class RoadLogEvent;

class RoadLogEvent {

public:
enum RoadLogEventFlags : quint32 {      //!<\todo Split out private enums from public such as PreviousEventLevel?

    SizeThread=         0x00000003,   //!< Number of bytes in thread_id          
    IsLevel2=           0x00000004,   //!< One or two bytes in event_level \todo Not used.  Allow two byte event_levels
    SizeId=             0x00000018,   //!< Number-1 of bytes in event_id
    SizeIdShift=        3,
    IsTime4=            0x00000020,   //!< 2 or 4 bytes in mod16_seconds
    SizeInt=            0x00000060,   //!< Number-1 of bytes in int_1, low-endian
    SizeIntShift=       5,
    SizeInt2=           0x00000180,   //!< Number-1 of bytes in int_2
    SizeInt2Shift=      7,
    SizeArg=            0x00000e00,   //!< Count code for arg_1 (ThDiskBlockRaw::THidSizeCodeToBytes, 7=quint32 count)
    SizeArgShift=       9,
    SizeArg2=           0x00007000,   //!< Count code for arg_2
    SizeArg2Shift=      12,
    ArgString=           7,            //!< Count code for quint32 count, low-endian, followed by utf8 bytes
    ArgCount=           0x00018000,   //!< Count-1 of arguments 
    ArgCountShift=      15,
    HasReturned=        0x00020000,     //!< Set when RoadLogEvent has returned
    IgnoreReturn=       0x00040000,     //!< Ignore immediate return from a ThCall \todo Can be determined from RoadEventStack at ThCall::trapReturn
    PreviousEventLevel= 0x80000000,     //!< Record previous eventLevel for a return event

    MaximumEventLevel=  MaximumQuint8,  //!< MaximumEventLevel for event_level -- updated by RoadEventStack::event_level
};//RoadLogEventFlags

#//!\name Fields
private:
    quint32             event_id;   //! Index into log_events for format_string and priority_flags
    quint32             event_number; //! Number of this event, by default, increments
    //!<\todo 64-bit event_number?  Or use event_milliseconds to order in time
    int                 thread_id;   //! Thread and event loop identifier, context for this event (isInitialized>=0)
    quint8              event_level; //! Level of event in call stack for this thread id (\see MaximumEventLevel) 
    quint64             event_milliseconds;  //! UTC milliseconds
    int                 event_size;  //! Number of bytes in this log event
    quint32             log_event_flags;   //! Defined above
    int                 int_1;       //! Integer argument (%d, for logging)
    int                 int_2;       //! Integer argument (%d, for logging)
    RoadArg64           arg64_1;     //! May include pointer into log buffer
    RoadArg64           arg64_2;
    mutable QByteArray  event_message; //! UTF8 formatted QByteArray from toUtf8()

#//!\name Class globals
    static quint64      previous_event_milliseconds;

#//!\name Constants
public:
    static const int    EventNumberBase= 10;         //!< Base for event_number in toUtf8()
    static const int    MinimumEventNumberWidth= 6;  //!< Minimum width for event_number in toUtf8()

public:
#//!\name Constructors, etc.
    RoadLogEvent();
    RoadLogEvent(quint32 eventId);
    RoadLogEvent(quint32 eventId, int d);
    RoadLogEvent(quint32 eventId, int d, int d2);
    RoadLogEvent(quint32 eventId, int d, int d2, RoadArg64 arg);
    RoadLogEvent(quint32 eventId, int d, int d2, RoadArg64 arg, RoadArg64 arg2);
    RoadLogEvent(quint32 eventId, quint32 eventFlags);
    RoadLogEvent(quint32 eventId, quint32 eventFlags, int d);
    RoadLogEvent(quint32 eventId, quint32 eventFlags, int d, int d2);
    RoadLogEvent(quint32 eventId, quint32 eventFlags, int d, int d2, RoadArg64 arg);
    RoadLogEvent(quint32 eventId, quint32 eventFlags, int d, int d2, RoadArg64 arg, RoadArg64 arg2);
    RoadLogEvent(const RoadLogEvent &other);
    RoadLogEvent &      operator=(const RoadLogEvent &other);
    ~RoadLogEvent() {};

#//!\name GetSet
    const char *        cstr1() const { return arg64_1.s(); } //!< utf8 via QString::sprintf \todo Rename to utf8?
    double              double1() const { return arg64_1.d(); }
    quint32             eventId() const { return event_id; }
    int                 eventLevel() const { return event_level; }
    quint32             eventFlags() const { return log_event_flags; }
    quint32             eventNumber() const { return event_number; }
    float               float1() const { return arg64_1.d(); }
    const char *        format() const;
    bool                hasReturned() const { return log_event_flags&HasReturned; }
    bool                ignoreReturn() const { return log_event_flags&IgnoreReturn; }
    void                initializeEvent(const char *fmt);
    bool                isInitialized() const { return thread_id>=0; }
    bool                isValid() const { return event_id!=0; }
    int                 int1() const { return int_1; }
    int                 int2() const { return int_2; }
    long long           int64() const { return arg64_1.i(); }
    quint64             millisecondsUtc() const { return event_milliseconds; } //!<\todo should millisecondsUtc have a type?
    void                setEventPriority(ThFlagsInt i) { log_event_flags &= ~RoadEvent::EventPriorityMask; log_event_flags |= i&RoadEvent::EventPriorityMask; }
    void                setEventType(ThFlagsInt i);
    void                setHasReturned() { log_event_flags |= HasReturned; }
    void                setIgnoreReturn() { log_event_flags |= IgnoreReturn; }
    const void *        void1() const { return arg64_1.p(); }

#//!\name Methods
    void                maybeSetEventMessage();
    void                updateEventNumberTime();
    QByteArray          toUtf8(const char *tag= NULL) const { RoadLogEvent other; return toUtf8(other, tag); } 
    QByteArray          toUtf8(const RoadLogEvent &other, const char *tag= NULL) const; //!< Include ThError::ROADtag for eventId

#//!\name Class functions
    static quint32      defaultEventId(const char *fmt);
    static bool         isDefaultEventId(quint32 eventId) { return eventId&IntHighBit; }

#//!\name Class helpers
private:
    static bool         firstExtraCode(std::ostream &os, char c, char *extraCode);


};//class RoadLogEvent

}//namespace Thesa

#endif RoadLogEvent_H
