/****************************************************************************
**
** Copyright (C) 2013-2018 C.B. Barber. All rights reserved.
** $Id: //main/2005/thid/src/road/RoadEvent.h#22 $$Change: 2390 $
** $Date: 2018/04/27 $$Author: bbarber $
**
****************************************************************************/

#ifndef RoadEvent_H
#define RoadEvent_H

// Do not use thesa.h to avoid circular references

#include "road/RoadArg64.h"
#include "thidcore/tnamespace.h"

namespace Thesa {

#//!\name Defined here
    //! RoadEvent -- Record information about an event
    class RoadEvent;

enum RoadEventGlobals : ThFlagsInt {
    // 0xff reserved by RoadEvent::RoadEventFlags
    LogDisk= 0x0000100,
    LogOK=   0x0000200,
};

class RoadEvent {
public:
    //! Categorize args for format string. Uses a C string (*char) with C's printf format types  \see RoadEvent::argFormat and RoadLogEvent::toUtf8
    enum RoadEventFormats : quint32 {
        FormatDD=   0x00,       //!< Format arguments. DD is default (isFormatDD)
        FormatDDAA= 0x01,       //!<   D= 'd', 'u', 'X', 'c', 'o', 'X' is hexadecimal
        FormatDADA= 0x02,       //!<  A= everything else (see L and P below)
        FormatDAAD= 0x03,
        FormatADDA= 0x04,
        FormatADAD= 0x05,
        FormatAADD= 0x06,
        FormatL=    0x00,        //!<   L= 64-bit 'i', 'x', 'e', 'f', 'g', 'E', 'G'
        FormatP=    0x10,        //!<   P= pointer 'p', 's' -- goes through argToUtf8()
        FormatL2=   0x00,       
        FormatP2=   0x40,        //!<   including QString, QVariant, Thid, and ThidId64
        MaxPrintable= 200,      //!< Maximum characters for logging tuple.row();
    };//!<RoadEventFormats

    enum RoadEventFlags : quint32 {
        IsFormatDD=     0x00000001, // is default, FormatDDAA
        EventTypeMask=  0x0000000e,     //!< See EventTypeFlags for one-letter codes
         LogEvent=      0x00000002,
         CallEvent=     0x00000004,
         ErrorEvent=    0x00000006,
         WarnEvent=     0x00000008,
         ReturnEvent=   0x0000000a,
        EventPriorityMask=  0x00000070,     //!<
         EventPriorityShift= 4,         //!< Convert logLevel to logPriority
         Always=        0x00000000,     //!< Always log.  Events in real time.
         Steps=         0x00000010,     //!< Log steps
         Normal=        0x00000020,     //!< Log events without overhead
         Detail=        0x00000030,     //!< Log details despite some overhead
         Detail2=       0x00000040,     //!< Log method details for troubleshooting
         Select=        0x00000070,     //!< Log if selected event (e.g., allocate/deallocate)
        IsSpecial=      0x00000080,     //!< Indicate a special event with a [*nnn] suffix (e.g., selected for ThSettings::LogBlockIds)
        Reserved=       0xffffff00,     //!< Reserved flags for RoadLogEvent -- not used \todo Rename, this is a mask.  Maybe event labels should be global symbols
        MaxFormatLength= 10000,    // Maximum length of event_format

        PrefixChar=         ':',            //! Marks end of prefix for event_format;
    };//RoadEventFlags

    typedef QByteArray (*roadArgToUtf8Function)(const RoadArg64 &arg);  // roadArgToUtf8Function

private:
    const char *        event_format;  //! Null-terminated, literal format string for printf 
    roadArgToUtf8Function arg_to_utf8;  //! Convert Arg1 to QString
    roadArgToUtf8Function arg2_to_utf8;
    quint8              event_priority;
    quint32             event_flags;

#//Class fields
public:
    static const char  *EventTypeFlags;     //! EventType single-character flags for RoadLogEvent::toUtf8 (e.g., 'L' for ThLog)

#//!\name Constructors, etc.
                        RoadEvent() : event_format(NULL), arg_to_utf8(NULL), arg2_to_utf8(NULL), event_priority(0), event_flags(0) {}
                        RoadEvent(const char *fmt, quint32 eventFlags) : event_format(fmt), arg_to_utf8(NULL), arg2_to_utf8(NULL), event_priority(0), event_flags(eventFlags) {}
                        RoadEvent(const RoadEvent &other);  
    RoadEvent &         operator=(const RoadEvent &other);
                        ~RoadEvent() { }

#//!\name GetSet
    ThFlagsInt          eventFlags() const { return event_flags; }
    const char *        eventFormat() const { return event_format; } // for QString::sprintf
    ThFlagsInt          eventPriority() const { return event_flags&EventPriorityMask; }
    ThFlagsInt          eventType() const { return event_flags&EventTypeMask; }
    bool                hasToString() const { return arg_to_utf8!=NULL; }
    bool                hasToString2() const { return arg2_to_utf8!=NULL; }
    bool                isFormatDD() const { return event_flags&IsFormatDD; }
    bool                isSpecial() const { return event_flags&IsSpecial; }

#//!\name Methods
    quint32             argFormat(const RoadArg64 &arg, const RoadArg64 &arg2) const;
    QByteArray          argToUtf8(const RoadArg64 &arg) const;
    QByteArray          arg2ToUtf8(const RoadArg64 &arg2) const;

#//!\name Class Methods
    static const RoadEvent & event(ThLocalInt eventId);
    static bool         isDeselect(ThLocalInt eventId);
    static bool         isLogging(ThLocalInt eventId, quint32 eventFlags);
    static bool         isOutput();
    static bool         isSelected(ThLocalInt eventId);
    static void         setIsDeselect(ThLocalInt eventId);
    static void         setIsSelected(ThLocalInt eventId);

};//class RoadEvent

}//namespace Thesa

#endif RoadEvent_H
