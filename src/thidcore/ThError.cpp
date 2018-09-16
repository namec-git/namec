/****************************************************************************
**
** Copyright (C) 2010-2018 C.B. Barber. All rights reserved.
** $Id: //main/2005/thid/src/thidcore/ThError.cpp#26 $$Change: 2450 $
** $Date: 2018/07/25 $$Author: bbarber $
**
****************************************************************************/

#//! ThError -- All exceptions thrown by Thid are DepotThesaErrors

// Included by thidcore/thesa.h
#include "thidcore/ThError.h"
#include "thidcore/thesa.h"

#include "road/RoadEventStack.h" // logStream()
#include "road/RoadLogEvent.h"

namespace Thesa {

#//!\name Constructor,etc

#//!\name Class functions

//!Warn about an error.  \todo Log with signature
//!\todo How to handle trapError.  It trips on preparing the parameter for warn()
void ThError::
warn(const ThError &e)
{
    static int last_error= NoIndex;
    static int last_error_count= 1;
    static int last_event= 0;
    RoadLogEvent logEvent= e.roadLogEvent();
    bool isNextEvent= (logEvent.eventNumber()==last_event+1);
    last_event= logEvent.eventNumber();
    if(last_error==e.errorCode() && isNextEvent){
        switch(last_error){ 
        // Always report these errors, even if repeated multiple times
        case 10704: /* ThRevisionId */
        case 10722: /* empty bootBlock */ 
        case 10728: /* ThRepairDisk checkDiskBlockIdHave no owner */
        case 10839: /* missing ThidId64 */
        case 10851: /* missing ThRevisionId ThidId64 */
        case 10852: /* ThRepairDisk missing ThidHash ThidId64 */
        case 10854: /* ThRepairDisk found missing ThidHash */
        case 0: /* logging */ 
            break;
        default:
            if(++last_error_count==3){
                //!\todo Fix logic for 'Skipping duplicate warnings' -- should account for ThLog, works sometimes but not all the time
                logStream() << "Warning Skipping duplicate warnings ...\n"; 
                //!\todo Print count of duplicate warnings.
                //!\todo Create a way to elide expected-warnings, e.g., from ThDiskBlockPool::determineFirstSector
                return;
            }else if(last_error_count>3){
                return;
            }
            break;
        }
    }
    //!\todo Replace logStream() in ThLog.trapLog, ThWarn.trapWarn, and ThError.warn with log event buffer
    logStream() << "Warning " << e.what() << "\n";  //!\todo log warn()
    if(last_error!=e.errorCode()){
        last_error= e.errorCode();
        last_error_count= 1;
    }
}//warn

}//namespace orgThid

