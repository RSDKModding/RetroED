#include "includes.hpp"
#include "ui_sceneeditorv5.h"

enum PropertiesTabIDs { PROP_SCN, PROP_LAYER, PROP_TILE, PROP_ENTITY, PROP_SCROLL };

TileSelector::TileSelector(QWidget *parent) : QWidget(parent), parentPtr((SceneEditorv5 *)parent)
{
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scrollArea->setGeometry(10, 10, 200, 200);

    QWidget *tileArea = new QWidget();

    QGridLayout *layout = new QGridLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(1);

    int i = 0;
    int x = 0, y = 0;
    for (auto &&im : parentPtr->viewer->tiles) {
        auto *tile = new TileLabel(&parentPtr->viewer->selectedTile, i, tileArea);
        tile->setPixmap(QPixmap::fromImage(im).scaled(im.width() * 2, im.height() * 2));
        tile->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        tile->resize(im.width(), im.height());
        layout->addWidget(tile, y, x);
        x++;
        if (x >= 4) {
            x = 0;
            y += 1;
        }
        tile->setFixedSize(im.width() * 2, im.height() * 2);
        i++;
        connect(tile, &TileLabel::requestRepaint, tileArea, QOverload<>::of(&QWidget::update));
    }

    tileArea->setLayout(layout);
    scrollArea->setWidget(tileArea);
    QVBoxLayout *l = new QVBoxLayout(this);
    l->addWidget(scrollArea);
    setLayout(l);
}

SceneEditorv5::SceneEditorv5(QWidget *parent) : QWidget(parent), ui(new Ui::SceneEditorv5)
{
    ui->setupUi(this);

    viewer = new SceneViewerv5(this);
    viewer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->viewerFrame->layout()->addWidget(viewer);
    viewer->show();

    scnProp = new ScenePropertiesv5(this);
    scnProp->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->scenePropFrame->layout()->addWidget(scnProp);
    scnProp->show();

    lyrProp = new SceneLayerPropertiesv5(this);
    lyrProp->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->layerPropFrame->layout()->addWidget(lyrProp);
    lyrProp->show();

    tileProp = new SceneTilePropertiesv5(this);
    tileProp->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->tilePropFrame->layout()->addWidget(tileProp);
    tileProp->show();

    objProp = new SceneObjectPropertiesv5(this);
    objProp->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->objPropFrame->layout()->addWidget(objProp);
    objProp->show();

    scrProp = new SceneScrollPropertiesv5(this);
    scrProp->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->scrPropFrame->layout()->addWidget(scrProp);
    scrProp->show();

    tileSel = new TileSelector(this);
    tileSel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->tilesPage->layout()->addWidget(tileSel);
    tileSel->show();

    viewer->m_sbHorizontal = ui->horizontalScrollBar;
    viewer->m_sbVertical   = ui->verticalScrollBar;
    viewer->statusLabel    = ui->statusLabel;

    snapSize = Vector2<int>(0x10, 0x10);

    ui->showParallax->setIcon(Utils::getColouredIcon(":/icons/ic_horizontal_split_48px.svg"));
    ui->showTileGrid->setIcon(Utils::getColouredIcon(":/icons/ic_grid_48px.svg"));
    ui->showPixelGrid->setIcon(Utils::getColouredIcon(":/icons/ic_grid_48px.svg"));

    ui->toolBox->setCurrentIndex(0);
    ui->propertiesBox->setCurrentIndex(0);

    connect(ui->horizontalScrollBar, &QScrollBar::valueChanged,
            [this](int v) { viewer->cam.pos.x = v; });

    connect(ui->verticalScrollBar, &QScrollBar::valueChanged, [this](int v) { viewer->cam.pos.y = v; });

    connect(ui->sceneFilter, QOverload<int>::of(&QSpinBox::valueChanged),
            [this](int v) { viewer->sceneFilter = v; });

    connect(ui->layerList, &QListWidget::currentRowChanged, [this](int c) {
        // m_uo->setDisabled(c == -1);
        // m_do->setDisabled(c == -1);
        // m_ro->setDisabled(c == -1);

        if (c == -1)
            return;

        // m_do->setDisabled(c == m_objectList->count() - 1);
        // m_uo->setDisabled(c == 0);

        viewer->selectedLayer = c;

        lyrProp->setupUI(&viewer->scene, viewer->selectedLayer);
        ui->propertiesBox->setCurrentWidget(ui->layerPropPage);

        createScrollList();
        ui->addScr->setDisabled(c == -1);
        ui->rmScr->setDisabled(c == -1);
    });

    connect(ui->objectList, &QListWidget::currentRowChanged, [this](int c) {
        // m_uo->setDisabled(c == -1);
        // m_do->setDisabled(c == -1);
        ui->rmObj->setDisabled(c == -1);

        if (c == -1)
            return;

        // m_do->setDisabled(c == m_objectList->count() - 1);
        // m_uo->setDisabled(c == 0);

        viewer->selectedObject = c;
    });

    connect(ui->addObj, &QToolButton::clicked, [this] {
        // uint c = m_objectList->currentRow() + 1;
        SceneObject objInfo;
        objInfo.name = "New Object";
        viewer->stageConfig.objects.append("New Object");
        viewer->objects.append(objInfo);

        auto *item = new QListWidgetItem();
        item->setText("New Object");
        ui->objectList->addItem(item);

        item->setFlags(item->flags() | Qt::ItemIsEditable);
        ui->objectList->setCurrentItem(item);
    });

    connect(ui->rmObj, &QToolButton::clicked, [this] {
        int c = ui->objectList->currentRow();
        int n = ui->objectList->currentRow() == ui->objectList->count() - 1 ? c - 1 : c;
        delete ui->objectList->item(c);
        viewer->objects.removeAt(c);
        ui->objectList->blockSignals(true);
        ui->objectList->setCurrentRow(n);
        ui->objectList->blockSignals(false);

        for (int o = viewer->entities.count() - 1; o >= 0; --o) {
            if (viewer->entities[o].type > c)
                viewer->entities[o].type--;
            else
                viewer->entities.removeAt(o);
        }
    });

    connect(ui->entityList, &QListWidget::currentRowChanged, [this](int c) {
        // m_uo->setDisabled(c == -1);
        // m_do->setDisabled(c == -1);
        ui->rmEnt->setDisabled(c == -1);

        if (c == -1)
            return;

        // m_do->setDisabled(c == m_objectList->count() - 1);
        // m_uo->setDisabled(c == 0);

        viewer->selectedEntity = c;

        viewer->cam.pos.x = viewer->entities[c].pos.x - (viewer->storedW / 2);
        viewer->cam.pos.y = viewer->entities[c].pos.y - (viewer->storedH / 2);

        objProp->setupUI(&viewer->objects, &viewer->entities[viewer->selectedEntity]);
        ui->propertiesBox->setCurrentWidget(ui->objPropPage);
    });

    connect(ui->addEnt, &QToolButton::clicked, [this] {
        // uint c = m_objectList->currentRow() + 1;
        SceneEntity entity;
        entity.type = viewer->selectedObject;
        if (viewer->selectedObject < 0)
            entity.type = 0; // backup
        entity.slotID = viewer->entities.count();

        viewer->entities.append(entity);

        createEntityList();

        ui->addEnt->setDisabled(viewer->entities.count() >= 0x800);
    });

    connect(ui->rmEnt, &QToolButton::clicked, [this] {
        int c = ui->entityList->currentRow();
        int n = ui->entityList->currentRow() == ui->entityList->count() - 1 ? c - 1 : c;
        delete ui->entityList->item(c);
        viewer->entities.removeAt(c);
        ui->entityList->blockSignals(true);
        ui->entityList->setCurrentRow(n);
        ui->entityList->blockSignals(false);

        ui->addEnt->setDisabled(viewer->entities.count() >= 0x800);
    });

    connect(ui->scrollList, &QListWidget::currentRowChanged, [this](int c) {
        ui->rmScr->setDisabled(c == -1);

        if (c == -1)
            return;

        viewer->selectedScrollInfo = c;

        scrProp->setupUI(&viewer->scene.layers[viewer->selectedLayer].scrollInfos[c]);
        ui->propertiesBox->setCurrentWidget(ui->scrollPropPage);
    });

    connect(ui->addScr, &QToolButton::clicked, [this] {
        RSDKv5::Scene::ScrollIndexInfo scr;
        viewer->scene.layers[viewer->selectedLayer].scrollInfos.append(scr);

        createScrollList();
    });

    connect(ui->rmScr, &QToolButton::clicked, [this] {
        int c = ui->scrollList->currentRow();
        int n = ui->scrollList->currentRow() == ui->scrollList->count() - 1 ? c - 1 : c;
        delete ui->scrollList->item(c);
        viewer->scene.layers[viewer->selectedLayer].scrollInfos.removeAt(c);
        ui->scrollList->blockSignals(true);
        ui->scrollList->setCurrentRow(n);
        ui->scrollList->blockSignals(false);
    });

    auto resetTools = [this](byte tool) {
        if (tool == 0xFF)
            tool = TOOL_MOUSE;
        viewer->curTool = tool;

        // Reset
        // m_mainView->m_selectedTile = -1;
        // m_mainView->m_selectedStamp = -1;
        viewer->selectedEntity = -1;
        objProp->unsetUI();
        viewer->isSelecting = false;

        unsetCursor();

        switch (tool) {
            default: break; // what
            case TOOL_MOUSE: break;
            case TOOL_SELECT: break;
            case TOOL_PENCIL: viewer->isSelecting = true; break;
            case TOOL_ERASER: viewer->isSelecting = true; break;
            case TOOL_ENTITY: viewer->isSelecting = true; break;
            case TOOL_PARALLAX: viewer->isSelecting = true; break;
        }
    };

    connect(ui->selToolBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            [resetTools](int v) { resetTools(v); });

    connect(ui->showCollisionA, &QPushButton::clicked, [this] {
        viewer->showPlaneA ^= 1;
        viewer->showPlaneB = false;
        ui->showCollisionA->setChecked(viewer->showPlaneA);
        ui->showCollisionB->setChecked(viewer->showPlaneB);
    });

    connect(ui->showCollisionB, &QPushButton::clicked, [this] {
        viewer->showPlaneA = false;
        viewer->showPlaneB ^= 1;
        ui->showCollisionA->setChecked(viewer->showPlaneA);
        ui->showCollisionB->setChecked(viewer->showPlaneB);
    });

    connect(ui->showTileGrid, &QPushButton::clicked, [this] { viewer->showTileGrid ^= 1; });
    connect(ui->showPixelGrid, &QPushButton::clicked, [this] { viewer->showPixelGrid ^= 1; });

    connect(scnProp->loadGlobalCB, &QCheckBox::toggled,
            [this](bool b) { viewer->stageConfig.loadGlobalObjects = b; });

    connect(ui->showParallax, &QPushButton::clicked, [this] { viewer->showParallax ^= 1; });

    connect(scnProp->m_editSCF, &QPushButton::clicked, [this] {
        QList<int> newTypes;

        StageconfigEditorv5 *edit = new StageconfigEditorv5(&viewer->stageConfig, this);
        edit->exec();

        QList<QString> names;
        for (auto &obj : viewer->stageConfig.objects) {
            names.append(obj);
        }

        int o = 0;
        newTypes.clear();
        newTypes.append(0); // Blank Object
        // Globals stay the same
        if (viewer->stageConfig.loadGlobalObjects) {
            int cnt = newTypes.count();

            for (; o < viewer->gameConfig.objects.count(); ++o) {
                newTypes.append(cnt + o);
            }
        }

        int cnt = newTypes.count();
        for (QString &object : viewer->stageConfig.objects) {
            int index = names.indexOf(object);
            if (index >= 0) {
                index += cnt;
            }
            else {
                viewer->objects.removeAt(newTypes.count());
            }
            newTypes.append(index);
        }

        for (SceneEntity &entity : viewer->entities) {
            if (newTypes[entity.type] >= 0) {
                entity.type = newTypes[entity.type];
            }
            else {
                viewer->entities.removeOne(entity);
            }
        }

        ui->objectList->clear();
        for (int o = 0; o < viewer->objects.count(); ++o)
            ui->objectList->addItem(viewer->objects[o].name);

        createEntityList();
    });

    connect(scnProp->m_editPAL, &QPushButton::clicked, [this] {
        PaletteEditor *edit = new PaletteEditor(viewer->stageConfig.m_filename, 1);
        edit->setWindowTitle("Edit StageConfig Palette");
        edit->show();
    });

    connect(ui->exportSceneImg, &QPushButton::clicked, [this] {
        QFileDialog filedialog(this, tr("Save Image"), "", tr("PNG Files (*.png)"));
        filedialog.setAcceptMode(QFileDialog::AcceptSave);
        if (filedialog.exec() == QDialog::Accepted) {
            SceneExportImgOptions *dlg = new SceneExportImgOptions(this);
            if (dlg->exec() == QDialog::Accepted) {

                QImage image(viewer->sceneWidth * 0x10, viewer->sceneHeight * 0x10,
                             QImage::Format_ARGB32);
                image.fill(0xFF00FF);
                QPainter painter(&image);

                for (int i = 0; i < viewer->scene.layers.count(); ++i) {
                    if (dlg->exportBG[i][0]) {
                        for (int y = 0; y < viewer->scene.layers[i].height; ++y) {
                            for (int x = 0; x < viewer->scene.layers[i].width; ++x) {
                                ushort tile = viewer->scene.layers[i].layout[y][x];
                                if (tile != 0xFFFF) {
                                    QImage tileImg = viewer->tiles[tile & 0x3FF];

                                    painter.drawImage(x * 0x10, y * 0x10, tileImg);
                                }
                            }
                        }
                    }
                }

                if (dlg->exportObjects) {
                    for (int o = 0; o < viewer->entities.count(); ++o) {
                        SceneEntity &entity = viewer->entities[o];

                        QString name = viewer->objects[entity.type].name;

                        painter.drawText(entity.pos.x, entity.pos.y, name);
                        // painter.drawImage(obj.getX(), obj.getY(), m_mainView->m_missingObj);

                        /*if (dlg->exportObjInfo) {
                            painter.drawText(obj.getX(), obj.getY() + 8,
                                             "st: " + QString::number(obj.m_propertyValue));
                            int offset = 16;

                            QList<QString> attribNames = { "ste",  "flip", "scl",  "rot",  "lyr",
                                                           "prio", "alph", "anim", "aspd", "frm",
                                                           "ink",  "v0",   "v2",   "v2",   "v3" };

                            for (int i = 0; i < 0x0F && viewer->gameType == ENGINE_v4; ++i) {
                                if (obj.m_attributes[i].m_active) {
                                    painter.drawText(obj.getX(), obj.getY() + offset,
                                                     attribNames[i] + ": "
                                                         + QString::number(obj.m_propertyValue));
                                    offset += 8;
                                }
                            }
                        }*/
                    }
                }

                image.save(filedialog.selectedFiles()[0]);
                setStatus("Scene exported to image sucessfully!");
            }
        }
    });

    for (QWidget *w : findChildren<QWidget *>()) {
        w->installEventFilter(this);
    }
}

SceneEditorv5::~SceneEditorv5() { delete ui; }

bool SceneEditorv5::event(QEvent *event)
{
    if (event->type() == (QEvent::Type)RE_EVENT_NEW) {
    }
    if (event->type() == (QEvent::Type)RE_EVENT_OPEN) {
        QList<QString> types = {
            "RSDKv5 Scenes (Scene*.bin)",
            "RSDKv5 Scenes (Pre-Plus) (Scene*.bin)",
        };

        QFileDialog filedialog(this, tr("Open Scene"), "",
                               tr(QString("%1;;%2").arg(types[0]).arg(types[1]).toStdString().c_str()));
        filedialog.setAcceptMode(QFileDialog::AcceptOpen);
        if (filedialog.exec() == QDialog::Accepted) {
            int filter     = types.indexOf(filedialog.selectedNameFilter());
            QString gcPath = "";

            QFileDialog gcdialog(this, tr("Open GameConfig"), "",
                                 tr("RSDKv5 GameConfig (GameConfig*.bin)"));
            gcdialog.setAcceptMode(QFileDialog::AcceptOpen);
            if (gcdialog.exec() == QDialog::Accepted) {
                gcPath = gcdialog.selectedFiles()[0];
            }
            else {
                if (!QFile::exists(viewer->gameConfig.m_filename)) {
                    return false;
                }
                gcPath = viewer->gameConfig.m_filename;
            }

            loadScene(filedialog.selectedFiles()[0], gcPath, filter);
            return true;
        }
    }

    if (event->type() == (QEvent::Type)RE_EVENT_SAVE) {
        QString path = viewer->scene.filepath;

        if (QFile::exists(path)) {
            setStatus("Saving Scene...");

            viewer->saveScene(path);

            setStatus("Saved Scene: " + Utils::getFilenameAndFolder(viewer->scene.filepath));
            return true;
        }
        else {
            QList<QString> types = {
                "RSDKv5 Scenes (Scene*.bin)",
            };

            QFileDialog filedialog(this, tr("Save Scene"), "",
                                   tr(QString("%1;;%2;;%3;;%4").arg(types[0]).toStdString().c_str()));
            filedialog.setAcceptMode(QFileDialog::AcceptSave);
            if (filedialog.exec() == QDialog::Accepted) {
                setStatus("Saving Scene...");
                QString path = filedialog.selectedFiles()[0];

                viewer->saveScene(path);

                appConfig.addRecentFile(ENGINE_v5, TOOL_SCENEEDITOR, filedialog.selectedFiles()[0],
                                        QList<QString>{ viewer->gameConfig.m_filename });
                setStatus("Saved Scene: " + Utils::getFilenameAndFolder(viewer->scene.filepath));
                return true;
            }
        }
    }
    if (event->type() == (QEvent::Type)RE_EVENT_SAVE_AS) {
        QList<QString> types = {
            "RSDKv5 Scenes (Scene*.bin)",
        };

        QFileDialog filedialog(this, tr("Save Scene"), "",
                               tr(QString("%1;;%2;;%3;;%4").arg(types[0]).toStdString().c_str()));
        filedialog.setAcceptMode(QFileDialog::AcceptSave);
        if (filedialog.exec() == QDialog::Accepted) {
            setStatus("Saving Scene...");
            QString path = filedialog.selectedFiles()[0];

            viewer->saveScene(path);

            appConfig.addRecentFile(ENGINE_v5, TOOL_SCENEEDITOR, filedialog.selectedFiles()[0],
                                    QList<QString>{ viewer->gameConfig.m_filename });
            setStatus("Saved Scene: " + Utils::getFilenameAndFolder(filedialog.selectedFiles()[0]));
            return true;
        }
    }

    return QWidget::event(event);
}

bool SceneEditorv5::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::FocusIn || event->type() == QEvent::FocusOut) {
        QApplication::sendEvent(this, event);
        return false;
    }

    if (object != viewer)
        return false;

    auto setTile = [this](float x, float y) {
        if (viewer->selectedTile < 0 || viewer->selectedLayer < 0)
            return;
        float tx = x;
        float ty = y;

        tx *= viewer->invZoom();
        ty *= viewer->invZoom();

        float tx2 = tx + fmodf(viewer->cam.pos.x, 0x10);
        float ty2 = ty + fmodf(viewer->cam.pos.y, 0x10);

        // clip to grid
        tx -= fmodf(tx2, 0x10);
        ty -= fmodf(ty2, 0x10);

        // Draw Selected Tile Preview
        float xpos = tx + viewer->cam.pos.x;
        float ypos = ty + viewer->cam.pos.y;

        xpos /= 0x10;
        ypos /= 0x10;
        if (ypos >= 0 && ypos < viewer->scene.layers[viewer->selectedLayer].height) {
            if (xpos >= 0 && xpos < viewer->scene.layers[viewer->selectedLayer].width) {
                viewer->scene.layers[viewer->selectedLayer].layout[ypos][xpos] = viewer->selectedTile;
            }
        }
    };

    auto resetTools = [this](byte tool) {
        if (tool == 0xFF)
            tool = TOOL_MOUSE;
        viewer->curTool = tool;

        // Reset
        // m_mainView->m_selectedTile = -1;
        // m_mainView->m_selectedStamp = -1;
        ui->selToolBox->setDisabled(true);
        ui->selToolBox->setCurrentIndex(tool);
        ui->selToolBox->setDisabled(false);
        viewer->selectedEntity = -1;
        objProp->unsetUI();
        viewer->isSelecting = false;

        unsetCursor();

        switch (tool) {
            default: break; // what
            case TOOL_MOUSE: break;
            case TOOL_SELECT: break;
            case TOOL_PENCIL: viewer->isSelecting = true; break;
            case TOOL_ERASER: viewer->isSelecting = true; break;
            case TOOL_ENTITY: viewer->isSelecting = true; break;
            case TOOL_PARALLAX: viewer->isSelecting = true; break;
        }
    };

    switch (event->type()) {
        case QEvent::MouseButtonPress: {
            QMouseEvent *mEvent = static_cast<QMouseEvent *>(event);
            viewer->m_reference = mEvent->pos();

            viewer->mousePos.x = viewer->cam.m_lastMousePos.x = mEvent->pos().x();
            viewer->mousePos.y = viewer->cam.m_lastMousePos.y = mEvent->pos().y();

            if ((mEvent->button() & Qt::LeftButton) == Qt::LeftButton)
                m_mouseDownL = true;
            if ((mEvent->button() & Qt::MiddleButton) == Qt::MiddleButton)
                m_mouseDownM = true;
            if ((mEvent->button() & Qt::RightButton) == Qt::RightButton)
                m_mouseDownR = true;

            if (m_mouseDownM || (m_mouseDownL && viewer->curTool == TOOL_MOUSE))
                setCursor(Qt::ClosedHandCursor);

            if (m_mouseDownL) {
                switch (viewer->curTool) {
                    case TOOL_MOUSE: break;
                    case TOOL_SELECT: break;
                    case TOOL_PENCIL: {
                        if (viewer->selectedTile >= 0 && viewer->isSelecting) {
                            setTile(mEvent->pos().x(), mEvent->pos().y());
                        }
                        else {
                            // click tile to move it or change properties
                            if (viewer->selectedLayer >= 0) {
                                Rect<float> box;

                                for (int y = 0; y < viewer->scene.layers[viewer->selectedLayer].height;
                                     ++y) {
                                    for (int x = 0;
                                         x < viewer->scene.layers[viewer->selectedLayer].width; ++x) {
                                        box = Rect<float>(x * 0x10, y * 0x10, 0x10, 0x10);

                                        Vector2<float> pos = Vector2<float>(
                                            (mEvent->pos().x() * viewer->invZoom()) + viewer->cam.pos.x,
                                            (mEvent->pos().y() * viewer->invZoom())
                                                + viewer->cam.pos.y);
                                        if (box.contains(pos)) {
                                            // idk but we're in the gaming zone now bitch!
                                            // ushort tile =
                                            // viewer->scene.layers[viewer->selectedLayer].layout[y][x];
                                            break;
                                        }
                                    }
                                }
                            }
                        }
                        break;
                    }
                    case TOOL_ERASER: {
                        if (viewer->isSelecting) {
                            viewer->selectedTile = 0x00;
                            setTile(mEvent->pos().x(), mEvent->pos().y());
                        }
                        break;
                    }
                    case TOOL_ENTITY: {
                        if (!viewer->isSelecting || viewer->selectedObject < 0) {
                            Rect<float> box;

                            for (int o = 0; o < viewer->entities.count(); ++o) {
                                box = Rect<float>(viewer->entities[o].pos.x - 0x10,
                                                  viewer->entities[o].pos.y - 0x10, 0x20, 0x20);

                                Vector2<float> pos = Vector2<float>(
                                    (mEvent->pos().x() * viewer->invZoom()) + viewer->cam.pos.x,
                                    (mEvent->pos().y() * viewer->invZoom()) + viewer->cam.pos.y);
                                if (box.contains(pos)) {
                                    viewer->selectedEntity = o;

                                    objProp->setupUI(&viewer->objects,
                                                     &viewer->entities[viewer->selectedEntity]);
                                    ui->propertiesBox->setCurrentWidget(ui->objPropPage);
                                    break;
                                }
                            }
                        }
                        else {
                            if (viewer->selectedObject >= 0) {
                                SceneEntity entity;
                                entity.type = viewer->selectedObject;
                                int xpos = (mEvent->pos().x() * viewer->invZoom()) + viewer->cam.pos.x;
                                xpos <<= 16;
                                int ypos = (mEvent->pos().y() * viewer->invZoom()) + viewer->cam.pos.y;
                                ypos <<= 16;

                                entity.pos.x =
                                    ((mEvent->pos().x() * viewer->invZoom()) + viewer->cam.pos.x);
                                entity.pos.y =
                                    ((mEvent->pos().y() * viewer->invZoom()) + viewer->cam.pos.y);

                                int cnt = viewer->entities.count();
                                viewer->entities.append(entity);
                                // viewer->m_compilerv3.m_objectEntityList[cnt].type =
                                //    viewer->selectedObject;
                                // viewer->m_compilerv3.m_objectEntityList[cnt].propertyValue = 0;
                                // viewer->m_compilerv3.m_objectEntityList[cnt].XPos          = xpos;
                                // viewer->m_compilerv3.m_objectEntityList[cnt].YPos          = ypos;
                                //
                                // viewer->m_compilerv4.m_objectEntityList[cnt].type =
                                //    viewer->selectedObject;
                                // viewer->m_compilerv4.m_objectEntityList[cnt].propertyValue = 0;
                                // viewer->m_compilerv4.m_objectEntityList[cnt].XPos          = xpos;
                                // viewer->m_compilerv4.m_objectEntityList[cnt].YPos          = ypos;
                            }
                        }

                        break;
                    }
                    default: break;
                }
                break;
            }

            if (m_mouseDownR) {
                switch (viewer->curTool) {
                    case TOOL_PENCIL:
                        if (viewer->selectedLayer >= 0) {
                            Rect<float> box;

                            for (int y = 0; y < viewer->scene.layers[viewer->selectedLayer].height;
                                 ++y) {
                                for (int x = 0; x < viewer->scene.layers[viewer->selectedLayer].width;
                                     ++x) {
                                    box = Rect<float>(x * 0x10, y * 0x10, 0x10, 0x10);

                                    Vector2<float> pos = Vector2<float>(
                                        (mEvent->pos().x() * viewer->invZoom()) + viewer->cam.pos.x,
                                        (mEvent->pos().y() * viewer->invZoom()) + viewer->cam.pos.y);
                                    if (box.contains(pos)) {
                                        ushort tile =
                                            viewer->scene.layers[viewer->selectedLayer].layout[y][x];

                                        tileProp->setupUI(
                                            &viewer->tileconfig.collisionPaths[0][tile & 0x3FF],
                                            &viewer->tileconfig.collisionPaths[1][tile & 0x3FF],
                                            &viewer->scene.layers[viewer->selectedLayer].layout[y][x],
                                            viewer->tiles[tile & 0x3FF]);
                                        ui->propertiesBox->setCurrentIndex(PROP_TILE);
                                        break;
                                    }
                                }
                            }
                        }
                        break;
                    case TOOL_ENTITY: {
                        Rect<float> box;
                        bool found = false;

                        for (int o = 0; o < viewer->entities.count(); ++o) {
                            box = Rect<float>(viewer->entities[o].pos.x - 0x10,
                                              viewer->entities[o].pos.y - 0x10, 0x20, 0x20);

                            Vector2<float> pos = Vector2<float>(
                                (mEvent->pos().x() * viewer->invZoom()) + viewer->cam.pos.x,
                                (mEvent->pos().y() * viewer->invZoom()) + +viewer->cam.pos.y);
                            if (box.contains(pos)) {
                                viewer->selectedEntity = o;
                                found                  = true;

                                objProp->setupUI(&viewer->objects,
                                                 &viewer->entities[viewer->selectedEntity]);
                                ui->propertiesBox->setCurrentWidget(ui->objPropPage);
                                break;
                            }
                        }

                        if (!found) {
                            viewer->selectedEntity = -1;
                            viewer->selectedObject = -1;
                            ui->objectList->setCurrentRow(-1);
                            ui->entityList->setCurrentRow(-1);

                            objProp->unsetUI();
                        }
                        break;
                    }
                    case TOOL_PARALLAX: break;
                    default: break;
                }
                break;
            }

            break;
        }

        case QEvent::MouseMove: {
            bool status         = false;
            QMouseEvent *mEvent = static_cast<QMouseEvent *>(event);

            viewer->mousePos.x = viewer->cam.m_lastMousePos.x = mEvent->pos().x();
            viewer->mousePos.y = viewer->cam.m_lastMousePos.y = mEvent->pos().y();

            Vector2<int> m_sceneMousePos((viewer->mousePos.x * viewer->invZoom()) + viewer->cam.pos.x,
                                         (viewer->mousePos.y * viewer->invZoom()) + viewer->cam.pos.y);

            if (m_mouseDownM || (m_mouseDownL && viewer->curTool == TOOL_MOUSE)) {
                viewer->cam.pos.x -= (viewer->mousePos.x - viewer->m_reference.x()) * viewer->invZoom();
                viewer->cam.pos.y -= (viewer->mousePos.y - viewer->m_reference.y()) * viewer->invZoom();
                viewer->m_reference = mEvent->pos();
                QPoint lp           = viewer->mapFromGlobal(QCursor::pos());
                if (!viewer->rect().contains(lp)) {
                    if (lp.x() < viewer->x())
                        lp.setX(lp.x() + viewer->width());
                    else if (lp.x() > viewer->x() + viewer->width())
                        lp.setX(lp.x() - viewer->width());
                    if (lp.y() < viewer->y())
                        lp.setY(lp.y() + viewer->height());
                    else if (lp.y() > viewer->y() + viewer->height())
                        lp.setY(lp.y() - viewer->height());
                    viewer->m_reference = lp;
                    QCursor::setPos(viewer->mapToGlobal(lp));
                }
                status = true;

                ui->horizontalScrollBar->blockSignals(true);
                ui->horizontalScrollBar->setMaximum((viewer->sceneWidth * 0x10) - viewer->storedW);
                ui->horizontalScrollBar->setValue(viewer->cam.pos.x);
                ui->horizontalScrollBar->blockSignals(false);

                ui->verticalScrollBar->blockSignals(true);
                ui->verticalScrollBar->setMaximum((viewer->sceneHeight * 0x10) - viewer->storedH);
                ui->verticalScrollBar->setValue(viewer->cam.pos.y);
                ui->verticalScrollBar->blockSignals(false);
            }

            if (viewer->curTool == TOOL_PENCIL || viewer->curTool == TOOL_ENTITY) {
                viewer->tilePos.x = viewer->mousePos.x;
                viewer->tilePos.y = viewer->mousePos.y;

                if (ctrlDownL) {
                    viewer->tilePos.x -= fmodf(viewer->tilePos.x + (0x10 / 2), 0x10);
                    viewer->tilePos.y -= fmodf(viewer->tilePos.y + (0x10 / 2), 0x10);

                    // TODO: entities not previewing correctly when snapping
                }
            }

            // Hover
            switch (viewer->curTool) {
                default: break;
                case TOOL_MOUSE: break;
                case TOOL_SELECT: break;
                case TOOL_PENCIL: break;
                case TOOL_ERASER: break;
                case TOOL_ENTITY: break;
                case TOOL_PARALLAX: break;
            }

            if (m_mouseDownL) {
                switch (viewer->curTool) {
                    default: break;
                    case TOOL_MOUSE: break;
                    case TOOL_SELECT: break;
                    case TOOL_PENCIL: {
                        if (viewer->selectedTile >= 0 && viewer->isSelecting) {
                            setTile(viewer->mousePos.x, viewer->mousePos.y);
                        }
                        break;
                    }
                    case TOOL_ERASER: {
                        if (viewer->isSelecting) {
                            viewer->selectedTile = 0x0;
                            setTile(viewer->mousePos.x, viewer->mousePos.y);
                        }
                        break;
                    }
                    case TOOL_ENTITY: {
                        if (viewer->selectedObject < 0 && viewer->selectedEntity >= 0) {
                            SceneEntity &entity = viewer->entities[viewer->selectedEntity];

                            entity.pos.x = (viewer->mousePos.x * viewer->invZoom()) + viewer->cam.pos.x;
                            entity.pos.y = (viewer->mousePos.y * viewer->invZoom()) + viewer->cam.pos.y;

                            if (ctrlDownL) {
                                entity.pos.x = entity.pos.x - fmodf(entity.pos.x, snapSize.x);
                                entity.pos.y = entity.pos.y - fmodf(entity.pos.y, snapSize.y);
                            }
                            // viewer->m_compilerv3.m_objectEntityList[viewer->selectedEntity].XPos =
                            //    object.m_position.x;
                            // viewer->m_compilerv3.m_objectEntityList[viewer->selectedEntity].YPos =
                            //    object.m_position.y;
                            //
                            // viewer->m_compilerv4.m_objectEntityList[viewer->selectedEntity].XPos =
                            //    object.m_position.x;
                            // viewer->m_compilerv4.m_objectEntityList[viewer->selectedEntity].YPos =
                            //    object.m_position.y;
                        }
                        break;
                    }
                    case TOOL_PARALLAX: break;
                }
            }

            if (status)
                return true;
            else
                break;
        };

        case QEvent::MouseButtonRelease: {
            QMouseEvent *mEvent = static_cast<QMouseEvent *>(event);
            viewer->mousePos.x  = mEvent->pos().x();
            viewer->mousePos.y  = mEvent->pos().y();

            if ((mEvent->button() & Qt::LeftButton) == Qt::LeftButton)
                m_mouseDownL = false;
            if ((mEvent->button() & Qt::MiddleButton) == Qt::MiddleButton)
                m_mouseDownM = false;
            if ((mEvent->button() & Qt::RightButton) == Qt::RightButton)
                m_mouseDownR = false;

            unsetCursor();
            break;
        }

        case QEvent::Wheel: {
            QWheelEvent *wEvent = static_cast<QWheelEvent *>(event);

            if (wEvent->modifiers() & Qt::ControlModifier) {
                if (wEvent->angleDelta().y() > 0 && viewer->zoom < 20)
                    viewer->zoom *= 1.5;
                else if (wEvent->angleDelta().y() < 0 && viewer->zoom > 0.5)
                    viewer->zoom /= 1.5;
                return true;
            }
            viewer->cam.pos.y -= wEvent->angleDelta().y() / 8;
            viewer->cam.pos.x -= wEvent->angleDelta().x() / 8;

            break;
        }

        case QEvent::KeyPress: {
            QKeyEvent *kEvent = static_cast<QKeyEvent *>(event);

            Vector2<int> m_sceneMousePos((viewer->mousePos.x * viewer->invZoom()) + viewer->cam.pos.x,
                                         (viewer->mousePos.y * viewer->invZoom()) + viewer->cam.pos.y);

            if (kEvent->key() == Qt::Key_Control)
                ctrlDownL = true;
            if (kEvent->key() == Qt::Key_Alt)
                altDownL = true;
            if (kEvent->key() == Qt::Key_Shift)
                shiftDownL = true;

            if ((kEvent->modifiers() & Qt::ControlModifier) == Qt::ControlModifier
                && kEvent->key() == Qt::Key_V) {
                if (clipboard) {
                    switch (clipboardType) {
                        default: break;
                        case COPY_ENTITY: {
                            SceneEntity entity = *(SceneEntity *)clipboard;
                            entity.pos.x       = m_sceneMousePos.x;
                            entity.pos.y       = m_sceneMousePos.y;

                            int cnt = viewer->entities.count();
                            viewer->entities.append(entity);
                            // viewer->m_compilerv3.m_objectEntityList[cnt].XPos = entity.pos.x;
                            // viewer->m_compilerv3.m_objectEntityList[cnt].YPos = entity.pos.y;
                            //
                            // viewer->m_compilerv4.m_objectEntityList[cnt].XPos = entity.pos.x;
                            // viewer->m_compilerv4.m_objectEntityList[cnt].YPos = entity.pos.y;
                        } break;
                    }
                }
            }

            byte prevTool = viewer->curTool;
            byte tool     = viewer->curTool;
            if (kEvent->key() == Qt::Key_S)
                tool = TOOL_MOUSE;
            if (kEvent->key() == Qt::Key_S)
                tool = TOOL_SELECT;
            if (kEvent->key() == Qt::Key_C)
                tool = TOOL_PENCIL;
            if (kEvent->key() == Qt::Key_E)
                tool = TOOL_ERASER;
            if (kEvent->key() == Qt::Key_O)
                tool = TOOL_ENTITY;
            if (kEvent->key() == Qt::Key_P)
                tool = TOOL_PARALLAX;

            if (tool != prevTool)
                resetTools(tool);

            switch (viewer->curTool) {
                case TOOL_MOUSE: break;
                case TOOL_SELECT: break;
                case TOOL_PENCIL:
                    if (kEvent->key() == Qt::Key_Z)
                        viewer->m_tileFlip.x = true;

                    if (kEvent->key() == Qt::Key_X)
                        viewer->m_tileFlip.y = true;

                    if (kEvent->key() == Qt::Key_Delete || kEvent->key() == Qt::Key_Backspace) {
                        if (viewer->selectedLayer >= 0) {
                            Rect<float> box;

                            for (int y = 0; y < viewer->scene.layers[viewer->selectedLayer].height;
                                 ++y) {
                                for (int x = 0; x < viewer->scene.layers[viewer->selectedLayer].width;
                                     ++x) {
                                    box = Rect<float>(x * 0x10, y * 0x10, 0x10, 0x10);

                                    Vector2<float> pos = Vector2<float>(
                                        (viewer->tilePos.x * viewer->invZoom()) + viewer->cam.pos.x,
                                        (viewer->tilePos.y * viewer->invZoom()) + viewer->cam.pos.y);
                                    if (box.contains(pos)) {
                                        viewer->scene.layers[viewer->selectedLayer].layout[y][x] = 0;

                                        // reset context
                                        break;
                                    }
                                }
                            }
                        }
                    }
                    break;
                case TOOL_ERASER: break;
                case TOOL_ENTITY:
                    if (kEvent->key() == Qt::Key_Delete || kEvent->key() == Qt::Key_Backspace) {
                        if (viewer->selectedEntity >= 0) {
                            viewer->scene.objects.removeAt(viewer->selectedEntity);
                            viewer->selectedEntity = -1;

                            objProp->unsetUI();
                            createEntityList();
                        }
                    }

                    if ((kEvent->modifiers() & Qt::ControlModifier) == Qt::ControlModifier
                        && kEvent->key() == Qt::Key_C) {
                        if (viewer->selectedEntity >= 0) {
                            clipboard     = &viewer->scene.objects[viewer->selectedEntity];
                            clipboardType = COPY_ENTITY;
                            clipboardInfo = viewer->selectedEntity;
                        }
                    }
                    break;
                case TOOL_PARALLAX: break;
            }
            break;
        }

        case QEvent::KeyRelease: {
            QKeyEvent *kEvent = static_cast<QKeyEvent *>(event);

            if (kEvent->key() == Qt::Key_Control)
                ctrlDownL = false;
            if (kEvent->key() == Qt::Key_Alt)
                altDownL = false;
            if (kEvent->key() == Qt::Key_Shift)
                shiftDownL = false;
            break;
        }

        default: return false;
    }

    return true;
}

void SceneEditorv5::loadScene(QString scnPath, QString gcfPath, byte sceneVer)
{
    setStatus("Loading Scene...");

    if (gcfPath != viewer->gameConfig.m_filename)
        viewer->gameConfig.read(gcfPath, sceneVer == 1);
    QString dataPath = QFileInfo(gcfPath).absolutePath();
    QDir dir(dataPath);
    dir.cdUp();
    viewer->dataPath = dir.path();

    viewer->loadScene(scnPath);

    ui->layerList->clear();
    for (int l = 0; l < viewer->scene.layers.count(); ++l)
        ui->layerList->addItem(viewer->scene.layers[l].m_name);

    ui->objectList->clear();
    // ui->objectList->addItem("Blank Object");
    // if (!sceneVer)
    //     ui->objectList->addItem("Dev Output");
    for (int o = 0; o < viewer->objects.count(); ++o) ui->objectList->addItem(viewer->objects[o].name);

    createEntityList();

    viewer->sceneWidth  = 0;
    viewer->sceneHeight = 0;
    for (int l = 0; l < viewer->scene.layers.count(); ++l) {
        if (viewer->sceneWidth < viewer->scene.layers[l].width)
            viewer->sceneWidth = viewer->scene.layers[l].width;
        if (viewer->sceneHeight < viewer->scene.layers[l].height)
            viewer->sceneHeight = viewer->scene.layers[l].height;
    }

    viewer->vertsPtr  = new QVector3D[viewer->sceneHeight * viewer->sceneWidth * 0x10 * 6];
    viewer->tVertsPtr = new QVector2D[viewer->sceneHeight * viewer->sceneWidth * 0x10 * 6];

    ui->horizontalScrollBar->setMaximum((viewer->sceneWidth * 0x10) - viewer->storedW);
    ui->verticalScrollBar->setMaximum((viewer->sceneHeight * 0x10) - viewer->storedH);
    ui->horizontalScrollBar->setPageStep(0x10);
    ui->verticalScrollBar->setPageStep(0x10);

    scnProp->setupUI(&viewer->scene, &viewer->stageConfig);

    scnProp->loadGlobalCB->blockSignals(true);
    scnProp->loadGlobalCB->setChecked(viewer->stageConfig.loadGlobalObjects);
    scnProp->loadGlobalCB->blockSignals(false);

    if (tileSel) {
        ui->tilesPage->layout()->removeWidget(tileSel);
        delete tileSel;
    }

    tileSel = new TileSelector(this);
    tileSel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->tilesPage->layout()->addWidget(tileSel);
    tileSel->show();

    ui->layerList->setCurrentRow(0);
    viewer->selectedLayer = 0;

    ui->toolBox->setCurrentIndex(0);
    ui->propertiesBox->setCurrentIndex(0);

    scnProp->setupUI(&viewer->scene, &viewer->stageConfig);
    lyrProp->unsetUI();
    tileProp->unsetUI();
    objProp->unsetUI();
    scrProp->unsetUI();

    gameLink.LinkGameObjects();

    for (int i = 0; i < viewer->objects.count(); ++i) {
        viewer->callGameEvent(gameLink.GetObjectInfo(viewer->objects[i].name),
                              SceneViewerv5::EVENT_LOAD);
    }

    appConfig.addRecentFile(ENGINE_v5, TOOL_SCENEEDITOR, scnPath, QList<QString>{ gcfPath });
    setStatus("Loaded Scene: " + QFileInfo(scnPath).fileName());
}
void SceneEditorv5::createEntityList()
{
    ui->entityList->blockSignals(true);
    ui->entityList->clear();

    // C++11 abute poggers
    std::sort(viewer->entities.begin(), viewer->entities.end(),
              [](const SceneEntity &a, const SceneEntity &b) -> bool { return a.slotID < b.slotID; });
    for (int i = 0; i < viewer->entities.count(); ++i) {
        QString name = viewer->objects[viewer->entities[i].type].name;
        ui->entityList->addItem(QString::number(viewer->entities[i].slotID) + ": " + name);
    }
    ui->entityList->blockSignals(false);
}

void SceneEditorv5::createScrollList()
{
    ui->scrollList->clear();

    if (viewer->selectedLayer < 0 || viewer->selectedLayer >= viewer->scene.layers.count())
        return;

    ui->scrollList->blockSignals(true);
    for (int i = 0; i < viewer->scene.layers[viewer->selectedLayer].scrollInfos.count(); ++i) {
        RSDKv5::Scene::ScrollIndexInfo &info =
            viewer->scene.layers[viewer->selectedLayer].scrollInfos[i];
        ui->scrollList->addItem(QString("Start: %1, Length %2").arg(info.startLine).arg(info.length));
    }

    ui->scrollList->blockSignals(false);
    ui->scrollList->setCurrentRow(-1);
}

#include "moc_sceneeditorv5.cpp"
