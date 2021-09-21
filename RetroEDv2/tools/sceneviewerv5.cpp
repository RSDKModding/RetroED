#include "includes.hpp"
#include <cmath>

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

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, QOverload<>::of(&SceneViewerv5::updateScene));
    timer->start(1000.0f / 60.0f);

    for (int a = 0; a < v5_SPRFILE_COUNT; ++a) {
        spriteAnimationList[a].scope = SCOPE_NONE;
    }
    for (int s = 0; s < v5_SURFACE_MAX; ++s) {
        gfxSurface[s].scope = SCOPE_NONE;
    }
}

SceneViewerv5::~SceneViewerv5()
{
    unloadScene();

    for (int o = 0; o < v5_SURFACE_MAX; ++o) {
        if (gfxSurface[o].scope != SCOPE_NONE) {
            if (gfxSurface[o].texturePtr) {
                gfxSurface[o].texturePtr->destroy();
                delete gfxSurface[o].texturePtr;
            }
            gfxSurface[o].texturePtr = nullptr;
            gfxSurface[o].scope      = SCOPE_NONE;
        }
    }

    screenVAO.destroy();
    rectVAO.destroy();
}

void SceneViewerv5::loadScene(QString path)
{
    // unloading
    unloadScene();

    // TEMP until SetEditableVar() gets completed
    {
        variableNames.clear();

        QFile file(":/resources/variableList.txt");
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream txtreader(&file);
            while (!txtreader.atEnd()) variableNames.append(txtreader.readLine());

            file.close();
        }
    }

    // Default Texture
    if (gfxSurface[0].scope == SCOPE_NONE) {
        gfxSurface[0].scope      = SCOPE_GLOBAL;
        gfxSurface[0].name       = ":/icons/missing.png";
        missingObj               = QImage(gfxSurface[0].name);
        gfxSurface[0].texturePtr = createTexture(missingObj);
        Utils::getHashInt(gfxSurface[0].name, gfxSurface[0].hash);
    }

    // loading
    QString pth      = path;
    QString basePath = pth.replace(QFileInfo(pth).fileName(), "");

    scene.read(path);

    tileconfig.read(basePath + "TileConfig.bin");
    stageConfig.read(basePath + "StageConfig.bin");

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

        for (int i = 0; i < objNames.count(); ++i) {
            if (Utils::getMd5HashByteArray(objNames[i]) == obj.name.hash) {
                object.name = objNames[i];
                break;
            }
        }

        for (int v = 0; v < obj.variables.count(); ++v) {
            auto &var = obj.variables[v];
            VariableInfo variable;
            variable.name = var.name.hashString();
            variable.hash = var.name.hash;
            variable.type = var.type;

            for (int i = 0; i < variableNames.count(); ++i) {
                if (Utils::getMd5HashByteArray(variableNames[i]) == var.name.hash) {
                    variable.name = variableNames[i];
                    variable.hash = Utils::getMd5HashByteArray(variable.name);
                    break;
                }
            }
            object.variables.append(variable);
        }

        for (RSDKv5::Scene::SceneEntity &ent : obj.entities) {
            SceneEntity entity;
            entity.slotID = ent.slotID;
            entity.type   = type;
            entity.pos.x  = Utils::fixedToFloat(ent.position.x);
            entity.pos.y  = Utils::fixedToFloat(ent.position.y);

            for (int v = 0; v < ent.variables.count(); ++v) {
                entity.variables.append(ent.variables[v]);
            }

            entities.append(entity);
        }
        objects.append(object);
        ++type;
    }

    if (QFile::exists(basePath + "16x16Tiles.gif")) {
        // setup tileset texture from png
        QImage tileset(basePath + "16x16Tiles.gif");
        m_tilesetTexture = createTexture(tileset);
        for (int i = 0; i < 0x400; ++i) {
            int tx         = ((i % (tileset.width() / 0x10)) * 0x10);
            int ty         = ((i / (tileset.width() / 0x10)) * 0x10);
            QImage tileTex = tileset.copy(tx, ty, 0x10, 0x10);

            tiles.append(tileTex);
        }
    }
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

    scene.write(path);

    tileconfig.write(basePath + "TileConfig.bin");
    stageConfig.write(basePath + "StageConfig.bin");
}

void SceneViewerv5::updateScene()
{
    this->repaint();

    if (statusLabel) {
        int mx = (int)((mousePos.x * invZoom()) + cam.pos.x);
        int my = (int)((mousePos.y * invZoom()) + cam.pos.y);
        statusLabel->setText(
            QString("Zoom: %1%, Mouse Position: (%2, %3), Chunk Position: (%4, %5), Selected Chunk: "
                    "%6, Selected Layer: %7 (%8), Selected Object: %9")
                .arg(zoom * 100)
                .arg(mx)
                .arg(my)
                .arg((int)mx / 0x10)
                .arg((int)my / 0x10)
                .arg(selectedTile)
                .arg(selectedLayer)
                .arg(selectedLayer >= 0 && selectedLayer < scene.layers.count()
                         ? scene.layers[selectedLayer].m_name
                         : "")
                .arg(selectedObject >= 0 && selectedObject < objects.count()
                         ? objects[selectedObject].name
                         : ""));
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
    if (!m_tilesetTexture)
        return;
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    f->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    f->glBlendEquation(GL_FUNC_ADD);

    // pre-render
    if ((cam.pos.x * zoom) < 0)
        cam.pos.x = 0;

    if ((cam.pos.y * zoom) < 0)
        cam.pos.y = 0;

    if ((cam.pos.x * zoom) + storedW > (sceneWidth * 0x10) * zoom)
        cam.pos.x = ((sceneWidth * 0x10) - (storedW * invZoom()));

    if ((cam.pos.y * zoom) + storedH > (sceneHeight * 0x10) * zoom)
        cam.pos.y = ((sceneHeight * 0x10) - (storedH * invZoom()));

    // draw bg colours
    primitiveShader.use();
    primitiveShader.setValue("colour", QVector4D(altBGColour.r / 255.0f, altBGColour.g / 255.0f,
                                                 altBGColour.b / 255.0f, 1.0f));
    primitiveShader.setValue("projection", getProjectionMatrix());
    primitiveShader.setValue("view", QMatrix4x4());
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
            if (scene.layers[l].drawOrder != p) {
                if (!(selectedLayer == l && p == 15))
                    continue;
            }

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
            m_tilesetTexture->bind();
            spriteShader.setValue("flipX", false);
            spriteShader.setValue("flipY", false);

            int vertCnt = 0;

            int camX = (cam.pos.x + camOffset.x);
            int camY = (cam.pos.y + camOffset.y);

            int basedX = qMax(camX / 0x10, 0);
            int basedY = qMax(camY / 0x10, 0);

            int countX = width * 0x10 > storedW ? (storedW / 0x10) : width;
            int countY = height * 0x10 > storedH ? (storedH / 0x10) : height;

            countX = ceil(countX / zoom);
            countY = ceil(countY / zoom);

            countX = qMin(basedX + countX + 3, width);
            countY = qMin(basedY + countY + 3, height);

            for (int y = basedY; y < countY; ++y) {
                for (int x = basedX; x < countX; ++x) {
                    ushort tile = layout[y][x];
                    if (tile != 0xFFFF) {
                        float xpos = (x * 0x10) - (cam.pos.x + camOffset.x);
                        float ypos = (y * 0x10) - (cam.pos.y + camOffset.y);
                        float zpos = selectedLayer == l ? 15.5 : (15 - l);

                        vertsPtr[vertCnt + 0].setX(0.0f + (xpos / 0x10));
                        vertsPtr[vertCnt + 0].setY(0.0f + (ypos / 0x10));

                        vertsPtr[vertCnt + 1].setX(1.0f + (xpos / 0x10));
                        vertsPtr[vertCnt + 1].setY(0.0f + (ypos / 0x10));

                        vertsPtr[vertCnt + 2].setX(1.0f + (xpos / 0x10));
                        vertsPtr[vertCnt + 2].setY(1.0f + (ypos / 0x10));

                        vertsPtr[vertCnt + 3].setX(1.0f + (xpos / 0x10));
                        vertsPtr[vertCnt + 3].setY(1.0f + (ypos / 0x10));

                        vertsPtr[vertCnt + 4].setX(0.0f + (xpos / 0x10));
                        vertsPtr[vertCnt + 4].setY(1.0f + (ypos / 0x10));

                        vertsPtr[vertCnt + 5].setX(0.0f + (xpos / 0x10));
                        vertsPtr[vertCnt + 5].setY(0.0f + (ypos / 0x10));

                        for (int i = 0; i < 6; ++i) {
                            vertsPtr[vertCnt + i].setZ(zpos);
                        }

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
                spriteShader.setAttributeBuffer(0, GL_FLOAT, 0, 3, 0); //*/

                QOpenGLBuffer tVBO2D;
                tVBO2D.create();
                tVBO2D.setUsagePattern(QOpenGLBuffer::StaticDraw);
                tVBO2D.bind();
                tVBO2D.allocate(tVertsPtr, vertCnt * sizeof(QVector2D));
                spriteShader.enableAttributeArray(1);
                spriteShader.setAttributeBuffer(1, GL_FLOAT, 0, 2, 0); //*/

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
                                int w = (width * 0x10) * zoom;
                                drawLine(0.0f * zoom, (info.startLine - cam.pos.y) * zoom, zpos,
                                         (w - cam.pos.x) * zoom, (info.startLine - cam.pos.y) * zoom,
                                         zpos, clr, primitiveShader);

                                drawLine(0.0f * zoom,
                                         ((info.startLine + info.length) - cam.pos.y) * zoom, zpos,
                                         (w - cam.pos.x) * zoom,
                                         ((info.startLine + info.length) - cam.pos.y) * zoom, zpos, clr,
                                         primitiveShader);
                            }
                            else if (scene.layers[l].type == 1) {
                                int h = (height * 0x10) * zoom;
                                drawLine((info.startLine - cam.pos.x) * zoom, 0.0f * zoom, zpos,
                                         (info.startLine - cam.pos.x) * zoom, (h - cam.pos.y) * zoom,
                                         zpos, clr, primitiveShader);

                                drawLine(((info.startLine + info.length) - cam.pos.x) * zoom,
                                         0.0f * zoom, zpos,
                                         ((info.startLine + info.length) - cam.pos.x) * zoom,
                                         (h - cam.pos.y) * zoom, zpos, clr, primitiveShader);
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

        currZ = curTool == TOOL_ENTITY ? 15.6 : p;
        for (int o = 0; o < drawLayers[p].entries.count(); ++o) {
            SceneEntity *entity = &entities[drawLayers[p].entries[o]];
            validDraw           = false;

            int filter = 0xFF;
            for (int v = 0; v < objects[entity->type].variables.count(); ++v) {
                if (objects[entity->type].variables[v].name == "filter") {
                    filter = entity->variables[v].value_uint8;
                    break;
                }
            }

            if (!(filter & sceneFilter))
                continue;

            entity->gameEntity->position.x = Utils::floatToFixed(entity->pos.x);
            entity->gameEntity->position.y = Utils::floatToFixed(entity->pos.y);

            callGameEvent(gameLink.GetObjectInfo(objects[entity->type].name), EVENT_DRAW, entity);

            // Draw Default Object Sprite if invalid
            // TODO: probably draw text intead
            if (!validDraw) {
                spriteShader.use();
                rectVAO.bind();

                float xpos = entity->pos.x - (cam.pos.x);
                float ypos = entity->pos.y - (cam.pos.y);
                float zpos = incZ();

                int w = gfxSurface[0].texturePtr->width(), h = gfxSurface[0].texturePtr->height();
                if (prevSprite) {
                    gfxSurface[0].texturePtr->bind();
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
        m_tilesetTexture->bind();
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

        float cx = cam.pos.x;
        float cy = cam.pos.y;

        validDraw = false;
        GameEntity gameEntity;
        gameEntity.position.x = (ex + cx) * 65536.0f;
        gameEntity.position.y = (ey + cy) * 65536.0f;
        gameEntity.objectID   = selectedObject;

        SceneEntity tempEntity;
        tempEntity.type       = selectedObject;
        tempEntity.pos.x      = (ex + cx) * 65536.0f;
        tempEntity.pos.y      = (ey + cy) * 65536.0f;
        tempEntity.slotID     = 0xFFFF;
        tempEntity.gameEntity = &gameEntity;
        callGameEvent(gameLink.GetObjectInfo(objects[selectedObject].name), EVENT_DRAW, &tempEntity);

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
        int w = gfxSurface[0].texturePtr->width(), h = gfxSurface[0].texturePtr->height();
        gfxSurface[0].texturePtr->bind();

        drawRect(((entity.pos.x - cam.pos.x) - (w / 2)) * zoom,
                 ((entity.pos.y - cam.pos.y) - (h / 2)) * zoom, 15.7f, w * zoom, h * zoom,
                 Vector4<float>(1.0f, 1.0f, 1.0f, 1.0f), primitiveShader, true);
    }

    if (showTileGrid) {
        rectVAO.bind();

        float camX = cam.pos.x;
        float camY = cam.pos.y;

        for (int y = camY - ((int)camY % 0x10); y < (camY + storedH) * (zoom < 1.0f ? invZoom() : 1.0f);
             y += 0x10) {
            drawLine((camX - camX) * zoom, (y - camY) * zoom, 15.6f,
                     (((camX + storedW * invZoom())) - camX) * zoom, (y - camY) * zoom, 15.6f,
                     Vector4<float>(1.0f, 1.0f, 1.0f, 1.0f), primitiveShader);
        }

        for (int x = camX - ((int)camX % 0x10); x < (camX + storedW) * (zoom < 1.0f ? invZoom() : 1.0f);
             x += 0x10) {
            drawLine((x + (zoom <= 1.0f ? 1.0f : 0.0f) - camX) * zoom, (camY - camY) * zoom, 15.6f,
                     (x + (zoom <= 1.0f ? 1.0f : 0.0f) - camX) * zoom,
                     (((camY + storedH * invZoom())) - camY) * zoom, 15.6f,
                     Vector4<float>(1.0f, 1.0f, 1.0f, 1.0f), primitiveShader);
        }
    }

    if (showPixelGrid && zoom >= 4.0f) {
        // f->glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ZERO);
        rectVAO.bind();
        QList<QVector3D> verts;
        primitiveShader.use();
        primitiveShader.setValue("colour", QVector4D(1.0f, 1.0f, 1.0f, 1.0f));

        float camX = cam.pos.x;
        float camY = cam.pos.y;

        for (int y = camY; y < (camY + storedH) * (zoom < 1.0f ? invZoom() : 1.0f); ++y) {
            verts.append(QVector3D((camX - camX) * zoom, (y - camY) * zoom, 15.6f));
            verts.append(
                QVector3D((((camX + storedW * invZoom())) - camX) * zoom, (y - camY) * zoom, 15.6f));
        }

        for (int x = camX; x < (camX + storedW) * (zoom < 1.0f ? invZoom() : 1.0f); ++x) {
            verts.append(QVector3D((x + (zoom <= 1.0f ? 1.0f : 0.0f) - camX) * zoom,
                                   (camY - camY) * zoom, 15.6f));
            verts.append(QVector3D((x + (zoom <= 1.0f ? 1.0f : 0.0f) - camX) * zoom,
                                   (((camY + storedH * invZoom())) - camY) * zoom, 15.6f));
        }

        QVector3D *vertsPtr = new QVector3D[(uint)verts.count()];
        for (int v = 0; v < verts.count(); ++v) vertsPtr[v] = verts[v];

        QOpenGLVertexArrayObject gridVAO;
        gridVAO.create();
        gridVAO.bind();

        QOpenGLBuffer gridVBO;
        gridVBO.create();
        gridVBO.setUsagePattern(QOpenGLBuffer::StaticDraw);
        gridVBO.bind();
        gridVBO.allocate(vertsPtr, verts.count() * sizeof(QVector3D));
        primitiveShader.enableAttributeArray(0);
        primitiveShader.setAttributeBuffer(0, GL_FLOAT, 0, 3, 0);

        f->glDrawArrays(GL_LINES, 0, verts.count());

        delete[] vertsPtr;

        gridVAO.release();
        gridVBO.release();
    }
}

void SceneViewerv5::unloadScene()
{
    // QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    if (m_tilesetTexture) {
        m_tilesetTexture->destroy();
        delete m_tilesetTexture;
    }
    m_tilesetTexture = nullptr;
    tiles.clear();

    for (int o = 0; o < v5_SURFACE_MAX; ++o) {
        if (gfxSurface[o].scope == SCOPE_STAGE) {
            if (gfxSurface[o].texturePtr)
                delete gfxSurface[o].texturePtr;
            gfxSurface[o].texturePtr = nullptr;
            gfxSurface[o].scope      = SCOPE_NONE;
        }
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
    vertsPtr    = NULL;
    tVertsPtr   = NULL;
    sceneWidth  = 0;
    sceneHeight = 0;
}

void SceneViewerv5::processObjects()
{
    for (int i = 0; i < v5_DRAWLAYER_COUNT; ++i) {
        drawLayers[i].entries.clear();
    }

    sceneInfo.entitySlot = 0;
    int posX             = (int)(cam.pos.x * 65536.0f);
    int posY             = (int)(cam.pos.y * 65536.0f);
    int offsetX          = (storedW) << 16;
    int offsetY          = (storedH) << 16;
    for (int e = 0; e < entities.count(); ++e) {
        sceneInfo.entity = entities[e].gameEntity;
        if (entities[e].type) {
            switch (sceneInfo.entity->active) {
                case ACTIVE_NEVER:
                case ACTIVE_PAUSED: sceneInfo.entity->inBounds = false; break;
                case ACTIVE_ALWAYS:
                case ACTIVE_NORMAL: sceneInfo.entity->inBounds = true; break;
                case ACTIVE_BOUNDS:
                    sceneInfo.entity->inBounds = false;
                    {
                        int sx = abs(sceneInfo.entity->position.x - posX);
                        int sy = abs(sceneInfo.entity->position.y - posY);
                        if (sx <= sceneInfo.entity->updateRange.x + offsetX
                            && sy <= sceneInfo.entity->updateRange.y + offsetY) {
                            sceneInfo.entity->inBounds = true;
                            break;
                        }
                    }
                    break;
                case ACTIVE_XBOUNDS:
                    sceneInfo.entity->inBounds = false;
                    {
                        int sx = abs(sceneInfo.entity->position.x - posX);
                        if (sx <= sceneInfo.entity->updateRange.x + offsetX) {
                            sceneInfo.entity->inBounds = true;
                            break;
                        }
                    }
                    break;
                case ACTIVE_YBOUNDS:
                    sceneInfo.entity->inBounds = false;
                    {
                        int sy = abs(sceneInfo.entity->position.y - posY);
                        if (sy <= sceneInfo.entity->updateRange.y + offsetY) {
                            sceneInfo.entity->inBounds = true;
                            break;
                        }
                    }
                    break;
                case ACTIVE_RBOUNDS:
                    sceneInfo.entity->inBounds = false;
                    {
                        int sx = (int)abs(sceneInfo.entity->position.x - posX) >> 0x10;
                        int sy = (int)abs(sceneInfo.entity->position.y - posY) >> 0x10;

                        if (sx * sx + sy * sy <= sceneInfo.entity->updateRange.x + offsetX) {
                            sceneInfo.entity->inBounds = true;
                            break;
                        }
                    }
                    break;
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
        if (sceneInfo.entity->inBounds && sceneInfo.entity->interaction) {
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
        entities[e].gameEntity->activeScreens = 0;
    }
}

void SceneViewerv5::callGameEvent(GameObjectInfo *info, byte eventID, SceneEntity *entity)
{
    if (!info)
        return;

    switch (eventID) {
        default: break;
        case EVENT_LOAD:
            if (info->editorLoad)
                info->editorLoad();
            break;
        case EVENT_CREATE:
            sceneInfo.entity     = entity->gameEntity;
            sceneInfo.entitySlot = entity->slotID;
            if (info->create)
                info->create(NULL);
            sceneInfo.entity     = NULL;
            sceneInfo.entitySlot = 0;
            break;
        case EVENT_UPDATE:
            // TODO: that
            sceneInfo.entity     = entity->gameEntity;
            sceneInfo.entitySlot = entity->slotID;
            if (info->update)
                info->update();
            sceneInfo.entity     = NULL;
            sceneInfo.entitySlot = 0;
            break;
        case EVENT_DRAW:
            sceneInfo.entity     = entity->gameEntity;
            sceneInfo.entitySlot = entity->slotID;
            if (info->editorDraw)
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
    qDebug() << "Widget Using OpenGL " + QString::number(fmt.majorVersion()) + "."
                    + QString::number(fmt.minorVersion());

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

    qDebug() << "OpenGL Details";
    qDebug() << "Vendor:       " + vendorStr;
    qDebug() << "Renderer:     " + rendererStr;
    qDebug() << "Version:      " + versionStr;
    qDebug() << "GLSL version: " + sdrVersionStr;
    qDebug() << "Extensions:   " + extensionsStr;
    qDebug() << (QOpenGLContext::currentContext()->isOpenGLES() ? "Using OpenGLES" : "Using OpenGL");
    qDebug() << (QOpenGLContext::currentContext()->isValid() ? "Is valid" : "Not valid");

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

    // Release (unbind) all
    rectVAO.release();
    vVBO2D.release();
}

void SceneViewerv5::resizeGL(int w, int h)
{
    storedW             = w;
    storedH             = h;
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glViewport(0, 0, w, h);

    // m_sbHorizontal->setMaximum((m_scene.m_sceneConfig.m_camBounds.w * 0x10) - m_storedW);
    // m_sbVertical->setMaximum((m_scene.m_sceneConfig.m_camBounds.h * 0x10) - m_storedH);
}

void SceneViewerv5::paintGL()
{
    // Draw the scene:
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    f->glClearColor(bgColour.r / 255.0f, bgColour.g / 255.0f, bgColour.b / 255.0f, 1.0f);
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    processObjects();

    drawScene();
}

int SceneViewerv5::addGraphicsFile(char *sheetPath, int sheetID, byte scope)
{
    if (sheetID >= 0 && sheetID < v5_SURFACE_MAX) {
        QImage sheet(sheetPath);
        gfxSurface[sheetID].name = QString(sheetPath);
        Utils::getHashInt(sheetPath, gfxSurface[sheetID].hash);
        gfxSurface[sheetID].texturePtr = createTexture(sheet);
        gfxSurface[sheetID].scope      = scope;
        gfxSurface[sheetID].width      = sheet.width();
        gfxSurface[sheetID].height     = sheet.height();
        return sheetID;
    }
    return -1;
}

void SceneViewerv5::removeGraphicsFile(char *sheetPath, int slot)
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
    float w = m_tilesetTexture->width(), h = m_tilesetTexture->height();

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

void SceneViewerv5::drawSpriteFlipped(float XPos, float YPos, int width, int height, int sprX, int sprY,
                                      int direction, int inkEffect, int alpha, int sheetID)
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

    // Draw Sprite
    float xpos = XPos;
    float ypos = YPos;
    float zpos = incZ();

    if (sheetID != prevSprite) {
        gfxSurface[sheetID].texturePtr->bind();
        prevSprite = sheetID;
    }
    float w = gfxSurface[sheetID].texturePtr->width(), h = gfxSurface[sheetID].texturePtr->height();

    Rect<int> check = Rect<int>();
    check.x         = (int)(xpos + (float)w) * zoom;
    check.y         = (int)(ypos + (float)h) * zoom;
    check.w         = (int)(xpos - (w / 2.0f)) * zoom;
    check.h         = (int)(ypos - (h / 2.0f)) * zoom;
    if (check.x < 0 || check.y < 0 || check.w >= storedW || check.h >= storedH || !sheetID) {
        return;
    }

    spriteShader.use();
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

    QVector2D *texCoords = nullptr;
    switch (direction) {
        case FLIP_NONE:
        default: {
            QVector2D tc[] = {
                QVector2D(tx, ty),           QVector2D(tx + tw, ty), QVector2D(tx + tw, ty + th),
                QVector2D(tx + tw, ty + th), QVector2D(tx, ty + th), QVector2D(tx, ty),
            };
            texCoords = tc;
            break;
        }
        case FLIP_X: {
            QVector2D tc[] = {
                QVector2D(tx + tw, ty), QVector2D(tx, ty),           QVector2D(tx, ty + th),
                QVector2D(tx, ty + th), QVector2D(tx + tw, ty + th), QVector2D(tx + tw, ty),
            };
            texCoords = tc;
            break;
        }
        case FLIP_Y: {
            QVector2D tc[] = {
                QVector2D(tx, ty + th), QVector2D(tx + tw, ty + th), QVector2D(tx + tw, ty),
                QVector2D(tx + tw, ty), QVector2D(tx, ty),           QVector2D(tx, ty + th),
            };
            texCoords = tc;
            break;
        }
        case FLIP_XY: {
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
    matModel.scale(width * zoom, height * zoom, 1.0f);

    matModel.translate((xpos + (width / 2)) / (float)width, (ypos + (height / 2)) / (float)height,
                       zpos);
    spriteShader.setValue("model", matModel);

    f->glDrawArrays(GL_TRIANGLES, 0, 6);
    validDraw = true;
}

void SceneViewerv5::drawSpriteRotozoom(float XPos, float YPos, int pivotX, int pivotY, int width,
                                       int height, int sprX, int sprY, int scaleX, int scaleY,
                                       int direction, short rotation, int inkEffect, int alpha,
                                       int sheetID)
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

    // XPos += pivotX;
    // YPos += pivotY;

    // Draw Sprite
    float xpos = XPos;
    float ypos = YPos;
    float zpos = incZ();

    if (sheetID != prevSprite) {
        gfxSurface[sheetID].texturePtr->bind();
        prevSprite = sheetID;
    }
    float w = gfxSurface[sheetID].texturePtr->width(), h = gfxSurface[sheetID].texturePtr->height();

    Rect<int> check = Rect<int>();
    check.x         = (int)(xpos + (float)w) * zoom;
    check.y         = (int)(ypos + (float)h) * zoom;
    check.w         = (int)(xpos - (w / 2.0f)) * zoom;
    check.h         = (int)(ypos - (h / 2.0f)) * zoom;
    if (check.x < 0 || check.y < 0 || check.w >= storedW || check.h >= storedH || !sheetID) {
        return;
    }

    spriteShader.use();
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

    QVector2D *texCoords = nullptr;
    switch (direction) {
        case FLIP_NONE:
        default: {
            QVector2D tc[] = {
                QVector2D(tx, ty),           QVector2D(tx + tw, ty), QVector2D(tx + tw, ty + th),
                QVector2D(tx + tw, ty + th), QVector2D(tx, ty + th), QVector2D(tx, ty),
            };
            texCoords = tc;
            break;
        }
        case FLIP_X: {
            QVector2D tc[] = {
                QVector2D(tx + tw, ty), QVector2D(tx, ty),           QVector2D(tx, ty + th),
                QVector2D(tx, ty + th), QVector2D(tx + tw, ty + th), QVector2D(tx + tw, ty),
            };
            texCoords = tc;
            break;
        }
        case FLIP_Y: {
            QVector2D tc[] = {
                QVector2D(tx, ty + th), QVector2D(tx + tw, ty + th), QVector2D(tx + tw, ty),
                QVector2D(tx + tw, ty), QVector2D(tx, ty),           QVector2D(tx, ty + th),
            };
            texCoords = tc;
            break;
        }
        case FLIP_XY: {
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
    float scalefX = (512.0f / scaleX);
    float scalefY = (512.0f / scaleY);
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
    validDraw = true;
}

void SceneViewerv5::drawLine(float x1, float y1, float z1, float x2, float y2, float z2,
                             Vector4<float> colour, Shader &shader)
{
    shader.use();
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

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

    validDraw = true;
}

void SceneViewerv5::drawRect(float x, float y, float z, float w, float h, Vector4<float> colour,
                             Shader &shader, bool outline)
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    shader.use();
    if (outline) {
        // top
        drawLine(x, y, z, x + w, y, z, colour, shader);
        // bottom
        drawLine(x, y + h, z, x + w, y + h, z, colour, shader);
        // left
        drawLine(x, y, z, x, y + h, z, colour, shader);
        // right
        drawLine(x + w, y, z, x + w, y + h, z, colour, shader);

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
