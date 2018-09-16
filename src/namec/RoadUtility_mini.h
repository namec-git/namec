/****************************************************************************
**
** Copyright (C) 2018-2018 C.B. Barber. All rights reserved.
** $Id: //main/2005/thid/src/namec/RoadUtility_mini.h#2 $$Change: 2480 $
** $Date: 2018/09/15 $$Author: bbarber $
**
****************************************************************************/

#ifndef Thesa_mini_H
#define Thesa_mini_H

#include "thidcore/thesa.h"
// Selected from thidcore/thesa.h and thidcore/predefined.h
//#include "road/RoadUtility.h"

#include <QtCore/QDateTime>
#include <QtCore/QStringList>
#include <QtCore/QtGlobal>

namespace Thesa {

//! Thesa_mini.cpp contains duplicated or shell code from the following classes

#//!\name Redefined classes to resolve external symbols
    class RoadUtility;

// from tnamespace.h
//    enum {
//        NoIndex=            -1,            //!< Not an index value 0..n-1
//        NoUtcTime= 0,  //!< If time not defined, use Jan 1, 1971 \todo Sets ThidData.text_modified_timed
//        InvalidUtcTime= 0xffffffff, //!< Returned by QDateTime.toTime_t
//    };

    typedef quint64         ThDiskBlockInt;         //!< ThDiskBlockId as an int
    typedef quint32         ThUtcTime;              //!< Will be qint64 someday.  Same as ThDiskBlock::modifiedTime_t()

class RoadUtility{

public:
    static const char * InvalidUtcTimeName; //!< "9999T99" for InvalidUtcTime;
    static const char * NoUtcTimeName; //!< "0000T00" for NoUtcTime;

#//!\name Constructors, etc.
                        RoadUtility() {}
                         ~RoadUtility() {} 
private:
                        Q_DISABLE_COPY(RoadUtility);
public:

    static QStringList  splitFirst(const QString &s, const QString &sep, int from= 0, Qt::CaseSensitivity cs= Qt::CaseSensitive);
    static QStringList  splitFirst(const QString &s, QChar ch, int from= 0, Qt::CaseSensitivity cs= Qt::CaseSensitive);
    static QStringList  splitLast(const QString &s, const QString &sep, int from= NoIndex, Qt::CaseSensitivity cs= Qt::CaseSensitive);
    static QStringList  splitLeftRight(const QString &s, int left, int right);
    static QString      toUtcTimestamp(ThUtcTime utc);

};//RoadUtility

#//!\name ThDiskBlockId subset
class ThDiskBlockId {

private:
    ThDiskBlockInt  block_id;   //!< Packed size and sector index

public:
};//ThDiskBlockId

#//!\name Methods
    int                 main(int argc, char *argv[]);

}//namespace Thesa

#endif Thesa_mini_H
