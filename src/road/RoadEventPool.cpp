/****************************************************************************
**
** Copyright (C) 2013-2018 C.B. Barber. All rights reserved.
** $Id: //main/2005/thid/src/road/RoadEventPool.cpp#18 $$Change: 2434 $
** $Date: 2018/07/08 $$Author: bbarber $
**
****************************************************************************/


#include "road/RoadEventPool.h"

#include "thidcore/ThError.h"
#include "road/RoadEventStack.h"
#ifndef TH_NO_THDISK
#include "thidcore/ThSettings.h"
#endif

#include <QtCore/QHash>

namespace Thesa {

#//\name Class variables

const QSet<quint32> RoadEventPool::
s_shared_null_set;        

int RoadEventPool::
Max_select_id= 0;

#//\name Constructors

RoadEventPool::
RoadEventPool()
: road_events()
, duplicate_events()
, event_stacks()
, deselect_events()
, selected_events()
, event_log()
, event_data()
, event_log_begin(0)
, event_log_end(0)
, event_log_first(0)
, event_log_next(0)
, data_begin(0)
, data_end(0)
, data_truncate(0)
, is_log_identifier(true)
{
#ifndef TH_NO_THDISK
    ThSettings settings;
    is_log_identifier= settings.value("logging/includeLogIdentifier", 1).toInt();
#endif
}//RoadEventPool default

RoadEventPool::
~RoadEventPool()
{
    int i= 0;
    Q_UNUSED(i)
}//destructor

#//!\name GetSet

void RoadEventPool::
addDeselectEvent(ThLocalInt selectId, quint32 eventId)
{
    QSet<quint32> *events= deselectEvents(selectId);
    events->insert(eventId);
}//addDeselectEvent

void RoadEventPool::
addSelectedEvent(ThLocalInt selectId, quint32 eventId)
{
    QSet<quint32> *events= selectedEvents(selectId);
    events->insert(eventId);
}//addSelectedEvent

void RoadEventPool::
clearDeselectEvents(ThLocalInt selectId)
{
    QSet<quint32> *events= selectedEvents(selectId);
    events->clear();
}//clearDeselectEvents

void RoadEventPool::
clearSelectedEvents(ThLocalInt selectId)
{
    QSet<quint32> *events= selectedEvents(selectId);
    events->clear();
}//clearSelectedEvents

const QSet<quint32> & RoadEventPool::
deselectEvents(ThLocalInt selectId) const
{
    if(selectId==NoIndex){
        throw ThError(10368, "RoadEventPool deselectEvents: define selectId before using deselectEvents");
    }else if(selectId<0 || selectId>RoadEventPool::Max_select_id){
        throw ThError(10447, "RoadEventPool deselectEvents: expecting 0...%d for selectId (logging/max-select-id).  Got %d", RoadEventPool::Max_select_id, selectId);
    }
    QHash<int, QSet<quint32> >::ConstIterator i= deselect_events.constFind(selectId);
    if(i==deselect_events.constEnd()){
        return s_shared_null_set;
    }
    return *i;
}//deselectEvents

QSet<quint32> * RoadEventPool::
deselectEvents(ThLocalInt selectId)
{
    if(selectId==NoIndex){
        throw ThError(11511, "RoadEventPool deselectEvents: define selectId before modifying deselectEvents");
    }else if(selectId<0 || selectId>RoadEventPool::Max_select_id){
        throw ThError(10448, "RoadEventPool deselectEvents: expecting 0...%d for selectId (logging/max-select-id).  Got %d for modifying deselectEvents", RoadEventPool::Max_select_id, selectId);
    }
    return &deselect_events[selectId];
}//deselectEvents

bool RoadEventPool::
isDeselectEvent(ThLocalInt selectId, quint32 eventId) const
{
    const QSet<quint32> &events= deselectEvents(selectId);
    return events.contains(eventId);
}//isDeselectEvent

bool RoadEventPool::
isSelectedEvent(ThLocalInt selectId, quint32 eventId) const
{
    const QSet<quint32> &events= selectedEvents(selectId);
    return events.contains(eventId);
}//isSelectedEvent

void RoadEventPool::
removeDeselectEvent(ThLocalInt selectId, quint32 eventId)
{
    QSet<quint32> *events= deselectEvents(selectId);
    events->remove(eventId);
}//removeDeselectEvent

void RoadEventPool::
removeSelectedEvent(ThLocalInt selectId, quint32 eventId)
{
    QSet<quint32> *events= selectedEvents(selectId);
    events->remove(eventId);
}//removeSelectedEvent

const RoadEvent & RoadEventPool::
roadEvent(quint32 eventId) const
{
    if(!road_events.contains(eventId)){
        throw ThError(10921, "RoadEventPool: eventId %d not found in RoadEventPool", eventId);
    }
    return *&road_events[eventId];
}//roadEvent

const QSet<quint32> & RoadEventPool::
selectedEvents(ThLocalInt selectId) const
{
    if(selectId==NoIndex){
        throw ThError(11509, "RoadEventPool selectedEvents: define selectId before using selectedEvents");
    }else if(selectId<0 || selectId>RoadEventPool::Max_select_id){
        throw ThError(11510, "RoadEventPool selectedEvents: expecting 0...%d for selectId (logging/max-select-id).  Got %d", RoadEventPool::Max_select_id, selectId);
    }
    QHash<int, QSet<quint32> >::ConstIterator i= selected_events.constFind(selectId);
    if(i==selected_events.constEnd()){
        return s_shared_null_set;
    }
    return *i;
}//selectedEvents

QSet<quint32> * RoadEventPool::
selectedEvents(ThLocalInt selectId)
{
    if(selectId==NoIndex){
        throw ThError(11512, "RoadEventPool selectedEvents: define selectId before modifying selectedEvents");
    }else if(selectId<0 || selectId>RoadEventPool::Max_select_id){
        throw ThError(11513, "RoadEventPool selectedEvents: expecting 0...%d for selectId (logging/max-select-id).  Got %d", RoadEventPool::Max_select_id, selectId);
    }
    return &selected_events[selectId];
}//selectedEvents

#//!\name Methods

void RoadEventPool::
addEvent(quint32 eventId, quint32 eventFlags, const char *fmt)
{
    if(road_events.contains(eventId)){
        if(road_events[eventId].eventFormat()!=fmt){
            duplicate_events.append(fmt);
        }
    }else{
        road_events.insert(eventId, RoadEvent(fmt, eventFlags));
    }
}//addEvent

void RoadEventPool::
addEventStack(RoadEventStack *t)
{
    if(event_stacks.contains(t->threadId())){
        throw ThError(10920, "RoadEventPool internal: event_stacks already contains Thread %d", t->threadId());
    }
    event_stacks.insert(t->threadId(), t);
}//addEventStack

// Not used, should set at constructor
void RoadEventPool::
setEventType(quint32 eventId, ThFlagsInt i)
{
    if(!road_events.contains(eventId)){
        throw ThError(10944, "RoadEventPool: event %d not found.  Cannot setEventType(%u)", eventId, i);
    }
    ThFlagsInt i2= i&RoadEvent::EventTypeMask;
    ThFlagsInt i3= road_events[eventId].eventFlags() & (~RoadEvent::EventTypeMask);
    Q_UNUSED(i2);
    Q_UNUSED(i3);
    // RoadEvent.setEventFlags     road_events[eventId].event_flags= i2+i3;
    throw ThError(10945, "RoadEventPool setEventType: not implemented");
}//setEventType

#//!\name Class functions

RoadEventPool * RoadEventPool::
globalEventPool()
{
    static RoadEventPool * global_event_pool= NULL;
    if(!global_event_pool){
        global_event_pool= new RoadEventPool;
    }
    return global_event_pool;
}//globalEventPool


}//namespace Thesa

