#include "includes.hpp"
#include "ui_sceneeditor.h"

// RSDKv4 SOLIDITY TYPES
// 0 - ALL SOLID
// 1 - TOP SOLID (can be gripped onto)
// 2 - LRB SOLID
// 3 - NONE SOLID
// 4 - TOP SOLID (cant be gripped onto)

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
        auto *chunk = new ChunkLabel(&parentWidget->viewer->selectedChunk, i, chunkArea);
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

    viewer = new SceneViewer(this);
    viewer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->viewerFrame->layout()->addWidget(viewer);
    viewer->show();
    startUpdates();

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

    viewer->sbHorizontal = ui->horizontalScrollBar;
    viewer->sbVertical   = ui->verticalScrollBar;
    viewer->statusLabel  = ui->statusLabel;
    viewer->objProp      = objProp;

    ui->toolBox->setCurrentIndex(0);
    ui->propertiesBox->setCurrentIndex(0);

    scnProp->gridX->setValue(viewer->gridSize.x);
    scnProp->gridY->setValue(viewer->gridSize.y);

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

        lyrProp->setupUI(&viewer->scene, &viewer->background, c, viewer->gameType);
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
            viewer->visibleLayers[c] = item->checkState() == Qt::Checked;
    });

    connect(ui->objectList, &QListWidget::currentRowChanged, [this](int c) {
        // m_uo->setDisabled(c == -1);
        // m_do->setDisabled(c == -1);
        ui->rmObj->setDisabled(c == -1 || c >= viewer->objects.count());

        if (c == -1 || c >= viewer->objects.count())
            return;

        bool global = c == 0;
        if (viewer->stageConfig.loadGlobalScripts) {
            for (auto &obj : viewer->gameConfig.objects) {
                if (viewer->objects[c].name == obj.m_name) {
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
        viewer->stageConfig.objects.append(objInfo);
        SceneViewer::ObjectInfo info;
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
        if (viewer->stageConfig.loadGlobalScripts)
            globalCount = viewer->gameConfig.objects.count() + 1;
        viewer->objects.removeAt(c);
        viewer->stageConfig.objects.removeAt(c - globalCount);
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

        viewer->cam.pos.x = viewer->entities[c].pos.x - ((viewer->storedW / 2) * viewer->invZoom());
        viewer->cam.pos.y = viewer->entities[c].pos.y - ((viewer->storedH / 2) * viewer->invZoom());

        objProp->setupUI(&viewer->entities[viewer->selectedEntity], viewer->selectedEntity,
                         &viewer->compilerv2.objectEntityList[viewer->selectedEntity],
                         &viewer->compilerv3.objectEntityList[viewer->selectedEntity],
                         &viewer->compilerv4.objectEntityList[viewer->selectedEntity],
                         viewer->gameType);
        ui->propertiesBox->setCurrentWidget(ui->objPropPage);
    });

    connect(ui->addEnt, &QToolButton::clicked, [this] {
        // uint c = objectList->currentRow() + 1;
        SceneViewer::EntityInfo ent;
        ent.type = viewer->selectedObject;
        if (viewer->selectedObject < 0)
            ent.type = 0; // backup
        ent.slotID     = viewer->entities.count();
        ent.prevSlotID = ent.slotID;

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

        scrProp->setupUI(&viewer->background.layers[viewer->selectedLayer - 1].scrollInfos[c],
                         viewer->background.layers[viewer->selectedLayer - 1].scrollInfos);
        ui->propertiesBox->setCurrentWidget(ui->scrollPropPage);
    });

    connect(ui->addScr, &QToolButton::clicked, [this] {
        auto &layer = viewer->background.layers[viewer->selectedLayer - 1];

        auto &last = layer.scrollInfos.last();

        FormatHelpers::Background::ScrollIndexInfo scr = last;
        scr.startLine                                  = last.startLine + last.length;
        scr.length                                     = 1;
        layer.scrollInfos.append(scr);

        createScrollList();
        doAction();
    });

    connect(ui->rmScr, &QToolButton::clicked, [this] {
        int c = ui->scrollList->currentRow();
        int n = ui->scrollList->currentRow() == ui->scrollList->count() - 1 ? c - 1 : c;
        delete ui->scrollList->item(c);
        viewer->background.layers[viewer->selectedLayer - 1].scrollInfos.removeAt(c);
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

    auto resetTools = [this](byte tool) {
        if (tool == 0xFF)
            tool = SceneViewer::TOOL_MOUSE;
        viewer->curTool = tool;

        // Reset
        // mainView->selectedTile = -1;
        // mainView->selectedStamp = -1;
        viewer->selectedEntity = -1;
        objProp->unsetUI();
        viewer->selecting = false;

        unsetCursor();

        switch (tool) {
            default: break; // what
            case SceneViewer::TOOL_MOUSE: break;
            case SceneViewer::TOOL_SELECT: break;
            case SceneViewer::TOOL_PENCIL: viewer->selecting = true; break;
            case SceneViewer::TOOL_ERASER: viewer->selecting = true; break;
            case SceneViewer::TOOL_ENTITY: viewer->selecting = true; break;
            case SceneViewer::TOOL_PARALLAX: viewer->selecting = true; break;
        }
    };

    connect(ui->selToolBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            [resetTools, this](int v) {
                resetTools(v);
                doAction();
            });

    connect(ui->showCollisionA, &QPushButton::clicked, [this] {
        viewer->showPlaneA ^= 1;
        viewer->showPlaneB = false;
        ui->showCollisionA->setChecked(viewer->showPlaneA);
        ui->showCollisionB->setChecked(viewer->showPlaneB);
        doAction();
    });

    connect(ui->showCollisionB, &QPushButton::clicked, [this] {
        viewer->showPlaneA = false;
        viewer->showPlaneB ^= 1;
        ui->showCollisionA->setChecked(viewer->showPlaneA);
        ui->showCollisionB->setChecked(viewer->showPlaneB);
        doAction();
    });

    connect(ui->showTileGrid, &QPushButton::clicked, [this] {
        viewer->showGrid ^= 1;
        doAction();
    });

    connect(scnProp->gridX, QOverload<int>::of(&QSpinBox::valueChanged),
            [this](int v) { viewer->gridSize.x = v; });

    connect(scnProp->gridY, QOverload<int>::of(&QSpinBox::valueChanged),
            [this](int v) { viewer->gridSize.y = v; });

    connect(scnProp->loadGlobalCB, &QCheckBox::toggled, [this](bool b) {
        viewer->stageConfig.loadGlobalScripts = b;
        if (viewer->stageConfig.loadGlobalScripts) { // assume we had no globals & are now adding em
            for (int o = viewer->entities.count() - 1; o >= 0; --o) {
                if (viewer->entities[o].type >= 1)
                    viewer->entities[o].type += viewer->gameConfig.objects.count();
            }

            for (int t = viewer->gameConfig.objects.count() - 1; t >= 0; --t) {
                SceneViewer::ObjectInfo info;
                info.name = viewer->gameConfig.objects[t].m_name;
                viewer->objects.insert(0, info);
            }
        }
        else { // assume we had globals & are now removing em
            for (int o = viewer->entities.count() - 1; o >= 0; --o) {
                if (viewer->entities[o].type > viewer->gameConfig.objects.count())
                    viewer->entities[o].type -= viewer->gameConfig.objects.count();
                else if (viewer->entities[o].type >= 1)
                    viewer->entities.removeAt(o);
            }

            for (int t = viewer->gameConfig.objects.count() - 1; t >= 0; --t) {
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

    connect(ui->showParallax, &QPushButton::clicked, [this] {
        viewer->showParallax ^= 1;
        doAction();
    });

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

        int count = viewer->stageConfig.loadGlobalScripts ? viewer->gameConfig.objects.count() : 0;
        switch (viewer->gameType) {
            case ENGINE_v4: {
                StageConfigEditorv4 *edit = new StageConfigEditorv4(
                    &viewer->stageConfig, count + 1, viewer->gameConfig.soundFX.count(), this);
                edit->exec();
                break;
            }
            case ENGINE_v3: {
                StageConfigEditorv3 *edit = new StageConfigEditorv3(
                    &viewer->stageConfig, count + 1, viewer->gameConfig.soundFX.count(), this);
                edit->exec();
                break;
            }
            case ENGINE_v2: {
                StageConfigEditorv2 *edit = new StageConfigEditorv2(
                    &viewer->stageConfig, count + 2, viewer->gameConfig.soundFX.count(), this);
                edit->exec();
                break;
            }
            case ENGINE_v1: {
                StageConfigEditorv1 *edit = new StageConfigEditorv1(&viewer->stageConfig, this);
                edit->exec();
                break;
            }
        }

        QList<QString> names;
        for (FormatHelpers::StageConfig::ObjectInfo &obj : viewer->stageConfig.objects) {
            names.append(obj.name);
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
        for (; o < viewer->objects.count(); ++o) {
            int index = names.indexOf(viewer->objects[o].name);
            if (index >= 0)
                index += cnt;
            newTypes.append(index);
        }

        for (SceneViewer::EntityInfo &obj : viewer->entities) {
            if (newTypes[obj.type] >= 0)
                obj.type = newTypes[obj.type];
            else
                obj.type = 0;
        }

        viewer->objects.clear();
        SceneViewer::ObjectInfo blankInfo;
        blankInfo.name = "Blank Object";
        viewer->objects.append(blankInfo);

        if (viewer->gameType != ENGINE_v1) {
            if (viewer->stageConfig.loadGlobalScripts) {
                if (viewer->gameType == ENGINE_v2) {
                    SceneViewer::ObjectInfo info;
                    info.name = "Player";
                    viewer->objects.append(info);
                }

                for (FormatHelpers::GameConfig::ObjectInfo &obj : viewer->gameConfig.objects) {
                    SceneViewer::ObjectInfo info;
                    info.name = obj.m_name;
                    viewer->objects.append(info);
                }
            }

            for (FormatHelpers::StageConfig::ObjectInfo &obj : viewer->stageConfig.objects) {
                SceneViewer::ObjectInfo info;
                info.name = obj.name;
                viewer->objects.append(info);
            }
        }
        else {
            for (QString &obj : globals) {
                SceneViewer::ObjectInfo info;
                info.name = obj;
                viewer->objects.append(info);
            }

            for (FormatHelpers::StageConfig::ObjectInfo &obj : viewer->stageConfig.objects) {
                SceneViewer::ObjectInfo info;
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
            for (int m = 0; m < viewer->stageConfig.music.count(); ++m)
                scnProp->musBox->addItem(viewer->stageConfig.music[o]);
            scnProp->musBox->blockSignals(false);
        }
        doAction();
    });

    connect(scnProp->editTSet, &QPushButton::clicked, [this] {
        TilesetEditor *edit = new TilesetEditor(viewer->tiles, viewer->tilePalette);
        edit->setWindowTitle("Edit Tileset");
        edit->exec();

        setStatus("Rebuilding tiles...");
        viewer->tilesetTexture = nullptr;

        QImage tileset(0x10, 0x400 * 0x10, QImage::Format_Indexed8);

        QVector<QRgb> pal;
        for (PaletteColour &col : viewer->tilePalette) pal.append(col.toQColor().rgb());
        tileset.setColorTable(pal);

        uchar *pixels = tileset.bits();
        for (int i = 0; i < 0x400; ++i) {
            uchar *src = viewer->tiles[i].bits();
            for (int y = 0; y < 16; ++y) {
                for (int x = 0; x < 16; ++x) {
                    *pixels++ = *src++;
                }
            }
        }

        viewer->tilesetTexture = viewer->createTexture(tileset);

        for (int i = 0; i < 0x200; ++i) {
            for (int y = 0; y < 8; ++y) {
                for (int x = 0; x < 8; ++x) {
                    ushort tile = viewer->chunkset.chunks[i].tiles[y][x].tileIndex;
                    viewer->chunkset.chunks[i].tiles[y][x].tileIndex = edit->tileIDs.indexOf(tile);
                }
            }
        }

        RSDKv4::TileConfig configStore = viewer->tileconfig;
        for (int i = 0; i < 0x400; ++i) {
            int id                                   = edit->tileIDs.indexOf(i);
            viewer->tileconfig.collisionPaths[0][id] = configStore.collisionPaths[0][i];
            viewer->tileconfig.collisionPaths[1][id] = configStore.collisionPaths[1][i];
        }

        viewer->chunks.clear();
        for (FormatHelpers::Chunks::Chunk &c : viewer->chunkset.chunks)
            viewer->chunks.append(c.getImage(viewer->tiles));

        chkProp->refreshList();

        doAction();
        setStatus("Finished rebuilding tiles!");
    });

    connect(scnProp->editPAL, &QPushButton::clicked, [this] {
        PaletteEditor *edit =
            new PaletteEditor(viewer->stageConfig.filePath, viewer->gameType + PALTYPE_STAGECONFIGv4);
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

        // ExportRSDKv5Scene *dlg = new ExportRSDKv5Scene(viewer->scene.filepath, this);
        // if (dlg->exec() == QDialog::Accepted) {
        // exportRSDKv5(dlg);
        // }
    });

    connect(scnProp->syncGC, &QPushButton::clicked, [this] {});
    scnProp->syncGC->setDisabled(true);

    connect(scnProp->syncSC, &QPushButton::clicked, [this] {});

    connect(scnProp->reloadLink, &QPushButton::clicked, [this] {
        setStatus("Reloading Game Link... Please Wait...");
        stopUpdates();

        initGameLink();

        startUpdates();
        setStatus("Game Link Reloaded Successfully!");
    });

    connect(ui->exportSceneImg, &QPushButton::clicked, [this] {
        QFileDialog filedialog(this, tr("Save Image"), "", tr("PNG Files (*.png)"));
        filedialog.setAcceptMode(QFileDialog::AcceptSave);
        if (filedialog.exec() == QDialog::Accepted) {
            int w = 0;
            int h = 0;

            if (viewer->visibleLayers[0]) {
                if (viewer->scene.width > w)
                    w = viewer->scene.width;
                if (viewer->scene.width > h)
                    h = viewer->scene.height;
            }

            for (int i = 0; i < viewer->background.layers.count(); ++i) {
                if (viewer->visibleLayers[i + 1]) {
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
                if (viewer->visibleLayers[i + 1]) {
                    for (int y = 0; y < viewer->background.layers[i].height; ++y) {
                        for (int x = 0; x < viewer->background.layers[i].width; ++x) {
                            ushort chunk     = viewer->background.layers[i].layout[y][x];
                            QImage &chunkImg = viewer->chunks[chunk];

                            painter.drawImage(x * 0x80, y * 0x80, chunkImg);
                        }
                    }
                }
            }

            if (viewer->visibleLayers[0]) {
                for (int y = 0; y < viewer->scene.height; ++y) {
                    for (int x = 0; x < viewer->scene.width; ++x) {
                        ushort chunk     = viewer->scene.layout[y][x];
                        QImage &chunkImg = viewer->chunks[chunk];

                        painter.drawImage(x * 0x80, y * 0x80, chunkImg);
                    }
                }
            }

            if (true) {
                for (int o = 0; o < viewer->entities.count(); ++o) {
                    SceneViewer::EntityInfo &obj = viewer->entities[o];

                    if (!viewer->objects[obj.type].visible)
                        continue;

                    QString name = viewer->objects[obj.type].name;

                    // painter.drawText(obj.pos.x, obj.pos.y, name);
                    painter.drawImage(obj.pos.x, obj.pos.y, viewer->missingObj);
                }
            }

            image.save(filedialog.selectedFiles()[0]);
            setStatus("Scene exported to image sucessfully!");
        }
    });

    clearActions();
    for (QWidget *w : findChildren<QWidget *>()) {
        w->installEventFilter(this);
    }
}

SceneEditor::~SceneEditor() { delete ui; }

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
                        if (!QFile::exists(viewer->gameConfig.filePath)) {
                            return false;
                        }
                        gcPath = viewer->gameConfig.filePath;
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
            tool = SceneViewer::TOOL_MOUSE;
        viewer->curTool = tool;

        // Reset
        // m_mainView->m_selectedTile = -1;
        // m_mainView->m_selectedStamp = -1;
        ui->selToolBox->setDisabled(true);
        ui->selToolBox->setCurrentIndex(tool);
        ui->selToolBox->setDisabled(false);
        viewer->selectedEntity = -1;
        objProp->unsetUI();
        viewer->selecting = false;

        unsetCursor();

        switch (tool) {
            default: break; // what
            case SceneViewer::TOOL_MOUSE: break;
            case SceneViewer::TOOL_SELECT: break;
            case SceneViewer::TOOL_PENCIL: viewer->selecting = true; break;
            case SceneViewer::TOOL_ERASER: viewer->selecting = true; break;
            case SceneViewer::TOOL_ENTITY: viewer->selecting = true; break;
            case SceneViewer::TOOL_PARALLAX: viewer->selecting = true; break;
        }
    };

    switch (event->type()) {
        case QEvent::MouseButtonPress: {
            QMouseEvent *mEvent = static_cast<QMouseEvent *>(event);
            viewer->reference   = mEvent->pos();

            viewer->mousePos.x = viewer->cam.lastMousePos.x = mEvent->pos().x();
            viewer->mousePos.y = viewer->cam.lastMousePos.y = mEvent->pos().y();

            if ((mEvent->button() & Qt::LeftButton) == Qt::LeftButton)
                mouseDownL = true;
            if ((mEvent->button() & Qt::MiddleButton) == Qt::MiddleButton)
                mouseDownM = true;
            if ((mEvent->button() & Qt::RightButton) == Qt::RightButton)
                mouseDownR = true;

            if (mouseDownM || (mouseDownL && viewer->curTool == SceneViewer::TOOL_MOUSE))
                setCursor(Qt::ClosedHandCursor);

            if (mouseDownL) {
                switch (viewer->curTool) {
                    case SceneViewer::TOOL_MOUSE: break;
                    case SceneViewer::TOOL_SELECT: break;
                    case SceneViewer::TOOL_PENCIL: {
                        if (viewer->selectedChunk >= 0 && viewer->selecting) {
                            setTile(mEvent->pos().x(), mEvent->pos().y());
                            doAction();
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
                    case SceneViewer::TOOL_ERASER: {
                        if (viewer->selecting) {
                            viewer->selectedChunk = 0x00;
                            setTile(mEvent->pos().x(), mEvent->pos().y());
                            doAction();
                        }
                        break;
                    }
                    case SceneViewer::TOOL_ENTITY: {
                        if (!viewer->selecting || viewer->selectedObject < 0) {
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
                                    (mEvent->pos().x() * viewer->invZoom()) + viewer->cam.pos.x,
                                    (mEvent->pos().y() * viewer->invZoom()) + +viewer->cam.pos.y);

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
                                        &viewer->compilerv2.objectEntityList[viewer->selectedEntity],
                                        &viewer->compilerv3.objectEntityList[viewer->selectedEntity],
                                        &viewer->compilerv4.objectEntityList[viewer->selectedEntity],
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
                                        &viewer->compilerv2.objectEntityList[viewer->selectedEntity],
                                        &viewer->compilerv3.objectEntityList[viewer->selectedEntity],
                                        &viewer->compilerv4.objectEntityList[viewer->selectedEntity],
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
                                SceneViewer::EntityInfo entity;
                                entity.type = viewer->selectedObject;
                                int xpos = ((mEvent->pos().x() * viewer->invZoom()) + viewer->cam.pos.x)
                                           * 65536;
                                int ypos = ((mEvent->pos().y() * viewer->invZoom()) + viewer->cam.pos.y)
                                           * 65536;

                                entity.pos.x =
                                    ((mEvent->pos().x() * viewer->invZoom()) + viewer->cam.pos.x);
                                entity.pos.y =
                                    ((mEvent->pos().y() * viewer->invZoom()) + viewer->cam.pos.y);

                                int cnt           = viewer->entities.count();
                                entity.slotID     = cnt;
                                entity.prevSlotID = entity.slotID;

                                viewer->entities.append(entity);
                                viewer->compilerv2.objectEntityList[cnt].type = viewer->selectedObject;
                                viewer->compilerv2.objectEntityList[cnt].propertyValue = 0;
                                viewer->compilerv2.objectEntityList[cnt].XPos          = xpos;
                                viewer->compilerv2.objectEntityList[cnt].YPos          = ypos;

                                viewer->compilerv3.objectEntityList[cnt].type = viewer->selectedObject;
                                viewer->compilerv3.objectEntityList[cnt].propertyValue = 0;
                                viewer->compilerv3.objectEntityList[cnt].XPos          = xpos;
                                viewer->compilerv3.objectEntityList[cnt].YPos          = ypos;

                                viewer->compilerv4.objectEntityList[cnt].type = viewer->selectedObject;
                                viewer->compilerv4.objectEntityList[cnt].propertyValue = 0;
                                viewer->compilerv4.objectEntityList[cnt].XPos          = xpos;
                                viewer->compilerv4.objectEntityList[cnt].YPos          = ypos;
                                viewer->selectedEntity                                 = cnt;

                                objProp->setupUI(
                                    &viewer->entities[viewer->selectedEntity], viewer->selectedEntity,
                                    &viewer->compilerv2.objectEntityList[viewer->selectedEntity],
                                    &viewer->compilerv3.objectEntityList[viewer->selectedEntity],
                                    &viewer->compilerv4.objectEntityList[viewer->selectedEntity],
                                    viewer->gameType);
                                ui->propertiesBox->setCurrentWidget(ui->objPropPage);
                                createEntityList();
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
                    case SceneViewer::TOOL_PENCIL:
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
                    case SceneViewer::TOOL_ENTITY: {
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
                                (mEvent->pos().x() * viewer->invZoom()) + viewer->cam.pos.x,
                                (mEvent->pos().y() * viewer->invZoom()) + viewer->cam.pos.y);

                            if (box.contains(pos) && firstSel == -1) {
                                firstSel = o;
                                firstPos = pos;
                            }

                            if (box.contains(pos) && viewer->selectedEntity < o) {
                                viewer->selectedEntity = o;
                                selectionOffset.x      = pos.x - viewer->entities[o].pos.x;
                                selectionOffset.y      = pos.y - viewer->entities[o].pos.y;
                                found                  = true;

                                objProp->setupUI(
                                    &viewer->entities[viewer->selectedEntity], viewer->selectedEntity,
                                    &viewer->compilerv2.objectEntityList[viewer->selectedEntity],
                                    &viewer->compilerv3.objectEntityList[viewer->selectedEntity],
                                    &viewer->compilerv4.objectEntityList[viewer->selectedEntity],
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
                                objProp->setupUI(
                                    &viewer->entities[viewer->selectedEntity], viewer->selectedEntity,
                                    &viewer->compilerv2.objectEntityList[viewer->selectedEntity],
                                    &viewer->compilerv3.objectEntityList[viewer->selectedEntity],
                                    &viewer->compilerv4.objectEntityList[viewer->selectedEntity],
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
                    case SceneViewer::TOOL_PARALLAX: break;
                    default: break;
                }
                break;
            }

            break;
        }

        case QEvent::MouseMove: {
            bool status         = false;
            QMouseEvent *mEvent = static_cast<QMouseEvent *>(event);

            viewer->mousePos.x = viewer->cam.lastMousePos.x = mEvent->pos().x();
            viewer->mousePos.y = viewer->cam.lastMousePos.y = mEvent->pos().y();

            Vector2<int> sceneMousePos((viewer->mousePos.x * viewer->invZoom()) + viewer->cam.pos.x,
                                       (viewer->mousePos.y * viewer->invZoom()) + viewer->cam.pos.y);

            if (mouseDownM || (mouseDownL && viewer->curTool == SceneViewer::TOOL_MOUSE)) {
                viewer->cam.pos.x -= (viewer->mousePos.x - viewer->reference.x()) * viewer->invZoom();
                viewer->cam.pos.y -= (viewer->mousePos.y - viewer->reference.y()) * viewer->invZoom();
                viewer->reference = mEvent->pos();
                status            = true;

                ui->horizontalScrollBar->blockSignals(true);
                ui->horizontalScrollBar->setMaximum((viewer->scene.width * 0x80) - viewer->storedW);
                ui->horizontalScrollBar->setValue(viewer->cam.pos.x);
                ui->horizontalScrollBar->blockSignals(false);

                ui->verticalScrollBar->blockSignals(true);
                ui->verticalScrollBar->setMaximum((viewer->scene.height * 0x80) - viewer->storedH);
                ui->verticalScrollBar->setValue(viewer->cam.pos.y);
                ui->verticalScrollBar->blockSignals(false);
            }

            if (viewer->curTool == SceneViewer::TOOL_PENCIL
                || viewer->curTool == SceneViewer::TOOL_ENTITY) {
                viewer->tilePos.x = viewer->mousePos.x;
                viewer->tilePos.y = viewer->mousePos.y;

                if (ctrlDownL) {
                    viewer->tilePos.x -= fmodf(viewer->tilePos.x + (0x80 / 2), 0x80);
                    viewer->tilePos.y -= fmodf(viewer->tilePos.y + (0x80 / 2), 0x80);

                    // TODO: entities not previewing correctly when snapping
                }
            }

            // Hover
            switch (viewer->curTool) {
                default: break;
                case SceneViewer::TOOL_MOUSE: break;
                case SceneViewer::TOOL_SELECT: break;
                case SceneViewer::TOOL_PENCIL: break;
                case SceneViewer::TOOL_ERASER: break;
                case SceneViewer::TOOL_ENTITY: break;
                case SceneViewer::TOOL_PARALLAX: break;
            }

            if (mouseDownL) {
                switch (viewer->curTool) {
                    default: break;
                    case SceneViewer::TOOL_MOUSE: break;
                    case SceneViewer::TOOL_SELECT: break;
                    case SceneViewer::TOOL_PENCIL: {
                        if (viewer->selectedChunk >= 0 && viewer->selecting) {
                            setTile(viewer->mousePos.x, viewer->mousePos.y);
                            doAction();
                        }
                        break;
                    }
                    case SceneViewer::TOOL_ERASER: {
                        if (viewer->selecting) {
                            viewer->selectedChunk = 0x0;
                            setTile(viewer->mousePos.x, viewer->mousePos.y);
                            doAction();
                        }
                        break;
                    }
                    case SceneViewer::TOOL_ENTITY: {
                        if (viewer->selectedObject < 0 && viewer->selectedEntity >= 0) {
                            SceneViewer::EntityInfo &entity = viewer->entities[viewer->selectedEntity];

                            entity.pos.x =
                                ((viewer->mousePos.x * viewer->invZoom()) + viewer->cam.pos.x);
                            entity.pos.y =
                                ((viewer->mousePos.y * viewer->invZoom()) + viewer->cam.pos.y);
                            entity.pos.x -= selectionOffset.x;
                            entity.pos.y -= selectionOffset.y;

                            if (ctrlDownL) {
                                entity.pos.x = (entity.pos.x - fmodf(entity.pos.x, viewer->gridSize.x));
                                entity.pos.y = (entity.pos.y - fmodf(entity.pos.y, viewer->gridSize.y));
                            }

                            int id                                       = viewer->selectedEntity;
                            viewer->compilerv2.objectEntityList[id].XPos = entity.pos.x * 65536;
                            viewer->compilerv2.objectEntityList[id].YPos = entity.pos.y * 65536;

                            viewer->compilerv3.objectEntityList[id].XPos = entity.pos.x * 65536;
                            viewer->compilerv3.objectEntityList[id].YPos = entity.pos.y * 65536;

                            viewer->compilerv4.objectEntityList[id].XPos = entity.pos.x * 65536;
                            viewer->compilerv4.objectEntityList[id].YPos = entity.pos.y * 65536;
                            doAction();
                        }
                        break;
                    }
                    case SceneViewer::TOOL_PARALLAX: break;
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
            viewer->cam.pos.y -= wEvent->angleDelta().y() / 8;
            viewer->cam.pos.x -= wEvent->angleDelta().x() / 8;

            ui->horizontalScrollBar->blockSignals(true);
            ui->horizontalScrollBar->setMaximum((viewer->scene.width * 0x80) - viewer->storedW);
            ui->horizontalScrollBar->setValue(viewer->cam.pos.x);
            ui->horizontalScrollBar->blockSignals(false);

            ui->verticalScrollBar->blockSignals(true);
            ui->verticalScrollBar->setMaximum((viewer->scene.height * 0x80) - viewer->storedH);
            ui->verticalScrollBar->setValue(viewer->cam.pos.y);
            ui->verticalScrollBar->blockSignals(false);

            break;
        }

        case QEvent::KeyPress: {
            QKeyEvent *kEvent = static_cast<QKeyEvent *>(event);

            Vector2<int> sceneMousePos((viewer->mousePos.x * viewer->invZoom()) + viewer->cam.pos.x,
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
                            if (viewer->entities.count() < FormatHelpers::Scene::entityLimit) {
                                SceneViewer::EntityInfo object = *(SceneViewer::EntityInfo *)clipboard;
                                object.pos.x                   = (sceneMousePos.x);
                                object.pos.y                   = (sceneMousePos.y);

                                int cnt = viewer->entities.count();
                                viewer->entities.append(object);
                                viewer->compilerv2.objectEntityList[cnt].XPos = object.pos.x * 65536;
                                viewer->compilerv2.objectEntityList[cnt].YPos = object.pos.y * 65536;

                                viewer->compilerv3.objectEntityList[cnt].XPos = object.pos.x * 65536;
                                viewer->compilerv3.objectEntityList[cnt].YPos = object.pos.y * 65536;

                                viewer->compilerv4.objectEntityList[cnt].XPos = object.pos.x * 65536;
                                viewer->compilerv4.objectEntityList[cnt].YPos = object.pos.y * 65536;
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
            if (kEvent->key() == Qt::Key_S)
                tool = SceneViewer::TOOL_MOUSE;
            if (kEvent->key() == Qt::Key_S)
                tool = SceneViewer::TOOL_SELECT;
            if (kEvent->key() == Qt::Key_C)
                tool = SceneViewer::TOOL_PENCIL;
            if (kEvent->key() == Qt::Key_E)
                tool = SceneViewer::TOOL_ERASER;
            if (kEvent->key() == Qt::Key_O)
                tool = SceneViewer::TOOL_ENTITY;
            if (kEvent->key() == Qt::Key_P)
                tool = SceneViewer::TOOL_PARALLAX;

            if (tool != prevTool)
                resetTools(tool);

            switch (viewer->curTool) {
                case SceneViewer::TOOL_MOUSE: break;
                case SceneViewer::TOOL_SELECT: break;
                case SceneViewer::TOOL_PENCIL:
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
                                        (viewer->tilePos.x * viewer->invZoom()) + viewer->cam.pos.x,
                                        (viewer->tilePos.y * viewer->invZoom()) + viewer->cam.pos.y);
                                    if (box.contains(pos)) {
                                        if (viewer->selectedLayer > 0) {
                                            viewer->background.layers[viewer->selectedLayer - 1]
                                                .layout[y][x] = 0;
                                        }
                                        else {
                                            viewer->scene.layout[y][x] = 0;
                                        }
                                        doAction();

                                        // reset context
                                        break;
                                    }
                                }
                            }
                        }
                    }
                    break;
                case SceneViewer::TOOL_ERASER: break;
                case SceneViewer::TOOL_ENTITY:
                    if (kEvent->key() == Qt::Key_Delete || kEvent->key() == Qt::Key_Backspace) {
                        if (viewer->selectedEntity >= 0) {
                            viewer->entities.removeAt(viewer->selectedEntity);
                            viewer->selectedEntity = -1;

                            objProp->unsetUI();
                            createEntityList();
                            doAction();
                        }
                    }

                    if ((kEvent->modifiers() & Qt::ControlModifier) == Qt::ControlModifier
                        && kEvent->key() == Qt::Key_C) {
                        if (viewer->selectedEntity >= 0) {
                            clipboard     = &viewer->entities[viewer->selectedEntity];
                            clipboardType = COPY_ENTITY;
                            clipboardInfo = viewer->selectedEntity;
                        }
                    }
                    break;
                case SceneViewer::TOOL_PARALLAX: break;
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

void SceneEditor::loadScene(QString scnPath, QString gcfPath, byte gameType)
{
    setStatus("Loading Scene...");

    viewer->objectsLoaded = false;

    if (gcfPath != viewer->gameConfig.filePath) {
        if (QFileInfo(gcfPath).suffix().toLower().contains("xml"))
            parseGameXML(gameType, gcfPath);
        else
            viewer->gameConfig.read(gameType, gcfPath);
    }
    QString dataPath = QFileInfo(gcfPath).absolutePath();
    QDir dir(dataPath);
    dir.cdUp();
    viewer->dataPath = dir.path();

    viewer->gameType = gameType;

    viewer->loadScene(scnPath, gameType);

    for (int i = 0; i < 9; ++i) viewer->visibleLayers[i] = false;
    for (int a = 0; a < 4; ++a) {
        byte id = viewer->scene.activeLayer[a];
        if (id < 9)
            viewer->visibleLayers[id] = true;
    }

    ui->layerList->blockSignals(true);
    ui->layerList->clear();
    QListWidgetItem *itemFG = new QListWidgetItem("Foreground", ui->layerList);
    itemFG->setFlags(itemFG->flags() | Qt::ItemIsUserCheckable);
    itemFG->setCheckState(viewer->visibleLayers[0] ? Qt::Checked : Qt::Unchecked);

    for (int l = 0; l < viewer->background.layers.count(); ++l) {
        QListWidgetItem *item =
            new QListWidgetItem("Background " + QString::number(l + 1), ui->layerList);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(viewer->visibleLayers[l + 1] ? Qt::Checked : Qt::Unchecked);
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

    ui->horizontalScrollBar->setMaximum((viewer->scene.width * 0x80) - viewer->storedW);
    ui->verticalScrollBar->setMaximum((viewer->scene.height * 0x80) - viewer->storedH);
    ui->horizontalScrollBar->setPageStep(0x80);
    ui->verticalScrollBar->setPageStep(0x80);

    if (viewer->gameType == ENGINE_v1) {
        scnProp->musBox->clear();

        for (int m = 0; m < viewer->stageConfig.music.count(); ++m)
            scnProp->musBox->addItem(viewer->stageConfig.music[m]);
    }
    scnProp->setupUI(&viewer->scene, viewer->gameType);

    scnProp->loadGlobalCB->blockSignals(true);
    scnProp->loadGlobalCB->setDisabled(viewer->gameType == ENGINE_v1);
    scnProp->loadGlobalCB->setChecked(viewer->stageConfig.loadGlobalScripts);
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

    scnProp->setupUI(&viewer->scene, viewer->gameType);
    lyrProp->setupUI(&viewer->scene, &viewer->background, 0, viewer->gameType);
    tileProp->unsetUI();
    objProp->unsetUI();
    scrProp->unsetUI();

    initGameLink();

    ui->addEnt->setDisabled(viewer->entities.count() >= FormatHelpers::Scene::entityLimit);

    tabTitle = Utils::getFilenameAndFolder(scnPath);

    viewer->objectsLoaded = true;
    clearActions();
    appConfig.addRecentFile(viewer->gameType, TOOL_SCENEEDITOR, scnPath, QList<QString>{ gcfPath });
    setStatus("Loaded Scene: " + QFileInfo(scnPath).fileName());
}

bool SceneEditor::saveScene(bool forceSaveAs)
{
    QString path = viewer->scene.filepath;

    if (!forceSaveAs && QFile::exists(path)) {
        setStatus("Saving Scene...");
        QString basePath = path.replace(QFileInfo(path).fileName(), "");

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

        if (viewer->gameType != ENGINE_v1) {
            viewer->scene.write(viewer->gameType, viewer->scene.filepath);
            viewer->background.write(viewer->gameType, basePath + "Backgrounds.bin");
            viewer->chunkset.write(viewer->gameType, basePath + "128x128Tiles.bin");
            viewer->tileconfig.write(basePath + "CollisionMasks.bin");
            viewer->stageConfig.write(viewer->gameType, basePath + "StageConfig.bin");
            tileset.write(basePath + "16x16Tiles.gif");
        }
        else {
            RSDKv1::TileConfig tileconfigRS;

            // TODO: port data

            viewer->scene.write(viewer->gameType, viewer->scene.filepath);
            viewer->background.write(viewer->gameType, basePath + "ZoneBG.map");
            viewer->chunkset.write(viewer->gameType, basePath + "Zone.til");
            tileconfigRS.write(basePath + "Zone.tcf");
            viewer->stageConfig.write(viewer->gameType, basePath + "Zone.zcf");

            RSDKv1::GFX *gfx = new RSDKv1::GFX;
            gfx->importImage(tileset);
            gfx->write(basePath + "Zone.gfx");

            delete gfx;
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
        filedialog.selectNameFilter(types[viewer->gameType - 1]);
        filedialog.setAcceptMode(QFileDialog::AcceptSave);

        if (filedialog.exec() == QDialog::Accepted) {
            int saveVer = types.indexOf(filedialog.selectedNameFilter()) + ENGINE_v4;

            setStatus("Saving Scene...");
            QString path     = filedialog.selectedFiles()[0];
            QString basePath = path.replace(QFileInfo(path).fileName(), "");

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

            RSDKv1::TileConfig tileconfigRS;

            // TODO: port data

            switch (saveVer) {
                default: break;
                case ENGINE_v4:
                case ENGINE_v3:
                case ENGINE_v2:
                    viewer->scene.write(saveVer, filedialog.selectedFiles()[0]);
                    viewer->background.write(saveVer, basePath + "Backgrounds.bin");
                    viewer->chunkset.write(saveVer, basePath + "128x128Tiles.bin");
                    viewer->tileconfig.write(basePath + "CollisionMasks.bin");
                    viewer->stageConfig.write(saveVer, basePath + "StageConfig.bin");
                    tileset.write(basePath + "16x16Tiles.gif");
                    break;
                case ENGINE_v1: {
                    viewer->scene.write(saveVer, filedialog.selectedFiles()[0]);
                    viewer->background.write(saveVer, basePath + "ZoneBG.map");
                    viewer->chunkset.write(saveVer, basePath + "Zone.til");
                    tileconfigRS.write(basePath + "Zone.tcf");
                    viewer->stageConfig.write(saveVer, basePath + "Zone.zcf");

                    RSDKv1::GFX *gfx = new RSDKv1::GFX;
                    gfx->importImage(tileset);
                    gfx->write(basePath + "Zone.gfx");

                    delete gfx;
                    break;
                }
            }

            appConfig.addRecentFile(viewer->gameType, TOOL_SCENEEDITOR, filedialog.selectedFiles()[0],
                                    QList<QString>{ viewer->gameConfig.filePath });
            setStatus("Saved Scene: " + Utils::getFilenameAndFolder(viewer->scene.filepath));
            return true;
        }
    }
    return false;
}

void SceneEditor::initGameLink()
{
    viewer->compilerv2.clearScriptData();
    viewer->compilerv3.clearScriptData();
    viewer->compilerv4.clearScriptData();

    for (int i = 0; i < ENTITY_COUNT; ++i) {
        memset(&viewer->compilerv2.objectEntityList[i], 0,
               sizeof(viewer->compilerv2.objectEntityList[i]));
        memset(&viewer->compilerv3.objectEntityList[i], 0,
               sizeof(viewer->compilerv3.objectEntityList[i]));
        memset(&viewer->compilerv4.objectEntityList[i], 0,
               sizeof(viewer->compilerv4.objectEntityList[i]));

        if (i < viewer->entities.count()) {
            viewer->compilerv2.objectEntityList[i].type          = viewer->entities[i].type;
            viewer->compilerv2.objectEntityList[i].propertyValue = viewer->entities[i].propertyValue;
            viewer->compilerv2.objectEntityList[i].XPos          = viewer->entities[i].pos.x * 65536;
            viewer->compilerv2.objectEntityList[i].YPos          = viewer->entities[i].pos.y * 65536;

            viewer->compilerv3.objectEntityList[i].type          = viewer->entities[i].type;
            viewer->compilerv3.objectEntityList[i].propertyValue = viewer->entities[i].propertyValue;
            viewer->compilerv3.objectEntityList[i].XPos          = viewer->entities[i].pos.x * 65536;
            viewer->compilerv3.objectEntityList[i].YPos          = viewer->entities[i].pos.y * 65536;

            viewer->compilerv4.objectEntityList[i].type          = viewer->entities[i].type;
            viewer->compilerv4.objectEntityList[i].propertyValue = viewer->entities[i].propertyValue;
            viewer->compilerv4.objectEntityList[i].XPos          = viewer->entities[i].pos.x * 65536;
            viewer->compilerv4.objectEntityList[i].YPos          = viewer->entities[i].pos.y * 65536;

            int *values[] = { &viewer->compilerv4.objectEntityList[i].state,
                              NULL,
                              &viewer->compilerv4.objectEntityList[i].scale,
                              &viewer->compilerv4.objectEntityList[i].rotation,
                              NULL,
                              NULL,
                              &viewer->compilerv4.objectEntityList[i].alpha,
                              NULL,
                              &viewer->compilerv4.objectEntityList[i].animationSpeed,
                              NULL,
                              NULL,
                              &viewer->compilerv4.objectEntityList[i].values[0],
                              &viewer->compilerv4.objectEntityList[i].values[1],
                              &viewer->compilerv4.objectEntityList[i].values[2],
                              &viewer->compilerv4.objectEntityList[i].values[3] };

            byte *valuesB[] = { NULL,
                                &viewer->compilerv4.objectEntityList[i].direction,
                                NULL,
                                NULL,
                                (byte *)&viewer->compilerv4.objectEntityList[i].drawOrder,
                                &viewer->compilerv4.objectEntityList[i].priority,
                                NULL,
                                &viewer->compilerv4.objectEntityList[i].animation,
                                NULL,
                                &viewer->compilerv4.objectEntityList[i].frame,
                                &viewer->compilerv4.objectEntityList[i].inkEffect,
                                NULL,
                                NULL,
                                NULL,
                                NULL };

            for (int v = 0; v < 0x0F; ++v) {
                if (viewer->entities[i].variables[v].active) {
                    if (values[v])
                        *values[v] = viewer->entities[i].variables[v].value;
                    else
                        *valuesB[v] = viewer->entities[i].variables[v].value;
                }
            }
        }
    }

    viewer->compilerv2.clearScriptData();
    viewer->compilerv3.clearScriptData();
    viewer->compilerv4.clearScriptData();
    int id                             = 0;
    viewer->compilerv3.typeNames[id]   = "Blank Object";
    viewer->compilerv4.typeNames[id++] = "Blank Object";
    for (int o = 0; o < viewer->gameConfig.objects.count(); ++o) {
        viewer->compilerv3.typeNames[id] = viewer->gameConfig.objects[o].m_name;
        viewer->compilerv4.typeNames[id] = viewer->gameConfig.objects[o].m_name;

        viewer->compilerv3.typeNames[id].replace(" ", "");
        viewer->compilerv4.typeNames[id].replace(" ", "");

        id++;
    }
    for (int o = 0; o < viewer->stageConfig.objects.count(); ++o) {
        viewer->compilerv3.typeNames[id] = viewer->stageConfig.objects[o].name;
        viewer->compilerv4.typeNames[id] = viewer->stageConfig.objects[o].name;

        viewer->compilerv3.typeNames[id].replace(" ", "");
        viewer->compilerv4.typeNames[id].replace(" ", "");

        id++;
    }

    for (int v = 0; v < viewer->gameConfig.globalVariables.count(); ++v) {
        viewer->compilerv2.globalVariables.append(viewer->gameConfig.globalVariables[v].name);
        viewer->compilerv3.globalVariables.append(viewer->gameConfig.globalVariables[v].name);
        viewer->compilerv4.globalVariables.append(viewer->gameConfig.globalVariables[v].name);
    }

    switch (viewer->gameType) {
        case ENGINE_v1: break; // read the editor stuff from this somehow (idk how to parse it lol)
        case ENGINE_v2: {      // parse the RSDK sub and use that data to know what to draw
            int scrID                 = 1;
            viewer->compilerv2.viewer = viewer;

            if (viewer->stageConfig.loadGlobalScripts) {
                for (int i = 0; i < viewer->gameConfig.objects.count(); ++i) {
                    QString scriptPath =
                        viewer->dataPath + "/Scripts/" + viewer->gameConfig.objects[i].script;
                    scriptPath = WorkingDirManager::GetPath(
                        "/Scripts/" + viewer->gameConfig.objects[i].script, scriptPath);

                    viewer->compilerv2.parseScriptFile(scriptPath, scrID++);

                    if (viewer->compilerv2.scriptError) {
                        printLog(viewer->compilerv2.errorMsg);
                        printLog(viewer->compilerv2.errorPos);
                        printLog(QString::number(viewer->compilerv2.errorLine));

                        QFileInfo info(viewer->compilerv2.errorScr);
                        QDir dir(info.dir());
                        dir.cdUp();
                        QString dirFile = dir.relativeFilePath(viewer->compilerv2.errorScr);

                        setStatus("Failed to compile script: " + dirFile);
                        viewer->compilerv2.objectScriptList[scrID - 1].subRSDK.scriptCodePtr = -1;
                        viewer->compilerv2.objectScriptList[scrID - 1].subRSDK.jumpTablePtr  = -1;
                    }
                }
            }

            for (int i = 0; i < viewer->stageConfig.objects.count(); ++i) {
                QString scriptPath =
                    viewer->dataPath + "/Scripts/" + viewer->stageConfig.objects[i].script;
                scriptPath = WorkingDirManager::GetPath(
                    "/Scripts/" + viewer->stageConfig.objects[i].script, scriptPath);
                viewer->compilerv2.parseScriptFile(scriptPath, scrID++);

                if (viewer->compilerv2.scriptError) {
                    printLog(viewer->compilerv2.errorMsg);
                    printLog(viewer->compilerv2.errorPos);
                    printLog(QString::number(viewer->compilerv2.errorLine));

                    QFileInfo info(viewer->compilerv2.errorScr);
                    QDir dir(info.dir());
                    dir.cdUp();
                    QString dirFile = dir.relativeFilePath(viewer->compilerv2.errorScr);

                    setStatus("Failed to compile script: " + dirFile);
                    viewer->compilerv2.objectScriptList[scrID - 1].subRSDK.scriptCodePtr = -1;
                    viewer->compilerv2.objectScriptList[scrID - 1].subRSDK.jumpTablePtr  = -1;
                }
            }
            break;
        }
        case ENGINE_v3: { // compile RSDKDraw & RSDKLoad and draw via those
            int scrID                            = 1;
            viewer->compilerv3.viewer            = viewer;
            viewer->compilerv3.gamePlatform      = "Editor";
            viewer->compilerv3.gameRenderType    = "SW_Rendering";
            viewer->compilerv3.gameHapticSetting = "No_Haptics";

            if (viewer->stageConfig.loadGlobalScripts) {
                for (int i = 0; i < viewer->gameConfig.objects.count(); ++i) {
                    QString scriptPath =
                        viewer->dataPath + "/Scripts/" + viewer->gameConfig.objects[i].script;
                    scriptPath = WorkingDirManager::GetPath(
                        "/Scripts/" + viewer->gameConfig.objects[i].script, scriptPath);
                    viewer->compilerv3.parseScriptFile(scriptPath, scrID++);

                    if (viewer->compilerv3.scriptError) {
                        printLog(viewer->compilerv3.errorMsg);
                        printLog(viewer->compilerv3.errorPos);
                        printLog(QString::number(viewer->compilerv3.errorLine));

                        QFileInfo info(viewer->compilerv3.errorScr);
                        QDir dir(info.dir());
                        dir.cdUp();
                        QString dirFile = dir.relativeFilePath(viewer->compilerv3.errorScr);

                        QMessageBox msgBox(QMessageBox::Information, "RetroED",
                                           QString("Failed to compile script: %1\n"
                                                   "Error: %2\n"
                                                   "\n"
                                                   "Pos: %3\n"
                                                   "Line: %4\n")
                                               .arg(viewer->gameConfig.objects[i].script)
                                               .arg(viewer->compilerv3.errorMsg)
                                               .arg(viewer->compilerv3.errorPos)
                                               .arg(viewer->compilerv3.errorLine),
                                           QMessageBox::Ok);
                        msgBox.exec();

                        setStatus("Failed to compile script: " + dirFile);
                        viewer->compilerv3.objectScriptList[scrID - 1].subRSDKDraw.scriptCodePtr = -1;
                        viewer->compilerv3.objectScriptList[scrID - 1].subRSDKDraw.jumpTablePtr  = -1;
                        viewer->compilerv3.objectScriptList[scrID - 1].subRSDKLoad.scriptCodePtr = -1;
                        viewer->compilerv3.objectScriptList[scrID - 1].subRSDKLoad.jumpTablePtr  = -1;
                        viewer->compilerv3.objectScriptList[scrID - 1].subRSDKEdit.scriptCodePtr = -1;
                        viewer->compilerv3.objectScriptList[scrID - 1].subRSDKEdit.jumpTablePtr  = -1;
                    }
                }
            }

            for (int i = 0; i < viewer->stageConfig.objects.count(); ++i) {
                QString scriptPath =
                    viewer->dataPath + "/Scripts/" + viewer->stageConfig.objects[i].script;
                scriptPath = WorkingDirManager::GetPath(
                    "/Scripts/" + viewer->stageConfig.objects[i].script, scriptPath);
                viewer->compilerv3.parseScriptFile(scriptPath, scrID++);

                if (viewer->compilerv3.scriptError) {
                    printLog(viewer->compilerv3.errorMsg);
                    printLog(viewer->compilerv3.errorPos);
                    printLog(QString::number(viewer->compilerv3.errorLine));

                    QFileInfo info(viewer->compilerv3.errorScr);
                    QDir dir(info.dir());
                    dir.cdUp();
                    QString dirFile = dir.relativeFilePath(viewer->compilerv3.errorScr);

                    QMessageBox msgBox(QMessageBox::Information, "RetroED",
                                       QString("Failed to compile script: %1\n"
                                               "Error: %2\n"
                                               "\n"
                                               "Pos: %3\n"
                                               "Line: %4\n")
                                           .arg(viewer->stageConfig.objects[i].script)
                                           .arg(viewer->compilerv3.errorMsg)
                                           .arg(viewer->compilerv3.errorPos)
                                           .arg(viewer->compilerv3.errorLine),
                                       QMessageBox::Ok);
                    msgBox.exec();

                    setStatus("Failed to compile script: " + dirFile);
                    viewer->compilerv3.objectScriptList[scrID - 1].subRSDKDraw.scriptCodePtr = -1;
                    viewer->compilerv3.objectScriptList[scrID - 1].subRSDKDraw.jumpTablePtr  = -1;
                    viewer->compilerv3.objectScriptList[scrID - 1].subRSDKLoad.scriptCodePtr = -1;
                    viewer->compilerv3.objectScriptList[scrID - 1].subRSDKLoad.jumpTablePtr  = -1;
                    viewer->compilerv3.objectScriptList[scrID - 1].subRSDKEdit.scriptCodePtr = -1;
                    viewer->compilerv3.objectScriptList[scrID - 1].subRSDKEdit.jumpTablePtr  = -1;
                }
            }
            break;
        }
        case ENGINE_v4: { // compile RSDKDraw & RSDKLoad and draw via those
            int scrID                            = 1;
            viewer->compilerv4.viewer            = viewer;
            viewer->compilerv4.gamePlatform      = "EDITOR";
            viewer->compilerv4.gameRenderType    = "SW_RENDERING";
            viewer->compilerv4.gameHapticSetting = "NO_F_FEEDBACK";

            if (viewer->stageConfig.loadGlobalScripts) {
                for (int i = 0; i < viewer->gameConfig.objects.count(); ++i) {
                    QString scriptPath =
                        viewer->dataPath + "/../Scripts/" + viewer->gameConfig.objects[i].script;
                    scriptPath = WorkingDirManager::GetPath(
                        "/../Scripts/" + viewer->gameConfig.objects[i].script, scriptPath);
                    viewer->compilerv4.parseScriptFile(scriptPath, scrID++);

                    if (viewer->compilerv4.scriptError) {
                        printLog(viewer->compilerv4.errorMsg);
                        printLog(viewer->compilerv4.errorPos);
                        printLog(QString::number(viewer->compilerv4.errorLine));

                        QFileInfo info(viewer->compilerv4.errorScr);
                        QDir dir(info.dir());
                        dir.cdUp();
                        QString dirFile = dir.relativeFilePath(viewer->compilerv4.errorScr);

                        QMessageBox msgBox(QMessageBox::Information, "RetroED",
                                           QString("Failed to compile script: %1\n"
                                                   "Error: %2\n"
                                                   "\n"
                                                   "Pos: %3\n"
                                                   "Line: %4\n")
                                               .arg(viewer->gameConfig.objects[i].script)
                                               .arg(viewer->compilerv4.errorMsg)
                                               .arg(viewer->compilerv4.errorPos)
                                               .arg(viewer->compilerv4.errorLine),
                                           QMessageBox::Ok);
                        msgBox.exec();

                        setStatus("Failed to compile script: " + dirFile);
                        viewer->compilerv4.objectScriptList[scrID - 1].eventRSDKDraw.scriptCodePtr = -1;
                        viewer->compilerv4.objectScriptList[scrID - 1].eventRSDKDraw.jumpTablePtr  = -1;
                        viewer->compilerv4.objectScriptList[scrID - 1].eventRSDKLoad.scriptCodePtr = -1;
                        viewer->compilerv4.objectScriptList[scrID - 1].eventRSDKLoad.jumpTablePtr  = -1;
                        viewer->compilerv4.objectScriptList[scrID - 1].eventRSDKEdit.scriptCodePtr = -1;
                        viewer->compilerv4.objectScriptList[scrID - 1].eventRSDKEdit.jumpTablePtr  = -1;
                    }
                }
            }

            for (int i = 0; i < viewer->stageConfig.objects.count(); ++i) {
                QString scriptPath =
                    viewer->dataPath + "/../Scripts/" + viewer->stageConfig.objects[i].script;
                scriptPath = WorkingDirManager::GetPath(
                    "/../Scripts/" + viewer->stageConfig.objects[i].script, scriptPath);
                viewer->compilerv4.parseScriptFile(scriptPath, scrID++);

                if (viewer->compilerv4.scriptError) {
                    printLog(viewer->compilerv4.errorMsg);
                    printLog(viewer->compilerv4.errorPos);
                    printLog(QString::number(viewer->compilerv4.errorLine));

                    QFileInfo info(viewer->compilerv4.errorScr);
                    QDir dir(info.dir());
                    dir.cdUp();
                    QString dirFile = dir.relativeFilePath(viewer->compilerv4.errorScr);

                    setStatus("Failed to compile script: " + dirFile);

                    QMessageBox msgBox(QMessageBox::Information, "RetroED",
                                       QString("Failed to compile script: %1\n"
                                               "Error: %2\n"
                                               "\n"
                                               "Pos: %3\n"
                                               "Line: %4\n")
                                           .arg(viewer->stageConfig.objects[i].script)
                                           .arg(viewer->compilerv4.errorMsg)
                                           .arg(viewer->compilerv4.errorPos)
                                           .arg(viewer->compilerv4.errorLine),
                                       QMessageBox::Ok);
                    msgBox.exec();

                    viewer->compilerv4.objectScriptList[scrID - 1].eventRSDKDraw.scriptCodePtr = -1;
                    viewer->compilerv4.objectScriptList[scrID - 1].eventRSDKDraw.jumpTablePtr  = -1;
                    viewer->compilerv4.objectScriptList[scrID - 1].eventRSDKLoad.scriptCodePtr = -1;
                    viewer->compilerv4.objectScriptList[scrID - 1].eventRSDKLoad.jumpTablePtr  = -1;
                    viewer->compilerv4.objectScriptList[scrID - 1].eventRSDKEdit.scriptCodePtr = -1;
                    viewer->compilerv4.objectScriptList[scrID - 1].eventRSDKEdit.jumpTablePtr  = -1;
                }
            }
            break;
        }
    }

    viewer->compilerv3.objectLoop      = ENTITY_COUNT - 1;
    viewer->compilerv4.objectEntityPos = ENTITY_COUNT - 1;
    for (int o = 0; o < viewer->objects.count(); ++o) {
        viewer->objects[o].variables.clear();
        viewer->objects[o].variablesAliases[SceneViewer::VAR_ALIAS_PROPVAL] = "PropertyValue";
        viewer->objects[o].variablesAliases[SceneViewer::VAR_ALIAS_VAL0]    = "Value0";
        viewer->objects[o].variablesAliases[SceneViewer::VAR_ALIAS_VAL1]    = "Value1";
        viewer->objects[o].variablesAliases[SceneViewer::VAR_ALIAS_VAL2]    = "Value2";
        viewer->objects[o].variablesAliases[SceneViewer::VAR_ALIAS_VAL3]    = "Value3";

        viewer->activeVarObj = o;
        viewer->callGameEvent(SceneViewer::EVENT_RSDKLOAD, o);
    }
    viewer->activeVarObj = -1;

    for (int e = 0; e < viewer->entities.count(); ++e) {
        viewer->entities[e].customVars.clear();
        for (auto &var : viewer->objects[viewer->entities[e].type].variables) {
            RSDKv5::Scene::VariableValue val;
            val.type        = VAR_UINT8;
            val.value_uint8 = 0;
            viewer->entities[e].customVars.append(val);
        }
    }

    // Initial property value check (used for capping the value & etc)
    for (int e = 0; e < viewer->entities.count(); ++e) {
        auto *entity   = &viewer->entities[e];
        auto *entityv2 = &viewer->compilerv2.objectEntityList[viewer->selectedEntity];
        auto *entityv3 = &viewer->compilerv3.objectEntityList[viewer->selectedEntity];
        auto *entityv4 = &viewer->compilerv4.objectEntityList[viewer->selectedEntity];

        if (entityv2)
            entityv2->propertyValue = entity->propertyValue;
        if (entityv3)
            entityv3->propertyValue = entity->propertyValue;
        if (entityv4)
            entityv4->propertyValue = entity->propertyValue;

        bool called  = false;
        byte propVal = objProp->callRSDKEdit(scnEditor->viewer, true, e, -1, 0, &called);
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

void SceneEditor::startUpdates() { viewer->updateTimer->start(1000.0f / 60.0f); }
void SceneEditor::stopUpdates() { viewer->updateTimer->stop(); }

void SceneEditor::createEntityList()
{
    ui->entityList->blockSignals(true);
    ui->entityList->clear();

    // C++11 absolute poggers
    std::sort(viewer->entities.begin(), viewer->entities.end(),
              [](const SceneViewer::EntityInfo &a, const SceneViewer::EntityInfo &b) -> bool {
                  return a.slotID < b.slotID;
              });
    for (int i = 0; i < viewer->entities.count(); ++i) {
        QString name = viewer->objects[viewer->entities[i].type].name;
        ui->entityList->addItem(QString::number(viewer->entities[i].slotID) + ": " + name);
    }
    ui->entityList->blockSignals(false);

    for (int i = 0; i < ENTITY_COUNT; ++i) {
        memset(&viewer->compilerv2.objectEntityList[i], 0,
               sizeof(viewer->compilerv2.objectEntityList[i]));
        memset(&viewer->compilerv3.objectEntityList[i], 0,
               sizeof(viewer->compilerv3.objectEntityList[i]));
        memset(&viewer->compilerv4.objectEntityList[i], 0,
               sizeof(viewer->compilerv4.objectEntityList[i]));

        if (i < viewer->entities.count()) {
            viewer->compilerv2.objectEntityList[i].type          = viewer->entities[i].type;
            viewer->compilerv2.objectEntityList[i].propertyValue = viewer->entities[i].propertyValue;
            viewer->compilerv2.objectEntityList[i].XPos          = viewer->entities[i].pos.x * 65536;
            viewer->compilerv2.objectEntityList[i].YPos          = viewer->entities[i].pos.y * 65536;

            viewer->compilerv3.objectEntityList[i].type          = viewer->entities[i].type;
            viewer->compilerv3.objectEntityList[i].propertyValue = viewer->entities[i].propertyValue;
            viewer->compilerv3.objectEntityList[i].XPos          = viewer->entities[i].pos.x * 65536;
            viewer->compilerv3.objectEntityList[i].YPos          = viewer->entities[i].pos.y * 65536;

            viewer->compilerv4.objectEntityList[i].type          = viewer->entities[i].type;
            viewer->compilerv4.objectEntityList[i].propertyValue = viewer->entities[i].propertyValue;
            viewer->compilerv4.objectEntityList[i].XPos          = viewer->entities[i].pos.x * 65536;
            viewer->compilerv4.objectEntityList[i].YPos          = viewer->entities[i].pos.y * 65536;

            int *values[] = { &viewer->compilerv4.objectEntityList[i].state,
                              NULL,
                              &viewer->compilerv4.objectEntityList[i].scale,
                              &viewer->compilerv4.objectEntityList[i].rotation,
                              NULL,
                              NULL,
                              &viewer->compilerv4.objectEntityList[i].alpha,
                              NULL,
                              &viewer->compilerv4.objectEntityList[i].animationSpeed,
                              NULL,
                              NULL,
                              &viewer->compilerv4.objectEntityList[i].values[0],
                              &viewer->compilerv4.objectEntityList[i].values[1],
                              &viewer->compilerv4.objectEntityList[i].values[2],
                              &viewer->compilerv4.objectEntityList[i].values[3] };

            byte *valuesB[] = { NULL,
                                &viewer->compilerv4.objectEntityList[i].direction,
                                NULL,
                                NULL,
                                (byte *)&viewer->compilerv4.objectEntityList[i].drawOrder,
                                &viewer->compilerv4.objectEntityList[i].priority,
                                NULL,
                                &viewer->compilerv4.objectEntityList[i].animation,
                                NULL,
                                &viewer->compilerv4.objectEntityList[i].frame,
                                &viewer->compilerv4.objectEntityList[i].inkEffect,
                                NULL,
                                NULL,
                                NULL,
                                NULL };

            for (int v = 0; v < 0x0F; ++v) {
                if (viewer->entities[i].variables[v].active) {
                    if (values[v])
                        *values[v] = viewer->entities[i].variables[v].value;
                    else
                        *valuesB[v] = viewer->entities[i].variables[v].value;
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
    for (int i = 0; i < viewer->background.layers[viewer->selectedLayer - 1].scrollInfos.count(); ++i) {
        FormatHelpers::Background::ScrollIndexInfo &info =
            viewer->background.layers[viewer->selectedLayer - 1].scrollInfos[i];
        ui->scrollList->addItem(QString("Start: %1, Length %2").arg(info.startLine).arg(info.length));
    }

    ui->scrollList->blockSignals(false);
    ui->scrollList->setCurrentRow(-1);
}

void SceneEditor::parseGameXML(byte gameType, QString path)
{
    viewer->gameConfig          = FormatHelpers::GameConfig();
    viewer->gameConfig.filePath = path;

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
                obj.m_name = objName;
                obj.script = objScript;
                viewer->gameConfig.objects.append(obj);
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
                viewer->gameConfig.stageLists[list].scenes.append(stage);
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

    viewer->gameConfig = actions[actionIndex].gameConfig;

    viewer->scene       = actions[actionIndex].scene;
    viewer->background  = actions[actionIndex].background;
    viewer->chunkset    = actions[actionIndex].chunkset;
    viewer->stageConfig = actions[actionIndex].stageConfig;

    viewer->tileconfig = actions[actionIndex].tileconfig;

    viewer->objects  = actions[actionIndex].objects;
    viewer->entities = actions[actionIndex].entities;

    // General Editing
    viewer->curTool   = actions[actionIndex].curTool;
    viewer->selecting = actions[actionIndex].selecting;
    viewer->mousePos  = actions[actionIndex].mousePos;

    // Layer Editing
    viewer->tilePos       = actions[actionIndex].tilePos;
    viewer->tileFlip      = actions[actionIndex].tileFlip;
    viewer->selectedChunk = actions[actionIndex].selectedChunk;
    viewer->selectedLayer = actions[actionIndex].selectedLayer;

    // Collision
    viewer->showPlaneA = actions[actionIndex].showPlaneA;
    viewer->showPlaneB = actions[actionIndex].showPlaneB;

    // Entity Editing
    viewer->selectedObject = actions[actionIndex].selectedObject; // placing
    viewer->selectedEntity = actions[actionIndex].selectedEntity; // viewing

    // Parallax Editing
    viewer->showParallax       = actions[actionIndex].showParallax;
    viewer->selectedScrollInfo = actions[actionIndex].selectedScrollInfo;

    // Camera
    viewer->cam.pos = actions[actionIndex].camPos;

    viewer->showGrid = actions[actionIndex].showTileGrid;

    // viewer->compilerv2 = actions[actionIndex].compilerv2;
    // viewer->compilerv3 = actions[actionIndex].compilerv3;
    // viewer->compilerv4 = actions[actionIndex].compilerv4;

    // updating UI
    ui->showTileGrid->blockSignals(true);
    ui->showTileGrid->setDown(viewer->showGrid);
    ui->showTileGrid->blockSignals(false);

    ui->showCollisionA->blockSignals(true);
    ui->showCollisionA->setDown(viewer->showPlaneA);
    ui->showCollisionA->blockSignals(false);

    ui->showCollisionB->blockSignals(true);
    ui->showCollisionB->setDown(viewer->showPlaneB);
    ui->showCollisionB->blockSignals(false);

    ui->showParallax->blockSignals(true);
    ui->showParallax->setDown(viewer->showParallax);
    ui->showParallax->blockSignals(false);

    ui->selToolBox->blockSignals(true);
    ui->selToolBox->setCurrentIndex(viewer->curTool);
    ui->selToolBox->blockSignals(false);

    updateTitle(actionIndex > 0);
}
void SceneEditor::doAction(QString name, bool setModified)
{
    ActionState action;

    action.name = name;

    action.tilePalette = viewer->tilePalette;
    action.tiles       = viewer->tiles;
    action.chunks      = viewer->chunks;

    action.gameConfig = viewer->gameConfig;

    action.scene       = viewer->scene;
    action.background  = viewer->background;
    action.chunkset    = viewer->chunkset;
    action.stageConfig = viewer->stageConfig;

    action.tileconfig = viewer->tileconfig;

    action.objects  = viewer->objects;
    action.entities = viewer->entities;

    // General Editing
    action.curTool   = viewer->curTool;
    action.selecting = viewer->selecting;
    action.mousePos  = viewer->mousePos;

    // Layer Editing
    action.tilePos       = viewer->tilePos;
    action.tileFlip      = viewer->tileFlip;
    action.selectedChunk = viewer->selectedChunk;
    action.selectedLayer = viewer->selectedLayer;

    // Collision
    action.showPlaneA = viewer->showPlaneA;
    action.showPlaneB = viewer->showPlaneB;

    // Entity Editing
    action.selectedObject = viewer->selectedObject; // placing
    action.selectedEntity = viewer->selectedEntity; // viewing

    // Parallax Editing
    action.showParallax       = viewer->showParallax;
    action.selectedScrollInfo = viewer->selectedScrollInfo;

    // Camera
    action.camPos = viewer->cam.pos;

    action.showTileGrid = viewer->showGrid;

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

    setStatus("Did Action: " + name);
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

    FormatHelpers::Background::Layer &layer = viewer->background.layers[layerID];
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
                FormatHelpers::Background::ScrollIndexInfo info;
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
                if (layerID > 0) {
                    FormatHelpers::Background::Layer &layer = viewer->background.layers[layerID - 1];

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
                }
                else {
                    for (int y = 0; y < viewer->scene.height; ++y) {
                        for (int x = 0; x < viewer->scene.width; ++x) {
                            bool ok                    = false;
                            viewer->scene.layout[y][x] = 0;
                            if (id < layout.count()) {
                                int index = layout[id].toInt(&ok);
                                if (ok)
                                    viewer->scene.layout[y][x] = index;
                            }

                            ++id;
                        }
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
                        viewer->scene.width      = width;
                        viewer->scene.height     = height;
                        viewer->visibleLayers[0] = visible;
                        layerID                  = 0;
                    }
                    else if (id < 8) {
                        FormatHelpers::Background::Layer layer;
                        layer.type   = layerTypes.indexOf(type) == -1 ? 1 : layerTypes.indexOf(type);
                        layer.width  = width;
                        layer.height = height;
                        layer.parallaxFactor      = parallaxFactor;
                        layer.scrollSpeed         = scrollSpeed;
                        viewer->visibleLayers[id] = visible;
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
    if (layerID > 0) {
        auto &layer = viewer->background.layers[layerID - 1];

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
    else {
        writeXMLIndentation(writer, indentPos++);
        writer.writeLine("<scrollingInfo>");

        writeXMLIndentation(writer, indentPos);
        writer.writeLine(QString("<scrollInfo startLine=\"%1\" length=\"%2\" "
                                 "parallaxFactor=\"%3\" scrollSpeed=\"%4\" "
                                 "deform=\"%5\"> </scrollInfo>")
                             .arg(0)
                             .arg(viewer->scene.height * 0x80)
                             .arg(1.0)
                             .arg(0.0)
                             .arg("false"));

        writeXMLIndentation(writer, --indentPos);
        writer.writeLine("</scrollingInfo>");
        writer.writeLine();
    }
}

void SceneEditor::writeXMLLayout(Writer &writer, int layerID, int indentPos)
{
    writeXMLIndentation(writer, indentPos++);
    writer.writeLine("<layout>");
    if (layerID > 0) {
        auto &layer = viewer->background.layers[layerID - 1];

        for (int y = 0; y < layer.height; ++y) {
            writeXMLIndentation(writer, indentPos);
            for (int x = 0; x < layer.width; ++x) {
                writer.writeText(QString::number(layer.layout[y][x]));
                writer.writeText(",");
            }
            writer.writeLine();
        }
    }
    else {
        for (int y = 0; y < viewer->scene.height; ++y) {
            writeXMLIndentation(writer, indentPos);
            for (int x = 0; x < viewer->scene.width; ++x) {
                writer.writeText(QString::number(viewer->scene.layout[y][x]));
                writer.writeText(",");
            }
            writer.writeLine();
        }
    }
    writeXMLIndentation(writer, --indentPos);
    writer.writeLine("</layout>");
    writer.writeLine();
}

void SceneEditor::writeXMLLayer(Writer &writer, int layerID, int indentPos)
{
    QList<QString> layerTypes = { "None", "HScroll", "VScroll", "3D Sky", "3D Floor" };

    if (layerID > 0) {
        auto &layer = viewer->background.layers[layerID - 1];

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
                             .arg(viewer->visibleLayers[layerID] ? "true" : "false"));

        writeXMLScrollInfo(writer, layerID, indentPos + 1);

        writeXMLLayout(writer, layerID, indentPos + 1);

        writeXMLIndentation(writer, indentPos);
        writer.writeLine("</layer>");
        writer.writeLine();
    }
    else {
        int drawOrder = 0;

        writeXMLIndentation(writer, indentPos);
        writer.writeLine(QString("<layer name=\"%1\" type=\"%2\" drawOrder=\"%3\" width=\"%4\" "
                                 "height=\"%5\" parallaxFactor=\"%6\" scrollSpeed=\"%7\" "
                                 "visible=\"%8\">")
                             .arg("Foreground")
                             .arg(layerTypes[1])
                             .arg(drawOrder)
                             .arg(viewer->scene.width)
                             .arg(viewer->scene.height)
                             .arg(1.0)
                             .arg(0.0)
                             .arg(viewer->visibleLayers[0] ? "true" : "false"));

        writeXMLScrollInfo(writer, 0, indentPos + 1);

        writeXMLLayout(writer, 0, indentPos + 1);

        writeXMLIndentation(writer, indentPos);
        writer.writeLine("</layer>");
        writer.writeLine();
    }
}

void SceneEditor::writeXMLObject(Writer &writer, int objID, int indentPos)
{
    auto &object = viewer->objects[objID];

    writeXMLIndentation(writer, indentPos);
    writer.writeLine(QString("<object name=\"%1\">").arg(object.name));

    writeXMLIndentation(writer, indentPos + 1);
    writer.writeLine(QString("<variable name=\"%1\" type=\"%2\"> </variable>")
                         .arg(viewer->objects[objID].variablesAliases[SceneViewer::VAR_ALIAS_PROPVAL])
                         .arg("uint8"));

    if (viewer->gameType == ENGINE_v4) {
        bool activeVars[0xF];
        memset(activeVars, 0, sizeof(activeVars));

        if (viewer->gameType == ENGINE_v4) {
            for (auto &entity : viewer->entities) {
                for (int v = 0; v < 0xF; ++v) {
                    if (!entity.variables[v].active)
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
                    name =
                        viewer->objects[objID].variablesAliases[SceneViewer::VAR_ALIAS_VAL0 + (v - 11)];
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
    writer.writeLine(
        QString("<variable name=\"%1\" type=\"uint8\">%2</variable>")
            .arg(viewer->objects[entity.type].variablesAliases[SceneViewer::VAR_ALIAS_PROPVAL])
            .arg(entity.propertyValue));

    if (viewer->gameType == ENGINE_v4) {
        for (int v = 0; v < 0xF; ++v) {
            if (!entity.variables[v].active)
                continue;

            QString name = RSDKv4::objectVariableNames[v];
            name.insert(0, name.at(0).toLower());
            name.remove(1, 1);
            if (v >= 11) {
                name = viewer->objects[entity.type]
                           .variablesAliases[SceneViewer::VAR_ALIAS_VAL0 + (v - 11)];
            }

            writeXMLIndentation(writer, indentPos + 1);
            writer.writeLine(QString("<variable name=\"%1\" type=\"%2\">%3</variable>")
                                 .arg(name)
                                 .arg(RSDKv4::objectVariableTypes[v])
                                 .arg(entity.variables[v].value));
        }
    }

    for (int v = 0; v < viewer->objects[entity.type].variables.count(); ++v) {
        int value = objProp->callRSDKEdit(viewer, true, entityID, v, 0);

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
        uint bgClr1 = (viewer->bgColour.r << 16) | (viewer->bgColour.g << 8) | (viewer->bgColour.b << 0)
                      | (0xFF << 24);
        uint bgClr2 = (viewer->altBGColour.r << 16) | (viewer->altBGColour.g << 8)
                      | (viewer->altBGColour.b << 0) | (0xFF << 24);
        writer.writeLine(
            QString("\t<metadata bgColour=\"%1\" "
                    "altBgColour=\"%2\" title=\"%3\" layerMidpoint=\"%4\" activeLayer0=\"%5\" "
                    "activeLayer1=\"%6\" activeLayer2=\"%7\" activeLayer3=\"%8\" musicID=\"%9\" "
                    "backgroundID=\"%10\" playerX=\"%11\" playerY=\"%12\"> </metadata>")
                .arg(bgClr1)
                .arg(bgClr2)
                .arg(viewer->scene.title)
                .arg(viewer->scene.midpoint)
                .arg(viewer->scene.activeLayer[0])
                .arg(viewer->scene.activeLayer[1])
                .arg(viewer->scene.activeLayer[2])
                .arg(viewer->scene.activeLayer[3])
                .arg(viewer->scene.musicID)
                .arg(viewer->scene.backgroundID)
                .arg(viewer->scene.playerX)
                .arg(viewer->scene.playerY));
    }
    indentPos--;

    indentPos++;
    {
        writer.writeLine();
        writeXMLIndentation(writer, indentPos);
        writer.writeLine("<layers>");

        writeXMLLayer(writer, 0, indentPos + 1);

        int id = 0;
        for (auto &layer : viewer->background.layers) {
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

#include "moc_sceneeditor.cpp"
