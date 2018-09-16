/****************************************************************************
**
** Copyright (C) 2013-2018 C.B. Barber. All rights reserved.
** $Id: //main/2005/thid/src/road/RoadArg64.cpp#25 $$Change: 2480 $
** $Date: 2018/09/15 $$Author: bbarber $
**
****************************************************************************/

#include "thidcore/thesa.h"

#include "road/RoadArg64.h"

#ifndef TH_NO_THID_ID
#include "thidcore/Thid.h"
#include "thidcore/ThidId64.h"
#include "thidcore/ThidInfo.h"
#include "thiddata/ThModelId.h"
#include "thidxref/ThXrefSignature.h"
#endif

#include <QtCore/QVariant>

namespace Thesa {

#//!\name Class globals
int RoadArg64::
maximum_string_length= DefaultMaximumStringLength;

#//!\name Constructors

#ifndef TH_NO_THID_ID
RoadArg64::
RoadArg64(const ThDiskBlockId &b)
: string_length(ArgThDiskBlockId)
{
    union_64.u= b.toDiskBlockInt();
}//ThDiskBlockId

RoadArg64::
RoadArg64(QPointer<Thid> thid)
: string_length(ArgThid)
{
    union_64.u= (quint64)thid.data();
}//Thid

RoadArg64::
RoadArg64(const ThidId &id) 
: string_length(ArgThidId64) 
{ 
    if(id.isThidId64()){
        ThidId64 id64= id.toThidId64();
        union_64.id64= id64.toUint64(); // ArgThidId64;
    }else{
        QString s= id.toString();
        string_length= ArgQString;
        union_64.u= (quint64)&s; 
    }
}//ThidId

RoadArg64::
RoadArg64(const ThidId64 &id64) 
: string_length(ArgThidId64) 
{ 
    union_64.id64= id64.toUint64(); 
}//ThidId64

RoadArg64::
RoadArg64(const ThidInfo &info) 
: string_length(ArgThDiskBlockId) 
{ 
    ThDiskBlockId b= info.thidBlockId();
    if(b.isValid()){
        union_64.u= b.toDiskBlockInt(); // ArArgThDiskBlockIdgThidId64;
    }else{
        ThidId id= info.thidId();
        if(id.isThidId64()){
            string_length= ArgThidId64;
            ThidId64 id64= id.toThidId64();
            union_64.id64= id64.toUint64(); 
        }else if(id.isValid()){
            string_length= ArgQString;
            QString s= id.toString();
            union_64.u= (quint64)&s; 
        }else{
            string_length= ArgQString;
            QString s= info.title();
            union_64.u= (quint64)&s; 
        }
    }
}//ThidInfo

RoadArg64::
RoadArg64(const ThModelId &m)
: string_length(ArgThModelId)
{
    union_64.u= m.toFlagsInt();
}//ThModelId

RoadArg64::
RoadArg64(const ThXrefSignature &x)
: string_length(ArgThXrefSignature)
{
    union_64.u= x.signature();
}//ThXrefSignature
#endif

// Quiet warnings about empty class
RoadArg64 & RoadArg64::
operator=(const RoadArg64 &other)
{
    string_length= other.string_length;
    union_64.i= other.union_64.i;
    return *this;
}//operator=

#//!name Methods

QByteArray RoadArg64::
toUtf8() const
{
    if(string_length>0){
        int cLen= cStringLength();
        if(union_64.s[cLen]=='\0'){
            return QByteArray::fromRawData(union_64.s, cLen);
        }else{
            return QByteArray(union_64.s, cLen); //! Adds null-terminator
        }
    }else if(string_length<ArgVoidStar){
        int n= -(string_length-ArgVoidStar);
        if(union_64.s[n]=='\0'){
            return QByteArray::fromRawData(union_64.s, n);
        }else{
            return QByteArray(union_64.s, n); //! Adds null-terminator
        }
    }else switch(string_length){
        case ArgQString:
            return union_64.qs->toUtf8();
        case ArgQByteArray:
            return *union_64.qb;
        case ArgQVariant:
            return union_64.qv->toString().toUtf8();  //!\todo Round-trip from RoadArg64 > QString > Utf8 is expensive, mainly due to memory
#ifndef TH_NO_THID_ID
        case ArgUtcTime:
            return RoadUtility::toUtcTimestamp(ThUtcTime(union_64.u)).toUtf8(); //<!\FIXUP How to include ArgUtcTime in namec build.
        case ArgThid:
            if(union_64.thid){
                return union_64.thid->toString().toUtf8();
            }
            return QByteArray("NULL");
        case ArgThidId64:
            return ThidId64(union_64.id64).toString().toUtf8();
        case ArgThDiskBlockId:
            return ThDiskBlockId(union_64.u).toString().toUtf8();
        case ArgThModelId:
            return ThModelId(union_64.u).toString().toUtf8();
        case ArgThXrefSignature:
            return ThXrefSignature(quint32(union_64.u)).toString().toUtf8();
#endif
    }
    return /*ThWarn(10938, "Unknown ...")*/ QString("TH10938: Unknown RoadArg64Flag (%1) for value 0x%2").arg(string_length).arg(union_64.i, 0, 16).toUtf8();
}//toUtf8

#//!\name Class Methods

//! Return usable length of a char pointer [static]
int RoadArg64::
argumentStringLength(const char *s)
{
    const char *t= s;
    const char *u= s+RoadArg64::maximum_string_length;
    try{
        while(*t!='\0' && t<u){
            ++t;
        }
    }catch(...){
        if(t>s){
            --t; // On segment faults, the previous value was OK.
        }
    }
    return int(t-s);
}//argumentStringLength

}//namespace Thesa

