/****************************************************************************
**
** Copyright (C) 2013-2018 C.B. Barber. All rights reserved.
** $Id: //main/2005/thid/src/road/RoadEventStack.h#22 $$Change: 2450 $
** $Date: 2018/07/25 $$Author: bbarber $
**
****************************************************************************/

#ifndef RoadEventStack_H
#define RoadEventStack_H

// Do not use thesa.h to avoid circular references
#include "thidcore/tnamespace.h"

#include <QtCore/QAtomicInt>
#include <QtCore/QByteArray>
#include <QtCore/QHash>
#include <QtCore/QList>
#include <QtCore/QTextStream>
#include <QtCore/QThreadStorage>

namespace Thesa {

#//!\name Defined here
    //! RoadEventStack -- Thread-specific RoadEvent's and RoadLogEvent's
    class RoadEventStack;

#//!\name Used here
    class RoadEvent;
    class RoadEventPool;

class RoadEventStack {
public:
    enum RoadEventStackFlags : quint32 {
    };//RoadEventStackFlags

    struct Statistic {
        int             event_count;
        int             event_msec;
        int             event_start_msec;
    };//RoadEventStack::Statistic

#//!\name Fields
private:
    int                 thread_id;    //!< Unique identifier for this RoadEventStack and QThread (0..)
    int                 disk_id;      //!< Disk identifier of global, persistent data structures (ThDisk::s_thid_disks)
    int                 event_level;  //!< event_level of last event for this thread
    RoadEventPool *     event_pool;   //!< RoadEventPool for this RoadEventStack
    QList<RoadEvent>    event_stack;  //!< Current stack of events
    QByteArray          event_context; //!< Bit array of hashed event_codes.  Defines context for counts and timing
    QList<RoadEvent>    event_log;    //!< Circular log of important events \todo review name, same as event_pool.  How are they different?
    QHash<quint32, Statistic> event_statistics; //!< RoadEventStatistic's by context
    ThFlagsInt          log_priority; //!< Maximum RoadEvent::EventPriorityMask to log
    quint32             first_log_event; //!< First currentEventNumber to be logged by this Thread \todo support ranges
    //!\todo RoadEventPool::selectId should be shared by Deselect and Selected -- same as RoadEventStack.  Move to ThSettings?
    ThLocalInt          log_selected; //!< Index of RoadEvents selected for logging, none if NoIndex
    ThLocalInt          log_deselect; //!< Index of RoadEvents to not log, none if NoIndex

#//!\name Class objects -- reentrant
    static QThreadStorage<RoadEventStack *> thread_event_stacks;
    static QAtomicInt   next_thread_id;
    static QAtomicInt   next_event_number;
    static bool         is_log_stdout; //!< True if logStream() is OutStream instead of ErrStream

#//!\name Constructors, etc.
public:
                        RoadEventStack();
                        ~RoadEventStack() { free(event_pool); }
private:
                        Q_DISABLE_COPY(RoadEventStack)

#//!\name GetSet
public:
    int                 diskId() const { return disk_id; }
    int                 eventLevel() const { return event_level; }
    RoadEventPool *     eventPool() const { return event_pool; }
    quint32             firstLogEvent() const { return first_log_event; }
    void                incrementLogLevel() { setLogLevel(logLevel()+1); }
    ThLocalInt          logDeselect() const { return log_deselect; }
    int                 logLevel() const;
    ThFlagsInt          logPriority() const { return log_priority; }
    ThLocalInt          logSelected() const { return log_selected; }
    void                setDiskId(int i) { disk_id= i; }
    void                setFirstLogEvent(quint32 i) { first_log_event= i; }
    void                setLogDeselect(ThLocalInt i) { log_deselect= i; }
    void                setLogLevel(int i);
    void                setLogPriority(ThFlagsInt i);
    void                setLogSelected(ThLocalInt i) { log_selected= i; }
    int                 threadId() const { return thread_id; }

#//!\name Class functions
    static quint32      currentEventNumber() { return next_event_number.load(); }
    static RoadEventStack * currentEventStack();
    static int          currentLogLevel();
    static bool         isDetailLogging();
    static bool         isDetail2Logging();
    static bool         isLogStdout() { return is_log_stdout; }
    static int          nextEventLevel();
    static quint32      nextEventNumber() { return next_event_number.fetchAndAddRelaxed(1); }
    static int          nextThreadId() { return next_thread_id.fetchAndAddRelaxed(1); }
    static int          previousEventLevel();
    static void         setErrStream(QIODevice *device);
    static void         setInStream(QIODevice *device);
    static void         setIsLogStdout() { is_log_stdout= true; }
    static void         setOutStream(QIODevice *device);
    static void         unsetIsLogStdout() { is_log_stdout= false; }
};//class RoadEventStack

#//!\name Global objects

//!\todo review use of QTextStreams.  Can't be per thread due to interleaving issues.  Compare with QProcess channels.
extern QTextStream      ErrStream;   //!< QTextStream for logging and error reporting.  Initialized to stderr.    Passed by reference.
extern QTextStream      InStream;    //!< QTextStream for input.  Initialized to stdin.  Passed by reference. 
extern QTextStream      OutStream;   //!< QTextStream for output.  Initialized to stdout.

#//!\name Global methods

extern QTextStream &    logStream();
}//namespace Thesa

#endif RoadEventStack_H
