QT       += testlib gifimage

TARGET = tst_qgifimagetest
CONFIG   += console
CONFIG   -= app_bundle
CONFIG   += testcase

TEMPLATE = app


SOURCES += tst_qgifimagetest.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"
