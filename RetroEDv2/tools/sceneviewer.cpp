#include "includes.hpp"
#include "qgifimage.h"

static QVector3D rectVertices[] = {
    QVector3D(-0.5f, -0.5f, -0.5f), QVector3D(0.5f, -0.5f, -0.5f), QVector3D(0.5f, 0.5f, -0.5f),
    QVector3D(0.5f, 0.5f, -0.5f),   QVector3D(-0.5f, 0.5f, -0.5f), QVector3D(-0.5f, -0.5f, -0.5f),
};

static QVector2D rectTexCoords[] = {
    QVector2D(0.0f, 0.0f), QVector2D(1.0f, 0.0f), QVector2D(1.0f, 1.0f),
    QVector2D(1.0f, 1.0f), QVector2D(0.0f, 1.0f), QVector2D(0.0f, 0.0f),
};

SceneViewer::SceneViewer(QWidget *parent)
{
    setMouseTracking(true);

    this->setFocusPolicy(Qt::WheelFocus);

    if (updateTimer)
        delete updateTimer;

    updateTimer = new QTimer(this);
    connect(updateTimer, &QTimer::timeout, this, QOverload<>::of(&SceneViewer::updateScene));
    startTimer();
}

SceneViewer::~SceneViewer() { dispose(); }

void SceneViewer::startTimer()
{
    if (updateTimer) {
        stopTimer();

        updateTimer->start(1000.0f / 60.0f);
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
        stopTimer();
        delete updateTimer;
    }

    screenVAO.destroy();
    rectVAO.destroy();
}

void SceneViewer::loadScene(QString path, byte ver)
{
    // unloading
    unloadScene();

    // loading
    QString pth      = path;
    QString basePath = pth.replace(QFileInfo(pth).fileName(), "");

    currentFolder = QDir(basePath).dirName();

    scene.read(ver, path);

    if (ver != ENGINE_v1) {
        QString pathTCF = WorkingDirManager::GetPath("Stages/" + currentFolder + "/CollisionMasks.bin",
                                                     basePath + "CollisionMasks.bin");
        QString pathSCF = WorkingDirManager::GetPath("Stages/" + currentFolder + "/StageConfig.bin",
                                                     basePath + "StageConfig.bin");
        QString pathBG  = WorkingDirManager::GetPath("Stages/" + currentFolder + "/Backgrounds.bin",
                                                    basePath + "Backgrounds.bin");
        QString pathTIL = WorkingDirManager::GetPath("Stages/" + currentFolder + "/128x128Tiles.bin",
                                                     basePath + "128x128Tiles.bin");

        background.read(ver, pathBG);
        chunkset.read(ver, pathTIL);
        tileconfig.read(pathTCF);
        stageConfig.read(ver, pathSCF);
    }
    else {
        QString pathTCF =
            WorkingDirManager::GetPath("Stages/" + currentFolder + "/Zone.tcf", basePath + "Zone.tcf");
        QString pathSCF =
            WorkingDirManager::GetPath("Stages/" + currentFolder + "/Zone.zcf", basePath + "Zone.zcf");
        QString pathBG = WorkingDirManager::GetPath("Stages/" + currentFolder + "/ZoneBG.map",
                                                    basePath + "ZoneBG.map");
        QString pathTIL =
            WorkingDirManager::GetPath("Stages/" + currentFolder + "/Zone.til", basePath + "Zone.til");

        background.read(ver, pathBG);
        chunkset.read(ver, pathTIL);
        tileconfig.read(pathTCF);
        stageConfig.read(ver, pathSCF);
    }

    for (int l = 0; l < 9; ++l) {
        visibleLayers[l] = true;
    }

    // Always have 8 layers, even if some have w/h of 0
    for (int l = background.layers.count(); l < 8; ++l)
        background.layers.append(FormatHelpers::Background::Layer());

    sceneWidth  = scene.width;
    sceneHeight = scene.height;

    for (int i = 0; i < background.layers.count(); ++i) {
        if (background.layers[i].width > sceneWidth)
            sceneWidth = background.layers[i].width;
        if (background.layers[i].height > sceneHeight)
            sceneHeight = background.layers[i].height;
    }

    refreshResize();

    if (ver != ENGINE_v1) {
        scene.objectTypeNames.clear();

        if (stageConfig.loadGlobalScripts) {
            if (ver == ENGINE_v2)
                scene.objectTypeNames.append("Player");

            for (FormatHelpers::GameConfig::ObjectInfo &obj : gameConfig.objects) {
                scene.objectTypeNames.append(obj.name);
            }
        }

        for (FormatHelpers::StageConfig::ObjectInfo &obj : stageConfig.objects) {
            scene.objectTypeNames.append(obj.name);
        }
    }
    else {
        scene.objectTypeNames.clear();

        // TODO: globals
        QList<QString> globals = {
            "Ring",              // 1
            "Dropped Ring",      // 2
            "Ring Sparkle",      // 3
            "Monitor",           // 4
            "Broken Monitor",    // 5
            "Yellow Spring",     // 6
            "Red Spring",        // 7
            "Spikes",            // 8
            "LampPost",          // 9
            "PlaneSwitch L",     // 10
            "PlaneSwitch R",     // 11
            "PlaneSwitch U",     // 12
            "PlaneSwitch D",     // 13
            "ForceSpin R",       // 14
            "ForceSpin L",       // 15
            "Unknown (ID 16)",   // 16
            "Unknown (ID 17)",   // 17
            "SignPost",          // 18
            "Egg Prison",        // 19
            "Small Explosion",   // 20
            "Large Explosion",   // 21
            "Egg Prison Debris", // 22
            "Animal",            // 23
            "Ring",              // 24
            "Ring",              // 25
            "Big Ring",          // 26
            "Water Splash",      // 27
            "Bubbler",           // 28
            "Small Air Bubble",  // 29
            "Smoke Puff",        // 30
        };

        for (QString &obj : globals) {
            scene.objectTypeNames.append(obj);
        }

        for (FormatHelpers::StageConfig::ObjectInfo &obj : stageConfig.objects) {
            scene.objectTypeNames.append(obj.name);
        }
    }

    if (ver == ENGINE_v1) {
        if (QFile::exists(basePath + "Zone.gfx")) {
            // setup tileset texture from png
            RSDKv1::GFX gfx(basePath + "Zone.gfx");
            QImage tileset = gfx.exportImage();
            tilesetTexture = createTexture(tileset);
            for (int i = 0; i < 0x400; ++i) {
                int tx         = ((i % (tileset.width() / 0x10)) * 0x10);
                int ty         = ((i / (tileset.width() / 0x10)) * 0x10);
                QImage tileTex = tileset.copy(tx, ty, 0x10, 0x10);

                tiles.append(tileTex);
            }

            for (FormatHelpers::Chunks::Chunk &c : chunkset.chunks) {
                chunks.append(c.getImage(tiles));
            }

            tilePalette.clear();
            for (Colour &col : gfx.palette) {
                PaletteColour clr;
                clr.r = col.r;
                clr.g = col.g;
                clr.b = col.b;
                tilePalette.append(clr);
            }
        }
    }
    else {
        if (QFile::exists(basePath + "16x16Tiles.gif")) {
            // setup tileset texture from png
            QGifImage tilesetGif(basePath + "16x16Tiles.gif");
            QImage tileset = tilesetGif.frame(0);
            tilesetTexture = createTexture(tileset);
            for (int i = 0; i < 0x400; ++i) {
                int tx         = ((i % (tileset.width() / 0x10)) * 0x10);
                int ty         = ((i / (tileset.width() / 0x10)) * 0x10);
                QImage tileTex = tileset.copy(tx, ty, 0x10, 0x10);

                tiles.append(tileTex);
            }

            for (FormatHelpers::Chunks::Chunk &c : chunkset.chunks) {
                chunks.append(c.getImage(tiles));
            }

            auto pal = tileset.colorTable();
            tilePalette.clear();
            for (QRgb &col : pal) {
                tilePalette.append(PaletteColour(col));
            }
        }
    }

    // objects
    objectSprites.clear();
    {
        TextureInfo tex;
        tex.name       = ":/icons/missing.png";
        missingObj     = QImage(tex.name);
        tex.texturePtr = createTexture(missingObj);
        objectSprites.append(tex);
    }

    rsPlayerSprite = createTexture(QImage(":/icons/player_v1.png"));

    objects.clear();
    entities.clear();

    ObjectInfo blankInfo;
    blankInfo.name = "Blank Object";
    objects.append(blankInfo);
    for (int i = 0; i < scene.objectTypeNames.count(); ++i) {
        ObjectInfo info;
        info.name = scene.objectTypeNames[i];
        info.variables.clear();
        objects.append(info);
    }

    for (int i = 0; i < scene.objects.count(); ++i) {
        EntityInfo info;
        info.slotID        = scene.objects[i].slotID;
        info.prevSlotID    = info.slotID;
        info.type          = scene.objects[i].type;
        info.propertyValue = scene.objects[i].propertyValue;
        info.pos.x         = scene.objects[i].getX();
        info.pos.y         = scene.objects[i].getY();

        if (gameType == ENGINE_v4) {
            for (int v = 0; v < 0xF; ++v) {
                info.variables[v] = scene.objects[i].variables[v];
            }
        }

        entities.append(info);
    }
}

void SceneViewer::updateScene()
{
    this->repaint();

    ((SceneObjectProperties *)objProp)->updateUI();

    if (statusLabel) {
        int mx = (int)((mousePos.x * invZoom()) + cam.pos.x);
        int my = (int)((mousePos.y * invZoom()) + cam.pos.y);
        statusLabel->setText(
            QString("Zoom: %1%, Mouse Position: (%2, %3), Chunk Position: (%4, %5), Selected Chunk: "
                    "%6, Selected Layer: %7 (%8), Selected Object: %9")
                .arg(zoom * 100)
                .arg(mx)
                .arg(my)
                .arg((int)mx / 0x80)
                .arg((int)my / 0x80)
                .arg(selectedChunk)
                .arg(selectedLayer)
                .arg(selectedLayer >= 0 && 9 ? selectedLayer == 0
                                                   ? "Foreground"
                                                   : "Background " + QString::number(selectedLayer)
                                             : "")
                .arg(selectedObject >= 0 && selectedObject < entities.count()
                         ? objects[selectedObject].name
                         : ""));
    }
}

void SceneViewer::drawScene()
{
    if (!tilesetTexture)
        return;
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    f->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    f->glBlendEquation(GL_FUNC_ADD);

    for (int i = 0; i < ENTITY_COUNT; ++i) {
        memset(&compilerv2.objectEntityList[i], 0, sizeof(compilerv2.objectEntityList[i]));
        memset(&compilerv3.objectEntityList[i], 0, sizeof(compilerv3.objectEntityList[i]));
        memset(&compilerv4.objectEntityList[i], 0, sizeof(compilerv4.objectEntityList[i]));

        if (i < entities.count()) {
            compilerv2.objectEntityList[i].type          = entities[i].type;
            compilerv2.objectEntityList[i].propertyValue = entities[i].propertyValue;
            compilerv2.objectEntityList[i].XPos          = entities[i].pos.x * 65536;
            compilerv2.objectEntityList[i].YPos          = entities[i].pos.y * 65536;

            compilerv3.objectEntityList[i].type          = entities[i].type;
            compilerv3.objectEntityList[i].propertyValue = entities[i].propertyValue;
            compilerv3.objectEntityList[i].XPos          = entities[i].pos.x * 65536;
            compilerv3.objectEntityList[i].YPos          = entities[i].pos.y * 65536;

            compilerv4.objectEntityList[i].type          = entities[i].type;
            compilerv4.objectEntityList[i].propertyValue = entities[i].propertyValue;
            compilerv4.objectEntityList[i].XPos          = entities[i].pos.x * 65536;
            compilerv4.objectEntityList[i].YPos          = entities[i].pos.y * 65536;

            int *values[] = { &compilerv4.objectEntityList[i].state,
                              NULL,
                              &compilerv4.objectEntityList[i].scale,
                              &compilerv4.objectEntityList[i].rotation,
                              NULL,
                              NULL,
                              &compilerv4.objectEntityList[i].alpha,
                              NULL,
                              &compilerv4.objectEntityList[i].animationSpeed,
                              NULL,
                              NULL,
                              &compilerv4.objectEntityList[i].values[0],
                              &compilerv4.objectEntityList[i].values[1],
                              &compilerv4.objectEntityList[i].values[2],
                              &compilerv4.objectEntityList[i].values[3] };

            byte *valuesB[] = { NULL,
                                &compilerv4.objectEntityList[i].direction,
                                NULL,
                                NULL,
                                (byte *)&compilerv4.objectEntityList[i].drawOrder,
                                &compilerv4.objectEntityList[i].priority,
                                NULL,
                                &compilerv4.objectEntityList[i].animation,
                                NULL,
                                &compilerv4.objectEntityList[i].frame,
                                &compilerv4.objectEntityList[i].inkEffect,
                                NULL,
                                NULL,
                                NULL,
                                NULL };

            for (int v = 0; v < 0x0F; ++v) {
                if (entities[i].variables[v].active) {
                    if (values[v])
                        *values[v] = entities[i].variables[v].value;
                    else
                        *valuesB[v] = entities[i].variables[v].value;
                }
            }
        }
    }

    int sceneW = scene.width;
    int sceneH = scene.height;

    for (int i = 0; i < background.layers.count(); ++i) {
        if (background.layers[i].width > sceneW)
            sceneW = background.layers[i].width;
        if (background.layers[i].height > sceneH)
            sceneH = background.layers[i].height;
    }

    if (sceneW != sceneWidth || sceneH != sceneHeight) {
        sceneWidth  = sceneW;
        sceneHeight = sceneH;

        refreshResize();
    }

    // pre-render
    if ((cam.pos.x * zoom) + storedW > (sceneWidth * 0x80) * zoom)
        cam.pos.x = ((sceneWidth * 0x80) - (storedW * invZoom()));

    if ((cam.pos.y * zoom) + storedH > (sceneHeight * 0x80) * zoom)
        cam.pos.y = ((sceneHeight * 0x80) - (storedH * invZoom()));

    if ((cam.pos.x * zoom) < -64)
        cam.pos.x = -64;

    if ((cam.pos.y * zoom) < -64)
        cam.pos.y = -64;

    // draw bg colours
    primitiveShader.use();
    primitiveShader.setValue("colour", QVector4D(altBGColour.r / 255.0f, altBGColour.g / 255.0f,
                                                 altBGColour.b / 255.0f, 1.0f));
    primitiveShader.setValue("projection", getProjectionMatrix());
    primitiveShader.setValue("view", QMatrix4x4());
    rectVAO.bind();

    int bgOffsetY = 0x80;
    bgOffsetY -= (int)cam.pos.y % 0x200;
    for (int y = bgOffsetY; y < (storedH + 0x80) * (zoom < 1.0f ? invZoom() : 1.0f); y += 0x100) {
        int bgOffsetX = (((y - bgOffsetY) % 0x200 == 0) ? 0x100 : 0x00);
        bgOffsetX += 0x80;
        bgOffsetX -= (int)cam.pos.x % 0x200;
        for (int x = bgOffsetX; x < (storedW + 0x80) * (zoom < 1.0f ? invZoom() : 1.0f); x += 0x200) {
            QMatrix4x4 matModel;
            matModel.scale(0x100 * zoom, 0x100 * zoom, 1.0f);
            matModel.translate(x / 256.0f, y / 256.0f, -15.0f);
            primitiveShader.setValue("model", matModel);

            f->glDrawArrays(GL_TRIANGLES, 0, 6);
        }
    }

    spriteShader.use();
    spriteShader.setValue("useAlpha", false);
    spriteShader.setValue("alpha", 1.0f);
    spriteShader.setValue("transparentColour", (gameType != ENGINE_v1 ? QVector3D(1.0f, 0.0f, 1.0f)
                                                                      : QVector3D(0.0f, 0.0f, 0.0f)));

    QMatrix4x4 matWorld;
    QMatrix4x4 matView;
    spriteShader.setValue("projection", matWorld = getProjectionMatrix());
    spriteShader.setValue("view", matView = QMatrix4x4());
    f->glActiveTexture(GL_TEXTURE0);

    rectVAO.bind();

    Vector3<float> camOffset(0.0f, 0.0f, 0.0f);

    QVector4D pixelSolidityClrs[5] = { QVector4D(1.0f, 1.0f, 1.0f, 1.0f),
                                       QVector4D(1.0f, 1.0f, 0.0f, 1.0f),
                                       QVector4D(1.0f, 0.0f, 0.0f, 1.0f),
                                       QVector4D(0.0f, 0.0f, 0.0f, 0.0f),
                                       QVector4D(0.0f, 0.0f, 1.0f, 1.0f) };
    bool showCLayers[2]            = { showPlaneA, showPlaneB };

    for (int l = 8; l >= 0; --l) {
        if (!visibleLayers[l])
            continue;

        // TILE LAYERS
        QList<QList<ushort>> layout = scene.layout;
        int width                   = scene.width;
        int height                  = scene.height;

        if (l > 0) {
            layout = background.layers[l - 1].layout;
            width  = background.layers[l - 1].width;
            height = background.layers[l - 1].height;
        }

        spriteShader.use();
        spriteShader.setValue("useAlpha", false);
        spriteShader.setValue("alpha", 1.0f);
        spriteShader.setValue(
            "transparentColour",
            (gameType != ENGINE_v1 ? QVector3D(1.0f, 0.0f, 1.0f) : QVector3D(0.0f, 0.0f, 0.0f)));
        rectVAO.bind();
        // manage properties
        camOffset = Vector3<float>(0.0f, 0.0f, 0.0f);

        if (selectedLayer >= 0) {
            if (selectedLayer == l) {
                spriteShader.setValue("useAlpha", false);
                spriteShader.setValue("alpha", 1.0f);
            }
            else {
                spriteShader.setValue("useAlpha", true);
                spriteShader.setValue("alpha", 0.5f);
            }
        }

        // draw
        tilesetTexture->bind();
        spriteShader.setValue("flipX", false);
        spriteShader.setValue("flipY", false);

        int vertCnt = 0;

        int camX = (cam.pos.x + camOffset.x);
        int camY = (cam.pos.y + camOffset.y);
        camX     = qMax(camX, 0);
        camY     = qMax(camY, 0);

        int basedX = qMax(camX / 0x80, 0);
        int basedY = qMax(camY / 0x80, 0);

        int sw = (storedW * invZoom());
        int sh = (storedH * invZoom());

        int countX = width * 0x80 > sw ? (sw / 0x80) : width;
        int countY = height * 0x80 > sh ? (sh / 0x80) : height;

        countX = qMin(basedX + countX + 2, width);
        countY = qMin(basedY + countY + 2, height);

        for (int y = basedY; y < countY; ++y) {
            for (int x = basedX; x < countX; ++x) {
                ushort chunkID = layout[y][x];
                if (chunkID != 0x0 && chunkID < 0x200) {
                    for (int ty = 0; ty < 8; ++ty) {
                        for (int tx = 0; tx < 8; ++tx) {
                            FormatHelpers::Chunks::Tile &tile = chunkset.chunks[chunkID].tiles[ty][tx];

                            float xpos = (x * 0x80) + (tx * 0x10) - (cam.pos.x + camOffset.x);
                            float ypos = (y * 0x80) + (ty * 0x10) - (cam.pos.y + camOffset.y);
                            float zpos = selectedLayer == l ? 8.5 : (8 - l);
                            if (tile.visualPlane == 1)
                                zpos += 0.25; // high plane

                            vertsPtr[vertCnt + 0].setX(0.0f + (xpos / 0x10));
                            vertsPtr[vertCnt + 0].setY(0.0f + (ypos / 0x10));
                            vertsPtr[vertCnt + 0].setZ(zpos);

                            vertsPtr[vertCnt + 1].setX(1.0f + (xpos / 0x10));
                            vertsPtr[vertCnt + 1].setY(0.0f + (ypos / 0x10));
                            vertsPtr[vertCnt + 1].setZ(zpos);

                            vertsPtr[vertCnt + 2].setX(1.0f + (xpos / 0x10));
                            vertsPtr[vertCnt + 2].setY(1.0f + (ypos / 0x10));
                            vertsPtr[vertCnt + 2].setZ(zpos);

                            vertsPtr[vertCnt + 3].setX(1.0f + (xpos / 0x10));
                            vertsPtr[vertCnt + 3].setY(1.0f + (ypos / 0x10));
                            vertsPtr[vertCnt + 3].setZ(zpos);

                            vertsPtr[vertCnt + 4].setX(0.0f + (xpos / 0x10));
                            vertsPtr[vertCnt + 4].setY(1.0f + (ypos / 0x10));
                            vertsPtr[vertCnt + 4].setZ(zpos);

                            vertsPtr[vertCnt + 5].setX(0.0f + (xpos / 0x10));
                            vertsPtr[vertCnt + 5].setY(0.0f + (ypos / 0x10));
                            vertsPtr[vertCnt + 5].setZ(zpos);

                            getTileVerts(tVertsPtr, vertCnt, tile.tileIndex * 0x10, tile.direction);
                            vertCnt += 6;
                        }
                    }
                }
            }
        }

        // Draw Tiles
        {
            QOpenGLVertexArrayObject vao;
            vao.create();
            vao.bind();

            QOpenGLBuffer vVBO2D;
            vVBO2D.create();
            vVBO2D.setUsagePattern(QOpenGLBuffer::StaticDraw);
            vVBO2D.bind();
            vVBO2D.allocate(vertsPtr, vertCnt * sizeof(QVector3D));
            spriteShader.enableAttributeArray(0);
            spriteShader.setAttributeBuffer(0, GL_FLOAT, 0, 3, 0);

            QOpenGLBuffer tVBO2D;
            tVBO2D.create();
            tVBO2D.setUsagePattern(QOpenGLBuffer::StaticDraw);
            tVBO2D.bind();
            tVBO2D.allocate(tVertsPtr, vertCnt * sizeof(QVector2D));
            spriteShader.enableAttributeArray(1);
            spriteShader.setAttributeBuffer(1, GL_FLOAT, 0, 2, 0);

            QMatrix4x4 matModel;
            matModel.scale(0x10 * zoom, 0x10 * zoom, 1.0f);
            spriteShader.setValue("model", matModel);

            f->glDrawArrays(GL_TRIANGLES, 0, vertCnt);

            vao.release();
            tVBO2D.release();
            vVBO2D.release();
        }

        // Collision Previews
        for (int c = 0; c < 2; ++c) {
            if (showCLayers[c]) {
                for (int y = 0; y < height; ++y) {
                    for (int x = 0; x < width; ++x) {
                        ushort chunkID = layout[y][x];
                        if (chunkID != 0x0) {
                            for (int ty = 0; ty < 8; ++ty) {
                                for (int tx = 0; tx < 8; ++tx) {
                                    FormatHelpers::Chunks::Tile &tile =
                                        chunkset.chunks[chunkID].tiles[ty][tx];

                                    float xpos = (x * 0x80) + (tx * 0x10) - cam.pos.x;
                                    float ypos = (y * 0x80) + (ty * 0x10) - cam.pos.y;

                                    Rect<int> check = Rect<int>();
                                    check.x         = (int)((xpos + 0x10) * zoom);
                                    check.y         = (int)((ypos + 0x10) * zoom);
                                    check.w         = (int)(xpos * zoom);
                                    check.h         = (int)(ypos * zoom);

                                    if (check.x < 0 || check.y < 0 || check.w >= storedW
                                        || check.h >= storedH) {
                                        continue;
                                    }

                                    float yStore = ypos;
                                    // draw pixel collision
                                    byte solidity = 0;
                                    RSDKv4::TileConfig::CollisionMask &cmask =
                                        tileconfig.collisionPaths[c][tile.tileIndex];

                                    solidity = !c ? tile.solidityA : tile.solidityB;

                                    if (solidity == 3)
                                        continue;

                                    for (byte cx = 0; cx < 16; ++cx) {
                                        int hm = cx;
                                        if (Utils::getBit(tile.direction, 0))
                                            hm = 15 - cx;

                                        if (!cmask.collision[hm].solid)
                                            continue;

                                        byte cy = cmask.collision[hm].height;
                                        byte ch = 16 - cy;
                                        if (Utils::getBit(tile.direction, 1) && !cmask.direction) {
                                            cy = 0;
                                            ch = 16 - cmask.collision[hm].height;
                                        }
                                        else if (!Utils::getBit(tile.direction, 1) && cmask.direction) {
                                            cy = 0;
                                            ch = cmask.collision[hm].height + 1;
                                        }
                                        else if (Utils::getBit(tile.direction, 1) && cmask.direction) {
                                            cy = 15 - cmask.collision[hm].height;
                                            ch = cmask.collision[hm].height + 1;
                                        }

                                        ypos = yStore + (ch / 2.0);

                                        drawRect((xpos + cx) * zoom, (ypos + cy) * zoom, 15.45,
                                                 1 * zoom, ch * zoom, pixelSolidityClrs[solidity],
                                                 primitiveShader);
                                    }
                                }
                            }
                            spriteShader.use();
                            rectVAO.bind();
                        }
                    }
                }
            }
        }

        // PARALLAX
        if (l == selectedLayer && l > 0) {
            if (background.layers[l - 1].type == 1 || background.layers[l - 1].type == 2) {
                primitiveShader.use();
                primitiveShader.setValue("projection", getProjectionMatrix());
                primitiveShader.setValue("view", QMatrix4x4());
                primitiveShader.setValue("useAlpha", false);
                primitiveShader.setValue("alpha", 1.0f);
                primitiveShader.setValue("projection", matWorld = getProjectionMatrix());
                primitiveShader.setValue("view", matView = QMatrix4x4());
                QMatrix4x4 matModel;
                primitiveShader.setValue("model", matModel);

                QOpenGLVertexArrayObject colVAO;
                colVAO.create();
                colVAO.bind();

                QList<QVector3D> verts;
                if (showParallax) {
                    int id = 0;
                    for (FormatHelpers::Background::ScrollIndexInfo &info :
                         background.layers[l - 1].scrollInfos) {
                        bool isSelected = selectedScrollInfo == id;

                        Vector4<float> clr(1.0f, 1.0f, 0.0f, 1.0f);
                        if (isSelected)
                            clr = Vector4<float>(0.0f, 0.0f, 1.0f, 1.0f);
                        float zpos = (isSelected ? 15.55f : 15.5f);

                        if (background.layers[l - 1].type == 1) {
                            int w = (width * 0x80);
                            drawLine(0.0f * zoom, (info.startLine - cam.pos.y) * zoom, zpos,
                                     (w - cam.pos.x) * zoom, (info.startLine - cam.pos.y) * zoom, zpos,
                                     1.0f, clr, primitiveShader);

                            drawLine(0.0f * zoom, ((info.startLine + info.length) - cam.pos.y) * zoom,
                                     zpos, (w - cam.pos.x) * zoom,
                                     ((info.startLine + info.length) - cam.pos.y) * zoom, zpos, 1.0f,
                                     clr, primitiveShader);
                        }
                        else if (background.layers[l - 1].type == 2) {
                            int h = (height * 0x80) * zoom;
                            drawLine((info.startLine - cam.pos.x) * zoom, 0.0f * zoom, zpos,
                                     (info.startLine - cam.pos.x) * zoom, (h - cam.pos.y) * zoom, zpos,
                                     1.0f, clr, primitiveShader);

                            drawLine(((info.startLine + info.length) - cam.pos.x) * zoom, 0.0f * zoom,
                                     zpos, ((info.startLine + info.length) - cam.pos.x) * zoom,
                                     (h - cam.pos.y) * zoom, zpos, 1.0f, clr, primitiveShader);
                        }

                        ++id;
                    }
                }
            }
        }
    }

    // ENTITIES
    prevSprite = -1;
    spriteShader.use();
    rectVAO.bind();
    spriteShader.setValue("flipX", false);
    spriteShader.setValue("flipY", false);
    spriteShader.setValue("useAlpha", false);
    spriteShader.setValue("alpha", 1.0f);
    for (int o = 0; o < entities.count(); ++o) {
        validDraw        = false;
        entities[o].box  = Rect<int>(0, 0, 0, 0);
        activeDrawEntity = &entities[o];

        if (!objects[entities[o].type].visible)
            continue;

        if (objectsLoaded)
            callGameEvent(EVENT_RSDKDRAW, o);

        if (!validDraw) {
            entities[o].box = Rect<int>(-0x10, -0x10, 0x10, 0x10);

            spriteShader.use();
            rectVAO.bind();
            // Draw Object
            float xpos = entities[o].pos.x - cam.pos.x;
            float ypos = entities[o].pos.y - cam.pos.y;
            float zpos = 10.0f;

            int w = objectSprites[0].texturePtr->width(), h = objectSprites[0].texturePtr->height();
            if (prevSprite) {
                objectSprites[0].texturePtr->bind();
                prevSprite = 0;
            }

            Rect<int> check = Rect<int>();
            check.x         = (int)(xpos + (float)w) * zoom;
            check.y         = (int)(ypos + (float)h) * zoom;
            check.w         = (int)(xpos - (w / 2.0f)) * zoom;
            check.h         = (int)(ypos - (h / 2.0f)) * zoom;
            if (check.x < 0 || check.y < 0 || check.w >= storedW || check.h >= storedH) {
                continue;
            }

            QMatrix4x4 matModel;
            matModel.scale(w * zoom, h * zoom, 1.0f);

            matModel.translate(xpos / (float)w, ypos / (float)h, zpos);
            spriteShader.setValue("model", matModel);

            f->glDrawArrays(GL_TRIANGLES, 0, 6);
        }
        //"Selection" Rect
        // drawRectangle(entities[o].pos.x - 8, entities[o].pos.y - 8, 16, 16, 0x00, 0x00, 0xFF, 0x80);
    }

    // CHUNK PREVIEW
    spriteShader.use();
    rectVAO.bind();
    spriteShader.setValue("useAlpha", true);
    spriteShader.setValue("alpha", 0.75f);
    if (selectedChunk >= 0 && selectedLayer >= 0 && selecting && curTool == TOOL_PENCIL) {
        tilesetTexture->bind();
        float tx = tilePos.x;
        float ty = tilePos.y;

        tx *= invZoom();
        ty *= invZoom();

        float tx2 = tx + fmodf(cam.pos.x, 0x80);
        float ty2 = ty + fmodf(cam.pos.y, 0x80);

        // clip to grid
        tx -= fmodf(tx2, 0x80);
        ty -= fmodf(ty2, 0x80);
        for (int y = 0; y < 8; ++y) {
            for (int x = 0; x < 8; ++x) {
                FormatHelpers::Chunks::Tile &tile = chunkset.chunks[selectedChunk].tiles[y][x];

                // Draw Selected Tile Preview
                float xpos = tx + (x * 0x10) + cam.pos.x;
                float ypos = ty + (y * 0x10) + cam.pos.y;
                float zpos = 15.0f;

                xpos -= (cam.pos.x + camOffset.x);
                ypos -= (cam.pos.y + camOffset.y);

                drawTile(xpos, ypos, zpos, 0, tile.tileIndex * 0x10, tile.direction);
            }
        }
    }

    // ENT PREVIEW
    spriteShader.use();
    rectVAO.bind();
    spriteShader.setValue("flipX", false);
    spriteShader.setValue("flipY", false);
    spriteShader.setValue("useAlpha", true);
    spriteShader.setValue("alpha", 0.75f);
    if (selectedObject >= 0 && selecting && curTool == TOOL_ENTITY) {
        EntityInfo tempEntity;
        activeDrawEntity = &tempEntity;
        bool flag        = false;
        float ex         = tilePos.x;
        float ey         = tilePos.y;

        ex *= invZoom();
        ey *= invZoom();

        float cx = cam.pos.x;
        float cy = cam.pos.y;
        switch (gameType) {
            case ENGINE_v1: {
                /*auto &curObj = compilerv1.objectScriptList[selectedObject];

                if (curObj.subRSDK.scriptCodePtr >= 0 && selectedObject != 0) {
                    compilerv1.objectLoop                              = ENTITY_COUNT - 1;
                    compilerv1.objectEntityList[ENTITY_COUNT - 1].XPos = (ex + cx) * 65536.0f;
                    compilerv1.objectEntityList[ENTITY_COUNT - 1].YPos = (ey + cy) * 65536.0f;
                    compilerv1.processScript(curObj.subRSDK.scriptCodePtr, curObj.subRSDK.jumpTablePtr,
                                             Compilerv1::SUB_RSDK);
                    flag = true;
                }*/
                break;
            }
            case ENGINE_v2: {
                auto &curObj = compilerv2.objectScriptList[selectedObject];

                if (curObj.subRSDK.scriptCodePtr >= 0 && selectedObject != 0) {
                    compilerv2.objectLoop                              = ENTITY_COUNT - 1;
                    compilerv2.objectEntityList[ENTITY_COUNT - 1].XPos = (ex + cx) * 65536.0f;
                    compilerv2.objectEntityList[ENTITY_COUNT - 1].YPos = (ey + cy) * 65536.0f;
                    compilerv2.processScript(curObj.subRSDK.scriptCodePtr, curObj.subRSDK.jumpTablePtr,
                                             Compilerv2::SUB_RSDK);
                    flag = true;
                }
                break;
            }
            case ENGINE_v3: {
                auto &curObj = compilerv3.objectScriptList[selectedObject];

                if (curObj.subRSDKDraw.scriptCodePtr >= 0 && selectedObject != 0) {
                    compilerv3.objectLoop                              = ENTITY_COUNT - 1;
                    compilerv3.objectEntityList[ENTITY_COUNT - 1].XPos = (ex + cx) * 65536.0f;
                    compilerv3.objectEntityList[ENTITY_COUNT - 1].YPos = (ey + cy) * 65536.0f;
                    compilerv3.processScript(curObj.subRSDKDraw.scriptCodePtr,
                                             curObj.subRSDKDraw.jumpTablePtr, Compilerv3::SUB_RSDKDRAW);
                    flag = true;
                }
                break;
            }
            case ENGINE_v4: {
                auto &curObj = compilerv4.objectScriptList[selectedObject];

                if (curObj.eventRSDKDraw.scriptCodePtr >= 0 && selectedObject != 0) {
                    compilerv4.objectEntityList[ENTITY_COUNT - 1].type = selectedObject;
                    compilerv4.objectEntityList[ENTITY_COUNT - 1].XPos = (ex + cx) * 65536.0f;
                    compilerv4.objectEntityList[ENTITY_COUNT - 1].YPos = (ey + cy) * 65536.0f;
                    compilerv4.objectEntityPos                         = ENTITY_COUNT - 1;
                    compilerv4.processScript(curObj.eventRSDKDraw.scriptCodePtr,
                                             curObj.eventRSDKDraw.jumpTablePtr,
                                             Compilerv4::EVENT_RSDKDRAW);
                    flag = true;
                }
                break;
            }
        }

        if (!flag) {
            // Draw Selected Object Preview
            float xpos = ex;
            float ypos = ey;
            float zpos = 15.0f;

            int w = objectSprites[0].texturePtr->width(), h = objectSprites[0].texturePtr->height();
            objectSprites[0].texturePtr->bind();

            QMatrix4x4 matModel;
            matModel.scale(w * zoom, h * zoom, 1.0f);

            matModel.translate(xpos / (float)w, ypos / (float)h, zpos);
            spriteShader.setValue("model", matModel);

            f->glDrawArrays(GL_TRIANGLES, 0, 6);
        }
    }

    spriteShader.setValue("useAlpha", false);
    spriteShader.setValue("alpha", 1.0f);

    // Draw Retro-Sonic Player spawn point
    if (gameType == ENGINE_v1) {
        float px = scene.playerX;
        float py = scene.playerY;

        px *= invZoom();
        py *= invZoom();

        // Draw Player Spawn Preview
        float xpos = px - cam.pos.x;
        float ypos = py - cam.pos.y;
        float zpos = 15.0f;

        int w = rsPlayerSprite->width(), h = rsPlayerSprite->height();
        rsPlayerSprite->bind();

        xpos += w / 2;
        ypos += h / 2;

        QMatrix4x4 matModel;
        matModel.scale(w * zoom, h * zoom, 1.0f);

        matModel.translate(xpos / (float)w, ypos / (float)h, zpos);
        spriteShader.setValue("model", matModel);

        f->glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    // Selected Ent Box
    rectVAO.bind();
    primitiveShader.use();
    primitiveShader.setValue("projection", getProjectionMatrix());
    primitiveShader.setValue("view", QMatrix4x4());
    primitiveShader.setValue("useAlpha", false);
    primitiveShader.setValue("alpha", 1.0f);
    QMatrix4x4 matModel;
    primitiveShader.setValue("model", matModel);
    if (selectedEntity >= 0) {
        EntityInfo &entity = entities[selectedEntity];

        float left   = entity.pos.x + entity.box.x;
        float top    = entity.pos.y + entity.box.y;
        float right  = entity.pos.x + entity.box.w;
        float bottom = entity.pos.y + entity.box.h;

        float w = fabsf(right - left), h = fabsf(bottom - top);
        objectSprites[0].texturePtr->bind();

        drawRect(left - cam.pos.x, top - cam.pos.y, 15.7f, w, h, Vector4<float>(1.0f, 1.0f, 1.0f, 1.0f),
                 primitiveShader, true);
    }

    if (showGrid) {
        rectVAO.bind();

        float camX = cam.pos.x;
        float camY = cam.pos.y;

        int x = camX;
        int y = camY;
        int w = (camX + storedW) * (zoom < 1.0f ? invZoom() : 1.0f);
        int h = (camY + storedH) * (zoom < 1.0f ? invZoom() : 1.0f);
        if (cam.pos.x < 0)
            x += abs(cam.pos.x);
        else
            x -= ((int)camX % gridSize.x);

        if (cam.pos.y < 0)
            y += abs(cam.pos.y);
        else
            y -= ((int)camY % gridSize.y);

        for (; y < h; y += gridSize.y) {
            float dy = (y - camY);

            float x1 = 0;
            float x2 = (((camX + storedW * invZoom())) - camX);
            if (cam.pos.x < 0) {
                x1 += abs(cam.pos.x);
            }

            drawLine(x1, dy, 15.6f, x2, dy, 15.6f, zoom, Vector4<float>(1.0f, 1.0f, 1.0f, 1.0f),
                     primitiveShader);
        }

        for (; x < w; x += gridSize.x) {
            float dx = (x + (zoom <= 1.0f ? 1.0f : 0.0f) - camX);

            float y1 = 0;
            float y2 = (((camY + storedH * invZoom())) - camY);
            if (cam.pos.y < 0) {
                y1 += abs(cam.pos.y);
            }

            drawLine(dx, y1, 15.6f, dx, y2, 15.6f, zoom, Vector4<float>(1.0f, 1.0f, 1.0f, 1.0f),
                     primitiveShader);
        }
    }
}

void SceneViewer::unloadScene()
{
    // QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    if (tilesetTexture) {
        tilesetTexture->destroy();
        delete tilesetTexture;
    }
    tilesetTexture = nullptr;
    tiles.clear();

    chunks.clear();

    for (int o = 0; o < objectSprites.count(); ++o) {
        objectSprites[o].texturePtr->destroy();
        delete objectSprites[o].texturePtr;
        objectSprites[o].name = "";
    }
    objectSprites.clear();

    if (rsPlayerSprite) {
        rsPlayerSprite->destroy();
        delete rsPlayerSprite;
    }

    compilerv2.clearScriptData();
    compilerv3.clearScriptData();
    compilerv4.clearScriptData();

    cam                = SceneCamera();
    selectedChunk      = -1;
    selectedEntity     = -1;
    selectedLayer      = -1;
    selectedScrollInfo = -1;
    selectedObject     = -1;
    selecting          = false;

    if (vertsPtr)
        delete[] vertsPtr;
    if (tVertsPtr)
        delete[] tVertsPtr;
    vertsPtr    = NULL;
    tVertsPtr   = NULL;
    sceneWidth  = 0;
    sceneHeight = 0;

    prevStoredW = -1;
    prevStoredH = -1;
}

void SceneViewer::initializeGL()
{
    // QOpenGLFunctions::initializeOpenGLFunctions();

    // Set up the rendering context, load shaders and other resources, etc.
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    QOpenGLContext *glContext = QOpenGLContext::currentContext();
    QSurfaceFormat fmt        = glContext->format();
    printLog("Widget Using OpenGL " + QString::number(fmt.majorVersion()) + "."
             + QString::number(fmt.minorVersion()));

    const unsigned char *vendor     = f->glGetString(GL_VENDOR);
    const unsigned char *renderer   = f->glGetString(GL_RENDERER);
    const unsigned char *version    = f->glGetString(GL_VERSION);
    const unsigned char *sdrVersion = f->glGetString(GL_SHADING_LANGUAGE_VERSION);
    const unsigned char *extensions = f->glGetString(GL_EXTENSIONS);

    QString vendorStr     = reinterpret_cast<const char *>(vendor);
    QString rendererStr   = reinterpret_cast<const char *>(renderer);
    QString versionStr    = reinterpret_cast<const char *>(version);
    QString sdrVersionStr = reinterpret_cast<const char *>(sdrVersion);
    QString extensionsStr = reinterpret_cast<const char *>(extensions);

    printLog("OpenGL Details");
    printLog("Vendor:       " + vendorStr);
    printLog("Renderer:     " + rendererStr);
    printLog("Version:      " + versionStr);
    printLog("GLSL version: " + sdrVersionStr);
    printLog("Extensions:   " + extensionsStr);
    printLog((QOpenGLContext::currentContext()->isOpenGLES() ? "Using OpenGLES" : "Using OpenGL"));
    printLog((QOpenGLContext::currentContext()->isValid() ? "Is valid" : "Not valid"));

    f->glEnable(GL_DEPTH_TEST);
    f->glDepthFunc(GL_LESS);
    f->glEnable(GL_BLEND);
    f->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    primitiveShader.loadShader(":/shaders/primitive.glv", QOpenGLShader::Vertex);
    primitiveShader.loadShader(":/shaders/primitive.glf", QOpenGLShader::Fragment);
    primitiveShader.link();
    primitiveShader.use();

    spriteShader.loadShader(":/shaders/sprite.glv", QOpenGLShader::Vertex);
    spriteShader.loadShader(":/shaders/sprite.glf", QOpenGLShader::Fragment);
    spriteShader.link();
    spriteShader.use();

    rectVAO.create();
    rectVAO.bind();

    QOpenGLBuffer vVBO2D;
    vVBO2D.create();
    vVBO2D.setUsagePattern(QOpenGLBuffer::StaticDraw);
    vVBO2D.bind();
    vVBO2D.allocate(rectVertices, 6 * sizeof(QVector3D));
    spriteShader.enableAttributeArray(0);
    spriteShader.setAttributeBuffer(0, GL_FLOAT, 0, 3, 0);

    QOpenGLBuffer tVBO2D;
    tVBO2D.create();
    tVBO2D.setUsagePattern(QOpenGLBuffer::StaticDraw);
    tVBO2D.bind();
    tVBO2D.allocate(rectTexCoords, 6 * sizeof(QVector2D));
    spriteShader.enableAttributeArray(1);
    spriteShader.setAttributeBuffer(1, GL_FLOAT, 0, 2, 0);

    // Release (unbind) all
    rectVAO.release();
    vVBO2D.release();
}

void SceneViewer::resizeGL(int w, int h)
{
    storedW             = w;
    storedH             = h;
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glViewport(0, 0, w, h);

    refreshResize();
}

void SceneViewer::paintGL()
{
    // Draw the scene:
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    f->glClearColor(bgColour.r / 255.0f, bgColour.g / 255.0f, bgColour.b / 255.0f, 1.0f);
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    drawScene();
}

void SceneViewer::drawLine(float x1, float y1, float z1, float x2, float y2, float z2, float scale,
                           Vector4<float> colour, Shader &shader)
{
    shader.use();
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    QMatrix4x4 matModel;
    matModel.scale(scale, scale, 1.0f);
    shader.setValue("model", matModel);
    shader.setValue("colour", QVector4D(colour.x, colour.y, colour.z, colour.w));

    QVector3D vertsPtr[2] = { QVector3D(x1, y1, z1), QVector3D(x2, y2, z2) };

    QOpenGLVertexArrayObject VAO;
    VAO.create();
    VAO.bind();

    QOpenGLBuffer VBO;
    VBO.create();
    VBO.setUsagePattern(QOpenGLBuffer::DynamicDraw);
    VBO.bind();
    VBO.allocate(vertsPtr, 2 * sizeof(QVector3D));
    shader.enableAttributeArray(0);
    shader.setAttributeBuffer(0, GL_FLOAT, 0, 3, 0);

    f->glDrawArrays(GL_LINES, 0, 2);

    VAO.release();
    VBO.release();

    VAO.destroy();
    VBO.destroy();
}

int SceneViewer::addGraphicsFile(QString sheetPath)
{
    QString path =
        WorkingDirManager::GetPath("Sprites/" + sheetPath, dataPath + "/Sprites/" + sheetPath);
    if (!QFile::exists(path))
        return 0;

    for (int i = 1; i < objectSprites.count(); ++i) {
        if (QString(sheetPath) == objectSprites[i].name)
            return i;
    }

    int sheetID = -1;
    for (int i = 1; i < objectSprites.count(); ++i) {
        if (objectSprites[i].name == "")
            sheetID = i;
    }

    if (sheetID >= 0) {
        QImage sheet(path);
        TextureInfo tex;
        tex.name               = QString(sheetPath);
        tex.texturePtr         = createTexture(sheet);
        objectSprites[sheetID] = tex;
        return sheetID;
    }
    else {
        QImage sheet(path);
        int cnt = objectSprites.count();
        TextureInfo tex;
        tex.name       = QString(sheetPath);
        tex.texturePtr = createTexture(sheet);
        objectSprites.append(tex);
        return cnt;
    }
}

void SceneViewer::removeGraphicsFile(QString sheetPath, int slot)
{
    if (slot >= 0) {
        objectSprites[slot].texturePtr->destroy();
        delete objectSprites[slot].texturePtr;
        objectSprites[slot].name = "";
    }
    else {
        for (int i = 1; i < objectSprites.count(); ++i) {
            if (QString(sheetPath) == objectSprites[i].name) {
                objectSprites[slot].texturePtr->destroy();
                delete objectSprites[slot].texturePtr;
                objectSprites[slot].name = "";
            }
        }
    }
}

void SceneViewer::drawTile(float XPos, float YPos, float ZPos, int tileX, int tileY, byte direction)
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    // Draw Sprite
    float w = tilesetTexture->width(), h = tilesetTexture->height();

    spriteShader.use();
    QOpenGLVertexArrayObject vao;
    vao.create();
    vao.bind();

    float tx = tileX / w;
    float ty = tileY / h;
    float tw = 0x10 / w;
    float th = 0x10 / h;

    QVector2D *texCoords = nullptr;
    switch (direction) {
        case 0:
        default: {
            QVector2D tc[] = {
                QVector2D(tx, ty),           QVector2D(tx + tw, ty), QVector2D(tx + tw, ty + th),
                QVector2D(tx + tw, ty + th), QVector2D(tx, ty + th), QVector2D(tx, ty),
            };
            texCoords = tc;
            break;
        }
        case 1: {
            QVector2D tc[] = {
                QVector2D(tx + tw, ty), QVector2D(tx, ty),           QVector2D(tx, ty + th),
                QVector2D(tx, ty + th), QVector2D(tx + tw, ty + th), QVector2D(tx + tw, ty),
            };
            texCoords = tc;
            break;
        }
        case 2: {
            QVector2D tc[] = {
                QVector2D(tx, ty + th), QVector2D(tx + tw, ty + th), QVector2D(tx + tw, ty),
                QVector2D(tx + tw, ty), QVector2D(tx, ty),           QVector2D(tx, ty + th),
            };
            texCoords = tc;
            break;
        }
        case 3: {
            QVector2D tc[] = {
                QVector2D(tx + tw, ty + th), QVector2D(tx, ty + th), QVector2D(tx, ty),
                QVector2D(tx, ty),           QVector2D(tx + tw, ty), QVector2D(tx + tw, ty + th),
            };
            texCoords = tc;
            break;
        }
    }

    QOpenGLBuffer vVBO2D;
    vVBO2D.create();
    vVBO2D.setUsagePattern(QOpenGLBuffer::DynamicDraw);
    vVBO2D.bind();
    vVBO2D.allocate(rectVertices, 6 * sizeof(QVector3D));
    spriteShader.enableAttributeArray(0);
    spriteShader.setAttributeBuffer(0, GL_FLOAT, 0, 3, 0);

    QOpenGLBuffer tVBO2D;
    tVBO2D.create();
    tVBO2D.setUsagePattern(QOpenGLBuffer::DynamicDraw);
    tVBO2D.bind();
    tVBO2D.allocate(texCoords, 6 * sizeof(QVector2D));
    spriteShader.enableAttributeArray(1);
    spriteShader.setAttributeBuffer(1, GL_FLOAT, 0, 2, 0);

    QMatrix4x4 matModel;
    matModel.scale(0x10 * zoom, 0x10 * zoom, 1.0f);

    matModel.translate((XPos + (0x10 / 2)) / (float)0x10, (YPos + (0x10 / 2)) / (float)0x10, ZPos);
    spriteShader.setValue("model", matModel);

    f->glDrawArrays(GL_TRIANGLES, 0, 6);

    vao.release();
    tVBO2D.destroy();
    vVBO2D.destroy();
    vao.destroy();
}

int sprDraws = 0;

void SceneViewer::drawRectangle(int x, int y, int w, int h, byte r, byte g, byte b, int a)
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    if (shaderID != 1)
        primitiveShader.use();
    shaderID = 1;

    float xpos = x - cam.pos.x;
    float ypos = y - cam.pos.y;
    float zpos = 10.0f + (sprDraws * 0.001f);
    primitiveShader.setValue(
        "colour", QVector4D(r / 255.0f, g / 255.0f, b / 255.0f, (a > 0xFF ? 0xFF : a) / 255.0f));

    rectVAO.bind();

    QMatrix4x4 matModel;
    matModel.scale(w, h, 1.0f);
    matModel.translate((xpos + (w / 2)) / (float)w, (ypos + (h / 2)) / (float)h, zpos);
    primitiveShader.setValue("model", matModel);

    f->glDrawArrays(GL_TRIANGLES, 0, 6);

    rectVAO.release();

    validDraw = true;
}

void SceneViewer::drawSprite(int XPos, int YPos, int width, int height, int sprX, int sprY, int sheetID)
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    if (width <= 0 || height <= 0)
        return;

    if (sheetID != prevSprite)
        sprDraws = 0;

    // Draw Sprite
    float xpos = XPos - cam.pos.x;
    float ypos = YPos - cam.pos.y;
    float zpos = 10.0f + (sprDraws * 0.001f);

    float entX      = activeDrawEntity->pos.x - cam.pos.x;
    float entY      = activeDrawEntity->pos.y - cam.pos.y;
    float boxLeft   = xpos;
    float boxTop    = ypos;
    float boxRight  = xpos + width;
    float boxBottom = ypos + height;
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

    if (sheetID != prevSprite) {
        objectSprites[sheetID].texturePtr->bind();
        prevSprite = sheetID;
        sprDraws   = 0;
    }
    float w = objectSprites[sheetID].texturePtr->width(),
          h = objectSprites[sheetID].texturePtr->height();

    Rect<int> check = Rect<int>();
    check.x         = (int)(xpos + (float)w) * zoom;
    check.y         = (int)(ypos + (float)h) * zoom;
    check.w         = (int)(xpos - (w / 2.0f)) * zoom;
    check.h         = (int)(ypos - (h / 2.0f)) * zoom;
    if (check.x < 0 || check.y < 0 || check.w >= storedW || check.h >= storedH || !sheetID) {
        return;
    }

    if (shaderID != 0)
        spriteShader.use();
    shaderID = 0;

    spriteShader.setValue("flipX", false);
    spriteShader.setValue("flipY", false);
    spriteShader.setValue("useAlpha", false);
    spriteShader.setValue("alpha", 1.0f);
    QOpenGLVertexArrayObject vao;
    vao.create();
    vao.bind();

    double tx = sprX / w;
    double ty = sprY / h;
    double tw = (sprX + width) / w;
    double th = (sprY + height) / h;

    const QVector2D texCoords[] = {
        QVector2D(tx, ty), QVector2D(tw, ty), QVector2D(tw, th),
        QVector2D(tw, th), QVector2D(tx, th), QVector2D(tx, ty),
    };

    QOpenGLBuffer vVBO2D;
    vVBO2D.create();
    vVBO2D.setUsagePattern(QOpenGLBuffer::DynamicDraw);
    vVBO2D.bind();
    vVBO2D.allocate(rectVertices, 6 * sizeof(QVector3D));
    spriteShader.enableAttributeArray(0);
    spriteShader.setAttributeBuffer(0, GL_FLOAT, 0, 3, 0);

    QOpenGLBuffer tVBO2D;
    tVBO2D.create();
    tVBO2D.setUsagePattern(QOpenGLBuffer::DynamicDraw);
    tVBO2D.bind();
    tVBO2D.allocate(texCoords, 6 * sizeof(QVector2D));
    spriteShader.enableAttributeArray(1);
    spriteShader.setAttributeBuffer(1, GL_FLOAT, 0, 2, 0);

    QMatrix4x4 matModel;
    matModel.scale(width * zoom, height * zoom, 1.0f);

    matModel.translate((xpos + (width / 2)) / (float)width, (ypos + (height / 2)) / (float)height,
                       zpos);
    spriteShader.setValue("model", matModel);

    f->glDrawArrays(GL_TRIANGLES, 0, 6);
    sprDraws++;

    validDraw = true;
}

void SceneViewer::drawSpriteFlipped(int XPos, int YPos, int width, int height, int sprX, int sprY,
                                    int direction, int sheetID)
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    if (width <= 0 || height <= 0)
        return;

    if (sheetID != prevSprite)
        sprDraws = 0;

    // Draw Sprite
    float xpos = XPos - cam.pos.x;
    float ypos = YPos - cam.pos.y;
    float zpos = 10.0f + (sprDraws * 0.001f);

    float entX      = activeDrawEntity->pos.x - cam.pos.x;
    float entY      = activeDrawEntity->pos.y - cam.pos.y;
    float boxLeft   = xpos;
    float boxTop    = ypos;
    float boxRight  = xpos + width;
    float boxBottom = ypos + height;
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

    if (sheetID != prevSprite) {
        objectSprites[sheetID].texturePtr->bind();
        prevSprite = sheetID;
    }
    float w = objectSprites[sheetID].texturePtr->width(),
          h = objectSprites[sheetID].texturePtr->height();

    Rect<int> check = Rect<int>();
    check.x         = (int)(xpos + (float)w) * zoom;
    check.y         = (int)(ypos + (float)h) * zoom;
    check.w         = (int)(xpos - (w / 2.0f)) * zoom;
    check.h         = (int)(ypos - (h / 2.0f)) * zoom;
    if (check.x < 0 || check.y < 0 || check.w >= storedW || check.h >= storedH || !sheetID) {
        return;
    }

    if (shaderID != 0)
        spriteShader.use();
    shaderID = 0;

    spriteShader.setValue("flipX", false);
    spriteShader.setValue("flipY", false);
    spriteShader.setValue("useAlpha", false);
    spriteShader.setValue("alpha", 1.0f);
    QOpenGLVertexArrayObject vao;
    vao.create();
    vao.bind();

    float tx = sprX / w;
    float ty = sprY / h;
    float tw = width / w;
    float th = height / h;

    QVector2D *texCoords = nullptr;
    switch (direction) {
        case 0:
        default: {
            QVector2D tc[] = {
                QVector2D(tx, ty),           QVector2D(tx + tw, ty), QVector2D(tx + tw, ty + th),
                QVector2D(tx + tw, ty + th), QVector2D(tx, ty + th), QVector2D(tx, ty),
            };
            texCoords = tc;
            break;
        }
        case 1: {
            QVector2D tc[] = {
                QVector2D(tx + tw, ty), QVector2D(tx, ty),           QVector2D(tx, ty + th),
                QVector2D(tx, ty + th), QVector2D(tx + tw, ty + th), QVector2D(tx + tw, ty),
            };
            texCoords = tc;
            break;
        }
        case 2: {
            QVector2D tc[] = {
                QVector2D(tx, ty + th), QVector2D(tx + tw, ty + th), QVector2D(tx + tw, ty),
                QVector2D(tx + tw, ty), QVector2D(tx, ty),           QVector2D(tx, ty + th),
            };
            texCoords = tc;
            break;
        }
        case 3: {
            QVector2D tc[] = {
                QVector2D(tx + tw, ty + th), QVector2D(tx, ty + th), QVector2D(tx, ty),
                QVector2D(tx, ty),           QVector2D(tx + tw, ty), QVector2D(tx + tw, ty + th),
            };
            texCoords = tc;
            break;
        }
    }

    QOpenGLBuffer vVBO2D;
    vVBO2D.create();
    vVBO2D.setUsagePattern(QOpenGLBuffer::DynamicDraw);
    vVBO2D.bind();
    vVBO2D.allocate(rectVertices, 6 * sizeof(QVector3D));
    spriteShader.enableAttributeArray(0);
    spriteShader.setAttributeBuffer(0, GL_FLOAT, 0, 3, 0);

    QOpenGLBuffer tVBO2D;
    tVBO2D.create();
    tVBO2D.setUsagePattern(QOpenGLBuffer::DynamicDraw);
    tVBO2D.bind();
    tVBO2D.allocate(texCoords, 6 * sizeof(QVector2D));
    spriteShader.enableAttributeArray(1);
    spriteShader.setAttributeBuffer(1, GL_FLOAT, 0, 2, 0);

    QMatrix4x4 matModel;
    matModel.scale(width * zoom, height * zoom, 1.0f);

    matModel.translate((xpos + (width / 2)) / (float)width, (ypos + (height / 2)) / (float)height,
                       zpos);
    spriteShader.setValue("model", matModel);

    f->glDrawArrays(GL_TRIANGLES, 0, 6);
    sprDraws++;

    validDraw = true;
}

void SceneViewer::drawSpriteScaled(int direction, int XPos, int YPos, int pivotX, int pivotY,
                                   int scaleX, int scaleY, int width, int height, int sprX, int sprY,
                                   int sheetID)
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    if (width <= 0 || height <= 0)
        return;

    if (sheetID != prevSprite)
        sprDraws = 0;

    // Draw Sprite
    XPos += pivotX;
    YPos += pivotY;
    float xpos = XPos - cam.pos.x;
    float ypos = YPos - cam.pos.y;
    float zpos = 10.0f + (sprDraws * 0.001f);

    float entX      = activeDrawEntity->pos.x - cam.pos.x;
    float entY      = activeDrawEntity->pos.y - cam.pos.y;
    float boxLeft   = xpos;
    float boxTop    = ypos;
    float boxRight  = xpos + width;
    float boxBottom = ypos + height;
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

    if (sheetID != prevSprite) {
        objectSprites[sheetID].texturePtr->bind();
        prevSprite = sheetID;
    }
    float w = objectSprites[sheetID].texturePtr->width(),
          h = objectSprites[sheetID].texturePtr->height();

    Rect<int> check = Rect<int>();
    check.x         = (int)(xpos + (float)w) * zoom;
    check.y         = (int)(ypos + (float)h) * zoom;
    check.w         = (int)(xpos - (w / 2.0f)) * zoom;
    check.h         = (int)(ypos - (h / 2.0f)) * zoom;
    if (check.x < 0 || check.y < 0 || check.w >= storedW || check.h >= storedH || !sheetID) {
        return;
    }

    if (shaderID != 0)
        spriteShader.use();
    shaderID = 0;

    spriteShader.setValue("flipX", false);
    spriteShader.setValue("flipY", false);
    spriteShader.setValue("useAlpha", false);
    spriteShader.setValue("alpha", 1.0f);
    QOpenGLVertexArrayObject vao;
    vao.create();
    vao.bind();

    float tx = sprX / w;
    float ty = sprY / h;
    float tw = width / w;
    float th = height / h;

    QVector2D *texCoords = nullptr;
    switch (direction) {
        case 0:
        default: {
            QVector2D tc[] = {
                QVector2D(tx, ty),           QVector2D(tx + tw, ty), QVector2D(tx + tw, ty + th),
                QVector2D(tx + tw, ty + th), QVector2D(tx, ty + th), QVector2D(tx, ty),
            };
            texCoords = tc;
            break;
        }
        case 1: {
            QVector2D tc[] = {
                QVector2D(tx + tw, ty), QVector2D(tx, ty),           QVector2D(tx, ty + th),
                QVector2D(tx, ty + th), QVector2D(tx + tw, ty + th), QVector2D(tx + tw, ty),
            };
            texCoords = tc;
            break;
        }
        case 2: {
            QVector2D tc[] = {
                QVector2D(tx, ty + th), QVector2D(tx + tw, ty + th), QVector2D(tx + tw, ty),
                QVector2D(tx + tw, ty), QVector2D(tx, ty),           QVector2D(tx, ty + th),
            };
            texCoords = tc;
            break;
        }
        case 3: {
            QVector2D tc[] = {
                QVector2D(tx + tw, ty + th), QVector2D(tx, ty + th), QVector2D(tx, ty),
                QVector2D(tx, ty),           QVector2D(tx + tw, ty), QVector2D(tx + tw, ty + th),
            };
            texCoords = tc;
            break;
        }
    }

    QOpenGLBuffer vVBO2D;
    vVBO2D.create();
    vVBO2D.setUsagePattern(QOpenGLBuffer::DynamicDraw);
    vVBO2D.bind();
    vVBO2D.allocate(rectVertices, 6 * sizeof(QVector3D));
    spriteShader.enableAttributeArray(0);
    spriteShader.setAttributeBuffer(0, GL_FLOAT, 0, 3, 0);

    QOpenGLBuffer tVBO2D;
    tVBO2D.create();
    tVBO2D.setUsagePattern(QOpenGLBuffer::DynamicDraw);
    tVBO2D.bind();
    tVBO2D.allocate(texCoords, 6 * sizeof(QVector2D));
    spriteShader.enableAttributeArray(1);
    spriteShader.setAttributeBuffer(1, GL_FLOAT, 0, 2, 0);

    float scalefX = (512.0f / scaleX);
    float scalefY = (512.0f / scaleY);

    QMatrix4x4 matModel;
    matModel.scale((width * scalefX) * zoom, (height * scalefY) * zoom, 1.0f);

    matModel.translate((xpos + (width / 2)) / (float)width, (ypos + (height / 2)) / (float)height,
                       zpos);
    spriteShader.setValue("model", matModel);

    f->glDrawArrays(GL_TRIANGLES, 0, 6);
    sprDraws++;
}
void SceneViewer::drawSpriteRotated(int direction, int XPos, int YPos, int pivotX, int pivotY, int sprX,
                                    int sprY, int width, int height, int rotation, int sheetID)
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    if (width <= 0 || height <= 0)
        return;

    if (sheetID != prevSprite)
        sprDraws = 0;

    // Draw Sprite
    XPos += pivotX;
    YPos += pivotY;
    float xpos = XPos - cam.pos.x;
    float ypos = YPos - cam.pos.y;
    float zpos = 10.0f + (sprDraws * 0.001f);

    float entX      = activeDrawEntity->pos.x - cam.pos.x;
    float entY      = activeDrawEntity->pos.y - cam.pos.y;
    float boxLeft   = xpos;
    float boxTop    = ypos;
    float boxRight  = xpos + width;
    float boxBottom = ypos + height;
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

    if (sheetID != prevSprite) {
        objectSprites[sheetID].texturePtr->bind();
        prevSprite = sheetID;
    }
    float w = objectSprites[sheetID].texturePtr->width(),
          h = objectSprites[sheetID].texturePtr->height();

    Rect<int> check = Rect<int>();
    check.x         = (int)(xpos + (float)w) * zoom;
    check.y         = (int)(ypos + (float)h) * zoom;
    check.w         = (int)(xpos - (w / 2.0f)) * zoom;
    check.h         = (int)(ypos - (h / 2.0f)) * zoom;
    if (check.x < 0 || check.y < 0 || check.w >= storedW || check.h >= storedH || !sheetID) {
        return;
    }

    if (shaderID != 0)
        spriteShader.use();
    shaderID = 0;

    spriteShader.setValue("flipX", false);
    spriteShader.setValue("flipY", false);
    spriteShader.setValue("useAlpha", false);
    spriteShader.setValue("alpha", 1.0f);
    QOpenGLVertexArrayObject vao;
    vao.create();
    vao.bind();

    float tx = sprX / w;
    float ty = sprY / h;
    float tw = width / w;
    float th = height / h;

    QVector2D *texCoords = nullptr;
    switch (direction) {
        case 0:
        default: {
            QVector2D tc[] = {
                QVector2D(tx, ty),           QVector2D(tx + tw, ty), QVector2D(tx + tw, ty + th),
                QVector2D(tx + tw, ty + th), QVector2D(tx, ty + th), QVector2D(tx, ty),
            };
            texCoords = tc;
            break;
        }
        case 1: {
            QVector2D tc[] = {
                QVector2D(tx + tw, ty), QVector2D(tx, ty),           QVector2D(tx, ty + th),
                QVector2D(tx, ty + th), QVector2D(tx + tw, ty + th), QVector2D(tx + tw, ty),
            };
            texCoords = tc;
            break;
        }
        case 2: {
            QVector2D tc[] = {
                QVector2D(tx, ty + th), QVector2D(tx + tw, ty + th), QVector2D(tx + tw, ty),
                QVector2D(tx + tw, ty), QVector2D(tx, ty),           QVector2D(tx, ty + th),
            };
            texCoords = tc;
            break;
        }
        case 3: {
            QVector2D tc[] = {
                QVector2D(tx + tw, ty + th), QVector2D(tx, ty + th), QVector2D(tx, ty),
                QVector2D(tx, ty),           QVector2D(tx + tw, ty), QVector2D(tx + tw, ty + th),
            };
            texCoords = tc;
            break;
        }
    }

    QOpenGLBuffer vVBO2D;
    vVBO2D.create();
    vVBO2D.setUsagePattern(QOpenGLBuffer::DynamicDraw);
    vVBO2D.bind();
    vVBO2D.allocate(rectVertices, 6 * sizeof(QVector3D));
    spriteShader.enableAttributeArray(0);
    spriteShader.setAttributeBuffer(0, GL_FLOAT, 0, 3, 0);

    QOpenGLBuffer tVBO2D;
    tVBO2D.create();
    tVBO2D.setUsagePattern(QOpenGLBuffer::DynamicDraw);
    tVBO2D.bind();
    tVBO2D.allocate(texCoords, 6 * sizeof(QVector2D));
    spriteShader.enableAttributeArray(1);
    spriteShader.setAttributeBuffer(1, GL_FLOAT, 0, 2, 0);

    QMatrix4x4 matModel;
    int angle = 0x200 - (rotation & 0x1FF);
    if (!(rotation & 0x1FF))
        angle = rotation & 0x1FF;
    float rotationf = (angle / 512.0f) * 360.0f;

    matModel.scale(width * zoom, height * zoom, 1.0f);
    matModel.rotate(rotationf, 0.0, 0.0, 1.0);

    matModel.translate((xpos + (width / 2)) / (float)width, (ypos + (height / 2)) / (float)height,
                       zpos);
    spriteShader.setValue("model", matModel);

    f->glDrawArrays(GL_TRIANGLES, 0, 6);
    sprDraws++;

    validDraw = true;
}
void SceneViewer::drawSpriteRotozoom(int direction, int XPos, int YPos, int pivotX, int pivotY,
                                     int sprX, int sprY, int width, int height, int rotation, int scale,
                                     int sheetID)
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    if (width <= 0 || height <= 0)
        return;

    if (sheetID != prevSprite)
        sprDraws = 0;

    // Draw Sprite
    XPos += pivotX;
    YPos += pivotY;
    float xpos = XPos - cam.pos.x;
    float ypos = YPos - cam.pos.y;
    float zpos = 10.0f + (sprDraws * 0.001f);

    float entX      = activeDrawEntity->pos.x - cam.pos.x;
    float entY      = activeDrawEntity->pos.y - cam.pos.y;
    float boxLeft   = xpos;
    float boxTop    = ypos;
    float boxRight  = xpos + width;
    float boxBottom = ypos + height;
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

    if (sheetID != prevSprite) {
        objectSprites[sheetID].texturePtr->bind();
        prevSprite = sheetID;
    }
    float w = objectSprites[sheetID].texturePtr->width(),
          h = objectSprites[sheetID].texturePtr->height();

    Rect<int> check = Rect<int>();
    check.x         = (int)(xpos + (float)w) * zoom;
    check.y         = (int)(ypos + (float)h) * zoom;
    check.w         = (int)(xpos - (w / 2.0f)) * zoom;
    check.h         = (int)(ypos - (h / 2.0f)) * zoom;
    if (check.x < 0 || check.y < 0 || check.w >= storedW || check.h >= storedH || !sheetID) {
        return;
    }

    if (shaderID != 0)
        spriteShader.use();
    shaderID = 0;

    spriteShader.setValue("flipX", false);
    spriteShader.setValue("flipY", false);
    spriteShader.setValue("useAlpha", false);
    spriteShader.setValue("alpha", 1.0f);
    QOpenGLVertexArrayObject vao;
    vao.create();
    vao.bind();

    float tx = sprX / w;
    float ty = sprY / h;
    float tw = width / w;
    float th = height / h;

    QVector2D *texCoords = nullptr;
    switch (direction) {
        case 0:
        default: {
            QVector2D tc[] = {
                QVector2D(tx, ty),           QVector2D(tx + tw, ty), QVector2D(tx + tw, ty + th),
                QVector2D(tx + tw, ty + th), QVector2D(tx, ty + th), QVector2D(tx, ty),
            };
            texCoords = tc;
            break;
        }
        case 1: {
            QVector2D tc[] = {
                QVector2D(tx + tw, ty), QVector2D(tx, ty),           QVector2D(tx, ty + th),
                QVector2D(tx, ty + th), QVector2D(tx + tw, ty + th), QVector2D(tx + tw, ty),
            };
            texCoords = tc;
            break;
        }
        case 2: {
            QVector2D tc[] = {
                QVector2D(tx, ty + th), QVector2D(tx + tw, ty + th), QVector2D(tx + tw, ty),
                QVector2D(tx + tw, ty), QVector2D(tx, ty),           QVector2D(tx, ty + th),
            };
            texCoords = tc;
            break;
        }
        case 3: {
            QVector2D tc[] = {
                QVector2D(tx + tw, ty + th), QVector2D(tx, ty + th), QVector2D(tx, ty),
                QVector2D(tx, ty),           QVector2D(tx + tw, ty), QVector2D(tx + tw, ty + th),
            };
            texCoords = tc;
            break;
        }
    }

    QOpenGLBuffer vVBO2D;
    vVBO2D.create();
    vVBO2D.setUsagePattern(QOpenGLBuffer::DynamicDraw);
    vVBO2D.bind();
    vVBO2D.allocate(rectVertices, 6 * sizeof(QVector3D));
    spriteShader.enableAttributeArray(0);
    spriteShader.setAttributeBuffer(0, GL_FLOAT, 0, 3, 0);

    QOpenGLBuffer tVBO2D;
    tVBO2D.create();
    tVBO2D.setUsagePattern(QOpenGLBuffer::DynamicDraw);
    tVBO2D.bind();
    tVBO2D.allocate(texCoords, 6 * sizeof(QVector2D));
    spriteShader.enableAttributeArray(1);
    spriteShader.setAttributeBuffer(1, GL_FLOAT, 0, 2, 0);

    QMatrix4x4 matModel;
    float scalefX = (512.0f / scale);
    float scalefY = (512.0f / scale);
    int angle     = 0x200 - (rotation & 0x1FF);
    if (!(rotation & 0x1FF))
        angle = rotation & 0x1FF;
    float rotationf = (angle / 512.0f) * 360.0f;

    matModel.scale((width * scalefX) * zoom, (height * scalefY) * zoom, 1.0f);
    matModel.rotate(rotationf, 0.0, 0.0, 1.0);

    matModel.translate((xpos + (width / 2)) / (float)width, (ypos + (height / 2)) / (float)height,
                       zpos);
    spriteShader.setValue("model", matModel);

    f->glDrawArrays(GL_TRIANGLES, 0, 6);
    sprDraws++;

    validDraw = true;
}

void SceneViewer::drawBlendedSprite(int XPos, int YPos, int width, int height, int sprX, int sprY,
                                    int sheetID)
{
    drawAlphaBlendedSprite(XPos, YPos, width, height, sprX, sprY, 0x80, sheetID);
}

void SceneViewer::drawAlphaBlendedSprite(int XPos, int YPos, int width, int height, int sprX, int sprY,
                                         int alpha, int sheetID)
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    if (width <= 0 || height <= 0)
        return;

    if (sheetID != prevSprite)
        sprDraws = 0;

    // Draw Sprite
    float xpos = XPos - cam.pos.x;
    float ypos = YPos - cam.pos.y;
    float zpos = 10.0f + (sprDraws * 0.001f);

    float entX      = activeDrawEntity->pos.x - cam.pos.x;
    float entY      = activeDrawEntity->pos.y - cam.pos.y;
    float boxLeft   = xpos;
    float boxTop    = ypos;
    float boxRight  = xpos + width;
    float boxBottom = ypos + height;
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

    if (sheetID != prevSprite) {
        objectSprites[sheetID].texturePtr->bind();
        prevSprite = sheetID;
    }
    float w = objectSprites[sheetID].texturePtr->width(),
          h = objectSprites[sheetID].texturePtr->height();

    Rect<int> check = Rect<int>();
    check.x         = (int)(xpos + (float)w) * zoom;
    check.y         = (int)(ypos + (float)h) * zoom;
    check.w         = (int)(xpos - (w / 2.0f)) * zoom;
    check.h         = (int)(ypos - (h / 2.0f)) * zoom;
    if (check.x < 0 || check.y < 0 || check.w >= storedW || check.h >= storedH || !sheetID) {
        return;
    }

    if (shaderID != 0)
        spriteShader.use();
    shaderID = 0;

    spriteShader.setValue("flipX", false);
    spriteShader.setValue("flipY", false);
    spriteShader.setValue("useAlpha", true);
    spriteShader.setValue("alpha", (alpha > 0xFF ? 0xFF : alpha) / 255.0f);
    QOpenGLVertexArrayObject vao;
    vao.create();
    vao.bind();

    float tx = sprX / w;
    float ty = sprY / h;
    float tw = width / w;
    float th = height / h;

    const QVector2D texCoords[] = {
        QVector2D(tx, ty),           QVector2D(tx + tw, ty), QVector2D(tx + tw, ty + th),
        QVector2D(tx + tw, ty + th), QVector2D(tx, ty + th), QVector2D(tx, ty),
    };

    QOpenGLBuffer vVBO2D;
    vVBO2D.create();
    vVBO2D.setUsagePattern(QOpenGLBuffer::DynamicDraw);
    vVBO2D.bind();
    vVBO2D.allocate(rectVertices, 6 * sizeof(QVector3D));
    spriteShader.enableAttributeArray(0);
    spriteShader.setAttributeBuffer(0, GL_FLOAT, 0, 3, 0);

    QOpenGLBuffer tVBO2D;
    tVBO2D.create();
    tVBO2D.setUsagePattern(QOpenGLBuffer::DynamicDraw);
    tVBO2D.bind();
    tVBO2D.allocate(texCoords, 6 * sizeof(QVector2D));
    spriteShader.enableAttributeArray(1);
    spriteShader.setAttributeBuffer(1, GL_FLOAT, 0, 2, 0);

    QMatrix4x4 matModel;
    matModel.scale(width * zoom, height * zoom, 1.0f);

    matModel.translate((xpos + (width / 2)) / (float)width, (ypos + (height / 2)) / (float)height,
                       zpos);
    spriteShader.setValue("model", matModel);

    f->glDrawArrays(GL_TRIANGLES, 0, 6);
    sprDraws++;

    validDraw = true;
}

// TODO: make these 2 not just "alpha"
void SceneViewer::drawAdditiveBlendedSprite(int XPos, int YPos, int width, int height, int sprX,
                                            int sprY, int alpha, int sheetID)
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    if (width <= 0 || height <= 0)
        return;

    if (sheetID != prevSprite)
        sprDraws = 0;

    // Draw Sprite
    float xpos = XPos - cam.pos.x;
    float ypos = YPos - cam.pos.y;
    float zpos = 10.0f + (sprDraws * 0.001f);

    float entX      = activeDrawEntity->pos.x - cam.pos.x;
    float entY      = activeDrawEntity->pos.y - cam.pos.y;
    float boxLeft   = xpos;
    float boxTop    = ypos;
    float boxRight  = xpos + width;
    float boxBottom = ypos + height;
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

    if (sheetID != prevSprite) {
        objectSprites[sheetID].texturePtr->bind();
        prevSprite = sheetID;
    }
    float w = objectSprites[sheetID].texturePtr->width(),
          h = objectSprites[sheetID].texturePtr->height();

    Rect<int> check = Rect<int>();
    check.x         = (int)(xpos + (float)w) * zoom;
    check.y         = (int)(ypos + (float)h) * zoom;
    check.w         = (int)(xpos - (w / 2.0f)) * zoom;
    check.h         = (int)(ypos - (h / 2.0f)) * zoom;
    if (check.x < 0 || check.y < 0 || check.w >= storedW || check.h >= storedH || !sheetID) {
        return;
    }

    if (shaderID != 0)
        spriteShader.use();
    shaderID = 0;

    spriteShader.setValue("flipX", false);
    spriteShader.setValue("flipY", false);
    spriteShader.setValue("useAlpha", true);
    spriteShader.setValue("alpha", (alpha > 0xFF ? 0xFF : alpha) / 255.0f);
    QOpenGLVertexArrayObject vao;
    vao.create();
    vao.bind();

    float tx = sprX / w;
    float ty = sprY / h;
    float tw = width / w;
    float th = height / h;

    const QVector2D texCoords[] = {
        QVector2D(tx, ty),           QVector2D(tx + tw, ty), QVector2D(tx + tw, ty + th),
        QVector2D(tx + tw, ty + th), QVector2D(tx, ty + th), QVector2D(tx, ty),
    };

    QOpenGLBuffer vVBO2D;
    vVBO2D.create();
    vVBO2D.setUsagePattern(QOpenGLBuffer::DynamicDraw);
    vVBO2D.bind();
    vVBO2D.allocate(rectVertices, 6 * sizeof(QVector3D));
    spriteShader.enableAttributeArray(0);
    spriteShader.setAttributeBuffer(0, GL_FLOAT, 0, 3, 0);

    QOpenGLBuffer tVBO2D;
    tVBO2D.create();
    tVBO2D.setUsagePattern(QOpenGLBuffer::DynamicDraw);
    tVBO2D.bind();
    tVBO2D.allocate(texCoords, 6 * sizeof(QVector2D));
    spriteShader.enableAttributeArray(1);
    spriteShader.setAttributeBuffer(1, GL_FLOAT, 0, 2, 0);

    QMatrix4x4 matModel;
    matModel.scale(width * zoom, height * zoom, 1.0f);

    matModel.translate((xpos + (width / 2)) / (float)width, (ypos + (height / 2)) / (float)height,
                       zpos);
    spriteShader.setValue("model", matModel);

    f->glDrawArrays(GL_TRIANGLES, 0, 6);
    sprDraws++;

    validDraw = true;
}
void SceneViewer::drawSubtractiveBlendedSprite(int XPos, int YPos, int width, int height, int sprX,
                                               int sprY, int alpha, int sheetID)
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    if (width <= 0 || height <= 0)
        return;

    if (sheetID != prevSprite)
        sprDraws = 0;

    // Draw Sprite
    float xpos = XPos - cam.pos.x;
    float ypos = YPos - cam.pos.y;
    float zpos = 10.0f + (sprDraws * 0.001f);

    float entX      = activeDrawEntity->pos.x - cam.pos.x;
    float entY      = activeDrawEntity->pos.y - cam.pos.y;
    float boxLeft   = xpos;
    float boxTop    = ypos;
    float boxRight  = xpos + width;
    float boxBottom = ypos + height;
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

    if (sheetID != prevSprite) {
        objectSprites[sheetID].texturePtr->bind();
        prevSprite = sheetID;
    }
    float w = objectSprites[sheetID].texturePtr->width(),
          h = objectSprites[sheetID].texturePtr->height();

    Rect<int> check = Rect<int>();
    check.x         = (int)(xpos + (float)w) * zoom;
    check.y         = (int)(ypos + (float)h) * zoom;
    check.w         = (int)(xpos - (w / 2.0f)) * zoom;
    check.h         = (int)(ypos - (h / 2.0f)) * zoom;
    if (check.x < 0 || check.y < 0 || check.w >= storedW || check.h >= storedH || !sheetID) {
        return;
    }

    if (shaderID != 0)
        spriteShader.use();
    shaderID = 0;

    spriteShader.setValue("flipX", false);
    spriteShader.setValue("flipY", false);
    spriteShader.setValue("useAlpha", true);
    spriteShader.setValue("alpha", (alpha > 0xFF ? 0xFF : alpha) / 255.0f);
    QOpenGLVertexArrayObject vao;
    vao.create();
    vao.bind();

    float tx = sprX / w;
    float ty = sprY / h;
    float tw = width / w;
    float th = height / h;

    const QVector2D texCoords[] = {
        QVector2D(tx, ty),           QVector2D(tx + tw, ty), QVector2D(tx + tw, ty + th),
        QVector2D(tx + tw, ty + th), QVector2D(tx, ty + th), QVector2D(tx, ty),
    };

    QOpenGLBuffer vVBO2D;
    vVBO2D.create();
    vVBO2D.setUsagePattern(QOpenGLBuffer::DynamicDraw);
    vVBO2D.bind();
    vVBO2D.allocate(rectVertices, 6 * sizeof(QVector3D));
    spriteShader.enableAttributeArray(0);
    spriteShader.setAttributeBuffer(0, GL_FLOAT, 0, 3, 0);

    QOpenGLBuffer tVBO2D;
    tVBO2D.create();
    tVBO2D.setUsagePattern(QOpenGLBuffer::DynamicDraw);
    tVBO2D.bind();
    tVBO2D.allocate(texCoords, 6 * sizeof(QVector2D));
    spriteShader.enableAttributeArray(1);
    spriteShader.setAttributeBuffer(1, GL_FLOAT, 0, 2, 0);

    QMatrix4x4 matModel;
    matModel.scale(width * zoom, height * zoom, 1.0f);

    matModel.translate((xpos + (width / 2)) / (float)width, (ypos + (height / 2)) / (float)height,
                       zpos);
    spriteShader.setValue("model", matModel);

    f->glDrawArrays(GL_TRIANGLES, 0, 6);
    sprDraws++;

    validDraw = true;
}

bool SceneViewer::callGameEvent(byte eventID, int id)
{
    bool called = false;
    switch (gameType) {
        case ENGINE_v3:
            switch (eventID) {
                case EVENT_RSDKLOAD: {
                    auto &curObj           = compilerv3.objectScriptList[id];
                    curObj.frameListOffset = compilerv3.scriptFrameCount;
                    curObj.spriteSheetID   = 0;
                    compilerv3.objectEntityList[ENTITY_COUNT - 1].type = id;

                    auto &curSub = curObj.subRSDKLoad;
                    if (curSub.scriptCodePtr != SCRIPTDATA_COUNT - 1) {
                        compilerv3.processScript(curSub.scriptCodePtr, curSub.jumpTablePtr,
                                                 Compilerv3::SUB_RSDKLOAD);
                        called = true;
                    }
                    curObj.spriteFrameCount = compilerv3.scriptFrameCount - curObj.frameListOffset;
                    break;
                }
                case EVENT_RSDKDRAW: {
                    auto &curObj = compilerv3.objectScriptList[entities[id].type];

                    if (curObj.subRSDKDraw.scriptCodePtr != SCRIPTDATA_COUNT - 1
                        && entities[id].type != 0) {
                        compilerv3.objectLoop = id;
                        compilerv3.processScript(curObj.subRSDKDraw.scriptCodePtr,
                                                 curObj.subRSDKDraw.jumpTablePtr,
                                                 Compilerv3::SUB_RSDKDRAW);
                        called = true;
                    }
                    break;
                }
                case EVENT_RSDKEDIT: {
                    auto &curObj = compilerv3.objectScriptList[entities[id].type];

                    if (curObj.subRSDKEdit.scriptCodePtr != SCRIPTDATA_COUNT - 1
                        && entities[id].type != 0) {
                        compilerv3.objectLoop = id;
                        activeVarObj          = entities[id].type;

                        compilerv3.processScript(curObj.subRSDKEdit.scriptCodePtr,
                                                 curObj.subRSDKEdit.jumpTablePtr,
                                                 Compilerv3::SUB_RSDKEDIT);
                        called = true;
                    }
                    break;
                }
            }
            break;
        case ENGINE_v4:
            switch (eventID) {
                case EVENT_RSDKLOAD: {
                    auto &curObj           = compilerv4.objectScriptList[id];
                    curObj.frameListOffset = compilerv4.scriptFrameCount;
                    curObj.spriteSheetID   = 0;
                    compilerv4.objectEntityList[ENTITY_COUNT - 1].type = id;

                    auto &curSub = curObj.eventRSDKLoad;
                    if (curSub.scriptCodePtr != SCRIPTDATA_COUNT - 1) {
                        compilerv4.processScript(curSub.scriptCodePtr, curSub.jumpTablePtr,
                                                 Compilerv4::EVENT_RSDKLOAD);
                        called = true;
                    }
                    curObj.spriteFrameCount = compilerv4.scriptFrameCount - curObj.frameListOffset;
                    break;
                }
                case EVENT_RSDKDRAW: {
                    auto &curObj = compilerv4.objectScriptList[entities[id].type];

                    if (curObj.eventRSDKDraw.scriptCodePtr != SCRIPTDATA_COUNT - 1
                        && entities[id].type != 0) {
                        compilerv4.objectEntityPos = id;
                        compilerv4.processScript(curObj.eventRSDKDraw.scriptCodePtr,
                                                 curObj.eventRSDKDraw.jumpTablePtr,
                                                 Compilerv4::EVENT_RSDKDRAW);
                        called = true;
                    }
                    break;
                }
                case EVENT_RSDKEDIT: {
                    auto &curObj = compilerv4.objectScriptList[entities[id].type];

                    if (curObj.eventRSDKEdit.scriptCodePtr != SCRIPTDATA_COUNT - 1
                        && entities[id].type != 0) {
                        compilerv4.objectEntityPos = id;
                        activeVarObj               = entities[id].type;

                        compilerv4.processScript(curObj.eventRSDKEdit.scriptCodePtr,
                                                 curObj.eventRSDKEdit.jumpTablePtr,
                                                 Compilerv4::EVENT_RSDKEDIT);
                        called = true;
                    }
                    break;
                }
            }
            break;
    }
    return called;
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

    // screens[0].width        = storedW;
    // screens[0].height       = storedH;
    // screens[0].centerX      = screens[0].width >> 1;
    // screens[0].centerY      = screens[0].height >> 1;
    // screens[0].clipBound_X1 = 0;
    // screens[0].clipBound_Y1 = 0;
    // screens[0].clipBound_X2 = storedW;
    // screens[0].clipBound_Y2 = storedH;
    // screens[0].pitch        = storedW;
    // screens[0].waterDrawPos = storedH;

    refreshScnEditorVerts(storedW >> 2, storedH >> 2);

    prevStoredW = storedW;
    prevStoredH = storedH;
}
