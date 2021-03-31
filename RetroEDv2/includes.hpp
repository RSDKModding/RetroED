#ifndef INCLUDES_H
#define INCLUDES_H

#define RETROED_VER   (0x100)
#define RETROED_DEBUG (1)

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
};

class RESaveEvent : public QEvent
{
public:
    RESaveEvent() : QEvent(static_cast<QEvent::Type>(RE_EVENT_SAVE)) {}

    static const QEvent::Type m_type = static_cast<QEvent::Type>(RE_EVENT_SAVE);
};

class REOpenEvent : public QEvent
{
public:
    REOpenEvent() : QEvent(static_cast<QEvent::Type>(RE_EVENT_OPEN)) {}

    static const QEvent::Type m_type = static_cast<QEvent::Type>(RE_EVENT_OPEN);
};

class RESaveAsEvent : public QEvent
{
public:
    RESaveAsEvent() : QEvent(static_cast<QEvent::Type>(RE_EVENT_SAVE_AS)) {}

    static const QEvent::Type m_type = static_cast<QEvent::Type>(RE_EVENT_SAVE_AS);
};

class RENewEvent : public QEvent
{
public:
    RENewEvent() : QEvent(static_cast<QEvent::Type>(RE_EVENT_NEW)) {}

    static const QEvent::Type m_type = static_cast<QEvent::Type>(RE_EVENT_NEW);
};

// RSDK
#include "dependencies/rsdk_reverse/include.hpp"

// Misc/Utils
#include "utils/retroedutils.hpp"
#include "utils/vectors.hpp"
#include "utils/colour.hpp"
#include "utils/palette.hpp"
#include "utils/shaders.hpp"

// Formats
#include "utils/appconfig.hpp"
#include "utils/staticobjectinfo.hpp"

// Compilers
#include "tools/compiler/compilerv3.hpp"
#include "tools/compiler/compilerv4.hpp"

// Decompilers
#include "tools/decompiler/decompilerv3.hpp"
#include "tools/decompiler/decompilerv4.hpp"
#include "tools/decompiler/decompilerv1.hpp"

// Form Utils
#include "tools/sceneviewer.hpp"
#include "tools/sceneproperties/sceneproperties.hpp"
#include "tools/sceneproperties/scenelayerproperties.hpp"
#include "tools/sceneproperties/sceneobjectproperties.hpp"
#include "tools/sceneproperties/scenescrollproperties.hpp"
#include "tools/sceneproperties/scenetileproperties.hpp"
#include "tools/sceneproperties/sceneexportimgoptions.hpp"

#include "tools/sceneproperties/stageconfigeditorv1.hpp"
#include "tools/sceneproperties/stageconfigeditorv2.hpp"
#include "tools/sceneproperties/stageconfigeditorv3.hpp"
#include "tools/sceneproperties/stageconfigeditorv4.hpp"
#include "tools/sceneproperties/chunkeditor.hpp"
#include "tools/exportrsdkv5scene.hpp"

// Forms
#include "tools/gameconfigeditorv2.hpp"
#include "tools/gameconfigeditorv3.hpp"
#include "tools/gameconfigeditorv4.hpp"
#include "tools/gameconfigeditorv5.hpp"
#include "tools/gfxtool.hpp"
#include "tools/staticobjecteditor.hpp"
#include "tools/savefileeditorv3.hpp"
#include "tools/savefileeditorv4.hpp"
#include "tools/paletteeditor.hpp"
#include "tools/paletteeditor/colourdialog.hpp"
#include "tools/rsdkunpacker.hpp"
#include "tools/scriptunpacker.hpp"
#include "tools/sceneeditor.hpp"
#include "tools/rsvtool.hpp"

#include "mainwindow.hpp"

extern QString homeDir;
extern QString appDir;
extern AppConfig appConfig;

extern QIcon icon_up;
extern QIcon icon_down;
extern QIcon icon_add;
extern QIcon icon_rm;

extern QLabel *statusLabel;
void setStatus(QString status);

#endif // INCLUDES_H
