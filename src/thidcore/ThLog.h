/****************************************************************************
**
** Copyright (C) 2013-2018 C.B. Barber. All rights reserved.
** $Id: //main/2005/thid/src/thidcore/ThLog.h#12 $$Change: 2464 $
** $Date: 2018/08/27 $$Author: bbarber $
**
****************************************************************************/

#ifndef ThLog_H
#define ThLog_H

// Do not use thesa.h to avoid circular references
#include "road/RoadLogEvent.h"

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

namespace Thesa {

#//!\name Defined here
    //! ThLog -- Report and log errors
    //!  See discussion in Saylan, G., "Practical C++ error handling in hybrid environments," Dr. Dobb's Journal, p. 50-55, March 2007.
    //!   He uses an auto_ptr to track a stringstream.  It constructs a string on the fly.  ThLog uses the copy constructor to transform RoadLogEvent into a string
    class ThLog;

class ThLog {
#//!\name Defined here
public:

private:
    RoadLogEvent        log_event;   //! Format string w/ arguments
public:

#//!\name Constructors, etc.
    ThLog();
    ThLog(int code, const char *fmt);
    ThLog(int code, const char *fmt, const QString &s);
    ThLog(int code, const char *fmt, int d);
    ThLog(int code, const char *fmt, int d, int d2);
    ThLog(int code, const char *fmt, int d, int d2, RoadArg64 arg);
    ThLog(int code, const char *fmt, int d, int d2, RoadArg64 arg, RoadArg64 arg2);
    ThLog(int code, ThFlagsInt eventFlags, const char *fmt);
    ThLog(int code, ThFlagsInt eventFlags, const char *fmt, const QString &s);
    ThLog(int code, ThFlagsInt eventFlags, const char *fmt, int d);
    ThLog(int code, ThFlagsInt eventFlags, const char *fmt, int d, int d2);
    ThLog(int code, ThFlagsInt eventFlags, const char *fmt, int d, int d2, RoadArg64 arg);
    ThLog(int code, ThFlagsInt eventFlags, const char *fmt, int d, int d2, RoadArg64 arg, RoadArg64 arg2);

    ThLog(const ThLog &other);  //! Called on throw, sets event_message
    ThLog &             operator=(const ThLog &other);
                        ~ThLog() {}

#//!\name GetSet
    bool                isDefined() const { return log_event.isInitialized(); }
    int                 errorCode() const { return log_event.eventId(); };
   //!\todo QH11021 should ThLog provide errorMessage.  Currently \ref what
    RoadLogEvent        roadLogEvent() const { return log_event; };

#//!\name Methods
    void                trapLog(const char *fmt);
    const char *        what() const;

};//class ThLog

}//namespace Thesa

#endif ThLog_H
