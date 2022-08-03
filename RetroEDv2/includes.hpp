#ifndef INCLUDES_H
#define INCLUDES_H

#include "version.hpp"

// QT
#include <QtCore>
#include <QStringList>
#include <QColorDialog>
#include <QDebug>
#include <QDialog>
#include <QDir>
#include <QFileDialog>
#include <QImage>
#include <QLabel>
#include <QLayout>
#include <QListWidget>
#include <QMdiArea>
#include <QToolButton>
#include <QWidget>
#include <QtEndian>
#include <QTreeView>
#include <QtGui>
#include <QtWidgets>
#include <QMainWindow>
#include <QFileSystemModel>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QMessageBox>
#include <QMenu>
#include <QMetaType>
#include <QDomElement>

#include "dependencies/qtcolorwidgets/color_preview.hpp"

typedef signed char sbyte;
typedef unsigned char byte;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;
typedef long long int64;
typedef unsigned long long uint64;

enum Event {
    RE_EVENT_SAVE = QEvent::User + 64,
    RE_EVENT_OPEN,
    RE_EVENT_NEW,
    RE_EVENT_SAVE_AS,
    RE_EVENT_UNDO,
    RE_EVENT_REDO,
    RE_EVENT_TAB_GAIN_FOCUS,
    RE_EVENT_TAB_LOSE_FOCUS,
};

class REAppEvent : public QEvent
{
public:
    REAppEvent(int event) : QEvent(static_cast<QEvent::Type>(event)) {}

    static const QEvent::Type type;
};

// RSDK
#include "dependencies/rsdk_reverse/rsdkreverse.hpp"

// Misc/Utils
#include "utils/retroedutils.hpp"
#include "utils/vectors.hpp"
#include "utils/colour.hpp"
#include "utils/palette.hpp"
#include "utils/shaders.hpp"
#include "utils/stringhelpers.hpp"
#include "utils/workingdirmanager.hpp"

// RSDKv5 Link
#include "tools/gamelink/gamelink.hpp"

// Formats
#include "utils/appconfig.hpp"
#include "utils/staticobjectinfo.hpp"

// Compilers
#include "tools/compiler/compilerv1.hpp"
#include "tools/compiler/compilerv2.hpp"
#include "tools/compiler/compilerv3.hpp"
#include "tools/compiler/compilerv4.hpp"

// Decompilers
#include "tools/decompiler/decompilerv1.hpp"
#include "tools/decompiler/decompilerv3.hpp"
#include "tools/decompiler/decompilerv4.hpp"

// Form Utils
#include "tools/utils/propertygrid.hpp"

// Scene Tools (v5)
#include "tools/sceneproperties/sceneincludesv5.hpp"
#include "tools/sceneproperties/scenepropertiesv5.hpp"
#include "tools/sceneproperties/scenelayerpropertiesv5.hpp"
#include "tools/sceneproperties/sceneobjectpropertiesv5.hpp"
#include "tools/sceneproperties/scenescrollpropertiesv5.hpp"
#include "tools/sceneproperties/scenetilepropertiesv5.hpp"
#include "tools/sceneproperties/scenestamppropertiesv5.hpp"
#include "tools/sceneproperties/objectselector.hpp"
#include "tools/sceneproperties/tilereplaceoptions.hpp"
#include "tools/sceneproperties/confirmgamelink.hpp"
#include "tools/sceneproperties/syncgcdetails.hpp"
#include "tools/sceneviewer.hpp"

// Scene Tools
#include "tools/sceneproperties/sceneproperties.hpp"
#include "tools/sceneproperties/scenelayerproperties.hpp"
#include "tools/sceneproperties/sceneobjectproperties.hpp"
#include "tools/sceneproperties/scenescrollproperties.hpp"
#include "tools/sceneproperties/scenetileproperties.hpp"

#include "tools/sceneproperties/stageconfigeditorv1.hpp"
#include "tools/sceneproperties/stageconfigeditorv2.hpp"
#include "tools/sceneproperties/stageconfigeditorv3.hpp"
#include "tools/sceneproperties/stageconfigeditorv4.hpp"
#include "tools/sceneproperties/stageconfigeditorv5.hpp"
#include "tools/sceneproperties/chunkeditor.hpp"
#include "tools/sceneproperties/tileseteditor.hpp"

#include "tools/animationeditor/animsheetselector.hpp"

// Model Manager Utils
#include "tools/utils/modelviewer.hpp"

// Forms
#include "tools/gameconfigeditorv1.hpp"
#include "tools/gameconfigeditorv2.hpp"
#include "tools/gameconfigeditorv3.hpp"
#include "tools/gameconfigeditorv4.hpp"
#include "tools/gameconfigeditorv5.hpp"
#include "tools/animationeditor.hpp"
#include "tools/modelmanager.hpp"
#include "tools/gfxtool.hpp"
#include "tools/paletteeditor.hpp"
#include "tools/paletteeditor/colourdialog.hpp"
#include "tools/rsdkunpacker.hpp"
#include "tools/scriptcompiler.hpp"
#include "tools/scriptunpacker.hpp"
#include "tools/sceneeditor.hpp"
#include "tools/sceneeditorv5.hpp"
#include "tools/userdbmanager.hpp"
#include "tools/rsvtool.hpp"

#include "aboutwindow.hpp"
#include "mainwindow.hpp"

extern QString homeDir;
extern QString appDir;
extern AppConfig appConfig;

extern QLabel *statusLabel;
extern QProgressBar *statusProgress;

extern QString argInitStage;
extern QString argInitScene;
extern QString argInitFilter;

extern SceneEditor *scnEditor;
extern SceneEditorv5 *v5Editor;

extern QIcon playPauseIco[2];

void PrintLog(QString msg);
void SetStatus(QString status, bool useStatus = false);
void AddStatusProgress(float percent);
void SetStatusProgress(float percent);

#endif // INCLUDES_H
