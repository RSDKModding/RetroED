#include "includes.hpp"
#include <cmath>
#include "qgifimage.h"

#include "version.hpp"

#include "sceneeditor.hpp"
#include "sceneeditorv5.hpp"
#include "sceneviewer.hpp"

#include "sceneproperties/sceneincludesv5.hpp"
#include "sceneproperties/sceneobjectpropertiesv5.hpp"
#include "sceneproperties/sceneproperties.hpp"
#include "sceneproperties/scenepropertiesv5.hpp"

union PlaceArgs {
    byte size[0x80];
    struct {
        uint texID;
        byte transparency;
    };
};

union CircleArgs {
    byte size[0x80];
    struct {
        float innerR;
        float outerR;
    };
};

SceneViewer::SceneViewer(byte gameType, QWidget *parent) : QOpenGLWidget(parent)
{
    this->gameType = gameType;
    tileSize       = gameType == ENGINE_v5 ? 0x10 : 0x80;

    setMouseTracking(true);

    this->setFocusPolicy(Qt::WheelFocus);

    if (updateTimer)
        delete updateTimer;

    gameEntityListv1 = new GameEntityBasev1[ENTITY_COUNT_v5 * 2];
    gameEntityListv2 = new GameEntityBasev2[ENTITY_COUNT_v5 * 2];
    gameEntityListvU = new GameEntityBasevU[ENTITY_COUNT_v5 * 2];

    updateTimer = new QTimer(this);
    connect(updateTimer, &QTimer::timeout, this, QOverload<>::of(&SceneViewer::updateScene));
    startTimer();
    fpsTimer.start();

    for (int a = 0; a < v5_SPRFILE_COUNT; ++a) {
        spriteAnimationList[a].scope = SCOPE_NONE;
    }
    for (int s = 0; s < v5_SURFACE_MAX; ++s) {
        gfxSurface[s].scope = SCOPE_NONE;
    }

    memset(&sceneInfo, 0, sizeof(sceneInfo));
    memset(&skuInfo, 0, sizeof(skuInfo));
    memset(&gameInfo, 0, sizeof(gameInfo));
    memset(controller, 0, sizeof(controller));
    memset(stickL, 0, sizeof(stickL));
    memset(stickR, 0, sizeof(stickR));
    memset(triggerL, 0, sizeof(triggerL));
    memset(triggerR, 0, sizeof(triggerR));
    memset(&touchMouse, 0, sizeof(touchMouse));
    memset(&unknownInfo, 0, sizeof(unknownInfo));
    memset(screens, 0, sizeof(screens));

    memset(&sceneInfoV1, 0, sizeof(sceneInfoV1));
    memset(&gameInfoV1, 0, sizeof(gameInfoV1));
    memset(controllerV1, 0, sizeof(controllerV1));
    memset(stickLV1, 0, sizeof(stickLV1));
    memset(&touchMouseV1, 0, sizeof(touchMouseV1));

    sceneInfo.entity           = 0;
    sceneInfo.listData         = 0;
    sceneInfo.listCategory     = 0;
    sceneInfo.timeCounter      = 0;
    sceneInfo.currentDrawGroup = 0;
    sceneInfo.currentScreenID  = 0;
    sceneInfo.listPos          = -1;
    sceneInfo.entitySlot       = 0;
    sceneInfo.createSlot       = 0;
    sceneInfo.classCount       = 0;
    sceneInfo.inEditor         = true;
    sceneInfo.effectGizmo      = false;
    sceneInfo.debugMode        = false;
    sceneInfo.useGlobalScripts = false;
    sceneInfo.timeEnabled      = false;
    sceneInfo.activeCategory   = 0;
    sceneInfo.categoryCount    = 0;
    sceneInfo.state            = 1;
    sceneInfo.filter           = 0xFF;
    sceneInfo.milliseconds     = 0;
    sceneInfo.seconds          = 0;
    sceneInfo.minutes          = 0;

    sceneInfoV1.entity           = 0;
    sceneInfoV1.listData         = 0;
    sceneInfoV1.listCategory     = 0;
    sceneInfoV1.timeCounter      = 0;
    sceneInfoV1.currentDrawGroup = 0;
    sceneInfoV1.currentScreenID  = 0;
    sceneInfoV1.listPos          = -1;
    sceneInfoV1.entitySlot       = 0;
    sceneInfoV1.createSlot       = 0;
    sceneInfoV1.classCount       = 0;
    sceneInfoV1.inEditor         = true;
    sceneInfoV1.effectGizmo      = false;
    sceneInfoV1.debugMode        = false;
    sceneInfoV1.useGlobalScripts = false;
    sceneInfoV1.timeEnabled      = false;
    sceneInfoV1.activeCategory   = 0;
    sceneInfoV1.categoryCount    = 0;
    sceneInfoV1.state            = 1;
    sceneInfoV1.milliseconds     = 0;
    sceneInfoV1.seconds          = 0;
    sceneInfoV1.minutes          = 0;

    sprintf(gameInfo.gameTitle, "%s", "RetroED");
    sprintf(gameInfo.gameSubname, "%s", "RetroED");
    sprintf(gameInfo.version, "%s", RE_VERSION);

    sprintf(gameInfoV1.gameTitle, "%s", "RetroED");
    sprintf(gameInfoV1.gameSubname, "%s", "RetroED");
    sprintf(gameInfoV1.version, "%s", RE_VERSION);

    skuInfo.platform    = 0xFF;
    gameInfoV1.platform = 0xFF;

    int vID = 0;
    for (int i = 0; i < vertexListLimit; i++) {
        baseIndexList[vID++] = (i << 2) + 0;
        baseIndexList[vID++] = (i << 2) + 1;
        baseIndexList[vID++] = (i << 2) + 2;
        baseIndexList[vID++] = (i << 2) + 1;
        baseIndexList[vID++] = (i << 2) + 3;
        baseIndexList[vID++] = (i << 2) + 2;

        vertexList[i].color.setX(0);
        vertexList[i].color.setY(0);
        vertexList[i].color.setZ(0);
        vertexList[i].color.setW(0);
    }

    for (int i = 0; i < 0x400; ++i) {
        int o              = i * 4;
        tileUVArray[o + 0] = 0;
        tileUVArray[o + 1] = i * 0x10;
        tileUVArray[o + 2] = 0x10;
        tileUVArray[o + 3] = i * 0x10 + 0x10;

        // FLIP X
        o += 0x400 * 4;
        tileUVArray[o + 0] = tileUVArray[i * 4 + 2];
        tileUVArray[o + 1] = tileUVArray[i * 4 + 1];
        tileUVArray[o + 2] = tileUVArray[i * 4 + 0];
        tileUVArray[o + 3] = tileUVArray[i * 4 + 3];

        // FLIP Y
        o += 0x400 * 4;
        tileUVArray[o + 0] = tileUVArray[i * 4 + 0];
        tileUVArray[o + 1] = tileUVArray[i * 4 + 3];
        tileUVArray[o + 2] = tileUVArray[i * 4 + 2];
        tileUVArray[o + 3] = tileUVArray[i * 4 + 1];

        // FLIP XY
        o += 0x400 * 4;
        tileUVArray[o + 0] = tileUVArray[i * 4 + 2];
        tileUVArray[o + 1] = tileUVArray[i * 4 + 3];
        tileUVArray[o + 2] = tileUVArray[i * 4 + 0];
        tileUVArray[o + 3] = tileUVArray[i * 4 + 1];
    }
}

SceneViewer::~SceneViewer() { dispose(); }

void SceneViewer::startTimer()
{
    if (updateTimer) {
        stopTimer();

        updateTimer->start(0);
    }
}

void SceneViewer::stopTimer()
{
    if (updateTimer) {
        if (updateTimer->isActive())
            updateTimer->stop();
    }
}

void SceneViewer::dispose()
{
    unloadScene();

    if (updateTimer) {
        disconnect(updateTimer, nullptr, nullptr, nullptr);
        updateTimer->stop();
        delete updateTimer;
    }

    VAO->destroy();
    fbpVAO->destroy();

    argInitStage  = "";
    argInitScene  = "";
    argInitFilter = "";

    delete[] gameEntityListv1;
    delete[] gameEntityListv2;
    delete[] gameEntityListvU;
}

void SceneViewer::initScene(QImage tileset)
{
    // unloading
    unloadScene();

    // Get Tiles (for tile list, tileset editing and collision viewer)
    tiles.clear();
    colTexStore = new QImage(0x80, 0x400 * 0x10, QImage::Format_Indexed8);
    colTexStore->setColorTable({ 0xFFFF00FF, 0xFFFFFF00, 0xFFFF0000, 0xFFFFFFFF, 0xFF808000});
    colTexStore->fill(0);
    for (int i = 0; i < 0x400; ++i) {
        int tx         = ((i % (tileset.width() / 0x10)) * 0x10);
        int ty         = ((i / (tileset.width() / 0x10)) * 0x10);
        QImage tileTex = tileset.copy(tx, ty, 0x10, 0x10);

        tiles.append(tileTex);

        for (int c = 0; c < 2; c++){
            for (byte y = 0; y < 16; ++y) {
                for (byte x = 0; x < 16; ++x) {
                    if (gameType != ENGINE_v1){
                        RSDKv5::TileConfig::CollisionMask &cmask = tileconfig.collisionPaths[c][i];
                        // draw pixel collision
                        int dir = !cmask.direction ? y : abs(y - 15);
                        if ((!cmask.direction ? cmask.collision[x].height <= dir : cmask.collision[x].height >= dir) && cmask.collision[x].solid){
                            for (byte m = 0; m < 4; ++m)
                                colTexStore->setPixel(x + tx + c * 0x40 + m * 0x10, dir + ty, m + 1);
                        }
                    } else {
                        RSDKv1::TileConfig::CollisionMask &cmask = tileconfigv1.collisionPaths[c][i];
                        // draw pixel collision
                        for (byte m = 0; m < 4; ++m){
                            int dirX = (m & 1) == 0 ? x : abs(x - 15);
                            int dirY = (m & 2) == 0 ? y : abs(y - 15);
                            if (((m & 2) == 0 ? cmask.collision[m][dirX].height <= dirY : cmask.collision[m][dirX].height >= dirY) && cmask.collision[m][dirX].solid) {
                                colTexStore->setPixel(dirX + tx + c * 0x40 + m * 0x10, dirY + ty, 3);
                            }
                        }
                    }
                }
            }
        }
    }

    // Get Tile Palette (for tileset editing)
    auto pal = tileset.colorTable();
    tilePalette.clear();
    for (QRgb &col : pal) {
        tilePalette.append(PaletteColor(col));
    }

    chunks.clear();
    if (gameType != ENGINE_v5) {
        for (FormatHelpers::Chunks::Chunk &c : chunkset.chunks) {
            chunks.append(c.getImage(tiles));
        }
    }

    // Tile Texture
    GLint active;
    glFuncs->glGetIntegerv(GL_ACTIVE_TEXTURE, &active);
    gfxSurface[0].scope      = SCOPE_STAGE;
    gfxSurface[0].name       = "Tileset";
    gfxSurface[0].width      = tileset.width();
    gfxSurface[0].height     = tileset.height();
    gfxSurface[0].texturePtr = createTexture(tileset, QOpenGLTexture::Target2D);
    gfxSurface[0].transClr   = tilePalette[0].toQColor();

    glFuncs->glActiveTexture(GL_TEXTURE30);
    glFuncs->glBindTexture(GL_TEXTURE_RECTANGLE, gfxSurface[0].texturePtr->textureId());
    glFuncs->glActiveTexture(active);

    gfxSurface[1].scope      = SCOPE_STAGE;
    gfxSurface[1].name       = "Collision A";
    gfxSurface[1].width      = colTexStore->width();
    gfxSurface[1].height     = colTexStore->height();
    gfxSurface[1].texturePtr = createTexture(*colTexStore, QOpenGLTexture::Target2D);
    gfxSurface[1].transClr   = QColor(0xFFFF00FF);

    gfxSurface[2].scope      = SCOPE_STAGE;
    gfxSurface[2].name       = "Collision B";
    gfxSurface[2].width      = colTexStore->width();
    gfxSurface[2].height     = colTexStore->height();
    gfxSurface[2].texturePtr = createTexture(*colTexStore, QOpenGLTexture::Target2D);
    gfxSurface[2].transClr   = QColor(0xFFFF00FF);

    // Default Texture
    if (gfxSurface[3].scope == SCOPE_NONE) {
        gfxSurface[3].scope = SCOPE_GLOBAL;
        gfxSurface[3].name  = gameType == ENGINE_v5 ? ":/icons/missingV5.png" : ":/icons/missing.png";
        missingObj          = QImage(gfxSurface[3].name);
        gfxSurface[3].texturePtr = createTexture(missingObj);
        Utils::getHashInt(gfxSurface[3].name, gfxSurface[3].hash);
        gfxSurface[3].width    = missingObj.width();
        gfxSurface[3].height   = missingObj.height();
        gfxSurface[3].transClr = QColor(0xFFFF00FF);
    }

    // Default Texture
    if (gameType == ENGINE_v1 && gfxSurface[4].scope == SCOPE_NONE) {
        gfxSurface[4].scope      = SCOPE_GLOBAL;
        gfxSurface[4].name       = ":/icons/player_v1.png";
        missingObj               = QImage(gfxSurface[4].name);
        gfxSurface[4].texturePtr = createTexture(missingObj);
        Utils::getHashInt(gfxSurface[4].name, gfxSurface[4].hash);
        gfxSurface[4].width    = missingObj.width();
        gfxSurface[4].height   = missingObj.height();
        gfxSurface[4].transClr = QColor(0xFFFF00FF);
    }
}

void SceneViewer::updateScene()
{
    this->repaint();

    argInitStage  = currentFolder;
    argInitScene  = currentSceneID;
    argInitFilter = QString::number(sceneFilter);

    if (statusLabel) {
        int mx = (int)((mousePos.x * invZoom()) + cameraPos.x);
        int my = (int)((mousePos.y * invZoom()) + cameraPos.y);

        QString status =
            QString("Zoom: %1%, Mouse Position: (%2, %3), %4 Position: (%5, %6), Selected %4: "
                    "%7, Selected Layer: %8 (%9), Selected Object: %10")
                .arg(zoom * 100)
                .arg(mx)
                .arg(my)
                .arg(gameType == ENGINE_v5 ? "Tile" : "Chunk")
                .arg((int)mx / tileSize)
                .arg((int)my / tileSize)
                .arg(gameType == ENGINE_v5 ? selectedTile : selectedChunk)
                .arg(selectedLayer)
                .arg(selectedLayer >= 0 && selectedLayer < layers.count() ? layers[selectedLayer].name
                                                                          : "[None]")
                .arg(selectedObject >= 0 && selectedObject < objects.count()
                         ? objects[selectedObject].name
                         : "[None]");
        if (gameType == ENGINE_v5){
            status += QString(", Selected Stamp: %1").arg(selectedStamp);
            if (engineRevision != 1)
                status += QString(", Filter: %1").arg(sceneFilter);
        }
        status += QString(", FPS: %1").arg(avgFps, 0, 'f', 1);
        statusLabel->setText(status);
    }

    if (v5Editor) {
        if (v5Editor->scnProp) {
            int activeCount = 0, inactiveCount = 0;
            for (auto &entity : entities) {
                int filter = 0xFF;
                for (int v = 0; v < objects[entity.type].variables.count(); ++v) {
                    if (objects[entity.type].variables[v].name == "filter") {
                        if (v < entity.variables.count())
                            filter = entity.variables[v].value_uint8;
                        break;
                    }
                }

                if (!(filter & sceneFilter) && filter)
                    ++inactiveCount;
                else
                    ++activeCount;
            }

            v5Editor->scnProp->layerCnt->setText(
                QString("Tile Layer Count: %1 Layers").arg(layers.count()));
            v5Editor->scnProp->objCnt->setText(
                QString("Object Count: %1 Objects").arg(objects.count()));
            v5Editor->scnProp->entCntTot->setText(
                QString("Total Entity Count: %1 Entities").arg(entities.count()));
            v5Editor->scnProp->entCntAct->setText(
                QString("Active Entity Count: %1 Entities").arg(activeCount));
            v5Editor->scnProp->entCntInac->setText(
                QString("Inactive Entity Count: %1 Entities").arg(inactiveCount));
        }
    }
    else if (scnEditor) {
        scnEditor->scnProp->objCnt->setText(QString("Object Count: %1 Objects").arg(objects.count()));
        scnEditor->scnProp->entCntTot->setText(
            QString("Entity Count: %1 Entities").arg(entities.count()));
    }
}


void SceneViewer::updateTileColMap(RSDKv5::TileConfig::CollisionMask *cmask, ushort sel, int colLyr)
{
    int tx         = ((sel % (gfxSurface[0].width / 0x10)) * 0x10);
    int ty         = ((sel / (gfxSurface[0].width / 0x10)) * 0x10);
    // draw pixel collision
    for (byte x = 0; x < 16; x++){
        for (byte y = 0; y < 16; y++){
            int dir = !cmask->direction ? y : abs(y - 15);
            for (byte m = 0; m < 4; ++m){
                int pos = x + tx + colLyr * 0x40 + m * 0x10;
                colTexStore->setPixel(pos, dir + ty, 0);
                if (!cmask->direction ? cmask->collision[x].height <= dir : cmask->collision[x].height >= dir) {
                    if (cmask->collision[x].solid)
                        colTexStore->setPixel(pos, dir + ty, m + 1);
                }
            }
        }
    }

    delete gfxSurface[colLyr + 1].texturePtr;
    gfxSurface[colLyr + 1].texturePtr = createTexture(*colTexStore, QOpenGLTexture::Target2D);
}

void SceneViewer::updateChunkColMap()
{
    colTexStore->fill(0);
    for (int i = 0; i < 0x400; ++i){
        int tx         = ((i % (gfxSurface[0].width / 0x10)) * 0x10);
        int ty         = ((i / (gfxSurface[0].width / 0x10)) * 0x10);
        for (int c = 0; c < 2; ++c){
            // draw pixel collision
            for (byte x = 0; x < 16; x++){
                for (byte y = 0; y < 16; y++){
                    if (gameType != ENGINE_v1){
                        RSDKv5::TileConfig::CollisionMask &cmask = tileconfig.collisionPaths[c][i];
                        // draw pixel collision
                        int dir = !cmask.direction ? y : abs(y - 15);
                        for (byte m = 0; m < 4; ++m){
                            int pos = x + tx + c * 0x40 + m * 0x10;
                            colTexStore->setPixel(pos, dir + ty, 0);
                            if ((!cmask.direction ? cmask.collision[x].height <= dir : cmask.collision[x].height >= dir) && cmask.collision[x].solid)
                                colTexStore->setPixel(pos, dir + ty, m + 1);
                        }
                    } else {
                        RSDKv1::TileConfig::CollisionMask &cmask = tileconfigv1.collisionPaths[c][i];
                        // draw pixel collision
                        for (byte m = 0; m < 4; ++m){
                            int dirX = (m & 1) == 0 ? x : abs(x - 15);
                            int dirY = (m & 2) == 0 ? y : abs(y - 15);
                            int pos = dirX + tx + c * 0x40 + m * 0x10;
                            colTexStore->setPixel(pos, dirY + ty, 0);
                            if (((m & 2) == 0 ? cmask.collision[m][dirX].height <= dirY : cmask.collision[m][dirX].height >= dirY) && cmask.collision[m][dirX].solid)
                                colTexStore->setPixel(pos, dirY + ty, 3);
                        }
                    }
                }
            }
        }
    }
    delete gfxSurface[1].texturePtr;
    delete gfxSurface[2].texturePtr;
    gfxSurface[1].texturePtr = createTexture(*colTexStore, QOpenGLTexture::Target2D);
    gfxSurface[2].texturePtr = createTexture(*colTexStore, QOpenGLTexture::Target2D);
}

void SceneViewer::updateChunkColTile(RSDKv5::TileConfig::CollisionMask *cmask, ushort sel, int colLyr)
{
    int tx         = ((sel % (gfxSurface[0].width / 0x10)) * 0x10);
    int ty         = ((sel / (gfxSurface[0].width / 0x10)) * 0x10);
    // draw pixel collision
    for (byte x = 0; x < 16; x++){
        for (byte y = 0; y < 16; y++){
            int dir = !cmask->direction ? y : abs(y - 15);
            for (byte m = 0; m < 4; ++m){
                int pos = x + tx + colLyr * 0x40 + m * 0x10;
                colTexStore->setPixel(pos, dir + ty, 0);
                if ((!cmask->direction ? cmask->collision[x].height <= dir : cmask->collision[x].height >= dir) && cmask->collision[x].solid) {
                    colTexStore->setPixel(pos, dir + ty, m + 1);
                }
            }
        }
    }

    delete gfxSurface[colLyr + 1].texturePtr;
    gfxSurface[colLyr + 1].texturePtr = createTexture(*colTexStore, QOpenGLTexture::Target2D);
}

void SceneViewer::updateChunkColTilev1(RSDKv1::TileConfig::CollisionMask *cmask, ushort sel, int colLyr)
{
    int tx         = ((sel % (gfxSurface[0].width / 0x10)) * 0x10);
    int ty         = ((sel / (gfxSurface[0].width / 0x10)) * 0x10);
    // draw pixel collision
    for (byte x = 0; x < 16; x++){
        for (byte y = 0; y < 16; y++){
            // draw pixel collision
            for (byte m = 0; m < 4; ++m){
                int dirX = (m & 1) == 0 ? x : abs(x - 15);
                int dirY = (m & 2) == 0 ? y : abs(y - 15);
                int pos = dirX + tx + colLyr * 0x40 + m * 0x10;
                colTexStore->setPixel(pos, dirY + ty, 0);
                if (((m & 2) == 0 ? cmask->collision[m][dirX].height <= dirY : cmask->collision[m][dirX].height >= dirY) && cmask->collision[m][dirX].solid) {
                    colTexStore->setPixel(pos, dirY + ty, 3);
                }
            }
        }
    }

    delete gfxSurface[colLyr + 1].texturePtr;
    gfxSurface[colLyr + 1].texturePtr = createTexture(*colTexStore, QOpenGLTexture::Target2D);
}

void SceneViewer::drawScene()
{
    // Constant stuff
    float iZoom = invZoom();

    int layerSizeX = 0;
    int layerSizeY = 0;
    for (int i = 0; i < layers.count(); ++i) {
        if ((layers[i].width * tileSize) > layerSizeX)
            layerSizeX = layers[i].width * tileSize;
        if ((layers[i].height * tileSize) > layerSizeY)
            layerSizeY = layers[i].height * tileSize;
    }

    Vector4<float> c = { metadata.backgroundColor2.red() / 255.f,
                         metadata.backgroundColor2.green() / 255.f,
                         metadata.backgroundColor2.blue() / 255.f, 1.f };

    float bgXOffset    = 0;
    float bgSquareSize = 0x100 * iZoom;
    float bgScreenW    = storedW * invZoom();
    float bgScreenH    = storedH * invZoom();
    for (float y = 0; y < layerSizeY; y += bgSquareSize) {
        for (float x = 0; x < layerSizeX; x += bgSquareSize * 2) {
            float rx = x + bgXOffset;

            // c[xy] is the center pos
            float cx = rx + 128 * invZoom();
            float cy = y + 128 * invZoom();

            bool32 collided =
                cx - bgSquareSize < cameraPos.x + bgScreenW && cx + bgSquareSize > cameraPos.x
                && cy - bgSquareSize < cameraPos.y + bgScreenH && cy + bgSquareSize > cameraPos.y;

            if (collided)
                drawRect(rx - cameraPos.x, y - cameraPos.y, bgSquareSize, bgSquareSize, c);
        }

        bgXOffset = fmod(bgXOffset + bgSquareSize, bgSquareSize * 2);
    }

    bool showCLayers[2] = { showPlaneA, showPlaneB };

    for (int l = 0; l < v5_DRAWGROUP_COUNT; ++l) {
        drawLayers[l].layerDrawList.clear();
    }

    for (int t = 0; t < layers.count(); ++t) {
        byte drawOrder = layers[t].drawGroup;
        if (drawOrder < v5_DRAWGROUP_COUNT)
            drawLayers[drawOrder].layerDrawList.append(t);
    }

    // if (fileRender)
    //     addStatusProgress(0.2); // finished rendering bg

    // LAYERS
    for (int p = 0; p < v5_DRAWGROUP_COUNT; ++p) {
        // NOTE FOR FUTURE RDC:
        // Pre-Program the layer drawOrders for <= v4 (since they cant be changed)
        // so BGLayer 8 is order 0, BGLayer 0 is order 7 & FG is 8, etc etc

        for (int l = layers.count() - 1; l >= 0; --l) {
            if (layers[l].drawGroup != p && !(p == 15 && layers[l].drawGroup >= 16)) {
                if (!(selectedLayer == l && p == 15))
                    continue;
            }
            if (!layers[l].visible)
                continue;

            // TILE LAYERS
            int camX = cameraPos.x;
            int camY = cameraPos.y;
            camX     = qMax(camX, 0);
            camY     = qMax(camY, 0);

            int basedX = qMax(camX / tileSize, 0);
            int basedY = qMax(camY / tileSize, 0);

            int sw = (storedW * iZoom);
            int sh = (storedH * iZoom);

            int countX = layers[l].width * tileSize > sw ? (sw / tileSize) : layers[l].width;
            int countY = layers[l].height * tileSize > sh ? (sh / tileSize) : layers[l].height;

            countX = qMin(basedX + countX + 2, (int)layers[l].width);
            countY = qMin(basedY + countY + 2, (int)layers[l].height);

            renderRenderStates();

            int count = 0;
            if (gameType == ENGINE_v5) {
                // Draw Tile-Based TileMap
                for (int y = basedY; y < countY; ++y) {
                    const QList<ushort> *row = &layers[l].layout.at(y);
                    for (int x = basedX; x < countX; ++x) {
                        ushort tile = row->at(x);
                        if (tile != 0xFFFF) {
                            ++count;
                            float xp = (x * 0x10) - cameraPos.x;
                            float yp = (y * 0x10) - cameraPos.y;

                            ushort t     = (tile & 0x3FF);
                            byte f       = (tile >> 10) & 3;
                            ushort point = (t << 2) | (f << 12);
                            addPoly(xp, yp, tileUVArray[point], tileUVArray[point + 1], 0, gfxSurface);
                            addPoly(xp + 0x10, yp, tileUVArray[point + 2], tileUVArray[point + 1], 0,
                                    gfxSurface);
                            addPoly(xp, yp + 0x10, tileUVArray[point], tileUVArray[point + 3], 0,
                                    gfxSurface);
                            addPoly(xp + 0x10, yp + 0x10, tileUVArray[point + 2],
                                    tileUVArray[point + 3], 0, gfxSurface);

                            // safety pass
                            if (renderCount >= vertexListLimit - 8) {
                                PlaceArgs args;
                                args.texID = 0;

                                renderCount -= count * 4;
                                addRenderState((selectedLayer == l || fileRender) ? INK_NONE
                                                                                  : INK_BLEND,
                                               count * 4, count * 6, &args, 0xFF, &placeShader);
                                renderCount += count * 4;
                                renderRenderStates();
                                count = 0;
                            }
                        }
                    }
                }
            }
            else {
                // Draw Chunk-Based TileMap
                for (int y = basedY; y < countY; ++y) {
                    const QList<ushort> *row = &layers[l].layout.at(y);
                    for (int x = basedX; x < countX; ++x) {
                        ushort chunkID = row->at(x);
                        if (chunkID != 0x0 && chunkID < 0x200) {
                            for (int ty = 0; ty < 8; ++ty) {
                                for (int tx = 0; tx < 8; ++tx) {
                                    FormatHelpers::Chunks::Tile &info =
                                        chunkset.chunks[chunkID].tiles[ty][tx];

                                    ++count;
                                    float xp = ((x * tileSize) + (tx * 0x10)) - cameraPos.x;
                                    float yp = ((y * tileSize) + (ty * 0x10)) - cameraPos.y;

                                    ushort point = (info.tileIndex << 2) | (info.direction << 12);
                                    addPoly(xp, yp, tileUVArray[point], tileUVArray[point + 1], 0,
                                            gfxSurface);
                                    addPoly(xp + 0x10, yp, tileUVArray[point + 2],
                                            tileUVArray[point + 1], 0, gfxSurface);
                                    addPoly(xp, yp + 0x10, tileUVArray[point], tileUVArray[point + 3],
                                            0, gfxSurface);
                                    addPoly(xp + 0x10, yp + 0x10, tileUVArray[point + 2],
                                            tileUVArray[point + 3], 0, gfxSurface);

                                    // safety pass
                                    if (renderCount >= vertexListLimit - 8) {
                                        PlaceArgs args;
                                        args.texID = 0;

                                        renderCount -= count * 4;
                                        addRenderState((selectedLayer == l || fileRender) ? INK_NONE
                                                                                          : INK_BLEND,
                                                       count * 4, count * 6, &args, 0xFF, &placeShader);
                                        renderCount += count * 4;
                                        renderRenderStates();
                                        count = 0;
                                    }
                                }
                            }
                        }
                    }
                }
            }

            renderCount -= count * 4;
            PlaceArgs args;
            args.texID = 0;
            addRenderState((selectedLayer == l || fileRender) ? INK_NONE : INK_BLEND, count * 4,
                           count * 6, &args, 0xFF, &placeShader);
            renderCount += count * 4;
            renderRenderStates();

            if (fileRender)
                continue;

            // welcome to the magic of rmg code. dangerous-looking code ahead
            // there's definitely better ways to do this, but for now this is what we gotta do
            // TODO: 2022 rmg here. this is shitty even if it works well. find someway else to do this
            // TODO (RDC Edition): once rmg's done here, adapt it for <= v4 like above
            // Reject stxtic code, embrace Leonx254 code >:)

            // Collision Previews
            for (int c = 0; c < 2 && l == selectedLayer; ++c) {
                if (showCLayers[c]){
                    int countCol = 0;
                    for (int y = basedY; y < countY; ++y) {
                        const QList<ushort> *row = &layers[l].layout.at(y);
                        for (int x = basedX; x < countX; ++x) {
                            if (gameType == ENGINE_v5){
                                ushort tile = row->at(x);
                                if (tile != 0xFFFF) {
                                    byte solidity = !c ? (tile >> 12) & 3 : (tile >> 14) & 3;
                                    if (!solidity)
                                        continue;
                                    ++countCol;
                                    float xp = (x * 0x10) - cameraPos.x;
                                    float yp = (y * 0x10) - cameraPos.y;

                                    ushort t     = (tile & 0x3FF);
                                    byte f       = (tile >> 10) & 3;
                                    ushort point = (t << 2) | (f << 12);
                                    ushort texOffset = (solidity - 1) * 0x10 + c * 0x40;
                                    addPoly(xp, yp, tileUVArray[point] + texOffset, tileUVArray[point + 1], 0, &gfxSurface[c + 1]);

                                    addPoly(xp + 0x10, yp, tileUVArray[point + 2] + texOffset, tileUVArray[point + 1], 0,
                                            &gfxSurface[c + 1]);
                                    addPoly(xp, yp + 0x10, tileUVArray[point] + texOffset, tileUVArray[point + 3], 0,
                                            &gfxSurface[c + 1]);
                                    addPoly(xp + 0x10, yp + 0x10, tileUVArray[point + 2] + texOffset,
                                            tileUVArray[point + 3], 0, &gfxSurface[c + 1]);
                                    // safety pass
                                    if (renderCount >= vertexListLimit - 8) {
                                        PlaceArgs args;
                                        args.texID = 0;

                                        renderCount -= countCol * 4;
                                        addRenderState(INK_NONE, countCol * 4, countCol * 6, &args, 0xFF, &placeShader);
                                        renderCount += countCol * 4;
                                        renderRenderStates();
                                        countCol = 0;
                                    }
                                }
                            } else {
                                ushort chunkID = row->at(x);
                                if (chunkID != 0x0 && chunkID < 0x200) {
                                    for (int ty = 0; ty < 8; ++ty) {
                                        for (int tx = 0; tx < 8; ++tx) {
                                            FormatHelpers::Chunks::Tile tile = chunkset.chunks[chunkID].tiles[ty][tx];
                                            byte solidity = !c ? tile.solidityA : tile.solidityB;
                                            if (solidity == 3)
                                                continue;
                                            ++countCol;

                                            float xp = ((x * tileSize) + (tx * 0x10)) - cameraPos.x;
                                            float yp = ((y * tileSize) + (ty * 0x10)) - cameraPos.y;

                                            byte solColor[5] = {2, 0, 1, 0, 3};
                                            ushort point = (tile.tileIndex << 2) | (tile.direction << 12);
                                            ushort texOffset = gameType == ENGINE_v1 ? c * 0x40 : c * 0x40 + solColor[solidity] * 0x10;

                                            addPoly(xp, yp, tileUVArray[point] + texOffset, tileUVArray[point + 1], 0,
                                                    &gfxSurface[c + 1]);
                                            addPoly(xp + 0x10, yp, tileUVArray[point + 2] + texOffset,
                                                    tileUVArray[point + 1], 0, &gfxSurface[c + 1]);
                                            addPoly(xp, yp + 0x10, tileUVArray[point] + texOffset, tileUVArray[point + 3],
                                                    0, &gfxSurface[c + 1]);
                                            addPoly(xp + 0x10, yp + 0x10, tileUVArray[point + 2] + texOffset,
                                                    tileUVArray[point + 3], 0, &gfxSurface[c + 1]);

                                            // safety pass
                                            if (renderCount >= vertexListLimit - 8) {
                                                PlaceArgs args;
                                                args.texID = 0;

                                                renderCount -= countCol * 4;
                                                addRenderState((selectedLayer == l || fileRender) ? INK_NONE
                                                                                                  : INK_BLEND,
                                                               countCol * 4, countCol * 6, &args, 0xFF, &placeShader);
                                                renderCount += countCol * 4;
                                                renderRenderStates();
                                                countCol = 0;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    renderCount -= countCol * 4;
                    PlaceArgs args;
                    args.texID = c + 1;
                    addRenderState(INK_NONE, countCol * 4, countCol * 6, &args, 0xFF, &placeShader);
                    renderCount += countCol * 4;
                    renderRenderStates();
                }
            }

            // PARALLAX
            if (l == selectedLayer && l >= 0) {
                if (layers[l].type == SceneHelpers::TileLayer::LAYER_HSCROLL
                    || layers[l].type == SceneHelpers::TileLayer::LAYER_VSCROLL) {
                    if (showParallax) {
                        SceneHelpers::TileLayer::ScrollIndexInfo *selInfo = nullptr;

                        if (useLayerScrollInfo) {
                            int id = 0;
                            for (SceneHelpers::TileLayer::ScrollIndexInfo &info :
                                 layers[l].scrollInfos) {
                                if (selectedHScrollInfo == id) {
                                    selInfo = &info;
                                    ++id;
                                    continue;
                                }

                                Vector4<float> clr(1.0f, 1.0f, 0.0f, 1.0f);
                                if (layers[l].type == SceneHelpers::TileLayer::LAYER_HSCROLL) {
                                    for (auto &instance : info.instances) {
                                        int w     = layers[l].width * tileSize;
                                        int start = instance.startLine;
                                        int end   = instance.startLine + instance.length;

                                        drawLine(0.0f, start - cameraPos.y, w - cameraPos.x,
                                                 start - cameraPos.y, clr);

                                        drawLine(0.0f, end - cameraPos.y, w - cameraPos.x,
                                                 end - cameraPos.y, clr);
                                    }
                                }
                                else if (layers[l].type == SceneHelpers::TileLayer::LAYER_VSCROLL) {
                                    for (auto &instance : info.instances) {
                                        int h     = layers[l].height * tileSize;
                                        int start = instance.startLine;
                                        int end   = instance.startLine + instance.length;

                                        drawLine(start - cameraPos.x, 0.0f, start - cameraPos.x,
                                                 h - cameraPos.y, clr);

                                        drawLine(end - cameraPos.x, 0.0f, end - cameraPos.x,
                                                 h - cameraPos.y, clr);
                                    }
                                }

                                ++id;
                            }

                            if (selInfo) {
                                Vector4<float> clr(0.0f, 0.0f, 1.0f, 1.0f);

                                if (layers[l].type == SceneHelpers::TileLayer::LAYER_HSCROLL) {
                                    for (auto &instance : selInfo->instances) {
                                        int w     = layers[l].width * tileSize;
                                        int start = instance.startLine;
                                        int end   = instance.startLine + instance.length;

                                        drawLine(0.0f, start - cameraPos.y, w - cameraPos.x,
                                                 start - cameraPos.y, clr);

                                        drawLine(0.0f, end - cameraPos.y, w - cameraPos.x,
                                                 end - cameraPos.y, clr);
                                    }
                                }
                                else if (layers[l].type == SceneHelpers::TileLayer::LAYER_VSCROLL) {
                                    for (auto &instance : selInfo->instances) {
                                        int h     = layers[l].height * tileSize;
                                        int start = instance.startLine;
                                        int end   = instance.startLine + instance.length;

                                        drawLine(start - cameraPos.x, 0.0f, start - cameraPos.x,
                                                 h - cameraPos.y, clr);

                                        drawLine(end - cameraPos.x, 0.0f, end - cameraPos.x,
                                                 h - cameraPos.y, clr);
                                    }
                                }
                            }
                        }
                        else {
                            selInfo = nullptr;
                            int id  = 0;
                            for (SceneHelpers::TileLayer::ScrollIndexInfo &info : hScroll) {
                                if (selectedHScrollInfo == id) {
                                    selInfo = &info;
                                    ++id;
                                    continue;
                                }

                                Vector4<float> clr(1.0f, 1.0f, 0.0f, 1.0f);
                                if (layers[l].type == SceneHelpers::TileLayer::LAYER_HSCROLL) {
                                    for (auto &instance : info.instances) {
                                        if (instance.layerID != l)
                                            continue;

                                        int w     = layers[l].width * tileSize;
                                        int start = instance.startLine;
                                        int end   = instance.startLine + instance.length;

                                        drawLine(0.0f, start - cameraPos.y, w - cameraPos.x,
                                                 start - cameraPos.y, clr);

                                        drawLine(0.0f, end - cameraPos.y, w - cameraPos.x,
                                                 end - cameraPos.y, clr);
                                    }
                                }

                                ++id;
                            }

                            // Selected HScroll
                            if (selInfo) {
                                Vector4<float> clr(0.0f, 0.0f, 1.0f, 1.0f);

                                if (layers[l].type == SceneHelpers::TileLayer::LAYER_HSCROLL) {
                                    for (auto &instance : selInfo->instances) {
                                        if (instance.layerID != l)
                                            continue;

                                        int w     = layers[l].width * tileSize;
                                        int start = instance.startLine;
                                        int end   = instance.startLine + instance.length;

                                        drawLine(0.0f, start - cameraPos.y, w - cameraPos.x,
                                                 start - cameraPos.y, clr);

                                        drawLine(0.0f, end - cameraPos.y, w - cameraPos.x,
                                                 end - cameraPos.y, clr);
                                    }
                                }
                            }

                            selInfo = nullptr;
                            id      = 0;
                            for (SceneHelpers::TileLayer::ScrollIndexInfo &info : vScroll) {
                                if (selectedVScrollInfo == id) {
                                    selInfo = &info;
                                    ++id;
                                    continue;
                                }

                                Vector4<float> clr(1.0f, 1.0f, 0.0f, 1.0f);
                                if (layers[l].type == SceneHelpers::TileLayer::LAYER_VSCROLL) {
                                    for (auto &instance : info.instances) {
                                        if (instance.layerID != l)
                                            continue;

                                        int h     = layers[l].height * tileSize;
                                        int start = instance.startLine;
                                        int end   = instance.startLine + instance.length;

                                        drawLine(start - cameraPos.x, 0.0f, start - cameraPos.x,
                                                 h - cameraPos.y, clr);

                                        drawLine(end - cameraPos.x, 0.0f, end - cameraPos.x,
                                                 h - cameraPos.y, clr);
                                    }
                                }

                                ++id;
                            }

                            // Selected VScroll
                            if (selInfo) {
                                Vector4<float> clr(0.0f, 0.0f, 1.0f, 1.0f);

                                if (layers[l].type == SceneHelpers::TileLayer::LAYER_VSCROLL) {
                                    for (auto &instance : selInfo->instances) {
                                        if (instance.layerID != l)
                                            continue;

                                        int h     = layers[l].height * tileSize;
                                        int start = instance.startLine;
                                        int end   = instance.startLine + instance.length;

                                        drawLine(start - cameraPos.x, 0.0f, start - cameraPos.x,
                                                 h - cameraPos.y, clr);

                                        drawLine(end - cameraPos.x, 0.0f, end - cameraPos.x,
                                                 h - cameraPos.y, clr);
                                    }
                                }

                                ++id;
                            }
                        }
                    }
                }
            }
        }
    }

    // if (fileRender)
    //     addStatusProgress(0.2); // finished rendering layers

    // ENTITIES
    Rect<float> viewArea = Rect<float>(cameraPos.x - 32, cameraPos.y - 32,
                                       storedW * invZoom() + cameraPos.x + 32, storedH * invZoom() + cameraPos.y + 32);
    for (int p = 0; p < v5_DRAWGROUP_COUNT; ++p) {
        sceneInfo.currentDrawGroup   = p;
        sceneInfoV1.currentDrawGroup = p;
        for (int o = 0; o < drawLayers[p].entries.count(); ++o) {
            SceneEntity *entity = &entities[drawLayers[p].entries[o]];
            activeDrawEntity    = entity;
            entity->box         = Rect<int>(0, 0, 0, 0);

            validDraw = false;

            if (!objects[entity->type].visible)
                continue;

            int filter = 0xFF;
            for (int v = 0; v < objects[entity->type].variables.count(); ++v) {
                if (objects[entity->type].variables[v].name == "filter") {
                    if (v < entity->variables.count())
                        filter = entity->variables[v].value_uint8;
                    break;
                }
            }

            if (!(filter & sceneFilter) && filter)
                continue;

            if (drawLayers[p].entries[o] == selectedEntity
                || selectedEntities.indexOf(drawLayers[p].entries[o]) >= 0)
                continue;

            if (entity->type != 0) {
                if (gameType == ENGINE_v5)
                    emit callGameEventv5(objects[entity->type].name, EVENT_DRAW, entity);
                else
                    emit callGameEvent(EVENT_DRAW, drawLayers[p].entries[o]);
            }

            // Draw Default Object Sprite if invalid
            if (!validDraw) {
                entity->box = Rect<int>(-0x10, -0x10, 0x10, 0x10);

                Vector2<float> pos = Vector2<float>(
                    entity->pos.x,
                    entity->pos.y);

                // Make sure is within the viewable area
                if (pos.x >= viewArea.x && pos.y >= viewArea.y && pos.x < viewArea.w && pos.y < viewArea.h){
                    drawSpriteFlipped((pos.x - cameraPos.x) - (gfxSurface[3].width >> 1), (pos.y - cameraPos.y) - (gfxSurface[3].height >> 1),
                                      gfxSurface[3].width, gfxSurface[3].height, 0, 0, FLIP_NONE, INK_NONE,
                                      0xFF, 3);
                }
            }
            if (fileRender){
                // render on smaller batches when exporting the image
                if (renderStateCount > 0xFF || renderCount >= vertexListLimit - 8)
                    renderRenderStates();
            }
        }
    }

    // Draw selected entity (single)
    if (selectedEntity >= 0 && selectedEntity < entities.count()) {
        SceneEntity *entity = &entities[selectedEntity];
        activeDrawEntity    = entity;
        entity->box         = Rect<int>(0, 0, 0, 0);

        validDraw = false;

        if (entity->type != 0) {
            if (gameType == ENGINE_v5)
                emit callGameEventv5(objects[entity->type].name, EVENT_DRAW, entity);
            else
                emit callGameEvent(EVENT_DRAW, selectedEntity);
        }

        // Draw Default Object Sprite if invalid
        if (!validDraw) {
            entity->box = Rect<int>(-0x10, -0x10, 0x10, 0x10);

            float xpos = entity->pos.x - (cameraPos.x);
            float ypos = entity->pos.y - (cameraPos.y);

            drawSpriteFlipped(xpos - (gfxSurface[3].width >> 1), ypos - (gfxSurface[3].height >> 1),
                              gfxSurface[3].width, gfxSurface[3].height, 0, 0, FLIP_NONE, INK_NONE,
                              0xFF, 3);
        }
    }

    int storedSelectedEntity = selectedEntity;
    int storedListPos        = sceneInfo.listPos;

    // Draw selected entity (multiple)
    for (int s = 0; s < selectedEntities.count(); ++s) {
        if (selectedEntities[s] >= 0 && selectedEntities[s] < entities.count()) {
            selectedEntity    = selectedEntities[s];
            sceneInfo.listPos = selectedEntities[s];

            SceneEntity *entity = &entities[selectedEntity];
            activeDrawEntity    = entity;
            entity->box         = Rect<int>(0, 0, 0, 0);

            validDraw = false;

            if (entity->type != 0) {
                if (gameType == ENGINE_v5)
                    emit callGameEventv5(objects[entity->type].name, EVENT_DRAW, entity);
                else
                    emit callGameEvent(EVENT_DRAW, selectedEntity);
            }

            // Draw Default Object Sprite if invalid
            if (!validDraw) {
                entity->box = Rect<int>(-0x10, -0x10, 0x10, 0x10);

                float xpos = entity->pos.x - (cameraPos.x);
                float ypos = entity->pos.y - (cameraPos.y);

                drawSpriteFlipped(xpos - (gfxSurface[3].width >> 1), ypos - (gfxSurface[3].height >> 1),
                                  gfxSurface[3].width, gfxSurface[3].height, 0, 0, FLIP_NONE, INK_NONE,
                                  0xFF, 3);
            }
        }
    }

    selectedEntity    = storedSelectedEntity;
    sceneInfo.listPos = storedListPos;

    // Draw Retro-Sonic Player spawn point
    if (gameType == ENGINE_v1) {
        SceneEntity entityBuffer;
        entityBuffer.pos.x = Utils::floatToFixed(playerPos.x);
        entityBuffer.pos.y = Utils::floatToFixed(playerPos.y);
        entityBuffer.box   = Rect<int>(0, 0, 0, 0);
        activeDrawEntity   = &entityBuffer;

        float xpos = playerPos.x - cameraPos.x;
        float ypos = playerPos.y - cameraPos.y;

        // Draw Player Spawn Preview
        drawSpriteFlipped(xpos, ypos, gfxSurface[4].width, gfxSurface[4].height, 0, 0, FLIP_NONE,
                          INK_NONE, 0xFF, 4);
    }

    if (fileRender) {
        // addStatusProgress(0.2); // finished rendering entities

        renderRenderStates();

        // addStatusProgress(0.2); // finished rendering states
        return;
    }

    // TILE PREVIEW
    if ((selectedTile != 0xFFFF || selectedChunk != 0xFFFF) && selectedLayer >= 0 && isSelecting && curTool == TOOL_PENCIL) {
        float tx = tilePos.x;
        float ty = tilePos.y;

        tx *= iZoom;
        ty *= iZoom;

        float tx2 = tx + fmodf(cameraPos.x, tileSize);
        float ty2 = ty + fmodf(cameraPos.y, tileSize);

        // clip to grid
        tx -= fmodf(tx2, tileSize);
        ty -= fmodf(ty2, tileSize);

        // Draw Selected Tile Preview
        float xpos = tx + cameraPos.x;
        float ypos = ty + cameraPos.y;

        xpos -= cameraPos.x;
        ypos -= cameraPos.y;

        int count = 0;
        if (tileSize == 0x10 && selectedTile != 0xFFFF) {
            ++count;

            byte f;
            if (selectedTile < 0x3FF){
                f  = (int)(tileFlip.x) | ((int)(tileFlip.y) << 1);
            } else {
                f  = Utils::getBit(selectedTile, 10) | (Utils::getBit(selectedTile, 11) << 1);
            }
            ushort point = ((selectedTile & 0x3FF) << 2) | (f << 12);
            addPoly(xpos, ypos, tileUVArray[point], tileUVArray[point + 1], 0, gfxSurface);
            addPoly(xpos + 0x10, ypos, tileUVArray[point + 2], tileUVArray[point + 1], 0, gfxSurface);
            addPoly(xpos, ypos + 0x10, tileUVArray[point], tileUVArray[point + 3], 0, gfxSurface);
            addPoly(xpos + 0x10, ypos + 0x10, tileUVArray[point + 2], tileUVArray[point + 3], 0,
                    gfxSurface);
        }
        else if (tileSize == 0x80 && selectedChunk != 0xFFFF) {
            for (int y = 0; y < 8; ++y) {
                for (int x = 0; x < 8; ++x) {
                    ++count;

                    FormatHelpers::Chunks::Tile &tile = chunkset.chunks[selectedChunk].tiles[y][x];
                    float tileX                       = xpos + (x * 0x10);
                    float tileY                       = ypos + (y * 0x10);

                    ushort point = (tile.tileIndex << 2) | (tile.direction << 12);
                    addPoly(tileX, tileY, tileUVArray[point], tileUVArray[point + 1], 0, gfxSurface);
                    addPoly(tileX + 0x10, tileY, tileUVArray[point + 2], tileUVArray[point + 1], 0,
                            gfxSurface);
                    addPoly(tileX, tileY + 0x10, tileUVArray[point], tileUVArray[point + 3], 0,
                            gfxSurface);
                    addPoly(tileX + 0x10, tileY + 0x10, tileUVArray[point + 2], tileUVArray[point + 3],
                            0, gfxSurface);
                }
            }
        }

        PlaceArgs args;
        args.texID = 0;

        renderCount -= count * 4;
        addRenderState(INK_BLEND, count * 4, count * 6, &args, 0xFF, &placeShader);
        renderCount += count * 4;
    }

    // ERASER PREVIEW
    if (curTool == TOOL_ERASER) {
        float tx = tilePos.x;
        float ty = tilePos.y;

        tx *= iZoom;
        ty *= iZoom;

        float tx2 = tx + fmodf(cameraPos.x, tileSize);
        float ty2 = ty + fmodf(cameraPos.y, tileSize);

        // clip to grid
        tx -= fmodf(tx2, tileSize);
        ty -= fmodf(ty2, tileSize);

        // Draw Selected Tile Preview
        float xpos = tx + cameraPos.x;
        float ypos = ty + cameraPos.y;

        xpos -= cameraPos.x;
        ypos -= cameraPos.y;

        drawRect(xpos, ypos, tileSize, tileSize, Vector4<float>(1.0f, 0.0f, 0.0f, 1.0f), false, 0x40,
                 INK_ALPHA);
        drawRect(xpos, ypos, tileSize, tileSize, Vector4<float>(1.0f, 0.0f, 0.0f, 1.0f), true);
    }

    // ENT PREVIEW
    if (selectedObject >= 0 && selectedObject < objects.count() && isSelecting
        && curTool == TOOL_ENTITY) {
        float ex = tilePos.x;
        float ey = tilePos.y;

        ex *= iZoom;
        ey *= iZoom;

        validDraw = false;

            if (gameType == ENGINE_v5)
                emit callGameEventv5(objects[selectedObject].name, EVENT_DRAW, NULL);
            else
                emit callGameEvent(EVENT_DRAW, -1);

        if (!validDraw) {
            SceneEntity *entity;
            if (gameType == ENGINE_v5)
                entity = &v5Editor->createTempEntity;
            else
                entity = &scnEditor->createTempEntity;
            activeDrawEntity    = entity;
            entity->box = Rect<int>(-0x10, -0x10, 0x10, 0x10);

            // Draw Selected Object Preview
            float xpos = ex;
            float ypos = ey;

            drawSpriteFlipped(xpos - (gfxSurface[3].width >> 1), ypos - (gfxSurface[3].height >> 1),
                              gfxSurface[3].width, gfxSurface[3].height, 0, 0, FLIP_NONE, INK_NONE,
                              0xFF, 3);
        }
    }

    if (showGrid) {
        float camX = cameraPos.x;
        float camY = cameraPos.y;

        int x = camX;
        int y = camY;
        int w = (camX + storedW) * (zoom < 1.0f ? iZoom : 1.0f);
        int h = (camY + storedH) * (zoom < 1.0f ? iZoom : 1.0f);
        if (cameraPos.x < 0)
            x += abs(cameraPos.x);
        else
            x -= ((int)camX % gridSize.x);

        if (cameraPos.y < 0)
            y += abs(cameraPos.y);
        else
            y -= ((int)camY % gridSize.y);

        for (; y < h; y += gridSize.y) {
            float dy = (y - camY);

            float x1 = 0;
            float x2 = (((camX + storedW * iZoom)) - camX);
            if (cameraPos.x < 0) {
                x1 += abs(cameraPos.x);
            }

            drawLine(x1, dy, x2, dy, Vector4<float>(1.0f, 1.0f, 1.0f, 1.0f));
        }

        for (; x < w; x += gridSize.x) {
            float dx = (x + (zoom <= 1.0f ? 1.0f : 0.0f) - camX);

            float y1 = 0;
            float y2 = (((camY + storedH * iZoom)) - camY);
            if (cameraPos.y < 0) {
                y1 += abs(cameraPos.y);
            }

            drawLine(dx, y1, dx, y2, Vector4<float>(1.0f, 1.0f, 1.0f, 1.0f));
        }
    }

    // Selected Stamp Box
    if (selectedStamp >= 0 && selectedStamp < stamps.stampList.count()) {
        RSDKv5::Stamps::StampEntry &stamp = stamps.stampList[selectedStamp];

        float left   = stamp.pos.x;
        float top    = stamp.pos.y;
        float right  = stamp.pos.x + stamp.size.x;
        float bottom = stamp.pos.y + stamp.size.y;

        float w = fabsf((right - left) * 16), h = fabsf((bottom - top) * 16);


        Vector4<float> c = {1.0f, 1.0f, 0.0f, 1.0f};

        int lyrWidth = layers[selectedLayer].width;
        int lyrHeight = layers[selectedLayer].height;
        if (left < 0 || left > lyrWidth || right < 0 || right > lyrWidth ||
            top < 0 || top > lyrHeight || bottom < 0 || bottom > lyrHeight)
            c = {1.0f, 0.0f, 0.0f, 1.0f};

        left *= 16; top *= 16; right *= 16; bottom *= 16;

        left -= cameraPos.x;
        top -= cameraPos.y;

        drawRect(left, top, w, h, c, false, 0x40, INK_ALPHA);
        drawRect(left, top, w, h, c, true);
    }

    // STAMP PREVIEW
    if ((selectedStamp != 0xFFFF) && (selectedLayer >= 0 && layers[selectedLayer].visible) && curTool == TOOL_STAMP) {
        float tx = tilePos.x;
        float ty = tilePos.y;

        tx *= iZoom;
        ty *= iZoom;

        float tx2 = tx + fmodf(cameraPos.x, tileSize);
        float ty2 = ty + fmodf(cameraPos.y, tileSize);

        // clip to grid
        tx -= fmodf(tx2, tileSize);
        ty -= fmodf(ty2, tileSize);

        // Draw Selected Tile Preview
        float xpos = tx + cameraPos.x;
        float ypos = ty + cameraPos.y;

        xpos -= cameraPos.x;
        ypos -= cameraPos.y;

        auto stamp = stamps.stampList[selectedStamp];

        // Invalid draw position prevention
        if (0 > stamp.pos.x || stamp.pos.x + stamp.size.x > layers[selectedLayer].width ||
            0 > stamp.pos.y || stamp.pos.y + stamp.size.y > layers[selectedLayer].height){

            drawRect(xpos, ypos, stamp.size.x * tileSize, stamp.size.y * tileSize, Vector4<float>(1.0f, 0.0f, 0.0f, 1.0f),
                     true, 0x40, INK_ALPHA);
        }
        else {
            int count = 0;
            for(int y = 0; y < stamp.size.y; y++){
                for(int x = 0; x < stamp.size.x; x++){
                    int tileXPos = stamp.pos.x + x;
                    int tileYPos = stamp.pos.y + y;
                    ushort tile = layers[selectedLayer].layout[tileYPos][tileXPos];
                    if (tile != 0xFFFF) {
                        ++count;

                        float tileX                       = xpos + (x * tileSize);
                        float tileY                       = ypos + (y * tileSize);

                        int flipX = Utils::getBit(tile, 10);
                        int flipY = Utils::getBit(tile, 11);
                        byte f       = flipX | (flipY << 1);
                        ushort point = ((tile & 0x3FF) << 2) | (f << 12);
                        addPoly(tileX, tileY, tileUVArray[point], tileUVArray[point + 1], 0, gfxSurface);
                        addPoly(tileX + 0x10, tileY, tileUVArray[point + 2], tileUVArray[point + 1], 0, gfxSurface);
                        addPoly(tileX, tileY + 0x10, tileUVArray[point], tileUVArray[point + 3], 0, gfxSurface);
                        addPoly(tileX + 0x10, tileY + 0x10, tileUVArray[point + 2], tileUVArray[point + 3], 0,
                                gfxSurface);
                    }
                }

            }

            PlaceArgs args;
            args.texID = 0;

            renderCount -= count * 4;
            addRenderState(INK_BLEND, count * 4, count * 6, &args, 0xFF, &placeShader);
            renderCount += count * 4;
            drawRect(xpos, ypos, stamp.size.x * tileSize, stamp.size.y * tileSize, Vector4<float>(0.0f, 1.0f, 0.0f, 1.0f),
                     true, 0x40, INK_ALPHA);
        }
    }

    // Selected Entity Box (Single)
    if (selectedEntity >= 0 && selectedEntity < entities.count()) {
        SceneEntity &entity = entities[selectedEntity];

        float left   = entity.pos.x + entity.box.x;
        float top    = entity.pos.y + entity.box.y;
        float right  = entity.pos.x + entity.box.w;
        float bottom = entity.pos.y + entity.box.h;

        float w = fabsf(right - left), h = fabsf(bottom - top);

        drawRect(left - cameraPos.x, top - cameraPos.y, w, h, Vector4<float>(1.0f, 1.0f, 1.0f, 1.0f),
                 false, 0x40, INK_ALPHA);
        drawRect(left - cameraPos.x, top - cameraPos.y, w, h, Vector4<float>(1.0f, 1.0f, 1.0f, 1.0f),
                 true);
    }

    // Selected Entity Box (Multiple)
    if (curTool == TOOL_SELECT && (isSelecting || (selectSize.x && selectSize.y))) {

        if (isSelecting) {
            drawRect(selectPos.x - cameraPos.x, selectPos.y - cameraPos.y, selectSize.x, selectSize.y,
                     Vector4<float>(0.0f, 0.0f, 1.0f, 1.f), true);

            drawRect(selectPos.x - cameraPos.x, selectPos.y - cameraPos.y, selectSize.x, selectSize.y,
                     Vector4<float>(0.0f, 0.0f, 1.0f, 1.f), false, 0x40, INK_ALPHA);
        }

        for (auto &id : selectedEntities) {
            SceneEntity &entity = entities[id];

            float left   = entity.pos.x + entity.box.x;
            float top    = entity.pos.y + entity.box.y;
            float right  = entity.pos.x + entity.box.w;
            float bottom = entity.pos.y + entity.box.h;

            float w = fabsf(right - left), h = fabsf(bottom - top);

            drawRect(left - cameraPos.x, top - cameraPos.y, w, h,
                     Vector4<float>(1.0f, 1.0f, 1.0f, 1.0f), false, 0x40, INK_ALPHA);
            drawRect(left - cameraPos.x, top - cameraPos.y, w, h,
                     Vector4<float>(1.0f, 1.0f, 1.0f, 1.0f), true);
        }
    }

    // Selected Stamp Maker
    if (curTool == TOOL_STAMP_MAKER) {

        // Hover on the current tile postion
        int tx = selectPos.x * iZoom;
        int ty = selectPos.y * iZoom;

        int tx2 = tx + fmodf(cameraPos.x, tileSize);
        int ty2 = ty + fmodf(cameraPos.y, tileSize);

        // clip to grid
        tx -= fmodf(tx2, tileSize);
        ty -= fmodf(ty2, tileSize);

        int xpos = tx;
        int ypos = ty;

        int w = ((int)selectSize.x / 0x10) * tileSize;
        int h = ((int)selectSize.y / 0x10) * tileSize;

        if (abs(w) < tileSize) {
            if (w > xpos) { w = -tileSize; }
            else { w = tileSize; }
        }
        if (abs(h) < tileSize) {
            if (h > ypos) { h = -tileSize; }
            else { h = tileSize; }
        }

        drawRect(xpos, ypos, w, h, Vector4<float>(1.0f, 1.0f, 0.0f, 1.0f), false, 0x40,
                 INK_ALPHA);
        drawRect(xpos, ypos, w, h, Vector4<float>(1.0f, 1.0f, 0.0f, 1.0f), true);
    }

    renderRenderStates();
}

void SceneViewer::unloadScene()
{
    disableObjects = true;

    tiles.clear();
    if (colTexStore) {
        delete colTexStore;
        colTexStore = nullptr;
    }

    for (int o = 0; o < v5_SURFACE_MAX; ++o) {
        if (gfxSurface[o].scope == SCOPE_STAGE) {
            if (gfxSurface[o].texturePtr)
                delete gfxSurface[o].texturePtr;
            gfxSurface[o].texturePtr = nullptr;
            gfxSurface[o].scope      = SCOPE_NONE;
        }
    }

    for (int a = 0; a < v5_SPRFILE_COUNT; ++a) {
        spriteAnimationList[a].scope = SCOPE_NONE;
    }

    cameraPos.x         = 0;
    cameraPos.y         = 0;
    selectedTile        = -1;
    selectedChunk       = -1;
    selectedEntity      = -1;
    selectedLayer       = -1;
    selectedHScrollInfo = -1;
    selectedObject      = -1;
    isSelecting         = false;
    selectedEntities.clear();

    memset(gameEntityListv1, 0, ENTITY_COUNT_v5 * 2 * sizeof(GameEntityBasev1));
    memset(gameEntityListv2, 0, ENTITY_COUNT_v5 * 2 * sizeof(GameEntityBasev2));
    memset(gameEntityListvU, 0, ENTITY_COUNT_v5 * 2 * sizeof(GameEntityBasevU));

    sceneBoundsL = 0;
    sceneBoundsT = 0;
    sceneBoundsR = 0;
    sceneBoundsB = 0;
    prevStoredW  = -1;
    prevStoredH  = -1;

    updateCTex[0] = false;
    updateCTex[1] = false;
}

void SceneViewer::processObjects(bool isImage)
{
    for (int i = 0; i < v5_DRAWGROUP_COUNT; ++i) {
        drawLayers[i].entries.clear();
    }

    sceneInfo.entitySlot   = 0;
    sceneInfoV1.entitySlot = 0;
    int centerX            = screens->centerX * invZoom();
    int centerY            = screens->centerY * invZoom();

    if (gameType == ENGINE_v5) {
        switch (v5Editor->viewer->engineRevision) {
            case 1: {
                for (int e = 0; e < entities.count(); ++e) {
                    if (!entities[e].gameEntity || !entities[e].type || isImage) {
                        drawLayers[15].entries.append(sceneInfo.entitySlot++);
                        sceneInfoV1.entitySlot = sceneInfo.entitySlot;
                        continue;
                    }

                    sceneInfo.entity     = entities[e].gameEntity;
                    sceneInfoV1.entity   = entities[e].gameEntity;
                    GameEntityv1 *entity = AS_ENTITY(sceneInfo.entity, GameEntityv1);
                    if (entities[e].type) {
                        entity->inRange = false;

                        int rangeX = entity->updateRange.x;
                        int rangeY = entity->updateRange.y;

                        switch (entity->active) {
                            case ACTIVE_NEVER:
                            case ACTIVE_PAUSED:
                            case ACTIVE_ALWAYS:
                            case ACTIVE_NORMAL: entity->inRange = true; break;
                            case ACTIVE_BOUNDS: {
                                int sx =
                                    abs(entity->position.x - ((screens->position.x + centerX) << 16));
                                int sy =
                                    abs(entity->position.y - ((screens->position.y + centerY) << 16));
                                if (sx <= rangeX + (centerX << 16) && sy <= rangeY + (centerY << 16)) {
                                    entity->inRange = true;
                                }
                                break;
                            }
                            case ACTIVE_XBOUNDS: {
                                int sx =
                                    abs(entity->position.x - ((screens->position.x + centerX) << 16));
                                if (sx <= rangeX + (centerX << 16)) {
                                    entity->inRange = true;
                                    break;
                                }
                                break;
                            }
                            case ACTIVE_YBOUNDS: {
                                int sy =
                                    abs(entity->position.y - ((screens->position.y + centerY) << 16));
                                if (sy <= rangeY + (centerY << 16)) {
                                    entity->inRange = true;
                                    break;
                                }
                                break;
                            }
                            case ACTIVE_RBOUNDS: {
                                int sx =
                                    abs(entity->position.x - ((screens->position.x + centerX) << 16))
                                    >> 16;
                                int sy =
                                    abs(entity->position.y - ((screens->position.y + centerY) << 16))
                                    >> 16;
                                if (sx * sx + sy * sy <= rangeX + (centerX << 16)) {
                                    entity->inRange = true;
                                    break;
                                }
                                break;
                            }
                        }

                        if (entity->inRange) {
                            // emit
                            // callGameEvent(gameLink.GetObjectInfo(objects[entities[e].type].name),
                            // EVENT_UPDATE,
                            //              &entities[e]);

                            if (entity->drawGroup < v5_DRAWGROUP_COUNT)
                                drawLayers[entity->drawGroup].entries.append(sceneInfo.entitySlot);
                        }
                    }
                    else {
                        entity->inRange = false;
                    }
                    sceneInfo.entitySlot++;
                    sceneInfoV1.entitySlot++;
                }

                for (int i = 0; i < TYPEGROUP_COUNT_v5; ++i) {
                    typeGroups[i].entries.clear();
                }

                sceneInfo.entitySlot   = 0;
                sceneInfoV1.entitySlot = 0;
                for (int e = 0; e < entities.count(); ++e) {
                    sceneInfo.entity     = entities[e].gameEntity;
                    sceneInfoV1.entity   = entities[e].gameEntity;
                    GameEntityv1 *entity = AS_ENTITY(sceneInfo.entity, GameEntityv1);

                    if (sceneInfo.entity && entity->inRange && entity->interaction) {
                        typeGroups[0].entries.append(e);               // All active objects
                        typeGroups[entity->classID].entries.append(e); // type-based slots
                        if (entity->group >= TYPE_COUNT) {
                            typeGroups[entity->group].entries.append(e); // extra slots
                        }
                    }
                    sceneInfo.entitySlot++;
                    sceneInfoV1.entitySlot++;
                }

                sceneInfo.entitySlot   = 0;
                sceneInfoV1.entitySlot = 0;
                for (int e = 0; e < entities.count(); ++e) {
                    if (entities[e].gameEntity) {
                        AS_ENTITY(entities[e].gameEntity, GameEntityvU)->onScreen = 0;
                    }
                }

                break;
            }

            case 2: {

                for (int e = 0; e < entities.count(); ++e) {
                    if (!entities[e].gameEntity || !entities[e].type || isImage) {
                        drawLayers[15].entries.append(sceneInfo.entitySlot++);
                        continue;
                    }

                    sceneInfo.entity     = entities[e].gameEntity;
                    GameEntityv2 *entity = AS_ENTITY(sceneInfo.entity, GameEntityv2);
                    if (entities[e].type) {
                        entity->inRange = false;

                        int rangeX = entity->updateRange.x;
                        int rangeY = entity->updateRange.y;

                        switch (entity->active) {
                            case ACTIVE_NEVER:
                            case ACTIVE_PAUSED:
                            case ACTIVE_ALWAYS:
                            case ACTIVE_NORMAL: entity->inRange = true; break;
                            case ACTIVE_BOUNDS: {
                                int sx =
                                    abs(entity->position.x - ((screens->position.x + centerX) << 16));
                                int sy =
                                    abs(entity->position.y - ((screens->position.y + centerY) << 16));
                                if (sx <= rangeX + (centerX << 16) && sy <= rangeY + (centerY << 16)) {
                                    entity->inRange = true;
                                }
                                break;
                            }
                            case ACTIVE_XBOUNDS: {
                                int sx =
                                    abs(entity->position.x - ((screens->position.x + centerX) << 16));
                                if (sx <= rangeX + (centerX << 16)) {
                                    entity->inRange = true;
                                    break;
                                }
                                break;
                            }
                            case ACTIVE_YBOUNDS: {
                                int sy =
                                    abs(entity->position.y - ((screens->position.y + centerY) << 16));
                                if (sy <= rangeY + (centerY << 16)) {
                                    entity->inRange = true;
                                    break;
                                }
                                break;
                            }
                            case ACTIVE_RBOUNDS: {
                                int sx =
                                    abs(entity->position.x - ((screens->position.x + centerX) << 16))
                                    >> 16;
                                int sy =
                                    abs(entity->position.y - ((screens->position.y + centerY) << 16))
                                    >> 16;
                                if (sx * sx + sy * sy <= rangeX + (centerX << 16)) {
                                    entity->inRange = true;
                                    break;
                                }
                                break;
                            }
                        }

                        if (entity->inRange) {
                            // emit
                            // callGameEvent(gameLink.GetObjectInfo(objects[entities[e].type].name),
                            // EVENT_UPDATE,
                            //              &entities[e]);

                            if (entity->drawGroup < v5_DRAWGROUP_COUNT)
                                drawLayers[entity->drawGroup].entries.append(sceneInfo.entitySlot);
                        }
                    }
                    else {
                        entity->inRange = false;
                    }
                    sceneInfo.entitySlot++;
                }

                for (int i = 0; i < TYPEGROUP_COUNT_v5; ++i) {
                    typeGroups[i].entries.clear();
                }

                sceneInfo.entitySlot = 0;
                for (int e = 0; e < entities.count(); ++e) {
                    sceneInfo.entity     = entities[e].gameEntity;
                    GameEntityv2 *entity = AS_ENTITY(sceneInfo.entity, GameEntityv2);

                    if (sceneInfo.entity && entity->inRange && entity->interaction) {
                        typeGroups[0].entries.append(e);               // All active objects
                        typeGroups[entity->classID].entries.append(e); // type-based slots
                        if (entity->group >= TYPE_COUNT) {
                            typeGroups[entity->group].entries.append(e); // extra slots
                        }
                    }
                    sceneInfo.entitySlot++;
                }

                sceneInfo.entitySlot = 0;
                for (int e = 0; e < entities.count(); ++e) {
                    if (entities[e].gameEntity) {
                        AS_ENTITY(entities[e].gameEntity, GameEntityvU)->onScreen = 0;
                    }
                }

                break;
            }

            default:
            case 3: {
                for (int e = 0; e < entities.count(); ++e) {
                    if (!entities[e].gameEntity || !entities[e].type || isImage) {
                        drawLayers[15].entries.append(sceneInfo.entitySlot++);
                        continue;
                    }

                    sceneInfo.entity     = entities[e].gameEntity;
                    GameEntityvU *entity = AS_ENTITY(sceneInfo.entity, GameEntityvU);
                    if (entities[e].type) {
                        entity->inRange = false;

                        int rangeX = entity->updateRange.x;
                        int rangeY = entity->updateRange.y;

                        switch (entity->active) {
                            case ACTIVE_NEVER:
                            case ACTIVE_PAUSED:
                            case ACTIVE_ALWAYS:
                            case ACTIVE_NORMAL: entity->inRange = true; break;
                            case ACTIVE_BOUNDS: {
                                int sx =
                                    abs(entity->position.x - ((screens->position.x + centerX) << 16));
                                int sy =
                                    abs(entity->position.y - ((screens->position.y + centerY) << 16));
                                if (sx <= rangeX + (centerX << 16) && sy <= rangeY + (centerY << 16)) {
                                    entity->inRange = true;
                                }
                                break;
                            }
                            case ACTIVE_XBOUNDS: {
                                int sx =
                                    abs(entity->position.x - ((screens->position.x + centerX) << 16));
                                if (sx <= rangeX + (centerX << 16)) {
                                    entity->inRange = true;
                                    break;
                                }
                                break;
                            }
                            case ACTIVE_YBOUNDS: {
                                int sy =
                                    abs(entity->position.y - ((screens->position.y + centerY) << 16));
                                if (sy <= rangeY + (centerY << 16)) {
                                    entity->inRange = true;
                                    break;
                                }
                                break;
                            }
                            case ACTIVE_RBOUNDS: {
                                int sx =
                                    abs(entity->position.x - ((screens->position.x + centerX) << 16))
                                    >> 16;
                                int sy =
                                    abs(entity->position.y - ((screens->position.y + centerY) << 16))
                                    >> 16;
                                if (sx * sx + sy * sy <= rangeX + (centerX << 16)) {
                                    entity->inRange = true;
                                    break;
                                }
                                break;
                            }
                        }

                        if (entity->inRange) {
                            // emit
                            // callGameEvent(gameLink.GetObjectInfo(objects[entities[e].type].name),
                            // EVENT_UPDATE,
                            //              &entities[e]);

                            if (entity->drawGroup < v5_DRAWGROUP_COUNT)
                                drawLayers[entity->drawGroup].entries.append(sceneInfo.entitySlot);
                        }
                    }
                    else {
                        entity->inRange = false;
                    }
                    sceneInfo.entitySlot++;
                }

                for (int i = 0; i < TYPEGROUP_COUNT_v5; ++i) {
                    typeGroups[i].entries.clear();
                }

                sceneInfo.entitySlot = 0;
                for (int e = 0; e < entities.count(); ++e) {
                    sceneInfo.entity     = entities[e].gameEntity;
                    GameEntityvU *entity = AS_ENTITY(sceneInfo.entity, GameEntityvU);

                    if (sceneInfo.entity && entity->inRange && entity->interaction) {
                        typeGroups[0].entries.append(e);               // All active objects
                        typeGroups[entity->classID].entries.append(e); // type-based slots
                        if (entity->group >= TYPE_COUNT) {
                            typeGroups[entity->group].entries.append(e); // extra slots
                        }
                    }
                    sceneInfo.entitySlot++;
                }

                sceneInfo.entitySlot = 0;
                for (int e = 0; e < entities.count(); ++e) {
                    if (entities[e].gameEntity) {
                        AS_ENTITY(entities[e].gameEntity, GameEntityvU)->onScreen = 0;
                    }
                }

                break;
            }
        }
    }
    else {
        for (int e = 0; e < entities.count(); ++e) {
            if (!entities[e].type || isImage) {
                drawLayers[15].entries.append(sceneInfo.entitySlot++);
                sceneInfoV1.entitySlot = sceneInfo.entitySlot;
                continue;
            }

            if (entities[e].type) {
                bool inBounds   = false;
                byte activeType = ACTIVE_NEVER;
                byte drawOrder  = 0;

                switch (gameType) {
                    case ENGINE_v2:
                        activeType = scnEditor->compilerv2->objectEntityList[entities[e].gameEntitySlot]
                                         .priority;
                        drawOrder = scnEditor->compilerv2->objectEntityList[entities[e].gameEntitySlot]
                                        .drawOrder;
                        break;
                    case ENGINE_v3:
                        activeType = scnEditor->compilerv3->objectEntityList[entities[e].gameEntitySlot]
                                         .priority;
                        drawOrder = scnEditor->compilerv3->objectEntityList[entities[e].gameEntitySlot]
                                        .drawOrder;
                        break;
                    case ENGINE_v4:
                        activeType = scnEditor->compilerv4->objectEntityList[entities[e].gameEntitySlot]
                                         .priority;
                        drawOrder = scnEditor->compilerv4->objectEntityList[entities[e].gameEntitySlot]
                                        .drawOrder;
                        break;
                }

                int posX   = entities[e].pos.x * 65536;
                int posY   = entities[e].pos.y * 65536;
                int rangeX = 0x800000;
                int rangeY = 0x800000;

                switch (activeType) {
                    case 0: activeType = ACTIVE_BOUNDS; break;
                    case 1: activeType = ACTIVE_NORMAL; break;
                    case 2: activeType = ACTIVE_PAUSED; break;
                    case 3: activeType = ACTIVE_XBOUNDS; break;
                    case 4: activeType = ACTIVE_XBOUNDS; break;
                    case 5: activeType = ACTIVE_NEVER; break;
                    case 6:
                        activeType = ACTIVE_BOUNDS;
                        rangeX     = 0x200000;
                        rangeY     = 0x200000;
                        break;
                    case 7: activeType = ACTIVE_NORMAL; break;
                }

                switch (activeType) {
                    case ACTIVE_NEVER:
                    case ACTIVE_PAUSED:
                    case ACTIVE_ALWAYS:
                    case ACTIVE_NORMAL: inBounds = true; break;
                    case ACTIVE_BOUNDS: {
                        int sx = abs(posX - ((screens->position.x + centerX) << 16));
                        int sy = abs(posY - ((screens->position.y + centerY) << 16));
                        if (sx <= rangeX + (centerX << 16) && sy <= rangeY + (centerY << 16)) {
                            inBounds = true;
                        }
                        break;
                    }
                    case ACTIVE_XBOUNDS: {
                        int sx = abs(posX - ((screens->position.x + centerX) << 16));
                        if (sx <= rangeX + (centerX << 16)) {
                            inBounds = true;
                            break;
                        }
                        break;
                    }
                }

                if (inBounds) {
                    // emit callGameEvent(gameLink.GetObjectInfo(objects[entities[e].type].name),
                    // EVENT_UPDATE,
                    //              &entities[e]);

                    if (drawOrder < v5_DRAWGROUP_COUNT)
                        drawLayers[drawOrder].entries.append(sceneInfo.entitySlot);
                }
            }
            sceneInfo.entitySlot++;
            sceneInfoV1.entitySlot++;
        }

        for (int i = 0; i < TYPEGROUP_COUNT_v5; ++i) {
            typeGroups[i].entries.clear();
        }

        // sceneInfo.entitySlot = 0;
        // sceneInfoV1.entitySlot = 0;
        // for (int e = 0; e < entities.count(); ++e) {
        //     sceneInfo.entity = entities[e].gameEntity;
        //     sceneInfoV1.entity = entities[e].gameEntity;
        //
        //     if (sceneInfo.entity && sceneInfo.entity->inBounds && sceneInfo.entity->interaction) {
        //         typeGroups[0].entries.append(e);                          // All active objects
        //         typeGroups[sceneInfo.entity->objectID].entries.append(e); // type-based slots
        //         if (sceneInfo.entity->group >= TYPE_COUNT) {
        //             typeGroups[sceneInfo.entity->group].entries.append(e); // extra slots
        //         }
        //     }
        //     sceneInfo.entitySlot++;
        //     sceneInfoV1.entitySlot++;
        // }
    }
}

void SceneViewer::initializeGL()
{
    // QOpenGLFunctions::initializeOpenGLFunctions();

    // Set up the rendering context, load shaders and other resources, etc.
    glFuncs = context()->functions();

    QSurfaceFormat fmt = context()->format();

    const unsigned char *vendor     = glFuncs->glGetString(GL_VENDOR);
    const unsigned char *renderer   = glFuncs->glGetString(GL_RENDERER);
    const unsigned char *version    = glFuncs->glGetString(GL_VERSION);
    const unsigned char *sdrVersion = glFuncs->glGetString(GL_SHADING_LANGUAGE_VERSION);
    const unsigned char *extensions = glFuncs->glGetString(GL_EXTENSIONS);

    QString vendorStr     = reinterpret_cast<const char *>(vendor);
    QString rendererStr   = reinterpret_cast<const char *>(renderer);
    QString versionStr    = reinterpret_cast<const char *>(version);
    QString sdrVersionStr = reinterpret_cast<const char *>(sdrVersion);
    QString extensionsStr = reinterpret_cast<const char *>(extensions);

    PrintLog("SceneViewer OpenGL Details");
    PrintLog("GL Version:   " + QString::number(fmt.majorVersion()) + "."
             + QString::number(fmt.minorVersion()));
    PrintLog("Vendor:       " + vendorStr);
    PrintLog("Renderer:     " + rendererStr);
    PrintLog("Version:      " + versionStr);
    PrintLog("GLSL version: " + sdrVersionStr);
    PrintLog("Extensions:   " + (extensionsStr == "" ? "None" : extensionsStr));
    PrintLog("GLType:       "
             + QString(QOpenGLContext::currentContext()->isOpenGLES() ? "OpenGLES" : "OpenGL"));
    PrintLog("Valid:        " + QString(QOpenGLContext::currentContext()->isValid() ? "Yes" : "No"));

    glFuncs->glDisable(GL_DEPTH_TEST);
    glFuncs->glDisable(GL_DITHER);

    glFuncs->glEnable(GL_BLEND);
    glFuncs->glDisable(GL_SCISSOR_TEST);
    glFuncs->glDisable(GL_CULL_FACE);

    glFuncs->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    placeShader.loadShader(":/shaders/pre/default.vert", QOpenGLShader::Vertex);
    placeShader.loadShader(":/shaders/pre/place.frag", QOpenGLShader::Fragment);
    placeShader.link();
    placeShader.argsCB = [this](Shader *self, void *state) {
        PlaceArgs *args = (PlaceArgs *)((RenderState *)state)->argBuffer;
        if (args->texID != (uint)-1) {
            self->setValue("useColor", false);
            glFuncs->glActiveTexture(GL_TEXTURE0);
            glFuncs->glBindTexture(GL_TEXTURE_2D, gfxSurface[args->texID].texturePtr->textureId());
            QColor tc = gfxSurface[args->texID].transClr;
            self->setValue("transColor",
                           QVector3D({ (float)tc.redF(), (float)tc.greenF(), (float)tc.blueF() }));
        }
        else
            self->setValue("useColor", true);
    };
    placeShader.use();
    placeShader.setValue("sprite", 0);

    circleShader.loadShader(":/shaders/pre/default.vert", QOpenGLShader::Vertex);
    circleShader.loadShader(":/shaders/pre/circle.frag", QOpenGLShader::Fragment);
    circleShader.link();
    circleShader.argsCB = [this](Shader *self, void *state) {
        CircleArgs *args = (CircleArgs *)((RenderState *)state)->argBuffer;
        self->setValue("innerRadius", args->innerR);
        self->setValue("outerRadius", args->outerR);
    };

    tileShader.loadShader(":/shaders/pre/default.vert", QOpenGLShader::Vertex);
    tileShader.loadShader(":/shaders/pre/tile.frag", QOpenGLShader::Fragment);
    tileShader.link();
    tileShader.use();
    tileShader.setValue("tiles", 30);

    passthroughFBShader.loadShader(":/shaders/fb/passthrough.vert", QOpenGLShader::Vertex);
    passthroughFBShader.loadShader(":/shaders/fb/passthrough.frag", QOpenGLShader::Fragment);
    passthroughFBShader.link();
    passthroughFBShader.use();
    passthroughFBShader.setValue("source", 20);
    passthroughFBShader.setValue("dest", 10);
    passthroughFBShader.argsCB = [](Shader *self, void *state) {
        RenderState *rs = (RenderState *)state;
        self->setValue("inkEffect", rs->blendMode);
        self->setValue("alpha", rs->alpha);
        // TODO: masked/unmasked
    };

    finalFBShader.loadShader(":/shaders/fb/passthrough.vert", QOpenGLShader::Vertex);
    finalFBShader.loadShader(":/shaders/fb/final.frag", QOpenGLShader::Fragment);
    finalFBShader.link();
    finalFBShader.use();
    finalFBShader.setValue("tex", 22);

    VAO = new QOpenGLVertexArrayObject;
    VAO->create();
    VAO->bind();

    attribVBO = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    attribVBO->create();
    attribVBO->bind();
    attribVBO->setUsagePattern(QOpenGLBuffer::DynamicDraw);
    attribVBO->allocate(vertexListLimit * sizeof(DrawVertex));
    // we have to do this directly bc for whatever reason it's normally per program ??????
    glFuncs->glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(DrawVertex), 0);
    glFuncs->glEnableVertexAttribArray(0);
    glFuncs->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(DrawVertex),
                                   (void *)offsetof(DrawVertex, uv));
    glFuncs->glEnableVertexAttribArray(1);
    glFuncs->glVertexAttribPointer(2, 4, GL_FLOAT, GL_TRUE, sizeof(DrawVertex),
                                   (void *)offsetof(DrawVertex, color));
    glFuncs->glEnableVertexAttribArray(2);
    indexVBO = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    indexVBO->create();
    indexVBO->bind();
    indexVBO->allocate(vertexListLimit * 6 * sizeof(ushort));

    fbpVAO = new QOpenGLVertexArrayObject;
    fbpVAO->create();
    fbpVAO->bind();

    fbpVBO = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    fbpVBO->create();
    fbpVBO->bind();
    fbpVBO->setUsagePattern(QOpenGLBuffer::DynamicDraw);
    fbpVBO->allocate(sizeof(int) * 16);

    glFuncs->glVertexAttribPointer(0, 2, GL_INT, GL_FALSE, 0, 0);
    glFuncs->glEnableVertexAttribArray(0);
    glFuncs->glVertexAttribPointer(1, 2, GL_INT, GL_FALSE, 0, (void *)(sizeof(int) * 8));
    glFuncs->glEnableVertexAttribArray(1);

    fbiVBO = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    fbiVBO->create();
    fbiVBO->bind();
    fbiVBO->allocate(baseIndexList, 6 * sizeof(ushort));

    resizeGL(width(), height());
}

void SceneViewer::resizeGL(int w, int h)
{
    // Get scaled size
    qreal dpr = devicePixelRatio();
    int scaledW = w * dpr;
    int scaledH = h * dpr;

    glFuncs = context()->functions();

    // clang-format off
    int pos[] = {     // POSITIONS
                      0, 0,
                      w, 0,
                      0, h,
                      w, h,
                      // UVS
                      0, 0,
                      1, 0,
                      0, 1,
                      1, 1
    };
    // clang-format on

    fbpVAO->bind();
    fbpVBO->bind();
    fbpVBO->write(0, pos, sizeof(int) * 16);

    storedW = w;
    storedH = h;
    refreshResize();

    matWorld.setToIdentity();
    matWorld.ortho(0.0f, (float)storedW, (float)storedH, 0.0f, -1.0f, 1.0f);

    QMatrix4x4 view = QMatrix4x4();

    passthroughFBShader.use();
    passthroughFBShader.setValue("projection", matWorld);
    passthroughFBShader.setValue("view", view);

    finalFBShader.use();
    finalFBShader.setValue("projection", matWorld);
    finalFBShader.setValue("view", view);

    glFuncs->glViewport(0, 0, w, h);

    glFuncs->glActiveTexture(GL_TEXTURE20);
    if (tFB)
        delete tFB;
    tFB = new QOpenGLFramebufferObject(scaledW, scaledH);
    glFuncs->glBindTexture(GL_TEXTURE_2D, tFB->texture());

    glFuncs->glActiveTexture(GL_TEXTURE20 + 1);
    if (t2FB)
        delete t2FB;
    t2FB = new QOpenGLFramebufferObject(scaledW, scaledH);
    glFuncs->glBindTexture(GL_TEXTURE_2D, t2FB->texture());

    glFuncs->glActiveTexture(GL_TEXTURE20 + 2);
    if (outFB)
        delete outFB;
    outFB = new QOpenGLFramebufferObject(scaledW, scaledH);
    glFuncs->glBindTexture(GL_TEXTURE_2D, outFB->texture());
}

void SceneViewer::paintGL()
{
    glFuncs = context()->functions();

    int boundsL = 0;
    int boundsT = 0;
    int boundsR = 0;
    int boundsB = 0;
    for (int i = 0; i < layers.count(); ++i) {
        if ((layers[i].width * tileSize) > boundsR)
            boundsR = layers[i].width * tileSize;
        if ((layers[i].height * tileSize) > boundsB)
            boundsB = layers[i].height * tileSize;
    }

    for (int e = 0; e < entities.count(); ++e) {
        auto &entity = entities[e];

        int left   = entity.pos.x + entity.box.x;
        int top    = entity.pos.y + entity.box.y;
        int right  = entity.pos.x + entity.box.w;
        int bottom = entity.pos.y + entity.box.h;

        // LEFT BOUNDS
        if (left < boundsL)
            boundsL = left;

        if (right < boundsL)
            boundsL = right;

        // TOP BOUNDS
        if (top < boundsT)
            boundsT = top;

        if (bottom < boundsT)
            boundsT = bottom;

        // RIGHT BOUNDS
        if (left > boundsR)
            boundsR = left;

        if (right > boundsR)
            boundsR = right;

        // BOTTOM BOUNDS
        if (top > boundsB)
            boundsB = top;

        if (bottom > boundsB)
            boundsB = bottom;
    }

    if (boundsL != sceneBoundsL)
        sceneBoundsL = boundsL;

    if (boundsT != sceneBoundsT)
        sceneBoundsT = boundsT;

    if (boundsR != sceneBoundsR)
        sceneBoundsR = boundsR;

    if (boundsB != sceneBoundsB)
        sceneBoundsB = boundsB;

    if (fileRender == 0) {
        // pre-render

        if ((cameraPos.x * zoom) + storedW > sceneBoundsR * zoom)
            cameraPos.x = (sceneBoundsR - (storedW * invZoom()));

        if ((cameraPos.y * zoom) + storedH > sceneBoundsB * zoom)
            cameraPos.y = (sceneBoundsB - (storedH * invZoom()));

        if (cameraPos.x < sceneBoundsL)
            cameraPos.x = sceneBoundsL;

        if (cameraPos.y < sceneBoundsT)
            cameraPos.y = sceneBoundsT;

        screens[0].position.x = cameraPos.x;
        screens[0].position.y = cameraPos.y;
    }
    else if (fileRender == 1) {
        auto preCam = cameraPos;
        float pz    = zoom;
        int preW = storedW, preH = storedH;
        int texSize = 1024;
        QOpenGLFramebufferObject *oOFB = outFB, *oTFB = tFB, *oTFB2 = t2FB;
        outFB = nullptr;
        tFB   = nullptr;
        t2FB  = nullptr;
        resizeGL(texSize, texSize);
        cameraPos = {(float)(boundsL), (float)boundsT};
        zoom      = 1;
        matView.setToIdentity();

        QImage *output = new QImage(boundsR + abs(boundsL), boundsB + abs(boundsT), QImage::Format_RGB888);
        QPainter p;
        p.begin(output);
        for (int h = 0; h < 1 + (boundsB + abs(boundsT)) / texSize; h++){
            for (int w = 0; w < 1 + (boundsR + abs(boundsL)) / texSize; w++){
                glFuncs->glActiveTexture(GL_TEXTURE20 + 2);
                delete outFB;
                outFB = new QOpenGLFramebufferObject(texSize, texSize);
                glFuncs->glBindTexture(GL_TEXTURE_2D, outFB->texture());

                outFB->bind();
                glFuncs->glClearColor(metadata.backgroundColor1.red() / 255.0f,
                                      metadata.backgroundColor1.green() / 255.0f,
                                      metadata.backgroundColor1.blue() / 255.0f, 1.0f);
                glFuncs->glClear(GL_COLOR_BUFFER_BIT);

                if (!disableObjects)
                    processObjects(true);

                if (!disableDrawScene)
                    drawScene();

                p.drawImage(w * texSize, h * texSize, outFB->toImage(false)
                                                        .convertToFormat(QImage::Format_ARGB32)
                                                        .convertToFormat(QImage::Format_RGB888));
                cameraPos.x += texSize;
            }
            cameraPos.y += texSize;
            cameraPos.x  = boundsL;
        }
        p.end();

        SetStatus("Rendered scene to image!");
        QImageWriter writer(renderFilename, "PNG");
        writer.write(*output);

        fileRender = 0;
        zoom       = pz;
        cameraPos  = preCam;
        resizeGL(preW, preH);

        delete outFB;
        delete tFB;
        delete t2FB;

        glFuncs->glActiveTexture(GL_TEXTURE20);
        tFB = oTFB;
        glFuncs->glBindTexture(GL_TEXTURE_2D, tFB->texture());

        glFuncs->glActiveTexture(GL_TEXTURE20 + 1);
        t2FB = oTFB2;
        glFuncs->glBindTexture(GL_TEXTURE_2D, t2FB->texture());

        glFuncs->glActiveTexture(GL_TEXTURE20 + 2);
        outFB = oOFB;
        glFuncs->glBindTexture(GL_TEXTURE_2D, outFB->texture());

    }

    matView.setToIdentity();
    matView.translate(zoom, -zoom);
    matView.scale(zoom, zoom);
    matView.translate(-invZoom(), invZoom());

    outFB->bind();
    glFuncs->glClearColor(metadata.backgroundColor1.red() / 255.0f,
                          metadata.backgroundColor1.green() / 255.0f,
                          metadata.backgroundColor1.blue() / 255.0f, 1.0f);
    glFuncs->glClear(GL_COLOR_BUFFER_BIT);

    if (!disableObjects)
        processObjects(false);

    if (!disableDrawScene)
        drawScene();

    // swap FB0
    glFuncs->glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebufferObject());
    finalFBShader.use();
    fbpVAO->bind();
    fbpVBO->bind();
    fbiVBO->bind();
    glFuncs->glBlendFunc(GL_ONE, GL_ZERO);
    glFuncs->glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);

    sumFps += fpsTimer.nsecsElapsed();
    fpsTimer.restart();
    ++updateCount;

    // Compute avg FPS over the last second
    if (sumFps >= 1e9) {
        avgFps = (double)(updateCount * 1e9) / sumFps;
        updateCount = 0;
        sumFps = 0.0;
    }
}

int SceneViewer::addGraphicsFile(QString sheetPath, int sheetID, byte scope)
{
    if (sheetID >= 0 && sheetID < v5_SURFACE_MAX) {
        QImage sheet;
        QColor transClr = QColor(0xFFFF00FF);
        if (QFileInfo(sheetPath).suffix() == "gif") {
            QGifImage gif(sheetPath);
            QVector<QRgb> table = gif.globalColorTable();
            // good for them
            transClr = QColor(table[0]);
            sheet    = gif.frame(0);
            if (gameType != ENGINE_v5 && scope == SCOPE_STAGE){
                // insert the stage palette
                for(int i = 128; i < 256; ++i){
                    QRgb col = PaletteColor(tilePalette[i]).toQColor().rgb();
                    sheet.setColor(i, col);
                }
            }
        }
        else {
            sheet = QImage(sheet);
        }
        gfxSurface[sheetID].name = QString(sheetPath);
        Utils::getHashInt(sheetPath, gfxSurface[sheetID].hash);
        gfxSurface[sheetID].texturePtr = createTexture(sheet);
        gfxSurface[sheetID].scope      = scope;
        gfxSurface[sheetID].width      = sheet.width();
        gfxSurface[sheetID].height     = sheet.height();
        gfxSurface[sheetID].transClr   = transClr;
        return sheetID;
    }
    return -1;
}

void SceneViewer::removeGraphicsFile(QString sheetPath, int slot)
{
    if (slot >= 0) {
        gfxSurface[slot].texturePtr->destroy();
        delete gfxSurface[slot].texturePtr;
        gfxSurface[slot].scope = SCOPE_NONE;
    }
    else {
        for (int i = 3; i < v5_SURFACE_MAX; ++i) {
            if (QString(sheetPath) == gfxSurface[i].name) {
                gfxSurface[i].texturePtr->destroy();
                delete gfxSurface[i].texturePtr;
                gfxSurface[i].scope = SCOPE_NONE;
            }
        }
    }
}

void SceneViewer::drawSpriteFlipped(float XPos, float YPos, float width, float height, float sprX,
                                    float sprY, int direction, InkEffects inkEffect, int alpha,
                                    int sheetID)
{

    if (sheetID == 0xFF)
        return;

    switch (inkEffect) {
        case INK_NONE: alpha = 0xFF; break;
        case INK_BLEND: alpha = 0x80; break;
        case INK_ALPHA:
            if (alpha > 0xFF) {
                inkEffect = INK_NONE;
            }
            else if (alpha <= 0) {
                return;
            }
            break;
        case INK_ADD:
        case INK_SUB:
            if (alpha > 0xFF) {
                alpha = 0xFF;
            }
            else if (alpha <= 0) {
                return;
            }
            break;
        case INK_TINT:
            // if (!lookupTable)
            //    return;
            break;
        case INK_MASKED: alpha = 0xFF; break;
        case INK_UNMASKED: alpha = 0xFF; break;
    }

    float startX = XPos;
    float startY = YPos;
    float startW = width;
    float startH = height;

    // TODO: some do clipping here

    if (width <= 0 || height <= 0)
        return;

    float entX      = activeDrawEntity->pos.x - cameraPos.x;
    float entY      = activeDrawEntity->pos.y - cameraPos.y;
    float boxLeft   = startX;
    float boxTop    = startY;
    float boxRight  = startX + startW;
    float boxBottom = startY + startH;

    if (!sceneInfo.debugMode) {
        if (boxLeft < entX + activeDrawEntity->box.x) {
            activeDrawEntity->box.x = boxLeft - entX;
        }
        if (boxTop < entY + activeDrawEntity->box.y) {
            activeDrawEntity->box.y = boxTop - entY;
        }
        if (boxRight > entX + activeDrawEntity->box.w) {
            activeDrawEntity->box.w = boxRight - entX;
        }
        if (boxBottom > entY + activeDrawEntity->box.h) {
            activeDrawEntity->box.h = boxBottom - entY;
        }
    }

    // Draw Sprite
    float xpos        = XPos;
    float ypos        = YPos;
    GFXSurface *sheet = &gfxSurface[sheetID];

    PlaceArgs args;
    args.texID = sheetID;
    addRenderState(inkEffect, 4, 6, &args, alpha);

    switch (direction) {
        case FLIP_NONE:
            addPoly(xpos, ypos, sprX, sprY, 0, sheet);
            addPoly(xpos + width, ypos, sprX + width, sprY, 0, sheet);
            addPoly(xpos, ypos + height, sprX, sprY + height, 0, sheet);
            addPoly(xpos + width, ypos + height, sprX + width, sprY + height, 0, sheet);
            break;
        case FLIP_X:
            addPoly(xpos, ypos, sprX + width, sprY, 0, sheet);
            addPoly(xpos + width, ypos, sprX, sprY, 0, sheet);
            addPoly(xpos, ypos + height, sprX + width, sprY + height, 0, sheet);
            addPoly(xpos + width, ypos + height, sprX, sprY + height, 0, sheet);
            break;
        case FLIP_Y:
            addPoly(xpos, ypos, sprX, sprY + height, 0, sheet);
            addPoly(xpos + width, ypos, sprX + width, sprY + height, 0, sheet);
            addPoly(xpos, ypos + height, sprX, sprY, 0, sheet);
            addPoly(xpos + width, ypos + height, sprX + width, sprY, 0, sheet);
            break;
        case FLIP_XY:
            addPoly(xpos, ypos, sprX + width, sprY + height, 0, sheet);
            addPoly(xpos + width, ypos, sprX, sprY + height, 0, sheet);
            addPoly(xpos, ypos + height, sprX + width, sprY, 0, sheet);
            addPoly(xpos + width, ypos + height, sprX, sprY, 0, sheet);
            break;
    }
    validDraw = true;
}

void SceneViewer::drawSpriteRotozoom(float XPos, float YPos, float pivotX, float pivotY, float width,
                                     float height, float sprX, float sprY, int scaleX, int scaleY,
                                     int direction, short rotation, InkEffects inkEffect, int alpha,
                                     int sheetID)
{
    if (sheetID == 0xFF)
        return;
    switch (inkEffect) {
        case INK_NONE: alpha = 0xFF; break;
        case INK_BLEND: alpha = 0x80; break;
        case INK_ALPHA:
            if (alpha > 0xFF)
                inkEffect = INK_NONE;
            else if (alpha <= 0)
                return;
            break;
        case INK_ADD:
        case INK_SUB:
            if (alpha > 0xFF)
                alpha = 0xFF;
            else if (alpha <= 0)
                return;
            break;
        case INK_TINT:
            // if (!lookupTable)
            //    return;
            break;
        case INK_MASKED: alpha = 0xFF; break;
        case INK_UNMASKED: alpha = 0xFF; break;
    }

    int angle = 0x200 - (rotation & 0x1FF);
    if (!(rotation & 0x1FF))
        angle = rotation & 0x1FF;

    GFXSurface *sheet = &gfxSurface[sheetID];
    float xpos        = XPos;
    float ypos        = YPos;

    PlaceArgs args;
    args.texID = sheetID;
    addRenderState(inkEffect, 4, 6, &args, alpha);

    float sY  = scaleY / (float)(1 << 9);
    float sX  = scaleX / (float)(1 << 9);
    float sin = sin512LookupTable[angle] / (float)(1 << 9);
    float cos = cos512LookupTable[angle] / (float)(1 << 9);

    float posX[4], posY[4];

    if (direction == FLIP_NONE) {
        int x   = pivotX;
        int y   = pivotY;
        posX[0] = xpos + ((x * cos + y * sin)) * sX;
        posY[0] = ypos + ((y * cos - x * sin)) * sY;

        x       = width + pivotX;
        y       = pivotY;
        posX[1] = xpos + ((x * cos + y * sin)) * sX;
        posY[1] = ypos + ((y * cos - x * sin)) * sY;

        x       = pivotX;
        y       = height + pivotY;
        posX[2] = xpos + ((x * cos + y * sin)) * sX;
        posY[2] = ypos + ((y * cos - x * sin)) * sY;

        x       = width + pivotX;
        y       = height + pivotY;
        posX[3] = xpos + ((x * cos + y * sin)) * sX;
        posY[3] = ypos + ((y * cos - x * sin)) * sY;
    }
    else {
        int x   = -pivotX;
        int y   = pivotY;
        posX[0] = xpos + ((x * cos + y * sin)) * sX;
        posY[0] = ypos + ((y * cos - x * sin)) * sY;

        x       = -pivotX - width;
        y       = pivotY;
        posX[1] = xpos + ((x * cos + y * sin)) * sX;
        posY[1] = ypos + ((y * cos - x * sin)) * sY;

        x       = -pivotX;
        y       = height + pivotY;
        posX[2] = xpos + ((x * cos + y * sin)) * sX;
        posY[2] = ypos + ((y * cos - x * sin)) * sY;

        x       = -pivotX - width;
        y       = height + pivotY;
        posX[3] = xpos + ((x * cos + y * sin)) * sX;
        posY[3] = ypos + ((y * cos - x * sin)) * sY;
    }
    float entX = activeDrawEntity->pos.x - cameraPos.x;
    float entY = activeDrawEntity->pos.y - cameraPos.y;
    float left = screens->pitch;
    for (int i = 0; i < 4; ++i) {
        if (posX[i] < left)
            left = posX[i];
    }

    float right = 0;
    for (int i = 0; i < 4; ++i) {
        if (posX[i] > right)
            right = posX[i];
    }

    float top = screens->height;
    for (int i = 0; i < 4; ++i) {
        if (posY[i] < top)
            top = posY[i];
    }

    float bottom = 0;
    for (int i = 0; i < 4; ++i) {
        if (posY[i] > bottom)
            bottom = posY[i];
    }

    if (!sceneInfo.debugMode) {
        if (left < entX + activeDrawEntity->box.x) {
            activeDrawEntity->box.x = left - entX;
        }
        if (top < entY + activeDrawEntity->box.y) {
            activeDrawEntity->box.y = top - entY;
        }
        if (right > entX + activeDrawEntity->box.w) {
            activeDrawEntity->box.w = right - entX;
        }
        if (bottom > entY + activeDrawEntity->box.h) {
            activeDrawEntity->box.h = bottom - entY;
        }
    }

    addPoly(posX[0], posY[0], sprX, sprY, 0, sheet);
    addPoly(posX[1], posY[1], sprX + width, sprY, 0, sheet);
    addPoly(posX[2], posY[2], sprX, sprY + height, 0, sheet);
    addPoly(posX[3], posY[3], sprX + width, sprY + height, 0, sheet);

    validDraw = true;
}

void SceneViewer::drawLine(float x1, float y1, float x2, float y2, Vector4<float> color, int alpha,
                           InkEffects ink)
{
    validDraw = true;

    // Bounds check
    if (x1 < 0 && x2 < 0)
        return;
    if (y1 < 0 && y2 < 0)
        return;
    if (x1 > storedW / zoom && x2 > storedW / zoom)
        return;
    if (y1 > storedH / zoom && y2 > storedH / zoom)
        return;

    addRenderState(ink, 2, 2, nullptr, alpha, &lineShader);
    addPoly(x1, y1, 0, 0, color);
    addPoly(x2, y2, 0, 0, color);
}

void SceneViewer::drawRect(float x, float y, float w, float h, Vector4<float> color, bool outline,
                           int alpha, InkEffects inkEffect)
{
    if (outline) {
        int corner = 1 * invZoom();
        // top
        drawLine(x, y, x + w, y, color);
        // bottom
        drawLine(x, y + h, x + w, y + h, color);
        // left
        drawLine(x, y, x, y + h + corner, color);
        // right
        drawLine(x + w, y, x + w, y + h, color);
    }
    else {
        PlaceArgs args;
        args.texID = -1;
        addRenderState(inkEffect, 4, 6, &args, alpha);

        addPoly(x, y, 0, 0, color);
        addPoly(x + w, y, 0, 0, color);
        addPoly(x, y + h, 0, 0, color);
        addPoly(x + w, y + h, 0, 0, color);
    }
    validDraw = true;
}

void SceneViewer::drawFace(Vector2<int> *vertices, int vertCount, int r, int g, int b, int alpha,
                           InkEffects inkEffect)
{
    return; /*
    uint color = (r << 16) | (g << 8) | (b << 0);

    int count = 3 * vertCount - 3;
    ushort indecies[0x800 * 6];
    for (int i = 0; i < vertCount; ++i) {
        if (i < vertCount - 1) {
            indecies[i * 3 + 0] = i;
            indecies[i * 3 + 1] = i + 1;
            indecies[i * 3 + 2] = i + 2;
        }
    }
    indecies[count - 1] = 0;

    addRenderState(inkEffect, vertCount, count, -1, alpha, &primitiveShader, indecies);

    for (int i = 0; i < vertCount; ++i)
        addPoly(vertices[i].x / (float)(1 << 16), vertices[i].y / (float)(1 << 16), 0, 0, 0,
    color);

    validDraw = true;//*/
}

void SceneViewer::drawBlendedFace(Vector2<int> *vertices, uint *colors, int vertCount, int alpha,
                                  InkEffects inkEffect)
{
    return; /*
    int count = 3 * vertCount - 3;
    ushort indecies[0x800 * 6];
    for (int i = 0; i < vertCount; ++i) {
        if (i < vertCount - 1) {
            indecies[i * 3 + 0] = i;
            indecies[i * 3 + 1] = i + 1;
            indecies[i * 3 + 2] = i + 2;
        }
    }
    indecies[count - 1] = 0;

    addRenderState(inkEffect, vertCount, count, -1, alpha, &primitiveShader, indecies);

    for (int i = 0; i < vertCount; ++i) {
        uint c = colors[i];
        c |= alpha << 24;
        addPoly(vertices[i].x / (float)(1 << 16), vertices[i].y / (float)(1 << 16), 0, 0, 0, c);
    }

    validDraw = true;//*/
}

inline void SceneViewer::addPoly(float x, float y, float u, float v, Vector4<float> color,
                                 GFXSurface *surface)
{
    vertexList[renderCount].pos.setX(x);
    vertexList[renderCount].pos.setY(y);
    if (surface) {
        u /= surface->width;
        v /= surface->height;
    }
    vertexList[renderCount].uv.setX(u);
    vertexList[renderCount].uv.setY(v);

    vertexList[renderCount].color.setX(color.x);
    vertexList[renderCount].color.setY(color.y);
    vertexList[renderCount].color.setZ(color.z);
    vertexList[renderCount].color.setW(color.w);
    renderCount++;
}

bool32 statesCompatible(SceneViewer::RenderState &one, SceneViewer::RenderState &two)
{
    if (one.blendMode) // one->blendMode != INK_NONE
        return false;  // the rest can't really be merged
    if (memcmp(one.argBuffer, two.argBuffer, sizeof(one.argBuffer)) || one.blendMode != two.blendMode
        || one.shader != two.shader || one.alpha != two.alpha
        || memcmp(&one.clipRectTL, &two.clipRectTL, sizeof(int) * 4))
        return false;
    return true;
}

void SceneViewer::addRenderState(int blendMode, ushort vertCount, ushort indexCount, void *args,
                                 byte alpha, Shader *shader, ushort *altIndex, Shader *fbShader,
                                 Shader *fbShader2, Vector2<int> *clipRect)
{
    if (!vertCount || !indexCount)
        return;
    if (!shader)
        shader = &placeShader;
    if (!fbShader)
        fbShader = &passthroughFBShader;

    // set up a new state minimally needed for comparison
    RenderState &newState = renderStates[renderStateCount];
    newState.blendMode  = blendMode;
    newState.indexCount = indexCount;
    newState.alpha      = alpha;
    newState.shader     = shader;
    newState.fbShader   = fbShader;
    newState.fbShader2  = fbShader2;

    if (args)
        memcpy(newState.argBuffer, args, 0x20);
    else
        memset(newState.argBuffer, 0, 0x20);

    if (!clipRect)
        memcpy(&newState.clipRectTL, &screens->clipBound_X1, sizeof(int) * 4);
    else {
        // newState.clipRectTL.x = maxVal(screens->clipBound_X1, clipRect[0].x);
        // newState.clipRectTL.y = minVal(screens->clipBound_Y1, clipRect[0].y);
        // newState.clipRectBR.x = maxVal(screens->clipBound_X2, clipRect[1].x);
        // newState.clipRectBR.y = minVal(screens->clipBound_Y2, clipRect[1].y);
    }

    if (!altIndex)
        altIndex = baseIndexList;

    if (renderStateCount && vertCount + renderCount < vertexListLimit) {
        RenderState &last = renderStates[renderStateCount - 1];
        if (last.indexCount + indexCount < (vertexListLimit * 6) && statesCompatible(last, newState)) {
            // merge em and we'll be on our way
            memcpy(&last.indices[last.indexCount], altIndex, indexCount * sizeof(ushort));
            for (int i = 0; i < indexCount; ++i) last.indices[i + last.indexCount] += renderCount;
            last.indexCount += indexCount;
            return;
        }
    }

    if (vertCount + renderCount >= vertexListLimit || renderStateCount >= renderStatesLimit - 1) {
        renderRenderStates(); // you should render NOW!
        // Move new state to the start
        memcpy(&renderStates[0], &newState, sizeof(RenderState));
        newState = renderStates[renderStateCount];
    }

    memcpy(newState.indices, altIndex, indexCount * sizeof(ushort));
    if (renderCount)
        for (int i = 0; i < indexCount; ++i) newState.indices[i] += renderCount;
    renderStateCount++;
}

void SceneViewer::renderRenderStates()
{
    if (!renderCount)
        return;

    VAO->bind();
    attribVBO->bind();
    attribVBO->write(0, vertexList, renderCount * sizeof(DrawVertex));
    indexVBO->bind();

    glFuncs->glClearColor(0, 0, 0, 0);
    glFuncs->glBlendFunc(GL_ONE, GL_ZERO);
    glFuncs->glLineWidth(zoom);

    int lastBlendMode = -1;

    for (int stateIndex = 0; stateIndex < renderStateCount; stateIndex++) {
        RenderState &renderState = renderStates[stateIndex];
        // hack
        bool lines = false;

        if (renderState.shader == &lineShader) {
            renderState.shader                           = &placeShader;
            ((PlaceArgs *)&renderState.argBuffer)->texID = -1;
            lines                                        = true;
        }

        renderState.shader->use();
        renderState.shader->setArgs(&renderState);

        if (renderState.fbShader2) {
            tFB->bind();
            glFuncs->glClear(GL_COLOR_BUFFER_BIT);
        }
        else {
            // cheat, render directly to fb2 for final
            t2FB->bind();
        }

        renderState.shader->setValue("projection", matWorld);
        renderState.shader->setValue("view", matView);

        indexVBO->write(0, renderState.indices, renderState.indexCount * sizeof(ushort));
        glFuncs->glDrawElements(lines ? GL_LINES : GL_TRIANGLES, renderState.indexCount,
                                GL_UNSIGNED_SHORT, 0);

        fbpVAO->bind();
        fbpVBO->bind();
        fbiVBO->bind();

        if (renderState.blendMode != lastBlendMode) {
            switch (renderState.blendMode) {
                case INK_NONE:
                case INK_MASKED:
                case INK_UNMASKED:
                case INK_SUB:
                case INK_BLEND:
                case INK_ALPHA:
                    glFuncs->glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);
                    break;
                case INK_ADD: glFuncs->glBlendFunc(GL_SRC_ALPHA, GL_ONE); break;
            }
            lastBlendMode = renderState.blendMode;
        }

        if (renderState.fbShader) {
            if (renderState.fbShader2) {
                // render inplace, swap shaders
                t2FB->bind();
                renderState.fbShader->use();
                renderState.fbShader->setValue("source", 20);
                renderState.fbShader->setValue("dest", 21);
                renderState.fbShader->setArgs(&renderState);
                glFuncs->glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
                renderState.fbShader = renderState.fbShader2;
            }
            // render to the out FB, clear t2
            outFB->bind();
            renderState.fbShader->use();
            renderState.fbShader->setValue("source", 21);
            renderState.fbShader->setValue("dest", 22);
            renderState.fbShader->setArgs(&renderState);

            glFuncs->glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
            t2FB->bind();
            glFuncs->glClear(GL_COLOR_BUFFER_BIT);
        }
        else {
            // if fbShader2, render "inplace" to prepare for the next send
            if (renderState.fbShader2) {
                t2FB->bind();
                renderState.fbShader2->use();
                renderState.fbShader2->setValue("source", 20);
                renderState.fbShader2->setValue("dest", 21);
                renderState.fbShader2->setArgs(&renderState);
                glFuncs->glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
            }
        }
        VAO->bind();
        attribVBO->bind(); // rebind
        indexVBO->bind();
    }

    renderCount = 0;
    renderStateCount = 0;
}

void SceneViewer::addEditorVariable(QString name)
{
    VariableInfo var;
    var.name = name;
    objects[activeVarObj].variables.append(var);
}
void SceneViewer::setActiveVariable(QString name)
{
    activeVar = -1;

    int v = 0;
    for (auto &var : objects[activeVarObj].variables) {
        if (var.name == name) {
            activeVar = v;
            break;
        }
        ++v;
    }
}
void SceneViewer::addEnumVariable(QString name, int value)
{
    if (activeVarObj >= 0) {
        VariableValue var;
        var.name  = name;
        var.value = value;

        objects[activeVarObj].variables[activeVar].values.append(var);
    }
}

void SceneViewer::setVariableAlias(int varID, QString alias)
{
    if (activeVarObj >= 0) {
        objects[activeVarObj].variablesAliases[varID] = alias;
    }
}

void SceneViewer::refreshResize()
{
    if (storedW == prevStoredW && storedH == prevStoredH)
        return;

    screens[0].width        = storedW;
    screens[0].height       = storedH;
    screens[0].centerX      = screens[0].width >> 1;
    screens[0].centerY      = screens[0].height >> 1;
    screens[0].clipBound_X1 = 0;
    screens[0].clipBound_Y1 = 0;
    screens[0].clipBound_X2 = storedW;
    screens[0].clipBound_Y2 = storedH;
    screens[0].pitch        = storedW;
    screens[0].waterDrawPos = storedH;

    prevStoredW = storedW;
    prevStoredH = storedH;
}

#include "moc_sceneviewer.cpp"
