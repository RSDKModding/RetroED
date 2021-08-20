#include "includes.hpp"
#include "ui_sceneeditor.h"

// RSDKv4 SOLIDITY TYPES
// 0 - ALL SOLID
// 1 - TOP SOLID (can be gripped onto)
// 2 - LRB SOLID
// 3 - NONE SOLID
// 4 - TOP SOLID (cant be gripped onto)

enum PropertiesTabIDs { PROP_SCN, PROP_LAYER, PROP_TILE, PROP_ENTITY, PROP_SCROLL };

ChunkSelector::ChunkSelector(QWidget *parent) : QWidget(parent), m_parent((SceneEditor *)parent)
{
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scrollArea->setGeometry(10, 10, 200, 200);

    QWidget *chunkArea = new QWidget();

    QGridLayout *layout = new QGridLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    int i = 0;
    for (auto &&im : m_parent->viewer->chunks) {
        auto *chunk = new ChunkLabel(&m_parent->viewer->selectedChunk, i, chunkArea);
        chunk->setPixmap(QPixmap::fromImage(im).scaled(im.width(), im.height()));
        chunk->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        chunk->resize(im.width(), im.height());
        layout->addWidget(chunk, i, 1);
        chunk->setFixedSize(im.width(), im.height() * 1.1f);
        i++;
        connect(chunk, &ChunkLabel::requestRepaint, chunkArea, QOverload<>::of(&QWidget::update));
    }

    chunkArea->setLayout(layout);
    scrollArea->setWidget(chunkArea);
    QVBoxLayout *l = new QVBoxLayout(this);
    l->addWidget(scrollArea);
    setLayout(l);
}

SceneEditor::SceneEditor(QWidget *parent) : QWidget(parent), ui(new Ui::SceneEditor)
{
    ui->setupUi(this);

    viewer = new SceneViewer(this);
    viewer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->viewerFrame->layout()->addWidget(viewer);
    viewer->show();

    m_scnProp = new SceneProperties(this);
    m_scnProp->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->scenePropFrame->layout()->addWidget(m_scnProp);
    m_scnProp->show();

    m_lyrProp = new SceneLayerProperties(this);
    m_lyrProp->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->layerPropFrame->layout()->addWidget(m_lyrProp);
    m_lyrProp->show();

    m_tileProp = new SceneTileProperties(this);
    m_tileProp->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->tilePropFrame->layout()->addWidget(m_tileProp);
    m_tileProp->show();

    m_objProp = new SceneObjectProperties(this);
    m_objProp->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->objPropFrame->layout()->addWidget(m_objProp);
    m_objProp->show();

    m_scrProp = new SceneScrollProperties(this);
    m_scrProp->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->scrPropFrame->layout()->addWidget(m_scrProp);
    m_scrProp->show();

    m_chkProp = new ChunkSelector(this);
    m_chkProp->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->chunksPage->layout()->addWidget(m_chkProp);
    m_chkProp->show();

    viewer->m_sbHorizontal = ui->horizontalScrollBar;
    viewer->m_sbVertical   = ui->verticalScrollBar;
    viewer->statusLabel    = ui->statusLabel;

    m_snapSize = Vector2<int>(0x10, 0x10);

    ui->showParallax->setIcon(Utils::getColouredIcon(":/icons/ic_horizontal_split_48px.svg"));
    ui->showChunkGrid->setIcon(Utils::getColouredIcon(":/icons/ic_grid_48px.svg"));
    ui->showTileGrid->setIcon(Utils::getColouredIcon(":/icons/ic_grid_48px.svg"));
    ui->showPixelGrid->setIcon(Utils::getColouredIcon(":/icons/ic_grid_48px.svg"));
    ui->exportRSDKv5->setIcon(Utils::getColouredIcon(":/icons/ic_redo_48px.svg"));

    ui->toolBox->setCurrentIndex(0);
    ui->propertiesBox->setCurrentIndex(0);

    connect(ui->horizontalScrollBar, &QScrollBar::valueChanged,
            [this](int v) { viewer->cam.pos.x = v; });

    connect(ui->verticalScrollBar, &QScrollBar::valueChanged, [this](int v) { viewer->cam.pos.y = v; });

    connect(ui->layerList, &QListWidget::currentRowChanged, [this](int c) {
        // m_uo->setDisabled(c == -1);
        // m_do->setDisabled(c == -1);
        // m_ro->setDisabled(c == -1);

        if (c == -1)
            return;

        // m_do->setDisabled(c == m_objectList->count() - 1);
        // m_uo->setDisabled(c == 0);

        viewer->selectedLayer = c;

        m_lyrProp->setupUI(&viewer->scene, &viewer->background, c, viewer->gameType);
        ui->propertiesBox->setCurrentWidget(ui->layerPropPage);

        createScrollList();
        ui->addScr->setDisabled(c < 1);
        ui->rmScr->setDisabled(c < 1);
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
        FormatHelpers::Stageconfig::ObjectInfo objInfo;
        objInfo.m_name = "New Object";
        viewer->stageConfig.objects.append(objInfo);
        viewer->scene.objectTypeNames.append("New Object");

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
        viewer->scene.objects.removeAt(c);
        ui->objectList->blockSignals(true);
        ui->objectList->setCurrentRow(n);
        ui->objectList->blockSignals(false);

        for (int o = viewer->scene.objects.count() - 1; o >= 0; --o) {
            if (viewer->scene.objects[o].type > c)
                viewer->scene.objects[o].type--;
            else
                viewer->scene.objects.removeAt(o);
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

        viewer->m_selectedEntity = c;

        viewer->cam.pos.x = viewer->scene.objects[c].getX() - (viewer->storedW / 2);
        viewer->cam.pos.y = viewer->scene.objects[c].getY() - (viewer->storedH / 2);

        m_objProp->setupUI(&viewer->scene.objects[viewer->m_selectedEntity],
                           &viewer->m_compilerv3.m_objectEntityList[viewer->m_selectedEntity],
                           &viewer->m_compilerv4.m_objectEntityList[viewer->m_selectedEntity],
                           viewer->gameType);
        ui->propertiesBox->setCurrentWidget(ui->objPropPage);
    });

    connect(ui->addEnt, &QToolButton::clicked, [this] {
        // uint c = m_objectList->currentRow() + 1;
        FormatHelpers::Scene::Object obj;
        obj.type = viewer->selectedObject;
        if (viewer->selectedObject < 0)
            obj.type = 0; // backup
        obj.m_id = viewer->scene.objects.count();

        viewer->scene.objects.append(obj);

        createEntityList();

        ui->addEnt->setDisabled(viewer->scene.objects.count() >= FormatHelpers::Scene::maxObjectCount);
    });

    connect(ui->rmEnt, &QToolButton::clicked, [this] {
        int c = ui->entityList->currentRow();
        int n = ui->entityList->currentRow() == ui->entityList->count() - 1 ? c - 1 : c;
        delete ui->entityList->item(c);
        viewer->scene.objects.removeAt(c);
        ui->entityList->blockSignals(true);
        ui->entityList->setCurrentRow(n);
        ui->entityList->blockSignals(false);

        ui->addEnt->setDisabled(viewer->scene.objects.count() >= FormatHelpers::Scene::maxObjectCount);
    });

    connect(ui->scrollList, &QListWidget::currentRowChanged, [this](int c) {
        ui->rmScr->setDisabled(c == -1);

        if (c == -1)
            return;

        viewer->selectedScrollInfo = c;

        m_scrProp->setupUI(&viewer->background.layers[viewer->selectedLayer - 1].scrollInfos[c]);
        ui->propertiesBox->setCurrentWidget(ui->scrollPropPage);
    });

    connect(ui->addScr, &QToolButton::clicked, [this] {
        FormatHelpers::Background::ScrollIndexInfo scr;
        viewer->background.layers[viewer->selectedLayer - 1].scrollInfos.append(scr);

        createScrollList();
    });

    connect(ui->rmScr, &QToolButton::clicked, [this] {
        int c = ui->scrollList->currentRow();
        int n = ui->scrollList->currentRow() == ui->scrollList->count() - 1 ? c - 1 : c;
        delete ui->scrollList->item(c);
        viewer->background.layers[viewer->selectedLayer - 1].scrollInfos.removeAt(c);
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
        viewer->m_selectedEntity = -1;
        m_objProp->unsetUI();
        viewer->m_selecting = false;

        unsetCursor();

        switch (tool) {
            default: break; // what
            case TOOL_MOUSE: break;
            case TOOL_SELECT: break;
            case TOOL_PENCIL: viewer->m_selecting = true; break;
            case TOOL_ERASER: viewer->m_selecting = true; break;
            case TOOL_ENTITY: viewer->m_selecting = true; break;
            case TOOL_PARALLAX: viewer->m_selecting = true; break;
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

    connect(ui->showChunkGrid, &QPushButton::clicked, [this] { viewer->showChunkGrid ^= 1; });
    connect(ui->showTileGrid, &QPushButton::clicked, [this] { viewer->showTileGrid ^= 1; });
    connect(ui->showPixelGrid, &QPushButton::clicked, [this] { viewer->showPixelGrid ^= 1; });

    connect(m_scnProp->loadGlobalCB, &QCheckBox::toggled,
            [this](bool b) { viewer->stageConfig.loadGlobalScripts = b; });

    connect(ui->showParallax, &QPushButton::clicked, [this] { viewer->showParallax ^= 1; });

    connect(m_scnProp->m_editTIL, &QPushButton::clicked, [this] {
        ChunkEditor *edit = new ChunkEditor(&viewer->chunkset, viewer->chunks, viewer->tiles,
                                            viewer->gameType == ENGINE_v1, this);
        edit->exec();
    });

    connect(m_scnProp->m_editSCF, &QPushButton::clicked, [this] {
        QList<int> newTypes;

        switch (viewer->gameType) {
            case ENGINE_v4: {
                StageconfigEditorv4 *edit = new StageconfigEditorv4(
                    &viewer->stageConfig, viewer->gameConfig.objects.count() + 1, this);
                edit->exec();
                break;
            }
            case ENGINE_v3: {
                StageconfigEditorv3 *edit = new StageconfigEditorv3(
                    &viewer->stageConfig, viewer->gameConfig.objects.count() + 1, this);
                edit->exec();
                break;
            }
            case ENGINE_v2: {
                StageconfigEditorv2 *edit = new StageconfigEditorv2(
                    &viewer->stageConfig, viewer->gameConfig.objects.count() + 2, this);
                edit->exec();
                break;
            }
            case ENGINE_v1: {
                StageconfigEditorv1 *edit = new StageconfigEditorv1(&viewer->stageConfig, this);
                edit->exec();
                break;
            }
        }

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

        QList<QString> names;
        for (FormatHelpers::Stageconfig::ObjectInfo &obj : viewer->stageConfig.objects) {
            names.append(obj.m_name);
        }

        int o = 0;
        newTypes.append(0); // Blank Object
        // Globals stay the same
        if (viewer->gameType != ENGINE_v1) {
            if (viewer->stageConfig.loadGlobalScripts) {
                if (viewer->gameType == ENGINE_v2)
                    newTypes.append(1); // Player
                int cnt = newTypes.count();

                for (; o < viewer->gameConfig.objects.count(); ++o) {
                    newTypes.append(cnt + o);
                }
            }
        }
        else {
            for (; o < globals.count(); ++o) {
                newTypes.append(1 + o);
            }
        }

        int cnt = newTypes.count();
        for (; o < viewer->scene.objectTypeNames.count(); ++o) {
            int index = names.indexOf(viewer->scene.objectTypeNames[o]);
            if (index >= 0)
                index += cnt;
            newTypes.append(index);
        }

        for (FormatHelpers::Scene::Object &obj : viewer->scene.objects) {
            if (newTypes[obj.type] >= 0)
                obj.type = newTypes[obj.type];
            else
                obj.type = 0;
        }

        viewer->scene.objectTypeNames.clear();
        if (viewer->gameType != ENGINE_v1) {
            if (viewer->stageConfig.loadGlobalScripts) {
                if (viewer->gameType == ENGINE_v2)
                    viewer->scene.objectTypeNames.append("Player");

                for (FormatHelpers::Gameconfig::ObjectInfo &obj : viewer->gameConfig.objects) {
                    viewer->scene.objectTypeNames.append(obj.m_name);
                }
            }

            for (FormatHelpers::Stageconfig::ObjectInfo &obj : viewer->stageConfig.objects) {
                viewer->scene.objectTypeNames.append(obj.m_name);
            }
        }
        else {
            for (QString &obj : globals) {
                viewer->scene.objectTypeNames.append(obj);
            }

            for (FormatHelpers::Stageconfig::ObjectInfo &obj : viewer->stageConfig.objects) {
                viewer->scene.objectTypeNames.append(obj.m_name);
            }
        }

        ui->objectList->clear();
        ui->objectList->addItem("Blank Object");
        for (int o = 0; o < viewer->scene.objectTypeNames.count(); ++o)
            ui->objectList->addItem(viewer->scene.objectTypeNames[o]);

        m_objProp->typeBox->blockSignals(true);
        m_objProp->typeBox->clear();
        m_objProp->typeBox->addItem("Blank Object");
        for (int o = 0; o < viewer->scene.objectTypeNames.count(); ++o)
            m_objProp->typeBox->addItem(viewer->scene.objectTypeNames[o]);
        m_objProp->typeBox->blockSignals(false);

        createEntityList();

        if (viewer->gameType == ENGINE_v1) {
            m_scnProp->m_musBox->blockSignals(true);
            m_scnProp->m_musBox->clear();
            for (int m = 0; m < viewer->stageConfig.m_music.count(); ++m)
                m_scnProp->m_musBox->addItem(viewer->stageConfig.m_music[o]);
            m_scnProp->m_musBox->blockSignals(false);
        }
    });

    connect(m_scnProp->m_editPAL, &QPushButton::clicked, [this] {
        PaletteEditor *edit = new PaletteEditor(viewer->stageConfig.m_filename, viewer->gameType + 2);
        edit->setWindowTitle("Edit StageConfig Palette");
        edit->show();
    });

    connect(ui->exportRSDKv5, &QPushButton::clicked, [this] {
        ExportRSDKv5Scene *dlg = new ExportRSDKv5Scene(viewer->scene.filepath, this);
        if (dlg->exec() == QDialog::Accepted) {
            exportRSDKv5(dlg);
        }
    });

    connect(ui->exportSceneImg, &QPushButton::clicked, [this] {
        QFileDialog filedialog(this, tr("Save Image"), "", tr("PNG Files (*.png)"));
        filedialog.setAcceptMode(QFileDialog::AcceptSave);
        if (filedialog.exec() == QDialog::Accepted) {
            SceneExportImgOptions *dlg = new SceneExportImgOptions(this);
            if (dlg->exec() == QDialog::Accepted) {
                int w = 0;
                int h = 0;

                if (dlg->exportFG[0] || dlg->exportFG[1]) {
                    if (viewer->scene.width > w)
                        w = viewer->scene.width;
                    if (viewer->scene.width > h)
                        h = viewer->scene.height;
                }

                for (int i = 0; i < viewer->background.layers.count(); ++i) {
                    if (dlg->exportBG[i][0] || dlg->exportBG[i][1]) {
                        if (viewer->background.layers[i].width > w)
                            w = viewer->background.layers[i].width;
                        if (viewer->background.layers[i].height > h)
                            h = viewer->background.layers[i].height;
                    }
                }

                QImage image(w * 0x80, h * 0x80, QImage::Format_ARGB32);
                image.fill(0xFF00FF);
                QPainter painter(&image);

                for (int i = 0; i < 8; ++i) {
                    if (dlg->exportBG[i][0] || dlg->exportBG[i][1]) {
                        for (int y = 0; y < viewer->background.layers[i].height; ++y) {
                            for (int x = 0; x < viewer->background.layers[i].width; ++x) {
                                ushort chunk     = viewer->background.layers[i].layout[y][x];
                                QImage &chunkImg = viewer->chunks[chunk];

                                painter.drawImage(x * 0x80, y * 0x80, chunkImg);
                            }
                        }
                    }
                }

                if (dlg->exportFG[0] || dlg->exportFG[1]) {
                    for (int y = 0; y < viewer->scene.height; ++y) {
                        for (int x = 0; x < viewer->scene.width; ++x) {
                            ushort chunk     = viewer->scene.layout[y][x];
                            QImage &chunkImg = viewer->chunks[chunk];

                            painter.drawImage(x * 0x80, y * 0x80, chunkImg);
                        }
                    }
                }

                if (dlg->exportObjects) {
                    for (int o = 0; o < viewer->scene.objects.count(); ++o) {
                        FormatHelpers::Scene::Object &obj = viewer->scene.objects[o];

                        QString name = obj.type >= 1 ? viewer->scene.objectTypeNames[obj.type - 1]
                                                     : "Blank Object";

                        painter.drawText(obj.getX(), obj.getY(), name);
                        // painter.drawImage(obj.getX(), obj.getY(), m_mainView->m_missingObj);

                        if (dlg->exportObjInfo) {
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
                        }
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

SceneEditor::~SceneEditor() { delete ui; }

bool SceneEditor::event(QEvent *event)
{
    if (event->type() == (QEvent::Type)RE_EVENT_NEW) {
    }
    if (event->type() == (QEvent::Type)RE_EVENT_OPEN) {
        QList<QString> types = {
            "RSDKv4 Scenes (Act*.bin)",
            "RSDKv3 Scenes (Act*.bin)",
            "RSDKv2 Scenes (Act*.bin)",
            "RSDKv1 Scenes (Act*.map)",
        };

        QList<QString> gcTypes = {
            "RSDKv4 GameConfig (GameConfig*.bin)",
            "RSDKv3 GameConfig (GameConfig*.bin)",
            "RSDKv2 GameConfig (GameConfig*.bin)",
        };

        QFileDialog filedialog(this, tr("Open Scene"), "",
                               tr(QString("%1;;%2;;%3;;%4")
                                      .arg(types[0])
                                      .arg(types[1])
                                      .arg(types[2])
                                      .arg(types[3])
                                      .toStdString()
                                      .c_str()));
        filedialog.setAcceptMode(QFileDialog::AcceptOpen);
        if (filedialog.exec() == QDialog::Accepted) {
            int filter     = types.indexOf(filedialog.selectedNameFilter());
            QString gcPath = "";

            if (filter < 3) {
                QFileDialog gcdialog(this, tr("Open GameConfig"), "",
                                     tr(gcTypes[filter].toStdString().c_str()));
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
            }

            loadScene(filedialog.selectedFiles()[0], gcPath, filter + 1);
            return true;
        }
    }

    if (event->type() == (QEvent::Type)RE_EVENT_SAVE) {
        QString path = viewer->scene.filepath;

        if (QFile::exists(path)) {
            setStatus("Saving Scene...");
            QString basePath = path.replace(QFileInfo(path).fileName(), "");

            if (viewer->gameType != ENGINE_v1) {
                viewer->scene.write(viewer->gameType, viewer->scene.filepath);
                viewer->background.write(viewer->gameType, basePath + "Backgrounds.bin");
                viewer->chunkset.write(viewer->gameType, basePath + "128x128Tiles.bin");
                viewer->tileconfig.write(basePath + "CollisionMasks.bin");
                viewer->stageConfig.write(viewer->gameType, basePath + "StageConfig.bin");
            }
            else {
                viewer->scene.write(viewer->gameType, viewer->scene.filepath);
                viewer->background.write(viewer->gameType, basePath + "ZoneBG.map");
                viewer->chunkset.write(viewer->gameType, basePath + "Zone.til");
                viewer->m_tileconfigRS.write(basePath + "Zone.tcf");
                viewer->stageConfig.write(viewer->gameType, basePath + "Zone.zcf");
            }

            setStatus("Saved Scene: " + Utils::getFilenameAndFolder(viewer->scene.filepath));
            return true;
        }
        else {
            QList<QString> types = {
                "RSDKv4 Scenes (Act*.bin)",
                "RSDKv3 Scenes (Act*.bin)",
                "RSDKv2 Scenes (Act*.bin)",
                "RSDKv1 Scenes (Act*.map)",
            };

            QFileDialog filedialog(this, tr("Save Scene"), "",
                                   tr(QString("%1;;%2;;%3;;%4")
                                          .arg(types[0])
                                          .arg(types[1])
                                          .arg(types[2])
                                          .arg(types[3])
                                          .toStdString()
                                          .c_str()));
            filedialog.setAcceptMode(QFileDialog::AcceptSave);
            if (filedialog.exec() == QDialog::Accepted) {
                int filter = types.indexOf(filedialog.selectedNameFilter());

                setStatus("Saving Scene...");
                QString path     = filedialog.selectedFiles()[0];
                QString basePath = path.replace(QFileInfo(path).fileName(), "");

                switch (filter + 1) {
                    case ENGINE_v4:
                    case ENGINE_v3:
                    case ENGINE_v2:
                        viewer->scene.write(filter + 1, filedialog.selectedFiles()[0]);
                        viewer->background.write(filter + 1, basePath + "Backgrounds.bin");
                        viewer->chunkset.write(filter + 1, basePath + "128x128Tiles.bin");
                        viewer->tileconfig.write(basePath + "CollisionMasks.bin");
                        viewer->stageConfig.write(filter + 1, basePath + "StageConfig.bin");
                        break;
                    case ENGINE_v1:
                        viewer->scene.write(filter + 1, filedialog.selectedFiles()[0]);
                        viewer->background.write(filter + 1, basePath + "ZoneBG.map");
                        viewer->chunkset.write(filter + 1, basePath + "Zone.til");
                        viewer->m_tileconfigRS.write(basePath + "Zone.tcf");
                        viewer->stageConfig.write(filter + 1, basePath + "Zone.zcf");
                        break;
                }

                appConfig.addRecentFile(viewer->gameType, TOOL_SCENEEDITOR,
                                        filedialog.selectedFiles()[0],
                                        QList<QString>{ viewer->gameConfig.m_filename });
                setStatus("Saved Scene: " + Utils::getFilenameAndFolder(viewer->scene.filepath));
                return true;
            }
        }
    }
    if (event->type() == (QEvent::Type)RE_EVENT_SAVE_AS) {
        QList<QString> types = {
            "RSDKv4 Scenes (Act*.bin)",
            "RSDKv3 Scenes (Act*.bin)",
            "RSDKv2 Scenes (Act*.bin)",
            "RSDKv1 Scenes (Act*.map)",
        };

        QFileDialog filedialog(this, tr("Save Scene"), "",
                               tr(QString("%1;;%2;;%3;;%4")
                                      .arg(types[0])
                                      .arg(types[1])
                                      .arg(types[2])
                                      .arg(types[3])
                                      .toStdString()
                                      .c_str()));
        filedialog.setAcceptMode(QFileDialog::AcceptSave);
        if (filedialog.exec() == QDialog::Accepted) {
            int filter = types.indexOf(filedialog.selectedNameFilter());

            setStatus("Saving Scene...");
            QString path     = filedialog.selectedFiles()[0];
            QString basePath = path.replace(QFileInfo(path).fileName(), "");

            switch (filter + 1) {
                case ENGINE_v4:
                case ENGINE_v3:
                case ENGINE_v2:
                    viewer->scene.write(filter + 1, filedialog.selectedFiles()[0]);
                    viewer->background.write(filter + 1, basePath + "Backgrounds.bin");
                    viewer->chunkset.write(filter + 1, basePath + "128x128Tiles.bin");
                    viewer->tileconfig.write(basePath + "CollisionMasks.bin");
                    viewer->stageConfig.write(filter + 1, basePath + "StageConfig.bin");
                    break;
                case ENGINE_v1:
                    viewer->scene.write(filter + 1, filedialog.selectedFiles()[0]);
                    viewer->background.write(filter + 1, basePath + "ZoneBG.map");
                    viewer->chunkset.write(filter + 1, basePath + "Zone.til");
                    viewer->m_tileconfigRS.write(basePath + "Zone.tcf");
                    viewer->stageConfig.write(filter + 1, basePath + "Zone.zcf");
                    break;
            }

            appConfig.addRecentFile(viewer->gameType, TOOL_SCENEEDITOR, filedialog.selectedFiles()[0],
                                    QList<QString>{ viewer->gameConfig.m_filename });
            setStatus("Saved Scene: " + Utils::getFilenameAndFolder(filedialog.selectedFiles()[0]));
            return true;
        }
    }

    return QWidget::event(event);
}

bool SceneEditor::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::FocusIn || event->type() == QEvent::FocusOut) {
        QApplication::sendEvent(this, event);
        return false;
    }

    if (object != viewer)
        return false;

    auto setTile = [this](float x, float y) {
        if (viewer->selectedChunk < 0 || viewer->selectedLayer < 0)
            return;
        float tx = x;
        float ty = y;

        tx *= viewer->invZoom();
        ty *= viewer->invZoom();

        float tx2 = tx + fmodf(viewer->cam.pos.x, 0x80);
        float ty2 = ty + fmodf(viewer->cam.pos.y, 0x80);

        // clip to grid
        tx -= fmodf(tx2, 0x80);
        ty -= fmodf(ty2, 0x80);

        // Draw Selected Tile Preview
        float xpos = tx + viewer->cam.pos.x;
        float ypos = ty + viewer->cam.pos.y;

        xpos /= 0x80;
        ypos /= 0x80;
        if (ypos >= 0 && ypos < viewer->layerHeight(viewer->selectedLayer)) {
            if (xpos >= 0 && xpos < viewer->layerWidth(viewer->selectedLayer)) {
                if (viewer->selectedLayer > 0) {
                    viewer->background.layers[viewer->selectedLayer - 1].layout[ypos][xpos] =
                        viewer->selectedChunk;
                }
                else {
                    viewer->scene.layout[ypos][xpos] = viewer->selectedChunk;
                }
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
        viewer->m_selectedEntity = -1;
        m_objProp->unsetUI();
        viewer->m_selecting = false;

        unsetCursor();

        switch (tool) {
            default: break; // what
            case TOOL_MOUSE: break;
            case TOOL_SELECT: break;
            case TOOL_PENCIL: viewer->m_selecting = true; break;
            case TOOL_ERASER: viewer->m_selecting = true; break;
            case TOOL_ENTITY: viewer->m_selecting = true; break;
            case TOOL_PARALLAX: viewer->m_selecting = true; break;
        }
    };

    switch (event->type()) {
        case QEvent::MouseButtonPress: {
            QMouseEvent *mEvent = static_cast<QMouseEvent *>(event);
            viewer->m_reference = mEvent->pos();

            viewer->m_mousePos.x = viewer->cam.m_lastMousePos.x = mEvent->pos().x();
            viewer->m_mousePos.y = viewer->cam.m_lastMousePos.y = mEvent->pos().y();

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
                        if (viewer->selectedChunk >= 0 && viewer->m_selecting) {
                            setTile(mEvent->pos().x(), mEvent->pos().y());
                        }
                        else {
                            // click tile to move it or change properties
                            if (viewer->selectedLayer >= 0) {
                                Rect<float> box;

                                for (int y = 0; y < viewer->layerHeight(viewer->selectedLayer); ++y) {
                                    for (int x = 0; x < viewer->layerWidth(viewer->selectedLayer);
                                         ++x) {
                                        box = Rect<float>(x * 0x80, y * 0x80, 0x80, 0x80);

                                        Vector2<float> pos = Vector2<float>(
                                            (mEvent->pos().x() * viewer->invZoom()) + viewer->cam.pos.x,
                                            (mEvent->pos().y() * viewer->invZoom())
                                                + viewer->cam.pos.y);
                                        if (box.contains(pos)) {
                                            // idk but we're in the gaming zone now bitch!
                                            // int tid = m_mainView->layerLayout(
                                            //    m_mainView->m_selectedLayer)[y][x];
                                            break;
                                        }
                                    }
                                }
                            }
                        }
                        break;
                    }
                    case TOOL_ERASER: {
                        if (viewer->m_selecting) {
                            viewer->selectedChunk = 0x00;
                            setTile(mEvent->pos().x(), mEvent->pos().y());
                        }
                        break;
                    }
                    case TOOL_ENTITY: {
                        if (!viewer->m_selecting || viewer->selectedObject < 0) {
                            Rect<float> box;

                            for (int o = 0; o < viewer->scene.objects.count(); ++o) {
                                box = Rect<float>((viewer->scene.objects[o].getX() - 0x10),
                                                  (viewer->scene.objects[o].getY() - 0x10), 0x20, 0x20);

                                Vector2<float> pos = Vector2<float>(
                                    (mEvent->pos().x() * viewer->invZoom()) + viewer->cam.pos.x,
                                    (mEvent->pos().y() * viewer->invZoom()) + +viewer->cam.pos.y);
                                if (box.contains(pos)) {
                                    viewer->m_selectedEntity = o;

                                    m_objProp->setupUI(
                                        &viewer->scene.objects[viewer->m_selectedEntity],
                                        &viewer->m_compilerv3
                                             .m_objectEntityList[viewer->m_selectedEntity],
                                        &viewer->m_compilerv4
                                             .m_objectEntityList[viewer->m_selectedEntity],
                                        viewer->gameType);
                                    ui->propertiesBox->setCurrentWidget(ui->objPropPage);
                                    break;
                                }
                            }
                        }
                        else {
                            if (viewer->selectedObject >= 0) {
                                FormatHelpers::Scene::Object obj;
                                obj.type = viewer->selectedObject;
                                int xpos = (mEvent->pos().x() * viewer->invZoom()) + viewer->cam.pos.x;
                                xpos <<= 16;
                                int ypos = (mEvent->pos().y() * viewer->invZoom()) + viewer->cam.pos.y;
                                ypos <<= 16;

                                obj.setX((mEvent->pos().x() * viewer->invZoom()) + viewer->cam.pos.x);
                                obj.setY((mEvent->pos().y() * viewer->invZoom()) + viewer->cam.pos.y);

                                int cnt = viewer->scene.objects.count();
                                viewer->scene.objects.append(obj);
                                viewer->m_compilerv3.m_objectEntityList[cnt].type =
                                    viewer->selectedObject;
                                viewer->m_compilerv3.m_objectEntityList[cnt].propertyValue = 0;
                                viewer->m_compilerv3.m_objectEntityList[cnt].XPos          = xpos;
                                viewer->m_compilerv3.m_objectEntityList[cnt].YPos          = ypos;

                                viewer->m_compilerv4.m_objectEntityList[cnt].type =
                                    viewer->selectedObject;
                                viewer->m_compilerv4.m_objectEntityList[cnt].propertyValue = 0;
                                viewer->m_compilerv4.m_objectEntityList[cnt].XPos          = xpos;
                                viewer->m_compilerv4.m_objectEntityList[cnt].YPos          = ypos;
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
                            Rect<float> box, box2;

                            for (int y = 0; y < viewer->layerHeight(viewer->selectedLayer); ++y) {
                                for (int x = 0; x < viewer->layerWidth(viewer->selectedLayer); ++x) {
                                    box = Rect<float>(x * 0x80, y * 0x80, 0x80, 0x80);

                                    Vector2<float> pos = Vector2<float>(
                                        (mEvent->pos().x() * viewer->invZoom()) + viewer->cam.pos.x,
                                        (mEvent->pos().y() * viewer->invZoom()) + viewer->cam.pos.y);
                                    if (box.contains(pos)) {
                                        ushort tid = 0;
                                        // idk but we're in the gaming zone now bitch!
                                        ushort chunk = viewer->selectedChunk =
                                            viewer->layerLayout(viewer->selectedLayer)[y][x];

                                        for (int cy = 0; cy < 8; ++cy) {
                                            for (int cx = 0; cx < 8; ++cx) {
                                                box2 = Rect<float>(box.x + (cx * 0x10),
                                                                   box.y + (cy * 0x10), 0x10, 0x10);
                                                if (box2.contains(pos)) {
                                                    tid = viewer->chunkset.chunks[chunk]
                                                              .tiles[cy][cx]
                                                              .tileIndex;

                                                    m_tileProp->setupUI(
                                                        &viewer->tileconfig.collisionPaths[0][tid],
                                                        &viewer->tileconfig.collisionPaths[1][tid], tid,
                                                        viewer->tiles[tid]);
                                                    ui->propertiesBox->setCurrentIndex(PROP_TILE);
                                                    break;
                                                }
                                            }
                                        }
                                        break;
                                    }
                                }
                            }
                        }
                        break;
                    case TOOL_ENTITY: {
                        Rect<float> box;
                        bool found = false;

                        for (int o = 0; o < viewer->scene.objects.count(); ++o) {
                            box = Rect<float>((viewer->scene.objects[o].getX() - 0x10),
                                              (viewer->scene.objects[o].getY() - 0x10), 0x20, 0x20);

                            Vector2<float> pos = Vector2<float>(
                                (mEvent->pos().x() * viewer->invZoom()) + viewer->cam.pos.x,
                                (mEvent->pos().y() * viewer->invZoom()) + +viewer->cam.pos.y);
                            if (box.contains(pos)) {
                                viewer->m_selectedEntity = o;
                                found                    = true;

                                m_objProp->setupUI(
                                    &viewer->scene.objects[viewer->m_selectedEntity],
                                    &viewer->m_compilerv3.m_objectEntityList[viewer->m_selectedEntity],
                                    &viewer->m_compilerv4.m_objectEntityList[viewer->m_selectedEntity],
                                    viewer->gameType);
                                ui->propertiesBox->setCurrentWidget(ui->objPropPage);
                                break;
                            }
                        }

                        if (!found) {
                            viewer->m_selectedEntity = -1;
                            viewer->selectedObject   = -1;
                            ui->objectList->setCurrentRow(-1);
                            ui->entityList->setCurrentRow(-1);

                            m_objProp->unsetUI();
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

            viewer->m_mousePos.x = viewer->cam.m_lastMousePos.x = mEvent->pos().x();
            viewer->m_mousePos.y = viewer->cam.m_lastMousePos.y = mEvent->pos().y();

            Vector2<int> m_sceneMousePos((viewer->m_mousePos.x * viewer->invZoom()) + viewer->cam.pos.x,
                                         (viewer->m_mousePos.y * viewer->invZoom())
                                             + viewer->cam.pos.y);

            if (m_mouseDownM || (m_mouseDownL && viewer->curTool == TOOL_MOUSE)) {
                viewer->cam.pos.x -=
                    (viewer->m_mousePos.x - viewer->m_reference.x()) * viewer->invZoom();
                viewer->cam.pos.y -=
                    (viewer->m_mousePos.y - viewer->m_reference.y()) * viewer->invZoom();
                viewer->m_reference = mEvent->pos();
                status              = true;

                ui->horizontalScrollBar->blockSignals(true);
                ui->horizontalScrollBar->setMaximum((viewer->scene.width * 0x80) - viewer->storedW);
                ui->horizontalScrollBar->setValue(viewer->cam.pos.x);
                ui->horizontalScrollBar->blockSignals(false);

                ui->verticalScrollBar->blockSignals(true);
                ui->verticalScrollBar->setMaximum((viewer->scene.height * 0x80) - viewer->storedH);
                ui->verticalScrollBar->setValue(viewer->cam.pos.y);
                ui->verticalScrollBar->blockSignals(false);
            }

            if (viewer->curTool == TOOL_PENCIL || viewer->curTool == TOOL_ENTITY) {
                viewer->m_tilePos.x = viewer->m_mousePos.x;
                viewer->m_tilePos.y = viewer->m_mousePos.y;

                if (m_ctrlDownL) {
                    viewer->m_tilePos.x -= fmodf(viewer->m_tilePos.x + (0x80 / 2), 0x80);
                    viewer->m_tilePos.y -= fmodf(viewer->m_tilePos.y + (0x80 / 2), 0x80);

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
                        if (viewer->selectedChunk >= 0 && viewer->m_selecting) {
                            setTile(viewer->m_mousePos.x, viewer->m_mousePos.y);
                        }
                        break;
                    }
                    case TOOL_ERASER: {
                        if (viewer->m_selecting) {
                            viewer->selectedChunk = 0x0;
                            setTile(viewer->m_mousePos.x, viewer->m_mousePos.y);
                        }
                        break;
                    }
                    case TOOL_ENTITY: {
                        if (viewer->selectedObject < 0 && viewer->m_selectedEntity >= 0) {
                            FormatHelpers::Scene::Object &object =
                                viewer->scene.objects[viewer->m_selectedEntity];

                            object.setX((viewer->m_mousePos.x * viewer->invZoom()) + viewer->cam.pos.x);
                            object.setY((viewer->m_mousePos.y * viewer->invZoom()) + viewer->cam.pos.y);

                            if (m_ctrlDownL) {
                                object.setX(object.getX() - fmodf(object.getX(), m_snapSize.x));
                                object.setY(object.getY() - fmodf(object.getY(), m_snapSize.y));
                            }
                            viewer->m_compilerv3.m_objectEntityList[viewer->m_selectedEntity].XPos =
                                object.m_position.x;
                            viewer->m_compilerv3.m_objectEntityList[viewer->m_selectedEntity].YPos =
                                object.m_position.y;

                            viewer->m_compilerv4.m_objectEntityList[viewer->m_selectedEntity].XPos =
                                object.m_position.x;
                            viewer->m_compilerv4.m_objectEntityList[viewer->m_selectedEntity].YPos =
                                object.m_position.y;
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
            QMouseEvent *mEvent  = static_cast<QMouseEvent *>(event);
            viewer->m_mousePos.x = mEvent->pos().x();
            viewer->m_mousePos.y = mEvent->pos().y();

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

            Vector2<int> m_sceneMousePos((viewer->m_mousePos.x * viewer->invZoom()) + viewer->cam.pos.x,
                                         (viewer->m_mousePos.y * viewer->invZoom())
                                             + viewer->cam.pos.y);

            if (kEvent->key() == Qt::Key_Control)
                m_ctrlDownL = true;
            if (kEvent->key() == Qt::Key_Alt)
                m_altDownL = true;
            if (kEvent->key() == Qt::Key_Shift)
                m_shiftDownL = true;

            if ((kEvent->modifiers() & Qt::ControlModifier) == Qt::ControlModifier
                && kEvent->key() == Qt::Key_V) {
                if (m_clipboard) {
                    switch (m_clipboardType) {
                        default: break;
                        case COPY_ENTITY: {
                            FormatHelpers::Scene::Object object =
                                *(FormatHelpers::Scene::Object *)m_clipboard;
                            object.setX(m_sceneMousePos.x);
                            object.setY(m_sceneMousePos.y);

                            int cnt = viewer->scene.objects.count();
                            viewer->scene.objects.append(object);
                            viewer->m_compilerv3.m_objectEntityList[cnt].XPos = object.m_position.x;
                            viewer->m_compilerv3.m_objectEntityList[cnt].YPos = object.m_position.y;

                            viewer->m_compilerv4.m_objectEntityList[cnt].XPos = object.m_position.x;
                            viewer->m_compilerv4.m_objectEntityList[cnt].YPos = object.m_position.y;
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
                        viewer->tileFlip.x = true;

                    if (kEvent->key() == Qt::Key_X)
                        viewer->tileFlip.y = true;

                    if (kEvent->key() == Qt::Key_Delete || kEvent->key() == Qt::Key_Backspace) {
                        if (viewer->selectedLayer >= 0) {
                            Rect<float> box;

                            for (int y = 0; y < viewer->layerHeight(viewer->selectedLayer); ++y) {
                                for (int x = 0; x < viewer->layerWidth(viewer->selectedLayer); ++x) {
                                    box = Rect<float>(x * 0x80, y * 0x80, 0x80, 0x80);

                                    Vector2<float> pos = Vector2<float>(
                                        (viewer->m_tilePos.x * viewer->invZoom()) + viewer->cam.pos.x,
                                        (viewer->m_tilePos.y * viewer->invZoom()) + viewer->cam.pos.y);
                                    if (box.contains(pos)) {
                                        if (viewer->selectedLayer > 0) {
                                            viewer->background.layers[viewer->selectedLayer - 1]
                                                .layout[y][x] = 0;
                                        }
                                        else {
                                            viewer->scene.layout[y][x] = 0;
                                        }

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
                        if (viewer->m_selectedEntity >= 0) {
                            viewer->scene.objects.removeAt(viewer->m_selectedEntity);
                            viewer->m_selectedEntity = -1;

                            m_objProp->unsetUI();
                            createEntityList();
                        }
                    }

                    if ((kEvent->modifiers() & Qt::ControlModifier) == Qt::ControlModifier
                        && kEvent->key() == Qt::Key_C) {
                        if (viewer->m_selectedEntity >= 0) {
                            m_clipboard     = &viewer->scene.objects[viewer->m_selectedEntity];
                            m_clipboardType = COPY_ENTITY;
                            m_clipboardInfo = viewer->m_selectedEntity;
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
                m_ctrlDownL = false;
            if (kEvent->key() == Qt::Key_Alt)
                m_altDownL = false;
            if (kEvent->key() == Qt::Key_Shift)
                m_shiftDownL = false;
            break;
        }

        default: return false;
    }

    return true;
}

void SceneEditor::loadScene(QString scnPath, QString gcfPath, byte gameType)
{

    setStatus("Loading Scene...");

    if (gcfPath != viewer->gameConfig.m_filename)
        viewer->gameConfig.read(gameType, gcfPath);
    QString dataPath = QFileInfo(gcfPath).absolutePath();
    QDir dir(dataPath);
    dir.cdUp();
    viewer->m_dataPath = dir.path();

    viewer->loadScene(scnPath, gameType);

    ui->layerList->clear();
    ui->layerList->addItem("Foreground");

    for (int l = 0; l < viewer->background.layers.count(); ++l)
        ui->layerList->addItem("Background " + QString::number(l + 1));

    ui->objectList->clear();
    ui->objectList->addItem("Blank Object");
    for (int o = 0; o < viewer->scene.objectTypeNames.count(); ++o)
        ui->objectList->addItem(viewer->scene.objectTypeNames[o]);

    createEntityList();

    viewer->gameType = gameType;

    ui->horizontalScrollBar->setMaximum((viewer->scene.width * 0x80) - viewer->storedW);
    ui->verticalScrollBar->setMaximum((viewer->scene.height * 0x80) - viewer->storedH);
    ui->horizontalScrollBar->setPageStep(0x80);
    ui->verticalScrollBar->setPageStep(0x80);

    m_objProp->typeBox->clear();
    m_objProp->typeBox->addItem("Blank Object");
    for (int o = 0; o < viewer->scene.objectTypeNames.count(); ++o)
        m_objProp->typeBox->addItem(viewer->scene.objectTypeNames[o]);

    if (viewer->gameType == ENGINE_v1) {
        m_scnProp->m_musBox->clear();

        for (int m = 0; m < viewer->stageConfig.m_music.count(); ++m)
            m_scnProp->m_musBox->addItem(viewer->stageConfig.m_music[m]);
    }
    m_scnProp->setupUI(&viewer->scene, viewer->gameType);

    m_scnProp->loadGlobalCB->blockSignals(true);
    m_scnProp->loadGlobalCB->setDisabled(viewer->gameType == ENGINE_v1);
    m_scnProp->loadGlobalCB->setChecked(viewer->stageConfig.loadGlobalScripts);
    m_scnProp->loadGlobalCB->blockSignals(false);

    if (m_chkProp) {
        ui->chunksPage->layout()->removeWidget(m_chkProp);
        delete m_chkProp;
    }

    m_chkProp = new ChunkSelector(this);
    m_chkProp->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->chunksPage->layout()->addWidget(m_chkProp);
    m_chkProp->show();

    ui->layerList->setCurrentRow(0);
    viewer->selectedLayer = 0;

    ui->toolBox->setCurrentIndex(0);
    ui->propertiesBox->setCurrentIndex(0);

    m_scnProp->setupUI(&viewer->scene, viewer->gameType);
    m_lyrProp->unsetUI();
    m_tileProp->unsetUI();
    m_objProp->unsetUI();
    m_scrProp->unsetUI();

    for (int i = 0; i < viewer->scene.objects.count(); ++i) {
        viewer->m_compilerv3.m_objectEntityList[i].type = viewer->scene.objects[i].type;
        viewer->m_compilerv3.m_objectEntityList[i].propertyValue =
            viewer->scene.objects[i].m_propertyValue;
        viewer->m_compilerv3.m_objectEntityList[i].XPos = viewer->scene.objects[i].m_position.x;
        viewer->m_compilerv3.m_objectEntityList[i].YPos = viewer->scene.objects[i].m_position.y;

        viewer->m_compilerv4.m_objectEntityList[i].type = viewer->scene.objects[i].type;
        viewer->m_compilerv4.m_objectEntityList[i].propertyValue =
            viewer->scene.objects[i].m_propertyValue;
        viewer->m_compilerv4.m_objectEntityList[i].XPos = viewer->scene.objects[i].m_position.x;
        viewer->m_compilerv4.m_objectEntityList[i].YPos = viewer->scene.objects[i].m_position.y;

        for (int v = 0; v < 8; ++v) viewer->m_compilerv3.m_objectEntityList[i].values[v] = 0;
    }

    viewer->m_compilerv3.clearScriptData();
    viewer->m_compilerv4.clearScriptData();
    int id                                 = 0;
    viewer->m_compilerv3.m_typeNames[id]   = "Blank Object";
    viewer->m_compilerv4.m_typeNames[id++] = "Blank Object";
    for (int o = 0; o < viewer->gameConfig.objects.count(); ++o) {
        viewer->m_compilerv3.m_typeNames[id] = viewer->gameConfig.objects[o].m_name;
        viewer->m_compilerv4.m_typeNames[id] = viewer->gameConfig.objects[o].m_name;

        viewer->m_compilerv3.m_typeNames[id].replace(" ", "");
        viewer->m_compilerv4.m_typeNames[id].replace(" ", "");

        id++;
    }
    for (int o = 0; o < viewer->stageConfig.objects.count(); ++o) {
        viewer->m_compilerv3.m_typeNames[id] = viewer->stageConfig.objects[o].m_name;
        viewer->m_compilerv4.m_typeNames[id] = viewer->stageConfig.objects[o].m_name;

        viewer->m_compilerv3.m_typeNames[id].replace(" ", "");
        viewer->m_compilerv4.m_typeNames[id].replace(" ", "");

        id++;
    }

    switch (gameType) {
        case ENGINE_v1: break; // read the editor stuff from this somehow (idk how to parse it lol)
        case ENGINE_v2: break; // parse the RSDK sub and use that data to know what to draw
        case ENGINE_v3: {      // compiler RSDKDraw & RSDKLoad and draw via those
            int scrID                     = 1;
            viewer->m_compilerv3.m_viewer = viewer;

            if (viewer->stageConfig.loadGlobalScripts) {
                for (int i = 0; i < viewer->gameConfig.objects.count(); ++i) {
                    viewer->m_compilerv3.parseScriptFile(viewer->m_dataPath + "/Scripts/"
                                                             + viewer->gameConfig.objects[i].m_script,
                                                         scrID++);

                    if (viewer->m_compilerv3.m_scriptError) {
                        qDebug() << viewer->m_compilerv3.errorMsg;
                        qDebug() << viewer->m_compilerv3.errorPos;
                        qDebug() << QString::number(viewer->m_compilerv3.m_errorLine);

                        QFileInfo info(viewer->m_compilerv3.m_errorScr);
                        QDir dir(info.dir());
                        dir.cdUp();
                        QString dirFile = dir.relativeFilePath(viewer->m_compilerv3.m_errorScr);

                        setStatus("Failed to compile script: " + dirFile);
                        viewer->m_compilerv3.m_objectScriptList[scrID - 1].subRSDKDraw.m_scriptCodePtr =
                            SCRIPTDATA_COUNT - 1;
                        viewer->m_compilerv3.m_objectScriptList[scrID - 1].subRSDKDraw.m_jumpTablePtr =
                            JUMPTABLE_COUNT - 1;
                        viewer->m_compilerv3.m_objectScriptList[scrID - 1].subRSDKLoad.m_scriptCodePtr =
                            SCRIPTDATA_COUNT - 1;
                        viewer->m_compilerv3.m_objectScriptList[scrID - 1].subRSDKLoad.m_jumpTablePtr =
                            JUMPTABLE_COUNT - 1;
                    }
                }
            }

            for (int i = 0; i < viewer->stageConfig.objects.count(); ++i) {
                viewer->m_compilerv3.parseScriptFile(viewer->m_dataPath + "/Scripts/"
                                                         + viewer->stageConfig.objects[i].m_script,
                                                     scrID++);

                if (viewer->m_compilerv3.m_scriptError) {
                    qDebug() << viewer->m_compilerv3.errorMsg;
                    qDebug() << viewer->m_compilerv3.errorPos;
                    qDebug() << QString::number(viewer->m_compilerv3.m_errorLine);

                    QFileInfo info(viewer->m_compilerv3.m_errorScr);
                    QDir dir(info.dir());
                    dir.cdUp();
                    QString dirFile = dir.relativeFilePath(viewer->m_compilerv3.m_errorScr);

                    setStatus("Failed to compile script: " + dirFile);
                    viewer->m_compilerv3.m_objectScriptList[scrID - 1].subRSDKDraw.m_scriptCodePtr =
                        SCRIPTDATA_COUNT - 1;
                    viewer->m_compilerv3.m_objectScriptList[scrID - 1].subRSDKDraw.m_jumpTablePtr =
                        JUMPTABLE_COUNT - 1;
                    viewer->m_compilerv3.m_objectScriptList[scrID - 1].subRSDKLoad.m_scriptCodePtr =
                        SCRIPTDATA_COUNT - 1;
                    viewer->m_compilerv3.m_objectScriptList[scrID - 1].subRSDKLoad.m_jumpTablePtr =
                        JUMPTABLE_COUNT - 1;
                }
            }

            viewer->m_compilerv3.m_objectLoop = ENTITY_COUNT - 1;
            for (int o = 0; o < OBJECT_COUNT; ++o) {
                auto &curObj           = viewer->m_compilerv3.m_objectScriptList[o];
                curObj.frameListOffset = viewer->m_compilerv3.scriptFrameCount;
                curObj.spriteSheetID   = 0;
                viewer->m_compilerv3.m_objectEntityList[ENTITY_COUNT - 1].type = o;

                auto &curSub = curObj.subRSDKLoad;
                if (curSub.m_scriptCodePtr != SCRIPTDATA_COUNT - 1) {
                    viewer->m_compilerv3.processScript(curSub.m_scriptCodePtr, curSub.m_jumpTablePtr,
                                                       Compilerv3::SUB_RSDKLOAD);
                }
                curObj.spriteFrameCount =
                    viewer->m_compilerv3.scriptFrameCount - curObj.frameListOffset;
            }
            break;
        }
        case ENGINE_v4: { // compiler RSDKDraw & RSDKLoad and draw via those
            int scrID                     = 1;
            viewer->m_compilerv4.m_viewer = viewer;

            if (viewer->stageConfig.loadGlobalScripts) {
                for (int i = 0; i < viewer->gameConfig.objects.count(); ++i) {
                    viewer->m_compilerv4.parseScriptFile(viewer->m_dataPath + "/../Scripts/"
                                                             + viewer->gameConfig.objects[i].m_script,
                                                         scrID++);

                    if (viewer->m_compilerv4.m_scriptError) {
                        qDebug() << viewer->m_compilerv4.errorMsg;
                        qDebug() << viewer->m_compilerv4.errorPos;
                        qDebug() << QString::number(viewer->m_compilerv4.m_errorLine);

                        QFileInfo info(viewer->m_compilerv4.m_errorScr);
                        QDir dir(info.dir());
                        dir.cdUp();
                        QString dirFile = dir.relativeFilePath(viewer->m_compilerv4.m_errorScr);

                        setStatus("Failed to compile script: " + dirFile);
                        viewer->m_compilerv4.m_objectScriptList[scrID - 1]
                            .eventRSDKDraw.m_scriptCodePtr = SCRIPTDATA_COUNT_v4 - 1;
                        viewer->m_compilerv4.m_objectScriptList[scrID - 1]
                            .eventRSDKDraw.m_jumpTablePtr = JUMPTABLE_COUNT_v4 - 1;
                        viewer->m_compilerv4.m_objectScriptList[scrID - 1]
                            .eventRSDKLoad.m_scriptCodePtr = SCRIPTDATA_COUNT_v4 - 1;
                        viewer->m_compilerv4.m_objectScriptList[scrID - 1]
                            .eventRSDKLoad.m_jumpTablePtr = JUMPTABLE_COUNT_v4 - 1;
                    }
                }
            }

            for (int i = 0; i < viewer->stageConfig.objects.count(); ++i) {
                viewer->m_compilerv4.parseScriptFile(viewer->m_dataPath + "/../Scripts/"
                                                         + viewer->stageConfig.objects[i].m_script,
                                                     scrID++);

                if (viewer->m_compilerv4.m_scriptError) {
                    qDebug() << viewer->m_compilerv4.errorMsg << "\n";
                    qDebug() << viewer->m_compilerv4.errorPos << "\n";
                    qDebug() << QString::number(viewer->m_compilerv4.m_errorLine) << "\n";

                    QFileInfo info(viewer->m_compilerv4.m_errorScr);
                    QDir dir(info.dir());
                    dir.cdUp();
                    QString dirFile = dir.relativeFilePath(viewer->m_compilerv4.m_errorScr);

                    setStatus("Failed to compile script: " + dirFile);
                    viewer->m_compilerv4.m_objectScriptList[scrID - 1].eventRSDKDraw.m_scriptCodePtr =
                        SCRIPTDATA_COUNT_v4 - 1;
                    viewer->m_compilerv4.m_objectScriptList[scrID - 1].eventRSDKDraw.m_jumpTablePtr =
                        JUMPTABLE_COUNT_v4 - 1;
                    viewer->m_compilerv4.m_objectScriptList[scrID - 1].eventRSDKLoad.m_scriptCodePtr =
                        SCRIPTDATA_COUNT_v4 - 1;
                    viewer->m_compilerv4.m_objectScriptList[scrID - 1].eventRSDKLoad.m_jumpTablePtr =
                        JUMPTABLE_COUNT_v4 - 1;
                }
            }

            viewer->m_compilerv4.m_objectEntityPos = ENTITY_COUNT - 1;
            for (int o = 0; o < OBJECT_COUNT; ++o) {
                auto &curObj           = viewer->m_compilerv4.m_objectScriptList[o];
                curObj.frameListOffset = viewer->m_compilerv4.scriptFrameCount;
                curObj.spriteSheetID   = 0;
                viewer->m_compilerv4.m_objectEntityList[ENTITY_COUNT - 1].type = o;

                auto &curSub = curObj.eventRSDKLoad;
                if (curSub.m_scriptCodePtr != SCRIPTDATA_COUNT - 1) {
                    viewer->m_compilerv4.processScript(curSub.m_scriptCodePtr, curSub.m_jumpTablePtr,
                                                       Compilerv4::EVENT_RSDKLOAD);
                }
                curObj.spriteFrameCount =
                    viewer->m_compilerv4.scriptFrameCount - curObj.frameListOffset;
            }
            break;
        }
    }

    appConfig.addRecentFile(viewer->gameType, TOOL_SCENEEDITOR, scnPath, QList<QString>{ gcfPath });
    setStatus("Loaded Scene: " + QFileInfo(scnPath).fileName());
}
void SceneEditor::createEntityList()
{
    ui->entityList->blockSignals(true);
    ui->entityList->clear();

    // C++11 abute poggers
    std::sort(viewer->scene.objects.begin(), viewer->scene.objects.end(),
              [](const FormatHelpers::Scene::Object &a, const FormatHelpers::Scene::Object &b) -> bool {
                  return a.m_id < b.m_id;
              });
    for (int i = 0; i < viewer->scene.objects.count(); ++i) {
        QString name = viewer->scene.objects[i].type >= 1
                           ? viewer->scene.objectTypeNames[viewer->scene.objects[i].type - 1]
                           : "Blank Object";
        //-1 because "blank object" is the internal type 0
        ui->entityList->addItem(QString::number(viewer->scene.objects[i].m_id) + ": " + name);
    }
    ui->entityList->blockSignals(false);
}

void SceneEditor::createScrollList()
{
    ui->scrollList->clear();

    if (viewer->selectedLayer < 1 || viewer->selectedLayer >= 9)
        return;

    ui->scrollList->blockSignals(true);
    for (int i = 0; i < viewer->background.layers[viewer->selectedLayer - 1].scrollInfos.count(); ++i) {
        FormatHelpers::Background::ScrollIndexInfo &info =
            viewer->background.layers[viewer->selectedLayer - 1].scrollInfos[i];
        ui->scrollList->addItem(QString("Start: %1, Length %2").arg(info.startLine).arg(info.length));
    }

    ui->scrollList->blockSignals(false);
    ui->scrollList->setCurrentRow(-1);
}

void SceneEditor::exportRSDKv5(ExportRSDKv5Scene *dlg)
{
    if (!QDir("").exists(dlg->m_outputPath))
        return;

    RSDKv5::Scene scene;
    RSDKv5::TileConfig tileConfig;
    RSDKv5::StageConfig stageConfig;

    if (dlg->exportStageConfig) {
        stageConfig.loadGlobalObjects = viewer->stageConfig.loadGlobalScripts;

        stageConfig.objects.clear();
        for (auto o : viewer->stageConfig.objects) {
            QString name = o.m_name;
            stageConfig.objects.append(name.replace(" ", ""));
        }

        stageConfig.soundFX.clear();
        for (auto s : viewer->stageConfig.m_soundFX)
            stageConfig.soundFX.append(RSDKv5::StageConfig::WAVConfiguration(s.m_path, 1));

        stageConfig.m_palettes[0].activeRows[6] = true;
        stageConfig.m_palettes[0].activeRows[7] = true;

        for (int c = 0; c < 0x20; ++c) {
            stageConfig.m_palettes[0].colours[(c / 0x10) + 0x60][c % 0x10].setRed(
                viewer->stageConfig.m_stagePalette.colours[c].r);
            stageConfig.m_palettes[0].colours[(c / 0x10) + 0x60][c % 0x10].setGreen(
                viewer->stageConfig.m_stagePalette.colours[c].g);
            stageConfig.m_palettes[0].colours[(c / 0x10) + 0x60][c % 0x10].setBlue(
                viewer->stageConfig.m_stagePalette.colours[c].b);
        }

        stageConfig.write(dlg->m_outputPath + "/StageConfig.bin");
    }

    if (dlg->exportTileConfig) {
        for (int c = 0; c < 2; ++c) {
            for (int t = 0; t < 0x400; ++t) {
                // TODO:: retro sonic

                if (viewer->gameType != ENGINE_v1) {
                    tileConfig.collisionPaths[c][t].behaviour =
                        viewer->tileconfig.collisionPaths[c][t].behaviour;
                    tileConfig.collisionPaths[c][t].flipY =
                        viewer->tileconfig.collisionPaths[c][t].flipY;
                    tileConfig.collisionPaths[c][t].floorAngle =
                        viewer->tileconfig.collisionPaths[c][t].floorAngle;
                    tileConfig.collisionPaths[c][t].lWallAngle =
                        viewer->tileconfig.collisionPaths[c][t].lWallAngle;
                    tileConfig.collisionPaths[c][t].rWallAngle =
                        viewer->tileconfig.collisionPaths[c][t].rWallAngle;
                    tileConfig.collisionPaths[c][t].roofAngle =
                        viewer->tileconfig.collisionPaths[c][t].roofAngle;

                    for (int h = 0; h < 0x10; ++h) {
                        tileConfig.collisionPaths[c][t].collision[h].height =
                            viewer->tileconfig.collisionPaths[c][t].collision[c].height;
                        tileConfig.collisionPaths[c][t].collision[h].solid =
                            viewer->tileconfig.collisionPaths[c][t].collision[c].solid;
                    }
                }
                else {
                }
            }
        }

        tileConfig.write(dlg->m_outputPath + "/TileConfig.bin");
    }

    if (dlg->exportChunks) {
        // TODO:
    }

    scene.layers.clear();
    if (dlg->exportLayers) {
        for (int l = 0; l < 9; ++l) {
            QString layerName = !l ? "FG " : QString("BG %1 ").arg(l);
            for (int p = 0; p < 2; ++p) {
                if (!viewer->layerWidth(l) || !viewer->layerHeight(l))
                    continue;

                RSDKv5::Scene::SceneLayer layer;
                layer.m_name = layerName + (!p ? "Low" : "High");
                layer.resize(viewer->layerWidth(l) * 8, viewer->layerHeight(l) * 8);
                layer.lineIndexes.clear();
                layer.drawOrder = 16;

                switch (viewer->gameType) {
                    case ENGINE_v4:
                    case ENGINE_v3:
                    case ENGINE_v2: {
                        for (int i = 0; i < 4; ++i) {
                            if (l == viewer->scene.activeLayer[i]) {
                                byte vPlane = i >= viewer->scene.midpoint;

                                if (vPlane == p) {
                                    if (viewer->scene.midpoint < 3 && viewer->gameType == ENGINE_v4) {
                                        switch (i) {
                                            case 0: layer.drawOrder = 1; break;
                                            case 1: layer.drawOrder = 2; break;
                                            case 2: layer.drawOrder = 5; break;
                                            case 3: layer.drawOrder = 5; break;
                                        }
                                    }
                                    else {
                                        switch (i) {
                                            case 0: layer.drawOrder = 1; break;
                                            case 1: layer.drawOrder = 2; break;
                                            case 2: layer.drawOrder = 3; break;
                                            case 3: layer.drawOrder = 5; break;
                                        }
                                    }
                                }
                            }
                        }
                        break;
                    }
                    case ENGINE_v1: {
                        // None, let user decide
                        break;
                    }
                }

                layer.type           = 0;
                layer.parallaxFactor = 1.0 * 0x100;
                layer.scrollSpeed    = 0.0 * 0x100;

                if (l > 0) {
                    layer.parallaxFactor = viewer->background.layers[l - 1].parallaxFactor * 0x100;
                    layer.scrollSpeed    = viewer->background.layers[l - 1].scrollSpeed * 0x100;
                }

                layer.scrollInfos.clear();
                if (l) {
                    if (viewer->background.layers[l - 1].scrollInfos.count() == 0) {
                        RSDKv5::Scene::ScrollIndexInfo info;
                        info.startLine      = 0;
                        info.length         = layer.height * 16;
                        info.parallaxFactor = 1.0f;
                        info.scrollSpeed    = 0.0f;
                        info.deform         = false;
                        layer.scrollInfos.append(info);
                    }
                    else {
                        for (int s = 0; s < viewer->background.layers[l - 1].scrollInfos.count(); ++s) {
                            RSDKv5::Scene::ScrollIndexInfo info;
                            info.startLine = viewer->background.layers[l - 1].scrollInfos[s].startLine;
                            info.length    = viewer->background.layers[l - 1].scrollInfos[s].length;
                            info.parallaxFactor =
                                viewer->background.layers[l - 1].scrollInfos[s].parallaxFactor;
                            info.scrollSpeed =
                                viewer->background.layers[l - 1].scrollInfos[s].scrollSpeed;
                            info.deform = viewer->background.layers[l - 1].scrollInfos[s].deform;
                            layer.scrollInfos.append(info);
                        }
                    }
                }
                else {
                    RSDKv5::Scene::ScrollIndexInfo info;
                    info.startLine      = 0;
                    info.length         = layer.height * 16;
                    info.parallaxFactor = 1.0f;
                    info.scrollSpeed    = 0.0f;
                    info.deform         = false;
                    layer.scrollInfos.append(info);
                }

                int cnt = 0;
                for (int y = 0; y < viewer->layerHeight(l); ++y) {
                    for (int x = 0; x < viewer->layerWidth(l); ++x) {
                        int chunkID                         = viewer->layerLayout(l)[y][x];
                        FormatHelpers::Chunks::Chunk &chunk = viewer->chunkset.chunks[chunkID];
                        for (int ty = 0; ty < 8; ++ty) {
                            for (int tx = 0; tx < 8; ++tx) {
                                // check we're on the right plane
                                if (chunk.tiles[ty][tx].visualPlane != p) {
                                    layer.layout[(y * 8) + ty][(x * 8) + tx] = 0xFFFF;
                                    continue;
                                }
                                cnt++;

                                ushort tile = chunk.tiles[ty][tx].tileIndex;
                                byte solA   = chunk.tiles[ty][tx].solidityA;
                                byte solB   = chunk.tiles[ty][tx].solidityB;

                                if (solA == 4)
                                    solA = 1;
                                if (solB == 4)
                                    solB = 1;

                                Utils::setBit(tile, (chunk.tiles[ty][tx].direction & 1) == 1, 10);
                                Utils::setBit(tile, (chunk.tiles[ty][tx].direction & 2) == 2, 11);
                                Utils::setBit(tile, solA == 0 || solA == 1, 12);
                                Utils::setBit(tile, solA == 0 || solA == 2, 13);
                                Utils::setBit(tile, solB == 0 || solB == 1, 14);
                                Utils::setBit(tile, solB == 0 || solB == 2, 15);

                                layer.layout[(y * 8) + ty][(x * 8) + tx] = tile;
                            }
                        }
                    }
                }

                if (scene.layers.count() < 8 && cnt)
                    scene.layers.append(layer);
            }
        }
    }

    scene.objects.clear();
    if (dlg->exportObjects) {
        RSDKv5::Scene::SceneObject blank;
        blank.name = RSDKv5::Scene::NameIdentifier("Blank Object");

        blank.variables.clear();
        RSDKv5::Scene::VariableInfo blankFilter;
        blankFilter.name = RSDKv5::Scene::NameIdentifier("filter");
        blankFilter.type = VAR_UINT8;
        blank.variables.append(blankFilter);

        RSDKv5::Scene::VariableInfo blankPropertyValue;
        blankPropertyValue.name = RSDKv5::Scene::NameIdentifier("propertyValue");
        blankPropertyValue.type = VAR_UINT8;
        blank.variables.append(blankPropertyValue);

        scene.objects.append(blank);

        for (int o = 0; o < viewer->scene.objectTypeNames.count(); ++o) {
            RSDKv5::Scene::SceneObject obj;

            QString objName = viewer->scene.objectTypeNames[o];
            objName.replace(" ", "");
            obj.name = RSDKv5::Scene::NameIdentifier(objName);
            obj.variables.clear();
            RSDKv5::Scene::VariableInfo filter;
            filter.name = RSDKv5::Scene::NameIdentifier("filter");
            filter.type = VAR_UINT8;
            obj.variables.append(filter);

            RSDKv5::Scene::VariableInfo propertyValue;
            propertyValue.name = RSDKv5::Scene::NameIdentifier("propertyValue");
            propertyValue.type = VAR_UINT8;
            obj.variables.append(propertyValue);

            scene.objects.append(obj);
        }

        for (int e = 0; e < viewer->scene.objects.count(); ++e) {
            RSDKv5::Scene::SceneEntity entity;
            entity.position.x = viewer->scene.objects[e].m_position.x;
            entity.position.y = viewer->scene.objects[e].m_position.y;

            entity.parent = &scene.objects[viewer->scene.objects[e].type];
            entity.slotID = e;

            entity.variables.clear();
            RSDKv5::Scene::VariableValue filter;
            filter.type        = VAR_UINT8;
            filter.value_uint8 = 0x05;
            entity.variables.append(filter);

            RSDKv5::Scene::VariableValue propertyValue;
            propertyValue.type        = VAR_UINT8;
            propertyValue.value_uint8 = viewer->scene.objects[e].m_propertyValue;
            entity.variables.append(propertyValue);

            entity.parent->entities.append(entity);
        }
    }

    if (dlg->exportLayers || dlg->exportObjects) {
        scene.write(dlg->m_outputPath + "/Scene.bin");
    }
}

#include "moc_sceneeditor.cpp"
