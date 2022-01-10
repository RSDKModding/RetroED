#include "includes.hpp"
#include <cmath>
#include "qgifimage.h"

static QVector3D rectVerticesv5[] = {
    QVector3D(-0.5f, -0.5f, -0.5f), QVector3D(0.5f, -0.5f, -0.5f), QVector3D(0.5f, 0.5f, -0.5f),
    QVector3D(0.5f, 0.5f, -0.5f),   QVector3D(-0.5f, 0.5f, -0.5f), QVector3D(-0.5f, -0.5f, -0.5f),
};

static QVector2D rectTexCoordsv5[] = {
    QVector2D(0.0f, 0.0f), QVector2D(1.0f, 0.0f), QVector2D(1.0f, 1.0f),
    QVector2D(1.0f, 1.0f), QVector2D(0.0f, 1.0f), QVector2D(0.0f, 0.0f),
};

SceneViewerv5::SceneViewerv5(QWidget *)
{
    setMouseTracking(true);

    this->setFocusPolicy(Qt::WheelFocus);

    if (updateTimer)
        delete updateTimer;

    updateTimer = new QTimer(this);
    connect(updateTimer, &QTimer::timeout, this, QOverload<>::of(&SceneViewerv5::updateScene));
    updateTimer->start(1000.0f / 60.0f);

    for (int a = 0; a < v5_SPRFILE_COUNT; ++a) {
        spriteAnimationList[a].scope = SCOPE_NONE;
    }
    for (int s = 0; s < v5_SURFACE_MAX; ++s) {
        gfxSurface[s].scope = SCOPE_NONE;
    }

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
    sceneInfo.debugMode        = true;
    sceneInfo.useGlobalScripts = false;
    sceneInfo.timeEnabled      = false;
    sceneInfo.activeCategory   = 0;
    sceneInfo.categoryCount    = 0;
    sceneInfo.state            = 1;
    sceneInfo.filter           = 0xFF;
    sceneInfo.milliseconds     = 0;
    sceneInfo.seconds          = 0;
    sceneInfo.minutes          = 0;

    sprintf(gameInfo.gameTitle, "%s", "RetroED");
    sprintf(gameInfo.gameSubname, "%s",
            "General Purpose Editor for RSDK Files\n\nCreated by: Rubberduckycooly");
    sprintf(gameInfo.version, "%s", "2.0");

    skuInfo.platform = 0xFF;

    int vID = 0;
    for (int i = 0; i < 0x800; i++) {
        baseIndexList[vID++] = (i << 2) + 0;
        baseIndexList[vID++] = (i << 2) + 1;
        baseIndexList[vID++] = (i << 2) + 2;
        baseIndexList[vID++] = (i << 2) + 1;
        baseIndexList[vID++] = (i << 2) + 3;
        baseIndexList[vID++] = (i << 2) + 2;

        vertexList[i].colour.setX(0);
        vertexList[i].colour.setY(0);
        vertexList[i].colour.setZ(0);
        vertexList[i].colour.setW(0);
    }
}

SceneViewerv5::~SceneViewerv5() { dispose(); }

void SceneViewerv5::startTimer()
{
    if (updateTimer) {
        if (updateTimer->isActive())
            updateTimer->stop();

        updateTimer->start(1000.0f / 60.0f);
    }
}

void SceneViewerv5::dispose()
{
    unloadScene();

    if (updateTimer) {
        disconnect(updateTimer, nullptr, nullptr, nullptr);
        updateTimer->stop();
        delete updateTimer;
    }

    screenVAO.destroy();
    rectVAO.destroy();
}

void SceneViewerv5::loadScene(QString path)
{
    // unloading
    unloadScene();

    QDirIterator it(dataPath + "/../", QStringList() << "*", QDir::Files,
                    QDirIterator::NoIteratorFlags);
    while (it.hasNext()) {
        QString filePath = it.next();

        if (QLibrary::isLibrary(filePath)) {
            GameLink link;
            gameLinks.append(link);
            gameLinks.last().LinkGameObjects(filePath);
        }
    }

    // Default Texture
    if (gfxSurface[0].scope == SCOPE_NONE) {
        gfxSurface[0].scope      = SCOPE_GLOBAL;
        gfxSurface[0].name       = ":/icons/missingV5.png";
        missingObj               = QImage(gfxSurface[0].name);
        gfxSurface[0].texturePtr = createTexture(missingObj);
        Utils::getHashInt(gfxSurface[0].name, gfxSurface[0].hash);
    }

    // loading
    QString pth      = path;
    QString basePath = pth.replace(QFileInfo(pth).fileName(), "");

    currentFolder = QDir(basePath).dirName();

    scene.read(path);

    QString pathTCF = WorkingDirManager::GetPath("Stages/" + currentFolder + "/TileConfig.bin",
                                                 basePath + "TileConfig.bin");
    QString pathSCF = WorkingDirManager::GetPath("Stages/" + currentFolder + "/StageConfig.bin",
                                                 basePath + "StageConfig.bin");

    tileconfig.read(pathTCF);
    stageConfig.read(pathSCF);

    if (scene.editorMetadata.stampName == "")
        scene.editorMetadata.stampName = "StampList.bin";

    QString pathSTP =
        WorkingDirManager::GetPath("Stages/" + currentFolder + "/" + scene.editorMetadata.stampName,
                                   basePath + scene.editorMetadata.stampName);

    if (QFile::exists(pathSTP)) {
        stamps.read(pathSTP);
    }
    else {
        stamps = RSDKv5::Stamps();
    }

    bgColour    = Colour(scene.editorMetadata.backgroundColor1.red(),
                      scene.editorMetadata.backgroundColor1.green(),
                      scene.editorMetadata.backgroundColor1.blue());
    altBGColour = Colour(scene.editorMetadata.backgroundColor2.red(),
                         scene.editorMetadata.backgroundColor2.green(),
                         scene.editorMetadata.backgroundColor2.blue());

    objects.clear();
    entities.clear();

    QList<QString> objNames;
    objNames.append("Blank Object");
    objNames.append("Dev Output");

    if (stageConfig.loadGlobalObjects) {
        for (QString &obj : gameConfig.objects) {
            objNames.append(obj);
        }
    }

    for (QString &obj : stageConfig.objects) {
        objNames.append(obj);
    }

    int type = 0;
    for (RSDKv5::Scene::SceneObject &obj : scene.objects) {
        SceneObject object;
        object.name = obj.name.hashString();

        bool foundName = false;
        for (int i = 0; i < objNames.count(); ++i) {
            if (Utils::getMd5HashByteArray(objNames[i]) == obj.name.hash) {
                object.name = objNames[i];
                foundName   = true;
                break;
            }
        }

        // if we haven't found the name, as a last resort, try looking through any linked objects
        if (!foundName) {
            for (GameLink &link : gameLinks) {
                for (int i = 0; i < link.gameObjectList.count(); ++i) {
                    if (Utils::getMd5HashByteArray(QString(link.gameObjectList[i].name))
                        == obj.name.hash) {
                        object.name = link.gameObjectList[i].name;
                        foundName   = true;
                        break;
                    }
                }
            }
        }

        // Add our variables (names are filled in via SetEditableVar() calls)
        for (int v = 0; v < obj.variables.count(); ++v) {
            auto &var = obj.variables[v];
            VariableInfo variable;
            variable.name = var.name.hashString();
            variable.hash = var.name.hash;
            variable.type = var.type;

            object.variables.append(variable);
        }

        for (RSDKv5::Scene::SceneEntity &ent : obj.entities) {
            SceneEntity entity;
            entity.slotID   = ent.slotID;
            entity.prevSlot = entity.slotID;
            entity.type     = type;
            entity.pos.x    = Utils::fixedToFloat(ent.position.x);
            entity.pos.y    = Utils::fixedToFloat(ent.position.y);

            for (int v = 0; v < ent.variables.count(); ++v) {
                entity.variables.append(ent.variables[v]);
            }

            entities.append(entity);
        }
        objects.append(object);
        ++type;
    }

    bool blankFlag = false;
    for (SceneObject &obj : objects) {
        if (obj.name == "Blank Object") {
            blankFlag = true;
            break;
        }
    }

    // obj type 0 is always blank obj
    if (!blankFlag) {
        SceneObject blankObj;
        blankObj.name = "Blank Object";
        objects.insert(0, blankObj);
        for (SceneEntity &ent : entities) {
            ent.type++;
        }
    }

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

        auto pal = tileset.colorTable();
        tilePalette.clear();
        for (QRgb &col : pal) {
            tilePalette.append(PaletteColour(col));
        }
    }

    disableObjects = false;
}

void SceneViewerv5::saveScene(QString path)
{
    // saving
    QString pth      = path;
    QString basePath = pth.replace(QFileInfo(pth).fileName(), "");

    scene.objects.clear();
    for (SceneObject &obj : objects) {
        RSDKv5::Scene::SceneObject object;
        object.name = RSDKv5::Scene::NameIdentifier(obj.name);
        for (int v = 0; v < obj.variables.count(); ++v) {
            RSDKv5::Scene::VariableInfo variable;
            variable.name = RSDKv5::Scene::NameIdentifier(obj.variables[v].name);
            variable.type = (RSDKv5::VariableTypes)obj.variables[v].type;
            object.variables.append(variable);
        }
        scene.objects.append(object);
    }

    for (SceneEntity &entity : entities) {
        if (entity.type >= 0 && entity.type < objects.count()) {
            RSDKv5::Scene::SceneEntity ent;
            ent.slotID     = entity.slotID;
            ent.position.x = entity.pos.x * 65536.0f;
            ent.position.y = entity.pos.y * 65536.0f;

            ent.variables.clear();
            int v = 0;
            for (; v < entity.variables.count(); ++v) {
                ent.variables.append(entity.variables[v]);
            }

            ent.parent = &scene.objects[entity.type];
            scene.objects[entity.type].entities.append(ent);
        }
        else {
            // what happened here???
        }
    }

    FormatHelpers::Gif tileset(16, 0x400 * 16);

    int c = 0;
    for (PaletteColour &col : tilePalette) tileset.palette[c++] = col.toQColor();

    int pos = 0;
    for (int i = 0; i < 0x400; ++i) {
        uchar *src = tiles[i].bits();
        for (int y = 0; y < 16; ++y) {
            for (int x = 0; x < 16; ++x) tileset.pixels[pos++] = *src++;
        }
    }

    scene.write(path);

    tileconfig.write(basePath + "TileConfig.bin");
    stageConfig.write(basePath + "StageConfig.bin");
    stamps.write(basePath + scene.editorMetadata.stampName);
    tileset.write(basePath + "16x16Tiles.gif");
}

void SceneViewerv5::updateScene()
{
    this->repaint();

    if (statusLabel) {
        int mx = (int)((mousePos.x * invZoom()) + cam.pos.x);
        int my = (int)((mousePos.y * invZoom()) + cam.pos.y);
        statusLabel->setText(
            QString("Zoom: %1%, Mouse Position: (%2, %3), Tile Position: (%4, %5), Selected Tile: "
                    "%6, Selected Layer: %7 (%8), Selected Object: %9")
                .arg(zoom * 100)
                .arg(mx)
                .arg(my)
                .arg((int)mx / 0x10)
                .arg((int)my / 0x10)
                .arg(selectedTile)
                .arg(selectedLayer)
                .arg(selectedLayer >= 0 && selectedLayer < scene.layers.count()
                         ? scene.layers[selectedLayer].name
                         : "")
                .arg(selectedObject >= 0 && selectedObject < objects.count()
                         ? objects[selectedObject].name
                         : ""));
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
                QString("Tile Layer Count: %1 Layers").arg(scene.layers.count()));
            v5Editor->scnProp->objCnt->setText(
                QString("Object Count: %1 Objects").arg(objects.count()));
            v5Editor->scnProp->entCntTot->setText(
                QString("Total Entity Count: %1 Entities").arg(entities.count()));
            v5Editor->scnProp->entCntAct->setText(
                QString("Active Entity Count: %1 Entities").arg(activeCount));
            v5Editor->scnProp->entCntInac->setText(
                QString("Inactive Layer Count: %1 Entities").arg(inactiveCount));
        }
    }
}

void SceneViewerv5::cleanCol(int x, int y, int w, int h)
{
    int ty = y + h;
    int tx = x + w;
    for (; x < tx; ++x) {
        for (; y < ty; ++y) {
            colTex->setPixel(x, y, 0);
        }
        y -= h;
    }
}

void SceneViewerv5::placeCol(int x, int y, sbyte h, int sol, int w)
{
    // rmg code opts at its finest
    if (h < 0) {
        y += h;
        h *= -1;
    }
    int ty = y + h;
    int tx = x + w;
    for (; x < tx; ++x) {
        for (; y < ty; ++y) {
            QRgb c = colTex->pixel(x, y);
            if (qBlue(c))
                continue;
            if (qRed(c) && ((sol == 1 && !qGreen(c)) || (sol == 2 && qGreen(c))))
                colTex->setPixel(x, y, 3);
            else
                colTex->setPixel(x, y, sol);
        }
        y -= h;
    }
}

void SceneViewerv5::drawScene()
{
    if (!tilesetTexture)
        return;
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    renderCount      = 0;
    renderStateIndex = -1;
    renderStates.clear();

    f->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    f->glBlendEquation(GL_FUNC_ADD);

    int sceneW = 0;
    int sceneH = 0;

    for (int i = 0; i < scene.layers.count(); ++i) {
        if (scene.layers[i].width > sceneW)
            sceneW = scene.layers[i].width;
        if (scene.layers[i].height > sceneH)
            sceneH = scene.layers[i].height;
    }

    if (sceneW != sceneWidth || sceneH != sceneHeight) {
        sceneWidth  = sceneW;
        sceneHeight = sceneH;
    }

    // pre-render
    if ((cam.pos.x * zoom) + storedW > (sceneWidth * 0x10) * zoom)
        cam.pos.x = ((sceneWidth * 0x10) - (storedW * invZoom()));

    if ((cam.pos.y * zoom) + storedH > (sceneHeight * 0x10) * zoom)
        cam.pos.y = ((sceneHeight * 0x10) - (storedH * invZoom()));

    if ((cam.pos.x * zoom) < -64)
        cam.pos.x = -64;

    if ((cam.pos.y * zoom) < -64)
        cam.pos.y = -64;

    screens[0].position.x = cam.pos.x;
    screens[0].position.y = cam.pos.y;

    // draw bg colours
    primitiveShader.use();
    primitiveShader.setValue("colour", QVector4D(altBGColour.r / 255.0f, altBGColour.g / 255.0f,
                                                 altBGColour.b / 255.0f, 1.0f));
    primitiveShader.setValue("projection", getProjectionMatrix());
    primitiveShader.setValue("view", QMatrix4x4());
    primitiveShader.setValue("useAlpha", false);
    primitiveShader.setValue("alpha", 1.0f);
    rectVAO.bind();

    float bgOffsetY = 0x80;
    bgOffsetY -= fmod(cam.pos.y, 0x200);
    for (float y = bgOffsetY; y < (storedH + 0x80) * (zoom < 1.0f ? invZoom() : 1.0f); y += 0x100) {
        float bgOffsetX = ((fmod((y - bgOffsetY), 0x200) == 0) ? 0x100 : 0x00);
        bgOffsetX += 0x80;
        bgOffsetX -= fmod(cam.pos.x, 0x200);
        for (float x = bgOffsetX; x < (storedW + 0x80) * (zoom < 1.0f ? invZoom() : 1.0f); x += 0x200) {
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
    spriteShader.setValue("transparentColour", QVector3D(1.0f, 0.0f, 1.0f));

    QMatrix4x4 matWorld;
    QMatrix4x4 matView;
    spriteShader.setValue("projection", matWorld = getProjectionMatrix());
    spriteShader.setValue("view", matView = QMatrix4x4());
    f->glActiveTexture(GL_TEXTURE0);

    rectVAO.bind();

    Vector3<float> camOffset(0.0f, 0.0f, 0.0f);

    bool showCLayers[2] = { showPlaneA, showPlaneB };

    for (int l = 0; l < v5_DRAWLAYER_COUNT; ++l) {
        drawLayers[l].layerDrawList.clear();
    }

    for (int t = 0; t < scene.layers.count(); ++t) {
        byte drawOrder = scene.layers[t].drawOrder;
        if (drawOrder < v5_DRAWLAYER_COUNT)
            drawLayers[drawOrder].layerDrawList.append(t);
    }

    for (int p = 0; p < v5_DRAWLAYER_COUNT; ++p) {
        for (int l = scene.layers.count() - 1; l >= 0; --l) {
            if (scene.layers[l].drawOrder != p && !(p == 15 && scene.layers[l].drawOrder >= 16)) {
                if (!(selectedLayer == l && p == 15))
                    continue;
            }
            if (!scene.layers[l].visible)
                continue;

            // TILE LAYERS
            QVector<QVector<ushort>> layout = scene.layers[l].layout;
            int width                       = scene.layers[l].width;
            int height                      = scene.layers[l].height;

            spriteShader.use();
            spriteShader.setValue("useAlpha", false);
            spriteShader.setValue("alpha", 1.0f);
            spriteShader.setValue("transparentColour", QVector3D(1.0f, 0.0f, 1.0f));
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

            int basedX = qMax(camX / 0x10, 0);
            int basedY = qMax(camY / 0x10, 0);

            int sw = (storedW * invZoom());
            int sh = (storedH * invZoom());

            int countX = width * 0x10 > sw ? (sw / 0x10) : width;
            int countY = height * 0x10 > sh ? (sh / 0x10) : height;

            countX = qMin(basedX + countX + 2, width);
            countY = qMin(basedY + countY + 2, height);

            for (int y = basedY; y < countY; ++y) {
                for (int x = basedX; x < countX; ++x) {
                    ushort tile = layout[y][x];
                    if (tile != 0xFFFF) {
                        float xpos = (x * 0x10) - (cam.pos.x + camOffset.x);
                        float ypos = (y * 0x10) - (cam.pos.y + camOffset.y);
                        float zpos = selectedLayer == l ? 15.2 : (p + (l * 0.01));

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
                        vertsPtr[vertCnt + 56].setZ(zpos);

                        byte direction = Utils::getBit(tile, 10) | (Utils::getBit(tile, 11) << 1);
                        getTileVerts(tVertsPtr, vertCnt, (tile & 0x3FF) * 0x10, direction);
                        vertCnt += 6;
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

            vertCnt = 0;

            // welcome to the magic of rmg code. dangerous-looking code ahead
            // there's definitely better ways to do this, but for now this is what we gotta do

            // Collision Previews

            spriteShader.setValue("transparentColour", QVector3D(0.0f, 0.0f, 0.0f));

            for (int c = 0; c < 2 && l == selectedLayer; ++c) {
                if (showCLayers[c]) {
                    cleanCol(basedX * 16, basedY * 16, (countX - basedX) * 16, (countY - basedY) * 16);
                    for (int y = countY - 1; y >= basedY; --y) {
                        for (int x = basedX; x < countX; ++x) {
                            ushort tile = layout[y][x];
                            if (tile != 0xFFFF) {
                                // draw pixel collision
                                float xpos = (x * 0x10);
                                float ypos = (y * 0x10);

                                byte solidity = 0;
                                RSDKv5::TileConfig::CollisionMask &cmask =
                                    tileconfig.collisionPaths[c][tile & 0x3FF];

                                solidity = !c ? (tile >> 12) & 3 : (tile >> 14) & 3;

                                if (!solidity)
                                    continue;

                                bool flipX = (tile >> 10) & 1;
                                bool flipY = (tile >> 11) & 1;

                                byte flipmode = flipY | (cmask.flipY << 1);

                                for (byte cx = 0; cx < 16; ++cx) {
                                    int hm = cx;
                                    if (flipX)
                                        hm = 15 - cx;

                                    if (!cmask.collision[hm].solid)
                                        continue;

                                    byte h  = cmask.collision[hm].height;
                                    byte cy = h;
                                    if (solidity != 2) {
                                        // yellow
                                        byte ycy = cy;

                                        if (flipmode) {
                                            if (flipmode <= 2)
                                                ycy = 0;
                                            if (flipmode == 3)
                                                ycy = 15 - h;
                                        }

                                        placeCol(xpos + cx, ypos + ycy, 16, 1);
                                    }
                                    if (solidity != 1) {
                                        // red
                                        byte rcy = h, rch = 16 - h;
                                        if (flipmode) {
                                            if (flipmode <= 2)
                                                rcy = 0;
                                            if (flipmode >= 2) {
                                                rch = h + 1;
                                                if (flipmode == 3) {
                                                    rcy = 15 - h;
                                                }
                                            }
                                        }

                                        rcy = rcy + rch;

                                        placeCol(xpos + cx, ypos + rcy, -16, 2);
                                    }
                                } //*/
                                if (solidity == 3) {
                                    cleanCol(xpos, ypos, 16, 16);
                                    for (byte cx = 0; cx < 16; ++cx) {
                                        int hm = cx;
                                        if (flipX)
                                            hm = 15 - cx;

                                        if (!cmask.collision[hm].solid)
                                            continue;

                                        byte h  = cmask.collision[hm].height;
                                        byte cy = h, ch = 16 - h;
                                        if (flipmode) {
                                            if (flipmode <= 2)
                                                cy = 0;
                                            if (flipmode >= 2) {
                                                ch = h + 1;
                                                if (flipmode == 3) {
                                                    cy = 15 - h;
                                                }
                                            }
                                        }
                                        placeCol(xpos + cx, ypos + cy, ch, solidity);
                                    }
                                }
                            }
                        }
                    }

                    float x = cam.pos.x + camOffset.x;
                    float y = cam.pos.y + camOffset.y;

                    auto tileGL = createTexture(colTex->copy(x, y, storedW / zoom, storedH / zoom));

                    tileGL->bind();

                    float w = storedW / 16.0f / zoom;
                    float h = storedH / 16.0f / zoom;

                    vertsPtr[0].setX(0);
                    vertsPtr[0].setY(0);

                    vertsPtr[1].setX(w);
                    vertsPtr[1].setY(0);

                    vertsPtr[2].setX(w);
                    vertsPtr[2].setY(h);

                    vertsPtr[3].setX(w);
                    vertsPtr[3].setY(h);

                    vertsPtr[4].setX(0);
                    vertsPtr[4].setY(h);

                    vertsPtr[5].setX(0);
                    vertsPtr[5].setY(0);

                    tVertsPtr[0].setX(0);
                    tVertsPtr[0].setY(0);

                    tVertsPtr[1].setX(1);
                    tVertsPtr[1].setY(0);

                    tVertsPtr[2].setX(1);
                    tVertsPtr[2].setY(1);

                    tVertsPtr[3].setX(1);
                    tVertsPtr[3].setY(1);

                    tVertsPtr[4].setX(0);
                    tVertsPtr[4].setY(1);

                    tVertsPtr[5].setX(0);
                    tVertsPtr[5].setY(0);

                    for (int i = 0; i < 6; ++i) {
                        vertsPtr[i].setZ(15.51);
                    }

                    {
                        QOpenGLVertexArrayObject vao;
                        vao.create();
                        vao.bind();

                        QOpenGLBuffer vVBO2D;
                        vVBO2D.create();
                        vVBO2D.setUsagePattern(QOpenGLBuffer::StaticDraw);
                        vVBO2D.bind();
                        vVBO2D.allocate(vertsPtr, 6 * sizeof(QVector3D));
                        spriteShader.enableAttributeArray(0);
                        spriteShader.setAttributeBuffer(0, GL_FLOAT, 0, 3, 0); //*/

                        QOpenGLBuffer tVBO2D;
                        tVBO2D.create();
                        tVBO2D.setUsagePattern(QOpenGLBuffer::StaticDraw);
                        tVBO2D.bind();
                        tVBO2D.allocate(tVertsPtr, 6 * sizeof(QVector2D));
                        spriteShader.enableAttributeArray(1);
                        spriteShader.setAttributeBuffer(1, GL_FLOAT, 0, 2, 0); //*/

                        QMatrix4x4 matModel;
                        matModel.scale(0x10 * zoom, 0x10 * zoom, 1.0f);
                        spriteShader.setValue("model", matModel);

                        f->glDrawArrays(GL_TRIANGLES, 0, 6);

                        vao.release();
                        tVBO2D.release();
                        vVBO2D.release();
                    }
                    tileGL->release();
                }
            }

            spriteShader.setValue("transparentColour", QVector3D(1.0f, 0.0f, 1.0f));

            // PARALLAX
            if (l == selectedLayer && l >= 0) {
                if (scene.layers[l].type == 0 || scene.layers[l].type == 1) {
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
                        for (RSDKv5::Scene::ScrollIndexInfo &info : scene.layers[l].scrollInfos) {
                            bool isSelected = selectedScrollInfo == id;

                            Vector4<float> clr(1.0f, 1.0f, 0.0f, 1.0f);
                            if (isSelected)
                                clr = Vector4<float>(0.0f, 0.0f, 1.0f, 1.0f);
                            float zpos = (isSelected ? 15.55f : 15.5f);

                            if (scene.layers[l].type == 0) {
                                int w = (width * 0x10);
                                drawLine(0.0f * zoom, (info.startLine - cam.pos.y) * zoom, zpos,
                                         (w - cam.pos.x) * zoom, (info.startLine - cam.pos.y) * zoom,
                                         zpos, 1.0f, clr, primitiveShader);

                                drawLine(0.0f * zoom,
                                         ((info.startLine + info.length) - cam.pos.y) * zoom, zpos,
                                         (w - cam.pos.x) * zoom,
                                         ((info.startLine + info.length) - cam.pos.y) * zoom, zpos,
                                         1.0f, clr, primitiveShader);
                            }
                            else if (scene.layers[l].type == 1) {
                                int h = (height * 0x10);
                                drawLine((info.startLine - cam.pos.x) * zoom, 0.0f * zoom, zpos,
                                         (info.startLine - cam.pos.x) * zoom, (h - cam.pos.y) * zoom,
                                         zpos, 1.0f, clr, primitiveShader);

                                drawLine(((info.startLine + info.length) - cam.pos.x) * zoom,
                                         0.0f * zoom, zpos,
                                         ((info.startLine + info.length) - cam.pos.x) * zoom,
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

        primitiveShader.use();
        primitiveShader.setValue("projection", getProjectionMatrix());
        primitiveShader.setValue("view", QMatrix4x4());
        primitiveShader.setValue("useAlpha", false);
        primitiveShader.setValue("alpha", 1.0f);

        spriteShader.use();
        rectVAO.bind();
        spriteShader.setValue("flipX", false);
        spriteShader.setValue("flipY", false);
        spriteShader.setValue("useAlpha", false);
        spriteShader.setValue("alpha", 1.0f);

        currZ                      = curTool == TOOL_ENTITY ? 15.75 : p;
        sceneInfo.currentDrawGroup = p;
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

            if (entity->gameEntity) {
                entity->gameEntity->position.x = Utils::floatToFixed(entity->pos.x);
                entity->gameEntity->position.y = Utils::floatToFixed(entity->pos.y);
            }

            if (entity->type != 0)
                callGameEvent(GetObjectInfo(objects[entity->type].name), EVENT_DRAW, entity);

            // Draw Default Object Sprite if invalid
            // TODO: probably draw text intead
            if (!validDraw) {
                entity->box = Rect<int>(-0x10, -0x10, 0x10, 0x10);
                spriteShader.use();
                rectVAO.bind();

                float xpos = entity->pos.x - (cam.pos.x);
                float ypos = entity->pos.y - (cam.pos.y);
                float zpos = incZ();

                int w = gfxSurface[0].texturePtr->width(), h = gfxSurface[0].texturePtr->height();
                if (prevSprite) {
                    gfxSurface[0].texturePtr->bind();
                    spriteShader.setValue("transparentColour", QVector3D(1.0f, 0.0f, 1.0f));
                    spriteShader.setValue("useAlpha", true);
                    spriteShader.setValue("alpha", 1.0f);
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
        }
    }

    // TILE PREVIEW
    spriteShader.use();
    rectVAO.bind();
    spriteShader.setValue("useAlpha", true);
    spriteShader.setValue("alpha", 0.75f);
    if (selectedTile >= 0 && selectedLayer >= 0 && isSelecting && curTool == TOOL_PENCIL) {
        tilesetTexture->bind();
        float tx = tilePos.x;
        float ty = tilePos.y;

        tx *= invZoom();
        ty *= invZoom();

        float tx2 = tx + fmodf(cam.pos.x, 0x10);
        float ty2 = ty + fmodf(cam.pos.y, 0x10);

        // clip to grid
        tx -= fmodf(tx2, 0x10);
        ty -= fmodf(ty2, 0x10);

        // Draw Selected Tile Preview
        float xpos = tx + cam.pos.x;
        float ypos = ty + cam.pos.y;
        float zpos = 15.0f;

        xpos -= (cam.pos.x + camOffset.x);
        ypos -= (cam.pos.y + camOffset.y);

        byte direction = (int)(tileFlip.x) | ((int)(tileFlip.y) << 1);
        drawTile(xpos, ypos, zpos, 0, selectedTile * 0x10, direction);
    }

    // ENT PREVIEW
    spriteShader.use();
    rectVAO.bind();
    spriteShader.setValue("flipX", false);
    spriteShader.setValue("flipY", false);
    spriteShader.setValue("useAlpha", true);
    spriteShader.setValue("alpha", 0.75f);
    if (selectedObject >= 0 && isSelecting && curTool == TOOL_ENTITY) {
        float ex = tilePos.x;
        float ey = tilePos.y;

        ex *= invZoom();
        ey *= invZoom();

        validDraw                   = false;
        createGameEntity.position.x = (ex + cam.pos.x) * 65536.0f;
        createGameEntity.position.y = (ey + cam.pos.y) * 65536.0f;
        createGameEntity.objectID   = selectedObject;

        createTempEntity.type       = selectedObject;
        createTempEntity.pos.x      = (ex + cam.pos.x) * 65536.0f;
        createTempEntity.pos.y      = (ey + cam.pos.y) * 65536.0f;
        createTempEntity.slotID     = 0xFFFF;
        createTempEntity.gameEntity = &createGameEntity;
        createTempEntity.box        = Rect<int>(0, 0, 0, 0);

        activeDrawEntity = &createTempEntity;
        if (selectedObject != 0) {
            callGameEvent(GetObjectInfo(objects[selectedObject].name), EVENT_DRAW, &createTempEntity);
        }

        if (!validDraw) {
            // Draw Selected Object Preview
            float xpos = ex;
            float ypos = ey;
            float zpos = 15.0f;

            int w = gfxSurface[0].texturePtr->width(), h = gfxSurface[0].texturePtr->height();
            gfxSurface[0].texturePtr->bind();

            QMatrix4x4 matModel;
            matModel.scale(w * zoom, h * zoom, 1.0f);

            matModel.translate(xpos / (float)w, ypos / (float)h, zpos);
            spriteShader.setValue("model", matModel);

            f->glDrawArrays(GL_TRIANGLES, 0, 6);
        }
    }

    spriteShader.setValue("useAlpha", false);
    spriteShader.setValue("alpha", 1.0f);

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
        SceneEntity &entity = entities[selectedEntity];

        float left   = entity.pos.x + entity.box.x;
        float top    = entity.pos.y + entity.box.y;
        float right  = entity.pos.x + entity.box.w;
        float bottom = entity.pos.y + entity.box.h;

        float w = fabsf(right - left), h = fabsf(bottom - top);
        gfxSurface[0].texturePtr->bind();

        drawRect(left - cam.pos.x, top - cam.pos.y, 15.7f, w, h, Vector4<float>(1.0f, 1.0f, 1.0f, 1.0f),
                 primitiveShader, true);
    }

    // Selected Stamp Box
    rectVAO.bind();
    primitiveShader.use();
    primitiveShader.setValue("projection", getProjectionMatrix());
    primitiveShader.setValue("view", QMatrix4x4());
    primitiveShader.setValue("useAlpha", false);
    primitiveShader.setValue("alpha", 1.0f);
    primitiveShader.setValue("model", matModel);
    if (selectedStamp >= 0) {
        RSDKv5::Stamps::StampEntry &stamp = stamps.stampList[selectedStamp];

        float left   = stamp.pos.x - (stamp.size.x / 2.0f);
        float top    = stamp.pos.y - (stamp.size.y / 2.0f);
        float right  = stamp.pos.x + (stamp.size.x / 2.0f);
        float bottom = stamp.pos.y + (stamp.size.y / 2.0f);

        float w = fabsf(right - left), h = fabsf(bottom - top);
        gfxSurface[0].texturePtr->bind();

        drawRect(left - cam.pos.x, top - cam.pos.y, 15.7f, w, h, Vector4<float>(1.0f, 0.0f, 0.0f, 1.0f),
                 primitiveShader, true);
    }

    // Selection Box
    rectVAO.bind();
    primitiveShader.use();
    primitiveShader.setValue("projection", getProjectionMatrix());
    primitiveShader.setValue("view", QMatrix4x4());
    primitiveShader.setValue("useAlpha", false);
    primitiveShader.setValue("alpha", 1.0f);
    primitiveShader.setValue("model", matModel);
    if (curTool == TOOL_SELECT && (isSelecting || (selectSize.x && selectSize.y))) {
        gfxSurface[0].texturePtr->bind();

        drawRect(selectPos.x - cam.pos.x, selectPos.y - cam.pos.y, 15.7f, selectSize.x, selectSize.y,
                 Vector4<float>(0.0f, 0.0f, 1.0f, 0.5f), primitiveShader, true);

        for (auto &id : selectedEntities) {
            SceneEntity &entity = entities[id];

            float left   = entity.pos.x + entity.box.x;
            float top    = entity.pos.y + entity.box.y;
            float right  = entity.pos.x + entity.box.w;
            float bottom = entity.pos.y + entity.box.h;

            float w = fabsf(right - left), h = fabsf(bottom - top);
            gfxSurface[0].texturePtr->bind();

            drawRect(left - cam.pos.x, top - cam.pos.y, 15.7f, w, h,
                     Vector4<float>(1.0f, 1.0f, 1.0f, 1.0f), primitiveShader, true);
        }
    }

    renderRenderStates();
}

void SceneViewerv5::unloadScene()
{
    disableObjects = true;

    for (int l = 0; l < gameLinks.count(); ++l) {
        gameLinks[l].unload();
    }
    gameLinks.clear();

    // QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    if (tilesetTexture) {
        tilesetTexture->destroy();
        delete tilesetTexture;
    }
    tilesetTexture = nullptr;
    tiles.clear();

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

    cam                = SceneCamerav5();
    selectedTile       = -1;
    selectedEntity     = -1;
    selectedLayer      = -1;
    selectedScrollInfo = -1;
    selectedObject     = -1;
    isSelecting        = false;

    memset(gameEntityList, 0, sizeof(gameEntityList));

    if (vertsPtr)
        delete[] vertsPtr;
    if (tVertsPtr)
        delete[] tVertsPtr;
    vertsPtr    = nullptr;
    tVertsPtr   = nullptr;
    sceneWidth  = 0;
    sceneHeight = 0;
    prevStoredW = -1;
    prevStoredH = -1;
}

void SceneViewerv5::processObjects()
{
    for (int i = 0; i < v5_DRAWLAYER_COUNT; ++i) {
        drawLayers[i].entries.clear();
    }

    sceneInfo.entitySlot = 0;
    int centerX          = screens->centerX * invZoom();
    int centerY          = screens->centerY * invZoom();
    for (int e = 0; e < entities.count(); ++e) {
        if (!entities[e].gameEntity || !entities[e].type) {
            drawLayers[15].entries.append(sceneInfo.entitySlot++);
            continue;
        }

        sceneInfo.entity = entities[e].gameEntity;
        if (entities[e].type) {
            sceneInfo.entity->inBounds = false;

            int rangeX = sceneInfo.entity->updateRange.x * invZoom();
            int rangeY = sceneInfo.entity->updateRange.y * invZoom();

            switch (sceneInfo.entity->active) {
                case ACTIVE_NEVER:
                case ACTIVE_PAUSED:
                case ACTIVE_ALWAYS:
                case ACTIVE_NORMAL: sceneInfo.entity->inBounds = true; break;
                case ACTIVE_BOUNDS: {
                    int sx =
                        abs(sceneInfo.entity->position.x - ((screens->position.x + centerX) << 16));
                    int sy =
                        abs(sceneInfo.entity->position.y - ((screens->position.y + centerY) << 16));
                    if (sx <= rangeX + (centerX << 16) && sy <= rangeY + (centerY << 16)) {
                        sceneInfo.entity->inBounds = true;
                    }
                    break;
                }
                case ACTIVE_XBOUNDS: {
                    int sx =
                        abs(sceneInfo.entity->position.x - ((screens->position.x + centerX) << 16));
                    if (sx <= rangeX + (centerX << 16)) {
                        sceneInfo.entity->inBounds = true;
                        break;
                    }
                    break;
                }
                case ACTIVE_YBOUNDS: {
                    int sy =
                        abs(sceneInfo.entity->position.y - ((screens->position.y + centerY) << 16));
                    if (sy <= rangeY + (centerY << 16)) {
                        sceneInfo.entity->inBounds = true;
                        break;
                    }
                    break;
                }
                case ACTIVE_RBOUNDS: {
                    int sx = abs(sceneInfo.entity->position.x - ((screens->position.x + centerX) << 16))
                             >> 16;
                    int sy = abs(sceneInfo.entity->position.y - ((screens->position.y + centerY) << 16))
                             >> 16;
                    if (sx * sx + sy * sy <= rangeX + (centerX << 16)) {
                        sceneInfo.entity->inBounds = true;
                        break;
                    }
                    break;
                }
            }

            if (sceneInfo.entity->inBounds) {
                // callGameEvent(gameLink.GetObjectInfo(objects[entities[e].type].name), EVENT_UPDATE,
                //              &entities[e]);

                if (sceneInfo.entity->drawOrder < v5_DRAWLAYER_COUNT)
                    drawLayers[sceneInfo.entity->drawOrder].entries.append(sceneInfo.entitySlot);
            }
        }
        else {
            sceneInfo.entity->inBounds = false;
        }
        sceneInfo.entitySlot++;
    }

    for (int i = 0; i < TYPEGROUP_COUNT; ++i) {
        typeGroups[i].entries.clear();
    }

    sceneInfo.entitySlot = 0;
    for (int e = 0; e < entities.count(); ++e) {
        sceneInfo.entity = entities[e].gameEntity;
        if (sceneInfo.entity && sceneInfo.entity->inBounds && sceneInfo.entity->interaction) {
            typeGroups[0].entries.append(e);                          // All active objects
            typeGroups[sceneInfo.entity->objectID].entries.append(e); // type-based slots
            if (sceneInfo.entity->group >= TYPE_COUNT) {
                typeGroups[sceneInfo.entity->group].entries.append(e); // extra slots
            }
        }
        sceneInfo.entitySlot++;
    }

    sceneInfo.entitySlot = 0;
    for (int e = 0; e < entities.count(); ++e) {
        if (entities[e].gameEntity)
            entities[e].gameEntity->activeScreens = 0;
    }
}

void SceneViewerv5::callGameEvent(GameObjectInfo *info, byte eventID, SceneEntity *entity)
{
    if (!info || (!entity && (eventID != EVENT_LOAD && eventID != EVENT_SERIALIZE)))
        return;

    foreachStackPtr = foreachStackList;
    switch (eventID) {
        default: break;
        case EVENT_LOAD:
            if (info->editorLoad)
                info->editorLoad();
            break;
        case EVENT_CREATE: {
            GameEntity *entityPtr =
                entity->slotID == 0xFFFF ? entity->gameEntity : &gameEntityList[entity->slotID];
            memset(entityPtr, 0, sizeof(GameEntityBase));
            entityPtr->position.x    = Utils::floatToFixed(entity->pos.x);
            entityPtr->position.y    = Utils::floatToFixed(entity->pos.y);
            entityPtr->interaction   = true;
            entityPtr->active        = ACTIVE_BOUNDS;
            entityPtr->updateRange.x = 0x800000;
            entityPtr->updateRange.y = 0x800000;
            entityPtr->scale.x       = 0x200;
            entityPtr->scale.y       = 0x200;
            entityPtr->objectID      = entity->type;

            for (int o = 0; o < entity->variables.length(); o++) {
                auto var    = objects[entity->type].variables[o];
                auto val    = entity->variables[o];
                auto offset = &((byte *)entityPtr)[var.offset];
                switch (var.type) {
                    case VAR_UINT8: memcpy(offset, &val.value_uint8, sizeof(byte)); break;
                    case VAR_INT8: memcpy(offset, &val.value_int8, sizeof(sbyte)); break;
                    case VAR_UINT16: memcpy(offset, &val.value_uint16, sizeof(ushort)); break;
                    case VAR_INT16: memcpy(offset, &val.value_int16, sizeof(short)); break;
                    case VAR_UINT32: memcpy(offset, &val.value_uint32, sizeof(uint)); break;
                    case VAR_INT32: memcpy(offset, &val.value_int32, sizeof(int)); break;
                    case VAR_ENUM: memcpy(offset, &val.value_enum, sizeof(int)); break;
                    case VAR_STRING: {
                        FunctionTable::setText((TextInfo *)offset,
                                               (char *)val.value_string.toStdString().c_str(), false);
                        break;
                    }
                    // i'm cheating w this 1
                    case VAR_VECTOR2: memcpy(offset, &val.value_vector2.x, sizeof(Vector2<int>)); break;
                    case VAR_UNKNOWN: // :urarakaconfuse:
                        memcpy(offset, &val.value_unknown, sizeof(int));
                        break;
                    case VAR_BOOL: {
                        bool32 value = val.value_bool;
                        memcpy(offset, &value, sizeof(bool32));
                        break;
                    }
                    case VAR_COLOUR: {
                        auto c   = val.value_color;
                        uint clr = (c.red() << 16) | (c.green() << 8) | (c.blue());
                        memcpy(offset, &clr, sizeof(uint));
                        break;
                    }
                }
            }
            entity->gameEntity = entityPtr;

            sceneInfo.entity     = entity->gameEntity;
            sceneInfo.entitySlot = entity->slotID;
            if (info->create && entity->gameEntity)
                info->create(NULL);
            sceneInfo.entity     = NULL;
            sceneInfo.entitySlot = 0;

            // editor defaults!
            for (int o = 0; o < entity->variables.length(); o++) {
                auto var    = objects[entity->type].variables[o];
                auto &val   = entity->variables[o];
                auto offset = &((byte *)entityPtr)[var.offset];
                switch (var.type) {
                    case VAR_UINT8: memcpy(&val.value_uint8, offset, sizeof(byte)); break;
                    case VAR_INT8: memcpy(&val.value_int8, offset, sizeof(sbyte)); break;
                    case VAR_UINT16: memcpy(&val.value_uint16, offset, sizeof(ushort)); break;
                    case VAR_INT16: memcpy(&val.value_int16, offset, sizeof(short)); break;
                    case VAR_UINT32: memcpy(&val.value_uint32, offset, sizeof(uint)); break;
                    case VAR_INT32: memcpy(&val.value_int32, offset, sizeof(int)); break;
                    case VAR_ENUM: memcpy(&val.value_enum, offset, sizeof(int)); break;
                    case VAR_STRING: {
                        char buffer[0x100];
                        FunctionTable::getCString(buffer, (TextInfo *)offset);
                        val.value_string = buffer;
                        break;
                    }
                    case VAR_VECTOR2:
                        memcpy(&val.value_vector2.x, offset, sizeof(Vector2<int>));
                        val.value_vector2f.x = Utils::fixedToFloat(val.value_vector2.x);
                        val.value_vector2f.y = Utils::fixedToFloat(val.value_vector2.y);
                        break;
                    case VAR_UNKNOWN: // :urarakaconfuse:
                        memcpy(&val.value_unknown, offset, sizeof(int));
                        break;
                    case VAR_BOOL: {
                        bool32 value = false;
                        memcpy(&value, offset, sizeof(bool32));
                        val.value_bool = value;
                        break;
                    }
                    case VAR_COLOUR: {
                        auto c   = val.value_color;
                        uint clr = 0;
                        memcpy(&clr, offset, sizeof(uint));
                        val.value_color = QColor(clr);
                        break;
                    }
                }
            }
            break;
        }
        case EVENT_UPDATE:
            // TODO: that(?)
            sceneInfo.entity     = entity->gameEntity;
            sceneInfo.entitySlot = entity->slotID;
            if (info->update && entity->gameEntity)
                info->update();
            sceneInfo.entity     = NULL;
            sceneInfo.entitySlot = 0;
            break;
        case EVENT_DRAW:
            sceneInfo.currentScreenID  = 0;
            sceneInfo.currentDrawGroup = 0; // TODO

            sceneInfo.entity     = entity->gameEntity;
            sceneInfo.entitySlot = entity->slotID;
            if (info->editorDraw && entity->gameEntity)
                info->editorDraw();
            sceneInfo.entity     = NULL;
            sceneInfo.entitySlot = 0;
            break;
        case EVENT_SERIALIZE:
            if (info->serialize)
                info->serialize();
            break;
    }
}

void SceneViewerv5::initializeGL()
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
    f->glEnable(GL_MULTISAMPLE);
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
    vVBO2D.allocate(rectVerticesv5, 6 * sizeof(QVector3D));
    spriteShader.enableAttributeArray(0);
    spriteShader.setAttributeBuffer(0, GL_FLOAT, 0, 3, 0);

    QOpenGLBuffer tVBO2D;
    tVBO2D.create();
    tVBO2D.setUsagePattern(QOpenGLBuffer::StaticDraw);
    tVBO2D.bind();
    tVBO2D.allocate(rectTexCoordsv5, 6 * sizeof(QVector2D));
    spriteShader.enableAttributeArray(1);
    spriteShader.setAttributeBuffer(1, GL_FLOAT, 0, 2, 0);

    QOpenGLBuffer gameVBO;
    gameVBO.create();
    gameVBO.setUsagePattern(QOpenGLBuffer::DynamicDraw);
    gameVBO.bind();
    gameVBO.allocate(vertexList, 0x800 * sizeof(DrawVertex));

    VBO.create();
    VBO.setUsagePattern(QOpenGLBuffer::DynamicDraw);
    VBO.bind();
    VBO.allocate(vertexList, 0x800 * sizeof(DrawVertex));

    f->glGenBuffers(1, &indexVBO);

    // Release (unbind) all
    rectVAO.release();
    // gameVAO.release();
    vVBO2D.release();
}

void SceneViewerv5::resizeGL(int w, int h)
{
    storedW = w;
    storedH = h;
    refreshResize();

    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glViewport(0, 0, w, h);
}

void SceneViewerv5::paintGL()
{
    // Draw the scene:
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    f->glClearColor(bgColour.r / 255.0f, bgColour.g / 255.0f, bgColour.b / 255.0f, 1.0f);
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (!disableObjects) {
        processObjects();

        if (vertsPtr && tVertsPtr)
            drawScene();
    }
}

int SceneViewerv5::addGraphicsFile(QString sheetPath, int sheetID, byte scope)
{
    if (sheetID >= 0 && sheetID < v5_SURFACE_MAX) {
        QImage sheet;
        QColor transClr = QColor(0xFFFF00FF);
        if (QFileInfo(sheetPath).suffix() == "gif") {
            QGifImage gif(sheetPath);
            QVector<QRgb> table = gif.globalColorTable();
            transClr            = QColor(table[0]);
            sheet               = gif.frame(0);
            sheet.setColor(0, 0x00000000);
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

void SceneViewerv5::removeGraphicsFile(QString sheetPath, int slot)
{
    if (slot >= 0) {
        gfxSurface[slot].texturePtr->destroy();
        delete gfxSurface[slot].texturePtr;
        gfxSurface[slot].scope = SCOPE_NONE;
    }
    else {
        for (int i = 1; i < v5_SURFACE_MAX; ++i) {
            if (QString(sheetPath) == gfxSurface[i].name) {
                gfxSurface[i].texturePtr->destroy();
                delete gfxSurface[i].texturePtr;
                gfxSurface[i].scope = SCOPE_NONE;
            }
        }
    }
}

void SceneViewerv5::drawTile(float XPos, float YPos, float ZPos, int tileX, int tileY, byte direction)
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
    vVBO2D.allocate(rectVerticesv5, 6 * sizeof(QVector3D));
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

void SceneViewerv5::drawSpriteFlipped(float XPos, float YPos, float width, float height, float sprX,
                                      float sprY, int direction, int inkEffect, int alpha, int sheetID)
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

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
        case INK_LOOKUP:
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

    if (width + XPos > screens->clipBound_X2 * invZoom())
        width = screens->clipBound_X2 * invZoom() - XPos;

    if (XPos < screens->clipBound_X1 * invZoom()) {
        float val = XPos - screens->clipBound_X1 * invZoom();
        if (!(direction & FLIP_X))
            sprX -= val;
        width += val;
        XPos = screens->clipBound_X1 * invZoom();
    }

    if (height + YPos > screens->clipBound_Y2 * invZoom())
        height = screens->clipBound_Y2 * invZoom() - YPos;

    if (YPos < screens->clipBound_Y1 * invZoom()) {
        float val = YPos - screens->clipBound_Y1 * invZoom();
        if (!(direction & FLIP_Y))
            sprY -= val;
        height += val;
        YPos = screens->clipBound_Y1 * invZoom();
    }

    if (width <= 0 || height <= 0)
        return;

    float entX      = activeDrawEntity->pos.x - cam.pos.x;
    float entY      = activeDrawEntity->pos.y - cam.pos.y;
    float boxLeft   = startX;
    float boxTop    = startY;
    float boxRight  = startX + startW;
    float boxBottom = startY + startH;
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

    // Draw Sprite
    float xpos = XPos;
    float ypos = YPos;
    float zpos = incZ();

    addRenderState(inkEffect, 4, 6, sheetID, alpha);

    switch (direction) {
        case FLIP_NONE:
            addPoly(xpos, ypos, sprX, sprY, 0, sheetID);
            addPoly(xpos + width, ypos, sprX + width, sprY, 0, sheetID);
            addPoly(xpos, ypos + height, sprX, sprY + height, 0, sheetID);
            addPoly(xpos + width, ypos + height, sprX + width, sprY + height, 0, sheetID);
            break;
        case FLIP_X:
            addPoly(xpos, ypos, sprX + width, sprY, 0, sheetID);
            addPoly(xpos + width, ypos, sprX, sprY, 0, sheetID);
            addPoly(xpos, ypos + height, sprX + width, sprY + height, 0, sheetID);
            addPoly(xpos + width, ypos + height, sprX, sprY + height, 0, sheetID);
            break;
        case FLIP_Y:
            addPoly(xpos, ypos, sprX, sprY + height, 0, sheetID);
            addPoly(xpos + width, ypos, sprX + width, sprY + height, 0, sheetID);
            addPoly(xpos, ypos + height, sprX, sprY, 0, sheetID);
            addPoly(xpos + width, ypos + height, sprX + width, sprY, 0, sheetID);
            break;
        case FLIP_XY:
            addPoly(xpos, ypos, sprX + width, sprY + height, 0, sheetID);
            addPoly(xpos + width, ypos, sprX, sprY + height, 0, sheetID);
            addPoly(xpos, ypos + height, sprX + width, sprY, 0, sheetID);
            addPoly(xpos + width, ypos + height, sprX, sprY, 0, sheetID);
            break;
    }
    validDraw = true;
}

void SceneViewerv5::drawSpriteRotozoom(float XPos, float YPos, float pivotX, float pivotY, float width,
                                       float height, float sprX, float sprY, int scaleX, int scaleY,
                                       int direction, short rotation, int inkEffect, int alpha,
                                       int sheetID)
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

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
        case INK_LOOKUP:
            // if (!lookupTable)
            //    return;
            break;
        case INK_MASKED: alpha = 0xFF; break;
        case INK_UNMASKED: alpha = 0xFF; break;
    }

    int angle = 0x200 - (rotation & 0x1FF);
    if (!(rotation & 0x1FF))
        angle = rotation & 0x1FF;

    int sine          = sinVal512[angle];
    int cosine        = cosVal512[angle];
    float fullScaleXS = scaleX * sine / 512.0f;
    float fullScaleXC = scaleX * cosine / 512.0f;
    float fullScaleYS = scaleY * sine / 512.0f;
    float fullScaleYC = scaleY * cosine / 512.0f;

    float posX[4];
    float posY[4];

    int xMax     = 0;
    int scaledX1 = 0;
    int scaledX2 = 0;
    int scaledY1 = 0;
    int scaledY2 = 0;
    switch (direction) {
        default:
        case FLIP_NONE: {
            scaledX1 = fullScaleXS * (pivotX - 2);
            scaledX2 = fullScaleXC * (pivotX - 2);
            scaledY1 = fullScaleYS * (pivotY - 2);
            scaledY2 = fullScaleYC * (pivotY - 2);
            xMax     = pivotX + 2 + width;
            posX[0]  = XPos + ((scaledX2 + scaledY1) / 512.0f);
            posY[0]  = YPos + ((fullScaleYC * (pivotY - 2) - scaledX1) / 512.0f);
            break;
        }
        case FLIP_X: {
            scaledX1 = fullScaleXS * (2 - pivotX);
            scaledX2 = fullScaleXC * (2 - pivotX);
            scaledY1 = fullScaleYS * (pivotY - 2);
            scaledY2 = fullScaleYC * (pivotY - 2);
            xMax     = -2 - pivotX - width;
            posX[0]  = XPos + ((scaledX2 + scaledY1) / 512.0f);
            posY[0]  = YPos + ((fullScaleYC * (pivotY - 2) - scaledX1) / 512.0f);
            break;
        }
        case FLIP_Y:
        case FLIP_XY: break;
    }

    int scaledXMaxS = fullScaleXS * xMax;
    int scaledXMaxC = fullScaleXC * xMax;
    int scaledYMaxC = fullScaleYC * (pivotY + 2 + height);
    int scaledYMaxS = fullScaleYS * (pivotY + 2 + height);
    posX[1]         = XPos + ((scaledXMaxC + scaledY1) / 512.0f);
    posY[1]         = YPos + ((scaledY2 - scaledXMaxS) / 512.0f);
    posX[2]         = XPos + ((scaledYMaxS + scaledX2) / 512.0f);
    posY[2]         = YPos + ((scaledYMaxC - scaledX1) / 512.0f);
    posX[3]         = XPos + ((scaledXMaxC + scaledYMaxS) / 512.0f);
    posY[3]         = YPos + ((scaledYMaxC - scaledXMaxS) / 512.0f);

    float left = screens->pitch;
    for (int i = 0; i < 4; ++i) {
        if (posX[i] < left)
            left = posX[i];
    }
    if (left < screens->clipBound_X1 * invZoom())
        left = screens->clipBound_X1 * invZoom();

    float right = 0;
    for (int i = 0; i < 4; ++i) {
        if (posX[i] > right)
            right = posX[i];
    }
    if (right > screens->clipBound_X2 * invZoom())
        right = screens->clipBound_X2 * invZoom();

    float top = screens->height;
    for (int i = 0; i < 4; ++i) {
        if (posY[i] < top)
            top = posY[i];
    }
    if (top < screens->clipBound_Y1 * invZoom())
        top = screens->clipBound_Y1 * invZoom();

    float bottom = 0;
    for (int i = 0; i < 4; ++i) {
        if (posY[i] > bottom)
            bottom = posY[i];
    }
    if (bottom > screens->clipBound_Y2 * invZoom())
        bottom = screens->clipBound_Y2 * invZoom();

    int xDif = right - left;
    int yDif = bottom - top;

    if (xDif >= 1 && yDif >= 1 && (scaleX != 0 || scaleY != 0)) {
        validDraw = true;

        float entX = activeDrawEntity->pos.x - cam.pos.x;
        float entY = activeDrawEntity->pos.y - cam.pos.y;
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

        // Draw Sprite
        float xpos = left;
        float ypos = top;
        float zpos = incZ();

        addRenderState(inkEffect, 4, 6, sheetID, alpha);

        float sY  = scaleY / (float)(1 << 9);
        float sX  = scaleX / (float)(1 << 9);
        float sin = sinVal512[angle] / (float)(1 << 9);
        float cos = cosVal512[angle] / (float)(1 << 9);
        if (direction == FLIP_NONE) {
            int x = pivotX;
            int y = pivotY;
            addPoly(xpos + ((x * cos + y * sin)) * sX, ypos + ((y * cos - x * sin)) * sY, sprX, sprY, 0,
                    sheetID);

            x = width + pivotX;
            y = pivotY;
            addPoly(xpos + ((x * cos + y * sin)) * sX, ypos + ((y * cos - x * sin)) * sY, sprX + width,
                    sprY, 0, sheetID);

            x = pivotX;
            y = height + pivotY;
            addPoly(xpos + ((x * cos + y * sin)) * sX, ypos + ((y * cos - x * sin)) * sY, sprX,
                    sprY + height, 0, sheetID);

            x = width + pivotX;
            y = height + pivotY;
            addPoly(xpos + ((x * cos + y * sin)) * sX, ypos + ((y * cos - x * sin)) * sY, sprX + width,
                    sprY + height, 0, sheetID);
        }
        else {
            int x = -pivotX;
            int y = pivotY;
            addPoly(xpos + ((x * cos + y * sin)) * sX, ypos + ((y * cos - x * sin)) * sY, sprX, sprY, 0,
                    sheetID);

            x = -pivotX - width;
            y = pivotY;
            addPoly(xpos + ((x * cos + y * sin)) * sX, ypos + ((y * cos - x * sin)) * sY, sprX + width,
                    sprY, 0, sheetID);

            x = -pivotX;
            y = height + pivotY;
            addPoly(xpos + ((x * cos + y * sin)) * sX, ypos + ((y * cos - x * sin)) * sY, sprX,
                    sprY + height, 0, sheetID);

            x = -pivotX - width;
            y = height + pivotY;
            addPoly(xpos + ((x * cos + y * sin)) * sX, ypos + ((y * cos - x * sin)) * sY, sprX + width,
                    sprY + height, 0, sheetID);
        }

        validDraw = true;
    }
}

void SceneViewerv5::drawLine(float x1, float y1, float z1, float x2, float y2, float z2, float scale,
                             Vector4<float> colour, Shader &shader)
{
    if (x1 == x2 && y1 == y2 && z1 == z2)
        return;

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

    // validDraw = true;
}

void SceneViewerv5::drawRect(float x, float y, float z, float w, float h, Vector4<float> colour,
                             Shader &shader, bool outline)
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    shader.use();
    if (outline) {
        // top
        drawLine(x, y, z, x + w, y, z, zoom, colour, shader);
        // bottom
        drawLine(x, y + h, z, x + w, y + h, z, zoom, colour, shader);
        // left
        drawLine(x, y, z, x, y + h, z, zoom, colour, shader);
        // right
        drawLine(x + w, y, z, x + w, y + h, z, zoom, colour, shader);

        validDraw = true;
    }
    else {
        shader.setValue("colour", QVector4D(colour.x, colour.y, colour.z, colour.w));

        rectVAO.bind();

        QMatrix4x4 matModel;
        matModel.scale(w, h, 1.0f);
        matModel.translate(x / w, y / h, z);
        shader.setValue("model", matModel);

        f->glDrawArrays(GL_TRIANGLES, 0, 6);

        rectVAO.release();

        validDraw = true;
    }
}

void SceneViewerv5::drawCircle(float x, float y, float z, float r, Vector4<float> colour,
                               Shader &shader, bool outline)
{
    Q_UNUSED(x)
    Q_UNUSED(y)
    Q_UNUSED(z)
    Q_UNUSED(r)
    Q_UNUSED(colour)
    Q_UNUSED(shader)

    // QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    if (outline) {
    }
    else {
    }
}

void SceneViewerv5::drawFace(Vector2<int> *vertices, int vertCount, int r, int g, int b, int alpha,
                             InkEffects inkEffect)
{
    uint colour = (r << 16) | (g << 8) | (b << 0);

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
        addPoly(vertices[i].x / (float)(1 << 16), vertices[i].y / (float)(1 << 16), 0, 0, 0, colour);

    validDraw = true;
}

void SceneViewerv5::drawBlendedFace(Vector2<int> *vertices, uint *colors, int vertCount, int alpha,
                                    InkEffects inkEffect)
{
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

    validDraw = true;
}

inline void SceneViewerv5::addPoly(float x, float y, float z, float u, float v, uint color,
                                   GFXSurface *surface)
{
    vertexList[renderCount].pos.setX(x);
    vertexList[renderCount].pos.setY(y);
    vertexList[renderCount].pos.setZ(z);
    if (surface && surface != gfxSurface) {
        u /= surface->width;
        v /= surface->height;
    }
    vertexList[renderCount].uv.setX(u);
    vertexList[renderCount].uv.setY(v);

    vertexList[renderCount].colour.setX(((color >> 16) & 0xFF) / 255.0f);
    vertexList[renderCount].colour.setX(((color >> 8) & 0xFF) / 255.0f);
    vertexList[renderCount].colour.setX(((color >> 0) & 0xFF) / 255.0f);
    vertexList[renderCount].colour.setX(((color >> 24) & 0xFF) / 255.0f);
    renderCount++;
}

bool32 statesCompatible(SceneViewerv5::RenderState *one, SceneViewerv5::RenderState *two)
{
    if (one->blendMode != INK_NONE && one->blendMode != INK_LOOKUP)
        return false; // the rest can't really be merged
    if (one->blendMode != two->blendMode || one->shader != two->shader || one->alpha != two->alpha
        || memcmp(&one->clipRectTL, &two->clipRectTL, sizeof(int) * 4))
        return false;
    return true;
}

void SceneViewerv5::addRenderState(int blendMode, ushort vertCount, ushort indexCount, int sheetID,
                                   byte alpha, Shader *shader, ushort *altIndex, Vector2<int> *clipRect)
{
    if (!vertCount || !indexCount)
        return;
    if (!shader)
        shader = &spriteShader;

    // set up a new state minimally needed for comparison
    RenderState newState;
    newState.blendMode  = blendMode;
    newState.indexCount = indexCount;
    newState.alpha      = alpha;
    newState.shader     = shader;

    if (sheetID >= 0)
        newState.sheetID = sheetID;
    else
        newState.sheetID = -1;

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

    if (renderStateIndex + 1 && vertCount + renderCount < 0x800) {
        RenderState *last = &renderStates[renderStateIndex];
        if (last->indexCount + indexCount < (0x800 * 6) && statesCompatible(last, &newState)) {
            // merge em and we'll be on our way
            memcpy(&last->indecies[last->indexCount], altIndex, indexCount * sizeof(ushort));
            for (int i = 0; i < indexCount; ++i) last->indecies[i + last->indexCount] += renderCount;
            last->indexCount += indexCount;
            return;
        }
    }
    renderStateIndex++;
    // if (vertCount + renderCount >= 0x800) {
    //     RenderRenderStates(); // you should render NOW!
    //     renderStateIndex++;
    // }

    memcpy(newState.indecies, altIndex, indexCount * sizeof(ushort));
    if (renderCount)
        for (int i = 0; i < indexCount; ++i) newState.indecies[i] += renderCount;
    renderStates.append(newState);
}

void SceneViewerv5::renderRenderStates()
{
    if (!renderCount)
        return;
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    rectVAO.bind();

    int prevBlendMode   = -1;
    int prevSheet       = -1;
    GFXSurface *surface = nullptr;
    Shader *prevShader  = nullptr;

    f->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO);
    VBO.bind();

    for (int i = 0; i < renderStates.count(); ++i) {
        RenderState *renderState = &renderStates[i];

        if (prevSheet != renderState->sheetID) {
            prevSheet = renderState->sheetID;

            if (prevSheet < 0x40) {
                surface = &gfxSurface[prevSheet];
                surface->texturePtr->bind();
            }
            else {
                surface = nullptr;
            }
        }

        if (renderState->shader != prevShader) {
            renderState->shader->use();
            renderState->shader->setValue("projection", getProjectionMatrix());
            renderState->shader->setValue("view", QMatrix4x4());
            renderState->shader->setValue("useAlpha", false);
            renderState->shader->setValue("alpha", 1.0f);

            QMatrix4x4 matModel;
            matModel.scale(zoom, zoom, 1.0f);
            renderState->shader->setValue("model", matModel);

            prevShader = renderState->shader;
        }

        if (prevBlendMode != renderState->blendMode) {
            switch (renderState->blendMode) {
                case INK_MASKED:
                case INK_UNMASKED:
                case INK_LOOKUP:
                case INK_NONE:
                case INK_SUB:
                case INK_BLEND:
                case INK_ALPHA: f->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); break;
                case INK_ADD: f->glBlendFunc(GL_SRC_ALPHA, GL_ONE); break;
            }

            prevBlendMode = renderState->blendMode;
        }

        // renderState->shader->setValue("colour", renderState->colour);

        f->glBufferData(GL_ELEMENT_ARRAY_BUFFER, renderState->indexCount * sizeof(ushort),
                        renderState->indecies, GL_DYNAMIC_DRAW);
        f->glDrawElements(GL_TRIANGLES, renderState->indexCount, GL_UNSIGNED_SHORT, 0);
    }

    // VBO.release();
    rectVAO.release();
    renderCount      = 0;
    renderStateIndex = -1;
    renderStates.clear();
    f->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void SceneViewerv5::refreshResize()
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

    refreshScnEditorVerts(storedW >> 2, storedH >> 2);

    prevStoredW = storedW;
    prevStoredH = storedH;
}

GameObjectInfo *SceneViewerv5::GetObjectInfo(QString name)
{
    QByteArray hashData = Utils::getMd5HashByteArray(name);
    byte data[0x10];
    for (int i = 0; i < 0x10; ++i) data[i] = hashData[i];

    uint hash[4];
    memcpy(hash, data, 0x10 * sizeof(byte));

    for (auto &link : gameLinks) {
        for (int i = 0; i < link.gameObjectList.count(); ++i) {
            if (memcmp(hash, link.gameObjectList[i].hash, 0x10 * sizeof(byte)) == 0) {
                return &link.gameObjectList[i];
            }
        }
    }
    return NULL;
}
