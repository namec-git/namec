/****************************************************************************
**
** Copyright (C) 2006-2018 C.B. Barber. All rights reserved.
** $Id: //main/2005/thid/src/thidcore/thesa.h#44 $$Change: 2480 $
** $Date: 2018/09/15 $$Author: bbarber $
**
****************************************************************************/

#ifndef thesa_H
#define thesa_H

#//Headers included with every Thesa source file

//!\todo Review use of QT_NO_STL -- used it anyway
#define QT_NO_STL

#ifndef TH_NO_PRECOMPILE
#include "precompiled.h"
#endif TH_NO_PRECOMPILE

#//keep this order and duplicate in precompiled.h
#include "thidcore/tnamespace.h"
#include "thidcore/tglobal.h"
#include "road/RoadArg64.h"
#include "road/RoadEvent.h"
#include "road/RoadLogEvent.h"
#include "road/RoadError.h"
#ifndef TH_NO_THID_ID
#include "road/RoadUtility.h"
#include "thidcore/ThObject.h"
#else
#include "namec/RoadUtility_mini.h"
#endif

#include "thidcore/ThCall.h"
#include "thidcore/ThError.h"
#include "thidcore/ThLog.h"
#include "thidcore/ThWarn.h"

// #ifdef TH_MINI_BUILD
//! #include "thidcore/ThDisk_mini.h"  \todo thesa.h/ThDisk_mini.h and precompiled.h/ThDisk_mini.h goof up intellisense since it becomes an external dependency of MSDEV
// #endif

// Same QtCore list as precompiled.h
#include <QtCore/QAtomicInt>
#include <QtCore/QBitArray>
#include <QtCore/QByteArray>
#include <QtCore/QChar>
#include <QtCore/QCoreApplication>  // translate
#include <QtCore/QCryptographicHash>
#include <QtCore/QDataStream>
#include <QtCore/QDate>
#include <QtCore/QDateTime>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QFlags>
#include <QtCore/QGlobalStatic>
#include <QtCore/QHash>
#include <QtCore/QHashIterator>
#include <QtCore/QIODevice>
#include <QtCore/QList>
#include <QtCore/QLocale>
#include <QtCore/QMap>
#include <QtCore/QMapIterator>
#include <QtCore/QMultiHash>
#include <QtCore/QMultiMap>
#include <QtCore/QMutableMapIterator>
#include <QtCore/QObject>
#include <QtCore/QPointer>
#include <QtCore/QRegularExpression>
#include <QtCore/QRegularExpressionMatch>
#include <QtCore/QSet>
#include <QtCore/QSettings>
#include <QtCore/QSharedData>
#include <QtCore/QSharedDataPointer>
#include <QtCore/QSize>
#include <QtCore/QString>
#include <QtCore/QStringBuilder>
#include <QtCore/QStringList>
#include <QtCore/QtDebug>
#include <QtCore/QtEndian>
#include <QtCore/QTextCodec>
#include <QtCore/QTextStream>
#include <QtCore/QtGlobal>
#include <QtCore/QThread>
#include <QtCore/QThreadStorage>
#include <QtCore/QTimeZone>
#include <QtCore/QUuid>
#include <QtCore/QVariant>
#include <QtCore/QVarLengthArray>
#include <QtCore/QVector>

#endif thesa_H
