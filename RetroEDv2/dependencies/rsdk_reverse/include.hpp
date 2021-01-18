#ifndef RSDK_INCLUDE_H
#define RSDK_INCLUDE_H

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

// IO
#include "io/reader.hpp"
#include "io/writer.hpp"

// Misc/Utils
#include "utils/utils.hpp"
#include "utils/vectors.hpp"
#include "utils/colour.hpp"
#include "utils/palette.hpp"

enum EngineVersion {
    ENGINE_NONE,
    ENGINE_v4,
    ENGINE_v3,
    ENGINE_v2,
    ENGINE_v1,
};

// RSDKv1
#include "formats/RSDKv1/animationv1.hpp"
#include "formats/RSDKv1/backgroundv1.hpp"
#include "formats/RSDKv1/bytecodev1.hpp"
#include "formats/RSDKv1/characterlistv1.hpp"
#include "formats/RSDKv1/chunksv1.hpp"
#include "formats/RSDKv1/datafilev1.hpp"
#include "formats/RSDKv1/gfxv1.hpp"
#include "formats/RSDKv1/savefilev1.hpp"
#include "formats/RSDKv1/scenev1.hpp"
#include "formats/RSDKv1/scriptv1.hpp"
#include "formats/RSDKv1/stageconfigv1.hpp"
#include "formats/RSDKv1/tileconfigv1.hpp"
#include "formats/RSDKv1/zonelistv1.hpp"

// RSDKv2
#include "formats/RSDKv2/animationv2.hpp"
#include "formats/RSDKv2/backgroundv2.hpp"
#include "formats/RSDKv2/chunksv2.hpp"
#include "formats/RSDKv2/datafilev2.hpp"
#include "formats/RSDKv2/gameconfigv2.hpp"
#include "formats/RSDKv2/gfxv2.hpp"
#include "formats/RSDKv2/scenev2.hpp"
#include "formats/RSDKv2/stageconfigv2.hpp"
#include "formats/RSDKv2/tileconfigv2.hpp"
#include "formats/RSDKv2/videov2.hpp"

// RSDKv3
#include "formats/RSDKv3/animationv3.hpp"
#include "formats/RSDKv3/arccontainerv3.hpp"
#include "formats/RSDKv3/backgroundv3.hpp"
#include "formats/RSDKv3/bytecodev3.hpp"
#include "formats/RSDKv3/chunksv3.hpp"
#include "formats/RSDKv3/datafilev3.hpp"
#include "formats/RSDKv3/gameconfigv3.hpp"
#include "formats/RSDKv3/gfxv3.hpp"
#include "formats/RSDKv3/savefilev3.hpp"
#include "formats/RSDKv3/scenev3.hpp"
#include "formats/RSDKv3/stageconfigv3.hpp"
#include "formats/RSDKv3/tileconfigv3.hpp"
#include "formats/RSDKv3/videov3.hpp"

// RSDKv4
#include "formats/RSDKv4/animationv4.hpp"
#include "formats/RSDKv4/backgroundv4.hpp"
#include "formats/RSDKv4/bytecodev4.hpp"
#include "formats/RSDKv4/chunksv4.hpp"
#include "formats/RSDKv4/datafilev4.hpp"
#include "formats/RSDKv4/gameconfigv4.hpp"
#include "formats/RSDKv4/modelv4.hpp"
#include "formats/RSDKv4/savefilev4.hpp"
#include "formats/RSDKv4/scenev4.hpp"
#include "formats/RSDKv4/stageconfigv4.hpp"
#include "formats/RSDKv4/tileconfigv4.hpp"

// RSDKv5
#include "formats/RSDKv5/palettev5.hpp"
#include "formats/RSDKv5/animationv5.hpp"
#include "formats/RSDKv5/datafilev5.hpp"
#include "formats/RSDKv5/gameconfigv5.hpp"
#include "formats/RSDKv5/modelv5.hpp"
#include "formats/RSDKv5/replayv5.hpp"
#include "formats/RSDKv5/rsdkconfigv5.hpp"
#include "formats/RSDKv5/scenev5.hpp"
#include "formats/RSDKv5/stageconfigv5.hpp"
#include "formats/RSDKv5/staticobjectv5.hpp"
#include "formats/RSDKv5/tileconfigv5.hpp"
#include "formats/RSDKv5/userdbv5.hpp"

// Helpers
#include "utils/formathelpers/gameconfig.hpp"
#include "utils/formathelpers/animation.hpp"
#include "utils/formathelpers/background.hpp"
#include "utils/formathelpers/chunks.hpp"
#include "utils/formathelpers/scene.hpp"
#include "utils/formathelpers/stageconfig.hpp"

#endif //! RSDK_INCLUDE_H
