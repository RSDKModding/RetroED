#include "includes.hpp"
#include "ui_sceneeditor.h"
#include "qgifimage.h"

// RSDKv4 SOLIDITY TYPES
// 0 - ALL SOLID
// 1 - TOP SOLID (can be gripped onto)
// 2 - LRB SOLID
// 3 - NONE SOLID
// 4 - TOP SOLID (cant be gripped onto)

QList<QString> globalsRS = {
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

enum PropertiesTabIDs { PROP_SCN, PROP_LAYER, PROP_TILE, PROP_ENTITY, PROP_SCROLL };

ChunkSelector::ChunkSelector(QWidget *parent) : QWidget(parent), parentWidget((SceneEditor *)parent)
{
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scrollArea->setGeometry(10, 10, 200, 200);

    QWidget *chunkArea = new QWidget();

    QGridLayout *layout = new QGridLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    int i = 0;
    for (auto &&im : parentWidget->viewer->chunks) {
        auto *chunk = new ChunkLabel(&parentWidget->viewer->selectedTile, i, chunkArea);
        chunk->setPixmap(QPixmap::fromImage(im).scaled(im.width(), im.height()));
        chunk->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        chunk->resize(im.width(), im.height());
        chunk->setFixedSize(im.width(), im.height() * 1.1f);
        layout->addWidget(chunk, i, 1);
        labels.append(chunk);
        i++;
        connect(chunk, &ChunkLabel::requestRepaint, chunkArea, QOverload<>::of(&QWidget::update));
    }

    chunkArea->setLayout(layout);
    scrollArea->setWidget(chunkArea);
    QVBoxLayout *l = new QVBoxLayout(this);
    l->addWidget(scrollArea);
    setLayout(l);
}

void ChunkSelector::refreshList()
{
    int i = 0;
    for (auto &&im : parentWidget->viewer->chunks) {
        auto *chunk = labels[i];
        chunk->setPixmap(QPixmap::fromImage(im).scaled(im.width(), im.height()));
        chunk->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        chunk->resize(im.width(), im.height());
        i++;
    }
}

SceneEditor::SceneEditor(QWidget *parent) : QWidget(parent), ui(new Ui::SceneEditor)
{
    ui->setupUi(this);

    compilerv2 = new Compilerv2;
    compilerv3 = new Compilerv3;
    compilerv4 = new Compilerv4;

    viewer = new SceneViewerv5(ENGINE_v4, this);
    viewer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->viewerFrame->layout()->addWidget(viewer);
    viewer->show();

    scnProp = new SceneProperties(this);
    scnProp->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->scenePropFrame->layout()->addWidget(scnProp);
    scnProp->show();

    lyrProp = new SceneLayerProperties(this);
    lyrProp->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->layerPropFrame->layout()->addWidget(lyrProp);
    lyrProp->show();

    tileProp = new SceneTileProperties(this);
    tileProp->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->tilePropFrame->layout()->addWidget(tileProp);
    tileProp->show();

    objProp = new SceneObjectProperties(this);
    objProp->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->objPropFrame->layout()->addWidget(objProp);
    objProp->show();

    scrProp = new SceneScrollProperties(this);
    scrProp->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->scrPropFrame->layout()->addWidget(scrProp);
    scrProp->show();

    chkProp = new ChunkSelector(this);
    chkProp->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->chunksPage->layout()->addWidget(chkProp);
    chkProp->show();

    viewer->statusLabel = ui->statusLabel;

    ui->toolBox->setCurrentIndex(0);
    ui->propertiesBox->setCurrentIndex(0);

    scnProp->gridX->setValue(viewer->gridSize.x);
    scnProp->gridY->setValue(viewer->gridSize.y);

    connect(viewer, &SceneViewerv5::callGameEvent,
            [this](byte eventID, int entityID) { callGameEvent(eventID, entityID); });

    connect(ui->horizontalScrollBar, &QScrollBar::valueChanged,
            [this](int v) { viewer->cameraPos.x = v; });

    connect(ui->verticalScrollBar, &QScrollBar::valueChanged,
            [this](int v) { viewer->cameraPos.y = v; });

    connect(ui->useGizmos, &QCheckBox::toggled, [this](bool c) { viewer->sceneInfo.effectGizmo = c; });

    connect(ui->layerList, &QListWidget::currentRowChanged, [this](int c) {
        // m_uo->setDisabled(c == -1);
        // m_do->setDisabled(c == -1);
        // m_ro->setDisabled(c == -1);

        if (c == -1)
            return;

        // m_do->setDisabled(c == m_objectList->count() - 1);
        // m_uo->setDisabled(c == 0);

        viewer->selectedLayer = c;

        lyrProp->setupUI(&scene, &background, c, viewer->gameType);
        ui->propertiesBox->setCurrentWidget(ui->layerPropPage);

        createScrollList();
        ui->addScr->setDisabled(c < 1);
        ui->rmScr->setDisabled(c < 1);
        ui->impScr->setDisabled(c < 1);
        ui->expScr->setDisabled(c < 1);
    });

    connect(ui->layerList, &QListWidget::itemChanged, [this](QListWidgetItem *item) {
        int c = ui->layerList->row(item);
        if ((uint)c < (uint)9)
            viewer->layers[c].visible = item->checkState() == Qt::Checked;
    });

    connect(ui->objectList, &QListWidget::currentRowChanged, [this](int c) {
        // m_uo->setDisabled(c == -1);
        // m_do->setDisabled(c == -1);
        ui->rmObj->setDisabled(c == -1 || c >= viewer->objects.count());

        if (c == -1 || c >= viewer->objects.count())
            return;

        bool global = c == 0;
        if (stageConfig.loadGlobalScripts) {
            for (auto &obj : gameConfig.objects) {
                if (viewer->objects[c].name == obj.name) {
                    global = true;
                    break;
                }
            }
        }

        // m_do->setDisabled(c == m_objectList->count() - 1);
        // m_uo->setDisabled(c == 0);

        viewer->selectedObject = c;
        ui->rmObj->setDisabled(c == -1 || global);
    });

    connect(ui->addObj, &QToolButton::clicked, [this] {
        // uint c = m_objectList->currentRow() + 1;
        FormatHelpers::StageConfig::ObjectInfo objInfo;
        objInfo.name = "New Object";
        stageConfig.objects.append(objInfo);

        SceneObject info;
        info.name = "New Object";
        viewer->objects.append(info);

        auto *item = new QListWidgetItem();
        item->setText("New Object");
        ui->objectList->addItem(item);

        item->setFlags(item->flags() | Qt::ItemIsEditable);
        ui->objectList->setCurrentItem(item);
        doAction();
    });

    connect(ui->rmObj, &QToolButton::clicked, [this] {
        int c = ui->objectList->currentRow();
        if (c == -1)
            return;
        int n = ui->objectList->currentRow() == ui->objectList->count() - 1 ? c - 1 : c;
        delete ui->objectList->item(c);
        int globalCount = 1;
        if (stageConfig.loadGlobalScripts)
            globalCount = gameConfig.objects.count() + 1;
        viewer->objects.removeAt(c);
        stageConfig.objects.removeAt(c - globalCount);
        ui->objectList->blockSignals(true);
        ui->objectList->setCurrentRow(n);
        ui->objectList->blockSignals(false);

        for (int o = viewer->entities.count() - 1; o >= 0; --o) {
            if (viewer->entities[o].type > c)
                viewer->entities[o].type--;
            else if (viewer->entities[o].type == c)
                viewer->entities.removeAt(o);
        }

        createEntityList();
        doAction();
    });

    connect(ui->objectList, &QListWidget::itemChanged, [this](QListWidgetItem *item) {
        int c = ui->objectList->row(item);
        if ((uint)c < (uint)viewer->objects.count())
            viewer->objects[c].visible = item->checkState() == Qt::Checked;
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

        viewer->cameraPos.x = viewer->entities[c].pos.x - ((viewer->storedW / 2) * viewer->invZoom());
        viewer->cameraPos.y = viewer->entities[c].pos.y - ((viewer->storedH / 2) * viewer->invZoom());

        objProp->setupUI(&viewer->entities[viewer->selectedEntity], viewer->selectedEntity,
                         &compilerv2->objectEntityList[viewer->selectedEntity],
                         &compilerv3->objectEntityList[viewer->selectedEntity],
                         &compilerv4->objectEntityList[viewer->selectedEntity], viewer->gameType);
        ui->propertiesBox->setCurrentWidget(ui->objPropPage);
    });

    connect(ui->addEnt, &QToolButton::clicked, [this] {
        // uint c = objectList->currentRow() + 1;
        SceneEntity ent;
        ent.type = viewer->selectedObject;
        if (viewer->selectedObject < 0)
            ent.type = 0; // backup
        ent.slotID   = viewer->entities.count();
        ent.prevSlot = ent.slotID;

        ent.variables.clear();
        for (auto &var : viewer->objects[ent.type].variables) {
            RSDKv5::Scene::VariableValue val;
            val.type        = VAR_UINT8;
            val.value_uint8 = 0;
            ent.variables.append(val);
        }

        viewer->entities.append(ent);

        createEntityList();

        ui->addEnt->setDisabled(viewer->entities.count() >= FormatHelpers::Scene::entityLimit);
        doAction();
    });

    connect(ui->rmEnt, &QToolButton::clicked, [this] {
        int c = ui->entityList->currentRow();
        if (c == -1)
            return;
        int n = ui->entityList->currentRow() == ui->entityList->count() - 1 ? c - 1 : c;
        delete ui->entityList->item(c);
        viewer->entities.removeAt(c);
        ui->entityList->blockSignals(true);
        ui->entityList->setCurrentRow(n);
        ui->entityList->blockSignals(false);

        createEntityList();

        if (viewer->entities.count() <= 0)
            viewer->selectedEntity = -1;

        ui->rmEnt->setDisabled(viewer->entities.count() <= 0);
        ui->addEnt->setDisabled(viewer->entities.count() >= FormatHelpers::Scene::entityLimit);
        doAction();
    });

    connect(ui->scrollList, &QListWidget::currentRowChanged, [this](int c) {
        ui->rmScr->setDisabled(c == -1);
        ui->impScr->setDisabled(c == -1);
        ui->expScr->setDisabled(c == -1);

        if (c == -1)
            return;

        viewer->selectedScrollInfo = c;

        scrProp->setupUI(&viewer->layers[viewer->selectedLayer].scrollInfos[c],
                         viewer->layers[viewer->selectedLayer].scrollInfos);
        ui->propertiesBox->setCurrentWidget(ui->scrollPropPage);
    });

    connect(ui->addScr, &QToolButton::clicked, [this] {
        auto &layer = viewer->layers[viewer->selectedLayer];

        auto &last = layer.scrollInfos.last();

        SceneHelpers::TileLayer::ScrollIndexInfo scr = last;
        scr.startLine                                = last.startLine + last.length;
        scr.length                                   = 1;
        layer.scrollInfos.append(scr);

        createScrollList();
        doAction();
    });

    connect(ui->rmScr, &QToolButton::clicked, [this] {
        int c = ui->scrollList->currentRow();
        int n = ui->scrollList->currentRow() == ui->scrollList->count() - 1 ? c - 1 : c;
        delete ui->scrollList->item(c);
        viewer->layers[viewer->selectedLayer].scrollInfos.removeAt(c);
        ui->scrollList->blockSignals(true);
        ui->scrollList->setCurrentRow(n);
        ui->scrollList->blockSignals(false);
        doAction();
    });

    connect(ui->impScr, &QToolButton::clicked, [this] {
        QFileDialog filedialog(this, tr("Import RSDK Scroll File"), "",
                               tr("RSDK Scroll Files (*.xml)"));
        filedialog.setAcceptMode(QFileDialog::AcceptOpen);
        if (filedialog.exec() == QDialog::Accepted) {
            Reader reader(filedialog.selectedFiles()[0]);
            QByteArray bytes = reader.readByteArray(reader.filesize, false);

            QXmlStreamReader xmlReader = QXmlStreamReader(bytes);

            readXMLScrollInfo(xmlReader, ui->layerList->currentRow());

            if (xmlReader.hasError()) {
                QMessageBox::critical(this, "Scroll.xml Parse Error", xmlReader.errorString(),
                                      QMessageBox::Ok);
                return;
            }

            // close reader and flush file
            xmlReader.clear();

            createScrollList();
            doAction();
        }
    });

    connect(ui->expScr, &QToolButton::clicked, [this] {
        QFileDialog filedialog(this, tr("Export RSDK Scroll File"), "",
                               tr("RSDK Scroll Files (*.xml)"));
        filedialog.setAcceptMode(QFileDialog::AcceptSave);
        if (filedialog.exec() == QDialog::Accepted) {
            Writer writer(filedialog.selectedFiles()[0]);
            writer.writeLine("<?xml version=\"1.0\"?>");
            writer.writeLine("");

            writeXMLScrollInfo(writer, ui->layerList->currentRow(), 0);

            writer.flush();
        }
    });

    connect(ui->selToolBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int v) {
        byte tool = SceneViewerv5::TOOL_MOUSE;
        switch (v) {
            default:
            case 0: tool = SceneViewerv5::TOOL_MOUSE; break;
            case 1: tool = SceneViewerv5::TOOL_SELECT; break;
            case 2: tool = SceneViewerv5::TOOL_PENCIL; break;
            case 3: tool = SceneViewerv5::TOOL_ERASER; break;
            case 4: tool = SceneViewerv5::TOOL_ENTITY; break;
            case 5: tool = SceneViewerv5::TOOL_PARALLAX; break;
        }

        resetTools(tool);
    });

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

    connect(ui->showTileGrid, &QPushButton::clicked, [this] { viewer->showGrid ^= 1; });

    connect(scnProp->gridX, QOverload<int>::of(&QSpinBox::valueChanged),
            [this](int v) { viewer->gridSize.x = v; });

    connect(scnProp->gridY, QOverload<int>::of(&QSpinBox::valueChanged),
            [this](int v) { viewer->gridSize.y = v; });

    connect(scnProp->loadGlobalCB, &QCheckBox::toggled, [this](bool b) {
        stageConfig.loadGlobalScripts = b;
        if (stageConfig.loadGlobalScripts) { // assume we had no globals & are now adding em
            for (int o = viewer->entities.count() - 1; o >= 0; --o) {
                if (viewer->entities[o].type >= 1)
                    viewer->entities[o].type += gameConfig.objects.count();
            }

            for (int t = gameConfig.objects.count() - 1; t >= 0; --t) {
                SceneObject info;
                info.name = gameConfig.objects[t].name;
                viewer->objects.insert(0, info);
            }
        }
        else { // assume we had globals & are now removing em
            for (int o = viewer->entities.count() - 1; o >= 0; --o) {
                if (viewer->entities[o].type > gameConfig.objects.count())
                    viewer->entities[o].type -= gameConfig.objects.count();
                else if (viewer->entities[o].type >= 1)
                    viewer->entities.removeAt(o);
            }

            for (int t = gameConfig.objects.count() - 1; t >= 0; --t) {
                viewer->objects.removeAt(t);
            }
        }

        ui->objectList->blockSignals(true);
        ui->objectList->clear();
        for (int o = 0; o < viewer->objects.count(); ++o) {
            QListWidgetItem *item = new QListWidgetItem(viewer->objects[o].name, ui->objectList);
            item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
            item->setCheckState(viewer->objects[o].visible ? Qt::Checked : Qt::Unchecked);
        }
        ui->objectList->blockSignals(false);

        createEntityList();
        doAction();
    });

    connect(ui->showParallax, &QPushButton::clicked, [this] { viewer->showParallax ^= 1; });

    connect(scnProp->editTIL, &QPushButton::clicked, [this] {
        if (chunkEdit == nullptr) {
            chunkEdit = new ChunkEditor(&viewer->chunkset, viewer->chunks, viewer->tiles,
                                        viewer->gameType, this);
            chunkEdit->show();
        }

        connect(chunkEdit, &QDialog::finished, [this] {
            chkProp->refreshList();
            doAction();
            chunkEdit = nullptr;
        });
    });

    connect(scnProp->editSCF, &QPushButton::clicked, [this] {
        QList<int> newTypes;

        int count = stageConfig.loadGlobalScripts ? gameConfig.objects.count() : 0;
        switch (viewer->gameType) {
            case ENGINE_v4: {
                StageConfigEditorv4 *edit =
                    new StageConfigEditorv4(&stageConfig, count + 1, gameConfig.soundFX.count(), this);
                edit->exec();
                break;
            }
            case ENGINE_v3: {
                StageConfigEditorv3 *edit =
                    new StageConfigEditorv3(&stageConfig, count + 1, gameConfig.soundFX.count(), this);
                edit->exec();
                break;
            }
            case ENGINE_v2: {
                StageConfigEditorv2 *edit =
                    new StageConfigEditorv2(&stageConfig, count + 2, gameConfig.soundFX.count(), this);
                edit->exec();
                break;
            }
            case ENGINE_v1: {
                StageConfigEditorv1 *edit = new StageConfigEditorv1(&stageConfig, this);
                edit->exec();
                break;
            }
        }

        QList<QString> names;
        for (FormatHelpers::StageConfig::ObjectInfo &obj : stageConfig.objects) {
            names.append(obj.name);
        }

        int o = 0;
        newTypes.append(0); // Blank Object
        // Globals stay the same
        if (viewer->gameType != ENGINE_v1) {
            if (stageConfig.loadGlobalScripts) {
                if (viewer->gameType == ENGINE_v2)
                    newTypes.append(1); // Player
                int cnt = newTypes.count();

                for (; o < gameConfig.objects.count(); ++o) {
                    newTypes.append(cnt + o);
                }
            }
        }
        else {
            for (; o < globalsRS.count(); ++o) {
                newTypes.append(1 + o);
            }
        }

        int cnt = newTypes.count();
        for (; o < viewer->objects.count(); ++o) {
            int index = names.indexOf(viewer->objects[o].name);
            if (index >= 0)
                index += cnt;
            newTypes.append(index);
        }

        for (SceneEntity &obj : viewer->entities) {
            if (newTypes[obj.type] >= 0)
                obj.type = newTypes[obj.type];
            else
                obj.type = 0;
        }

        viewer->objects.clear();
        SceneObject blankInfo;
        blankInfo.name = "Blank Object";
        viewer->objects.append(blankInfo);

        if (viewer->gameType != ENGINE_v1) {
            if (stageConfig.loadGlobalScripts) {
                if (viewer->gameType == ENGINE_v2) {
                    SceneObject info;
                    info.name = "Player";
                    viewer->objects.append(info);
                }

                for (FormatHelpers::GameConfig::ObjectInfo &obj : gameConfig.objects) {
                    SceneObject info;
                    info.name = obj.name;
                    viewer->objects.append(info);
                }
            }

            for (FormatHelpers::StageConfig::ObjectInfo &obj : stageConfig.objects) {
                SceneObject info;
                info.name = obj.name;
                viewer->objects.append(info);
            }
        }
        else {
            for (QString &obj : globalsRS) {
                SceneObject info;
                info.name = obj;
                viewer->objects.append(info);
            }

            for (FormatHelpers::StageConfig::ObjectInfo &obj : stageConfig.objects) {
                SceneObject info;
                info.name = obj.name;
                viewer->objects.append(info);
            }
        }

        ui->objectList->blockSignals(true);
        ui->objectList->clear();
        for (int o = 0; o < viewer->objects.count(); ++o) {
            QListWidgetItem *item = new QListWidgetItem(viewer->objects[o].name, ui->objectList);
            item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
            item->setCheckState(viewer->objects[o].visible ? Qt::Checked : Qt::Unchecked);
        }
        ui->objectList->blockSignals(false);

        createEntityList();

        if (viewer->gameType == ENGINE_v1) {
            scnProp->musBox->blockSignals(true);
            scnProp->musBox->clear();
            for (int m = 0; m < stageConfig.music.count(); ++m)
                scnProp->musBox->addItem(stageConfig.music[o]);
            scnProp->musBox->blockSignals(false);
        }
        doAction();
    });

    connect(scnProp->editTSet, &QPushButton::clicked, [this] {
        TilesetEditor *edit = new TilesetEditor(viewer->tiles, viewer->tilePalette);
        edit->setWindowTitle("Edit Tileset");
        edit->exec();

        setStatus("Rebuilding tiles...", true);
        viewer->tilesetTexture = nullptr;

        QImage tileset(0x10, 0x400 * 0x10, QImage::Format_Indexed8);

        QVector<QRgb> pal;
        for (PaletteColour &col : viewer->tilePalette) pal.append(col.toQColor().rgb());
        tileset.setColorTable(pal);
        addStatusProgress(0.2); // finished setup

        uchar *pixels = tileset.bits();
        for (int i = 0; i < 0x400; ++i) {
            uchar *src = viewer->tiles[i].bits();
            for (int y = 0; y < 16; ++y) {
                for (int x = 0; x < 16; ++x) {
                    *pixels++ = *src++;
                }
            }
        }
        addStatusProgress(0.2); // finished copying tiles

        viewer->tilesetTexture = viewer->createTexture(tileset);

        for (int i = 0; i < 0x200; ++i) {
            for (int y = 0; y < 8; ++y) {
                for (int x = 0; x < 8; ++x) {
                    ushort tile = viewer->chunkset.chunks[i].tiles[y][x].tileIndex;
                    viewer->chunkset.chunks[i].tiles[y][x].tileIndex = edit->tileIDs.indexOf(tile);
                }
            }
        }
        addStatusProgress(0.2); // finished updating layout

        RSDKv5::TileConfig configStore = viewer->tileconfig;
        for (int i = 0; i < 0x400; ++i) {
            int id                                   = edit->tileIDs.indexOf(i);
            viewer->tileconfig.collisionPaths[0][id] = configStore.collisionPaths[0][i];
            viewer->tileconfig.collisionPaths[1][id] = configStore.collisionPaths[1][i];
        }
        addStatusProgress(0.2); // finished updating collision masks

        viewer->chunks.clear();
        for (FormatHelpers::Chunks::Chunk &c : viewer->chunkset.chunks)
            viewer->chunks.append(c.getImage(viewer->tiles));

        chkProp->refreshList();

        doAction("Edited Tiles");
        setStatus("Finished rebuilding tiles!"); // done!
    });

    connect(scnProp->editPAL, &QPushButton::clicked, [this] {
        PaletteEditor *edit =
            new PaletteEditor(stageConfig.filePath, viewer->gameType + PALTYPE_STAGECONFIGv4);
        edit->setWindowTitle("Edit StageConfig Palette");
        edit->show();
        doAction();
    });

    connect(ui->exportScn, &QPushButton::clicked, [this] {
        QFileDialog dlg(this, tr("Save Scene XML"), "", tr("RSDK Scene XML Files (*.xml)"));
        dlg.setAcceptMode(QFileDialog::AcceptSave);
        if (dlg.exec() == QDialog::Accepted) {
            Writer writer(dlg.selectedFiles()[0]);

            writer.writeLine("<?xml version=\"1.0\"?>");
            writer.writeLine("");
            writeXMLScene(writer);

            writer.flush();
        }
    });

    connect(scnProp->syncGC, &QPushButton::clicked, [] {});
    scnProp->syncGC->setDisabled(true);

    connect(scnProp->syncSC, &QPushButton::clicked, [] {});

    connect(scnProp->reloadLink, &QPushButton::clicked, [this] {
        setStatus("Reloading Game Link...", true);
        viewer->stopTimer();

        initGameLink();

        viewer->startTimer();
        setStatus("Game Link reloaded successfully!");
    });

    connect(ui->exportSceneImg, &QPushButton::clicked, [this] {
        QFileDialog filedialog(this, tr("Save Image"), "", tr("PNG Files (*.png)"));
        filedialog.setAcceptMode(QFileDialog::AcceptSave);
        if (filedialog.exec() == QDialog::Accepted) {
            setStatus("Rendering image...", true);
            viewer->queueRender(filedialog.selectedFiles()[0]);
        }
    });

    clearActions();
    for (QWidget *w : findChildren<QWidget *>()) {
        w->installEventFilter(this);
    }
}

SceneEditor::~SceneEditor()
{
    delete ui;

    delete compilerv2;
    delete compilerv3;
    delete compilerv4;
}

bool SceneEditor::event(QEvent *event)
{

    switch ((int)event->type()) {
        default: break;

        case RE_EVENT_NEW:
            tabTitle = "Scene Editor";
            clearActions();
            break;

        case RE_EVENT_OPEN: {
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
                                         tr((gcTypes[filter] + ";;RSDK Game.xml Files (Game*.xml)")
                                                .toStdString()
                                                .c_str()));
                    gcdialog.setAcceptMode(QFileDialog::AcceptOpen);
                    if (gcdialog.exec() == QDialog::Accepted) {
                        gcPath = gcdialog.selectedFiles()[0];
                    }
                    else {
                        if (!QFile::exists(gameConfig.filePath)) {
                            return false;
                        }
                        gcPath = gameConfig.filePath;
                    }
                }

                loadScene(filedialog.selectedFiles()[0], gcPath, filter + 1);
                return true;
            }
            break;
        }

        case RE_EVENT_SAVE: {
            if (saveScene())
                return true;
            break;
        }

        case RE_EVENT_SAVE_AS: {
            if (saveScene(true))
                return true;
            break;
        }

        case RE_EVENT_UNDO: undoAction(); return true;
        case RE_EVENT_REDO: redoAction(); return true;

        case RE_EVENT_TAB_GAIN_FOCUS: viewer->startTimer(); break;
        case RE_EVENT_TAB_LOSE_FOCUS: viewer->stopTimer(); break;

        case QEvent::Close:
            if (modified) {
                bool cancelled = false;
                if (MainWindow::showCloseWarning(this, &cancelled)) {
                    if (saveScene())
                        return true;
                }
                else if (cancelled) {
                    event->ignore();
                    return true;
                }
            }
            break;

            // You cant have a focus here & viewer, so copy code :]
        case QEvent::KeyPress: handleKeyPress(static_cast<QKeyEvent *>(event)); break;
        case QEvent::KeyRelease: handleKeyRelease(static_cast<QKeyEvent *>(event)); break;
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

    switch (event->type()) {
        case QEvent::MouseButtonPress: {
            QMouseEvent *mEvent = static_cast<QMouseEvent *>(event);
            viewer->reference   = mEvent->pos();

            viewer->mousePos.x = mEvent->pos().x();
            viewer->mousePos.y = mEvent->pos().y();

            Vector2<float> sceneMousePos((viewer->mousePos.x * viewer->invZoom()) + viewer->cameraPos.x,
                                         (viewer->mousePos.y * viewer->invZoom())
                                             + viewer->cameraPos.y);

            if ((mEvent->button() & Qt::LeftButton) == Qt::LeftButton)
                mouseDownL = true;
            if ((mEvent->button() & Qt::MiddleButton) == Qt::MiddleButton)
                mouseDownM = true;
            if ((mEvent->button() & Qt::RightButton) == Qt::RightButton)
                mouseDownR = true;

            if (mouseDownM || (mouseDownL && viewer->curTool == SceneViewerv5::TOOL_MOUSE))
                setCursor(Qt::ClosedHandCursor);

            if (mouseDownL) {
                switch (viewer->curTool) {
                    case SceneViewerv5::TOOL_MOUSE: break;
                    case SceneViewerv5::TOOL_SELECT:
                        viewer->isSelecting  = true;
                        viewer->selectPos.x  = sceneMousePos.x;
                        viewer->selectPos.y  = sceneMousePos.y;
                        viewer->selectSize.x = 0;
                        viewer->selectSize.y = 0;
                        viewer->selectedEntities.clear();
                        break;

                    case SceneViewerv5::TOOL_PENCIL: {
                        if (viewer->selectedTile >= 0 && viewer->isSelecting) {
                            setTile(mEvent->pos().x(), mEvent->pos().y());
                            doAction();
                        }
                        else {
                            // click tile to move it or change properties
                            if (viewer->selectedLayer >= 0) {
                                Rect<float> box;

                                for (int y = 0; y < viewer->sceneHeight; ++y) {
                                    for (int x = 0; x < viewer->sceneWidth; ++x) {
                                        box = Rect<float>(x * 0x80, y * 0x80, 0x80, 0x80);

                                        Vector2<float> pos =
                                            Vector2<float>((mEvent->pos().x() * viewer->invZoom())
                                                               + viewer->cameraPos.x,
                                                           (mEvent->pos().y() * viewer->invZoom())
                                                               + viewer->cameraPos.y);
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
                    case SceneViewerv5::TOOL_ERASER: {
                        if (viewer->isSelecting) {
                            viewer->selectedTile = 0x00;
                            setTile(mEvent->pos().x(), mEvent->pos().y());
                            doAction();
                        }
                        break;
                    }
                    case SceneViewerv5::TOOL_ENTITY: {
                        if (!viewer->isSelecting || viewer->selectedObject < 0) {
                            Rect<float> box;
                            int firstSel = -1;
                            Vector2<float> firstPos;

                            viewer->selectedEntity = -1;
                            for (int o = 0; o < viewer->entities.count(); ++o) {
                                int left   = viewer->entities[o].pos.x + viewer->entities[o].box.x;
                                int top    = viewer->entities[o].pos.y + viewer->entities[o].box.y;
                                int right  = viewer->entities[o].pos.x + viewer->entities[o].box.w;
                                int bottom = viewer->entities[o].pos.y + viewer->entities[o].box.h;
                                box = Rect<float>(left, top, abs(right - left), abs(bottom - top));

                                Vector2<float> pos = Vector2<float>(
                                    (mEvent->pos().x() * viewer->invZoom()) + viewer->cameraPos.x,
                                    (mEvent->pos().y() * viewer->invZoom()) + +viewer->cameraPos.y);

                                if (box.contains(pos) && firstSel == -1) {
                                    firstSel = o;
                                    firstPos = pos;
                                }

                                if (box.contains(pos) && viewer->selectedEntity < o) {
                                    viewer->selectedEntity = o;
                                    selectionOffset.x      = pos.x - viewer->entities[o].pos.x;
                                    selectionOffset.y      = pos.y - viewer->entities[o].pos.y;

                                    objProp->setupUI(
                                        &viewer->entities[viewer->selectedEntity],
                                        viewer->selectedEntity,
                                        &compilerv2->objectEntityList[viewer->selectedEntity],
                                        &compilerv3->objectEntityList[viewer->selectedEntity],
                                        &compilerv4->objectEntityList[viewer->selectedEntity],
                                        viewer->gameType);
                                    ui->propertiesBox->setCurrentWidget(ui->objPropPage);
                                    doAction();
                                    break;
                                }
                            }

                            if (viewer->selectedEntity == -1) {
                                if (firstSel >= 0) {
                                    viewer->selectedEntity = firstSel;
                                    selectionOffset.x =
                                        firstPos.x - viewer->entities[viewer->selectedEntity].pos.x;
                                    selectionOffset.y =
                                        firstPos.y - viewer->entities[viewer->selectedEntity].pos.y;

                                    objProp->setupUI(
                                        &viewer->entities[viewer->selectedEntity],
                                        viewer->selectedEntity,
                                        &compilerv2->objectEntityList[viewer->selectedEntity],
                                        &compilerv3->objectEntityList[viewer->selectedEntity],
                                        &compilerv4->objectEntityList[viewer->selectedEntity],
                                        viewer->gameType);
                                    ui->propertiesBox->setCurrentWidget(ui->objPropPage);
                                }
                                else {
                                    selectionOffset.x = 0;
                                    selectionOffset.y = 0;
                                }
                            }
                        }
                        else {
                            if (viewer->selectedObject >= 0
                                && viewer->entities.count() < FormatHelpers::Scene::entityLimit) {
                                SceneEntity entity;
                                entity.type = viewer->selectedObject;
                                int xpos =
                                    ((mEvent->pos().x() * viewer->invZoom()) + viewer->cameraPos.x)
                                    * 65536;
                                int ypos =
                                    ((mEvent->pos().y() * viewer->invZoom()) + viewer->cameraPos.y)
                                    * 65536;

                                entity.pos.x =
                                    ((mEvent->pos().x() * viewer->invZoom()) + viewer->cameraPos.x);
                                entity.pos.y =
                                    ((mEvent->pos().y() * viewer->invZoom()) + viewer->cameraPos.y);

                                if (ctrlDownL) {
                                    entity.pos.x =
                                        (entity.pos.x - fmodf(entity.pos.x, viewer->gridSize.x));
                                    entity.pos.y =
                                        (entity.pos.y - fmodf(entity.pos.y, viewer->gridSize.y));
                                }

                                int cnt         = viewer->entities.count();
                                entity.slotID   = cnt;
                                entity.prevSlot = entity.slotID;

                                entity.variables.clear();
                                for (auto &var : viewer->objects[entity.type].variables) {
                                    RSDKv5::Scene::VariableValue val;
                                    val.type        = VAR_UINT8;
                                    val.value_uint8 = 0;
                                    entity.variables.append(val);
                                }

                                viewer->entities.append(entity);
                                compilerv2->objectEntityList[cnt].type = viewer->selectedObject;
                                compilerv2->objectEntityList[cnt].propertyValue = 0;
                                compilerv2->objectEntityList[cnt].XPos          = xpos;
                                compilerv2->objectEntityList[cnt].YPos          = ypos;

                                compilerv3->objectEntityList[cnt].type = viewer->selectedObject;
                                compilerv3->objectEntityList[cnt].propertyValue = 0;
                                compilerv3->objectEntityList[cnt].XPos          = xpos;
                                compilerv3->objectEntityList[cnt].YPos          = ypos;

                                compilerv4->objectEntityList[cnt].type = viewer->selectedObject;
                                compilerv4->objectEntityList[cnt].propertyValue = 0;
                                compilerv4->objectEntityList[cnt].XPos          = xpos;
                                compilerv4->objectEntityList[cnt].YPos          = ypos;
                                viewer->selectedEntity                          = cnt;

                                createEntityList();
                                objProp->setupUI(&viewer->entities[viewer->selectedEntity],
                                                 viewer->selectedEntity,
                                                 &compilerv2->objectEntityList[viewer->selectedEntity],
                                                 &compilerv3->objectEntityList[viewer->selectedEntity],
                                                 &compilerv4->objectEntityList[viewer->selectedEntity],
                                                 viewer->gameType);
                                ui->propertiesBox->setCurrentWidget(ui->objPropPage);

                                doAction();
                            }
                            else if (viewer->entities.count() >= FormatHelpers::Scene::entityLimit) {
                                QMessageBox msgBox =
                                    QMessageBox(QMessageBox::Information, "RetroED",
                                                QString("Entity Cap has been reached.\nUnable to add "
                                                        "new entity.\nPlease remove an entity first."),
                                                QMessageBox::NoButton, this);
                                msgBox.exec();
                            }
                        }

                        break;
                    }
                    default: break;
                }
                break;
            }

            if (mouseDownR) {
                switch (viewer->curTool) {
                    case SceneViewerv5::TOOL_PENCIL:
                        if (viewer->selectedLayer >= 0) {
                            Rect<float> box, box2;

                            for (int y = 0; y < viewer->sceneHeight; ++y) {
                                for (int x = 0; x < viewer->sceneWidth; ++x) {
                                    box = Rect<float>(x * 0x80, y * 0x80, 0x80, 0x80);

                                    Vector2<float> pos = Vector2<float>(
                                        (mEvent->pos().x() * viewer->invZoom()) + viewer->cameraPos.x,
                                        (mEvent->pos().y() * viewer->invZoom()) + viewer->cameraPos.y);
                                    if (box.contains(pos)) {
                                        ushort tid = 0;
                                        // idk but we're in the gaming zone now bitch!
                                        ushort chunk = viewer->selectedTile =
                                            viewer->layers[viewer->selectedLayer].layout[y][x];

                                        for (int cy = 0; cy < 8; ++cy) {
                                            for (int cx = 0; cx < 8; ++cx) {
                                                box2 = Rect<float>(box.x + (cx * 0x10),
                                                                   box.y + (cy * 0x10), 0x10, 0x10);
                                                if (box2.contains(pos)) {
                                                    tid = viewer->chunkset.chunks[chunk]
                                                              .tiles[cy][cx]
                                                              .tileIndex;

                                                    tileProp->setupUI(
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
                    case SceneViewerv5::TOOL_ENTITY: {
                        Rect<float> box;
                        bool found   = false;
                        int firstSel = -1;
                        Vector2<float> firstPos;

                        for (int o = 0; o < viewer->entities.count(); ++o) {
                            int left   = viewer->entities[o].pos.x + viewer->entities[o].box.x;
                            int top    = viewer->entities[o].pos.y + viewer->entities[o].box.y;
                            int right  = viewer->entities[o].pos.x + viewer->entities[o].box.w;
                            int bottom = viewer->entities[o].pos.y + viewer->entities[o].box.h;
                            box        = Rect<float>(left, top, abs(right - left), abs(bottom - top));

                            Vector2<float> pos = Vector2<float>(
                                (mEvent->pos().x() * viewer->invZoom()) + viewer->cameraPos.x,
                                (mEvent->pos().y() * viewer->invZoom()) + viewer->cameraPos.y);

                            if (box.contains(pos) && firstSel == -1) {
                                firstSel = o;
                                firstPos = pos;
                            }

                            if (box.contains(pos) && viewer->selectedEntity < o) {
                                viewer->selectedEntity = o;
                                selectionOffset.x      = pos.x - viewer->entities[o].pos.x;
                                selectionOffset.y      = pos.y - viewer->entities[o].pos.y;
                                found                  = true;

                                objProp->setupUI(&viewer->entities[viewer->selectedEntity],
                                                 viewer->selectedEntity,
                                                 &compilerv2->objectEntityList[viewer->selectedEntity],
                                                 &compilerv3->objectEntityList[viewer->selectedEntity],
                                                 &compilerv4->objectEntityList[viewer->selectedEntity],
                                                 viewer->gameType);
                                ui->propertiesBox->setCurrentWidget(ui->objPropPage);
                                break;
                            }
                        }

                        if (!found) {
                            if (firstSel >= 0) {
                                viewer->selectedEntity = firstSel;
                                selectionOffset.x =
                                    firstPos.x - viewer->entities[viewer->selectedEntity].pos.x;
                                selectionOffset.y =
                                    firstPos.y - viewer->entities[viewer->selectedEntity].pos.y;
                                objProp->setupUI(&viewer->entities[viewer->selectedEntity],
                                                 viewer->selectedEntity,
                                                 &compilerv2->objectEntityList[viewer->selectedEntity],
                                                 &compilerv3->objectEntityList[viewer->selectedEntity],
                                                 &compilerv4->objectEntityList[viewer->selectedEntity],
                                                 viewer->gameType);
                                ui->propertiesBox->setCurrentWidget(ui->objPropPage);
                            }
                            else {
                                viewer->selectedEntity = -1;
                                viewer->selectedObject = -1;
                                selectionOffset.x      = 0;
                                selectionOffset.y      = 0;
                                ui->objectList->setCurrentRow(-1);
                                ui->entityList->setCurrentRow(-1);

                                objProp->unsetUI();
                            }
                        }
                        break;
                    }
                    case SceneViewerv5::TOOL_PARALLAX: break;
                    default: break;
                }
                break;
            }

            break;
        }

        case QEvent::MouseMove: {
            bool status         = false;
            QMouseEvent *mEvent = static_cast<QMouseEvent *>(event);

            auto mPos = viewer->mapFromGlobal(QCursor::pos());

            viewer->mousePos.x = mPos.x();
            viewer->mousePos.y = mPos.y();

            Vector2<float> sceneMousePos((viewer->mousePos.x * viewer->invZoom()) + viewer->cameraPos.x,
                                         (viewer->mousePos.y * viewer->invZoom())
                                             + viewer->cameraPos.y);

            if (mouseDownM || (mouseDownL && viewer->curTool == SceneViewerv5::TOOL_MOUSE)) {
                float moveX = viewer->mousePos.x - viewer->reference.x();
                float moveY = viewer->mousePos.y - viewer->reference.y();

                QPoint cursorPos = QCursor::pos();
                QRect screenRect = QGuiApplication::screenAt(viewer->pos())
                                       ->availableGeometry()
                                       .adjusted(20, 20, -19, -19);

                if (!screenRect.contains(cursorPos)) {
                    if (cursorPos.x() < screenRect.x()) {
                        cursorPos.setX(cursorPos.x() + (screenRect.x() + screenRect.width()));
                        viewer->reference.setX(viewer->reference.x()
                                               + (screenRect.x() + screenRect.width()));
                    }
                    if (cursorPos.x() > screenRect.x() + screenRect.width()) {
                        cursorPos.setX(cursorPos.x() - (screenRect.x() + screenRect.width()));
                        viewer->reference.setX(viewer->reference.x()
                                               - (screenRect.x() + screenRect.width()));
                    }

                    if (cursorPos.y() < screenRect.y()) {
                        cursorPos.setY(cursorPos.y() + (screenRect.y() + screenRect.height()));
                        viewer->reference.setY(viewer->reference.y()
                                               + (screenRect.y() + screenRect.height()));
                    }
                    if (cursorPos.y() > screenRect.y() + screenRect.height()) {
                        cursorPos.setY(cursorPos.y() - (screenRect.y() + screenRect.height()));
                        viewer->reference.setY(viewer->reference.y()
                                               - (screenRect.y() + screenRect.height()));
                    }

                    auto mousePos = viewer->mapFromGlobal(cursorPos);
                    moveX         = mousePos.x() - viewer->reference.x();
                    moveY         = mousePos.y() - viewer->reference.y();

                    viewer->mousePos.x = mousePos.x();
                    viewer->mousePos.y = mousePos.y();

                    QCursor::setPos(cursorPos);
                    viewer->reference = mousePos;
                }
                else {
                    viewer->reference = mPos;
                }

                viewer->cameraPos.x -= moveX * viewer->invZoom();
                viewer->cameraPos.y -= moveY * viewer->invZoom();

                sceneMousePos =
                    Vector2<float>((viewer->mousePos.x * viewer->invZoom()) + viewer->cameraPos.x,
                                   (viewer->mousePos.y * viewer->invZoom()) + viewer->cameraPos.y);

                status = true;

                ui->horizontalScrollBar->blockSignals(true);
                ui->horizontalScrollBar->setMaximum((viewer->sceneWidth * 0x80) - viewer->storedW);
                ui->horizontalScrollBar->setValue(viewer->cameraPos.x);
                ui->horizontalScrollBar->blockSignals(false);

                ui->verticalScrollBar->blockSignals(true);
                ui->verticalScrollBar->setMaximum((viewer->sceneHeight * 0x80) - viewer->storedH);
                ui->verticalScrollBar->setValue(viewer->cameraPos.y);
                ui->verticalScrollBar->blockSignals(false);

                viewer->screens->position.x = viewer->cameraPos.x;
                viewer->screens->position.y = viewer->cameraPos.y;
            }

            if (viewer->curTool == SceneViewerv5::TOOL_PENCIL
                || viewer->curTool == SceneViewerv5::TOOL_ENTITY) {
                viewer->tilePos.x = viewer->mousePos.x;
                viewer->tilePos.y = viewer->mousePos.y;

                if (ctrlDownL) {
                    if (viewer->curTool == SceneViewerv5::TOOL_PENCIL) {
                        viewer->tilePos.x -= fmodf(viewer->tilePos.x + (0x80 / 2), 0x80);
                        viewer->tilePos.y -= fmodf(viewer->tilePos.y + (0x80 / 2), 0x80);
                    }
                    else {
                        float camOffX = fmodf(viewer->cameraPos.x, viewer->gridSize.x);
                        float camOffY = fmodf(viewer->cameraPos.y, viewer->gridSize.y);

                        viewer->tilePos.x -= fmodf(viewer->tilePos.x + camOffX, viewer->gridSize.x);
                        viewer->tilePos.y -= fmodf(viewer->tilePos.y + camOffY, viewer->gridSize.y);
                    }
                }
            }

            // Hover
            switch (viewer->curTool) {
                default: break;
                case SceneViewerv5::TOOL_MOUSE: break;
                case SceneViewerv5::TOOL_SELECT: break;
                case SceneViewerv5::TOOL_PENCIL: break;
                case SceneViewerv5::TOOL_ERASER: break;
                case SceneViewerv5::TOOL_ENTITY: break;
                case SceneViewerv5::TOOL_PARALLAX: break;
            }

            if (mouseDownL) {
                switch (viewer->curTool) {
                    default: break;
                    case SceneViewerv5::TOOL_MOUSE: break;
                    case SceneViewerv5::TOOL_SELECT: {
                        viewer->isSelecting  = true;
                        viewer->selectSize.x = sceneMousePos.x - viewer->selectPos.x;
                        viewer->selectSize.y = sceneMousePos.y - viewer->selectPos.y;

                        float cx = viewer->selectPos.x;
                        float cy = viewer->selectPos.y;
                        if (viewer->selectSize.x < 0)
                            cx -= fabsf(viewer->selectSize.x);
                        if (viewer->selectSize.y < 0)
                            cy -= fabsf(viewer->selectSize.y);

                        cx += fabsf(viewer->selectSize.x) / 2;
                        cy += fabsf(viewer->selectSize.y) / 2;

                        float oLeft   = cx - (fabsf(viewer->selectSize.x) / 2);
                        float oTop    = cy - (fabsf(viewer->selectSize.y) / 2);
                        float oRight  = cx + (fabsf(viewer->selectSize.x) / 2);
                        float oBottom = cy + (fabsf(viewer->selectSize.y) / 2);

                        viewer->selectedEntities.clear();
                        for (int e = 0; e < viewer->entities.count(); ++e) {
                            SceneEntity &entity = viewer->entities[e];

                            int filter = 0xFF;
                            for (int v = 0; v < viewer->objects[entity.type].variables.count(); ++v) {
                                if (viewer->objects[entity.type].variables[v].name == "filter") {
                                    if (v < entity.variables.count())
                                        filter = entity.variables[v].value_uint8;
                                    break;
                                }
                            }

                            if (!(filter & viewer->sceneFilter) && filter)
                                continue;

                            float left   = entity.pos.x + entity.box.x;
                            float top    = entity.pos.y + entity.box.y;
                            float right  = entity.pos.x + entity.box.w;
                            float bottom = entity.pos.y + entity.box.h;

                            if (left < oRight && right > oLeft && top < oBottom && bottom > oTop) {
                                viewer->selectedEntities.append(e);
                            }
                        }
                        break;
                    }
                    case SceneViewerv5::TOOL_PENCIL: {
                        if (viewer->selectedTile >= 0 && viewer->isSelecting) {
                            setTile(viewer->mousePos.x, viewer->mousePos.y);
                            doAction();
                        }
                        break;
                    }
                    case SceneViewerv5::TOOL_ERASER: {
                        if (viewer->isSelecting) {
                            viewer->selectedTile = 0x0;
                            setTile(viewer->mousePos.x, viewer->mousePos.y);
                            doAction();
                        }
                        break;
                    }
                    case SceneViewerv5::TOOL_ENTITY: {
                        if (viewer->selectedObject < 0 && viewer->selectedEntity >= 0) {
                            SceneEntity &entity = viewer->entities[viewer->selectedEntity];

                            entity.pos.x = sceneMousePos.x;
                            entity.pos.y = sceneMousePos.y;

                            if (ctrlDownL) {
                                entity.pos.x = (entity.pos.x - fmodf(entity.pos.x, viewer->gridSize.x));
                                entity.pos.y = (entity.pos.y - fmodf(entity.pos.y, viewer->gridSize.y));
                            }
                            else {
                                entity.pos.x -= selectionOffset.x;
                                entity.pos.y -= selectionOffset.y;
                            }

                            int id                                = viewer->selectedEntity;
                            compilerv2->objectEntityList[id].XPos = entity.pos.x * 65536;
                            compilerv2->objectEntityList[id].YPos = entity.pos.y * 65536;

                            compilerv3->objectEntityList[id].XPos = entity.pos.x * 65536;
                            compilerv3->objectEntityList[id].YPos = entity.pos.y * 65536;

                            compilerv4->objectEntityList[id].XPos = entity.pos.x * 65536;
                            compilerv4->objectEntityList[id].YPos = entity.pos.y * 65536;

                            doAction();
                        }
                        break;
                    }
                    case SceneViewerv5::TOOL_PARALLAX: break;
                }
            }

            if (status)
                return true;

            break;
        };

        case QEvent::MouseButtonRelease: {
            QMouseEvent *mEvent = static_cast<QMouseEvent *>(event);
            viewer->mousePos.x  = mEvent->pos().x();
            viewer->mousePos.y  = mEvent->pos().y();

            switch (viewer->curTool) {
                case SceneViewerv5::TOOL_MOUSE: break;
                case SceneViewerv5::TOOL_SELECT: viewer->isSelecting = false; break;
                case SceneViewerv5::TOOL_PENCIL: {
                    if (viewer->selectedTile >= 0 && viewer->isSelecting) {
                        doAction(QString("Placed Tile(s): (%1, %2)")
                                     .arg(mEvent->pos().x())
                                     .arg(mEvent->pos().y()));
                    }
                    break;
                }
                case SceneViewerv5::TOOL_ERASER: {
                    if (viewer->isSelecting) {
                        doAction(QString("Erased Tile(s): (%1, %2)")
                                     .arg(mEvent->pos().x())
                                     .arg(mEvent->pos().y()));
                    }
                    break;
                }
                default: break;
            }

            if ((mEvent->button() & Qt::LeftButton) == Qt::LeftButton)
                mouseDownL = false;
            if ((mEvent->button() & Qt::MiddleButton) == Qt::MiddleButton)
                mouseDownM = false;
            if ((mEvent->button() & Qt::RightButton) == Qt::RightButton)
                mouseDownR = false;

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
            viewer->cameraPos.y -= wEvent->angleDelta().y() / 8;
            viewer->cameraPos.x -= wEvent->angleDelta().x() / 8;

            ui->horizontalScrollBar->blockSignals(true);
            ui->horizontalScrollBar->setMaximum((viewer->sceneWidth * 0x80) - viewer->storedW);
            ui->horizontalScrollBar->setValue(viewer->cameraPos.x);
            ui->horizontalScrollBar->blockSignals(false);

            ui->verticalScrollBar->blockSignals(true);
            ui->verticalScrollBar->setMaximum((viewer->sceneHeight * 0x80) - viewer->storedH);
            ui->verticalScrollBar->setValue(viewer->cameraPos.y);
            ui->verticalScrollBar->blockSignals(false);

            break;
        }

        case QEvent::KeyPress: handleKeyPress(static_cast<QKeyEvent *>(event)); break;
        case QEvent::KeyRelease: handleKeyRelease(static_cast<QKeyEvent *>(event)); break;

        default: break;
    }

    return false;
}

void SceneEditor::loadScene(QString scnPath, QString gcfPath, byte gameType)
{
    setStatus("Loading scene...", true);

    viewer->objectsLoaded = false;

    if (gcfPath != gameConfig.filePath) {
        if (QFileInfo(gcfPath).suffix().toLower().contains("xml"))
            parseGameXML(gameType, gcfPath);
        else
            gameConfig.read(gameType, gcfPath);
    }
    QString dataPath = QFileInfo(gcfPath).absolutePath();
    QDir dir(dataPath);
    dir.cdUp();
    viewer->dataPath = dir.path();

    addStatusProgress(1. / 6); // finish initial setup

    // loading
    QString pth      = scnPath;
    QString basePath = pth.replace(QFileInfo(pth).fileName(), "");

    viewer->currentFolder = QDir(basePath).dirName();

    scene.read(gameType, scnPath);

    viewer->metadata = RSDKv5::Scene::SceneEditorMetadata();

    if (gameType != ENGINE_v1) {
        QString pathTCF = WorkingDirManager::GetPath(
            "Stages/" + viewer->currentFolder + "/CollisionMasks.bin", basePath + "CollisionMasks.bin");
        QString pathSCF = WorkingDirManager::GetPath(
            "Stages/" + viewer->currentFolder + "/StageConfig.bin", basePath + "StageConfig.bin");
        QString pathBG = WorkingDirManager::GetPath(
            "Stages/" + viewer->currentFolder + "/Backgrounds.bin", basePath + "Backgrounds.bin");
        QString pathTIL = WorkingDirManager::GetPath(
            "Stages/" + viewer->currentFolder + "/128x128Tiles.bin", basePath + "128x128Tiles.bin");

        RSDKv4::TileConfig tileconfig;

        background.read(gameType, pathBG);
        chunkset.read(gameType, pathTIL);
        tileconfig.read(pathTCF);
        stageConfig.read(gameType, pathSCF);

        for (int p = 0; p < 2; ++p) {
            for (int t = 0; t < 0x400; ++t) {
                auto *dstTile = &viewer->tileconfig.collisionPaths[p][t];
                auto *srcTile = &tileconfig.collisionPaths[p][t];

                for (int c = 0; c < 16; ++c) {
                    dstTile->collision[c].height = srcTile->collision[c].height;
                    dstTile->collision[c].solid  = srcTile->collision[c].solid;
                }

                dstTile->direction  = srcTile->direction;
                dstTile->flags      = srcTile->flags;
                dstTile->floorAngle = srcTile->floorAngle;
                dstTile->lWallAngle = srcTile->lWallAngle;
                dstTile->roofAngle  = srcTile->roofAngle;
                dstTile->rWallAngle = srcTile->rWallAngle;
            }
        }
    }
    else {
        QString pathTCF = WorkingDirManager::GetPath("Stages/" + viewer->currentFolder + "/Zone.tcf",
                                                     basePath + "Zone.tcf");
        QString pathSCF = WorkingDirManager::GetPath("Stages/" + viewer->currentFolder + "/Zone.zcf",
                                                     basePath + "Zone.zcf");
        QString pathBG  = WorkingDirManager::GetPath("Stages/" + viewer->currentFolder + "/ZoneBG.map",
                                                    basePath + "ZoneBG.map");
        QString pathTIL = WorkingDirManager::GetPath("Stages/" + viewer->currentFolder + "/Zone.til",
                                                     basePath + "Zone.til");

        RSDKv1::TileConfig tileconfig;

        background.read(gameType, pathBG);
        chunkset.read(gameType, pathTIL);
        tileconfig.read(pathTCF);
        stageConfig.read(gameType, pathSCF);

        for (int p = 0; p < 2; ++p) {
            for (int t = 0; t < 0x400; ++t) {
                // auto *dstTile = &viewer->tileconfig.collisionPaths[p][t];
                // auto *srcTile = &tileconfig.collisionPaths[p][t];
                //
                // for (int c = 0; c < 16; ++c) {
                //     dstTile->collision[c].height = srcTile->collision[c].height;
                //     dstTile->collision[c].solid  = srcTile->collision[c].solid;
                // }
                //
                // dstTile->direction  = srcTile->direction;
                // dstTile->flags      = srcTile->flags;
                // dstTile->floorAngle = srcTile->floorAngle;
                // dstTile->lWallAngle = srcTile->lWallAngle;
                // dstTile->roofAngle  = srcTile->roofAngle;
                // dstTile->rWallAngle = srcTile->rWallAngle;
            }
        }
    }
    viewer->chunkset = chunkset;

    addStatusProgress(1. / 6); // finish reading files

    // Always have 8 layers, even if some have w/h of 0
    for (int l = background.layers.count(); l < 8; ++l)
        background.layers.append(FormatHelpers::Background::Layer());

    viewer->layers.clear();

    int layerID = 8;

    // Foreground
    {
        SceneHelpers::TileLayer viewLayer;

        viewLayer.name           = "Foreground";
        viewLayer.width          = scene.width;
        viewLayer.height         = scene.height;
        viewLayer.drawOrder      = layerID--;
        viewLayer.visible        = false;
        viewLayer.parallaxFactor = 1.0f;
        viewLayer.scrollSpeed    = 0.0f;
        viewLayer.type           = SceneHelpers::TileLayer::LAYER_HSCROLL;

        viewLayer.scrollInfos.clear();
        {
            SceneHelpers::TileLayer::ScrollIndexInfo scroll;

            scroll.startLine      = 0;
            scroll.length         = scene.height * 0x80;
            scroll.parallaxFactor = 1.0f;
            scroll.scrollSpeed    = 0.0f;
            scroll.deform         = false;
            scroll.unknown        = 0;

            viewLayer.scrollInfos.append(scroll);
        }

        viewLayer.layout.clear();
        for (int y = 0; y < scene.height; ++y) {
            viewLayer.layout.append(QList<ushort>());
            for (int x = 0; x < scene.width; ++x) viewLayer.layout[y].append(scene.layout[y][x]);
        }

        viewer->layers.append(viewLayer);
    }

    // Background
    for (auto &layer : background.layers) {
        SceneHelpers::TileLayer viewLayer;

        viewLayer.name           = "Background" + QString::number(layerID);
        viewLayer.width          = layer.width;
        viewLayer.height         = layer.height;
        viewLayer.drawOrder      = layerID--;
        viewLayer.visible        = false;
        viewLayer.parallaxFactor = layer.parallaxFactor / 256.0f;
        viewLayer.scrollSpeed    = layer.scrollSpeed / 256.0f;

        viewLayer.type = SceneHelpers::TileLayer::LAYER_NONE;
        switch (layer.type) {
            default: break;
            case 1: viewLayer.type = SceneHelpers::TileLayer::LAYER_HSCROLL; break;
            case 2: viewLayer.type = SceneHelpers::TileLayer::LAYER_VSCROLL; break;
            case 3: viewLayer.type = SceneHelpers::TileLayer::LAYER_FLOOR3D; break;
            case 4: viewLayer.type = SceneHelpers::TileLayer::LAYER_SKY3D; break;
        }

        viewLayer.scrollInfos.clear();
        for (auto &info : layer.scrollInfos) {
            SceneHelpers::TileLayer::ScrollIndexInfo scroll;

            scroll.startLine      = info.startLine;
            scroll.length         = info.length;
            scroll.parallaxFactor = info.parallaxFactor;
            scroll.scrollSpeed    = info.scrollSpeed;
            scroll.deform         = info.deform;
            scroll.unknown        = 0;

            viewLayer.scrollInfos.append(scroll);
        }

        viewLayer.layout.clear();
        for (int y = 0; y < layer.height; ++y) {
            viewLayer.layout.append(QList<ushort>());
            for (int x = 0; x < layer.width; ++x) viewLayer.layout[y].append(layer.layout[y][x]);
        }

        viewer->layers.append(viewLayer);
    }

    for (int a = 0; a < 4; ++a) {
        byte id = scene.activeLayer[a];
        if (id < 9)
            viewer->layers[id].visible = true;
    }

    addStatusProgress(1. / 6); // finish setting up layers

    if (gameType != ENGINE_v1) {
        scene.objectTypeNames.clear();

        if (stageConfig.loadGlobalScripts) {
            if (gameType == ENGINE_v2)
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

        for (QString &obj : globalsRS) {
            scene.objectTypeNames.append(obj);
        }

        for (FormatHelpers::StageConfig::ObjectInfo &obj : stageConfig.objects) {
            scene.objectTypeNames.append(obj.name);
        }
    }

    viewer->objects.clear();
    viewer->entities.clear();

    SceneObject blankInfo;
    blankInfo.name = "Blank Object";
    viewer->objects.append(blankInfo);

    for (int i = 0; i < scene.objectTypeNames.count(); ++i) {
        SceneObject info;
        info.name = scene.objectTypeNames[i];
        info.variables.clear();
        viewer->objects.append(info);
    }

    for (int i = 0; i < scene.objects.count(); ++i) {
        SceneEntity info;
        info.slotID        = scene.objects[i].slotID;
        info.prevSlot      = info.slotID;
        info.type          = scene.objects[i].type;
        info.propertyValue = scene.objects[i].propertyValue;
        info.pos.x         = scene.objects[i].getX();
        info.pos.y         = scene.objects[i].getY();

        if (gameType == ENGINE_v4) {
            for (int v = 0; v < 0xF; ++v) {
                info.sceneVariables[v] = scene.objects[i].variables[v];
            }
        }

        viewer->entities.append(info);
    }

    addStatusProgress(1. / 6); // finish objects & entities

    QImage tileset(16, 0x400 * 16, QImage::Format_Indexed8);
    if (gameType == ENGINE_v1) {
        if (QFile::exists(basePath + "Zone.gfx")) {
            // setup tileset texture from png
            RSDKv1::GFX gfx(basePath + "Zone.gfx");
            tileset = gfx.exportImage();
        }
    }
    else {
        if (QFile::exists(basePath + "16x16Tiles.gif")) {
            // setup tileset texture from png
            QGifImage tilesetGif(basePath + "16x16Tiles.gif");
            tileset = tilesetGif.frame(0);
        }
    }

    viewer->gameType = gameType;
    viewer->initScene(tileset);

    addStatusProgress(1. / 6); // finish building tiles & chunks

    ui->layerList->blockSignals(true);
    ui->layerList->clear();
    QListWidgetItem *itemFG = new QListWidgetItem("Foreground", ui->layerList);
    itemFG->setFlags(itemFG->flags() | Qt::ItemIsUserCheckable);
    itemFG->setCheckState(viewer->layers[0].visible ? Qt::Checked : Qt::Unchecked);

    for (int l = 0; l < background.layers.count(); ++l) {
        QListWidgetItem *item =
            new QListWidgetItem("Background " + QString::number(l + 1), ui->layerList);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(viewer->layers[l + 1].visible ? Qt::Checked : Qt::Unchecked);
    }
    ui->layerList->blockSignals(false);

    ui->objectList->blockSignals(true);
    ui->objectList->clear();
    for (int o = 0; o < viewer->objects.count(); ++o) {
        QListWidgetItem *item = new QListWidgetItem(viewer->objects[o].name, ui->objectList);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(viewer->objects[o].visible ? Qt::Checked : Qt::Unchecked);
    }
    ui->objectList->blockSignals(false);

    createEntityList();

    ui->horizontalScrollBar->setMaximum((viewer->sceneWidth * 0x80) - viewer->storedW);
    ui->verticalScrollBar->setMaximum((viewer->sceneHeight * 0x80) - viewer->storedH);
    ui->horizontalScrollBar->setPageStep(0x80);
    ui->verticalScrollBar->setPageStep(0x80);

    if (viewer->gameType == ENGINE_v1) {
        scnProp->musBox->clear();

        for (int m = 0; m < stageConfig.music.count(); ++m)
            scnProp->musBox->addItem(stageConfig.music[m]);
    }
    scnProp->setupUI(&scene, viewer->gameType);

    scnProp->loadGlobalCB->blockSignals(true);
    scnProp->loadGlobalCB->setDisabled(viewer->gameType == ENGINE_v1);
    scnProp->loadGlobalCB->setChecked(stageConfig.loadGlobalScripts);
    scnProp->loadGlobalCB->blockSignals(false);

    if (chkProp) {
        ui->chunksPage->layout()->removeWidget(chkProp);
        delete chkProp;
    }

    chkProp = new ChunkSelector(this);
    chkProp->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->chunksPage->layout()->addWidget(chkProp);
    chkProp->show();

    ui->layerList->setCurrentRow(0);
    viewer->selectedLayer = 0;

    ui->toolBox->setCurrentIndex(0);
    ui->propertiesBox->setCurrentIndex(0);

    scnProp->setupUI(&scene, viewer->gameType);
    lyrProp->setupUI(&scene, &background, 0, viewer->gameType);
    tileProp->unsetUI();
    objProp->unsetUI();
    scrProp->unsetUI();

    initGameLink();

    ui->addEnt->setDisabled(viewer->entities.count() >= FormatHelpers::Scene::entityLimit);

    tabTitle = Utils::getFilenameAndFolder(scnPath);

    viewer->objectsLoaded = true;
    clearActions();
    appConfig.addRecentFile(viewer->gameType, TOOL_SCENEEDITOR, scnPath, QList<QString>{ gcfPath });
    setStatus("Loaded scene " + QFileInfo(scnPath).fileName()); // done!
}

bool SceneEditor::saveScene(bool forceSaveAs)
{
    byte saveVer     = ENGINE_v4;
    QString savePath = "";
    if (!forceSaveAs && QFile::exists(scene.filepath)) {
        savePath = scene.filepath;
        saveVer  = viewer->gameType;
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
        filedialog.selectNameFilter(types[viewer->gameType - 1]);
        filedialog.setAcceptMode(QFileDialog::AcceptSave);

        if (filedialog.exec() == QDialog::Accepted) {
            saveVer  = types.indexOf(filedialog.selectedNameFilter()) + ENGINE_v4;
            savePath = filedialog.selectedFiles()[0];

            return true;
        }
        else {
            return false;
        }
    }

    QString basePath = savePath.replace(QFileInfo(savePath).fileName(), "");

    setStatus("Saving scene...", true);
    FormatHelpers::Gif tileset(16, 0x400 * 16);

    int c = 0;
    for (PaletteColour &col : viewer->tilePalette) tileset.palette[c++] = col.toQColor();

    int pos = 0;
    for (int i = 0; i < 0x400; ++i) {
        uchar *src = viewer->tiles[i].bits();
        for (int y = 0; y < 16; ++y) {
            for (int x = 0; x < 16; ++x) tileset.pixels[pos++] = *src++;
        }
    }
    addStatusProgress(1.f / 5); // created tileset

    // FG Layer
    {
        auto &viewLayer = viewer->layers[0];

        scene.width  = viewLayer.width;
        scene.height = viewLayer.height;

        scene.layout.clear();
        for (int y = 0; y < viewLayer.height; ++y) {
            scene.layout.append(QList<ushort>());
            for (int x = 0; x < viewLayer.width; ++x) scene.layout[y].append(viewLayer.layout[y][x]);
        }
    }

    // BG Layers
    background.layers.clear();
    for (int l = 1; l < viewer->layers.count(); ++l) {
        auto &viewLayer = viewer->layers[l];

        FormatHelpers::Background::Layer layer;

        layer.width          = viewLayer.width;
        layer.height         = viewLayer.height;
        layer.parallaxFactor = viewLayer.parallaxFactor;
        layer.scrollSpeed    = viewLayer.scrollSpeed;

        layer.type = 0;
        switch (layer.type) {
            default: break;
            case SceneHelpers::TileLayer::LAYER_HSCROLL: layer.type = 1; break;
            case SceneHelpers::TileLayer::LAYER_VSCROLL: layer.type = 2; break;
            case SceneHelpers::TileLayer::LAYER_ROTOZOOM: layer.type = 3; break;
            case SceneHelpers::TileLayer::LAYER_BASIC: layer.type = 4; break;
        }

        layer.scrollInfos.clear();
        for (auto &info : viewLayer.scrollInfos) {
            FormatHelpers::Background::ScrollIndexInfo scroll;

            scroll.startLine      = info.startLine;
            scroll.length         = info.length;
            scroll.parallaxFactor = info.parallaxFactor;
            scroll.scrollSpeed    = info.scrollSpeed;
            scroll.deform         = info.deform;

            layer.scrollInfos.append(scroll);
        }

        layer.layout.clear();
        for (int y = 0; y < viewLayer.height; ++y) {
            layer.layout.append(QList<ushort>());
            for (int x = 0; x < viewLayer.width; ++x) layer.layout[y].append(viewLayer.layout[y][x]);
        }

        background.layers.append(layer);
    }
    addStatusProgress(1.f / 5); // created tile layers

    scene.objects.clear();
    scene.objectTypeNames.clear();

    int globalObjCount = 0;
    if (stageConfig.loadGlobalScripts) {
        if (viewer->gameType == ENGINE_v2)
            globalObjCount += 1; // player

        globalObjCount += gameConfig.objects.count();
    }

    int objID = 0;
    for (auto &obj : viewer->objects) {
        if (objID >= globalObjCount)
            scene.objectTypeNames.append(obj.name);

        objID++;
    }
    addStatusProgress(1.f / 5); // created object list

    for (auto &ent : viewer->entities) {
        FormatHelpers::Scene::Object entity;
        entity.type          = ent.type;
        entity.propertyValue = ent.propertyValue;
        entity.pos.x         = ent.pos.x * 65536;
        entity.pos.y         = ent.pos.y * 65536;
        entity.slotID        = ent.slotID;

        for (int v = 0; v < 15; ++v) {
            entity.variables[v].value  = ent.sceneVariables[v].value;
            entity.variables[v].active = ent.sceneVariables[v].active;
        }

        scene.objects.append(entity);
    }
    addStatusProgress(1.f / 5); // created entity list

    if (viewer->gameType != ENGINE_v1) {
        RSDKv4::TileConfig tileconfig;

        for (int p = 0; p < 2; ++p) {
            for (int t = 0; t < 0x400; ++t) {
                auto *dstTile = &tileconfig.collisionPaths[p][t];
                auto *srcTile = &viewer->tileconfig.collisionPaths[p][t];

                for (int c = 0; c < 16; ++c) {
                    dstTile->collision[c].height = srcTile->collision[c].height;
                    dstTile->collision[c].solid  = srcTile->collision[c].solid;
                }

                dstTile->direction  = srcTile->direction;
                dstTile->flags      = srcTile->flags;
                dstTile->floorAngle = srcTile->floorAngle;
                dstTile->lWallAngle = srcTile->lWallAngle;
                dstTile->roofAngle  = srcTile->roofAngle;
                dstTile->rWallAngle = srcTile->rWallAngle;
            }
        }

        scene.write(viewer->gameType, savePath);
        background.write(viewer->gameType, basePath + "Backgrounds.bin");
        viewer->chunkset.write(viewer->gameType, basePath + "128x128Tiles.bin");
        tileconfig.write(basePath + "CollisionMasks.bin");
        stageConfig.write(viewer->gameType, basePath + "StageConfig.bin");
        tileset.write(basePath + "16x16Tiles.gif");
    }
    else {
        RSDKv1::TileConfig tileconfig;

        // TODO: port data

        for (int p = 0; p < 2; ++p) {
            for (int t = 0; t < 0x400; ++t) {
                // auto *dstTile = &tileconfig.collisionPaths[p][t];
                // auto *srcTile = &viewer->tileconfig.collisionPaths[p][t];
                //
                // for (int c = 0; c < 16; ++c) {
                //     dstTile->collision[c].height = srcTile->collision[c].height;
                //     dstTile->collision[c].solid  = srcTile->collision[c].solid;
                // }
                //
                // dstTile->direction  = srcTile->direction;
                // dstTile->flags      = srcTile->flags;
                // dstTile->floorAngle = srcTile->floorAngle;
                // dstTile->lWallAngle = srcTile->lWallAngle;
                // dstTile->roofAngle  = srcTile->roofAngle;
                // dstTile->rWallAngle = srcTile->rWallAngle;
            }
        }

        scene.write(viewer->gameType, savePath);
        background.write(viewer->gameType, basePath + "ZoneBG.map");
        viewer->chunkset.write(viewer->gameType, basePath + "Zone.til");
        tileconfig.write(basePath + "Zone.tcf");
        stageConfig.write(viewer->gameType, basePath + "Zone.zcf");

        RSDKv1::GFX *gfx = new RSDKv1::GFX;
        gfx->importImage(tileset);
        gfx->write(basePath + "Zone.gfx");

        delete gfx;
    }

    tabTitle = Utils::getFilenameAndFolder(savePath);
    clearActions();
    setStatus("Saved scene to " + Utils::getFilenameAndFolder(savePath));
    return true;
}

void SceneEditor::initGameLink()
{
    compilerv2->clearScriptData();
    compilerv3->clearScriptData();
    compilerv4->clearScriptData();

    for (int i = 0; i < ENTITY_COUNT; ++i) {
        memset(&compilerv2->objectEntityList[i], 0, sizeof(compilerv2->objectEntityList[i]));
        memset(&compilerv3->objectEntityList[i], 0, sizeof(compilerv3->objectEntityList[i]));
        memset(&compilerv4->objectEntityList[i], 0, sizeof(compilerv4->objectEntityList[i]));

        if (i < viewer->entities.count()) {
            compilerv2->objectEntityList[i].type          = viewer->entities[i].type;
            compilerv2->objectEntityList[i].propertyValue = viewer->entities[i].propertyValue;
            compilerv2->objectEntityList[i].XPos          = viewer->entities[i].pos.x * 65536;
            compilerv2->objectEntityList[i].YPos          = viewer->entities[i].pos.y * 65536;

            compilerv3->objectEntityList[i].type          = viewer->entities[i].type;
            compilerv3->objectEntityList[i].propertyValue = viewer->entities[i].propertyValue;
            compilerv3->objectEntityList[i].XPos          = viewer->entities[i].pos.x * 65536;
            compilerv3->objectEntityList[i].YPos          = viewer->entities[i].pos.y * 65536;

            compilerv4->objectEntityList[i].type          = viewer->entities[i].type;
            compilerv4->objectEntityList[i].propertyValue = viewer->entities[i].propertyValue;
            compilerv4->objectEntityList[i].XPos          = viewer->entities[i].pos.x * 65536;
            compilerv4->objectEntityList[i].YPos          = viewer->entities[i].pos.y * 65536;

            int *values[] = { &compilerv4->objectEntityList[i].state,
                              NULL,
                              &compilerv4->objectEntityList[i].scale,
                              &compilerv4->objectEntityList[i].rotation,
                              NULL,
                              NULL,
                              &compilerv4->objectEntityList[i].alpha,
                              NULL,
                              &compilerv4->objectEntityList[i].animationSpeed,
                              NULL,
                              NULL,
                              &compilerv4->objectEntityList[i].values[0],
                              &compilerv4->objectEntityList[i].values[1],
                              &compilerv4->objectEntityList[i].values[2],
                              &compilerv4->objectEntityList[i].values[3] };

            byte *valuesB[] = { NULL,
                                &compilerv4->objectEntityList[i].direction,
                                NULL,
                                NULL,
                                (byte *)&compilerv4->objectEntityList[i].drawOrder,
                                &compilerv4->objectEntityList[i].priority,
                                NULL,
                                &compilerv4->objectEntityList[i].animation,
                                NULL,
                                &compilerv4->objectEntityList[i].frame,
                                &compilerv4->objectEntityList[i].inkEffect,
                                NULL,
                                NULL,
                                NULL,
                                NULL };

            for (int v = 0; v < 0x0F; ++v) {
                if (viewer->entities[i].sceneVariables[v].active) {
                    if (values[v])
                        *values[v] = viewer->entities[i].sceneVariables[v].value;
                    else
                        *valuesB[v] = viewer->entities[i].sceneVariables[v].value;
                }
            }
        }
    }

    compilerv2->clearScriptData();
    compilerv3->clearScriptData();
    compilerv4->clearScriptData();
    int id                      = 0;
    compilerv3->typeNames[id]   = "Blank Object";
    compilerv4->typeNames[id++] = "Blank Object";
    for (int o = 0; o < gameConfig.objects.count(); ++o) {
        compilerv3->typeNames[id] = gameConfig.objects[o].name;
        compilerv4->typeNames[id] = gameConfig.objects[o].name;

        compilerv3->typeNames[id].replace(" ", "");
        compilerv4->typeNames[id].replace(" ", "");

        id++;
    }
    for (int o = 0; o < stageConfig.objects.count(); ++o) {
        compilerv3->typeNames[id] = stageConfig.objects[o].name;
        compilerv4->typeNames[id] = stageConfig.objects[o].name;

        compilerv3->typeNames[id].replace(" ", "");
        compilerv4->typeNames[id].replace(" ", "");

        id++;
    }

    for (int v = 0; v < gameConfig.globalVariables.count(); ++v) {
        compilerv2->globalVariables.append(gameConfig.globalVariables[v].name);
        compilerv3->globalVariables.append(gameConfig.globalVariables[v].name);
        compilerv4->globalVariables.append(gameConfig.globalVariables[v].name);
    }

    switch (viewer->gameType) {
        case ENGINE_v1: break; // read the editor stuff from this somehow (idk how to parse it lol)
        case ENGINE_v2: {      // parse the RSDK sub and use that data to know what to draw
            int scrID          = 1;
            compilerv2->editor = this;

            if (stageConfig.loadGlobalScripts) {
                for (int i = 0; i < gameConfig.objects.count(); ++i) {
                    QString scriptPath = viewer->dataPath + "/Scripts/" + gameConfig.objects[i].script;
                    scriptPath = WorkingDirManager::GetPath("/Scripts/" + gameConfig.objects[i].script,
                                                            scriptPath);

                    compilerv2->parseScriptFile(scriptPath, scrID++);

                    if (compilerv2->scriptError) {
                        printLog(compilerv2->errorMsg);
                        printLog(compilerv2->errorPos);
                        printLog(QString::number(compilerv2->errorLine));

                        QFileInfo info(compilerv2->errorScr);
                        QDir dir(info.dir());
                        dir.cdUp();
                        QString dirFile = dir.relativeFilePath(compilerv2->errorScr);

                        setStatus("Failed to compile script: " + dirFile);
                        compilerv2->objectScriptList[scrID - 1].subRSDK.scriptCodePtr = -1;
                        compilerv2->objectScriptList[scrID - 1].subRSDK.jumpTablePtr  = -1;
                    }
                }
            }

            for (int i = 0; i < stageConfig.objects.count(); ++i) {
                QString scriptPath = viewer->dataPath + "/Scripts/" + stageConfig.objects[i].script;
                scriptPath =
                    WorkingDirManager::GetPath("/Scripts/" + stageConfig.objects[i].script, scriptPath);
                compilerv2->parseScriptFile(scriptPath, scrID++);

                if (compilerv2->scriptError) {
                    printLog(compilerv2->errorMsg);
                    printLog(compilerv2->errorPos);
                    printLog(QString::number(compilerv2->errorLine));

                    QFileInfo info(compilerv2->errorScr);
                    QDir dir(info.dir());
                    dir.cdUp();
                    QString dirFile = dir.relativeFilePath(compilerv2->errorScr);

                    setStatus("Failed to compile script: " + dirFile);
                    compilerv2->objectScriptList[scrID - 1].subRSDK.scriptCodePtr = -1;
                    compilerv2->objectScriptList[scrID - 1].subRSDK.jumpTablePtr  = -1;
                }
            }
            break;
        }
        case ENGINE_v3: { // compile RSDKDraw & RSDKLoad and draw via those
            int scrID                     = 1;
            compilerv3->editor            = this;
            compilerv3->gamePlatform      = "Editor";
            compilerv3->gameRenderType    = "SW_Rendering";
            compilerv3->gameHapticSetting = "No_Haptics";

            if (stageConfig.loadGlobalScripts) {
                for (int i = 0; i < gameConfig.objects.count(); ++i) {
                    QString scriptPath = viewer->dataPath + "/Scripts/" + gameConfig.objects[i].script;
                    scriptPath = WorkingDirManager::GetPath("/Scripts/" + gameConfig.objects[i].script,
                                                            scriptPath);
                    compilerv3->parseScriptFile(scriptPath, scrID++);

                    if (compilerv3->scriptError) {
                        printLog(compilerv3->errorMsg);
                        printLog(compilerv3->errorPos);
                        printLog(QString::number(compilerv3->errorLine));

                        QFileInfo info(compilerv3->errorScr);
                        QDir dir(info.dir());
                        dir.cdUp();
                        QString dirFile = dir.relativeFilePath(compilerv3->errorScr);

                        QMessageBox msgBox(QMessageBox::Information, "RetroED",
                                           QString("Failed to compile script: %1\n"
                                                   "Error: %2\n"
                                                   "\n"
                                                   "Pos: %3\n"
                                                   "Line: %4\n")
                                               .arg(gameConfig.objects[i].script)
                                               .arg(compilerv3->errorMsg)
                                               .arg(compilerv3->errorPos)
                                               .arg(compilerv3->errorLine),
                                           QMessageBox::Ok);
                        msgBox.exec();

                        setStatus("Failed to compile script: " + dirFile);
                        compilerv3->objectScriptList[scrID - 1].subRSDKDraw.scriptCodePtr = -1;
                        compilerv3->objectScriptList[scrID - 1].subRSDKDraw.jumpTablePtr  = -1;
                        compilerv3->objectScriptList[scrID - 1].subRSDKLoad.scriptCodePtr = -1;
                        compilerv3->objectScriptList[scrID - 1].subRSDKLoad.jumpTablePtr  = -1;
                        compilerv3->objectScriptList[scrID - 1].subRSDKEdit.scriptCodePtr = -1;
                        compilerv3->objectScriptList[scrID - 1].subRSDKEdit.jumpTablePtr  = -1;
                    }
                }
            }

            for (int i = 0; i < stageConfig.objects.count(); ++i) {
                QString scriptPath = viewer->dataPath + "/Scripts/" + stageConfig.objects[i].script;
                scriptPath =
                    WorkingDirManager::GetPath("/Scripts/" + stageConfig.objects[i].script, scriptPath);
                compilerv3->parseScriptFile(scriptPath, scrID++);

                if (compilerv3->scriptError) {
                    printLog(compilerv3->errorMsg);
                    printLog(compilerv3->errorPos);
                    printLog(QString::number(compilerv3->errorLine));

                    QFileInfo info(compilerv3->errorScr);
                    QDir dir(info.dir());
                    dir.cdUp();
                    QString dirFile = dir.relativeFilePath(compilerv3->errorScr);

                    QMessageBox msgBox(QMessageBox::Information, "RetroED",
                                       QString("Failed to compile script: %1\n"
                                               "Error: %2\n"
                                               "\n"
                                               "Pos: %3\n"
                                               "Line: %4\n")
                                           .arg(stageConfig.objects[i].script)
                                           .arg(compilerv3->errorMsg)
                                           .arg(compilerv3->errorPos)
                                           .arg(compilerv3->errorLine),
                                       QMessageBox::Ok);
                    msgBox.exec();

                    setStatus("Failed to compile script: " + dirFile);
                    compilerv3->objectScriptList[scrID - 1].subRSDKDraw.scriptCodePtr = -1;
                    compilerv3->objectScriptList[scrID - 1].subRSDKDraw.jumpTablePtr  = -1;
                    compilerv3->objectScriptList[scrID - 1].subRSDKLoad.scriptCodePtr = -1;
                    compilerv3->objectScriptList[scrID - 1].subRSDKLoad.jumpTablePtr  = -1;
                    compilerv3->objectScriptList[scrID - 1].subRSDKEdit.scriptCodePtr = -1;
                    compilerv3->objectScriptList[scrID - 1].subRSDKEdit.jumpTablePtr  = -1;
                }
            }
            break;
        }
        case ENGINE_v4: { // compile RSDKDraw & RSDKLoad and draw via those
            int scrID                     = 1;
            compilerv4->editor            = this;
            compilerv4->gamePlatform      = "EDITOR";
            compilerv4->gameRenderType    = "SW_RENDERING";
            compilerv4->gameHapticSetting = "NO_F_FEEDBACK";

            if (stageConfig.loadGlobalScripts) {
                for (int i = 0; i < gameConfig.objects.count(); ++i) {
                    QString scriptPath =
                        viewer->dataPath + "/../Scripts/" + gameConfig.objects[i].script;
                    scriptPath = WorkingDirManager::GetPath(
                        "/../Scripts/" + gameConfig.objects[i].script, scriptPath);
                    compilerv4->parseScriptFile(scriptPath, scrID++);

                    if (compilerv4->scriptError) {
                        printLog(compilerv4->errorMsg);
                        printLog(compilerv4->errorPos);
                        printLog(QString::number(compilerv4->errorLine));

                        QFileInfo info(compilerv4->errorScr);
                        QDir dir(info.dir());
                        dir.cdUp();
                        QString dirFile = dir.relativeFilePath(compilerv4->errorScr);

                        QMessageBox msgBox(QMessageBox::Information, "RetroED",
                                           QString("Failed to compile script: %1\n"
                                                   "Error: %2\n"
                                                   "\n"
                                                   "Pos: %3\n"
                                                   "Line: %4\n")
                                               .arg(gameConfig.objects[i].script)
                                               .arg(compilerv4->errorMsg)
                                               .arg(compilerv4->errorPos)
                                               .arg(compilerv4->errorLine),
                                           QMessageBox::Ok);
                        msgBox.exec();

                        setStatus("Failed to compile script: " + dirFile);
                        compilerv4->objectScriptList[scrID - 1].eventRSDKDraw.scriptCodePtr = -1;
                        compilerv4->objectScriptList[scrID - 1].eventRSDKDraw.jumpTablePtr  = -1;
                        compilerv4->objectScriptList[scrID - 1].eventRSDKLoad.scriptCodePtr = -1;
                        compilerv4->objectScriptList[scrID - 1].eventRSDKLoad.jumpTablePtr  = -1;
                        compilerv4->objectScriptList[scrID - 1].eventRSDKEdit.scriptCodePtr = -1;
                        compilerv4->objectScriptList[scrID - 1].eventRSDKEdit.jumpTablePtr  = -1;
                    }
                }
            }

            for (int i = 0; i < stageConfig.objects.count(); ++i) {
                QString scriptPath = viewer->dataPath + "/../Scripts/" + stageConfig.objects[i].script;
                scriptPath = WorkingDirManager::GetPath("/../Scripts/" + stageConfig.objects[i].script,
                                                        scriptPath);
                compilerv4->parseScriptFile(scriptPath, scrID++);

                if (compilerv4->scriptError) {
                    printLog(compilerv4->errorMsg);
                    printLog(compilerv4->errorPos);
                    printLog(QString::number(compilerv4->errorLine));

                    QFileInfo info(compilerv4->errorScr);
                    QDir dir(info.dir());
                    dir.cdUp();
                    QString dirFile = dir.relativeFilePath(compilerv4->errorScr);

                    setStatus("Failed to compile script: " + dirFile);

                    QMessageBox msgBox(QMessageBox::Information, "RetroED",
                                       QString("Failed to compile script: %1\n"
                                               "Error: %2\n"
                                               "\n"
                                               "Pos: %3\n"
                                               "Line: %4\n")
                                           .arg(stageConfig.objects[i].script)
                                           .arg(compilerv4->errorMsg)
                                           .arg(compilerv4->errorPos)
                                           .arg(compilerv4->errorLine),
                                       QMessageBox::Ok);
                    msgBox.exec();

                    compilerv4->objectScriptList[scrID - 1].eventRSDKDraw.scriptCodePtr = -1;
                    compilerv4->objectScriptList[scrID - 1].eventRSDKDraw.jumpTablePtr  = -1;
                    compilerv4->objectScriptList[scrID - 1].eventRSDKLoad.scriptCodePtr = -1;
                    compilerv4->objectScriptList[scrID - 1].eventRSDKLoad.jumpTablePtr  = -1;
                    compilerv4->objectScriptList[scrID - 1].eventRSDKEdit.scriptCodePtr = -1;
                    compilerv4->objectScriptList[scrID - 1].eventRSDKEdit.jumpTablePtr  = -1;
                }
            }
            break;
        }
    }

    compilerv3->objectLoop      = ENTITY_COUNT - 1;
    compilerv4->objectEntityPos = ENTITY_COUNT - 1;
    for (int o = 0; o < viewer->objects.count(); ++o) {
        viewer->objects[o].variables.clear();
        viewer->objects[o].variablesAliases[VAR_ALIAS_PROPVAL] = "PropertyValue";
        viewer->objects[o].variablesAliases[VAR_ALIAS_VAL0]    = "Value0";
        viewer->objects[o].variablesAliases[VAR_ALIAS_VAL1]    = "Value1";
        viewer->objects[o].variablesAliases[VAR_ALIAS_VAL2]    = "Value2";
        viewer->objects[o].variablesAliases[VAR_ALIAS_VAL3]    = "Value3";

        viewer->activeVarObj = o;
        callGameEvent(SceneViewerv5::EVENT_LOAD, o);
    }
    viewer->activeVarObj = -1;

    for (int e = 0; e < viewer->entities.count(); ++e) {
        viewer->entities[e].variables.clear();
        for (auto &var : viewer->objects[viewer->entities[e].type].variables) {
            RSDKv5::Scene::VariableValue val;
            val.type        = VAR_UINT8;
            val.value_uint8 = 0;
            viewer->entities[e].variables.append(val);
        }
    }

    // Initial property value check (used for capping the value & etc)
    for (int e = 0; e < viewer->entities.count(); ++e) {
        auto *entity   = &viewer->entities[e];
        auto *entityv2 = &compilerv2->objectEntityList[viewer->selectedEntity];
        auto *entityv3 = &compilerv3->objectEntityList[viewer->selectedEntity];
        auto *entityv4 = &compilerv4->objectEntityList[viewer->selectedEntity];

        if (entityv2)
            entityv2->propertyValue = entity->propertyValue;
        if (entityv3)
            entityv3->propertyValue = entity->propertyValue;
        if (entityv4)
            entityv4->propertyValue = entity->propertyValue;

        bool called  = false;
        byte propVal = objProp->callRSDKEdit(this, true, e, -1, 0, &called);
        if (called) {
            entity->propertyValue = propVal;
            if (entityv2)
                entityv2->propertyValue = propVal;
            if (entityv3)
                entityv3->propertyValue = propVal;
            if (entityv4)
                entityv4->propertyValue = propVal;
        }
    }
}

void SceneEditor::setTile(float x, float y)
{
    if (viewer->selectedTile < 0 || viewer->selectedLayer < 0)
        return;
    float tx = x;
    float ty = y;

    tx *= viewer->invZoom();
    ty *= viewer->invZoom();

    float tx2 = tx + fmodf(viewer->cameraPos.x, 0x80);
    float ty2 = ty + fmodf(viewer->cameraPos.y, 0x80);

    // clip to grid
    tx -= fmodf(tx2, 0x80);
    ty -= fmodf(ty2, 0x80);

    // Draw Selected Tile Preview
    float xpos = tx + viewer->cameraPos.x;
    float ypos = ty + viewer->cameraPos.y;

    xpos /= 0x80;
    ypos /= 0x80;
    if (ypos >= 0 && ypos < viewer->sceneHeight) {
        if (xpos >= 0 && xpos < viewer->sceneWidth) {
            viewer->layers[viewer->selectedLayer].layout[ypos][xpos] = viewer->selectedTile;
        }
    }
};

void SceneEditor::resetTools(byte tool)
{
    if (tool == 0xFF)
        tool = SceneViewerv5::TOOL_MOUSE;
    viewer->curTool = tool;

    // Reset

    ui->selToolBox->setDisabled(true);
    byte index = 0;
    switch (tool) {
        default:
        case SceneViewerv5::TOOL_MOUSE: index = 0; break;
        case SceneViewerv5::TOOL_SELECT: index = 1; break;
        case SceneViewerv5::TOOL_PENCIL: index = 2; break;
        case SceneViewerv5::TOOL_ERASER: index = 3; break;
        case SceneViewerv5::TOOL_ENTITY: index = 4; break;
        case SceneViewerv5::TOOL_PARALLAX: index = 5; break;
    }
    ui->selToolBox->setCurrentIndex(index);
    ui->selToolBox->setDisabled(false);
    viewer->selectedEntity = -1;
    objProp->unsetUI();
    viewer->isSelecting = false;

    unsetCursor();

    switch (tool) {
        default: break; // what
        case SceneViewerv5::TOOL_MOUSE: break;
        case SceneViewerv5::TOOL_SELECT: break;
        case SceneViewerv5::TOOL_PENCIL: viewer->isSelecting = true; break;
        case SceneViewerv5::TOOL_ERASER: viewer->isSelecting = true; break;
        case SceneViewerv5::TOOL_ENTITY: viewer->isSelecting = true; break;
        case SceneViewerv5::TOOL_PARALLAX: viewer->isSelecting = true; break;
    }
};

bool SceneEditor::handleKeyPress(QKeyEvent *event)
{

    Vector2<int> sceneMousePos((viewer->mousePos.x * viewer->invZoom()) + viewer->cameraPos.x,
                               (viewer->mousePos.y * viewer->invZoom()) + viewer->cameraPos.y);

    if (event->key() == Qt::Key_Control)
        ctrlDownL = true;
    if (event->key() == Qt::Key_Alt)
        altDownL = true;
    if (event->key() == Qt::Key_Shift)
        shiftDownL = true;

    if ((event->modifiers() & Qt::ControlModifier) == Qt::ControlModifier
        && event->key() == Qt::Key_V) {
        if (clipboard) {
            switch (clipboardType) {
                default: break;
                case COPY_ENTITY: {
                    if (viewer->entities.count() < FormatHelpers::Scene::entityLimit) {
                        SceneEntity entity = *(SceneEntity *)clipboard;
                        entity.pos.x       = (sceneMousePos.x);
                        entity.pos.y       = (sceneMousePos.y);

                        int cnt = viewer->entities.count();
                        viewer->entities.append(entity);
                        compilerv2->objectEntityList[cnt].XPos = entity.pos.x * 65536;
                        compilerv2->objectEntityList[cnt].YPos = entity.pos.y * 65536;

                        compilerv3->objectEntityList[cnt].XPos = entity.pos.x * 65536;
                        compilerv3->objectEntityList[cnt].YPos = entity.pos.y * 65536;

                        compilerv4->objectEntityList[cnt].XPos = entity.pos.x * 65536;
                        compilerv4->objectEntityList[cnt].YPos = entity.pos.y * 65536;
                    }
                    else {
                        QMessageBox msgBox =
                            QMessageBox(QMessageBox::Information, "RetroED",
                                        QString("Entity Cap has been reached.\nUnable to add "
                                                "new entity.\nPlease remove an entity first."),
                                        QMessageBox::NoButton, this);
                        msgBox.exec();
                    }
                    break;
                }
            }
        }
    }

    byte prevTool = viewer->curTool;
    byte tool     = viewer->curTool;
    if (event->key() == Qt::Key_S)
        tool = SceneViewerv5::TOOL_MOUSE;
    if (event->key() == Qt::Key_S)
        tool = SceneViewerv5::TOOL_SELECT;
    if (event->key() == Qt::Key_C)
        tool = SceneViewerv5::TOOL_PENCIL;
    if (event->key() == Qt::Key_E)
        tool = SceneViewerv5::TOOL_ERASER;
    if (event->key() == Qt::Key_O)
        tool = SceneViewerv5::TOOL_ENTITY;
    if (event->key() == Qt::Key_P)
        tool = SceneViewerv5::TOOL_PARALLAX;

    if (tool != prevTool)
        resetTools(tool);

    switch (viewer->curTool) {
        case SceneViewerv5::TOOL_MOUSE: break;

        case SceneViewerv5::TOOL_SELECT: break;

        case SceneViewerv5::TOOL_PENCIL:
            if (event->key() == Qt::Key_Z)
                viewer->tileFlip.x = true;

            if (event->key() == Qt::Key_X)
                viewer->tileFlip.y = true;

            if (event->key() == Qt::Key_Delete || event->key() == Qt::Key_Backspace) {
                if (viewer->selectedLayer >= 0) {
                    Rect<float> box;

                    for (int y = 0; y < viewer->sceneHeight; ++y) {
                        for (int x = 0; x < viewer->sceneWidth; ++x) {
                            box = Rect<float>(x * 0x80, y * 0x80, 0x80, 0x80);

                            Vector2<float> pos = Vector2<float>(
                                (viewer->tilePos.x * viewer->invZoom()) + viewer->cameraPos.x,
                                (viewer->tilePos.y * viewer->invZoom()) + viewer->cameraPos.y);
                            if (box.contains(pos)) {
                                viewer->layers[viewer->selectedLayer].layout[y][x] = 0;
                                doAction();

                                // reset context
                                break;
                            }
                        }
                    }
                }
            }
            break;

        case SceneViewerv5::TOOL_ERASER: break;

        case SceneViewerv5::TOOL_ENTITY:
            if (event->key() == Qt::Key_Delete || event->key() == Qt::Key_Backspace) {
                if (viewer->selectedEntity >= 0) {
                    viewer->entities.removeAt(viewer->selectedEntity);
                    viewer->selectedEntity = -1;

                    objProp->unsetUI();
                    createEntityList();
                    doAction();
                }
            }

            if ((event->modifiers() & Qt::ControlModifier) == Qt::ControlModifier
                && event->key() == Qt::Key_C) {
                if (viewer->selectedEntity >= 0) {
                    clipboard     = &viewer->entities[viewer->selectedEntity];
                    clipboardType = COPY_ENTITY;
                    clipboardInfo = viewer->selectedEntity;
                }
            }
            break;

        case SceneViewerv5::TOOL_PARALLAX: break;
    }
    return false;
}
bool SceneEditor::handleKeyRelease(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Control)
        ctrlDownL = false;
    if (event->key() == Qt::Key_Alt)
        altDownL = false;
    if (event->key() == Qt::Key_Shift)
        shiftDownL = false;
    return false;
}

void SceneEditor::createEntityList()
{
    ui->entityList->blockSignals(true);
    ui->entityList->clear();

    // C++11 absolute poggers
    std::sort(viewer->entities.begin(), viewer->entities.end(),
              [](const SceneEntity &a, const SceneEntity &b) -> bool { return a.slotID < b.slotID; });
    for (int i = 0; i < viewer->entities.count(); ++i) {
        QString name = viewer->objects[viewer->entities[i].type].name;
        ui->entityList->addItem(QString::number(viewer->entities[i].slotID) + ": " + name);
    }
    ui->entityList->blockSignals(false);

    for (int i = 0; i < ENTITY_COUNT; ++i) {
        memset(&compilerv2->objectEntityList[i], 0, sizeof(compilerv2->objectEntityList[i]));
        memset(&compilerv3->objectEntityList[i], 0, sizeof(compilerv3->objectEntityList[i]));
        memset(&compilerv4->objectEntityList[i], 0, sizeof(compilerv4->objectEntityList[i]));

        if (i < viewer->entities.count()) {
            compilerv2->objectEntityList[i].type          = viewer->entities[i].type;
            compilerv2->objectEntityList[i].propertyValue = viewer->entities[i].propertyValue;
            compilerv2->objectEntityList[i].XPos          = viewer->entities[i].pos.x * 65536;
            compilerv2->objectEntityList[i].YPos          = viewer->entities[i].pos.y * 65536;

            compilerv3->objectEntityList[i].type          = viewer->entities[i].type;
            compilerv3->objectEntityList[i].propertyValue = viewer->entities[i].propertyValue;
            compilerv3->objectEntityList[i].XPos          = viewer->entities[i].pos.x * 65536;
            compilerv3->objectEntityList[i].YPos          = viewer->entities[i].pos.y * 65536;

            compilerv4->objectEntityList[i].type          = viewer->entities[i].type;
            compilerv4->objectEntityList[i].propertyValue = viewer->entities[i].propertyValue;
            compilerv4->objectEntityList[i].XPos          = viewer->entities[i].pos.x * 65536;
            compilerv4->objectEntityList[i].YPos          = viewer->entities[i].pos.y * 65536;

            int *values[] = { &compilerv4->objectEntityList[i].state,
                              NULL,
                              &compilerv4->objectEntityList[i].scale,
                              &compilerv4->objectEntityList[i].rotation,
                              NULL,
                              NULL,
                              &compilerv4->objectEntityList[i].alpha,
                              NULL,
                              &compilerv4->objectEntityList[i].animationSpeed,
                              NULL,
                              NULL,
                              &compilerv4->objectEntityList[i].values[0],
                              &compilerv4->objectEntityList[i].values[1],
                              &compilerv4->objectEntityList[i].values[2],
                              &compilerv4->objectEntityList[i].values[3] };

            byte *valuesB[] = { NULL,
                                &compilerv4->objectEntityList[i].direction,
                                NULL,
                                NULL,
                                (byte *)&compilerv4->objectEntityList[i].drawOrder,
                                &compilerv4->objectEntityList[i].priority,
                                NULL,
                                &compilerv4->objectEntityList[i].animation,
                                NULL,
                                &compilerv4->objectEntityList[i].frame,
                                &compilerv4->objectEntityList[i].inkEffect,
                                NULL,
                                NULL,
                                NULL,
                                NULL };

            for (int v = 0; v < 0x0F; ++v) {
                if (viewer->entities[i].sceneVariables[v].active) {
                    if (values[v])
                        *values[v] = viewer->entities[i].sceneVariables[v].value;
                    else
                        *valuesB[v] = viewer->entities[i].sceneVariables[v].value;
                }
            }
        }
    }
}
void SceneEditor::createScrollList()
{
    ui->scrollList->clear();

    if (viewer->selectedLayer < 1 || viewer->selectedLayer >= 9)
        return;

    ui->scrollList->blockSignals(true);
    for (int i = 0; i < viewer->layers[viewer->selectedLayer].scrollInfos.count(); ++i) {
        auto &info = viewer->layers[viewer->selectedLayer].scrollInfos[i];
        ui->scrollList->addItem(QString("Start: %1, Length %2").arg(info.startLine).arg(info.length));
    }

    ui->scrollList->blockSignals(false);
    ui->scrollList->setCurrentRow(-1);
}

bool SceneEditor::callGameEvent(byte eventID, int id)
{
    bool called = false;
    switch (viewer->gameType) {
        case ENGINE_v3:
            switch (eventID) {
                case SceneViewerv5::EVENT_LOAD: {
                    auto &curObj           = compilerv3->objectScriptList[id];
                    curObj.frameListOffset = compilerv3->scriptFrameCount;
                    curObj.spriteSheetID   = 0;
                    compilerv3->objectEntityList[ENTITY_COUNT - 1].type = id;

                    auto &curSub = curObj.subRSDKLoad;
                    if (curSub.scriptCodePtr != SCRIPTDATA_COUNT - 1) {
                        compilerv3->processScript(curSub.scriptCodePtr, curSub.jumpTablePtr,
                                                  Compilerv3::SUB_RSDKLOAD);
                        called = true;
                    }
                    curObj.spriteFrameCount = compilerv3->scriptFrameCount - curObj.frameListOffset;
                    break;
                }
                case SceneViewerv5::EVENT_DRAW: {
                    if (id == -1) {
                        auto &tmpObj = compilerv3->objectScriptList[viewer->selectedObject];

                        if (tmpObj.subRSDKDraw.scriptCodePtr >= 0 && viewer->selectedObject != 0) {

                            float ex = viewer->tilePos.x;
                            float ey = viewer->tilePos.y;

                            ex *= viewer->invZoom();
                            ey *= viewer->invZoom();

                            compilerv3->objectEntityList[ENTITY_COUNT - 1].type =
                                viewer->selectedObject;
                            compilerv3->objectEntityList[ENTITY_COUNT - 1].XPos =
                                (ex + viewer->cameraPos.x) * 65536.0f;
                            compilerv3->objectEntityList[ENTITY_COUNT - 1].YPos =
                                (ey + viewer->cameraPos.y) * 65536.0f;
                            compilerv3->objectLoop = ENTITY_COUNT - 1;
                            id                     = ENTITY_COUNT - 1;
                        }
                    }
                    if (id == -1)
                        return false;

                    auto &curObj = compilerv3->objectScriptList[viewer->entities[id].type];

                    if (curObj.subRSDKDraw.scriptCodePtr != SCRIPTDATA_COUNT - 1
                        && viewer->entities[id].type != 0) {
                        compilerv3->objectLoop = id;
                        compilerv3->processScript(curObj.subRSDKDraw.scriptCodePtr,
                                                  curObj.subRSDKDraw.jumpTablePtr,
                                                  Compilerv3::SUB_RSDKDRAW);
                        called = true;
                    }
                    break;
                }
                case SceneViewerv5::EVENT_EDIT: {
                    auto &curObj = compilerv3->objectScriptList[viewer->entities[id].type];

                    if (curObj.subRSDKEdit.scriptCodePtr != SCRIPTDATA_COUNT - 1
                        && viewer->entities[id].type != 0) {
                        compilerv3->objectLoop = id;
                        viewer->activeVarObj   = viewer->entities[id].type;

                        compilerv3->processScript(curObj.subRSDKEdit.scriptCodePtr,
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
                case SceneViewerv5::EVENT_LOAD: {
                    auto &curObj           = compilerv4->objectScriptList[id];
                    curObj.frameListOffset = compilerv4->scriptFrameCount;
                    curObj.spriteSheetID   = 0;
                    compilerv4->objectEntityList[ENTITY_COUNT - 1].type = id;

                    auto &curSub = curObj.eventRSDKLoad;
                    if (curSub.scriptCodePtr != SCRIPTDATA_COUNT - 1) {
                        compilerv4->processScript(curSub.scriptCodePtr, curSub.jumpTablePtr,
                                                  Compilerv4::EVENT_RSDKLOAD);
                        called = true;
                    }
                    curObj.spriteFrameCount = compilerv4->scriptFrameCount - curObj.frameListOffset;
                    break;
                }
                case SceneViewerv5::EVENT_DRAW: {
                    SceneEntity *entity = nullptr;
                    if (id == -1) {
                        auto &tmpObj = compilerv4->objectScriptList[viewer->selectedObject];

                        if (tmpObj.eventRSDKDraw.scriptCodePtr >= 0 && viewer->selectedObject != 0) {

                            float ex = viewer->tilePos.x;
                            float ey = viewer->tilePos.y;

                            ex *= viewer->invZoom();
                            ey *= viewer->invZoom();

                            compilerv4->objectEntityList[ENTITY_COUNT - 1].type =
                                viewer->selectedObject;
                            compilerv4->objectEntityList[ENTITY_COUNT - 1].XPos =
                                (ex + viewer->cameraPos.x) * 65536.0f;
                            compilerv4->objectEntityList[ENTITY_COUNT - 1].YPos =
                                (ey + viewer->cameraPos.y) * 65536.0f;
                            compilerv4->objectEntityPos = ENTITY_COUNT - 1;
                            id                          = ENTITY_COUNT - 1;

                            createTempEntity.type   = viewer->selectedObject;
                            createTempEntity.pos.x  = (ex + viewer->cameraPos.x) * 65536.0f;
                            createTempEntity.pos.y  = (ey + viewer->cameraPos.y) * 65536.0f;
                            createTempEntity.slotID = 0xFFFF;
                            createTempEntity.box    = Rect<int>(0, 0, 0, 0);
                            entity = viewer->activeDrawEntity = &createTempEntity;
                        }
                    }
                    else {
                        entity = &viewer->entities[id];
                    }

                    if (id == -1)
                        return false;

                    auto &curObj = compilerv4->objectScriptList[entity->type];

                    if (curObj.eventRSDKDraw.scriptCodePtr != SCRIPTDATA_COUNT - 1
                        && entity->type != 0) {
                        compilerv4->objectEntityPos = id;
                        compilerv4->processScript(curObj.eventRSDKDraw.scriptCodePtr,
                                                  curObj.eventRSDKDraw.jumpTablePtr,
                                                  Compilerv4::EVENT_RSDKDRAW);
                        called = true;
                    }
                    break;
                }
                case SceneViewerv5::EVENT_EDIT: {
                    auto &curObj = compilerv4->objectScriptList[viewer->entities[id].type];

                    if (curObj.eventRSDKEdit.scriptCodePtr != SCRIPTDATA_COUNT - 1
                        && viewer->entities[id].type != 0) {
                        compilerv4->objectEntityPos = id;
                        viewer->activeVarObj        = viewer->entities[id].type;

                        compilerv4->processScript(curObj.eventRSDKEdit.scriptCodePtr,
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

void SceneEditor::parseGameXML(byte gameType, QString path)
{
    gameConfig          = FormatHelpers::GameConfig();
    gameConfig.filePath = path;

    Reader fileReader = Reader(path);
    QByteArray bytes  = fileReader.readByteArray(fileReader.filesize, false);

    QXmlStreamReader xmlReader = QXmlStreamReader(bytes);

    int list        = -1;
    bool objectFlag = false;
    // Parse the XML until we reach end of it
    while (!xmlReader.atEnd() && !xmlReader.hasError()) {
        // Read next element
        QXmlStreamReader::TokenType token = xmlReader.readNext();
        // If token is just StartDocument - go to next
        if (token == QXmlStreamReader::StartDocument) {
            continue;
        }
        // If token is StartElement - read it
        if (token == QXmlStreamReader::StartElement) {
            QList<QString> listNames = { "presentationStages", "regularStages", "bonusStages",
                                         "specialStages" };

            const QStringRef name = xmlReader.name();
            if (name == "objects") {
                objectFlag ^= 1;
            }
            else if (objectFlag && name == "object") {
                QString objName   = "";
                QString objScript = "";
                for (const QXmlStreamAttribute &attr : xmlReader.attributes()) {
                    if (attr.name().toString() == QLatin1String("name")) {
                        objName = attr.value().toString();
                    }
                    if (attr.name().toString() == QLatin1String("script")) {
                        objScript = attr.value().toString();
                    }
                }
                FormatHelpers::GameConfig::ObjectInfo obj;
                obj.name   = objName;
                obj.script = objScript;
                gameConfig.objects.append(obj);
            }
            else if (listNames.indexOf(name.toString()) != -1) {
                if (list == listNames.indexOf(name.toString()))
                    list = -1;
                else
                    list = listNames.indexOf(name.toString());
            }
            else if (list != -1 && name == "stage") {
                QString stgName   = "";
                QString stgFolder = "";
                QString stgID     = "";
                int stgHighlight  = 0;
                for (const QXmlStreamAttribute &attr : xmlReader.attributes()) {
                    if (attr.name().toString() == QLatin1String("name")) {
                        stgName = attr.value().toString();
                    }
                    if (attr.name().toString() == QLatin1String("folder")) {
                        stgFolder = attr.value().toString();
                    }
                    if (attr.name().toString() == QLatin1String("id")) {
                        stgID = attr.value().toString();
                    }
                    if (attr.name().toString() == QLatin1String("highlight")) {
                        stgHighlight = attr.value().toInt();
                    }
                }
                FormatHelpers::GameConfig::SceneInfo stage;
                stage.m_name      = stgName;
                stage.folder      = stgFolder;
                stage.id          = stgID;
                stage.highlighted = stgHighlight;
                gameConfig.stageLists[list].scenes.append(stage);
            }
        }
    }

    if (xmlReader.hasError()) {
        QMessageBox::critical(this, "Game.xml Parse Error", xmlReader.errorString(), QMessageBox::Ok);
        return;
    }

    // close reader and flush file
    xmlReader.clear();
}

void SceneEditor::undoAction()
{
    if (actionIndex > 0) {
        actionIndex--;
        resetAction();
    }
}
void SceneEditor::redoAction()
{
    if (actionIndex + 1 < actions.count()) {
        actionIndex++;
        resetAction();
    }
}
void SceneEditor::resetAction()
{
    viewer->tilePalette = actions[actionIndex].tilePalette;
    viewer->tiles       = actions[actionIndex].tiles;
    viewer->chunks      = actions[actionIndex].chunks;

    gameConfig = actions[actionIndex].gameConfig;

    scene            = actions[actionIndex].scene;
    background       = actions[actionIndex].background;
    viewer->chunkset = actions[actionIndex].chunkset;
    stageConfig      = actions[actionIndex].stageConfig;

    viewer->tileconfig = actions[actionIndex].tileconfig;

    viewer->objects  = actions[actionIndex].objects;
    viewer->entities = actions[actionIndex].entities;

    // General Editing
    // viewer->curTool   = actions[actionIndex].curTool;
    // viewer->isSelecting = actions[actionIndex].selecting;
    // viewer->mousePos  = actions[actionIndex].mousePos;

    // Layer Editing
    viewer->tilePos       = actions[actionIndex].tilePos;
    viewer->tileFlip      = actions[actionIndex].tileFlip;
    viewer->selectedTile  = actions[actionIndex].selectedTile;
    viewer->selectedLayer = actions[actionIndex].selectedLayer;

    // Collision
    // viewer->showPlaneA = actions[actionIndex].showPlaneA;
    // viewer->showPlaneB = actions[actionIndex].showPlaneB;

    // Entity Editing
    viewer->selectedObject = actions[actionIndex].selectedObject; // placing
    viewer->selectedEntity = actions[actionIndex].selectedEntity; // viewing

    // Parallax Editing
    // viewer->showParallax       = actions[actionIndex].showParallax;
    viewer->selectedScrollInfo = actions[actionIndex].selectedScrollInfo;

    // Camera
    viewer->cameraPos = actions[actionIndex].camPos;

    // viewer->showGrid = actions[actionIndex].showTileGrid;

    // viewer->compilerv2 = actions[actionIndex].compilerv2;
    // viewer->compilerv3 = actions[actionIndex].compilerv3;
    // viewer->compilerv4 = actions[actionIndex].compilerv4;

    // updating UI
    // ui->showTileGrid->blockSignals(true);
    // ui->showTileGrid->setDown(viewer->showGrid);
    // ui->showTileGrid->blockSignals(false);
    //
    // ui->showCollisionA->blockSignals(true);
    // ui->showCollisionA->setDown(viewer->showPlaneA);
    // ui->showCollisionA->blockSignals(false);
    //
    // ui->showCollisionB->blockSignals(true);
    // ui->showCollisionB->setDown(viewer->showPlaneB);
    // ui->showCollisionB->blockSignals(false);
    //
    // ui->showParallax->blockSignals(true);
    // ui->showParallax->setDown(viewer->showParallax);
    // ui->showParallax->blockSignals(false);
    //
    // ui->selToolBox->blockSignals(true);
    // ui->selToolBox->setCurrentIndex(viewer->curTool);
    // ui->selToolBox->blockSignals(false);

    updateTitle(actionIndex > 0);
}
void SceneEditor::doAction(QString name, bool setModified)
{
    ActionState action;

    action.name = name;

    action.tilePalette = viewer->tilePalette;
    action.tiles       = viewer->tiles;
    action.chunks      = viewer->chunks;

    action.gameConfig = gameConfig;

    action.scene       = scene;
    action.background  = background;
    action.chunkset    = viewer->chunkset;
    action.stageConfig = stageConfig;

    action.tileconfig = viewer->tileconfig;

    action.objects  = viewer->objects;
    action.entities = viewer->entities;

    // General Editing
    // action.curTool   = viewer->curTool;
    // action.selecting = viewer->isSelecting;
    // action.mousePos  = viewer->mousePos;

    // Layer Editing
    action.tilePos       = viewer->tilePos;
    action.tileFlip      = viewer->tileFlip;
    action.selectedTile  = viewer->selectedTile;
    action.selectedLayer = viewer->selectedLayer;

    // Collision
    // action.showPlaneA = viewer->showPlaneA;
    // action.showPlaneB = viewer->showPlaneB;

    // Entity Editing
    action.selectedObject = viewer->selectedObject; // placing
    action.selectedEntity = viewer->selectedEntity; // viewing

    // Parallax Editing
    // action.showParallax       = viewer->showParallax;
    action.selectedScrollInfo = viewer->selectedScrollInfo;

    // Camera
    action.camPos = viewer->cameraPos;

    // action.showTileGrid = viewer->showGrid;

    // action.compilerv2 = viewer->compilerv2;
    // action.compilerv3 = viewer->compilerv3;
    // action.compilerv4 = viewer->compilerv4;

    // Actions
    for (int i = actions.count() - 1; i > actionIndex; --i) {
        actions.removeAt(i);
    }

    actions.append(action);
    actionIndex = actions.count() - 1;

    updateTitle(setModified);

    // setStatus("Did Action: " + name);
}
void SceneEditor::clearActions()
{
    actions.clear();
    actionIndex = 0;
    doAction("Action Setup", false); // first action, cant be undone
}

// XML Management
void SceneEditor::readXMLScrollInfo(QXmlStreamReader &xmlReader, int layerID, byte mode)
{
    if (layerID <= 0 || layerID >= 9)
        return;

    auto &layer = viewer->layers[layerID];
    layer.scrollInfos.clear();
    // Parse the XML until we reach end of it
    while (!xmlReader.atEnd() && !xmlReader.hasError() && mode < 2) {
        // Read next element
        QXmlStreamReader::TokenType token = xmlReader.readNext();
        // If token is just StartDocument - go to next
        if (token == QXmlStreamReader::StartDocument)
            continue;

        // If token is StartElement - read it
        if (token == QXmlStreamReader::StartElement) {
            const QStringRef name = xmlReader.name();
            if (name == "scrollingInfo")
                mode++;
            else if (name == "layers")
                mode = 2;
            else if (mode && name == "scrollInfo") {
                int startLine        = 0;
                int length           = 0;
                float parallaxFactor = 1.0f;
                float scrollSpeed    = 0.0f;
                bool deform          = false;
                for (const QXmlStreamAttribute &attr : xmlReader.attributes()) {
                    if (attr.name().toString() == QLatin1String("startLine"))
                        startLine = attr.value().toInt();
                    if (attr.name().toString() == QLatin1String("length"))
                        length = attr.value().toInt();
                    if (attr.name().toString() == QLatin1String("parallaxFactor"))
                        parallaxFactor = attr.value().toFloat();
                    if (attr.name().toString() == QLatin1String("scrollSpeed"))
                        scrollSpeed = attr.value().toFloat();
                    if (attr.name().toString() == QLatin1String("deform"))
                        deform = attr.value().toString() != "false" && attr.value().toString() != "0";
                }
                SceneHelpers::TileLayer::ScrollIndexInfo info;
                info.startLine      = startLine;
                info.length         = length;
                info.parallaxFactor = parallaxFactor;
                info.scrollSpeed    = scrollSpeed;
                info.deform         = deform;
                layer.scrollInfos.append(info);
            }
        }
    }
}

void SceneEditor::readXMLLayout(QXmlStreamReader &xmlReader, int layerID, byte mode)
{
    if (layerID < 0 || layerID >= 9)
        return;

    // Parse the XML until we reach end of it
    while (!xmlReader.atEnd() && !xmlReader.hasError() && mode < 2) {
        // Read next element
        QXmlStreamReader::TokenType token = xmlReader.readNext();
        // If token is just StartDocument - go to next
        if (token == QXmlStreamReader::StartDocument)
            continue;

        // If token is StartElement - read it
        if (token == QXmlStreamReader::StartElement) {
            const QStringRef name = xmlReader.name();
            if (name == "layout" || mode == 1) {
                QString text = xmlReader.readElementText()
                                   .replace('\n', "")
                                   .replace('\r', "")
                                   .replace('\t', "")
                                   .replace(' ', "");
                QStringList layout = text.split(",");

                int id = 0;

                auto &layer = viewer->layers[layerID];
                for (int y = 0; y < layer.height; ++y) {
                    for (int x = 0; x < layer.width; ++x) {
                        bool ok            = false;
                        layer.layout[y][x] = 0;
                        if (id < layout.count()) {
                            int index = layout[id].toInt(&ok);
                            if (ok)
                                layer.layout[y][x] = index;
                        }

                        ++id;
                    }
                }

                mode += 2;
            }
            else if (name == "layers")
                mode = 2;
        }
    }
}

void SceneEditor::readXMLLayers(QXmlStreamReader &xmlReader)
{
    QList<QString> layerTypes = { "None", "HScroll", "VScroll", "3D Sky", "3D Floor" };

    byte mode   = 0;
    int id      = 0;
    int layerID = 0;
    // Parse the XML until we reach end of it
    while (!xmlReader.atEnd() && !xmlReader.hasError() && mode < 2) {
        // Read next element
        QXmlStreamReader::TokenType token = xmlReader.readNext();
        // If token is just StartDocument - go to next
        if (token == QXmlStreamReader::StartDocument)
            continue;

        // If token is StartElement - read it
        if (token == QXmlStreamReader::StartElement) {
            const QStringRef name = xmlReader.name();
            if (name == "layers") {
                mode++;
            }
            else if (mode) {
                if (name == "layer") {
                    QString name = "", type = "";
                    int width = 0, height = 0, drawOrder = 0;
                    float parallaxFactor = 1.0f, scrollSpeed = 0.0f;
                    bool visible = false;
                    for (const QXmlStreamAttribute &attr : xmlReader.attributes()) {
                        if (attr.name().toString() == QLatin1String("name"))
                            name = attr.value().toString();
                        if (attr.name().toString() == QLatin1String("type"))
                            type = attr.value().toString();
                        if (attr.name().toString() == QLatin1String("drawOrder"))
                            drawOrder = attr.value().toInt();
                        if (attr.name().toString() == QLatin1String("width"))
                            width = attr.value().toInt();
                        if (attr.name().toString() == QLatin1String("height"))
                            height = attr.value().toInt();
                        if (attr.name().toString() == QLatin1String("parallaxFactor"))
                            parallaxFactor = attr.value().toFloat();
                        if (attr.name().toString() == QLatin1String("scrollSpeed"))
                            scrollSpeed = attr.value().toFloat();
                        if (attr.name().toString() == QLatin1String("visible"))
                            visible =
                                attr.value().toString() != "false" && attr.value().toString() != "0";
                    }

                    layerID = id + 1;

                    if (name == "Foreground") {
                        viewer->layers[0].width   = width;
                        viewer->layers[0].height  = height;
                        viewer->layers[0].visible = visible;
                        layerID                   = 0;
                    }
                    else if (id < 8) {
                        int layerType = layerTypes.indexOf(type) == -1 ? 1 : layerTypes.indexOf(type);

                        viewer->layers[id].type           = layerType;
                        viewer->layers[id].width          = width;
                        viewer->layers[id].height         = height;
                        viewer->layers[id].parallaxFactor = parallaxFactor;
                        viewer->layers[id].scrollSpeed    = scrollSpeed;
                        viewer->layers[id].visible        = visible;
                    }

                    id++;
                }
                else if (name == "scrollingInfo")
                    readXMLScrollInfo(xmlReader, layerID, 1);
                else if (name == "layout")
                    readXMLLayout(xmlReader, layerID, 1);
            }
        }
    }
}

void SceneEditor::writeXMLScrollInfo(Writer &writer, int layerID, int indentPos)
{
    auto &layer = viewer->layers[layerID];

    if (layer.scrollInfos.count()) {
        writeXMLIndentation(writer, indentPos++);
        writer.writeLine("<scrollingInfo>");
        for (auto &scroll : layer.scrollInfos) {
            writeXMLIndentation(writer, indentPos);
            writer.writeLine(QString("<scrollInfo startLine=\"%1\" length=\"%2\" "
                                     "parallaxFactor=\"%3\" scrollSpeed=\"%4\" "
                                     "deform=\"%5\"> </scrollInfo>")
                                 .arg(scroll.startLine)
                                 .arg(scroll.length)
                                 .arg(scroll.parallaxFactor)
                                 .arg(scroll.scrollSpeed)
                                 .arg(scroll.deform ? "true" : "false"));
        }
        writeXMLIndentation(writer, --indentPos);
        writer.writeLine("</scrollingInfo>");
        writer.writeLine();
    }
}

void SceneEditor::writeXMLLayout(Writer &writer, int layerID, int indentPos)
{
    writeXMLIndentation(writer, indentPos++);
    writer.writeLine("<layout>");

    auto &layer = viewer->layers[layerID];
    for (int y = 0; y < layer.height; ++y) {
        writeXMLIndentation(writer, indentPos);
        for (int x = 0; x < layer.width; ++x) {
            writer.writeText(QString::number(layer.layout[y][x]));
            writer.writeText(",");
        }
        writer.writeLine();
    }

    writeXMLIndentation(writer, --indentPos);
    writer.writeLine("</layout>");
    writer.writeLine();
}

void SceneEditor::writeXMLLayer(Writer &writer, int layerID, int indentPos)
{
    QList<QString> layerTypes = { "None", "HScroll", "VScroll", "3D Sky", "3D Floor" };

    auto &layer = viewer->layers[layerID];

    int drawOrder = 0;

    writeXMLIndentation(writer, indentPos);
    writer.writeLine(QString("<layer name=\"%1\" type=\"%2\" drawOrder=\"%3\" width=\"%4\" "
                             "height=\"%5\" parallaxFactor=\"%6\" scrollSpeed=\"%7\" "
                             "visible=\"%8\">")
                         .arg("Background " + QString::number(layerID))
                         .arg(layerTypes[layer.type])
                         .arg(drawOrder)
                         .arg(layer.width)
                         .arg(layer.height)
                         .arg(layer.parallaxFactor)
                         .arg(layer.scrollSpeed)
                         .arg(viewer->layers[layerID].visible ? "true" : "false"));

    writeXMLScrollInfo(writer, layerID, indentPos + 1);

    writeXMLLayout(writer, layerID, indentPos + 1);

    writeXMLIndentation(writer, indentPos);
    writer.writeLine("</layer>");
    writer.writeLine();
}

void SceneEditor::writeXMLObject(Writer &writer, int objID, int indentPos)
{
    auto &object = viewer->objects[objID];

    writeXMLIndentation(writer, indentPos);
    writer.writeLine(QString("<object name=\"%1\">").arg(object.name));

    writeXMLIndentation(writer, indentPos + 1);
    writer.writeLine(QString("<variable name=\"%1\" type=\"%2\"> </variable>")
                         .arg(viewer->objects[objID].variablesAliases[VAR_ALIAS_PROPVAL])
                         .arg("uint8"));

    if (viewer->gameType == ENGINE_v4) {
        bool activeVars[0xF];
        memset(activeVars, 0, sizeof(activeVars));

        if (viewer->gameType == ENGINE_v4) {
            for (auto &entity : viewer->entities) {
                for (int v = 0; v < 0xF; ++v) {
                    if (!entity.sceneVariables[v].active)
                        continue;

                    activeVars[v] = true;
                }
            }

            for (int v = 0; v < 0xF; ++v) {
                if (!activeVars[v])
                    continue;

                QString name = RSDKv4::objectVariableNames[v];
                name.insert(0, name.at(0).toLower());
                name.remove(1, 1);
                if (v >= 11) {
                    name = viewer->objects[objID].variablesAliases[VAR_ALIAS_VAL0 + (v - 11)];
                }

                writeXMLIndentation(writer, indentPos + 1);
                writer.writeLine(QString("<variable name=\"%1\" type=\"%2\"> </variable>")
                                     .arg(name)
                                     .arg(RSDKv4::objectVariableTypes[v]));
            }
        }
    }

    if (object.variables.count()) {
        for (auto &variable : object.variables) {
            writeXMLIndentation(writer, indentPos + 1);
            writer.writeLine(QString("<variable name=\"%1\" type=\"%2\"> </variable>")
                                 .arg(variable.name)
                                 .arg("int32"));
        }
    }
    writeXMLIndentation(writer, indentPos);
    writer.writeLine("</object>");
}

void SceneEditor::writeXMLEntity(Writer &writer, int entityID, int indentPos)
{

    QList<QString> types = { "uint8", "uint16", "uint32", "int8",    "int16",   "int32",
                             "enum",  "bool",   "string", "vector2", "unknown", "colour" };

    auto &entity = viewer->entities[entityID];

    writeXMLIndentation(writer, indentPos);
    writer.writeLine(QString("<entity name=\"%1\" slotID=\"%2\" x=\"%3\" y=\"%4\">")
                         .arg(viewer->objects[entity.type].name)
                         .arg(entity.slotID)
                         .arg(entity.pos.x)
                         .arg(entity.pos.y));

    writeXMLIndentation(writer, indentPos + 1);
    writer.writeLine(QString("<variable name=\"%1\" type=\"uint8\">%2</variable>")
                         .arg(viewer->objects[entity.type].variablesAliases[VAR_ALIAS_PROPVAL])
                         .arg(entity.propertyValue));

    if (viewer->gameType == ENGINE_v4) {
        for (int v = 0; v < 0xF; ++v) {
            if (!entity.sceneVariables[v].active)
                continue;

            QString name = RSDKv4::objectVariableNames[v];
            name.insert(0, name.at(0).toLower());
            name.remove(1, 1);
            if (v >= 11) {
                name = viewer->objects[entity.type].variablesAliases[VAR_ALIAS_VAL0 + (v - 11)];
            }

            writeXMLIndentation(writer, indentPos + 1);
            writer.writeLine(QString("<variable name=\"%1\" type=\"%2\">%3</variable>")
                                 .arg(name)
                                 .arg(RSDKv4::objectVariableTypes[v])
                                 .arg(entity.sceneVariables[v].value));
        }
    }

    for (int v = 0; v < viewer->objects[entity.type].variables.count(); ++v) {
        int value = objProp->callRSDKEdit(this, true, entityID, v, 0);

        writeXMLIndentation(writer, indentPos + 1);
        writer.writeText(QString("<variable name=\"%1\" type=\"%2\">")
                             .arg(viewer->objects[entity.type].variables[v].name)
                             .arg("enum"));

        if (!viewer->objects[entity.type].variables[v].values.count()) {
            writer.writeText(QString::number(value));
        }
        else {
            QString valStr = "UNKNOWN";
            for (auto &val : viewer->objects[entity.type].variables[v].values) {
                if (val.value == value) {
                    valStr = val.name;
                    break;
                }
            }
            writer.writeText(valStr);
        }
        writer.writeLine(QString("</variable>"));
    }
    writeXMLIndentation(writer, indentPos);
    writer.writeLine("</entity>");
}

void SceneEditor::writeXMLChunk(Writer &writer, int chunkID, int indentPos)
{
    QList<QString> directions = { "No Flip", "Flip X", "Flip Y", "Flip XY" };
    QList<QString> solidities = { "All Solid", "Solid Top", "Solid LRB", "Not Solid",
                                  "Solid Top (No Grip)" };

    auto &chunk = viewer->chunkset.chunks[chunkID];

    writeXMLIndentation(writer, indentPos);
    writer.writeLine(QString("\t\t<chunk>"));
    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            FormatHelpers::Chunks::Tile &tile = chunk.tiles[y][x];
            writeXMLIndentation(writer, indentPos + 1);
            writer.writeLine(QString("\t\t\t<tile index=\"%1\" direction=\"%2\" visualPlane=\"%3\" "
                                     "solidityA=\"%4\" solidityB=\"%5\"> </tile>")
                                 .arg(tile.tileIndex)
                                 .arg(directions[tile.direction])
                                 .arg(tile.visualPlane ? "High" : "Low")
                                 .arg(solidities[tile.solidityA])
                                 .arg(solidities[tile.solidityB]));
        }
    }

    writeXMLIndentation(writer, indentPos);
    writer.writeLine("\t\t</chunk>");
    writer.writeLine();
}

void SceneEditor::writeXMLScene(Writer &writer)
{
    int indentPos = 0;

    writer.writeLine("<scene>");

    QList<QString> types = { "uint8", "uint16", "uint32", "int8",    "int16",   "int32",
                             "enum",  "bool",   "string", "vector2", "unknown", "colour" };

    indentPos++;
    {
        uint bgClr1 = (viewer->metadata.backgroundColor1.red() << 16)
                      | (viewer->metadata.backgroundColor1.green() << 8)
                      | (viewer->metadata.backgroundColor1.blue() << 0) | (0xFF << 24);
        uint bgClr2 = (viewer->metadata.backgroundColor2.red() << 16)
                      | (viewer->metadata.backgroundColor2.green() << 8)
                      | (viewer->metadata.backgroundColor2.blue() << 0) | (0xFF << 24);
        writer.writeLine(
            QString("\t<metadata bgColour=\"%1\" "
                    "altBgColour=\"%2\" title=\"%3\" layerMidpoint=\"%4\" activeLayer0=\"%5\" "
                    "activeLayer1=\"%6\" activeLayer2=\"%7\" activeLayer3=\"%8\" musicID=\"%9\" "
                    "backgroundID=\"%10\" playerX=\"%11\" playerY=\"%12\"> </metadata>")
                .arg(bgClr1)
                .arg(bgClr2)
                .arg(scene.title)
                .arg(scene.midpoint)
                .arg(scene.activeLayer[0])
                .arg(scene.activeLayer[1])
                .arg(scene.activeLayer[2])
                .arg(scene.activeLayer[3])
                .arg(scene.musicID)
                .arg(scene.backgroundID)
                .arg(scene.playerX)
                .arg(scene.playerY));
    }
    indentPos--;

    indentPos++;
    {
        writer.writeLine();
        writeXMLIndentation(writer, indentPos);
        writer.writeLine("<layers>");

        int id = 0;
        for (auto &layer : viewer->layers) {
            ++id;
            if (layer.width == 0 || layer.height == 0)
                continue;

            writeXMLLayer(writer, id, indentPos + 1);
        }

        writeXMLIndentation(writer, indentPos);
        writer.writeLine("<layers>");
    }
    indentPos--;

    indentPos++;
    if (viewer->objects.count()) {
        writer.writeLine();
        writeXMLIndentation(writer, indentPos);
        writer.writeLine("<objects>");

        for (int o = 0; o < viewer->objects.count(); ++o) writeXMLObject(writer, o, indentPos + 1);

        writeXMLIndentation(writer, indentPos);
        writer.writeLine("</objects>");
        writer.writeLine();
    }
    indentPos--;

    indentPos++;
    if (viewer->objects.count() && viewer->entities.count()) {
        writer.writeLine();
        writeXMLIndentation(writer, indentPos);
        writer.writeLine("<entities>");

        for (int e = 0; e < viewer->entities.count(); ++e) writeXMLEntity(writer, e, indentPos + 1);

        writeXMLIndentation(writer, indentPos);
        writer.writeLine("</entities>");
        writer.writeLine();
    }
    indentPos--;

    indentPos++;
    {
        writer.writeLine();
        writeXMLIndentation(writer, indentPos);
        writer.writeLine("<chunks>");

        for (int c = 0; c < (viewer->gameType == ENGINE_v1 ? 0x100 : 0x200); ++c)
            writeXMLChunk(writer, c, indentPos + 1);

        writeXMLIndentation(writer, indentPos);
        writer.writeLine("</chunks>");
        writer.writeLine();
    }
    indentPos--;

    writer.writeLine("</scene>");
}

ushort SceneEditor::loadSpriteSheet(QString filename)
{
    QString buffer = WorkingDirManager::GetPath(QString("Sprites/") + filename,
                                                viewer->dataPath + "/Sprites/" + filename);

    uint hash[4];
    Utils::getHashInt(buffer, hash);

    for (int i = 0; i < v5_SURFACE_MAX; ++i) {
        if (viewer->gfxSurface[i].scope != SCOPE_NONE
            && memcmp(viewer->gfxSurface[i].hash, hash, 0x10 * sizeof(byte)) == 0) {
            return i;
        }
    }

    ushort id = -1;
    for (id = 0; id < v5_SURFACE_MAX; ++id) {
        if (viewer->gfxSurface[id].scope == SCOPE_NONE)
            break;
    }

    if (id >= v5_SURFACE_MAX)
        return -1;

    return viewer->addGraphicsFile(buffer, id, SCOPE_STAGE);
}

void SceneEditor::drawSpriteFlipped(float XPos, float YPos, float width, float height, float sprX,
                                    float sprY, int direction, InkEffects inkEffect, int alpha,
                                    int sheetID, bool screenRelative)
{
    Vector2<float> pos = { XPos, YPos };
    if (!screenRelative) {
        pos.x -= scnEditor->viewer->cameraPos.x;
        pos.y -= scnEditor->viewer->cameraPos.y;
    }

    viewer->drawSpriteFlipped(pos.x, pos.y, width, height, sprX, sprY, direction, inkEffect, alpha,
                              sheetID);
}

void SceneEditor::drawSpriteRotozoom(float XPos, float YPos, float pivotX, float pivotY, float width,
                                     float height, float sprX, float sprY, int scaleX, int scaleY,
                                     int direction, short rotation, InkEffects inkEffect, int alpha,
                                     int sheetID, bool screenRelative)
{
    Vector2<float> pos = { XPos, YPos };
    if (!screenRelative) {
        pos.x -= scnEditor->viewer->cameraPos.x;
        pos.y -= scnEditor->viewer->cameraPos.y;
    }

    viewer->drawSpriteRotozoom(pos.x, pos.y, pivotX, pivotY, width, height, sprX, sprY, scaleX, scaleY,
                               direction, rotation, inkEffect, alpha, sheetID);
}

#include "moc_sceneeditor.cpp"
