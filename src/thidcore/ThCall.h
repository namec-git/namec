/****************************************************************************
**
** Copyright (C) 2013-2018 C.B. Barber. All rights reserved.
** $Id: //main/2005/thid/src/thidcore/ThCall.h#14 $$Change: 2464 $
** $Date: 2018/08/27 $$Author: bbarber $
**
****************************************************************************/

#ifndef ThCall_H
#define ThCall_H

// Do not use thesa.h to avoid circular references
#include "road/RoadLogEvent.h"

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

namespace Thesa {

#//!\name Defined here
    //! ThCall -- Report and log errors
    //!  See discussion in Saylan, G., "Practical C++ error handling in hybrid environments," Dr. Dobb's Journal, p. 50-55, March 2007.
    //!   He uses an auto_ptr to track a stringstream.  It constructs a string on the fly.  ThCall uses the copy constructor to transform RoadLogEvent into a string
    class ThCall;

class ThCall {
#//!\name Defined here
public:

private:
    RoadLogEvent        log_event;   //! Format string w/ arguments
public:

#//!\name Constructors, etc.
    ThCall();
    ThCall(int code, const char *fmt);
    ThCall(int code, const char *fmt, const QString &s);
    ThCall(int code, const char *fmt, int d);
    ThCall(int code, const char *fmt, int d, int d2);
    ThCall(int code, const char *fmt, int d, int d2, RoadArg64 arg);
    ThCall(int code, const char *fmt, int d, int d2, RoadArg64 arg, RoadArg64 arg2);
    ThCall(int code, ThFlagsInt eventFlags, const char *fmt);
    ThCall(int code, ThFlagsInt eventFlags, const char *fmt, const QString &s);
    ThCall(int code, ThFlagsInt eventFlags, const char *fmt, int d);
    ThCall(int code, ThFlagsInt eventFlags, const char *fmt, int d, int d2);
    ThCall(int code, ThFlagsInt eventFlags, const char *fmt, int d, int d2, RoadArg64 arg);
    ThCall(int code, ThFlagsInt eventFlags, const char *fmt, int d, int d2, RoadArg64 arg, RoadArg64 arg2);

    ThCall(const ThCall &other);  //! Called on throw, sets event_message
    ThCall &            operator=(const ThCall &other);
                        ~ThCall();

#//!\name GetSet
    bool                isDefined() const { return log_event.isInitialized(); }
    int                 errorCode() const { return log_event.eventId(); };
   //!\todo QH11021 should ThCall provide errorMessage.  Currently \ref what
    RoadLogEvent        roadLogEvent() const { return log_event; };

#//!\name Methods
    void                calls(int code, const char *fmt);
    void                calls(int code, const char *fmt, const QString &s);
    void                calls(int code, const char *fmt, int d);
    void                calls(int code, const char *fmt, int d, int d2);
    void                calls(int code, const char *fmt, int d, int d2, RoadArg64 arg);
    void                calls(int code, const char *fmt, int d, int d2, RoadArg64 arg, RoadArg64 arg2);
    void                calls(int code, ThFlagsInt eventFlags, const char *fmt);
    void                calls(int code, ThFlagsInt eventFlags, const char *fmt, const QString &s) { calls(code, eventFlags, fmt, 0,0, RoadArg64(s)); }
    void                calls(int code, ThFlagsInt eventFlags, const char *fmt, int d);
    void                calls(int code, ThFlagsInt eventFlags, const char *fmt, int d, int d2);
    void                calls(int code, ThFlagsInt eventFlags, const char *fmt, int d, int d2, RoadArg64 arg);
    void                calls(int code, ThFlagsInt eventFlags, const char *fmt, int d, int d2, RoadArg64 arg, RoadArg64 arg2);
    void                returns() {}
    void                returns(int code, const char *fmt) { returns(code, 0, fmt); }
    void                returns(int code, const char *fmt, const QString &s) { returns(code, fmt, 0,0, RoadArg64(s)); }
    void                returns(int code, const char *fmt, int d) { returns(code, 0, fmt, d); }
    void                returns(int code, const char *fmt, int d, int d2) { returns(code, 0, fmt, d, d2); }
    void                returns(int code, const char *fmt, int d, int d2, RoadArg64 arg) { returns(code, 0, fmt, d, d2, arg); }
    void                returns(int code, const char *fmt, int d, int d2, RoadArg64 arg, RoadArg64 arg2) { returns(code, 0, fmt, d, d2, arg, arg2); }
    void                returns(int code, ThFlagsInt eventFlags, const char *fmt);
    void                returns(int code, ThFlagsInt eventFlags, const char *fmt, const QString &s) { returns(code, eventFlags, fmt, 0,0, RoadArg64(s)); }
    void                returns(int code, ThFlagsInt eventFlags, const char *fmt, int d);
    void                returns(int code, ThFlagsInt eventFlags, const char *fmt, int d, int d2);
    void                returns(int code, ThFlagsInt eventFlags, const char *fmt, int d, int d2, RoadArg64 arg);
    void                returns(int code, ThFlagsInt eventFlags, const char *fmt, int d, int d2, RoadArg64 arg, RoadArg64 arg2);
    void                trapCall(const char *fmt);
    void                trapReturn(RoadLogEvent *e, const char *fmt);
    const char *        what() const;

};//class ThCall

}//namespace Thesa

#//!\name Global functions

#endif ThCall_H
