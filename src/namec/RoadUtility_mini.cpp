/****************************************************************************
**
** Copyright (C) 2018-2018 C.B. Barber. All rights reserved.
** $Id: //main/2005/thid/src/namec/RoadUtility_mini.cpp#2 $$Change: 2472 $
** $Date: 2018/09/07 $$Author: bbarber $
**
****************************************************************************/

#include "namec/RoadUtility_mini.h"

namespace Thesa{

#//!\name RoadUtility constants and methods

const char* RoadUtility::
InvalidUtcTimeName= "9999T99";

const char* RoadUtility::
NoUtcTimeName= "0000T00";

//! Split string by first occurrence of a QString
//! Returns a one element list on failure
QStringList RoadUtility::
splitFirst(const QString &s, const QString &sep, int from/*= 0*/, Qt::CaseSensitivity cs/*= Qt::CaseSensitive*/)
{
    int left= s.indexOf(sep, from, cs);
    int right= left+sep.length();
    return splitLeftRight(s, left, right);
}//splitFirst

//! Split string by first occurrence of a QChar
//! Returns a one element list on failure
QStringList RoadUtility::
splitFirst(const QString &s, QChar ch, int from/*= 0*/, Qt::CaseSensitivity cs/*= Qt::CaseSensitive*/)
{
    int left= s.indexOf(ch, from, cs);
    int right= left+1;
    return splitLeftRight(s, left, right);
}//splitFirst

//! Split string by last occurrence of a QString
//! Returns a one element list on failure
QStringList RoadUtility::
splitLast(const QString &s, const QString &sep, int from/*= NoIndex*/, Qt::CaseSensitivity cs/*= Qt::CaseSensitive*/)
{
    int left= s.lastIndexOf(sep, from, cs);
    int right= left+sep.length();
    return splitLeftRight(s, left, right);
}//splitLast

//! Split string into left and right portions
//! \arg left is count of characters
//! Returns a one element list if \arg left<0
//! \arg right is starting index
QStringList RoadUtility::
splitLeftRight(const QString &s, int left, int right)
{
    QStringList result;
    if(left<0){
        result.append(s);
    }else{
        result.append(s.left(left));
        int remainder= s.length()-right;
        if(remainder>=0){
            result.append(s.right(remainder));
        }
    }
    return result;
}//splitLeftRight

//! \see currentUtcTime
QString RoadUtility::
toUtcTimestamp(ThUtcTime utc)
{
    if(utc==NoUtcTime){
        return QString(RoadUtility::NoUtcTimeName);
    }else if(utc==InvalidUtcTime){
        return QString(RoadUtility::InvalidUtcTimeName);
    }
    QString timestamp= QDateTime::fromTime_t(utc, Qt::UTC).toString("yyyyMMdd'T'hhmmss");
    return timestamp;
}//toUItcTimestamp Utc

}//namespace Thesa


