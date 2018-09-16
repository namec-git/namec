# thidcode.pro -- qmake project file
#  $Id: //main/2005/thid/src/nameasm/nameasm.pro#1 $
#
# need to remove thidasm.vcxproj from build and set to 32-bit

include(../thid-warn.pri)
include(../thid-lib.pri)

!all:build_pass:CONFIG(debug, debug|release):{
    TARGET = thidcode_md
    OBJECTS_DIR = ../../build/x64/thidcode/Debug
}else:!all:build_pass:CONFIG(release, debug|release):{
    TARGET = thidcode_m
    OBJECTS_DIR = ../../build/x64/thidcode/Release
}else:build_pass:CONFIG(debug, debug|release):{
    TARGET = thidcode_d
    OBJECTS_DIR = ../../build/x64/thidcode/Debug
}else:build_pass:CONFIG(release, debug|release):{
    TARGET = thidcode
    OBJECTS_DIR = ../../build/x64/thidcode/Release
}

all: HEADERS += thidcode/ThCode.h

all: SOURCES += thidcode/ThCode.cpp

# Source files used by ThCode as a standalone program
# HEADERS += road/RoadError.h
# HEADERS += road/RoadLogEvent.h
# HEADERS += road/RoadUtility.h
# HEADERS += thidcode/ThCode.h
# HEADERS += thidcore/ThError.h
# SOURCES += road/RoadError.cpp
# SOURCES += road/RoadLogEvent.cpp
# SOURCES += road/RoadUtility.cpp
# SOURCES += thidcore/ThError.cpp
