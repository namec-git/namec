/****************************************************************************
**
** Copyright (C) 2013-2018 C.B. Barber. All rights reserved.
** $Id: //main/2005/thid/src/thidcore/ThWarn.h#11 $$Change: 2464 $
** $Date: 2018/08/27 $$Author: bbarber $
**
****************************************************************************/

#ifndef ThWarn_H
#define ThWarn_H

// Do not use thesa.h to avoid circular references
#include "road/RoadLogEvent.h"

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

namespace Thesa {

#//!\name Defined here
    //! ThWarn -- Report and log warnings
    class ThWarn;

class ThWarn {
#//!\name Defined here
public:

private:
    RoadLogEvent        log_event;   //! Format string w/ arguments
public:

#//!\name Constructors, etc.
    ThWarn();
    ThWarn(int code, const char *fmt);
    ThWarn(int code, const char *fmt, const QString &s);
    ThWarn(int code, const char *fmt, int d);
    ThWarn(int code, const char *fmt, int d, int d2);
    ThWarn(int code, const char *fmt, int d, int d2, RoadArg64 arg);
    ThWarn(int code, const char *fmt, int d, int d2, RoadArg64 arg, RoadArg64 arg2);
    ThWarn(int code, ThFlagsInt eventFlags, const char *fmt);
    ThWarn(int code, ThFlagsInt eventFlags, const char *fmt, const QString &s);
    ThWarn(int code, ThFlagsInt eventFlags, const char *fmt, int d);
    ThWarn(int code, ThFlagsInt eventFlags, const char *fmt, int d, int d2);
    ThWarn(int code, ThFlagsInt eventFlags, const char *fmt, int d, int d2, RoadArg64 arg);
    ThWarn(int code, ThFlagsInt eventFlags, const char *fmt, int d, int d2, RoadArg64 arg, RoadArg64 arg2);

                        ThWarn(const ThWarn &other); 
    ThWarn &            operator=(const ThWarn &other);
                        ~ThWarn() {}

#//!\name GetSet
    bool                isDefined() const { return log_event.isInitialized(); }
    int                 errorCode() const { return log_event.eventId(); };
   //!\todo QH11021 should ThWarn provide errorMessage.  Currently \ref what
    RoadLogEvent        roadLogEvent() const { return log_event; };

#//!\name Methods
    void                trapWarn(const char *fmt);
    const char *        what() const;

};//class ThWarn

}//namespace Thesa

#//!\name Global functions

#endif ThWarn_H
