QT       += core gui svg xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets xml

CONFIG += c++17 resources_big silent

DEFINES += QT_DEPRECATED_WARNINGS #\
#           QT_DISABLE_DEPRECATED_BEFORE=0x060000

include(dependencies/phantom/phantom.pri)
include(dependencies/QtGifImage/src/gifimage/qtgifimage.pri)
include(dependencies/qtcolorwidgets/color_widgets.pri)
include(dependencies/libRSDK/libRSDK.pri)
include(dependencies/imageviewer/imageviewer.pri)
include(dependencies/QtPropertyBrowser/src/qtpropertybrowser.pri)

QMAKE_LFLAGS += -lz

!win32 {
LIBS += -lz
}

INCLUDEPATH += \
    utils/ \
    dependencies/libRSDK/ \
    dependencies/libRSDK/formats 

SOURCES += \
    aboutwindow.cpp \
    gamemanager.cpp \
    includes.cpp \
    mainwindow.cpp \
    splashscreen.cpp \
    tools/animationeditor.cpp \
    tools/animationeditor/animsheetselector.cpp \
    tools/compiler/compilerv1.cpp \
    tools/compiler/compilerv2.cpp \
    tools/compiler/compilerv3.cpp \
    tools/compiler/compilerv4.cpp \
    tools/gameconfigeditorv1.cpp \
    tools/gameconfigeditorv5.cpp \
    tools/gamelink/gamelink.cpp \
    tools/gamelink/gamemath.cpp \
    tools/gamelink/gameobjects.cpp \
    tools/gamelink/gamematrix.cpp \
    tools/gamelink/gamedraw.cpp \
    tools/gamelink/gamestorage.cpp \
    tools/modelmanager.cpp \
    tools/paletteeditor/colourdialog.cpp \
    tools/paletteeditor/paletteimport.cpp \
    tools/rsvtool.cpp \
    tools/gameconfigeditorv2.cpp \
    tools/gameconfigeditorv3.cpp \
    tools/gameconfigeditorv4.cpp \
    tools/gfxtool.cpp \
    tools/paletteeditor.cpp \
    tools/rsdkunpacker.cpp \
    tools/sceneeditor.cpp \
    tools/sceneeditorv5.cpp \
    tools/sceneproperties/chunkcollisioneditor.cpp \
    tools/sceneproperties/chunkcollisioneditorv1.cpp \
    tools/sceneproperties/chunkeditor.cpp \
    tools/sceneproperties/chunkreplaceoptions.cpp \
    tools/sceneproperties/confirmgamelink.cpp \
    tools/sceneproperties/copyplane.cpp \
    tools/sceneproperties/createscene.cpp \
    tools/sceneproperties/gotopos.cpp \
    tools/sceneproperties/objectselector.cpp \
    tools/sceneproperties/scenelayerproperties.cpp \
    tools/sceneproperties/scenelayerpropertiesv5.cpp \
    tools/sceneproperties/scenelayershift.cpp \
    tools/sceneproperties/sceneobjectproperties.cpp \
    tools/sceneproperties/sceneobjectpropertiesv5.cpp \
    tools/sceneproperties/sceneproperties.cpp \
    tools/sceneproperties/scenepropertiesv5.cpp \
    tools/sceneproperties/scenescrollproperties.cpp \
    tools/sceneproperties/scenescrollpropertiesv5.cpp \
    tools/sceneproperties/scenestamppropertiesv5.cpp \
    tools/sceneproperties/scenetileproperties.cpp \
    tools/sceneproperties/scenetilepropertiesv5.cpp \
    tools/sceneproperties/stageconfigeditorv2.cpp \
    tools/sceneproperties/stageconfigeditorv3.cpp \
    tools/sceneproperties/stageconfigeditorv4.cpp \
    tools/sceneproperties/stageconfigeditorv1.cpp \
    tools/sceneproperties/stageconfigeditorv5.cpp \
    tools/sceneproperties/syncgcdetails.cpp \
    tools/sceneproperties/tilereplaceoptions.cpp \
    tools/sceneproperties/tileseteditor.cpp \
    tools/sceneviewer.cpp \
    tools/scriptcompiler.cpp \
    tools/userdbmanager.cpp \
    tools/utils/modelviewer.cpp \
    tools/utils/propertygrid.cpp \
    utils/appconfig.cpp \
    utils/retroedutils.cpp \
    utils/shaders.cpp \
    main.cpp \
    utils/stringhelpers.cpp \
    utils/workingdirmanager.cpp

HEADERS += \
    aboutwindow.hpp \
    gamemanager.hpp \
    mainwindow.hpp \
    splashscreen.hpp \
    tools/animationeditor.hpp \
    tools/animationeditor/animsheetselector.hpp \
    tools/animationeditor/focuslabel.h \
    tools/compiler/compilerv1.hpp \
    tools/compiler/compilerv2.hpp \
    tools/compiler/compilerv3.hpp \
    tools/compiler/compilerv4.hpp \
    tools/gameconfigeditorv1.hpp \
    tools/gameconfigeditorv5.hpp \
    tools/gamelink/gamelink.hpp \
    tools/gamelink/gamemath.hpp \
    tools/gamelink/gameobjects.hpp \
    tools/gamelink/gamematrix.hpp \
    tools/gamelink/gamedraw.hpp \
    tools/gamelink/gamestorage.hpp \
    tools/modelmanager.hpp \
    tools/paletteeditor/colourdialog.hpp \
    tools/paletteeditor/paletteimport.hpp \
    tools/rsvtool.hpp \
    tools/gameconfigeditorv2.hpp \
    tools/gameconfigeditorv3.hpp \
    tools/gameconfigeditorv4.hpp \
    tools/gfxtool.hpp \
    tools/paletteeditor.hpp \
    tools/rsdkunpacker.hpp \
    tools/sceneeditor.hpp \
    tools/sceneeditorv5.hpp \
    tools/sceneproperties/chunkcollisioneditor.hpp \
    tools/sceneproperties/chunkcollisioneditorv1.hpp \
    tools/sceneproperties/chunkeditor.hpp \
    tools/sceneproperties/chunkreplaceoptions.hpp \
    tools/sceneproperties/confirmgamelink.hpp \
    tools/sceneproperties/copyplane.hpp \
    tools/sceneproperties/createscene.hpp \
    tools/sceneproperties/gotopos.hpp \
    tools/sceneproperties/objectselector.hpp \
    tools/sceneproperties/sceneincludesv5.hpp \
    tools/sceneproperties/scenelayerproperties.hpp \
    tools/sceneproperties/scenelayerpropertiesv5.hpp \
    tools/sceneproperties/scenelayershift.hpp \
    tools/sceneproperties/sceneobjectproperties.hpp \
    tools/sceneproperties/sceneobjectpropertiesv5.hpp \
    tools/sceneproperties/sceneproperties.hpp \
    tools/sceneproperties/scenepropertiesv5.hpp \
    tools/sceneproperties/scenescrollproperties.hpp \
    tools/sceneproperties/scenescrollpropertiesv5.hpp \
    tools/sceneproperties/scenestamppropertiesv5.hpp \
    tools/sceneproperties/scenetileproperties.hpp \
    tools/sceneproperties/scenetilepropertiesv5.hpp \
    tools/sceneproperties/stageconfigeditorv2.hpp \
    tools/sceneproperties/stageconfigeditorv3.hpp \
    tools/sceneproperties/stageconfigeditorv4.hpp \
    tools/sceneproperties/stageconfigeditorv1.hpp \
    tools/sceneproperties/stageconfigeditorv5.hpp \
    tools/sceneproperties/syncgcdetails.hpp \
    tools/sceneproperties/tilereplaceoptions.hpp \
    tools/sceneproperties/tileseteditor.hpp \
    tools/sceneviewer.hpp \
    tools/scriptcompiler.hpp \
    tools/userdbmanager.hpp \
    tools/utils/modelviewer.hpp \
    tools/utils/propertygrid.hpp \
    utils/appconfig.hpp \
    utils/retroedutils.hpp \
    utils/shaders.hpp \
    includes.hpp \
    utils/stringhelpers.hpp \
    utils/workingdirmanager.hpp \
    version.hpp

FORMS += \
    aboutwindow.ui \
    gamemanager.ui \
    mainwindow.ui \
    tools/animationeditor.ui \
    tools/animationeditor/animsheetselector.ui \
    tools/gameconfigeditorv1.ui \
    tools/gameconfigeditorv5.ui \
    tools/modelmanager.ui \
    tools/paletteeditor/colourdialog.ui \
    tools/paletteeditor/paletteimport.ui \
    tools/rsvtool.ui \
    tools/gameconfigeditorv2.ui \
    tools/gameconfigeditorv3.ui \
    tools/gameconfigeditorv4.ui \
    tools/gfxtool.ui \
    tools/paletteeditor.ui \
    tools/rsdkunpacker.ui \
    tools/sceneeditor.ui \
    tools/sceneeditorv5.ui \
    tools/sceneproperties/chunkcollisioneditor.ui \
    tools/sceneproperties/chunkcollisioneditorv1.ui \
    tools/sceneproperties/chunkeditor.ui \
    tools/sceneproperties/chunkreplaceoptions.ui \
    tools/sceneproperties/confirmgamelink.ui \
    tools/sceneproperties/copyplane.ui \
    tools/sceneproperties/createscene.ui \
    tools/sceneproperties/gotopos.ui \
    tools/sceneproperties/objectselector.ui \
    tools/sceneproperties/scenelayerproperties.ui \
    tools/sceneproperties/scenelayerpropertiesv5.ui \
    tools/sceneproperties/scenelayershift.ui \
    tools/sceneproperties/sceneobjectproperties.ui \
    tools/sceneproperties/sceneobjectpropertiesv5.ui \
    tools/sceneproperties/sceneproperties.ui \
    tools/sceneproperties/scenepropertiesv5.ui \
    tools/sceneproperties/scenescrollproperties.ui \
    tools/sceneproperties/scenescrollpropertiesv5.ui \
    tools/sceneproperties/scenestamppropertiesv5.ui \
    tools/sceneproperties/scenetileproperties.ui \
    tools/sceneproperties/scenetilepropertiesv5.ui \
    tools/sceneproperties/stageconfigeditorv2.ui \
    tools/sceneproperties/stageconfigeditorv3.ui \
    tools/sceneproperties/stageconfigeditorv4.ui \
    tools/sceneproperties/stageconfigeditorv1.ui \
    tools/sceneproperties/stageconfigeditorv5.ui \
    tools/sceneproperties/syncgcdetails.ui \
    tools/sceneproperties/tilereplaceoptions.ui \
    tools/sceneproperties/tileseteditor.ui \
    tools/scriptcompiler.ui \
    tools/userdbmanager.ui

TARGET = RetroED
VERSION = 2.0.0

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

#windows
win32 {
    RC_ICONS = icons/win.ico
    QMAKE_TARGET_COMPANY = RSDKModdingTeam
    QMAKE_TARGET_DESCRIPTION = RetroED
    QMAKE_TARGET_COPYRIGHT = RSDKModdingTeam
    CONFIG(debug, debug | release){ DESTDIR = $$OUT_PWD/debug/RetroED }
    else {
        DESTDIR = $$OUT_PWD/release/RetroED
        !static {
            contains(ENV, GITHUB_ACTION) { QMAKE_POST_LINK = windeployqt $$shell_path($$DESTDIR/$${TARGET}.exe) }
            else {
                #bruh
                win32-msvc { QMAKE_POST_LINK = $$(QTDIR)/bin/windeployqt.exe $$shell_path($$DESTDIR/$${TARGET}.exe) }
                else { QMAKE_POST_LINK = windeployqt $$shell_path($$DESTDIR/$${TARGET}.exe) }
            }
        }
    }
}

#mac
mac {
    ICON = icons/mac.icns
}

RESOURCES += \
    resources.qrc

DISTFILES +=
