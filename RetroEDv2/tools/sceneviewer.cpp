#include "includes.hpp"

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

    // QSurfaceFormat format;
    // format.setDepthBufferSize(24);
    // format.setStencilBufferSize(8);
    // format.setMajorVersion(3);
    // format.setMinorVersion(2);
    // format.setProfile(QSurfaceFormat::CoreProfile);
    // QSurfaceFormat::setDefaultFormat(format);

    this->setFocusPolicy(Qt::WheelFocus);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, QOverload<>::of(&SceneViewer::updateScene));
    timer->start(1000.0f / 60.0f);
}

SceneViewer::~SceneViewer()
{
    unloadScene();

    m_screenVAO.destroy();
    m_rectVAO.destroy();
}

void SceneViewer::loadScene(QString path, byte ver)
{
    // unloading
    unloadScene();

    // loading
    QString pth      = path;
    QString basePath = pth.replace(QFileInfo(pth).fileName(), "");

    m_scene.read(ver, path);

    if (ver != ENGINE_v1) {
        m_background.read(ver, basePath + "Backgrounds.bin");
        m_chunkset.read(ver, basePath + "128x128Tiles.bin");
        m_tileconfig.read(basePath + "CollisionMasks.bin");
        m_stageconfig.read(ver, basePath + "StageConfig.bin");
    }
    else {
        m_background.read(ver, basePath + "ZoneBG.map");
        m_chunkset.read(ver, basePath + "Zone.til");
        m_tileconfig.read(basePath + "Zone.tcf");
        m_stageconfig.read(ver, basePath + "Zone.zcf");
    }

    // Always have 8 layers, even if some have w/h of 0
    for (int l = m_background.m_layers.count(); l < 8; ++l)
        m_background.m_layers.append(FormatHelpers::Background::Layer());

    if (ver != ENGINE_v1) {
        m_scene.m_objectTypeNames.clear();

        if (m_stageconfig.m_loadGlobalScripts) {
            if (ver == ENGINE_v2)
                m_scene.m_objectTypeNames.append("Player");

            for (FormatHelpers::Gameconfig::ObjectInfo &obj : m_gameconfig.m_objects) {
                m_scene.m_objectTypeNames.append(obj.m_name);
            }
        }

        for (FormatHelpers::Stageconfig::ObjectInfo &obj : m_stageconfig.m_objects) {
            m_scene.m_objectTypeNames.append(obj.m_name);
        }
    }
    else {
        m_scene.m_objectTypeNames.clear();

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
            "StarPost",          // 9
            "PlaneSwitch A",     // 10
            "PlaneSwitch B",     // 11
            "Unknown (ID 12)",   // 12
            "Unknown (ID 13)",   // 13
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
            m_scene.m_objectTypeNames.append(obj);
        }

        for (FormatHelpers::Stageconfig::ObjectInfo &obj : m_stageconfig.m_objects) {
            m_scene.m_objectTypeNames.append(obj.m_name);
        }
    }

    if (ver == ENGINE_v1) {
        if (QFile::exists(basePath + "Zone.gfx")) {
            // setup tileset texture from png
            RSDKv1::GFX gfx(basePath + "Zone.gfx");
            QImage tileset = gfx.exportImage();
            for (int i = 0; i < 0x400; ++i) {
                int tx         = ((i % (tileset.width() / 0x10)) * 0x10);
                int ty         = ((i / (tileset.width() / 0x10)) * 0x10);
                QImage tileTex = tileset.copy(tx, ty, 0x10, 0x10);

                m_tiles.append(tileTex);
            }

            for (FormatHelpers::Chunks::Chunk &c : m_chunkset.m_chunks) {
                QImage img = c.getImage(m_tiles);
                m_chunks.append(img);
                m_chunkTextures.append(createTexture(img));
            }
        }
    }
    else {
        if (QFile::exists(basePath + "16x16Tiles.gif")) {
            // setup tileset texture from png
            QImage tileset(basePath + "16x16Tiles.gif");
            for (int i = 0; i < 0x400; ++i) {
                int tx         = ((i % (tileset.width() / 0x10)) * 0x10);
                int ty         = ((i / (tileset.width() / 0x10)) * 0x10);
                QImage tileTex = tileset.copy(tx, ty, 0x10, 0x10);

                m_tiles.append(tileTex);
            }

            for (FormatHelpers::Chunks::Chunk &c : m_chunkset.m_chunks) {
                QImage img = c.getImage(m_tiles);
                m_chunks.append(img);
                m_chunkTextures.append(createTexture(img));
            }
        }
    }

    // objects
    m_objectSprites.clear();
    {
        TextureInfo tex;
        tex.m_name    = ":/icons/missing.png";
        m_missingObj  = QImage(tex.m_name);
        tex.m_texture = createTexture(m_missingObj);
        m_objectSprites.append(tex);
    }

    m_rsPlayerSprite = createTexture(QImage(":/icons/player_v1.png"));
}

void SceneViewer::updateScene()
{
    this->repaint();

    if (m_statusLabel) {
        int mx = (int)((m_mousePos.x * invZoom()) + m_cam.m_position.x);
        int my = (int)((m_mousePos.y * invZoom()) + m_cam.m_position.y);
        m_statusLabel->setText(
            QString("Zoom: %1%, Mouse Position: (%2, %3), Chunk Position: (%4, %5), Selected Chunk: "
                    "%6, Selected Layer: %7 (%8), Selected Object: %9")
                .arg(m_zoom * 100)
                .arg(mx)
                .arg(my)
                .arg((int)mx / 0x80)
                .arg((int)my / 0x80)
                .arg(m_selectedChunk)
                .arg(m_selectedLayer)
                .arg(m_selectedLayer >= 0 && 9 ? m_selectedLayer == 0
                                                     ? "Foreground"
                                                     : "Background " + QString::number(m_selectedLayer)
                                               : "")
                .arg(m_selectedObject >= 0 && m_selectedObject < m_scene.m_objects.count()
                         ? m_scene.m_objectTypeNames[m_selectedObject]
                         : ""));
    }
}

void SceneViewer::drawScene()
{
    if (m_chunkTextures.count() <= 0)
        return;
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    f->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    f->glBlendEquation(GL_FUNC_ADD);

    // pre-render
    if ((m_cam.m_position.x * m_zoom) < 0 * m_zoom)
        m_cam.m_position.x = (0 * m_zoom);

    if ((m_cam.m_position.y * m_zoom) < 0 * m_zoom)
        m_cam.m_position.y = (0 * m_zoom);

    if ((m_cam.m_position.x * m_zoom) + m_storedW > (m_scene.m_width * 0x80) * m_zoom)
        m_cam.m_position.x = ((m_scene.m_width * 0x80) - (m_storedW * invZoom()));

    if ((m_cam.m_position.y * m_zoom) + m_storedH > (m_scene.m_height * 0x80) * m_zoom)
        m_cam.m_position.y = ((m_scene.m_height * 0x80) - (m_storedH * invZoom()));

    // draw bg colours
    m_primitiveShader.use();
    m_primitiveShader.setValue("colour",
                               QVector4D(m_altBGColour.m_r / 255.0f, m_altBGColour.m_g / 255.0f,
                                         m_altBGColour.m_b / 255.0f, 1.0f));
    m_primitiveShader.setValue("projection", getProjectionMatrix());
    m_primitiveShader.setValue("view", QMatrix4x4());
    m_rectVAO.bind();

    int bgOffsetY = 0x80;
    bgOffsetY -= (int)m_cam.m_position.y % 0x200;
    for (int y = bgOffsetY; y < (m_storedH + 0x80) * (m_zoom < 1.0f ? invZoom() : 1.0f); y += 0x100) {
        int bgOffsetX = (((y - bgOffsetY) % 0x200 == 0) ? 0x100 : 0x00);
        bgOffsetX += 0x80;
        bgOffsetX -= (int)m_cam.m_position.x % 0x200;
        for (int x = bgOffsetX; x < (m_storedW + 0x80) * (m_zoom < 1.0f ? invZoom() : 1.0f);
             x += 0x200) {
            QMatrix4x4 matModel;
            matModel.scale(0x100 * m_zoom, 0x100 * m_zoom, 1.0f);
            matModel.translate(x / 256.0f, y / 256.0f, -15.0f);
            m_primitiveShader.setValue("model", matModel);

            f->glDrawArrays(GL_TRIANGLES, 0, 6);
        }
    }

    m_spriteShader.use();
    m_spriteShader.setValue("useAlpha", false);
    m_spriteShader.setValue("alpha", 1.0f);
    m_spriteShader.setValue(
        "transparentColour",
        (m_gameType != ENGINE_v1 ? QVector3D(1.0f, 0.0f, 1.0f) : QVector3D(0.0f, 0.0f, 0.0f)));

    QMatrix4x4 matWorld;
    QMatrix4x4 matView;
    m_spriteShader.setValue("projection", matWorld = getProjectionMatrix());
    m_spriteShader.setValue("view", matView = QMatrix4x4());
    f->glActiveTexture(GL_TEXTURE0);

    m_rectVAO.bind();

    int prevChunk = -1;
    Vector3<float> camOffset(0.0f, 0.0f, 0.0f);

    QVector4D pixelSolidityClrs[5] = { QVector4D(1.0f, 1.0f, 1.0f, 1.0f),
                                       QVector4D(1.0f, 1.0f, 0.0f, 1.0f),
                                       QVector4D(1.0f, 0.0f, 0.0f, 1.0f),
                                       QVector4D(0.0f, 0.0f, 0.0f, 0.0f),
                                       QVector4D(0.0f, 0.0f, 1.0f, 1.0f) };
    bool m_showCLayers[2]          = { m_showPlaneA, m_showPlaneB };

    for (int l = 8; l >= 0; --l) {
        // TILE LAYERS
        QList<QList<ushort>> layout = m_scene.m_layout;
        int width                   = m_scene.m_width;
        int height                  = m_scene.m_height;

        if (l > 0) {
            layout = m_background.m_layers[l - 1].m_layout;
            width  = m_background.m_layers[l - 1].m_width;
            height = m_background.m_layers[l - 1].m_height;
        }

        m_spriteShader.use();
        m_spriteShader.setValue("useAlpha", false);
        m_spriteShader.setValue("alpha", 1.0f);
        m_spriteShader.setValue(
            "transparentColour",
            (m_gameType != ENGINE_v1 ? QVector3D(1.0f, 0.0f, 1.0f) : QVector3D(0.0f, 0.0f, 0.0f)));
        m_rectVAO.bind();
        // manage properties
        camOffset = Vector3<float>(0.0f, 0.0f, 0.0f);

        if (m_selectedLayer >= 0) {
            m_spriteShader.setValue("useAlpha", true);
            if (m_selectedLayer == l) {
                m_spriteShader.setValue("alpha", 1.0f);
            }
            else {
                m_spriteShader.setValue("alpha", 0.5f);
            }
        }

        // m_tilesets[layer.m_tileSheet]->bind();

        // draw
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                ushort chunkID = layout[y][x];
                if (chunkID != 0x0) {
                    float xpos = (x * 0x80) - (m_cam.m_position.x + camOffset.x);
                    float ypos = (y * 0x80) - (m_cam.m_position.y + camOffset.y);
                    float zpos = (8 - l);

                    Rect<int> check = Rect<int>();
                    check.x         = (int)((xpos + 0x80) * m_zoom);
                    check.y         = (int)((ypos + 0x80) * m_zoom);
                    check.w         = (int)((xpos - (0x80 / 2)) * m_zoom);
                    check.h         = (int)((ypos - (0x80 / 2)) * m_zoom);

                    if (check.x < 0 || check.y < 0 || check.w >= m_storedW || check.h >= m_storedH) {
                        continue;
                    }

                    if (chunkID != prevChunk) {
                        m_chunkTextures[chunkID]->bind();
                        prevChunk = chunkID;
                    }

                    QMatrix4x4 matModel;
                    xpos += (0x80 / 2);
                    ypos += (0x80 / 2);
                    matModel.scale(0x80 * m_zoom, 0x80 * m_zoom, 1.0f);

                    matModel.translate(xpos / 0x80, ypos / 0x80, zpos);
                    m_spriteShader.setValue("model", matModel);

                    f->glDrawArrays(GL_TRIANGLES, 0, 6);

                    // update dumb offsets
                    xpos -= (0x80 / 2);
                    ypos -= (0x80 / 2);
                    xpos += 0.5; // 1.0 / 2
                    float yStore = ypos;
                    // draw pixel collision
                    for (int c = 0; c < 2; ++c) {
                        if (m_showCLayers[c]) {
                            for (int ty = 0; ty < 8; ++ty) {
                                for (int tx = 0; tx < 8; ++tx) {
                                    FormatHelpers::Chunks::Tile &tile =
                                        m_chunkset.m_chunks[chunkID].m_tiles[ty][tx];

                                    byte solidity = 0;
                                    RSDKv4::Tileconfig::CollisionMask &cmask =
                                        m_tileconfig.m_collisionPaths[c][tile.m_tileIndex];

                                    solidity = !c ? tile.m_solidityA : tile.m_solidityB;

                                    if (solidity == 3)
                                        continue;
                                    if (solidity > 3)
                                        solidity = 4;

                                    for (byte cx = 0; cx < 16; ++cx) {
                                        int hm = cx;
                                        if (Utils::getBit(tile.m_direction, 0))
                                            hm = 15 - cx;

                                        if (!cmask.m_collision[hm].m_solid)
                                            continue;

                                        byte cy = cmask.m_collision[hm].m_height;
                                        byte ch = 16 - cy;
                                        if (Utils::getBit(tile.m_direction, 1) && !cmask.m_isCeiling) {
                                            cy = 0;
                                            ch = 16 - cmask.m_collision[hm].m_height;
                                        }
                                        else if (!Utils::getBit(tile.m_direction, 1)
                                                 && cmask.m_isCeiling) {
                                            cy = 0;
                                            ch = cmask.m_collision[hm].m_height + 1;
                                        }
                                        else if (Utils::getBit(tile.m_direction, 1)
                                                 && cmask.m_isCeiling) {
                                            cy = 15 - cmask.m_collision[hm].m_height;
                                            ch = cmask.m_collision[hm].m_height + 1;
                                        }

                                        ypos = yStore + (ch / 2.0);

                                        drawRect((xpos + (tx * 16) + cx) * m_zoom,
                                                 (ypos + (ty * 16) + cy) * m_zoom, 15.45, 1 * m_zoom,
                                                 ch * m_zoom, pixelSolidityClrs[solidity],
                                                 m_primitiveShader);
                                    }
                                }
                            }
                            m_spriteShader.use();
                            m_rectVAO.bind();
                        }
                    }
                }
            }
        }

        // PARALLAX
        if (l == m_selectedLayer && l > 0) {
            if (m_background.m_layers[l - 1].m_behaviour == 1
                || m_background.m_layers[l - 1].m_behaviour == 2) {
                m_primitiveShader.use();
                m_primitiveShader.setValue("projection", getProjectionMatrix());
                m_primitiveShader.setValue("view", QMatrix4x4());
                m_primitiveShader.setValue("useAlpha", false);
                m_primitiveShader.setValue("alpha", 1.0f);
                QMatrix4x4 matModel;
                m_primitiveShader.setValue("model", matModel);

                QOpenGLVertexArrayObject colVAO;
                colVAO.create();
                colVAO.bind();

                QList<QVector3D> verts;
                if (m_showParallax) {
                    int id = 0;
                    for (FormatHelpers::Background::ScrollIndexInfo &info :
                         m_background.m_layers[l - 1].m_scrollInfos) {
                        bool isSelected = m_selectedScrollInfo == id;

                        Vector4<float> clr(1.0f, 1.0f, 0.0f, 1.0f);
                        if (isSelected)
                            clr = Vector4<float>(0.0f, 0.0f, 1.0f, 1.0f);
                        float zpos = (isSelected ? 15.55f : 15.5f);

                        if (m_background.m_layers[l - 1].m_behaviour == 1) {
                            int w = (width * 0x80) * m_zoom;
                            drawLine(0.0f * m_zoom, (info.m_startLine - m_cam.m_position.y) * m_zoom,
                                     zpos, (w - m_cam.m_position.x) * m_zoom,
                                     (info.m_startLine - m_cam.m_position.y) * m_zoom, zpos, clr,
                                     m_primitiveShader);

                            drawLine(0.0f * m_zoom,
                                     ((info.m_startLine + info.m_length) - m_cam.m_position.y) * m_zoom,
                                     zpos, (w - m_cam.m_position.x) * m_zoom,
                                     ((info.m_startLine + info.m_length) - m_cam.m_position.y) * m_zoom,
                                     zpos, clr, m_primitiveShader);
                        }
                        else if (m_background.m_layers[l - 1].m_behaviour == 2) {
                            int h = (height * 0x80) * m_zoom;
                            drawLine((info.m_startLine - m_cam.m_position.x) * m_zoom, 0.0f * m_zoom,
                                     zpos, (info.m_startLine - m_cam.m_position.x) * m_zoom,
                                     (h - m_cam.m_position.y) * m_zoom, zpos, clr, m_primitiveShader);

                            drawLine(((info.m_startLine + info.m_length) - m_cam.m_position.x) * m_zoom,
                                     0.0f * m_zoom, zpos,
                                     ((info.m_startLine + info.m_length) - m_cam.m_position.x) * m_zoom,
                                     (h - m_cam.m_position.y) * m_zoom, zpos, clr, m_primitiveShader);
                        }

                        ++id;
                    }
                }
            }
        }
    }

    // ENTITIES
    m_prevSprite = -1;
    m_spriteShader.use();
    m_rectVAO.bind();
    m_spriteShader.setValue("flipX", false);
    m_spriteShader.setValue("flipY", false);
    m_spriteShader.setValue("useAlpha", false);
    m_spriteShader.setValue("alpha", 1.0f);
    m_compilerv3.xScrollOffset = m_cam.m_position.x;
    m_compilerv3.yScrollOffset = m_cam.m_position.y;
    // m_compilerv4.xScrollOffset = m_cam.m_position.x;
    // m_compilerv4.yScrollOffset = m_cam.m_position.y;
    for (int o = 0; o < m_scene.m_objects.count(); ++o) {
        switch (m_gameType) {
            case ENGINE_v1: break;
            case ENGINE_v2: break;
            case ENGINE_v3: {
                auto &curObj = m_compilerv3.m_objectScriptList[m_scene.m_objects[o].m_type];

                if (curObj.subRSDKDraw.m_scriptCodePtr != SCRIPTDATA_COUNT - 1) {
                    m_compilerv3.m_objectLoop = o;
                    m_compilerv3.processScript(curObj.subRSDKDraw.m_scriptCodePtr,
                                               curObj.subRSDKDraw.m_jumpTablePtr,
                                               Compilerv3::SUB_RSDKDRAW);
                    continue;
                }
                break;
            }
            case ENGINE_v4: {
                // auto &curObj = m_compilerv3.m_objectScriptList[m_scene.m_objects[o].m_type];
                //
                // if (curObj.subRSDKDraw.m_scriptCodePtr != SCRIPTDATA_COUNT - 1) {
                //    m_compilerv3.m_objectLoop = o;
                //    m_compilerv3.processScript(curObj.subRSDKDraw.m_scriptCodePtr,
                //                               curObj.subRSDKDraw.m_jumpTablePtr,
                //                               Compilerv3::SUB_RSDKDRAW);
                //    // continue;
                //}
                break;
            }
        }

        m_spriteShader.use();
        m_rectVAO.bind();
        // Draw Object
        float xpos = m_scene.m_objects[o].getX() - (m_cam.m_position.x);
        float ypos = m_scene.m_objects[o].getY() - (m_cam.m_position.y);
        float zpos = 9.0f;

        int w = m_objectSprites[0].m_texture->width(), h = m_objectSprites[0].m_texture->height();
        if (m_prevSprite) {
            m_objectSprites[0].m_texture->bind();
            m_prevSprite = 0;
        }

        Rect<int> check = Rect<int>();
        check.x         = (int)(xpos + (float)w) * m_zoom;
        check.y         = (int)(ypos + (float)h) * m_zoom;
        check.w         = (int)(xpos - (w / 2.0f)) * m_zoom;
        check.h         = (int)(ypos - (h / 2.0f)) * m_zoom;
        if (check.x < 0 || check.y < 0 || check.w >= m_storedW || check.h >= m_storedH) {
            continue;
        }

        QMatrix4x4 matModel;
        matModel.scale(w * m_zoom, h * m_zoom, 1.0f);

        matModel.translate(xpos / (float)w, ypos / (float)h, zpos);
        m_spriteShader.setValue("model", matModel);

        f->glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    m_spriteShader.use();
    m_rectVAO.bind();
    m_spriteShader.setValue("useAlpha", true);
    m_spriteShader.setValue("alpha", 0.75f);
    if (m_selectedChunk >= 0 && m_selectedLayer >= 0 && m_selecting && m_tool == TOOL_PENCIL) {
        float tx = m_tilePos.x;
        float ty = m_tilePos.y;

        tx *= invZoom();
        ty *= invZoom();

        float tx2 = tx + fmodf(m_cam.m_position.x, 0x80);
        float ty2 = ty + fmodf(m_cam.m_position.y, 0x80);

        // clip to grid
        tx -= fmodf(tx2, 0x80);
        ty -= fmodf(ty2, 0x80);

        // Draw Selected Tile Preview
        float xpos = tx + m_cam.m_position.x;
        float ypos = ty + m_cam.m_position.y;
        float zpos = 15.0f;

        xpos -= (m_cam.m_position.x + camOffset.x);
        ypos -= (m_cam.m_position.y + camOffset.y);

        m_chunkTextures[m_selectedChunk]->bind();

        QMatrix4x4 matModel;
        xpos += (0x80 / 2);
        ypos += (0x80 / 2);
        matModel.scale(0x80 * m_zoom, 0x80 * m_zoom, 1.0f);

        matModel.translate((xpos / 0x80), (ypos / 0x80), zpos);
        m_spriteShader.setValue("model", matModel);

        f->glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    // ENT PREVIEW
    m_spriteShader.use();
    m_rectVAO.bind();
    m_spriteShader.setValue("flipX", false);
    m_spriteShader.setValue("flipY", false);
    m_spriteShader.setValue("useAlpha", true);
    m_spriteShader.setValue("alpha", 0.75f);
    if (m_selectedObject >= 0 && m_selecting && m_tool == TOOL_ENTITY) {
        float ex = m_tilePos.x;
        float ey = m_tilePos.y;

        ex *= invZoom();
        ey *= invZoom();

        // Draw Selected Object Preview
        float xpos = ex; //+ m_cam.m_position.x;
        float ypos = ey; //+ m_cam.m_position.y;
        float zpos = 15.0f;

        int w = m_objectSprites[0].m_texture->width(), h = m_objectSprites[0].m_texture->height();
        m_objectSprites[0].m_texture->bind();

        QMatrix4x4 matModel;
        matModel.scale(w * m_zoom, h * m_zoom, 1.0f);

        matModel.translate(xpos / (float)w, ypos / (float)h, zpos);
        m_spriteShader.setValue("model", matModel);

        f->glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    m_spriteShader.setValue("useAlpha", false);
    m_spriteShader.setValue("alpha", 1.0f);

    // Draw Retro-Sonic Player spawn point
    if (m_gameType == ENGINE_v1) {
        float px = m_scene.m_playerXPos;
        float py = m_scene.m_playerYPos;

        px *= invZoom();
        py *= invZoom();

        // Draw Player Spawn Preview
        float xpos = px - m_cam.m_position.x;
        float ypos = py - m_cam.m_position.y;
        float zpos = 15.0f;

        int w = m_rsPlayerSprite->width(), h = m_rsPlayerSprite->height();
        m_rsPlayerSprite->bind();

        xpos += w / 2;
        ypos += h / 2;

        QMatrix4x4 matModel;
        matModel.scale(w * m_zoom, h * m_zoom, 1.0f);

        matModel.translate(xpos / (float)w, ypos / (float)h, zpos);
        m_spriteShader.setValue("model", matModel);

        f->glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    // Selected Ent Box
    m_rectVAO.bind();
    m_primitiveShader.use();
    m_primitiveShader.setValue("projection", getProjectionMatrix());
    m_primitiveShader.setValue("view", QMatrix4x4());
    m_primitiveShader.setValue("useAlpha", false);
    m_primitiveShader.setValue("alpha", 1.0f);
    QMatrix4x4 matModel;
    m_primitiveShader.setValue("model", matModel);
    if (m_selectedEntity >= 0) {
        FormatHelpers::Scene::Object &object = m_scene.m_objects[m_selectedEntity];
        int w = m_objectSprites[0].m_texture->width(), h = m_objectSprites[0].m_texture->height();
        m_objectSprites[0].m_texture->bind();

        drawRect(((object.getX() - m_cam.m_position.x) - (w / 2)) * m_zoom,
                 ((object.getY() - m_cam.m_position.y) - (h / 2)) * m_zoom, 15.7f, w * m_zoom,
                 h * m_zoom, Vector4<float>(1.0f, 1.0f, 1.0f, 1.0f), m_primitiveShader, true);
    }

    if (m_showTileGrid) {
        m_rectVAO.bind();

        float camX = m_cam.m_position.x;
        float camY = m_cam.m_position.y;

        for (int y = camY - ((int)camY % 0x80);
             y < (camY + m_storedH) * (m_zoom < 1.0f ? invZoom() : 1.0f); y += 0x80) {
            drawLine((camX - camX) * m_zoom, (y - camY) * m_zoom, 15.6f,
                     (((camX + m_storedW * invZoom())) - camX) * m_zoom, (y - camY) * m_zoom, 15.6f,
                     Vector4<float>(1.0f, 1.0f, 1.0f, 1.0f), m_primitiveShader);
        }

        for (int x = camX - ((int)camX % 0x80);
             x < (camX + m_storedW) * (m_zoom < 1.0f ? invZoom() : 1.0f); x += 0x80) {
            drawLine((x + (m_zoom <= 1.0f ? 1.0f : 0.0f) - camX) * m_zoom, (camY - camY) * m_zoom,
                     15.6f, (x + (m_zoom <= 1.0f ? 1.0f : 0.0f) - camX) * m_zoom,
                     (((camY + m_storedH * invZoom())) - camY) * m_zoom, 15.6f,
                     Vector4<float>(1.0f, 1.0f, 1.0f, 1.0f), m_primitiveShader);
        }
    }

    if (m_showPixelGrid && m_zoom >= 4.0f) {
        // f->glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ZERO);
        m_rectVAO.bind();
        QList<QVector3D> verts;
        m_primitiveShader.use();
        // m_primitiveShader.setValue("colour", QVector4D(1.0f, 1.0f, 1.0f, 0.25f)); // 0xFF999999
        m_primitiveShader.setValue("colour", QVector4D(1.0f, 1.0f, 1.0f, 1.0f)); // 0xFF999999

        float camX = m_cam.m_position.x;
        float camY = m_cam.m_position.y;

        for (int y = camY; y < (camY + m_storedH) * (m_zoom < 1.0f ? invZoom() : 1.0f); ++y) {
            verts.append(QVector3D((camX - camX) * m_zoom, (y - camY) * m_zoom, 15.6f));
            verts.append(QVector3D((((camX + m_storedW * invZoom())) - camX) * m_zoom,
                                   (y - camY) * m_zoom, 15.6f));
        }

        for (int x = camX; x < (camX + m_storedW) * (m_zoom < 1.0f ? invZoom() : 1.0f); ++x) {
            verts.append(QVector3D((x + (m_zoom <= 1.0f ? 1.0f : 0.0f) - camX) * m_zoom,
                                   (camY - camY) * m_zoom, 15.6f));
            verts.append(QVector3D((x + (m_zoom <= 1.0f ? 1.0f : 0.0f) - camX) * m_zoom,
                                   (((camY + m_storedH * invZoom())) - camY) * m_zoom, 15.6f));
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
        m_primitiveShader.enableAttributeArray(0);
        m_primitiveShader.setAttributeBuffer(0, GL_FLOAT, 0, 3, 0);

        f->glDrawArrays(GL_LINES, 0, verts.count());

        delete[] vertsPtr;

        gridVAO.release();
        gridVBO.release();
    }
}

void SceneViewer::unloadScene()
{
    // QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    for (int t = 0; t < m_chunkTextures.count(); ++t) {
        // how tf do I call these functions since the context is null
        m_chunkTextures[t]->destroy();
        delete m_chunkTextures[t];
    }
    m_chunkTextures.clear();
    m_tiles.clear();

    m_chunkTextures.clear();
    m_tiles.clear();

    m_chunks.clear();

    for (int o = 0; o < m_objectSprites.count(); ++o) {
        m_objectSprites[o].m_texture->destroy();
        delete m_objectSprites[o].m_texture;
        m_objectSprites[o].m_name = "";
    }
    m_objectSprites.clear();

    if (m_rsPlayerSprite) {
        m_rsPlayerSprite->destroy();
        delete m_rsPlayerSprite;
    }

    m_cam                = SceneCamera();
    m_selectedChunk      = -1;
    m_selectedEntity     = -1;
    m_selectedLayer      = -1;
    m_selectedScrollInfo = -1;
    m_selectedObject     = -1;
    m_selecting          = false;
}

void SceneViewer::initializeGL()
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
    f->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_primitiveShader.loadShader(":/shaders/primitive.glv", QOpenGLShader::Vertex);
    m_primitiveShader.loadShader(":/shaders/primitive.glf", QOpenGLShader::Fragment);
    m_primitiveShader.link();
    m_primitiveShader.use();

    m_spriteShader.loadShader(":/shaders/sprite.glv", QOpenGLShader::Vertex);
    m_spriteShader.loadShader(":/shaders/sprite.glf", QOpenGLShader::Fragment);
    m_spriteShader.link();
    m_spriteShader.use();

    m_rectVAO.create();
    m_rectVAO.bind();

    QOpenGLBuffer vVBO2D;
    vVBO2D.create();
    vVBO2D.setUsagePattern(QOpenGLBuffer::StaticDraw);
    vVBO2D.bind();
    vVBO2D.allocate(rectVertices, 6 * sizeof(QVector3D));
    m_spriteShader.enableAttributeArray(0);
    m_spriteShader.setAttributeBuffer(0, GL_FLOAT, 0, 3, 0);

    QOpenGLBuffer tVBO2D;
    tVBO2D.create();
    tVBO2D.setUsagePattern(QOpenGLBuffer::StaticDraw);
    tVBO2D.bind();
    tVBO2D.allocate(rectTexCoords, 6 * sizeof(QVector2D));
    m_spriteShader.enableAttributeArray(1);
    m_spriteShader.setAttributeBuffer(1, GL_FLOAT, 0, 2, 0);

    // Release (unbind) all
    m_rectVAO.release();
    vVBO2D.release();
}

void SceneViewer::resizeGL(int w, int h)
{
    m_storedW           = w;
    m_storedH           = h;
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glViewport(0, 0, w, h);

    // m_sbHorizontal->setMaximum((m_scene.m_sceneConfig.m_camBounds.w * 0x80) - m_storedW);
    // m_sbVertical->setMaximum((m_scene.m_sceneConfig.m_camBounds.h * 0x80) - m_storedH);
}

void SceneViewer::paintGL()
{
    // Draw the scene:
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    f->glClearColor(m_bgColour.m_r / 255.0f, m_bgColour.m_g / 255.0f, m_bgColour.m_b / 255.0f, 1.0f);
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    drawScene();
}

int SceneViewer::addGraphicsFile(char *sheetPath)
{
    QString path = m_dataPath + "/Sprites/" + sheetPath;
    if (!QFile::exists(path))
        return 0;

    for (int i = 1; i < m_objectSprites.count(); ++i) {
        if (QString(sheetPath) == m_objectSprites[i].m_name)
            return i;
    }

    int sheetID = -1;
    for (int i = 1; i < m_objectSprites.count(); ++i) {
        if (m_objectSprites[i].m_name == "")
            sheetID = i;
    }

    if (sheetID >= 0) {
        QImage sheet(path);
        TextureInfo tex;
        tex.m_name               = QString(sheetPath);
        tex.m_texture            = createTexture(sheet);
        m_objectSprites[sheetID] = tex;
        return sheetID;
    }
    else {
        QImage sheet(path);
        int cnt = m_objectSprites.count();
        TextureInfo tex;
        tex.m_name    = QString(sheetPath);
        tex.m_texture = createTexture(sheet);
        m_objectSprites.append(tex);
        return cnt;
    }
}

void SceneViewer::removeGraphicsFile(char *sheetPath, int slot)
{
    if (slot >= 0) {
        m_objectSprites[slot].m_texture->destroy();
        delete m_objectSprites[slot].m_texture;
        m_objectSprites[slot].m_name = "";
    }
    else {
        for (int i = 1; i < m_objectSprites.count(); ++i) {
            if (QString(sheetPath) == m_objectSprites[i].m_name) {
                m_objectSprites[slot].m_texture->destroy();
                delete m_objectSprites[slot].m_texture;
                m_objectSprites[slot].m_name = "";
            }
        }
    }
}

void SceneViewer::drawSprite(int XPos, int YPos, int width, int height, int sprX, int sprY, int sheetID)
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    // Draw Sprite
    float xpos = XPos - m_cam.m_position.x;
    float ypos = YPos - m_cam.m_position.y;
    float zpos = 9.0f;

    if (sheetID != m_prevSprite) {
        m_objectSprites[sheetID].m_texture->bind();
        m_prevSprite = sheetID;
    }
    float w = m_objectSprites[sheetID].m_texture->width(),
          h = m_objectSprites[sheetID].m_texture->height();

    Rect<int> check = Rect<int>();
    check.x         = (int)(xpos + (float)w) * m_zoom;
    check.y         = (int)(ypos + (float)h) * m_zoom;
    check.w         = (int)(xpos - (w / 2.0f)) * m_zoom;
    check.h         = (int)(ypos - (h / 2.0f)) * m_zoom;
    if (check.x < 0 || check.y < 0 || check.w >= m_storedW || check.h >= m_storedH || !sheetID) {
        return;
    }

    m_spriteShader.use();
    m_spriteShader.setValue("flipX", false);
    m_spriteShader.setValue("flipY", false);
    m_spriteShader.setValue("useAlpha", false);
    m_spriteShader.setValue("alpha", 1.0f);
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
    m_spriteShader.enableAttributeArray(0);
    m_spriteShader.setAttributeBuffer(0, GL_FLOAT, 0, 3, 0);

    QOpenGLBuffer tVBO2D;
    tVBO2D.create();
    tVBO2D.setUsagePattern(QOpenGLBuffer::DynamicDraw);
    tVBO2D.bind();
    tVBO2D.allocate(texCoords, 6 * sizeof(QVector2D));
    m_spriteShader.enableAttributeArray(1);
    m_spriteShader.setAttributeBuffer(1, GL_FLOAT, 0, 2, 0);

    QMatrix4x4 matModel;
    matModel.scale(width * m_zoom, height * m_zoom, 1.0f);

    matModel.translate((xpos + (width / 2)) / (float)width, (ypos + (height / 2)) / (float)height,
                       zpos);
    m_spriteShader.setValue("model", matModel);

    f->glDrawArrays(GL_TRIANGLES, 0, 6);
}

void SceneViewer::drawSpriteFlipped(int XPos, int YPos, int width, int height, int sprX, int sprY,
                                    int direction, int sheetID)
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    // Draw Sprite
    float xpos = XPos - m_cam.m_position.x;
    float ypos = YPos - m_cam.m_position.y;
    float zpos = 9.0f;

    if (sheetID != m_prevSprite) {
        m_objectSprites[sheetID].m_texture->bind();
        m_prevSprite = sheetID;
    }
    float w = m_objectSprites[sheetID].m_texture->width(),
          h = m_objectSprites[sheetID].m_texture->height();

    Rect<int> check = Rect<int>();
    check.x         = (int)(xpos + (float)w) * m_zoom;
    check.y         = (int)(ypos + (float)h) * m_zoom;
    check.w         = (int)(xpos - (w / 2.0f)) * m_zoom;
    check.h         = (int)(ypos - (h / 2.0f)) * m_zoom;
    if (check.x < 0 || check.y < 0 || check.w >= m_storedW || check.h >= m_storedH || !sheetID) {
        return;
    }

    m_spriteShader.use();
    m_spriteShader.setValue("flipX", false);
    m_spriteShader.setValue("flipY", false);
    m_spriteShader.setValue("useAlpha", false);
    m_spriteShader.setValue("alpha", 1.0f);
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
    m_spriteShader.enableAttributeArray(0);
    m_spriteShader.setAttributeBuffer(0, GL_FLOAT, 0, 3, 0);

    QOpenGLBuffer tVBO2D;
    tVBO2D.create();
    tVBO2D.setUsagePattern(QOpenGLBuffer::DynamicDraw);
    tVBO2D.bind();
    tVBO2D.allocate(texCoords, 6 * sizeof(QVector2D));
    m_spriteShader.enableAttributeArray(1);
    m_spriteShader.setAttributeBuffer(1, GL_FLOAT, 0, 2, 0);

    QMatrix4x4 matModel;
    matModel.scale(width * m_zoom, height * m_zoom, 1.0f);

    matModel.translate((xpos + (width / 2)) / (float)width, (ypos + (height / 2)) / (float)height,
                       zpos);
    m_spriteShader.setValue("model", matModel);

    f->glDrawArrays(GL_TRIANGLES, 0, 6);
}

void SceneViewer::drawBlendedSprite(int XPos, int YPos, int width, int height, int sprX, int sprY,
                                    int sheetID)
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    // Draw Sprite
    float xpos = XPos - m_cam.m_position.x;
    float ypos = YPos - m_cam.m_position.y;
    float zpos = 9.0f;

    if (sheetID != m_prevSprite) {
        m_objectSprites[sheetID].m_texture->bind();
        m_prevSprite = sheetID;
    }
    float w = m_objectSprites[sheetID].m_texture->width(),
          h = m_objectSprites[sheetID].m_texture->height();

    Rect<int> check = Rect<int>();
    check.x         = (int)(xpos + (float)w) * m_zoom;
    check.y         = (int)(ypos + (float)h) * m_zoom;
    check.w         = (int)(xpos - (w / 2.0f)) * m_zoom;
    check.h         = (int)(ypos - (h / 2.0f)) * m_zoom;
    if (check.x < 0 || check.y < 0 || check.w >= m_storedW || check.h >= m_storedH || !sheetID) {
        return;
    }

    m_spriteShader.use();
    m_spriteShader.setValue("flipX", false);
    m_spriteShader.setValue("flipY", false);
    m_spriteShader.setValue("useAlpha", true);
    m_spriteShader.setValue("alpha", 0.5f);
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
    m_spriteShader.enableAttributeArray(0);
    m_spriteShader.setAttributeBuffer(0, GL_FLOAT, 0, 3, 0);

    QOpenGLBuffer tVBO2D;
    tVBO2D.create();
    tVBO2D.setUsagePattern(QOpenGLBuffer::DynamicDraw);
    tVBO2D.bind();
    tVBO2D.allocate(texCoords, 6 * sizeof(QVector2D));
    m_spriteShader.enableAttributeArray(1);
    m_spriteShader.setAttributeBuffer(1, GL_FLOAT, 0, 2, 0);

    QMatrix4x4 matModel;
    matModel.scale(width * m_zoom, height * m_zoom, 1.0f);

    matModel.translate((xpos + (width / 2)) / (float)width, (ypos + (height / 2)) / (float)height,
                       zpos);
    m_spriteShader.setValue("model", matModel);

    f->glDrawArrays(GL_TRIANGLES, 0, 6);
}

void SceneViewer::drawAlphaBlendedSprite(int XPos, int YPos, int width, int height, int sprX, int sprY,
                                         int alpha, int sheetID)
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    // Draw Sprite
    float xpos = XPos - m_cam.m_position.x;
    float ypos = YPos - m_cam.m_position.y;
    float zpos = 9.0f;

    if (sheetID != m_prevSprite) {
        m_objectSprites[sheetID].m_texture->bind();
        m_prevSprite = sheetID;
    }
    float w = m_objectSprites[sheetID].m_texture->width(),
          h = m_objectSprites[sheetID].m_texture->height();

    Rect<int> check = Rect<int>();
    check.x         = (int)(xpos + (float)w) * m_zoom;
    check.y         = (int)(ypos + (float)h) * m_zoom;
    check.w         = (int)(xpos - (w / 2.0f)) * m_zoom;
    check.h         = (int)(ypos - (h / 2.0f)) * m_zoom;
    if (check.x < 0 || check.y < 0 || check.w >= m_storedW || check.h >= m_storedH || !sheetID) {
        return;
    }

    m_spriteShader.use();
    m_spriteShader.setValue("flipX", false);
    m_spriteShader.setValue("flipY", false);
    m_spriteShader.setValue("useAlpha", true);
    m_spriteShader.setValue("alpha", (alpha > 0xFF ? 0xFF : alpha) / 255.0f);
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
    m_spriteShader.enableAttributeArray(0);
    m_spriteShader.setAttributeBuffer(0, GL_FLOAT, 0, 3, 0);

    QOpenGLBuffer tVBO2D;
    tVBO2D.create();
    tVBO2D.setUsagePattern(QOpenGLBuffer::DynamicDraw);
    tVBO2D.bind();
    tVBO2D.allocate(texCoords, 6 * sizeof(QVector2D));
    m_spriteShader.enableAttributeArray(1);
    m_spriteShader.setAttributeBuffer(1, GL_FLOAT, 0, 2, 0);

    QMatrix4x4 matModel;
    matModel.scale(width * m_zoom, height * m_zoom, 1.0f);

    matModel.translate((xpos + (width / 2)) / (float)width, (ypos + (height / 2)) / (float)height,
                       zpos);
    m_spriteShader.setValue("model", matModel);

    f->glDrawArrays(GL_TRIANGLES, 0, 6);
}
