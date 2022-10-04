QT       += gifimage

TARGET = extractFrames
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += main.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"
