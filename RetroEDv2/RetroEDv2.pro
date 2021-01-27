QT       += core gui svg xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17 resources_big

DEFINES += QT_DEPRECATED_WARNINGS \
           QT_DISABLE_DEPRECATED_BEFORE=0x060000

include(dependencies/phantom/phantom.pri)
include(dependencies/qtgifimage/src/gifimage/qtgifimage.pri)
include(dependencies/qtcolorwidgets/color_widgets.pri)
include(dependencies/rsdk_reverse/rsdk_reverse.pri)

QMAKE_LFLAGS += -lz -static
LIBS += -lz

INCLUDEPATH += \
    utils/

SOURCES += \
    includes.cpp \
    mainwindow.cpp \
    tools/compiler/compilerv3.cpp \
    tools/decompiler/decompilerv3.cpp \
    tools/decompiler/decompilerv4.cpp \
    tools/decompiler/decompilerv1.cpp \
    tools/gameconfigeditorv5.cpp \
    tools/paletteeditor/colourdialog.cpp \
    tools/rsvtool.cpp \
    tools/savefileeditorv3.cpp \
    tools/sceneProperties/chunkeditor.cpp \
    tools/exportrsdkv5scene.cpp \
    tools/gameconfigeditorv2.cpp \
    tools/gameconfigeditorv3.cpp \
    tools/gameconfigeditorv4.cpp \
    tools/gfxtool.cpp \
    tools/paletteeditor.cpp \
    tools/rsdkunpacker.cpp \
    tools/sceneProperties/scenelayerproperties.cpp \
    tools/sceneProperties/sceneobjectproperties.cpp \
    tools/sceneProperties/sceneproperties.cpp \
    tools/sceneProperties/scenescrollproperties.cpp \
    tools/sceneeditor.cpp \
    tools/sceneproperties/sceneexportimgoptions.cpp \
    tools/sceneproperties/scenetileproperties.cpp \
    tools/sceneproperties/stageconfigeditorv2.cpp \
    tools/sceneproperties/stageconfigeditorv4.cpp \
    tools/sceneproperties/stageconfigeditorv1.cpp \
    tools/sceneviewer.cpp \
    tools/scriptunpacker.cpp \
    tools/sceneProperties/stageconfigeditorv3.cpp \
    tools/staticobjecteditor.cpp \
    utils/appconfig.cpp \
    utils/retroedutils.cpp \
    utils/shaders.cpp \
    main.cpp \
    utils/staticobjectinfo.cpp

HEADERS += \
    mainwindow.hpp \
    tools/compiler/compilerv3.hpp \
    tools/decompiler/decompilerv3.hpp \
    tools/decompiler/decompilerv4.hpp \
    tools/decompiler/decompilerv1.hpp \
    tools/gameconfigeditorv5.hpp \
    tools/paletteeditor/colourdialog.hpp \
    tools/rsvtool.hpp \
    tools/savefileeditorv3.hpp \
    tools/sceneProperties/chunkeditor.hpp \
    tools/exportrsdkv5scene.hpp \
    tools/gameconfigeditorv2.hpp \
    tools/gameconfigeditorv3.hpp \
    tools/gameconfigeditorv4.hpp \
    tools/gfxtool.hpp \
    tools/paletteeditor.hpp \
    tools/rsdkunpacker.hpp \
    tools/sceneProperties/scenelayerproperties.hpp \
    tools/sceneProperties/sceneobjectproperties.hpp \
    tools/sceneProperties/sceneproperties.hpp \
    tools/sceneProperties/scenescrollproperties.hpp \
    tools/sceneeditor.hpp \
    tools/sceneproperties/sceneexportimgoptions.hpp \
    tools/sceneproperties/scenetileproperties.hpp \
    tools/sceneproperties/stageconfigeditorv2.hpp \
    tools/sceneproperties/stageconfigeditorv4.hpp \
    tools/sceneproperties/stageconfigeditorv1.hpp \
    tools/sceneviewer.hpp \
    tools/scriptunpacker.hpp \
    tools/sceneProperties/stageconfigeditorv3.hpp \
    tools/staticobjecteditor.hpp \
    utils/appconfig.hpp \
    utils/retroedutils.hpp \
    utils/shaders.hpp \
    includes.hpp \
    utils/staticobjectinfo.hpp

FORMS += \
    mainwindow.ui \
    tools/gameconfigeditorv5.ui \
    tools/paletteeditor/colourdialog.ui \
    tools/rsvtool.ui \
    tools/savefileeditorv3.ui \
    tools/sceneProperties/chunkeditor.ui \
    tools/exportrsdkv5scene.ui \
    tools/gameconfigeditorv2.ui \
    tools/gameconfigeditorv3.ui \
    tools/gameconfigeditorv4.ui \
    tools/gfxtool.ui \
    tools/paletteeditor.ui \
    tools/rsdkunpacker.ui \
    tools/sceneProperties/scenelayerproperties.ui \
    tools/sceneProperties/sceneobjectproperties.ui \
    tools/sceneProperties/sceneproperties.ui \
    tools/sceneProperties/scenescrollproperties.ui \
    tools/sceneeditor.ui \
    tools/sceneproperties/sceneexportimgoptions.ui \
    tools/sceneproperties/scenetileproperties.ui \
    tools/sceneproperties/stageconfigeditorv2.ui \
    tools/sceneproperties/stageconfigeditorv4.ui \
    tools/sceneproperties/stageconfigeditorv1.ui \
    tools/scriptunpacker.ui \
    tools/sceneProperties/stageconfigeditorv3.ui \
    tools/staticobjecteditor.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

#windows
win32 {
    RC_ICONS = icons/win.ico
    VERSION = 2.0.0
    QMAKE_TARGET_COMPANY = Rubberduckycooly
    QMAKE_TARGET_DESCRIPTION = RetroED
    QMAKE_TARGET_COPYRIGHT = Rubberduckycooly
}

TARGET = RetroED

RESOURCES += \
    resources.qrc

DISTFILES +=
