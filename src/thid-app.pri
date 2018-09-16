# thid-app.pri -- qmake include file for Thid applications
#
# Paths are relative to the caller (src/*/*.pro)

# Place executables (TARGET) in build/x64/
DESTDIR= ../..
win32{
    TEMPLATE= vcapp
}else{
    TEMPLATE= app
}
!mini:CONFIG += mini all
!all:DEFINES += TH_MINI_BUILD
CONFIG += qt debug precompile_header
CONFIG -= app_bundle
QT += network testlib
# QT -= gui

DEFINES +=  QT_NO_CAST_TO_ASCII

!contains( DEFINES, TH_NO_PRECOMPILE ): PRECOMPILED_HEADER = ../../src/precompiled.h

MOC_DIR= ../../build/moc

INCLUDEPATH += ../../src ../../build
# For precompiled.h and thidproto/ and thidtest.pro
INCLUDEPATH += c:/bash/local/protobuf-2.6.1/src

VPATH= ../../src
