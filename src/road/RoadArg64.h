/****************************************************************************
**
** Copyright (C) 2013-2018 C.B. Barber. All rights reserved.
** $Id: //main/2005/thid/src/road/RoadArg64.h#23 $$Change: 2480 $
** $Date: 2018/09/15 $$Author: bbarber $
**
****************************************************************************/

#ifndef RoadArg64_H
#define RoadArg64_H

// Do not include Thesa code.  Pass as pointer instead.  Otherwise circular references through thesa.h

#include <QtCore/QPointer>

#//!\name Used here
    class QString;
    class QVariant;

namespace Thesa {

#//!\name Defined here
    //! RoadLogEvent -- Record an event for the RoadLog
    class RoadArg64;

#//!\name Used here
    class ThDiskBlockId;
    class Thid;
    class ThidId;
    class ThidId64;
    class ThidInfo;
    class ThModelId;
    class ThXrefSignature;
    typedef quint32 ThUtcTime;  // copied from tnamespace.h

class RoadArg64 {

#//!\name Fields
public:
    enum RoadArg64Flags : qint32 {
        ArgQString= -1,     //!< string_length flag for QString *
        ArgQByteArray= -2,  //!< string_length flag for QByteArray *
        ArgQVariant= -3,    //!< string_length flag for QVariant *
        ArgThid= -4,        //!< string_length flag for Thid *
        ArgThidId64= -5,    //!< string_length flag for ThidId64 (as quint64)
        ArgUtcTime= -6,     //!< string_length flag for RoadUtility::toUtcTimestamp(ThUtcTime t)
        ArgThXrefSignature= -7, //!< string_length flag for ThXrefSignature
        ArgThDiskBlockId= -8,   //!< string_length flag for ThXrefDiskBlockId
        ArgThModelId= -9,   //!< string_length flag for ThModelId
        // Last entry
        ArgVoidStar= -20,   //!< negative count is its length in bytes

        DefaultMaximumStringLength= 30,
    };//RoadArg64Flags

    typedef union {             
        const char *    s;   //!< Cstr argument (%s) -- Saved data with quint32 count (default is null-terminated)
                             //!< string_length==1 if empty string
        const void *    p;   //!< Void* argument (%llx) -- Pointer without data
        qint64          i;   //!< signed int64 (%lli, %llu)
        quint64         u;   //!< unsigned int64 (%lli, %llu).  Ambiguous if signed is also defined.
        double          d;   //!< Double argument (%e)
        const QString * qs;  //!< QString argument \todo Not automatically ref copied, cannot store classes with initializers
        const QByteArray * qb;  //!< QByteArray argument \todo Not automatically ref copied, cannot store classes with initializers
        const QVariant * qv;  //!< QVariant argument \todo Not automatically ref copied
        Thid *          thid;//!< Thid argument \todo Interaction with removing thids
        quint64         id64;//!< ThidId64 argument \todo Interaction with removing thids
    } Union64;

#//!\name Fields
private:
    int                 string_length;      //! 0 if no data, n+1 if null-terminated string, -n for data
    Union64             union_64; // Keep last for msdev 2005 debug display 

#//!\name Class globals
    static int          maximum_string_length;      
public:

#//!\name Constructors with automatic conversion
                        RoadArg64() : string_length(0) { union_64.i= 0; }
                        RoadArg64(const char *s) : string_length(RoadArg64::argumentStringLength(s)+1) { union_64.u= (quint64)s; } // 0-extend to 64-bits on 32-bit host (for %llx)
                        RoadArg64(const void *p) : string_length(0) { union_64.u= (quint64)p; }
                        RoadArg64(double d) : string_length(0) { union_64.d= d; }
                        RoadArg64(int i) : string_length(0) { union_64.i= i; }
                        RoadArg64(qint64 i) : string_length(0) { union_64.i= i; }
                        RoadArg64(quint64 u) : string_length(0) { union_64.u= u; }
                        RoadArg64(const QByteArray &qb) : string_length(ArgQByteArray) { union_64.u= (quint64)&qb; }
                        RoadArg64(const QString &qs) : string_length(ArgQString) { union_64.u= (quint64)&qs; } 
                        RoadArg64(const QVariant &qv) : string_length(ArgQVariant) { union_64.u= (quint64)&qv; }

                        RoadArg64(const Thid *thid) : string_length(ArgThid) { union_64.u= (quint64)thid; }
                        RoadArg64(Thid *thid) : string_length(ArgThid) { union_64.u= (quint64)thid; }
                        RoadArg64(const ThUtcTime &t) : string_length(ArgUtcTime) { union_64.u= t; }

                        // Define in cpp file to avoid #include
#ifndef TH_NO_THID_ID
                        RoadArg64(const ThDiskBlockId &b);  
                        RoadArg64(QPointer<Thid> thid);
                        RoadArg64(const ThidId &id);
                        RoadArg64(const ThidId64 &id64);
                        RoadArg64(const ThidInfo &info);
                        RoadArg64(const ThModelId &m);
                        RoadArg64(const ThXrefSignature &x);
#endif
                        RoadArg64(const RoadArg64 &other) : string_length(other.string_length) { union_64.i= other.union_64.i; }
                        RoadArg64(RoadArg64Flags type, qint32 i) : string_length(type) { union_64.i= i; } 
                        RoadArg64(RoadArg64Flags type, quint32 u) : string_length(type) { union_64.u= u; }
                        RoadArg64(RoadArg64Flags type, qint64 i) : string_length(type) { union_64.i= i; }
                        RoadArg64(RoadArg64Flags type, quint64 u) : string_length(type) { union_64.u= u; }
    RoadArg64 &         operator=(const RoadArg64 &other);
                        ~RoadArg64() {}

#//!\name GetSet
    const char *        s() const { return union_64.s; } 
    const void *        p() const { return union_64.p; }
    qint64              i() const { return union_64.i; }
    quint64             u() const { return union_64.u; }
    double              d() const { return union_64.d; }
    bool                hasToUtf8() const { return string_length!=0; }
    void                set(const char *s) { union_64.s= s; }
    void                set(const void *p) { union_64.p= p; }
    void                set(int i) { union_64.i= i; }
    void                set(qint64 i) { union_64.i= i; }
    void                set(quint64 u) { union_64.u= u; }
    void                set(double d) { union_64.d= d; }
    int                 cStringLength() const { return string_length-1; } //!< n+1 for null-terminated string.  -n for types or void*data

#//!\name Methods
    QByteArray          toUtf8() const;

#//!\name Class Methods
    static int          argumentStringLength(const char *s);

};//class RoadArg64

}//namespace Thesa

#endif RoadArg64_H
