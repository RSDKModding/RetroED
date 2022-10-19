#pragma once

// Dev temp stuff
#define RE_USE_UNSTABLE (0)

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
#include <libRSDK.hpp>

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

// Form Utils
#include "tools/utils/propertygrid.hpp"

// Forms
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

// forward declarations
class SceneEditor;
class SceneEditorv5;

extern SceneEditor *scnEditor;
extern SceneEditorv5 *v5Editor;

extern QIcon playPauseIco[2];

void PrintLog(QString msg);
void SetStatus(QString status, bool useStatus = false);
void AddStatusProgress(float percent);
void SetStatusProgress(float percent);
