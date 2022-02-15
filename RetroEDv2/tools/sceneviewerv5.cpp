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
    for (int i = 0; i < 0x8000; i++) {
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

    VAO->destroy();
    fbpVAO->destroy();
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
    // loading
    QString pth      = path;
    QString basePath = pth.replace(QFileInfo(pth).fileName(), "");

    // Tile Texture
    if (QFile::exists(basePath + "16x16Tiles.gif")) {
        // setup tileset texture from png
        QGifImage tilesetGif(basePath + "16x16Tiles.gif");
        QImage tileset = tilesetGif.frame(0);

        GLint active, a;
        glFuncs->glGetIntegerv(GL_ACTIVE_TEXTURE, &active);
        gfxSurface[0].scope      = SCOPE_STAGE;
        gfxSurface[0].name       = "Tileset";
        gfxSurface[0].width      = tileset.width();
        gfxSurface[0].height     = tileset.height();
        gfxSurface[0].texturePtr = createTexture(tileset, QOpenGLTexture::Target2D);

        glFuncs->glActiveTexture(GL_TEXTURE30);
        glFuncs->glBindTexture(GL_TEXTURE_RECTANGLE, gfxSurface[0].texturePtr->textureId());
        glFuncs->glGetIntegerv(GL_TEXTURE_BINDING_RECTANGLE, &a);
        printLog(QString::number(a));
        glFuncs->glActiveTexture(active);

        auto pal = tileset.colorTable();
        tilePalette.clear();
        for (QRgb &col : pal) {
            tilePalette.append(PaletteColour(col));
        }

        gfxSurface[0].transClr = tilePalette[0].toQColor();
    }

    // Default Texture
    if (gfxSurface[1].scope == SCOPE_NONE) {
        gfxSurface[1].scope      = SCOPE_GLOBAL;
        gfxSurface[1].name       = ":/icons/missingV5.png";
        missingObj               = QImage(gfxSurface[1].name);
        gfxSurface[1].texturePtr = createTexture(missingObj);
        Utils::getHashInt(gfxSurface[1].name, gfxSurface[1].hash);
    }

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

    objProp->updateUI();

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
    if (cam.pos.x + storedW > (sceneWidth * 0x10))
        cam.pos.x = ((sceneWidth * 0x10) - storedW);

    if (cam.pos.y + storedH > (sceneHeight * 0x10))
        cam.pos.y = ((sceneHeight * 0x10) - storedH);

    if (cam.pos.x < -64)
        cam.pos.x = -64;

    if (cam.pos.y < -64)
        cam.pos.y = -64;

    screens[0].position.x = cam.pos.x;
    screens[0].position.y = cam.pos.y;

    float bgOffsetY  = 0;
    Vector4<float> c = { altBGColour.r / 255.f, altBGColour.g / 255.f, altBGColour.b / 255.f, 1.f };
    bgOffsetY -= fmod(cam.pos.y, 0x200 * invZoom());
    for (float y = bgOffsetY; y < (storedH + 0x80); y += 0x100 * invZoom()) {
        float bgOffsetX = ((fmod((y - bgOffsetY), 0x200 * invZoom()) == 0) ? 0x100 : 0x00) - fmod(cam.pos.x, 0x200 * invZoom());
        for (float x = bgOffsetX; x < (storedW + 0x80); x += 0x200 * invZoom()) {
            drawRect(x, y, 0x100 * invZoom(), 0x100 * invZoom(), c);
        }
    }

    bool showCLayers[2] = { showPlaneA, showPlaneB };

    for (int l = 0; l < v5_DRAWLAYER_COUNT; ++l) {
        drawLayers[l].layerDrawList.clear();
    }

    for (int t = 0; t < scene.layers.count(); ++t) {
        byte drawOrder = scene.layers[t].drawOrder;
        if (drawOrder < v5_DRAWLAYER_COUNT)
            drawLayers[drawOrder].layerDrawList.append(t);
    }

    // LAYERS
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

            int vertCnt = 0;

            int camX = cam.pos.x;
            int camY = cam.pos.y;
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

            renderRenderStates();

            int count = 0;
            for (int y = basedY; y < countY; ++y) {
                for (int x = basedX; x < countX; ++x) {
                    ushort tile = layout[y][x];
                    if (tile != 0xFFFF) {
                        ++count;
                        float xp = (x * 0x10) - cam.pos.x;
                        float yp = (y * 0x10) - cam.pos.y;

                        ushort t     = (tile & 0x3FF);
                        byte f       = (tile >> 10) & 3;
                        ushort point = (t << 2) | (f << 12);
                        addPoly(xp, yp, tileUVArray[point], tileUVArray[point + 1], 0, gfxSurface);
                        addPoly(xp + 0x10, yp, tileUVArray[point + 2], tileUVArray[point + 1], 0,
                                gfxSurface);
                        addPoly(xp, yp + 0x10, tileUVArray[point], tileUVArray[point + 3], 0,
                                gfxSurface);
                        addPoly(xp + 0x10, yp + 0x10, tileUVArray[point + 2], tileUVArray[point + 3], 0,
                                gfxSurface);

                        // safety pass
                        if (renderCount >= 0x8000 - 8) {
                            renderCount -= count * 4;
                            PlaceArgs args;
                            args.texID = 0;
                            addRenderState(selectedLayer == l ? INK_NONE : INK_BLEND, count * 4,
                                           count * 6, &args, 0xFF, &placeShader);
                            renderCount += count * 4;
                            renderRenderStates();
                            count = 0;
                        }
                    }
                }
            }
            renderCount -= count * 4;
            PlaceArgs args;
            args.texID = 0;
            addRenderState(selectedLayer == l ? INK_NONE : INK_BLEND, count * 4, count * 6, &args, 0xFF,
                           &placeShader);
            renderCount += count * 4;
            renderRenderStates();

            // welcome to the magic of rmg code. dangerous-looking code ahead
            // there's definitely better ways to do this, but for now this is what we gotta do
            // TODO: 2022 rmg here. this is shitty even if it works well. find someway else to do
            // this

            // Collision Previews
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

                                byte flipmode = flipY | (cmask.direction << 1);

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

                    float x = cam.pos.x;
                    float y = cam.pos.y;

                    auto tileGL = createTexture(colTex->copy(x, y, storedW / zoom, storedH / zoom));
                }
            }

            // PARALLAX
            if (l == selectedLayer && l >= 0) {
                if (scene.layers[l].type == 0 || scene.layers[l].type == 1) {

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
                                drawLine(0.0f, info.startLine - cam.pos.y, (w - cam.pos.x),
                                         (info.startLine - cam.pos.y), clr);

                                drawLine(0.0f, ((info.startLine + info.length) - cam.pos.y),
                                         (w - cam.pos.x), ((info.startLine + info.length) - cam.pos.y),
                                         clr);
                            }
                            else if (scene.layers[l].type == 1) {
                                int h = (height * 0x10);
                                drawLine((info.startLine - cam.pos.x), 0.0f,
                                         (info.startLine - cam.pos.x), (h - cam.pos.y), clr);

                                drawLine(((info.startLine + info.length) - cam.pos.x), 0.0f,
                                         ((info.startLine + info.length) - cam.pos.x), (h - cam.pos.y),
                                         clr);
                            }

                            ++id;
                        }
                    }
                }
            }
        }
    }
    // ENTITIES
    for (int p = 0; p < v5_DRAWLAYER_COUNT; ++p) {
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

                float xpos = entity->pos.x - (cam.pos.x);
                float ypos = entity->pos.y - (cam.pos.y);

                drawSpriteFlipped(xpos, ypos, gfxSurface[1].width, gfxSurface[1].height, 0, 0,
                                  FLIP_NONE, INK_NONE, 0xFF, 1);
            }
        }
    }

    // TILE PREVIEW
    if (selectedTile >= 0 && selectedLayer >= 0 && isSelecting && curTool == TOOL_PENCIL) {
        float tx = tilePos.x;
        float ty = tilePos.y;

        float tx2 = tx + fmodf(cam.pos.x, 0x10);
        float ty2 = ty + fmodf(cam.pos.y, 0x10);

        // clip to grid
        tx -= fmodf(tx2, 0x10);
        ty -= fmodf(ty2, 0x10);

        // Draw Selected Tile Preview
        float xpos = tx + cam.pos.x;
        float ypos = ty + cam.pos.y;

        xpos -= cam.pos.x;
        ypos -= cam.pos.y;

        addRenderState(INK_BLEND, 4, 6, nullptr, 0xFF, &placeShader);

        ushort t     = (selectedTile & 0x3FF);
        byte f       = (selectedTile >> 10) & 3;
        ushort point = (t << 2) | (f << 12);
        addPoly(xpos, ypos, 0, tileUVArray[point], tileUVArray[point + 1]);
        addPoly(xpos + 0x10, ypos, 0, tileUVArray[point + 2], tileUVArray[point + 1]);
        addPoly(xpos, ypos + 0x10, 0, tileUVArray[point], tileUVArray[point + 3]);
        addPoly(xpos + 0x10, ypos + 0x10, 0, tileUVArray[point + 2], tileUVArray[point + 3]);
    }

    // ENT PREVIEW

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

            drawSpriteFlipped(xpos, ypos, gfxSurface[1].width, gfxSurface[1].height, 0, 0, FLIP_NONE,
                              INK_NONE, 0xFF, 1);
        }
    }

    if (showGrid) {
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

            drawLine(x1, dy, x2, dy, Vector4<float>(1.0f, 1.0f, 1.0f, 1.0f));
        }

        for (; x < w; x += gridSize.x) {
            float dx = (x + (zoom <= 1.0f ? 1.0f : 0.0f) - camX);

            float y1 = 0;
            float y2 = (((camY + storedH * invZoom())) - camY);
            if (cam.pos.y < 0) {
                y1 += abs(cam.pos.y);
            }

            drawLine(dx, y1, dx, y2, Vector4<float>(1.0f, 1.0f, 1.0f, 1.0f));
        }
    }

    if (selectedEntity >= 0) {
        SceneEntity &entity = entities[selectedEntity];

        float left   = entity.pos.x + entity.box.x;
        float top    = entity.pos.y + entity.box.y;
        float right  = entity.pos.x + entity.box.w;
        float bottom = entity.pos.y + entity.box.h;

        float w = fabsf(right - left), h = fabsf(bottom - top);

        drawRect(left - cam.pos.x, top - cam.pos.y, w, h, Vector4<float>(1.0f, 1.0f, 1.0f, 1.0f), false, 0x40, INK_ALPHA);
        drawRect(left - cam.pos.x, top - cam.pos.y, w, h, Vector4<float>(1.0f, 1.0f, 1.0f, 1.0f), true);
    }

    // Selected Stamp Box
    if (selectedStamp >= 0) {
        RSDKv5::Stamps::StampEntry &stamp = stamps.stampList[selectedStamp];

        float left   = stamp.pos.x - (stamp.size.x / 2.0f);
        float top    = stamp.pos.y - (stamp.size.y / 2.0f);
        float right  = stamp.pos.x + (stamp.size.x / 2.0f);
        float bottom = stamp.pos.y + (stamp.size.y / 2.0f);

        float w = fabsf(right - left), h = fabsf(bottom - top);
        gfxSurface[0].texturePtr->bind();

        drawRect(left - cam.pos.x, top - cam.pos.y, w, h, Vector4<float>(1.0f, 0.0f, 0.0f, 1.0f), false,
                 0x40, INK_ALPHA);
        drawRect(left - cam.pos.x, top - cam.pos.y, w, h, Vector4<float>(1.0f, 0.0f, 0.0f, 1.0f), true);
    }

    // Selection Box
    if (curTool == TOOL_SELECT && (isSelecting || (selectSize.x && selectSize.y))) {

        drawRect(selectPos.x - cam.pos.x, selectPos.y - cam.pos.y, selectSize.x, selectSize.y,
                 Vector4<float>(0.0f, 0.0f, 1.0f, 1.f), true);

        drawRect(selectPos.x - cam.pos.x, selectPos.y - cam.pos.y, selectSize.x, selectSize.y,
                 Vector4<float>(0.0f, 0.0f, 1.0f, 1.f), false, 0x40, INK_ALPHA);


        for (auto &id : selectedEntities) {
            SceneEntity &entity = entities[id];

            float left   = entity.pos.x + entity.box.x;
            float top    = entity.pos.y + entity.box.y;
            float right  = entity.pos.x + entity.box.w;
            float bottom = entity.pos.y + entity.box.h;

            float w = fabsf(right - left), h = fabsf(bottom - top);

            drawRect(left - cam.pos.x, top - cam.pos.y, w, h, Vector4<float>(1.0f, 1.0f, 1.0f, 1.0f),
                     false, 0x40, INK_ALPHA);
            drawRect(left - cam.pos.x, top - cam.pos.y, w, h, Vector4<float>(1.0f, 1.0f, 1.0f, 1.0f),
                     true);
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
                // callGameEvent(gameLink.GetObjectInfo(objects[entities[e].type].name),
                // EVENT_UPDATE,
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
    glFuncs = context()->functions();

    QSurfaceFormat fmt = context()->format();
    printLog("Widget Using OpenGL " + QString::number(fmt.majorVersion()) + "."
             + QString::number(fmt.minorVersion()));

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

    printLog("OpenGL Details");
    printLog("Vendor:       " + vendorStr);
    printLog("Renderer:     " + rendererStr);
    printLog("Version:      " + versionStr);
    printLog("GLSL version: " + sdrVersionStr);
    printLog("Extensions:   " + extensionsStr);
    printLog(context()->isOpenGLES() ? "Using OpenGLES" : "Using OpenGL");
    printLog(context()->isValid() ? "Is valid" : "Not valid");

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
        if (args->texID != -1) {
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
    passthroughFBShader.argsCB = [this](Shader *self, void *state) {
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
    attribVBO->allocate(0x8000 * sizeof(DrawVertex));
    // we have to do this directly bc for whatever reason it's normally per program ??????
    glFuncs->glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(DrawVertex), 0);
    glFuncs->glEnableVertexAttribArray(0);
    glFuncs->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(DrawVertex),
                                   (void *)offsetof(DrawVertex, uv));
    glFuncs->glEnableVertexAttribArray(1);
    glFuncs->glVertexAttribPointer(2, 4, GL_FLOAT, GL_TRUE, sizeof(DrawVertex),
                                   (void *)offsetof(DrawVertex, colour));
    glFuncs->glEnableVertexAttribArray(2);
    indexVBO = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    indexVBO->create();
    indexVBO->bind();
    indexVBO->allocate(0x8000 * 6 * sizeof(ushort));

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

void SceneViewerv5::resizeGL(int w, int h)
{

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

    fbpVBO->bind();
    fbpVBO->write(0, pos, sizeof(int) * 16);

    storedW = w;
    storedH = h;
    refreshResize();

    passthroughFBShader.use();
    passthroughFBShader.setValue("projection", getProjectionMatrix());
    passthroughFBShader.setValue("view", QMatrix4x4());

    finalFBShader.use();
    finalFBShader.setValue("projection", getProjectionMatrix());
    finalFBShader.setValue("view", QMatrix4x4());

    glFuncs->glViewport(0, 0, w, h);

    glFuncs->glActiveTexture(GL_TEXTURE20);
    if (tFB)
        delete tFB;
    tFB = new QOpenGLFramebufferObject(w, h);
    glFuncs->glBindTexture(GL_TEXTURE_2D, tFB->texture());

    glFuncs->glActiveTexture(GL_TEXTURE20 + 1);
    if (t2FB)
        delete t2FB;
    t2FB = new QOpenGLFramebufferObject(w, h);
    glFuncs->glBindTexture(GL_TEXTURE_2D, t2FB->texture());

    glFuncs->glActiveTexture(GL_TEXTURE20 + 2);
    if (outFB)
        delete outFB;
    outFB = new QOpenGLFramebufferObject(w, h);
    glFuncs->glBindTexture(GL_TEXTURE_2D, outFB->texture());
}

void SceneViewerv5::paintGL()
{
    glFuncs = context()->functions();

    outFB->bind();
    glFuncs->glClearColor(bgColour.r / 255.0f, bgColour.g / 255.0f, bgColour.b / 255.0f, 1.0f);
    glFuncs->glClear(GL_COLOR_BUFFER_BIT);

    if (!disableObjects)
        processObjects();
    drawScene();

    // swap FB0
    glFuncs->glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebufferObject());
    finalFBShader.use();
    fbpVAO->bind();
    fbpVBO->bind();
    fbiVBO->bind();
    glFuncs->glBlendFunc(GL_ONE, GL_ZERO);
    glFuncs->glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
}

int SceneViewerv5::addGraphicsFile(QString sheetPath, int sheetID, byte scope)
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

void SceneViewerv5::drawTile(float XPos, float YPos, int tileX, int tileY, byte direction)
{
    Q_UNUSED(XPos);
    Q_UNUSED(YPos);
    Q_UNUSED(tileX);
    Q_UNUSED(tileY);
    Q_UNUSED(direction);
}

void SceneViewerv5::drawSpriteFlipped(float XPos, float YPos, float width, float height, float sprX,
                                      float sprY, int direction, InkEffects inkEffect, int alpha,
                                      int sheetID)
{
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

void SceneViewerv5::drawSpriteRotozoom(float XPos, float YPos, float pivotX, float pivotY, float width,
                                       float height, float sprX, float sprY, int scaleX, int scaleY,
                                       int direction, short rotation, InkEffects inkEffect, int alpha,
                                       int sheetID)
{
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

    GFXSurface *sheet = &gfxSurface[sheetID];
    float xpos        = XPos;
    float ypos        = YPos;

    PlaceArgs args;
    args.texID = sheetID;
    addRenderState(inkEffect, 4, 6, &args, alpha);

    float sY  = scaleY / (float)(1 << 9);
    float sX  = scaleX / (float)(1 << 9);
    float sin = sinVal512[angle] / (float)(1 << 9);
    float cos = cosVal512[angle] / (float)(1 << 9);

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
    float entX = activeDrawEntity->pos.x - cam.pos.x;
    float entY = activeDrawEntity->pos.y - cam.pos.y;
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

    addPoly(posX[0], posY[0], sprX, sprY, 0, sheet);
    addPoly(posX[1], posY[1], sprX + width, sprY, 0, sheet);
    addPoly(posX[2], posY[2], sprX, sprY + height, 0, sheet);
    addPoly(posX[3], posY[3], sprX + width, sprY + height, 0, sheet);

    validDraw = true;
}

void SceneViewerv5::drawLine(float x1, float y1, float x2, float y2, Vector4<float> colour, int alpha,
                             InkEffects ink)
{
    addRenderState(ink, 2, 2, nullptr, alpha, &lineShader);
    addPoly(x1, y1, 0, 0, colour);
    addPoly(x2, y2, 0, 0, colour);

    validDraw = true;
}

void SceneViewerv5::drawRect(float x, float y, float w, float h, Vector4<float> colour, bool outline,
                             int alpha, InkEffects inkEffect)
{
    if (outline) {
        // top
        drawLine(x, y, x + w, y, colour);
        // bottom
        drawLine(x, y + h, x + w, y + h, colour);
        // left
        drawLine(x, y, x, y + h, colour);
        // right
        drawLine(x + w, y, x + w, y + h, colour);
    }
    else {
        PlaceArgs args;
        args.texID = -1;
        addRenderState(inkEffect, 4, 6, &args, alpha);

        addPoly(x, y, 0, 0, colour);
        addPoly(x + w, y, 0, 0, colour);
        addPoly(x, y + h, 0, 0, colour);
        addPoly(x + w, y + h, 0, 0, colour);
    }
    validDraw = true;
}

void SceneViewerv5::drawCircle(float x, float y, float innerR, float outerR, Vector4<float> colour,
                               int alpha, InkEffects inkEffect)
{
    Q_UNUSED(x)
    Q_UNUSED(y)
    Q_UNUSED(innerR)
    Q_UNUSED(outerR)
    Q_UNUSED(colour)
    Q_UNUSED(alpha)
    Q_UNUSED(inkEffect)
}

void SceneViewerv5::drawFace(Vector2<int> *vertices, int vertCount, int r, int g, int b, int alpha,
                             InkEffects inkEffect)
{
    return; /*
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
        addPoly(vertices[i].x / (float)(1 << 16), vertices[i].y / (float)(1 << 16), 0, 0, 0,
    colour);

    validDraw = true;//*/
}

void SceneViewerv5::drawBlendedFace(Vector2<int> *vertices, uint *colors, int vertCount, int alpha,
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

inline void SceneViewerv5::addPoly(float x, float y, float u, float v, Vector4<float> color,
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

    vertexList[renderCount].colour.setX(color.x);
    vertexList[renderCount].colour.setY(color.y);
    vertexList[renderCount].colour.setZ(color.z);
    vertexList[renderCount].colour.setW(color.w);
    renderCount++;
}

bool32 statesCompatible(SceneViewerv5::RenderState &one, SceneViewerv5::RenderState &two)
{
    if (one.blendMode) // one->blendMode != INK_NONE
        return false;  // the rest can't really be merged
    if (memcmp(one.argBuffer, two.argBuffer, sizeof(one.argBuffer)) || one.blendMode != two.blendMode
        || one.shader != two.shader || one.alpha != two.alpha
        || memcmp(&one.clipRectTL, &two.clipRectTL, sizeof(int) * 4))
        return false;
    return true;
}

void SceneViewerv5::addRenderState(int blendMode, ushort vertCount, ushort indexCount, void *args,
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
    RenderState newState;
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

    if (renderStates.count() && vertCount + renderCount < 0x8000) {
        RenderState &last = renderStates.last();
        if (last.indexCount + indexCount < (0x8000 * 6) && statesCompatible(last, newState)) {
            // merge em and we'll be on our way
            memcpy(&last.indecies[last.indexCount], altIndex, indexCount * sizeof(ushort));
            for (int i = 0; i < indexCount; ++i) last.indecies[i + last.indexCount] += renderCount;
            last.indexCount += indexCount;
            return;
        }
    }
    if (vertCount + renderCount >= 0x8000) {
        renderRenderStates(); // you should render NOW!
    }

    memcpy(newState.indecies, altIndex, indexCount * sizeof(ushort));
    if (renderCount)
        for (int i = 0; i < indexCount; ++i) newState.indecies[i] += renderCount;
    renderStates.append(newState);
}

void SceneViewerv5::renderRenderStates()
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

    while (renderStates.count()) {
        RenderState renderState = renderStates.takeFirst();
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

        renderState.shader->setValue("projection", getProjectionMatrix());
        QMatrix4x4 matView;
        matView.translate(zoom, -zoom);
        matView.scale(zoom, zoom);
        matView.translate(-invZoom(), invZoom());

        renderState.shader->setValue("view", matView);

        indexVBO->write(0, renderState.indecies, renderState.indexCount * sizeof(ushort));
        glFuncs->glDrawElements(lines ? GL_LINES : GL_TRIANGLES, renderState.indexCount,
                                GL_UNSIGNED_SHORT, 0);

        fbpVAO->bind();
        fbpVBO->bind();
        fbiVBO->bind();

        switch (renderState.blendMode) {
            case INK_MASKED:
            case INK_UNMASKED:
            case INK_NONE:
            case INK_SUB:
            case INK_BLEND:
            case INK_ALPHA: glFuncs->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); break;
            case INK_ADD: glFuncs->glBlendFunc(GL_SRC_ALPHA, GL_ONE); break;
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
    renderStates.clear();
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
