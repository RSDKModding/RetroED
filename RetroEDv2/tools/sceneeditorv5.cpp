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
    // ui->showPixelGrid->setIcon(Utils::getColouredIcon(":/icons/ic_grid_48px.svg"));

    connect(ui->tileFlipX, &QCheckBox::toggled, [this](bool c) { viewer->tileFlip.x = c; });
    connect(ui->tileFlipY, &QCheckBox::toggled, [this](bool c) { viewer->tileFlip.y = c; });
    connect(ui->tileSolidTopA, &QCheckBox::toggled, [this](bool c) { viewer->tileSolidA.x = c; });
    connect(ui->tileSolidLRBA, &QCheckBox::toggled, [this](bool c) { viewer->tileSolidA.y = c; });
    connect(ui->tileSolidTopB, &QCheckBox::toggled, [this](bool c) { viewer->tileSolidB.x = c; });
    connect(ui->tileSolidLRBB, &QCheckBox::toggled, [this](bool c) { viewer->tileSolidB.y = c; });

    ui->toolBox->setCurrentIndex(0);
    ui->propertiesBox->setCurrentIndex(0);

    connect(ui->horizontalScrollBar, &QScrollBar::valueChanged, [this](int v) {
        viewer->cam.pos.x           = v;
        viewer->screens->position.x = v;
    });

    connect(ui->verticalScrollBar, &QScrollBar::valueChanged, [this](int v) {
        viewer->cam.pos.y           = v;
        viewer->screens->position.y = v;
    });

    connect(ui->sceneFilter, QOverload<int>::of(&QSpinBox::valueChanged), [this](int v) {
        viewer->sceneFilter      = v;
        viewer->sceneInfo.filter = v;
        doAction("Filter Change: " + QString::number(v));
    });

    connect(ui->useGizmos, &QCheckBox::toggled, [this](bool c) { viewer->sceneInfo.effectGizmo = c; });

    connect(ui->layerList, &QListWidget::currentRowChanged, [this](int c) {
        ui->rmLayer->setDisabled(c == -1);
        ui->upLayer->setDisabled(c == -1);
        ui->downLayer->setDisabled(c == -1);

        if (c == -1)
            return;

        ui->addLayer->setDisabled(viewer->scene.layers.count() >= 8);
        ui->rmLayer->setDisabled(viewer->scene.layers.count() < 0);
        ui->upLayer->setDisabled(c == 0);
        ui->downLayer->setDisabled(c == viewer->scene.layers.count() - 1);

        viewer->selectedLayer = c;

        lyrProp->setupUI(&viewer->scene, viewer->selectedLayer);
        ui->propertiesBox->setCurrentWidget(ui->layerPropPage);

        createScrollList();
        ui->addScr->setDisabled(c == -1);
        ui->rmScr->setDisabled(c == -1);
        ui->impScr->setDisabled(c == -1);
        ui->expScr->setDisabled(c == -1);
    });

    connect(ui->addLayer, &QToolButton::clicked, [this] {
        uint c = ui->layerList->currentRow() + 1;
        RSDKv5::Scene::SceneLayer layer;
        layer.name = "New Layer " + QString::number(c);
        viewer->scene.layers.append(layer);

        QListWidgetItem *item = new QListWidgetItem(layer.name, ui->layerList);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Checked);

        item->setFlags(item->flags() | Qt::ItemIsEditable);
        ui->layerList->setCurrentItem(item);
        doAction("Add Layer: " + layer.name);

        ui->addLayer->setDisabled(viewer->scene.layers.count() >= 8);
    });

    connect(ui->rmLayer, &QToolButton::clicked, [this] {
        int c = ui->layerList->currentRow();
        int n = ui->layerList->currentRow() == ui->layerList->count() - 1 ? c - 1 : c;
        delete ui->layerList->item(c);
        viewer->scene.layers.removeAt(c);
        ui->layerList->blockSignals(true);
        ui->layerList->setCurrentRow(n);
        ui->layerList->blockSignals(false);

        doAction("Remove Layer: " + QString::number(c));

        ui->rmLayer->setDisabled(viewer->scene.layers.count() < 0);
    });

    connect(ui->upLayer, &QToolButton::clicked, [this] {
        uint c     = ui->layerList->currentRow();
        auto *item = ui->layerList->takeItem(c);
        viewer->scene.layers.move(c, c - 1);
        ui->layerList->insertItem(c - 1, item);
        ui->layerList->setCurrentRow(c - 1);
    });

    connect(ui->downLayer, &QToolButton::clicked, [this] {
        uint c     = ui->layerList->currentRow();
        auto *item = ui->layerList->takeItem(c);
        viewer->scene.layers.move(c, c + 1);
        ui->layerList->insertItem(c + 1, item);
        ui->layerList->setCurrentRow(c + 1);
    });

    connect(ui->layerList, &QListWidget::itemChanged, [this](QListWidgetItem *item) {
        int c = ui->layerList->row(item);
        if ((uint)c < (uint)viewer->scene.layers.count())
            viewer->scene.layers[c].visible = item->checkState() == Qt::Checked;
    });

    connect(ui->objectList, &QListWidget::currentRowChanged, [this](int c) {
        ui->rmObj->setDisabled(c == -1 || c >= viewer->objects.count());

        if (c == -1 || c >= viewer->objects.count())
            return;

        bool global = c == 0;
        if (viewer->stageConfig.loadGlobalObjects) {
            for (auto &obj : viewer->gameConfig.objects) {
                if (viewer->objects[c].name == obj) {
                    global = true;
                    break;
                }
            }
        }

        viewer->selectedObject = c;

        memset(&viewer->createGameEntity, 0, sizeof(GameEntityBase));
        viewer->createGameEntity.position.x = 0;
        viewer->createGameEntity.position.y = 0;
        viewer->createGameEntity.objectID   = viewer->selectedObject;

        viewer->createTempEntity.type       = viewer->selectedObject;
        viewer->createTempEntity.pos.x      = 0;
        viewer->createTempEntity.pos.y      = 0;
        viewer->createTempEntity.slotID     = 0xFFFF;
        viewer->createTempEntity.gameEntity = &viewer->createGameEntity;
        viewer->createTempEntity.box        = Rect<int>(0, 0, 0, 0);

        viewer->activeDrawEntity = &viewer->createTempEntity;
        viewer->callGameEvent(viewer->GetObjectInfo(viewer->objects[viewer->selectedObject].name),
                              SceneViewerv5::EVENT_CREATE, &viewer->createTempEntity);

        ui->rmObj->setDisabled(c == -1 || global);
    });

    connect(ui->addObj, &QToolButton::clicked, [this] {
        // uint c = m_objectList->currentRow() + 1;
        SceneObject objInfo;
        objInfo.name = "New Object";
        viewer->stageConfig.objects.append("New Object");
        viewer->objects.append(objInfo);

        // viewer->callGameEvent(gameLink.GetObjectInfo(objInfo.name), SceneViewerv5::EVENT_SERIALIZE,
        //                       NULL);
        //
        // viewer->callGameEvent(gameLink.GetObjectInfo(objInfo.name), SceneViewerv5::EVENT_LOAD, NULL);

        auto *item = new QListWidgetItem();
        item->setText("New Object");
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Checked);
        ui->objectList->addItem(item);

        item->setFlags(item->flags() | Qt::ItemIsEditable);
        ui->objectList->setCurrentItem(item);
        doAction("Add Object: " + objInfo.name);
    });

    connect(ui->rmObj, &QToolButton::clicked, [this] {
        int c = ui->objectList->currentRow();
        int n = ui->objectList->currentRow() == ui->objectList->count() - 1 ? c - 1 : c;
        delete ui->objectList->item(c);
        viewer->objects.removeAt(c);
        int globalCount = 1;
        if (viewer->stageConfig.loadGlobalObjects)
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
        doAction("Remove Object: " + QString::number(c));
    });

    connect(ui->objectList, &QListWidget::itemChanged, [this](QListWidgetItem *item) {
        int c = ui->objectList->row(item);
        if ((uint)c < (uint)viewer->objects.count())
            viewer->objects[c].visible = item->checkState() == Qt::Checked;
    });

    connect(ui->entityList, &QListWidget::currentRowChanged, [this](int c) {
        ui->upEnt->setDisabled(c == -1);
        ui->downEnt->setDisabled(c == -1);
        ui->rmEnt->setDisabled(c == -1);

        if (c == -1)
            return;

        ui->downEnt->setDisabled(c == viewer->entities.count() - 1);
        ui->upEnt->setDisabled(c == 0);

        viewer->selectedEntity    = c;
        viewer->sceneInfo.listPos = c;

        viewer->cam.pos.x = viewer->entities[c].pos.x - ((viewer->storedW / 2) * viewer->invZoom());
        viewer->cam.pos.y = viewer->entities[c].pos.y - ((viewer->storedH / 2) * viewer->invZoom());

        viewer->screens->position.x = viewer->cam.pos.x;
        viewer->screens->position.y = viewer->cam.pos.y;

        objProp->setupUI(&viewer->entities[viewer->selectedEntity]);
        ui->propertiesBox->setCurrentWidget(ui->objPropPage);
    });

    connect(ui->addEnt, &QToolButton::clicked, [this] {
        // uint c = m_objectList->currentRow() + 1;
        SceneEntity entity;
        entity.type = viewer->selectedObject;
        if (viewer->selectedObject < 0)
            entity.type = 0; // backup
        entity.slotID   = viewer->entities.count();
        entity.prevSlot = entity.slotID;

        entity.variables.clear();
        for (int v = 0; v < viewer->objects[entity.type].variables.length(); ++v) {
            RSDKv5::Scene::VariableValue var;
            var.type = viewer->objects[entity.type].variables[v].type;
            entity.variables.append(var);
        }

        for (int v = 0; v < viewer->objects[entity.type].variables.count(); ++v) {
            if (viewer->objects[entity.type].variables[v].name == "filter") {
                if (v < entity.variables.count())
                    entity.variables[v].value_uint8 = viewer->sceneFilter;
                break;
            }
        }

        viewer->callGameEvent(viewer->GetObjectInfo(viewer->objects[entity.type].name),
                              SceneViewerv5::EVENT_CREATE, &entity);
        viewer->entities.append(entity);

        createEntityList();

        ui->addEnt->setDisabled(viewer->activeEntityCount() >= 0x800);
        doAction("Add Entity: " + QString::number(entity.slotID));
    });

    connect(ui->rmEnt, &QToolButton::clicked, [this] {
        int c = ui->entityList->currentRow();
        int n = ui->entityList->currentRow() == ui->entityList->count() - 1 ? c - 1 : c;
        delete ui->entityList->item(c);
        viewer->entities.removeAt(c);
        ui->entityList->blockSignals(true);
        ui->entityList->setCurrentRow(n);
        ui->entityList->blockSignals(false);

        createEntityList();

        if (viewer->objects.count() <= 0) {
            viewer->selectedEntity    = -1;
            viewer->sceneInfo.listPos = c;
        }

        ui->rmEnt->setDisabled(viewer->objects.count() <= 0);

        ui->addEnt->setDisabled(viewer->activeEntityCount() >= 0x800);
        doAction("Remove Entity: " + QString::number(c));
    });

    connect(ui->upEnt, &QToolButton::clicked, [this] {
        ui->entityList->blockSignals(true);
        uint c     = ui->entityList->currentRow();
        auto *item = ui->entityList->takeItem(c);

        int slot                       = viewer->entities[c].slotID;
        viewer->entities[c].slotID     = viewer->entities[c - 1].slotID;
        viewer->entities[c - 1].slotID = slot;

        viewer->entities.move(c, c - 1);

        ui->entityList->insertItem(c - 1, item);
        ui->entityList->setCurrentRow(c - 1);
        ui->entityList->blockSignals(false);

        ui->entityList->item(c)->setText(QString::number(viewer->entities[c].slotID) + ": "
                                         + viewer->objects[viewer->entities[c].type].name);

        ui->entityList->item(c - 1)->setText(QString::number(viewer->entities[c - 1].slotID) + ": "
                                             + viewer->objects[viewer->entities[c - 1].type].name);
    });

    connect(ui->downEnt, &QToolButton::clicked, [this] {
        ui->entityList->blockSignals(true);
        uint c     = ui->entityList->currentRow();
        auto *item = ui->entityList->takeItem(c);

        int slot                       = viewer->entities[c].slotID;
        viewer->entities[c].slotID     = viewer->entities[c + 1].slotID;
        viewer->entities[c + 1].slotID = slot;

        viewer->entities.move(c, c + 1);

        ui->entityList->insertItem(c + 1, item);
        ui->entityList->setCurrentRow(c + 1);
        ui->entityList->blockSignals(false);

        ui->entityList->item(c)->setText(QString::number(viewer->entities[c].slotID) + ": "
                                         + viewer->objects[viewer->entities[c].type].name);

        ui->entityList->item(c + 1)->setText(QString::number(viewer->entities[c + 1].slotID) + ": "
                                             + viewer->objects[viewer->entities[c + 1].type].name);
    });

    connect(ui->scrollList, &QListWidget::currentRowChanged, [this](int c) {
        ui->rmScr->setDisabled(c == -1);

        if (c == -1)
            return;

        viewer->selectedScrollInfo = c;

        scrProp->setupUI(&viewer->scene.layers[viewer->selectedLayer].scrollInfos[c],
                         viewer->scene.layers[viewer->selectedLayer].scrollInfos);
        ui->propertiesBox->setCurrentWidget(ui->scrollPropPage);
    });

    connect(ui->addScr, &QToolButton::clicked, [this] {
        auto &layer = viewer->scene.layers[viewer->selectedLayer];

        auto &last = layer.scrollInfos.last();

        RSDKv5::Scene::ScrollIndexInfo scr = last;
        scr.startLine                      = last.startLine + last.length;
        scr.length                         = 1;
        layer.scrollInfos.append(scr);

        createScrollList();
        doAction("Add Scroll Info");
    });

    connect(ui->rmScr, &QToolButton::clicked, [this] {
        int c = ui->scrollList->currentRow();
        int n = ui->scrollList->currentRow() == ui->scrollList->count() - 1 ? c - 1 : c;
        delete ui->scrollList->item(c);
        viewer->scene.layers[viewer->selectedLayer].scrollInfos.removeAt(c);
        ui->scrollList->blockSignals(true);
        ui->scrollList->setCurrentRow(n);
        ui->scrollList->blockSignals(false);
        doAction("Remov Scroll Info: " + QString::number(c));
    });

    connect(ui->impScr, &QToolButton::clicked, [this] {
        QFileDialog filedialog(this, tr("Import RSDK Scroll File"), "",
                               tr("RSDK Scroll Files (*.bin)"));
        filedialog.setAcceptMode(QFileDialog::AcceptOpen);
        if (filedialog.exec() == QDialog::Accepted) {
            Reader reader(filedialog.selectedFiles()[0]);
            RSDKv5::Scene::SceneLayer &layer = viewer->scene.layers[ui->layerList->currentRow()];
            layer.scrollInfos.clear();
            ushort count = reader.read<ushort>();
            for (int i = 0; i < count; ++i) {
                RSDKv5::Scene::ScrollIndexInfo info;
                info.startLine      = reader.read<int>();
                info.length         = reader.read<int>();
                info.parallaxFactor = reader.read<float>();
                info.scrollSpeed    = reader.read<float>();
                info.deform         = reader.read<byte>();
                info.unknown        = reader.read<byte>();
                layer.scrollInfos.append(info);
            }
            createScrollList();
            doAction("Import Scroll Info for layer: " + layer.name);
        }
    });

    connect(ui->expScr, &QToolButton::clicked, [this] {
        QFileDialog filedialog(this, tr("Export RSDK Scroll File"), "",
                               tr("RSDK Scroll Files (*.bin)"));
        filedialog.setAcceptMode(QFileDialog::AcceptSave);
        if (filedialog.exec() == QDialog::Accepted) {
            Writer writer(filedialog.selectedFiles()[0]);
            RSDKv5::Scene::SceneLayer &layer = viewer->scene.layers[ui->layerList->currentRow()];

            writer.write<ushort>(layer.scrollInfos.count());
            for (int i = 0; i < layer.scrollInfos.count(); ++i) {
                RSDKv5::Scene::ScrollIndexInfo &info = layer.scrollInfos[i];
                writer.write<int>(info.startLine);
                writer.write<int>(info.length);
                writer.write<float>(info.parallaxFactor);
                writer.write<float>(info.scrollSpeed);
                writer.write<byte>(info.deform);
                writer.write<byte>(info.unknown);
            }
            writer.flush();
        }
    });

    auto resetTools = [this](byte tool) {
        if (tool == 0xFF)
            tool = TOOL_MOUSE;
        viewer->curTool = tool;

        // Reset
        // m_mainView->m_selectedTile = -1;
        // m_mainView->m_selectedStamp = -1;
        viewer->selectedEntity    = -1;
        viewer->sceneInfo.listPos = -1;
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
    // connect(ui->showPixelGrid, &QPushButton::clicked, [this] { viewer->showPixelGrid ^= 1; });

    connect(scnProp->loadGlobalCB, &QCheckBox::toggled, [this](bool b) {
        viewer->stageConfig.loadGlobalObjects = b;
        if (viewer->stageConfig.loadGlobalObjects) { // assume we had no globals & are now adding em
            for (int o = viewer->entities.count() - 1; o >= 0; --o) {
                if (viewer->entities[o].type >= 1)
                    viewer->entities[o].type += viewer->gameConfig.objects.count();
            }

            for (int t = viewer->gameConfig.objects.count() - 1; t >= 0; --t) {
                SceneObject obj;
                obj.name = viewer->gameConfig.objects[t];
                // TODO: vars
                viewer->objects.insert(1, obj);
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
                viewer->objects.removeAt(t + 1);
            }
        }

        ui->objectList->clear();
        for (int o = 0; o < viewer->objects.count(); ++o) {
            QListWidgetItem *item = new QListWidgetItem(viewer->objects[o].name, ui->objectList);
            item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
            item->setCheckState(Qt::Checked);
        }

        createEntityList();
        scnProp->syncGC->setDisabled(!viewer->stageConfig.loadGlobalObjects);
        doAction(viewer->stageConfig.loadGlobalObjects ? "Added Global Objects"
                                                       : "Removed Global Objects");
    });

    connect(scnProp->stampNameEdit, &QLineEdit::textChanged, [this](QString s) {
        viewer->scene.editorMetadata.stampName = s;
        // doAction();
    });

    connect(ui->showParallax, &QPushButton::clicked, [this] {
        viewer->showParallax ^= 1;
        doAction("Toggle Parallax Viewer");
    });

    connect(scnProp->editSCF, &QPushButton::clicked, [this] {
        QList<int> newTypes;

        QList<QString> prevObjs = viewer->stageConfig.objects;

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
        for (QString &object : prevObjs) {
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
        for (int o = 0; o < viewer->objects.count(); ++o) {
            QListWidgetItem *item = new QListWidgetItem(viewer->objects[o].name, ui->objectList);
            item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
            item->setCheckState(Qt::Checked);
        }

        createEntityList();
        doAction("Edited Stageconfig");
    });

    connect(scnProp->editTSet, &QPushButton::clicked, [this] {
        TilesetEditor *edit = new TilesetEditor(&viewer->tiles, &viewer->tilePalette);
        edit->setWindowTitle("Edit Tileset");
        edit->exec();

        setStatus("Rebuilding tiles...");
        viewer->tilesetTexture = nullptr;

        QImage tileset(0x10, 0x400 * 0x10, QImage::Format_Indexed8);

        QVector<QRgb> pal;
        for (PaletteColour &col : viewer->tilePalette) {
            pal.append(col.toQColor().rgb());
        }
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

        for (int i = 0; i < viewer->scene.layers.count(); ++i) {
            auto &layer = viewer->scene.layers[i];
            for (int y = 0; y < layer.height; ++y) {
                for (int x = 0; x < layer.width; ++x) {
                    ushort tile        = layer.layout[y][x];
                    layer.layout[y][x] = edit->tileIDs.indexOf(tile & 0x3FF);
                    layer.layout[y][x] |= (tile >> 10) << 10;
                }
            }
        }

        RSDKv5::TileConfig configStore = viewer->tileconfig;
        for (int i = 0; i < 0x400; ++i) {
            int id                                   = edit->tileIDs.indexOf(i);
            viewer->tileconfig.collisionPaths[0][id] = configStore.collisionPaths[0][i];
            viewer->tileconfig.collisionPaths[1][id] = configStore.collisionPaths[1][i];
        }

        doAction("Edited Tiles");
        setStatus("Finished rebuilding tiles!");
    });

    connect(scnProp->editPAL, &QPushButton::clicked, [this] {
        PaletteEditor *edit = new PaletteEditor(viewer->stageConfig.filePath, PALTYPE_GAMECONFIGv4);
        edit->setWindowTitle("Edit StageConfig Palette");
        edit->show();
        doAction("Edited Palette");
    });

    connect(scnProp->syncGC, &QPushButton::clicked, [this] {
        int oldGlobalCount = 0;
        QList<QString> oldGlobals;
        QList<int> oldGlobalIDs;
        QList<int> removeFlags;

        // get info about old objs
        for (int i = 1; i <= oldGlobalCount; ++i) {
            oldGlobals.append(viewer->objects[i].name);
            oldGlobalIDs.append(i);
            removeFlags.append(-1);
        }

        // remove old global objs
        for (int i = oldGlobalCount; i > 0; --i) {
            viewer->objects.removeAt(i);
        }

        int id = 1;
        for (QString &global : viewer->gameConfig.objects) {
            SceneObject objInfo;
            objInfo.name = global;
            viewer->objects.insert(id, objInfo);

            // FunctionTable::setEditableVar(VAR_UINT8, "filter", id,
            //                               offsetof(GameEntity, filter));
            //
            // viewer->callGameEvent(gameLink.GetObjectInfo(objInfo.name),
            //                       SceneViewerv5::EVENT_SERIALIZE, NULL);

            int index = oldGlobals.indexOf(global);
            if (index < 0)
                index = oldGlobals.indexOf(Utils::getMd5HashString(global));

            if (index >= 0) {
                removeFlags[index] = id;
            }
            ++id;
        }

        // start by removing objects we dont need
        id = 1;
        for (auto &flag : removeFlags) {
            if (flag < 0) {
                for (auto &entity : viewer->entities) {
                    if (entity.type == id) {
                        viewer->entities.removeOne(entity);
                    }
                }
            }
            ++id;
        }

        // then update the remaining ones
        id = 1;
        for (auto &flag : removeFlags) {
            if (flag > 0) {
                for (auto &entity : viewer->entities) {
                    if (entity.type == id) {
                        entity.type = flag;
                    }
                }
            }
            ++id;
        }

        // update stage object ids to be proper
        int idChange = viewer->gameConfig.objects.count() - oldGlobalCount;
        for (auto &entity : viewer->entities) {
            if (entity.type >= oldGlobalCount) {
                entity.type += idChange;
            }
        }

        // load assets for new objects
        for (int i = 1; i <= viewer->gameConfig.objects.count(); ++i) {
            // viewer->callGameEvent(gameLink.GetObjectInfo(viewer->objects[i].name),
            //                       SceneViewerv5::EVENT_LOAD, NULL);
        }

        ui->objectList->clear();
        for (int o = 0; o < viewer->objects.count(); ++o) {
            QListWidgetItem *item = new QListWidgetItem(viewer->objects[o].name, ui->objectList);
            item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
            item->setCheckState(Qt::Checked);
        }

        createEntityList();
    });
    scnProp->syncGC->setDisabled(true);

    connect(scnProp->syncSC, &QPushButton::clicked, [this] {});

    connect(ui->exportSceneImg, &QPushButton::clicked, [this] {
        QFileDialog filedialog(this, tr("Save Image"), "", tr("PNG Files (*.png)"));
        filedialog.setAcceptMode(QFileDialog::AcceptSave);
        if (filedialog.exec() == QDialog::Accepted) {

            int width = 0, height = 0;

            for (auto &layer : viewer->scene.layers) {
                if (!layer.visible || layer.drawOrder >= 16)
                    continue;

                if (layer.width > width)
                    width = layer.width;
                if (layer.height > height)
                    height = layer.height;
            }

            QImage image(width * 0x10, height * 0x10, QImage::Format_ARGB32);
            image.fill(0xFFFF00FF);
            QPainter painter(&image);

            for (auto &layer : viewer->scene.layers) {
                if (!layer.visible || layer.drawOrder >= 16)
                    continue;

                for (int y = 0; y < layer.height; ++y) {
                    for (int x = 0; x < layer.width; ++x) {
                        ushort tile = layer.layout[y][x];
                        if (tile != 0xFFFF) {
                            QImage tileImg = viewer->tiles[tile & 0x3FF].copy().mirrored(
                                (tile >> 10) & 1, (tile >> 11) & 1);
                            tileImg.setColor(0, 0x00000000);

                            painter.drawImage(x * 0x10, y * 0x10, tileImg);
                        }
                    }
                }
            }

            painter.beginNativePainting();

            /*QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
            f->glEnable(GL_DEPTH_TEST);
            f->glDepthFunc(GL_LESS);
            f->glEnable(GL_BLEND);
            f->glEnable(GL_MULTISAMPLE);
            f->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            QMatrix4x4 matWorld;
            matWorld.ortho(0.0f, (float)width, (float)height, 0.0f, -16.0f, 16.0f);

            viewer->primitiveShader.use();
            viewer->primitiveShader.setValue("projection", matWorld);
            viewer->primitiveShader.setValue("view", QMatrix4x4());
            viewer->primitiveShader.setValue("useAlpha", false);
            viewer->primitiveShader.setValue("alpha", 1.0f);

            viewer->drawRect(0, 0, 0, 0x40, 0x40, Vector4<float>(0, 0, 0, 1.0), viewer->primitiveShader,
                             false);

            viewer->spriteShader.use();
            viewer->rectVAO.bind();
            viewer->spriteShader.setValue("flipX", false);
            viewer->spriteShader.setValue("flipY", false);
            viewer->spriteShader.setValue("useAlpha", false);
            viewer->spriteShader.setValue("alpha", 1.0f);

            for (auto &entity : viewer->entities) {
                viewer->activeDrawEntity = NULL;

                validDraw = false;

                if (!viewer->objects[entity.type].visible)
                    continue;

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

                if (entity.gameEntity) {
                    entity.gameEntity->position.x = Utils::floatToFixed(entity.pos.x);
                    entity.gameEntity->position.y = Utils::floatToFixed(entity.pos.y);
                }

                // if (entity->type != 0)
                // viewer->callGameEvent(gameLink.GetObjectInfo(viewer->objects[entity->type].name),
                //    EVENT_DRAW, entity);

                // Draw Default Object Sprite if invalid
                // if (!validDraw) {
                //     entity->box = Rect<int>(-0x10, -0x10, 0x10, 0x10);
                //     viewer->spriteShader.use();
                //     viewer->rectVAO.bind();
                //
                //     float xpos = entity->pos.x;
                //     float ypos = entity->pos.y;
                //     float zpos = viewer->incZ();
                //
                //     int w = viewer->gfxSurface[0].texturePtr->width(),
                //         h = viewer->gfxSurface[0].texturePtr->height();
                //     if (viewer->prevSprite) {
                //         viewer->gfxSurface[0].texturePtr->bind();
                //         viewer->spriteShader.setValue("transparentColour", QVector3D(1.0f,
            0.0f, 1.0f));
                //         viewer->spriteShader.setValue("useAlpha", true);
                //         viewer->spriteShader.setValue("alpha", 1.0f);
                //         viewer->prevSprite = 0;
                //     }
                //
                //     QMatrix4x4 matModel;
                //     matModel.scale(w * viewer->zoom, h * viewer->zoom, 1.0f);
                //
                //     matModel.translate(xpos / (float)w, ypos / (float)h, zpos);
                //     viewer->spriteShader.setValue("model", matModel);
                //
                //     f->glDrawArrays(GL_TRIANGLES, 0, 6);
                // }
            }*/

            painter.endNativePainting();
            painter.end();

            image.save(filedialog.selectedFiles()[0]);
            setStatus("Scene exported to image sucessfully!");
        }
    });

    connect(ui->expScene, &QPushButton::clicked, [this] {
        QFileDialog filedialog(this, tr("Save File"), "", tr("XML Files (*.xml)"));
        filedialog.setAcceptMode(QFileDialog::AcceptSave);
        if (filedialog.exec() == QDialog::Accepted) {
            Writer writer(filedialog.selectedFiles()[0]);

            writer.writeLine("<?xml version=\"1.0\"?>");
            writer.writeLine("");
            writer.writeLine("<scene>");

            QList<QString> types = { "uint8", "uint16", "uint32", "int8",    "int16",   "int32",
                                     "enum",  "bool",   "string", "vector2", "unknown", "colour" };

            writer.writeLine(QString("\t<metadata libraryFile=\"%1\" bgColour=\"%2\" "
                                     "altBgColour=\"%2\"> </metadata>")
                                 .arg(viewer->scene.editorMetadata.stampName)
                                 .arg(viewer->scene.editorMetadata.backgroundColor1.rgba())
                                 .arg(viewer->scene.editorMetadata.backgroundColor2.rgba()));

            if (viewer->scene.layers.count()) {
                writer.writeLine();
                writer.writeLine("\t<layers>");

                for (auto &layer : viewer->scene.layers) {

                    writer.writeText(
                        QString(
                            "\t\t<layer name=\"%1\" type=\"%2\" drawOrder=\"%3\" width=\"%4\" "
                            "height=\"%5\" parallaxFactor=\"%6\" scrollSpeed=\"%7\" visible=\"%8\">")
                            .arg(layer.name)
                            .arg(layer.type)
                            .arg(layer.drawOrder)
                            .arg(layer.width)
                            .arg(layer.height)
                            .arg(layer.parallaxFactor)
                            .arg(layer.scrollSpeed)
                            .arg(layer.visible));

                    if (layer.scrollingInfo.count()) {
                        writer.writeLine();
                        for (auto &scroll : layer.scrollInfos) {
                            writer.writeLine(QString("\t\t\t<scrollInfo startLine=\"%1\" length=\"%2\" "
                                                     "parallaxFactor=\"%3\" scrollSpeed=\"%4\" "
                                                     "deform=\"%5\"> </scrollInfo>")
                                                 .arg(scroll.startLine)
                                                 .arg(scroll.length)
                                                 .arg(scroll.parallaxFactor)
                                                 .arg(scroll.scrollSpeed)
                                                 .arg(scroll.deform));
                        }
                    }
                    writer.writeLine();

                    writer.writeLine("\t\t\t<layout>");
                    for (int y = 0; y < layer.height; ++y) {
                        writer.writeText("\t\t\t\t");
                        for (int x = 0; x < layer.width; ++x) {
                            writer.writeText(QString::number(layer.layout[y][x]));
                            writer.writeText(",");
                        }
                        writer.writeLine();
                    }
                    writer.writeLine("\t\t\t</layout>");

                    writer.writeLine("\t\t</layer>");
                }

                writer.writeLine("\t</layers>");
            }

            if (viewer->objects.count()) {
                writer.writeLine();
                writer.writeLine("\t<objects>");

                for (auto &object : viewer->objects) {
                    writer.writeText(QString("\t\t<object name=\"%1\">").arg(object.name));
                    if (object.variables.count()) {
                        writer.writeLine();
                        for (auto &variable : object.variables) {
                            writer.writeLine(
                                QString("\t\t\t<variable name=\"%1\" type=\"%2\"> </variable>")
                                    .arg(variable.name)
                                    .arg(types[variable.type]));
                        }
                    }
                    else {
                        writer.writeText(" ");
                    }
                    writer.writeLine("\t\t</object>");
                }

                writer.writeLine("\t</objects>");
            }

            if (viewer->objects.count()) {
                writer.writeLine();
                writer.writeLine("\t<entities>");

                for (auto &entity : viewer->entities) {
                    writer.writeText(QString("\t\t<entity name=\"%1\" slotID=\"%2\" x=\"%3\" y=\"%4\">")
                                         .arg(viewer->objects[entity.type].name)
                                         .arg(entity.slotID)
                                         .arg(entity.pos.x)
                                         .arg(entity.pos.y));
                    if (entity.variables.count()) {
                        writer.writeLine();

                        int id = 0;
                        for (auto &variable : entity.variables) {
                            writer.writeText(QString("\t\t\t<variable name=\"%1\" type=\"%2\">")
                                                 .arg(viewer->objects[entity.type].variables[id++].name)
                                                 .arg(types[variable.type]));

                            switch (variable.type) {
                                default: break;
                                case VAR_UINT8:
                                    writer.writeText(QString::number(variable.value_uint8));
                                    break;
                                case VAR_UINT16:
                                    writer.writeText(QString::number(variable.value_uint16));
                                    break;
                                case VAR_UINT32:
                                    writer.writeText(QString::number(variable.value_uint32));
                                    break;
                                case VAR_INT8:
                                    writer.writeText(QString::number(variable.value_int8));
                                    break;
                                case VAR_INT16:
                                    writer.writeText(QString::number(variable.value_int16));
                                    break;
                                case VAR_INT32:
                                    writer.writeText(QString::number(variable.value_int32));
                                    break;
                                case VAR_ENUM:
                                    writer.writeText(QString::number(variable.value_enum));
                                    break;
                                case VAR_BOOL:
                                    writer.writeText(QString::number(variable.value_bool));
                                    break;
                                case VAR_STRING: writer.writeText(variable.value_string); break;
                                case VAR_VECTOR2:
                                    writer.writeLine();
                                    writer.writeLine(
                                        QString("\t\t\t\t<x>%1</x>").arg(variable.value_vector2.x));
                                    writer.writeLine(
                                        QString("\t\t\t\t<y>%1</y>").arg(variable.value_vector2.y));
                                    writer.writeText("\t\t\t");
                                    break;
                                case VAR_UNKNOWN:
                                    writer.writeText(QString::number(variable.value_unknown));
                                    break;
                                case VAR_COLOUR:
                                    writer.writeText(QString::number(variable.value_color.rgb()));
                                    break;
                            }
                            writer.writeLine(QString("</variable>"));
                        }
                    }
                    else {
                        writer.writeText(" ");
                    }
                    writer.writeLine("\t\t</entity>");
                }

                writer.writeLine("\t</entities>");
            }

            writer.writeLine("</scene>");

            writer.flush();
        }
    });

    for (QWidget *w : findChildren<QWidget *>()) {
        w->installEventFilter(this);
    }

    initStorage(dataStorage);
}

SceneEditorv5::~SceneEditorv5()
{
    delete ui;

    releaseStorage(dataStorage);
}

bool SceneEditorv5::event(QEvent *event)
{
    if (event->type() == (QEvent::Type)RE_EVENT_NEW) {
        tabTitle = "Scene Editor (v5)";
        clearActions();
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

            QFileDialog gcdialog(
                this, tr("Open GameConfig"), "",
                tr("RSDKv5 GameConfig (GameConfig*.bin);;RSDK Game.xml Files (Game*.xml)"));
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

            loadScene(filedialog.selectedFiles()[0], gcPath, filter);
            return true;
        }
    }

    if (event->type() == (QEvent::Type)RE_EVENT_SAVE) {
        QString path = viewer->scene.filepath;

        if (QFile::exists(path)) {
            setStatus("Saving Scene...");

            viewer->saveScene(path);

            tabTitle = Utils::getFilenameAndFolder(path);
            clearActions();
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

                tabTitle = Utils::getFilenameAndFolder(path);
                clearActions();
                appConfig.addRecentFile(ENGINE_v5, TOOL_SCENEEDITOR, filedialog.selectedFiles()[0],
                                        QList<QString>{ viewer->gameConfig.filePath });
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

            tabTitle = Utils::getFilenameAndFolder(path);
            clearActions();
            appConfig.addRecentFile(ENGINE_v5, TOOL_SCENEEDITOR, filedialog.selectedFiles()[0],
                                    QList<QString>{ viewer->gameConfig.filePath });
            setStatus("Saved Scene: " + Utils::getFilenameAndFolder(filedialog.selectedFiles()[0]));
            return true;
        }
    }

    if (event->type() == (QEvent::Type)RE_EVENT_UNDO) {
        undoAction();
        return true;
    }
    if (event->type() == (QEvent::Type)RE_EVENT_REDO) {
        redoAction();
        return true;
    }

    if (event->type() == QEvent::Close) {
        if (modified) {
            bool cancelled = false;
            if (MainWindow::showCloseWarning(this, &cancelled)) {
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

                    QFileDialog filedialog(
                        this, tr("Save Scene"), "",
                        tr(QString("%1;;%2;;%3;;%4").arg(types[0]).toStdString().c_str()));
                    filedialog.setAcceptMode(QFileDialog::AcceptSave);
                    if (filedialog.exec() == QDialog::Accepted) {
                        setStatus("Saving Scene...");
                        QString path = filedialog.selectedFiles()[0];

                        viewer->saveScene(path);

                        appConfig.addRecentFile(ENGINE_v5, TOOL_SCENEEDITOR,
                                                filedialog.selectedFiles()[0],
                                                QList<QString>{ viewer->gameConfig.filePath });
                        setStatus("Saved Scene: "
                                  + Utils::getFilenameAndFolder(viewer->scene.filepath));
                        return true;
                    }
                }
            }
            else if (cancelled) {
                event->ignore();
                return true;
            }
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

        float xpos = tx + viewer->cam.pos.x;
        float ypos = ty + viewer->cam.pos.y;

        xpos /= 0x10;
        ypos /= 0x10;

        ushort tile = viewer->selectedTile;
        Utils::setBit(tile, viewer->tileFlip.x, 10);
        Utils::setBit(tile, viewer->tileFlip.y, 11);
        Utils::setBit(tile, viewer->tileSolidA.x, 12);
        Utils::setBit(tile, viewer->tileSolidA.y, 13);
        Utils::setBit(tile, viewer->tileSolidB.x, 14);
        Utils::setBit(tile, viewer->tileSolidB.y, 15);
        if (viewer->selectedTile == 0xFFFF)
            tile = 0xFFFF;

        if (ypos >= 0 && ypos < viewer->scene.layers[viewer->selectedLayer].height) {
            if (xpos >= 0 && xpos < viewer->scene.layers[viewer->selectedLayer].width) {
                viewer->scene.layers[viewer->selectedLayer].layout[ypos][xpos] = tile;
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
        viewer->selectedEntity    = -1;
        viewer->sceneInfo.listPos = -1;
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
            viewer->reference   = mEvent->pos();

            viewer->mousePos.x = viewer->cam.lastMousePos.x = mEvent->pos().x();
            viewer->mousePos.y = viewer->cam.lastMousePos.y = mEvent->pos().y();

            if ((mEvent->button() & Qt::LeftButton) == Qt::LeftButton)
                mouseDownL = true;
            if ((mEvent->button() & Qt::MiddleButton) == Qt::MiddleButton)
                mouseDownM = true;
            if ((mEvent->button() & Qt::RightButton) == Qt::RightButton)
                mouseDownR = true;

            if (mouseDownM || (mouseDownL && viewer->curTool == TOOL_MOUSE))
                setCursor(Qt::ClosedHandCursor);

            if (mouseDownL) {
                switch (viewer->curTool) {
                    case TOOL_MOUSE: break;
                    case TOOL_SELECT: break;
                    case TOOL_PENCIL: {
                        if (viewer->selectedTile >= 0 && viewer->isSelecting) {
                            setTile(mEvent->pos().x(), mEvent->pos().y());
                            doAction(QString("Placed Tile: (%1, %2)")
                                         .arg(mEvent->pos().x())
                                         .arg(mEvent->pos().y()));
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
                            int sel              = viewer->selectedTile;
                            viewer->selectedTile = 0xFFFF;
                            setTile(mEvent->pos().x(), mEvent->pos().y());
                            viewer->selectedTile = sel;
                            doAction(QString("Erased Tile: (%1, %2)")
                                         .arg(mEvent->pos().x())
                                         .arg(mEvent->pos().y()));
                        }
                        break;
                    }
                    case TOOL_ENTITY: {
                        if (!viewer->isSelecting || viewer->selectedObject < 0) {
                            Rect<float> box;
                            int firstSel = -1;
                            Vector2<float> firstPos;

                            viewer->selectedEntity    = -1;
                            viewer->sceneInfo.listPos = -1;
                            for (int o = 0; o < viewer->entities.count(); ++o) {
                                int left   = viewer->entities[o].pos.x + viewer->entities[o].box.x;
                                int top    = viewer->entities[o].pos.y + viewer->entities[o].box.y;
                                int right  = viewer->entities[o].pos.x + viewer->entities[o].box.w;
                                int bottom = viewer->entities[o].pos.y + viewer->entities[o].box.h;
                                box = Rect<float>(left, top, abs(right - left), abs(bottom - top));

                                Vector2<float> pos = Vector2<float>(
                                    (mEvent->pos().x() * viewer->invZoom()) + viewer->cam.pos.x,
                                    (mEvent->pos().y() * viewer->invZoom()) + viewer->cam.pos.y);

                                int filter = 0xFF;
                                for (int v = 0;
                                     v < viewer->objects[viewer->entities[o].type].variables.count();
                                     ++v) {
                                    if (viewer->objects[viewer->entities[o].type].variables[v].name
                                        == "filter") {
                                        if (v < viewer->entities[o].variables.count())
                                            filter = viewer->entities[o].variables[v].value_uint8;
                                        break;
                                    }
                                }

                                bool filterFlag = true;
                                if (!(filter & viewer->sceneFilter) && filter)
                                    filterFlag = false;

                                if (box.contains(pos) && firstSel == -1 && filterFlag) {
                                    firstSel = o;
                                    firstPos = pos;
                                }

                                if (box.contains(pos) && viewer->selectedEntity < o && filterFlag) {
                                    viewer->selectedEntity    = o;
                                    viewer->sceneInfo.listPos = o;
                                    selectionOffset.x         = pos.x - viewer->entities[o].pos.x;
                                    selectionOffset.y         = pos.y - viewer->entities[o].pos.y;

                                    objProp->setupUI(&viewer->entities[viewer->selectedEntity]);
                                    ui->propertiesBox->setCurrentWidget(ui->objPropPage);
                                    doAction(QString("Selected Entity: %1 (%2, %3)")
                                                 .arg(o)
                                                 .arg(viewer->entities[o].pos.x)
                                                 .arg(viewer->entities[o].pos.y));
                                    break;
                                }
                            }

                            if (viewer->selectedEntity == -1) {
                                if (firstSel >= 0) {
                                    viewer->selectedEntity    = firstSel;
                                    viewer->sceneInfo.listPos = firstSel;
                                    selectionOffset.x =
                                        firstPos.x - viewer->entities[viewer->selectedEntity].pos.x;
                                    selectionOffset.y =
                                        firstPos.y - viewer->entities[viewer->selectedEntity].pos.y;
                                    // selectionOffset.x = 0;
                                    // selectionOffset.y = 0;
                                    objProp->setupUI(&viewer->entities[viewer->selectedEntity]);
                                    ui->propertiesBox->setCurrentWidget(ui->objPropPage);
                                }
                                else {
                                    selectionOffset.x = 0;
                                    selectionOffset.y = 0;
                                }
                            }
                        }
                        else {
                            if (viewer->selectedObject >= 0 && viewer->activeEntityCount() < 0x800) {
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

                                entity.variables.clear();
                                for (int v = 0; v < viewer->objects[entity.type].variables.length();
                                     ++v) {
                                    RSDKv5::Scene::VariableValue var;
                                    var.type = viewer->objects[entity.type].variables[v].type;
                                    entity.variables.append(var);
                                }

                                for (int v = 0; v < viewer->objects[entity.type].variables.count();
                                     ++v) {
                                    if (viewer->objects[entity.type].variables[v].name == "filter") {
                                        if (v < entity.variables.count())
                                            entity.variables[v].value_uint8 = viewer->sceneFilter;
                                        break;
                                    }
                                }

                                int cnt         = viewer->entities.count();
                                entity.slotID   = cnt;
                                entity.prevSlot = entity.slotID;

                                viewer->callGameEvent(
                                    viewer->GetObjectInfo(viewer->objects[entity.type].name),
                                    SceneViewerv5::EVENT_CREATE, &entity);

                                viewer->entities.append(entity);
                                viewer->selectedEntity    = cnt;
                                viewer->sceneInfo.listPos = cnt;

                                objProp->setupUI(&viewer->entities[viewer->selectedEntity]);
                                ui->propertiesBox->setCurrentWidget(ui->objPropPage);
                                createEntityList();
                                doAction(QString("Created Entity: %1 (%2, %3)")
                                             .arg(viewer->objects[entity.type].name)
                                             .arg(entity.pos.x)
                                             .arg(entity.pos.y));
                            }
                            else if (viewer->activeEntityCount() >= 0x800) {
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
                                        viewer->selectedTile = tile;
                                        doAction(QString("Selected Tile: %1 (%2, %3)")
                                                     .arg(tile & 0x3FF)
                                                     .arg(pos.x)
                                                     .arg(pos.y));
                                        break;
                                    }
                                }
                            }
                        }
                        break;
                    case TOOL_ENTITY: {
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
                                (mEvent->pos().y() * viewer->invZoom()) + +viewer->cam.pos.y);

                            int filter = 0xFF;
                            for (int v = 0;
                                 v < viewer->objects[viewer->entities[o].type].variables.count(); ++v) {
                                if (viewer->objects[viewer->entities[o].type].variables[v].name
                                    == "filter") {
                                    if (v < viewer->entities[o].variables.count())
                                        filter = viewer->entities[o].variables[v].value_uint8;
                                    break;
                                }
                            }

                            bool filterFlag = true;
                            if (!(filter & viewer->sceneFilter) && filter)
                                filterFlag = false;

                            if (box.contains(pos) && firstSel == -1 && filterFlag) {
                                firstSel = o;
                                firstPos = pos;
                            }

                            if (box.contains(pos) && viewer->selectedEntity < o && filterFlag) {
                                viewer->selectedEntity    = o;
                                viewer->sceneInfo.listPos = o;
                                selectionOffset.x         = pos.x - viewer->entities[o].pos.x;
                                selectionOffset.y         = pos.y - viewer->entities[o].pos.y;
                                found                     = true;

                                objProp->setupUI(&viewer->entities[viewer->selectedEntity]);
                                ui->propertiesBox->setCurrentWidget(ui->objPropPage);
                                break;
                            }
                        }

                        if (!found) {
                            if (firstSel >= 0) {
                                viewer->selectedEntity    = firstSel;
                                viewer->sceneInfo.listPos = firstSel;
                                selectionOffset.x =
                                    firstPos.x - viewer->entities[viewer->selectedEntity].pos.x;
                                selectionOffset.y =
                                    firstPos.y - viewer->entities[viewer->selectedEntity].pos.y;
                                // selectionOffset.x = 0;
                                // selectionOffset.y = 0;
                                objProp->setupUI(&viewer->entities[viewer->selectedEntity]);
                                ui->propertiesBox->setCurrentWidget(ui->objPropPage);
                            }
                            else {
                                viewer->selectedEntity    = -1;
                                viewer->sceneInfo.listPos = -1;
                                viewer->selectedObject    = -1;
                                selectionOffset.x         = 0;
                                selectionOffset.y         = 0;
                                ui->objectList->setCurrentRow(-1);
                                ui->entityList->setCurrentRow(-1);

                                objProp->unsetUI();
                                // doAction();
                            }
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

            viewer->mousePos.x = viewer->cam.lastMousePos.x = mEvent->pos().x();
            viewer->mousePos.y = viewer->cam.lastMousePos.y = mEvent->pos().y();

            Vector2<int> m_sceneMousePos((viewer->mousePos.x * viewer->invZoom()) + viewer->cam.pos.x,
                                         (viewer->mousePos.y * viewer->invZoom()) + viewer->cam.pos.y);

            if (mouseDownM || (mouseDownL && viewer->curTool == TOOL_MOUSE)) {
                viewer->cam.pos.x -= (viewer->mousePos.x - viewer->reference.x()) * viewer->invZoom();
                viewer->cam.pos.y -= (viewer->mousePos.y - viewer->reference.y()) * viewer->invZoom();
                QPoint lp        = QCursor::pos();
                QRect screenRect = QGuiApplication::screenAt(viewer->pos())
                                       ->availableGeometry()
                                       .adjusted(20, 20, -19, -19);
                viewer->reference = mEvent->pos();
                if (!screenRect.contains(lp)) {
                    if (lp.x() < screenRect.x())
                        lp.setX(screenRect.x() + screenRect.width());
                    if (lp.x() > screenRect.x() + screenRect.width())
                        lp.setX(screenRect.x());
                    if (lp.y() < screenRect.y())
                        lp.setY(screenRect.y() + screenRect.height());
                    if (lp.y() > screenRect.y() + screenRect.height())
                        lp.setY(screenRect.y());

                    QCursor::setPos(lp);
                    viewer->reference = viewer->mapFromGlobal(lp);
                }

                status = true;

                ui->horizontalScrollBar->blockSignals(true);
                ui->horizontalScrollBar->setValue(viewer->cam.pos.x);
                ui->horizontalScrollBar->blockSignals(false);

                ui->verticalScrollBar->blockSignals(true);
                ui->verticalScrollBar->setValue(viewer->cam.pos.y);
                ui->verticalScrollBar->blockSignals(false);

                viewer->screens->position.x = viewer->cam.pos.x;
                viewer->screens->position.y = viewer->cam.pos.y;
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

            if (mouseDownL) {
                switch (viewer->curTool) {
                    default: break;
                    case TOOL_MOUSE: break;
                    case TOOL_SELECT: break;
                    case TOOL_PENCIL: {
                        if (viewer->selectedTile >= 0 && viewer->isSelecting) {
                            setTile(viewer->mousePos.x, viewer->mousePos.y);
                            doAction(QString("Placed Tile: %1 (%2, %3)")
                                         .arg(viewer->selectedTile)
                                         .arg(viewer->mousePos.x)
                                         .arg(viewer->mousePos.y));
                        }
                        break;
                    }
                    case TOOL_ERASER: {
                        if (viewer->isSelecting) {
                            int sel              = viewer->selectedTile;
                            viewer->selectedTile = 0xFFFF;
                            setTile(viewer->mousePos.x, viewer->mousePos.y);
                            viewer->selectedTile = sel;
                            doAction(QString("Erased Tile: %1 (%2, %3)")
                                         .arg(viewer->selectedTile)
                                         .arg(viewer->mousePos.x)
                                         .arg(viewer->mousePos.y));
                        }
                        break;
                    }
                    case TOOL_ENTITY: {
                        if (viewer->selectedObject < 0 && viewer->selectedEntity >= 0) {
                            SceneEntity &entity = viewer->entities[viewer->selectedEntity];

                            entity.pos.x = (viewer->mousePos.x * viewer->invZoom()) + viewer->cam.pos.x;
                            entity.pos.y = (viewer->mousePos.y * viewer->invZoom()) + viewer->cam.pos.y;
                            entity.pos.x -= selectionOffset.x;
                            entity.pos.y -= selectionOffset.y;

                            if (ctrlDownL) {
                                entity.pos.x = entity.pos.x - fmodf(entity.pos.x, snapSize.x);
                                entity.pos.y = entity.pos.y - fmodf(entity.pos.y, snapSize.y);
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
                    viewer->zoom *= 2;
                else if (wEvent->angleDelta().y() < 0 && viewer->zoom > 0.5)
                    viewer->zoom /= 2;
                ui->horizontalScrollBar->setMaximum((viewer->sceneWidth * 0x10)
                                                    - viewer->storedW / viewer->zoom);
                ui->verticalScrollBar->setMaximum((viewer->sceneHeight * 0x10)
                                                  - viewer->storedH / viewer->zoom);
                return true;
            }
            viewer->cam.pos.y -= wEvent->angleDelta().y() / 8;
            viewer->cam.pos.x -= wEvent->angleDelta().x() / 8;

            ui->horizontalScrollBar->blockSignals(true);
            ui->horizontalScrollBar->setMaximum((viewer->sceneWidth * 0x10)
                                                - (viewer->storedW * viewer->invZoom()));
            ui->horizontalScrollBar->setValue(viewer->cam.pos.x);
            ui->horizontalScrollBar->blockSignals(false);

            ui->verticalScrollBar->blockSignals(true);
            ui->verticalScrollBar->setMaximum((viewer->sceneHeight * 0x10)
                                              - (viewer->storedH * viewer->invZoom()));
            ui->verticalScrollBar->setValue(viewer->cam.pos.y);
            ui->verticalScrollBar->blockSignals(false);

            viewer->screens->position.x = viewer->cam.pos.x;
            viewer->screens->position.y = viewer->cam.pos.y;

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
                            if (viewer->activeEntityCount() < 0x800) {
                                SceneEntity entity = *(SceneEntity *)clipboard;
                                entity.pos.x       = m_sceneMousePos.x;
                                entity.pos.y       = m_sceneMousePos.y;

                                viewer->entities.append(entity);
                                viewer->callGameEvent(
                                    viewer->GetObjectInfo(viewer->objects[entity.type].name),
                                    SceneViewerv5::EVENT_CREATE, &entity);

                                doAction(QString("Pasted Entity: %1 (%2, %3)")
                                             .arg(viewer->objects[entity.type].name)
                                             .arg(entity.pos.x)
                                             .arg(entity.pos.y));
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

                            for (int y = 0; y < viewer->scene.layers[viewer->selectedLayer].height;
                                 ++y) {
                                for (int x = 0; x < viewer->scene.layers[viewer->selectedLayer].width;
                                     ++x) {
                                    box = Rect<float>(x * 0x10, y * 0x10, 0x10, 0x10);

                                    Vector2<float> pos = Vector2<float>(
                                        (viewer->tilePos.x * viewer->invZoom()) + viewer->cam.pos.x,
                                        (viewer->tilePos.y * viewer->invZoom()) + viewer->cam.pos.y);
                                    if (box.contains(pos)) {
                                        viewer->scene.layers[viewer->selectedLayer].layout[y][x] =
                                            0xFFFF;

                                        doAction(
                                            QString("Erased Tile: (%1, %2)").arg(pos.x).arg(pos.y));

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
                            viewer->entities.removeAt(viewer->selectedEntity);
                            viewer->selectedEntity    = -1;
                            viewer->sceneInfo.listPos = -1;

                            objProp->unsetUI();
                            createEntityList();

                            doAction(QString("Deleted Entity: %1").arg(viewer->selectedEntity));
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

    if (viewer->updateTimer)
        viewer->updateTimer->stop();

    releaseStorage(dataStorage);
    initStorage(dataStorage);

    if (gcfPath != viewer->gameConfig.filePath) {
        if (QFileInfo(gcfPath).suffix().toLower().contains("xml"))
            parseGameXML(gcfPath);
        else
            viewer->gameConfig.read(gcfPath, sceneVer == 1);
        dataPath = QFileInfo(gcfPath).absolutePath();
        QDir dir(dataPath);
        dir.cdUp();
        dataPath = dir.path();
    }

    viewer->dataPath = dataPath;

    viewer->loadScene(scnPath);

    ui->layerList->clear();
    ui->layerList->blockSignals(true);
    for (int l = 0; l < viewer->scene.layers.count(); ++l) {
        QListWidgetItem *item = new QListWidgetItem(viewer->scene.layers[l].name, ui->layerList);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(viewer->scene.layers[l].visible ? Qt::Checked : Qt::Unchecked);
    }
    ui->layerList->blockSignals(false);

    ui->objectList->clear();
    ui->objectList->blockSignals(true);
    for (int o = 0; o < viewer->objects.count(); ++o) {
        QListWidgetItem *item = new QListWidgetItem(viewer->objects[o].name, ui->objectList);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Checked);
    }
    ui->objectList->blockSignals(false);

    createEntityList();

    viewer->sceneWidth  = 0;
    viewer->sceneHeight = 0;
    for (int l = 0; l < viewer->scene.layers.count(); ++l) {
        if (viewer->sceneWidth < viewer->scene.layers[l].width)
            viewer->sceneWidth = viewer->scene.layers[l].width;
        if (viewer->sceneHeight < viewer->scene.layers[l].height)
            viewer->sceneHeight = viewer->scene.layers[l].height;
    }

    viewer->refreshResize();

    viewer->colTex =
        new QImage(viewer->sceneWidth * 0x10, viewer->sceneHeight * 0x10, QImage::Format_Indexed8);
    viewer->colTex->setColorTable(
        { qRgb(0, 0, 0), qRgb(255, 255, 0), qRgb(255, 0, 0), qRgb(255, 255, 255) });

    ui->horizontalScrollBar->setMaximum((viewer->sceneWidth * 0x10) - viewer->storedW);
    ui->verticalScrollBar->setMaximum((viewer->sceneHeight * 0x10) - viewer->storedH);
    ui->horizontalScrollBar->setPageStep(0x10);
    ui->verticalScrollBar->setPageStep(0x10);

    scnProp->setupUI(&viewer->scene, &viewer->stageConfig);

    scnProp->loadGlobalCB->blockSignals(true);
    scnProp->loadGlobalCB->setChecked(viewer->stageConfig.loadGlobalObjects);
    scnProp->loadGlobalCB->blockSignals(false);

    scnProp->stampNameEdit->blockSignals(true);
    scnProp->stampNameEdit->setText(viewer->scene.editorMetadata.stampName);
    scnProp->stampNameEdit->blockSignals(false);

    scnProp->syncGC->setDisabled(!viewer->stageConfig.loadGlobalObjects);

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
    lyrProp->setupUI(&viewer->scene, 0);
    tileProp->unsetUI();
    objProp->unsetUI();
    scrProp->unsetUI();

    for (int i = 0; i < viewer->objects.count(); ++i) {
        GameObjectInfo *info = viewer->GetObjectInfo(viewer->objects[i].name);
        if (info) {
            allocateStorage(v5Editor->dataStorage, info->objectSize, (void **)info->type, DATASET_STG,
                            true);

            GameObject *obj = *info->type;
            obj->objectID   = i;
            obj->active     = ACTIVE_NORMAL;
        }
    }

    for (int i = 0; i < viewer->objects.count(); ++i) {
        if (sceneVer != 1)
            FunctionTable::setEditableVar(VAR_UINT8, "filter", i, offsetof(GameEntity, filter));
    }

    for (auto &entity : viewer->entities) {
        auto &obj = viewer->objects[entity.type];
        if (entity.variables.count() != obj.variables.count()) {
            // add & update
            if (entity.variables.count() < obj.variables.count()) {
                for (int v = 0; v < obj.variables.count(); ++v) {
                    // if lower, update the type
                    if (v < entity.variables.count()) {
                        // if we can transfer values, we should
                        if (entity.variables[v].type <= VAR_BOOL && obj.variables[v].type <= VAR_BOOL) {
                            int value = 0;
                            switch (entity.variables[v].type) {
                                case VAR_UINT8: value = entity.variables[v].value_uint8; break;
                                case VAR_INT8: value = entity.variables[v].value_int8; break;
                                case VAR_UINT16: value = entity.variables[v].value_uint16; break;
                                case VAR_INT16: value = entity.variables[v].value_int16; break;
                                case VAR_UINT32: value = entity.variables[v].value_uint32; break;
                                case VAR_INT32: value = entity.variables[v].value_int32; break;
                                case VAR_ENUM: value = entity.variables[v].value_enum; break;
                                case VAR_BOOL: value = entity.variables[v].value_bool ? 1 : 0; break;
                            }

                            switch (obj.variables[v].type) {
                                case VAR_UINT8: entity.variables[v].value_uint8 = value; break;
                                case VAR_INT8: entity.variables[v].value_int8 = value; break;
                                case VAR_UINT16: entity.variables[v].value_uint16 = value; break;
                                case VAR_INT16: entity.variables[v].value_int16 = value; break;
                                case VAR_UINT32: entity.variables[v].value_uint32 = value; break;
                                case VAR_INT32: entity.variables[v].value_int32 = value; break;
                                case VAR_ENUM: entity.variables[v].value_enum = value; break;
                                case VAR_BOOL: entity.variables[v].value_bool = value != 0; break;
                            }
                        }

                        entity.variables[v].type = obj.variables[v].type;
                    }
                    else { // otherwise we'll need to add new vars
                        RSDKv5::Scene::VariableValue variable;
                        variable.type = obj.variables[v].type;
                        if (obj.variables[v].name == "filter")
                            variable.value_uint8 = 0xFF;
                        entity.variables.insert(v, variable);
                    }
                }
            }
            else { // remove & update
                // remove excess variables
                for (int v = entity.variables.count(); v >= obj.variables.count(); --v) {
                    entity.variables.removeAt(v);
                }
                // update remaining variables
                for (int v = 0; v < obj.variables.count(); ++v) {
                    // if we can transfer values, we should
                    if (entity.variables[v].type <= VAR_BOOL && obj.variables[v].type <= VAR_BOOL) {
                        int value = 0;
                        switch (entity.variables[v].type) {
                            case VAR_UINT8: value = entity.variables[v].value_uint8; break;
                            case VAR_INT8: value = entity.variables[v].value_int8; break;
                            case VAR_UINT16: value = entity.variables[v].value_uint16; break;
                            case VAR_INT16: value = entity.variables[v].value_int16; break;
                            case VAR_UINT32: value = entity.variables[v].value_uint32; break;
                            case VAR_INT32: value = entity.variables[v].value_int32; break;
                            case VAR_ENUM: value = entity.variables[v].value_enum; break;
                            case VAR_BOOL: value = entity.variables[v].value_bool ? 1 : 0; break;
                        }

                        switch (obj.variables[v].type) {
                            case VAR_UINT8: entity.variables[v].value_uint8 = value; break;
                            case VAR_INT8: entity.variables[v].value_int8 = value; break;
                            case VAR_UINT16: entity.variables[v].value_uint16 = value; break;
                            case VAR_INT16: entity.variables[v].value_int16 = value; break;
                            case VAR_UINT32: entity.variables[v].value_uint32 = value; break;
                            case VAR_INT32: entity.variables[v].value_int32 = value; break;
                            case VAR_ENUM: entity.variables[v].value_enum = value; break;
                            case VAR_BOOL: entity.variables[v].value_bool = value != 0; break;
                        }
                    }

                    entity.variables[v].type = obj.variables[v].type;
                }
            }
        }
    }

    for (int i = 0; i < viewer->objects.count() && viewer->gameLinks.count(); ++i) {
        viewer->callGameEvent(viewer->GetObjectInfo(viewer->objects[i].name),
                              SceneViewerv5::EVENT_SERIALIZE, NULL);

        for (int v = viewer->objects[i].variables.count() - 1; v >= 0; --v) {
            // check if var no longer exists
            if (viewer->objects[i].variables[v].offset == -1) {
                for (auto &entity : viewer->entities) {
                    if (entity.type == i)
                        entity.variables.removeAt(v);
                }

                viewer->objects[i].variables.removeAt(v);
            }
        }
    }

    // EditorLoad should have all the info before being called
    for (int i = 0; i < viewer->objects.count() && viewer->gameLinks.count(); ++i) {
        viewer->callGameEvent(viewer->GetObjectInfo(viewer->objects[i].name), SceneViewerv5::EVENT_LOAD,
                              NULL);
    }

    TypeGroupList list = viewer->typeGroups[0];
    for (int i = 0; i < viewer->entities.count() && viewer->gameLinks.count(); ++i) {
        SceneEntity *entity = &viewer->entities[i];
        viewer->callGameEvent(viewer->GetObjectInfo(viewer->objects[entity->type].name),
                              SceneViewerv5::EVENT_CREATE, entity);
    }

    ui->addEnt->setDisabled(viewer->activeEntityCount() >= 0x800);

    tabTitle = Utils::getFilenameAndFolder(scnPath);

    clearActions();
    appConfig.addRecentFile(ENGINE_v5, TOOL_SCENEEDITOR, scnPath, QList<QString>{ gcfPath });
    setStatus("Loaded Scene: " + QFileInfo(scnPath).fileName());

    viewer->startTimer();
}
void SceneEditorv5::createEntityList()
{
    ui->entityList->blockSignals(true);
    ui->entityList->clear();

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

void SceneEditorv5::parseGameXML(QString path)
{
    viewer->gameConfig          = RSDKv5::GameConfig();
    viewer->gameConfig.filePath = path;

    Reader fileReader = Reader(path);
    QByteArray bytes  = fileReader.readByteArray(fileReader.filesize, false);

    QXmlStreamReader xmlReader = QXmlStreamReader(bytes);

    int list        = -1;
    bool objectFlag = false;
    bool listFlag   = false;
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
            const QStringRef name = xmlReader.name();
            if (name == "objects") {
                objectFlag ^= 1;
            }
            else if (objectFlag && name == "object") {
                QString objName = "";
                foreach (const QXmlStreamAttribute &attr, xmlReader.attributes()) {
                    if (attr.name().toString() == QLatin1String("name")) {
                        objName = attr.value().toString();
                    }
                }
                viewer->gameConfig.objects.append(objName);
            }
            else if (name == "categories") {
                listFlag ^= 1;
            }
            else if (listFlag && name == "category") {
                QString listName = "";
                foreach (const QXmlStreamAttribute &attr, xmlReader.attributes()) {
                    if (attr.name().toString() == QLatin1String("name")) {
                        listName = attr.value().toString();
                    }
                }
                RSDKv5::GameConfig::Category stageList;
                stageList.name = listName;
                list           = viewer->gameConfig.categories.count();
                viewer->gameConfig.categories.append(stageList);
            }
            else if (list != -1 && name == "stage") {
                QString stgName   = "";
                QString stgFolder = "";
                QString stgID     = "";
                int stgFilter     = 0;
                foreach (const QXmlStreamAttribute &attr, xmlReader.attributes()) {
                    if (attr.name().toString() == QLatin1String("name")) {
                        stgName = attr.value().toString();
                    }
                    if (attr.name().toString() == QLatin1String("folder")) {
                        stgFolder = attr.value().toString();
                    }
                    if (attr.name().toString() == QLatin1String("id")) {
                        stgID = attr.value().toString();
                    }
                    if (attr.name().toString() == QLatin1String("filter")) {
                        stgFilter = attr.value().toInt();
                    }
                }
                RSDKv5::GameConfig::SceneInfo stage;
                stage.name   = stgName;
                stage.folder = stgFolder;
                stage.id     = stgID;
                stage.filter = stgFilter;
                viewer->gameConfig.categories[list].scenes.append(stage);
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

void SceneEditorv5::undoAction()
{
    if (actionIndex > 0) {
        actionIndex--;
        resetAction();
    }
}
void SceneEditorv5::redoAction()
{
    if (actionIndex + 1 < actions.count()) {
        actionIndex++;
        resetAction();
    }
}

void SceneEditorv5::resetAction()
{
    viewer->tilePalette = actions[actionIndex].tilePalette;
    viewer->tiles       = actions[actionIndex].tiles;

    viewer->gameConfig = actions[actionIndex].gameConfig;

    viewer->scene       = actions[actionIndex].scene;
    viewer->stageConfig = actions[actionIndex].stageConfig;

    viewer->tileconfig = actions[actionIndex].tileconfig;

    // Objects & Entities
    viewer->objects  = actions[actionIndex].objects;
    viewer->entities = actions[actionIndex].entities;

    // General Editing
    viewer->curTool     = actions[actionIndex].curTool;
    viewer->isSelecting = actions[actionIndex].selecting;
    viewer->mousePos    = actions[actionIndex].mousePos;

    // Layer Editing
    viewer->tilePos       = actions[actionIndex].tilePos;
    viewer->tileFlip      = actions[actionIndex].tileFlip;
    viewer->selectedLayer = actions[actionIndex].selectedLayer;

    // Collision
    viewer->showPlaneA = actions[actionIndex].showPlaneA;
    viewer->showPlaneB = actions[actionIndex].showPlaneB;

    // Entity Editing
    viewer->selectedObject    = actions[actionIndex].selectedObject; // placing
    viewer->selectedEntity    = actions[actionIndex].selectedEntity; // viewing
    viewer->sceneInfo.listPos = viewer->selectedEntity;

    // Parallax Editing
    viewer->showParallax       = actions[actionIndex].showParallax;
    viewer->selectedScrollInfo = actions[actionIndex].selectedScrollInfo;

    // Camera
    viewer->showPixelGrid = actions[actionIndex].showPixelGrid;
    viewer->showTileGrid  = actions[actionIndex].showTileGrid;

    // updating UI

    ui->layerList->blockSignals(true);
    ui->layerList->clear();
    for (int l = 0; l < viewer->scene.layers.count(); ++l) {
        QListWidgetItem *item = new QListWidgetItem(viewer->scene.layers[l].name, ui->layerList);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(viewer->scene.layers[l].visible ? Qt::Checked : Qt::Unchecked);
    }
    ui->layerList->blockSignals(false);

    ui->objectList->blockSignals(true);
    ui->objectList->clear();
    for (int o = 0; o < viewer->objects.count(); ++o) {
        QListWidgetItem *item = new QListWidgetItem(viewer->objects[o].name, ui->objectList);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Checked);
    }
    ui->objectList->blockSignals(false);

    createEntityList();

    ui->showTileGrid->blockSignals(true);
    ui->showTileGrid->setDown(viewer->showTileGrid);
    ui->showTileGrid->blockSignals(false);

    // ui->showPixelGrid->blockSignals(true);
    // ui->showPixelGrid->setDown(viewer->showPixelGrid);
    // ui->showPixelGrid->blockSignals(false);

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

void SceneEditorv5::doAction(QString name, bool setModified)
{
    ActionState action;

    action.name = name;

    action.tilePalette = viewer->tilePalette;
    action.tiles       = viewer->tiles;

    action.gameConfig = viewer->gameConfig;

    action.scene       = viewer->scene;
    action.stageConfig = viewer->stageConfig;
    action.tileconfig  = viewer->tileconfig;

    // Objects & Entities
    action.objects  = viewer->objects;
    action.entities = viewer->entities;

    // General Editing
    action.curTool   = viewer->curTool;
    action.selecting = viewer->isSelecting;
    action.mousePos  = viewer->mousePos;

    // Layer Editing
    action.tilePos       = viewer->tilePos;
    action.tileFlip      = viewer->tileFlip;
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

    // Actions
    for (int i = actions.count() - 1; i > actionIndex; --i) {
        actions.removeAt(i);
    }

    actions.append(action);
    actionIndex = actions.count() - 1;

    updateTitle(setModified);

    setStatus("Did Action: " + name);
}
void SceneEditorv5::clearActions()
{
    actions.clear();
    actionIndex = 0;
    doAction("Action Setup", false); // first action, cant be undone
}

#include "moc_sceneeditorv5.cpp"
