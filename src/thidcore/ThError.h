/****************************************************************************
**
** Copyright (C) 2010-2018 C.B. Barber. All rights reserved.
** $Id: //main/2005/thid/src/thidcore/ThError.h#26 $$Change: 2353 $
** $Date: 2018/02/15 $$Author: bbarber $
**
****************************************************************************/

#ifndef ThError_H
#define ThError_H

// Do not use thesa.h to avoid circular references
#include "road/RoadError.h"

#include <assert.h>
#include <stdexcept>
#include <string>
#include <QtCore/QString>

namespace Thesa {

#//!\name Defined here
    class ThError;

//! ThError -- std::exception class for Qhull
//! \see RoadEvent.h for formating codes and eventFlags
//! \see RoadEventStack for logPriority, etc.
class ThError : public RoadError {

public:
#//!\name Constants
    // Review unique ThError, ThCall, ThLog, ThWarn: cd $th/src && grep -E '[EhTh][rChW][raLa][olr][rlgn]\(110|\(10[0-9][0-9][0-9]|40[0-9][0-9][0-9]' */*.cpp *.cpp */*/*.cpp 2>/dev/null | sed -r -e 's/^[^(]*\(//' -e 's/^([0-9]+)[^0-9].*$/\1/' >x.x;
    enum {
        THIDfirstError= 1000,
        THIDlastError= 1050,  // Moved to Changes.txt
        NOthrow= 1
    };

#//!\name Constructors, etc.
    // default constructors
    ThError() : RoadError() {}
    ThError(const ThError &other) : RoadError(other) {}
    ThError(int code, const char *fmt) : RoadError(code, fmt) {}
    ThError(int code, const char *fmt, const QString &s) : RoadError(code, fmt, 0,0, RoadArg64(s)) {}
    ThError(int code, const char *fmt, int d) : RoadError(code, fmt, d) {}
    ThError(int code, const char *fmt, int d, int d2) : RoadError(code, fmt, d, d2) {}
    ThError(int code, const char *fmt, int d, int d2, RoadArg64 arg) : RoadError(code, fmt, d, d2, arg) {}
    ThError(int code, const char *fmt, int d, int d2, RoadArg64 arg, RoadArg64 arg2) : RoadError(code, fmt, d, d2, arg, arg2) {}
    ThError(int code, ThFlagsInt eventFlags, const char *fmt) : RoadError(code, eventFlags, fmt) {}
    ThError(int code, ThFlagsInt eventFlags, const char *fmt, const QString &s) : RoadError(code, eventFlags, fmt, 0,0, RoadArg64(s)) {}
    ThError(int code, ThFlagsInt eventFlags, const char *fmt, int d) : RoadError(code, eventFlags, fmt, d) {}
    ThError(int code, ThFlagsInt eventFlags, const char *fmt, int d, int d2) : RoadError(code, eventFlags, fmt, d, d2) {}
    ThError(int code, ThFlagsInt eventFlags, const char *fmt, int d, int d2, RoadArg64 arg) : RoadError(code, eventFlags, fmt, d, d2, arg) {}
    ThError(int code, ThFlagsInt eventFlags, const char *fmt, int d, int d2, RoadArg64 arg, RoadArg64 arg2) : RoadError(code, eventFlags, fmt, d, d2, arg, arg2) {}
    ~ThError() throw() {}

#//!\name Class functions
    //! Most ThError's are thrown.
    //! RoadError::Setting setting(RoadError::ErrorExpected);
    static void         warn(const ThError &e);
};//class ThError

}//namespace Thid

#//!\name Global functions

inline std::ostream &operator<<(std::ostream &os, const Thesa::ThError &e) { return os << e.what(); }

#endif ThError_H
