# namec.pro -- qmake project file
#  $Id: //main/2005/thid/src/namec/namec.pro#9 $

DEFINES += TH_NO_PRECOMPILE TH_NO_THID_ID TH_NO_THDISK

include(../thid-warn.pri)
include(../thid-app.pri)

TARGET= bin/namec

CONFIG -= precompile_header
QT -= axserver gui 

build_pass:CONFIG(debug, debug|release):{
   LIBS += -L../../build/x64/Debug
   OBJECTS_DIR= ../../build/x64/namec/Debug
}else:build_pass:CONFIG(release, debug|release):{
   LIBS += -L../../build/x64/Release
   OBJECTS_DIR= ../../build/x64/namec/Release
}

all: HEADERS += thidcode/ThAsmCode.h
all: HEADERS += thidcode/ThCode.h
all: HEADERS += thidcode/ThCoreCode.h
all: HEADERS += thidcode/ThExecuteCode.h
all: HEADERS += namec/RoadUtility_mini.h
mini:HEADERS += road/RoadArg64.h
mini:HEADERS += road/RoadError.h
mini:HEADERS += road/RoadEvent.h
mini:HEADERS += road/RoadEventPool.h
mini:HEADERS += road/RoadEventStack.h
mini:HEADERS += road/RoadLogEvent.h
mini:HEADERS += thidcore/tglobal.h 
mini:HEADERS += thidcore/thesa.h 
mini:HEADERS += thidcore/tnamespace.h 
mini:HEADERS += thidcore/ThCall.h
mini:HEADERS += thidcore/ThError.h
mini:HEADERS += thidcore/ThLog.h
mini:HEADERS += thidcore/ThWarn.h

all: SOURCES += namec/namec.cpp
all: SOURCES += thidcode/ThAsmCode.cpp
all: SOURCES += thidcode/ThCode.cpp
all: SOURCES += thidcode/ThCoreCode.cpp
all: SOURCES += thidcode/ThExecuteCode.cpp

all: SOURCES += namec/RoadUtility_mini.cpp
all: SOURCES += road/RoadArg64.cpp
all: SOURCES += road/RoadError.cpp
all: SOURCES += road/RoadEvent.cpp
all: SOURCES += road/RoadEventPool.cpp
all: SOURCES += road/RoadEventStack.cpp
all: SOURCES += road/RoadLogEvent.cpp
all: SOURCES += thidcore/ThCall.cpp
all: SOURCES += thidcore/ThError.cpp
all: SOURCES += thidcore/ThLog.cpp
all: SOURCES += thidcore/ThWarn.cpp

all: FILES += 