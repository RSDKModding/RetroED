QT += core

INCLUDEPATH += $$PWD

HEADERS +=    \
    $$PWD/formats/RSDKv1/animationv1.hpp \
    $$PWD/formats/RSDKv1/backgroundv1.hpp \
    $$PWD/formats/RSDKv1/bytecodev1.hpp \
    $$PWD/formats/RSDKv1/characterlistv1.hpp \
    $$PWD/formats/RSDKv1/chunksv1.hpp \
    $$PWD/formats/RSDKv1/datafilev1.hpp \
    $$PWD/formats/RSDKv1/gfxv1.hpp \
    $$PWD/formats/RSDKv1/savefilev1.hpp \
    $$PWD/formats/RSDKv1/scenev1.hpp \
    $$PWD/formats/RSDKv1/scriptv1.hpp \
    $$PWD/formats/RSDKv1/stageconfigv1.hpp \
    $$PWD/formats/RSDKv1/tileconfigv1.hpp \
    $$PWD/formats/RSDKv1/zonelistv1.hpp \
    $$PWD/formats/RSDKv2/animationv2.hpp \
    $$PWD/formats/RSDKv2/backgroundv2.hpp \
    $$PWD/formats/RSDKv2/chunksv2.hpp \
    $$PWD/formats/RSDKv2/datafilev2.hpp \
    $$PWD/formats/RSDKv2/gameconfigv2.hpp \
    $$PWD/formats/RSDKv2/gfxv2.hpp \
    $$PWD/formats/RSDKv2/scenev2.hpp \
    $$PWD/formats/RSDKv2/stageconfigv2.hpp \
    $$PWD/formats/RSDKv2/tileconfigv2.hpp \
    $$PWD/formats/RSDKv2/videov2.hpp \
    $$PWD/formats/RSDKv3/animationv3.hpp \
    $$PWD/formats/RSDKv3/backgroundv3.hpp \
    $$PWD/formats/RSDKv3/bytecodev3.hpp \
    $$PWD/formats/RSDKv3/chunksv3.hpp \
    $$PWD/formats/RSDKv3/datafilev3.hpp \
    $$PWD/formats/RSDKv3/gameconfigv3.hpp \
    $$PWD/formats/RSDKv3/gfxv3.hpp \
    $$PWD/formats/RSDKv3/scenev3.hpp \
    $$PWD/formats/RSDKv3/stageconfigv3.hpp \
    $$PWD/formats/RSDKv3/tileconfigv3.hpp \
    $$PWD/formats/RSDKv3/arccontainerv3.hpp \
    $$PWD/formats/RSDKv3/configv3.hpp \
    $$PWD/formats/RSDKv3/savefilev3.hpp \
    $$PWD/formats/RSDKv3/videov3.hpp \
    $$PWD/formats/RSDKv4/animationv4.hpp \
    $$PWD/formats/RSDKv4/backgroundv4.hpp \
    $$PWD/formats/RSDKv4/bytecodev4.hpp \
    $$PWD/formats/RSDKv4/chunksv4.hpp \
    $$PWD/formats/RSDKv4/datafilev4.hpp \
    $$PWD/formats/RSDKv4/gameconfigv4.hpp \
    $$PWD/formats/RSDKv4/scenev4.hpp \
    $$PWD/formats/RSDKv4/stageconfigv4.hpp \
    $$PWD/formats/RSDKv4/tileconfigv4.hpp \
    $$PWD/formats/RSDKv4/modelv4.hpp \
    $$PWD/formats/RSDKv4/savefilev4.hpp \
    $$PWD/formats/RSDKv5/animationv5.hpp \
    $$PWD/formats/RSDKv5/datafilev5.hpp \
    $$PWD/formats/RSDKv5/gameconfigv5.hpp \
    $$PWD/formats/RSDKv5/modelv5.hpp \
    $$PWD/formats/RSDKv5/palettev5.hpp \
    $$PWD/formats/RSDKv5/replayv5.hpp \
    $$PWD/formats/RSDKv5/rsdkconfigv5.hpp \
    $$PWD/formats/RSDKv5/savefilev5.hpp \
    $$PWD/formats/RSDKv5/scenev5.hpp \
    $$PWD/formats/RSDKv5/stageconfigv5.hpp \
    $$PWD/formats/RSDKv5/staticobjectv5.hpp \
    $$PWD/formats/RSDKv5/tileconfigv5.hpp \
    $$PWD/formats/RSDKv5/userdbv5.hpp \
    $$PWD/io/reader.hpp \
    $$PWD/io/writer.hpp \
    $$PWD/utils/utils.hpp \
    $$PWD/utils/vectors.hpp \
    $$PWD/utils/colour.hpp \
    $$PWD/utils/palette.hpp \
    $$PWD/utils/formathelpers/animation.hpp \
    $$PWD/utils/formathelpers/background.hpp \
    $$PWD/utils/formathelpers/chunks.hpp \
    $$PWD/utils/formathelpers/gameconfig.hpp \
    $$PWD/utils/formathelpers/scene.hpp \
    $$PWD/utils/formathelpers/stageconfig.hpp \
    $$PWD/include.hpp \

SOURCES +=  \
    $$PWD/formats/RSDKv1/animationv1.cpp \
    $$PWD/formats/RSDKv1/backgroundv1.cpp \
    $$PWD/formats/RSDKv1/bytecodev1.cpp \
    $$PWD/formats/RSDKv1/characterlistv1.cpp \
    $$PWD/formats/RSDKv1/chunksv1.cpp \
    $$PWD/formats/RSDKv1/datafilev1.cpp \
    $$PWD/formats/RSDKv1/gfxv1.cpp \
    $$PWD/formats/RSDKv1/savefilev1.cpp \
    $$PWD/formats/RSDKv1/scenev1.cpp \
    $$PWD/formats/RSDKv1/scriptv1.cpp \
    $$PWD/formats/RSDKv1/stageconfigv1.cpp \
    $$PWD/formats/RSDKv1/tileconfigv1.cpp \
    $$PWD/formats/RSDKv1/zonelistv1.cpp \
    $$PWD/formats/RSDKv2/animationv2.cpp \
    $$PWD/formats/RSDKv2/backgroundv2.cpp \
    $$PWD/formats/RSDKv2/chunksv2.cpp \
    $$PWD/formats/RSDKv2/datafilev2.cpp \
    $$PWD/formats/RSDKv2/gameconfigv2.cpp \
    $$PWD/formats/RSDKv2/gfxv2.cpp \
    $$PWD/formats/RSDKv2/scenev2.cpp \
    $$PWD/formats/RSDKv2/stageconfigv2.cpp \
    $$PWD/formats/RSDKv2/tileconfigv2.cpp \
    $$PWD/formats/RSDKv2/videov2.cpp \
    $$PWD/formats/RSDKv3/animationv3.cpp \
    $$PWD/formats/RSDKv3/backgroundv3.cpp \
    $$PWD/formats/RSDKv3/bytecodev3.cpp \
    $$PWD/formats/RSDKv3/chunksv3.cpp \
    $$PWD/formats/RSDKv3/datafilev3.cpp \
    $$PWD/formats/RSDKv3/gameconfigv3.cpp \
    $$PWD/formats/RSDKv3/gfxv3.cpp \
    $$PWD/formats/RSDKv3/scenev3.cpp \
    $$PWD/formats/RSDKv3/stageconfigv3.cpp \
    $$PWD/formats/RSDKv3/tileconfigv3.cpp \
    $$PWD/formats/RSDKv3/arccontainerv3.cpp \
    $$PWD/formats/RSDKv3/configv3.cpp \
    $$PWD/formats/RSDKv3/savefilev3.cpp \
    $$PWD/formats/RSDKv3/videov3.cpp \
    $$PWD/formats/RSDKv4/animationv4.cpp \
    $$PWD/formats/RSDKv4/backgroundv4.cpp \
    $$PWD/formats/RSDKv4/bytecodev4.cpp \
    $$PWD/formats/RSDKv4/chunksv4.cpp \
    $$PWD/formats/RSDKv4/datafilev4.cpp \
    $$PWD/formats/RSDKv4/gameconfigv4.cpp \
    $$PWD/formats/RSDKv4/scenev4.cpp \
    $$PWD/formats/RSDKv4/stageconfigv4.cpp \
    $$PWD/formats/RSDKv4/tileconfigv4.cpp \
    $$PWD/formats/RSDKv4/modelv4.cpp \
    $$PWD/formats/RSDKv4/savefilev4.cpp \
    $$PWD/formats/RSDKv5/animationv5.cpp \
    $$PWD/formats/RSDKv5/datafilev5.cpp \
    $$PWD/formats/RSDKv5/gameconfigv5.cpp \
    $$PWD/formats/RSDKv5/modelv5.cpp \
    $$PWD/formats/RSDKv5/palettev5.cpp \
    $$PWD/formats/RSDKv5/replayv5.cpp \
    $$PWD/formats/RSDKv5/rsdkconfigv5.cpp \
    $$PWD/formats/RSDKv5/savefilev5.cpp \
    $$PWD/formats/RSDKv5/scenev5.cpp \
    $$PWD/formats/RSDKv5/stageconfigv5.cpp \
    $$PWD/formats/RSDKv5/staticobjectv5.cpp \
    $$PWD/formats/RSDKv5/tileconfigv5.cpp \
    $$PWD/formats/RSDKv5/userdbv5.cpp \
    $$PWD/io/reader.cpp \
    $$PWD/io/writer.cpp \
    $$PWD/utils/utils.cpp \
    $$PWD/utils/vectors.cpp \
    $$PWD/utils/colour.cpp \
    $$PWD/utils/palette.cpp \
    $$PWD/utils/formathelpers/animation.cpp \
    $$PWD/utils/formathelpers/background.cpp \
    $$PWD/utils/formathelpers/chunks.cpp \
    $$PWD/utils/formathelpers/gameconfig.cpp \
    $$PWD/utils/formathelpers/scene.cpp \
    $$PWD/utils/formathelpers/stageconfig.cpp \
