#pragma once

// QT
#include <QtCore>
#include <QStringList>
#include <QDebug>
#include <QDir>
#include <QImage>
#include <QtEndian>
#include <QtGui>
#include <QtWidgets>
#include <QFileSystemModel>

typedef signed char sbyte;
typedef unsigned char byte;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;
typedef long long int64;
typedef unsigned long long uint64;

// IO
#include "io/reader.hpp"
#include "io/writer.hpp"

// Misc/Utils
#include "utils/utils.hpp"
#include "utils/vectors.hpp"
#include "utils/colour.hpp"
#include "utils/palette.hpp"

enum EngineVersion { ENGINE_v5, ENGINE_v4, ENGINE_v3, ENGINE_v2, ENGINE_v1, ENGINE_NONE = 0xFF };

// Helpers
#include "utils/formathelpers/gif.hpp"

#include "utils/formathelpers/gameconfig.hpp"
#include "utils/formathelpers/animation.hpp"
#include "utils/formathelpers/background.hpp"
#include "utils/formathelpers/chunks.hpp"
#include "utils/formathelpers/scene.hpp"
#include "utils/formathelpers/stageconfig.hpp"