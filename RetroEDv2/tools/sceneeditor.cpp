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
    for (auto &&im : m_parent->m_mainView->m_chunks) {
        auto *chunk = new ChunkLabel(&m_parent->m_mainView->m_selectedChunk, i, chunkArea);
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

    m_mainView = new SceneViewer(this);
    m_mainView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->viewerFrame->layout()->addWidget(m_mainView);
    m_mainView->show();

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

    m_mainView->m_sbHorizontal = ui->horizontalScrollBar;
    m_mainView->m_sbVertical   = ui->verticalScrollBar;
    m_mainView->m_statusLabel  = ui->statusLabel;

    m_snapSize = Vector2<int>(0x10, 0x10);

    ui->showParallax->setIcon(Utils::getColouredIcon(":/icons/ic_horizontal_split_48px.svg"));
    ui->showTileGrid->setIcon(Utils::getColouredIcon(":/icons/ic_grid_48px.svg"));
    ui->showPixelGrid->setIcon(Utils::getColouredIcon(":/icons/ic_grid_48px.svg"));
    ui->exportRSDKv5->setIcon(Utils::getColouredIcon(":/icons/ic_redo_48px.svg"));

    ui->toolBox->setCurrentIndex(0);
    ui->propertiesBox->setCurrentIndex(0);

    connect(ui->horizontalScrollBar, &QScrollBar::valueChanged,
            [this](int v) { m_mainView->m_cam.m_position.x = v; });

    connect(ui->verticalScrollBar, &QScrollBar::valueChanged,
            [this](int v) { m_mainView->m_cam.m_position.y = v; });

    connect(ui->layerList, &QListWidget::currentRowChanged, [this](int c) {
        // m_uo->setDisabled(c == -1);
        // m_do->setDisabled(c == -1);
        // m_ro->setDisabled(c == -1);

        if (c == -1)
            return;

        // m_do->setDisabled(c == m_objectList->count() - 1);
        // m_uo->setDisabled(c == 0);

        m_mainView->m_selectedLayer = c;

        m_lyrProp->setupUI(&m_mainView->m_scene, &m_mainView->m_background, c, m_mainView->m_gameType);
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

        m_mainView->m_selectedObject = c;
    });

    connect(ui->addObj, &QToolButton::clicked, [this] {
        // uint c = m_objectList->currentRow() + 1;
        FormatHelpers::Stageconfig::ObjectInfo objInfo;
        objInfo.m_name = "New Object";
        m_mainView->m_stageconfig.m_objects.append(objInfo);
        m_mainView->m_scene.m_objectTypeNames.append("New Object");

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
        m_mainView->m_scene.m_objects.removeAt(c);
        ui->objectList->blockSignals(true);
        ui->objectList->setCurrentRow(n);
        ui->objectList->blockSignals(false);

        for (int o = m_mainView->m_scene.m_objects.count() - 1; o >= 0; --o) {
            if (m_mainView->m_scene.m_objects[o].m_type > c)
                m_mainView->m_scene.m_objects[o].m_type--;
            else
                m_mainView->m_scene.m_objects.removeAt(o);
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

        m_mainView->m_selectedEntity = c;

        m_mainView->m_cam.m_position.x =
            m_mainView->m_scene.m_objects[c].getX() - (m_mainView->m_storedW / 2);
        m_mainView->m_cam.m_position.y =
            m_mainView->m_scene.m_objects[c].getY() - (m_mainView->m_storedH / 2);

        m_objProp->setupUI(&m_mainView->m_scene.m_objects[m_mainView->m_selectedEntity],
                           m_mainView->m_gameType);
        ui->propertiesBox->setCurrentWidget(ui->objPropPage);
    });

    connect(ui->addEnt, &QToolButton::clicked, [this] {
        // uint c = m_objectList->currentRow() + 1;
        FormatHelpers::Scene::Object obj;
        obj.m_type = m_mainView->m_selectedObject;
        if (m_mainView->m_selectedObject < 0)
            obj.m_type = 0; // backup
        obj.m_id = m_mainView->m_scene.m_objects.count();

        m_mainView->m_scene.m_objects.append(obj);

        createEntityList();

        ui->addEnt->setDisabled(m_mainView->m_scene.m_objects.count()
                                >= FormatHelpers::Scene::maxObjectCount);
    });

    connect(ui->rmEnt, &QToolButton::clicked, [this] {
        int c = ui->entityList->currentRow();
        int n = ui->entityList->currentRow() == ui->entityList->count() - 1 ? c - 1 : c;
        delete ui->entityList->item(c);
        m_mainView->m_scene.m_objects.removeAt(c);
        ui->entityList->blockSignals(true);
        ui->entityList->setCurrentRow(n);
        ui->entityList->blockSignals(false);

        ui->addEnt->setDisabled(m_mainView->m_scene.m_objects.count()
                                >= FormatHelpers::Scene::maxObjectCount);
    });

    connect(ui->scrollList, &QListWidget::currentRowChanged, [this](int c) {
        ui->rmScr->setDisabled(c == -1);

        if (c == -1)
            return;

        m_mainView->m_selectedScrollInfo = c;

        m_scrProp->setupUI(
            &m_mainView->m_background.m_layers[m_mainView->m_selectedLayer - 1].m_scrollInfos[c]);
        ui->propertiesBox->setCurrentWidget(ui->scrollPropPage);
    });

    connect(ui->addScr, &QToolButton::clicked, [this] {
        FormatHelpers::Background::ScrollIndexInfo scr;
        m_mainView->m_background.m_layers[m_mainView->m_selectedLayer - 1].m_scrollInfos.append(scr);

        createScrollList();
    });

    connect(ui->rmScr, &QToolButton::clicked, [this] {
        int c = ui->scrollList->currentRow();
        int n = ui->scrollList->currentRow() == ui->scrollList->count() - 1 ? c - 1 : c;
        delete ui->scrollList->item(c);
        m_mainView->m_background.m_layers[m_mainView->m_selectedLayer - 1].m_scrollInfos.removeAt(c);
        ui->scrollList->blockSignals(true);
        ui->scrollList->setCurrentRow(n);
        ui->scrollList->blockSignals(false);
    });

    auto resetTools = [this](byte tool) {
        if (tool == 0xFF)
            tool = TOOL_MOUSE;
        m_mainView->m_tool = tool;

        // Reset
        // m_mainView->m_selectedTile = -1;
        // m_mainView->m_selectedStamp = -1;
        m_mainView->m_selectedEntity = -1;
        m_objProp->unsetUI();
        m_mainView->m_selecting = false;

        unsetCursor();

        switch (tool) {
            default: break; // what
            case TOOL_MOUSE: break;
            case TOOL_SELECT: break;
            case TOOL_PENCIL: m_mainView->m_selecting = true; break;
            case TOOL_ERASER: m_mainView->m_selecting = true; break;
            case TOOL_ENTITY: m_mainView->m_selecting = true; break;
            case TOOL_PARALLAX: m_mainView->m_selecting = true; break;
        }
    };

    connect(ui->selToolBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            [resetTools](int v) { resetTools(v); });

    connect(ui->showCollisionA, &QPushButton::clicked, [this] {
        m_mainView->m_showPlaneA ^= 1;
        m_mainView->m_showPlaneB = false;
        ui->showCollisionA->setChecked(m_mainView->m_showPlaneA);
        ui->showCollisionB->setChecked(m_mainView->m_showPlaneB);
    });

    connect(ui->showCollisionB, &QPushButton::clicked, [this] {
        m_mainView->m_showPlaneA = false;
        m_mainView->m_showPlaneB ^= 1;
        ui->showCollisionA->setChecked(m_mainView->m_showPlaneA);
        ui->showCollisionB->setChecked(m_mainView->m_showPlaneB);
    });

    connect(ui->showTileGrid, &QPushButton::clicked, [this] { m_mainView->m_showTileGrid ^= 1; });

    connect(ui->showPixelGrid, &QPushButton::clicked, [this] { m_mainView->m_showPixelGrid ^= 1; });

    connect(m_scnProp->m_loadGlobalCB, &QCheckBox::toggled,
            [this](bool b) { m_mainView->m_stageconfig.m_loadGlobalScripts = b; });

    connect(ui->showParallax, &QPushButton::clicked, [this] { m_mainView->m_showParallax ^= 1; });

    connect(m_scnProp->m_editTIL, &QPushButton::clicked, [this] {
        ChunkEditor *edit =
            new ChunkEditor(&m_mainView->m_chunkset, m_mainView->m_chunks, m_mainView->m_tiles,
                            m_mainView->m_gameType == ENGINE_v1, this);
        edit->exec();
    });

    connect(m_scnProp->m_editSCF, &QPushButton::clicked, [this] {
        QList<int> newTypes;

        switch (m_mainView->m_gameType) {
            case ENGINE_v4: {
                StageconfigEditorv4 *edit = new StageconfigEditorv4(
                    &m_mainView->m_stageconfig, m_mainView->m_gameconfig.m_objects.count() + 1, this);
                edit->exec();
                break;
            }
            case ENGINE_v3: {
                StageconfigEditorv3 *edit = new StageconfigEditorv3(
                    &m_mainView->m_stageconfig, m_mainView->m_gameconfig.m_objects.count() + 1, this);
                edit->exec();
                break;
            }
            case ENGINE_v2: {
                StageconfigEditorv2 *edit = new StageconfigEditorv2(
                    &m_mainView->m_stageconfig, m_mainView->m_gameconfig.m_objects.count() + 2, this);
                edit->exec();
                break;
            }
            case ENGINE_v1: {
                StageconfigEditorv1 *edit = new StageconfigEditorv1(&m_mainView->m_stageconfig, this);
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
        for (FormatHelpers::Stageconfig::ObjectInfo &obj : m_mainView->m_stageconfig.m_objects) {
            names.append(obj.m_name);
        }

        int o = 0;
        newTypes.append(0); // Blank Object
        // Globals stay the same
        if (m_mainView->m_gameType != ENGINE_v1) {
            if (m_mainView->m_stageconfig.m_loadGlobalScripts) {
                if (m_mainView->m_gameType == ENGINE_v2)
                    newTypes.append(1); // Player
                int cnt = newTypes.count();

                for (; o < m_mainView->m_gameconfig.m_objects.count(); ++o) {
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
        for (; o < m_mainView->m_scene.m_objectTypeNames.count(); ++o) {
            int index = names.indexOf(m_mainView->m_scene.m_objectTypeNames[o]);
            if (index >= 0)
                index += cnt;
            newTypes.append(index);
        }

        for (FormatHelpers::Scene::Object &obj : m_mainView->m_scene.m_objects) {
            if (newTypes[obj.m_type] >= 0)
                obj.m_type = newTypes[obj.m_type];
            else
                obj.m_type = 0;
        }

        m_mainView->m_scene.m_objectTypeNames.clear();
        if (m_mainView->m_gameType != ENGINE_v1) {
            if (m_mainView->m_stageconfig.m_loadGlobalScripts) {
                if (m_mainView->m_gameType == ENGINE_v2)
                    m_mainView->m_scene.m_objectTypeNames.append("Player");

                for (FormatHelpers::Gameconfig::ObjectInfo &obj : m_mainView->m_gameconfig.m_objects) {
                    m_mainView->m_scene.m_objectTypeNames.append(obj.m_name);
                }
            }

            for (FormatHelpers::Stageconfig::ObjectInfo &obj : m_mainView->m_stageconfig.m_objects) {
                m_mainView->m_scene.m_objectTypeNames.append(obj.m_name);
            }
        }
        else {
            for (QString &obj : globals) {
                m_mainView->m_scene.m_objectTypeNames.append(obj);
            }

            for (FormatHelpers::Stageconfig::ObjectInfo &obj : m_mainView->m_stageconfig.m_objects) {
                m_mainView->m_scene.m_objectTypeNames.append(obj.m_name);
            }
        }

        ui->objectList->clear();
        ui->objectList->addItem("Blank Object");
        for (int o = 0; o < m_mainView->m_scene.m_objectTypeNames.count(); ++o)
            ui->objectList->addItem(m_mainView->m_scene.m_objectTypeNames[o]);

        m_objProp->m_typeBox->blockSignals(true);
        m_objProp->m_typeBox->clear();
        m_objProp->m_typeBox->addItem("Blank Object");
        for (int o = 0; o < m_mainView->m_scene.m_objectTypeNames.count(); ++o)
            m_objProp->m_typeBox->addItem(m_mainView->m_scene.m_objectTypeNames[o]);
        m_objProp->m_typeBox->blockSignals(false);

        createEntityList();

        if (m_mainView->m_gameType == ENGINE_v1) {
            m_scnProp->m_musBox->blockSignals(true);
            m_scnProp->m_musBox->clear();
            for (int m = 0; m < m_mainView->m_stageconfig.m_music.count(); ++m)
                m_scnProp->m_musBox->addItem(m_mainView->m_stageconfig.m_music[o]);
            m_scnProp->m_musBox->blockSignals(false);
        }
    });

    connect(m_scnProp->m_editPAL, &QPushButton::clicked, [this] {
        PaletteEditor *edit =
            new PaletteEditor(m_mainView->m_stageconfig.m_filename, m_mainView->m_gameType + 2);
        edit->show();
    });

    connect(ui->exportRSDKv5, &QPushButton::clicked, [this] {
        ExportRSDKv5Scene *dlg = new ExportRSDKv5Scene(m_mainView->m_scene.m_filename, this);
        if (dlg->exec() == QDialog::Accepted) {
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

                if (dlg->m_exportFG[0] || dlg->m_exportFG[1]) {
                    if (m_mainView->m_scene.m_width > w)
                        w = m_mainView->m_scene.m_width;
                    if (m_mainView->m_scene.m_width > h)
                        h = m_mainView->m_scene.m_height;
                }

                for (int i = 0; i < m_mainView->m_background.m_layers.count(); ++i) {
                    if (dlg->m_exportBG[i][0] || dlg->m_exportBG[i][1]) {
                        if (m_mainView->m_background.m_layers[i].m_width > w)
                            w = m_mainView->m_background.m_layers[i].m_width;
                        if (m_mainView->m_background.m_layers[i].m_height > h)
                            h = m_mainView->m_background.m_layers[i].m_height;
                    }
                }

                QImage image(w * 0x80, h * 0x80, QImage::Format_ARGB32);
                image.fill(0xFF00FF);
                QPainter painter(&image);

                for (int i = 0; i < 8; ++i) {
                    if (dlg->m_exportBG[i][0] || dlg->m_exportBG[i][1]) {
                        for (int y = 0; y < m_mainView->m_background.m_layers[i].m_height; ++y) {
                            for (int x = 0; x < m_mainView->m_background.m_layers[i].m_width; ++x) {
                                ushort chunk     = m_mainView->m_background.m_layers[i].m_layout[y][x];
                                QImage &chunkImg = m_mainView->m_chunks[chunk];

                                painter.drawImage(x * 0x80, y * 0x80, chunkImg);
                            }
                        }
                    }
                }

                if (dlg->m_exportFG[0] || dlg->m_exportFG[1]) {
                    for (int y = 0; y < m_mainView->m_scene.m_height; ++y) {
                        for (int x = 0; x < m_mainView->m_scene.m_width; ++x) {
                            ushort chunk     = m_mainView->m_scene.m_layout[y][x];
                            QImage &chunkImg = m_mainView->m_chunks[chunk];

                            painter.drawImage(x * 0x80, y * 0x80, chunkImg);
                        }
                    }
                }

                if (dlg->m_exportObjects) {
                    for (int o = 0; o < m_mainView->m_scene.m_objects.count(); ++o) {
                        FormatHelpers::Scene::Object &obj = m_mainView->m_scene.m_objects[o];

                        QString name = obj.m_type >= 1
                                           ? m_mainView->m_scene.m_objectTypeNames[obj.m_type - 1]
                                           : "Blank Object";

                        painter.drawText(obj.getX(), obj.getY(), name);
                        // painter.drawImage(obj.getX(), obj.getY(), m_mainView->m_missingObj);

                        if (dlg->m_exportObjInfo) {
                            painter.drawText(obj.getX(), obj.getY() + 8,
                                             "st: " + QString::number(obj.m_subtype));
                            int offset = 16;

                            QList<QString> attribNames = { "ste",  "flip", "scl",  "rot",  "lyr",
                                                           "prio", "alph", "anim", "aspd", "frm",
                                                           "ink",  "v0",   "v2",   "v2",   "v3" };

                            for (int i = 0; i < 0x0F && m_mainView->m_gameType == ENGINE_v4; ++i) {
                                if (obj.m_attributes[i].m_active) {
                                    painter.drawText(obj.getX(), obj.getY() + offset,
                                                     attribNames[i] + ": "
                                                         + QString::number(obj.m_subtype));
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
                    if (!QFile::exists(m_mainView->m_gameconfig.m_filename)) {
                        return false;
                    }
                    gcPath = m_mainView->m_gameconfig.m_filename;
                }
            }

            loadScene(filedialog.selectedFiles()[0], gcPath, filter + 1);
            return true;
        }
    }

    if (event->type() == (QEvent::Type)RE_EVENT_SAVE) {
        QString path = m_mainView->m_scene.m_filename;

        if (QFile::exists(path)) {
            setStatus("Saving Scene...");
            QString basePath = path.replace(QFileInfo(path).fileName(), "");

            if (m_mainView->m_gameType != ENGINE_v1) {
                m_mainView->m_scene.write(m_mainView->m_gameType, m_mainView->m_scene.m_filename);
                m_mainView->m_background.write(m_mainView->m_gameType, basePath + "Backgrounds.bin");
                m_mainView->m_chunkset.write(m_mainView->m_gameType, basePath + "128x128Tiles.bin");
                m_mainView->m_tileconfig.write(basePath + "CollisionMasks.bin");
                m_mainView->m_stageconfig.write(m_mainView->m_gameType, basePath + "StageConfig.bin");
            }
            else {
                m_mainView->m_scene.write(m_mainView->m_gameType, m_mainView->m_scene.m_filename);
                m_mainView->m_background.write(m_mainView->m_gameType, basePath + "ZoneBG.map");
                m_mainView->m_chunkset.write(m_mainView->m_gameType, basePath + "Zone.til");
                m_mainView->m_tileconfigRS.write(basePath + "Zone.tcf");
                m_mainView->m_stageconfig.write(m_mainView->m_gameType, basePath + "Zone.zcf");
            }

            setStatus("Saved Scene: " + Utils::getFilenameAndFolder(m_mainView->m_scene.m_filename));
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
                        m_mainView->m_scene.write(filter + 1, filedialog.selectedFiles()[0]);
                        m_mainView->m_background.write(filter + 1, basePath + "Backgrounds.bin");
                        m_mainView->m_chunkset.write(filter + 1, basePath + "128x128Tiles.bin");
                        m_mainView->m_tileconfig.write(basePath + "CollisionMasks.bin");
                        m_mainView->m_stageconfig.write(filter + 1, basePath + "StageConfig.bin");
                        break;
                    case ENGINE_v1:
                        m_mainView->m_scene.write(filter + 1, filedialog.selectedFiles()[0]);
                        m_mainView->m_background.write(filter + 1, basePath + "ZoneBG.map");
                        m_mainView->m_chunkset.write(filter + 1, basePath + "Zone.til");
                        m_mainView->m_tileconfigRS.write(basePath + "Zone.tcf");
                        m_mainView->m_stageconfig.write(filter + 1, basePath + "Zone.zcf");
                        break;
                }

                appConfig.addRecentFile(m_mainView->m_gameType, TOOL_SCENEEDITOR,
                                        filedialog.selectedFiles()[0],
                                        QList<QString>{ m_mainView->m_gameconfig.m_filename });
                setStatus("Saved Scene: "
                          + Utils::getFilenameAndFolder(m_mainView->m_scene.m_filename));
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
                    m_mainView->m_scene.write(filter + 1, filedialog.selectedFiles()[0]);
                    m_mainView->m_background.write(filter + 1, basePath + "Backgrounds.bin");
                    m_mainView->m_chunkset.write(filter + 1, basePath + "128x128Tiles.bin");
                    m_mainView->m_tileconfig.write(basePath + "CollisionMasks.bin");
                    m_mainView->m_stageconfig.write(filter + 1, basePath + "StageConfig.bin");
                    break;
                case ENGINE_v1:
                    m_mainView->m_scene.write(filter + 1, filedialog.selectedFiles()[0]);
                    m_mainView->m_background.write(filter + 1, basePath + "ZoneBG.map");
                    m_mainView->m_chunkset.write(filter + 1, basePath + "Zone.til");
                    m_mainView->m_tileconfigRS.write(basePath + "Zone.tcf");
                    m_mainView->m_stageconfig.write(filter + 1, basePath + "Zone.zcf");
                    break;
            }

            appConfig.addRecentFile(m_mainView->m_gameType, TOOL_SCENEEDITOR,
                                    filedialog.selectedFiles()[0],
                                    QList<QString>{ m_mainView->m_gameconfig.m_filename });
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

    if (object != m_mainView)
        return false;

    auto setTile = [this](float x, float y) {
        if (m_mainView->m_selectedChunk < 0 || m_mainView->m_selectedLayer < 0)
            return;
        float tx = x;
        float ty = y;

        tx *= m_mainView->invZoom();
        ty *= m_mainView->invZoom();

        float tx2 = tx + fmodf(m_mainView->m_cam.m_position.x, 0x80);
        float ty2 = ty + fmodf(m_mainView->m_cam.m_position.y, 0x80);

        // clip to grid
        tx -= fmodf(tx2, 0x80);
        ty -= fmodf(ty2, 0x80);

        // Draw Selected Tile Preview
        float xpos = tx + m_mainView->m_cam.m_position.x;
        float ypos = ty + m_mainView->m_cam.m_position.y;

        xpos /= 0x80;
        ypos /= 0x80;
        if (ypos >= 0 && ypos < m_mainView->layerHeight(m_mainView->m_selectedLayer)) {
            if (xpos >= 0 && xpos < m_mainView->layerWidth(m_mainView->m_selectedLayer)) {
                if (m_mainView->m_selectedLayer > 0) {
                    m_mainView->m_background.m_layers[m_mainView->m_selectedLayer - 1]
                        .m_layout[ypos][xpos] = m_mainView->m_selectedChunk;
                }
                else {
                    m_mainView->m_scene.m_layout[ypos][xpos] = m_mainView->m_selectedChunk;
                }
            }
        }
    };

    switch (event->type()) {
        case QEvent::MouseButtonPress: {
            QMouseEvent *mEvent     = static_cast<QMouseEvent *>(event);
            m_mainView->m_reference = mEvent->pos();

            m_mainView->m_mousePos.x = m_mainView->m_cam.m_lastMousePos.x = mEvent->pos().x();
            m_mainView->m_mousePos.y = m_mainView->m_cam.m_lastMousePos.y = mEvent->pos().y();

            if ((mEvent->button() & Qt::LeftButton) == Qt::LeftButton)
                m_mouseDownL = true;
            if ((mEvent->button() & Qt::MiddleButton) == Qt::MiddleButton)
                m_mouseDownM = true;
            if ((mEvent->button() & Qt::RightButton) == Qt::RightButton)
                m_mouseDownR = true;

            if (m_mouseDownM || (m_mouseDownL && m_mainView->m_tool == TOOL_MOUSE))
                setCursor(Qt::ClosedHandCursor);

            if (m_mouseDownL) {
                switch (m_mainView->m_tool) {
                    case TOOL_MOUSE: break;
                    case TOOL_SELECT: break;
                    case TOOL_PENCIL: {
                        if (m_mainView->m_selectedChunk >= 0 && m_mainView->m_selecting) {
                            setTile(mEvent->pos().x(), mEvent->pos().y());
                        }
                        else {
                            // click tile to move it or change properties
                            if (m_mainView->m_selectedLayer >= 0) {
                                Rect<float> box;

                                for (int y = 0;
                                     y < m_mainView->layerHeight(m_mainView->m_selectedLayer); ++y) {
                                    for (int x = 0;
                                         x < m_mainView->layerWidth(m_mainView->m_selectedLayer); ++x) {
                                        box = Rect<float>(x * 0x80, y * 0x80, 0x80, 0x80);

                                        Vector2<float> pos =
                                            Vector2<float>((mEvent->pos().x() * m_mainView->invZoom())
                                                               + m_mainView->m_cam.m_position.x,
                                                           (mEvent->pos().y() * m_mainView->invZoom())
                                                               + m_mainView->m_cam.m_position.y);
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
                        if (m_mainView->m_selecting) {
                            m_mainView->m_selectedChunk = 0x00;
                            setTile(mEvent->pos().x(), mEvent->pos().y());
                        }
                        break;
                    }
                    case TOOL_ENTITY: {
                        if (!m_mainView->m_selecting || m_mainView->m_selectedObject < 0) {
                            Rect<float> box;

                            for (int o = 0; o < m_mainView->m_scene.m_objects.count(); ++o) {
                                box = Rect<float>((m_mainView->m_scene.m_objects[o].getX() - 0x10),
                                                  (m_mainView->m_scene.m_objects[o].getY() - 0x10),
                                                  0x20, 0x20);

                                Vector2<float> pos =
                                    Vector2<float>((mEvent->pos().x() * m_mainView->invZoom())
                                                       + m_mainView->m_cam.m_position.x,
                                                   (mEvent->pos().y() * m_mainView->invZoom())
                                                       + +m_mainView->m_cam.m_position.y);
                                if (box.contains(pos)) {
                                    m_mainView->m_selectedEntity = o;

                                    m_objProp->setupUI(
                                        &m_mainView->m_scene.m_objects[m_mainView->m_selectedEntity],
                                        m_mainView->m_gameType);
                                    ui->propertiesBox->setCurrentWidget(ui->objPropPage);
                                    break;
                                }
                            }
                        }
                        else {
                            if (m_mainView->m_selectedObject >= 0) {
                                FormatHelpers::Scene::Object obj;
                                obj.m_type = m_mainView->m_selectedObject;

                                obj.setX((mEvent->pos().x() * m_mainView->invZoom())
                                         + m_mainView->m_cam.m_position.x);
                                obj.setY((mEvent->pos().y() * m_mainView->invZoom())
                                         + m_mainView->m_cam.m_position.y);

                                m_mainView->m_scene.m_objects.append(obj);
                            }
                        }

                        break;
                    }
                    default: break;
                }
                break;
            }

            if (m_mouseDownR) {
                switch (m_mainView->m_tool) {
                    case TOOL_PENCIL:
                        if (m_mainView->m_selectedLayer >= 0) {
                            Rect<float> box, box2;

                            for (int y = 0; y < m_mainView->layerHeight(m_mainView->m_selectedLayer);
                                 ++y) {
                                for (int x = 0; x < m_mainView->layerWidth(m_mainView->m_selectedLayer);
                                     ++x) {
                                    box = Rect<float>(x * 0x80, y * 0x80, 0x80, 0x80);

                                    Vector2<float> pos =
                                        Vector2<float>((mEvent->pos().x() * m_mainView->invZoom())
                                                           + m_mainView->m_cam.m_position.x,
                                                       (mEvent->pos().y() * m_mainView->invZoom())
                                                           + m_mainView->m_cam.m_position.y);
                                    if (box.contains(pos)) {
                                        ushort tid = 0;
                                        // idk but we're in the gaming zone now bitch!
                                        ushort chunk = m_mainView->m_selectedChunk =
                                            m_mainView->layerLayout(m_mainView->m_selectedLayer)[y][x];
                                        for (int cy = 0; cy < 8; ++cy) {
                                            for (int cx = 0; cx < 8; ++cx) {
                                                box2 = Rect<float>(box.x + (cx * 0x10),
                                                                   box.y + (cy * 0x10), 0x10, 0x10);
                                                if (box2.contains(pos)) {
                                                    tid = m_mainView->m_chunkset.m_chunks[chunk]
                                                              .m_tiles[cy][cx]
                                                              .m_tileIndex;

                                                    m_tileProp->setupUI(&m_mainView->m_tileconfig
                                                                             .m_collisionPaths[0][tid],
                                                                        &m_mainView->m_tileconfig
                                                                             .m_collisionPaths[1][tid],
                                                                        tid, m_mainView->m_tiles[tid]);
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

                        for (int o = 0; o < m_mainView->m_scene.m_objects.count(); ++o) {
                            box = Rect<float>((m_mainView->m_scene.m_objects[o].getX() - 0x10),
                                              (m_mainView->m_scene.m_objects[o].getY() - 0x10), 0x20,
                                              0x20);

                            Vector2<float> pos =
                                Vector2<float>((mEvent->pos().x() * m_mainView->invZoom())
                                                   + m_mainView->m_cam.m_position.x,
                                               (mEvent->pos().y() * m_mainView->invZoom())
                                                   + +m_mainView->m_cam.m_position.y);
                            if (box.contains(pos)) {
                                m_mainView->m_selectedEntity = o;
                                found                        = true;

                                m_objProp->setupUI(
                                    &m_mainView->m_scene.m_objects[m_mainView->m_selectedEntity],
                                    m_mainView->m_gameType);
                                ui->propertiesBox->setCurrentWidget(ui->objPropPage);
                                break;
                            }
                        }

                        if (!found) {
                            m_mainView->m_selectedEntity = -1;
                            m_mainView->m_selectedObject = -1;
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

            m_mainView->m_mousePos.x = m_mainView->m_cam.m_lastMousePos.x = mEvent->pos().x();
            m_mainView->m_mousePos.y = m_mainView->m_cam.m_lastMousePos.y = mEvent->pos().y();

            Vector2<int> m_sceneMousePos(
                (m_mainView->m_mousePos.x * m_mainView->invZoom()) + m_mainView->m_cam.m_position.x,
                (m_mainView->m_mousePos.y * m_mainView->invZoom()) + m_mainView->m_cam.m_position.y);

            if (m_mouseDownM || (m_mouseDownL && m_mainView->m_tool == TOOL_MOUSE)) {
                m_mainView->m_cam.m_position.x -=
                    (m_mainView->m_mousePos.x - m_mainView->m_reference.x()) * m_mainView->invZoom();
                m_mainView->m_cam.m_position.y -=
                    (m_mainView->m_mousePos.y - m_mainView->m_reference.y()) * m_mainView->invZoom();
                m_mainView->m_reference = mEvent->pos();
                status                  = true;
            }

            if (m_mainView->m_tool == TOOL_PENCIL || m_mainView->m_tool == TOOL_ENTITY) {
                m_mainView->m_tilePos.x = m_mainView->m_mousePos.x;
                m_mainView->m_tilePos.y = m_mainView->m_mousePos.y;

                if (m_ctrlDownL) {
                    m_mainView->m_tilePos.x -= fmodf(m_mainView->m_tilePos.x + (0x80 / 2), 0x80);
                    m_mainView->m_tilePos.y -= fmodf(m_mainView->m_tilePos.y + (0x80 / 2), 0x80);

                    // TODO: entities not previewing correctly when snapping
                }
            }

            // Hover
            switch (m_mainView->m_tool) {
                default: break;
                case TOOL_MOUSE: break;
                case TOOL_SELECT: break;
                case TOOL_PENCIL: break;
                case TOOL_ERASER: break;
                case TOOL_ENTITY: break;
                case TOOL_PARALLAX: break;
            }

            if (m_mouseDownL) {
                switch (m_mainView->m_tool) {
                    default: break;
                    case TOOL_MOUSE: break;
                    case TOOL_SELECT: break;
                    case TOOL_PENCIL: {
                        if (m_mainView->m_selectedChunk >= 0 && m_mainView->m_selecting) {
                            setTile(m_mainView->m_mousePos.x, m_mainView->m_mousePos.y);
                        }
                        break;
                    }
                    case TOOL_ERASER: {
                        if (m_mainView->m_selecting) {
                            m_mainView->m_selectedChunk = 0x0;
                            setTile(m_mainView->m_mousePos.x, m_mainView->m_mousePos.y);
                        }
                        break;
                    }
                    case TOOL_ENTITY: {
                        if (m_mainView->m_selectedObject < 0 && m_mainView->m_selectedEntity >= 0) {
                            FormatHelpers::Scene::Object &object =
                                m_mainView->m_scene.m_objects[m_mainView->m_selectedEntity];

                            object.setX((m_mainView->m_mousePos.x * m_mainView->invZoom())
                                        + m_mainView->m_cam.m_position.x);
                            object.setY((m_mainView->m_mousePos.y * m_mainView->invZoom())
                                        + m_mainView->m_cam.m_position.y);

                            if (m_ctrlDownL) {
                                object.setX(object.getX() - fmodf(object.getX(), m_snapSize.x));
                                object.setY(object.getY() - fmodf(object.getY(), m_snapSize.y));
                            }
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
            QMouseEvent *mEvent      = static_cast<QMouseEvent *>(event);
            m_mainView->m_mousePos.x = mEvent->pos().x();
            m_mainView->m_mousePos.y = mEvent->pos().y();

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
                if (wEvent->angleDelta().y() > 0 && m_mainView->m_zoom < 20)
                    m_mainView->m_zoom *= 1.5;
                else if (wEvent->angleDelta().y() < 0 && m_mainView->m_zoom > 0.5)
                    m_mainView->m_zoom /= 1.5;
                return true;
            }

            m_mainView->m_cam.m_position.y -= wEvent->angleDelta().y() / 8;
            m_mainView->m_cam.m_position.x -= wEvent->angleDelta().x() / 8;
            break;
        }

        case QEvent::KeyPress: {
            QKeyEvent *kEvent = static_cast<QKeyEvent *>(event);

            Vector2<int> m_sceneMousePos(
                (m_mainView->m_mousePos.x * m_mainView->invZoom()) + m_mainView->m_cam.m_position.x,
                (m_mainView->m_mousePos.y * m_mainView->invZoom()) + m_mainView->m_cam.m_position.y);

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

                            m_mainView->m_scene.m_objects.append(object);
                        } break;
                    }
                }
            }

            switch (m_mainView->m_tool) {
                case TOOL_MOUSE: break;
                case TOOL_SELECT: break;
                case TOOL_PENCIL:
                    if (kEvent->key() == Qt::Key_Z)
                        m_mainView->m_tileFlip.x = true;

                    if (kEvent->key() == Qt::Key_X)
                        m_mainView->m_tileFlip.y = true;

                    if (kEvent->key() == Qt::Key_Delete || kEvent->key() == Qt::Key_Backspace) {
                        if (m_mainView->m_selectedLayer >= 0) {
                            Rect<float> box;

                            for (int y = 0; y < m_mainView->layerHeight(m_mainView->m_selectedLayer);
                                 ++y) {
                                for (int x = 0; x < m_mainView->layerWidth(m_mainView->m_selectedLayer);
                                     ++x) {
                                    box = Rect<float>(x * 0x80, y * 0x80, 0x80, 0x80);

                                    Vector2<float> pos =
                                        Vector2<float>((m_mainView->m_tilePos.x * m_mainView->invZoom())
                                                           + m_mainView->m_cam.m_position.x,
                                                       (m_mainView->m_tilePos.y * m_mainView->invZoom())
                                                           + m_mainView->m_cam.m_position.y);
                                    if (box.contains(pos)) {
                                        if (m_mainView->m_selectedLayer > 0) {
                                            m_mainView->m_background
                                                .m_layers[m_mainView->m_selectedLayer - 1]
                                                .m_layout[y][x] = 0;
                                        }
                                        else {
                                            m_mainView->m_scene.m_layout[y][x] = 0;
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
                        if (m_mainView->m_selectedEntity >= 0) {
                            m_mainView->m_scene.m_objects.removeAt(m_mainView->m_selectedEntity);
                            m_mainView->m_selectedEntity = -1;

                            m_objProp->unsetUI();
                            createEntityList();
                        }
                    }

                    if ((kEvent->modifiers() & Qt::ControlModifier) == Qt::ControlModifier
                        && kEvent->key() == Qt::Key_C) {
                        if (m_mainView->m_selectedEntity >= 0) {
                            m_clipboard = &m_mainView->m_scene.m_objects[m_mainView->m_selectedEntity];
                            m_clipboardType = COPY_ENTITY;
                            m_clipboardInfo = m_mainView->m_selectedEntity;
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

    if (gcfPath != m_mainView->m_gameconfig.m_filename)
        m_mainView->m_gameconfig.read(gameType, gcfPath);

    m_mainView->loadScene(scnPath, gameType);

    ui->layerList->clear();
    ui->layerList->addItem("Foreground");

    for (int l = 0; l < m_mainView->m_background.m_layers.count(); ++l)
        ui->layerList->addItem("Background " + QString::number(l + 1));

    ui->objectList->clear();
    ui->objectList->addItem("Blank Object");
    for (int o = 0; o < m_mainView->m_scene.m_objectTypeNames.count(); ++o)
        ui->objectList->addItem(m_mainView->m_scene.m_objectTypeNames[o]);

    createEntityList();

    m_mainView->m_gameType = gameType;

    ui->horizontalScrollBar->setMaximum((m_mainView->m_scene.m_width * 0x80) - m_mainView->m_storedW);
    ui->verticalScrollBar->setMaximum((m_mainView->m_scene.m_height * 0x80) - m_mainView->m_storedH);
    ui->horizontalScrollBar->setPageStep(0x80);
    ui->verticalScrollBar->setPageStep(0x80);

    m_objProp->m_typeBox->clear();
    m_objProp->m_typeBox->addItem("Blank Object");
    for (int o = 0; o < m_mainView->m_scene.m_objectTypeNames.count(); ++o)
        m_objProp->m_typeBox->addItem(m_mainView->m_scene.m_objectTypeNames[o]);

    if (m_mainView->m_gameType == ENGINE_v1) {
        m_scnProp->m_musBox->clear();

        for (int m = 0; m < m_mainView->m_stageconfig.m_music.count(); ++m)
            m_scnProp->m_musBox->addItem(m_mainView->m_stageconfig.m_music[m]);
    }
    m_scnProp->setupUI(&m_mainView->m_scene, m_mainView->m_gameType);

    m_scnProp->m_loadGlobalCB->blockSignals(true);
    m_scnProp->m_loadGlobalCB->setDisabled(m_mainView->m_gameType == ENGINE_v1);
    m_scnProp->m_loadGlobalCB->setChecked(m_mainView->m_stageconfig.m_loadGlobalScripts);
    m_scnProp->m_loadGlobalCB->blockSignals(false);

    if (m_chkProp) {
        ui->chunksPage->layout()->removeWidget(m_chkProp);
        delete m_chkProp;
    }

    m_chkProp = new ChunkSelector(this);
    m_chkProp->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->chunksPage->layout()->addWidget(m_chkProp);
    m_chkProp->show();

    ui->layerList->setCurrentRow(0);
    m_mainView->m_selectedLayer = 0;

    ui->toolBox->setCurrentIndex(0);
    ui->propertiesBox->setCurrentIndex(0);

    m_scnProp->setupUI(&m_mainView->m_scene, m_mainView->m_gameType);
    m_lyrProp->unsetUI();
    m_tileProp->unsetUI();
    m_objProp->unsetUI();
    m_scrProp->unsetUI();

    appConfig.addRecentFile(m_mainView->m_gameType, TOOL_SCENEEDITOR, scnPath,
                            QList<QString>{ gcfPath });
    setStatus("Loaded Scene: " + QFileInfo(scnPath).fileName());
}
void SceneEditor::createEntityList()
{
    ui->entityList->blockSignals(true);
    ui->entityList->clear();

    // C++11 abute poggers
    std::sort(m_mainView->m_scene.m_objects.begin(), m_mainView->m_scene.m_objects.end(),
              [](const FormatHelpers::Scene::Object &a, const FormatHelpers::Scene::Object &b) -> bool {
                  return a.m_id < b.m_id;
              });
    for (int i = 0; i < m_mainView->m_scene.m_objects.count(); ++i) {
        QString name =
            m_mainView->m_scene.m_objects[i].m_type >= 1
                ? m_mainView->m_scene.m_objectTypeNames[m_mainView->m_scene.m_objects[i].m_type - 1]
                : "Blank Object";
        //-1 because "blank object" is the internal type 0
        ui->entityList->addItem(QString::number(m_mainView->m_scene.m_objects[i].m_id) + ": " + name);
    }
    ui->entityList->blockSignals(false);
}

void SceneEditor::createScrollList()
{
    ui->scrollList->clear();

    if (m_mainView->m_selectedLayer < 1 || m_mainView->m_selectedLayer >= 9)
        return;

    ui->scrollList->blockSignals(true);
    for (int i = 0;
         i < m_mainView->m_background.m_layers[m_mainView->m_selectedLayer - 1].m_scrollInfos.count();
         ++i) {
        FormatHelpers::Background::ScrollIndexInfo &info =
            m_mainView->m_background.m_layers[m_mainView->m_selectedLayer - 1].m_scrollInfos[i];
        ui->scrollList->addItem(
            QString("Start: %1, Length %2").arg(info.m_startLine).arg(info.m_length));
    }

    ui->scrollList->blockSignals(false);
    ui->scrollList->setCurrentRow(-1);
}

#include "moc_sceneeditor.cpp"
