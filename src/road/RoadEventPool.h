/****************************************************************************
**
** Copyright (C) 2013-2018 C.B. Barber. All rights reserved.
** $Id: //main/2005/thid/src/road/RoadEventPool.h#16 $$Change: 2452 $
** $Date: 2018/07/31 $$Author: bbarber $
**
****************************************************************************/

#ifndef RoadEventPool_H
#define RoadEventPool_H

#include "road/RoadEvent.h"
#include "road/RoadLogEvent.h"

#include <QtCore/QByteArray>
#include <QtCore/QHash>
#include <QtCore/QList>
#include <QtCore/QSet>

namespace Thesa {

#//!\name Defined here
    //! RoadEventPool -- Pool of RoadEvent's and RoadLogEvent's
    class RoadEventPool;

#//!\name Used here
    class RoadEventStack;

class RoadEventPool {

#//!\name Fields
private:
    QHash<quint32, RoadEvent> road_events;  //!< RoadEvent by event_id
    QList<const char *> duplicate_events; //!< event_format's with with the same event_id
    QHash<quint32, RoadEventStack *> event_stacks;  //!< RoadEventStack by thread_id.  Owned by RoadEventStack::thread_event_stacks
    QHash<ThLocalInt, QSet<quint32> > deselect_events;  //!< Events not selected for logging \todo Convert to a QList, faster
    QHash<ThLocalInt, QSet<quint32> > selected_events;  //!< Events selected for logging
    QList<RoadLogEvent> event_log;      //!< Circular list of RoadLogEvents with associated data \todo Compress event_log    
    QByteArray          event_data;     //!< Circular compressed data for event_log, each event prefixed by event_id 
    int                 event_log_begin; //!< Index of first RoadLoadEvent in event_log
    int                 event_log_end;  //!< Index after last RoadLoadEvent in event_log
    quint32             event_log_first; //!< First sequential, event_id in event_log
    quint32             event_log_next; //!< Next event_id for event_log (i.e., at event_log_end or event_log.data)
    int                 data_begin;  //!< Index of first data in event_data
    int                 data_end;    //!< Index after last data in event_data
    int                 data_truncate;  //!< Truncate longer events in event_data
    bool                is_log_identifier; //!< True 

#//!\name Class variables
    static const QSet<quint32> s_shared_null_set;        
public:
    static int          Max_select_id; //!< Maximum selectId for selected_events and deselect_events "logging/max-select-id"

#//!\name Constructors, etc.
public:
                        RoadEventPool();
                        ~RoadEventPool();
private:
                        Q_DISABLE_COPY(RoadEventPool)

#//!\name GetSet
public:
    //!\todo selectId should be shared by Deselected and Selected -- same as RoadEventStack.  Move to ThSettings?
    void                addDeselectEvent(ThLocalInt selectId, quint32 eventId);
    void                addSelectedEvent(ThLocalInt selectId, quint32 eventId);
    const QSet<quint32> & deselectEvents(ThLocalInt selectId) const;
    QSet<quint32> *     deselectEvents(ThLocalInt selectId);
    void                clearDeselectEvents(ThLocalInt selectId);
    void                clearSelectedEvents(ThLocalInt selectId);
    QList<const char *> duplicateEvents() const { return duplicate_events; }
    bool                isDeselectEvent(ThLocalInt selectId, quint32 eventId) const;
    bool                isLogIdentifier() const { return is_log_identifier; }
    bool                isSelectedEvent(ThLocalInt selectId, quint32 eventId) const;
    void                removeDeselectEvent(ThLocalInt selectId, quint32 eventId);
    void                removeSelectedEvent(ThLocalInt selectId, quint32 eventId);
    const RoadEvent &   roadEvent(quint32 eventId) const;
    const QSet<quint32> & selectedEvents(ThLocalInt selectId) const;
    QSet<quint32> *    selectedEvents(ThLocalInt selectId);


#//!\name Methods
    void                addEvent(quint32 eventId, quint32 eventFlags, const char *fmt);
    void                addEventStack(RoadEventStack *t);
    void                setEventType(quint32 eventId, ThFlagsInt i);

#//!\name Class objects
    static RoadEventPool * globalEventPool();

};//class RoadEventPool

typedef  QHash<int, RoadEventPool>      RoadEventPools;

}//namespace Thesa

#endif RoadEventPool_H
