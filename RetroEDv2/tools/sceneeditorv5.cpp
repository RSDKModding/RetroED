#include "includes.hpp"
#include "ui_sceneeditorv5.h"
#include "qgifimage.h"

#include "sceneeditorv5.hpp"

#include "sceneproperties/sceneincludesv5.hpp"
#include "sceneproperties/scenepropertiesv5.hpp"
#include "sceneproperties/scenelayerpropertiesv5.hpp"
#include "sceneproperties/sceneobjectpropertiesv5.hpp"
#include "sceneproperties/scenescrollpropertiesv5.hpp"
#include "sceneproperties/scenetilepropertiesv5.hpp"
#include "sceneproperties/scenestamppropertiesv5.hpp"
#include "sceneproperties/objectselector.hpp"
#include "sceneproperties/tilereplaceoptions.hpp"
#include "sceneproperties/gotopos.hpp"
#include "sceneproperties/createscene.hpp"

#include "sceneproperties/stageconfigeditorv5.hpp"

#include "paletteeditor.hpp"
#include "paletteeditor/colourdialog.hpp"

#include "sceneproperties/tileseteditor.hpp"

#include "sceneproperties/confirmgamelink.hpp"

TileSelector::TileSelector(QWidget *parent) : QWidget(parent), parentPtr((SceneEditorv5 *)parent)
{
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scrollArea->setGeometry(10, 10, 200, 200);

    QWidget *tileArea = new QWidget();

    QGridLayout *tileLayout = new QGridLayout();
    tileLayout->setContentsMargins(0, 0, 0, 0);
    tileLayout->setSpacing(1);

    int i = 0;
    int x = 0, y = 0;
    for (auto &&tile : parentPtr->viewer->tiles) {
        TileLabel *label = new TileLabel(&parentPtr->viewer->selectedTile, i, tileArea);
        label->setPixmap(QPixmap::fromImage(tile).scaled(tile.width() * 2, tile.height() * 2));
        label->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        label->resize(tile.width(), tile.height());
        tileLayout->addWidget(label, y, x);
        x++;
        if (x >= 4) {
            x = 0;
            y += 1;
        }
        label->setFixedSize(tile.width() * 2, tile.height() * 2);
        connect(label, &TileLabel::requestRepaint, tileArea, QOverload<>::of(&QWidget::update));
        connect(label, &TileLabel::requestRepaint, parentPtr, QOverload<>::of(&SceneEditorv5::updateTileSel));
        tiles[i++] = label;
    }

    tileArea->setLayout(tileLayout);
    scrollArea->setWidget(tileArea);
    QVBoxLayout *l = new QVBoxLayout(this);
    l->addWidget(scrollArea);
    setLayout(l);
}

void TileSelector::RefreshList()
{
    int t = 0;
    for (auto &&tile : parentPtr->viewer->tiles) {
        tiles[t++]->setPixmap(QPixmap::fromImage(tile).scaled(tile.width() * 2, tile.height() * 2));
    }
}

SceneEditorv5::SceneEditorv5(QWidget *parent) : QWidget(parent), ui(new Ui::SceneEditorv5)
{
    ui->setupUi(this);

    viewer = new SceneViewer(ENGINE_v5, this);
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

    stampProp = new SceneStampPropertiesv5(this);
    stampProp->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->stampPropFrame->layout()->addWidget(stampProp);
    stampProp->show();

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
    ui->tileListFrame->layout()->addWidget(tileSel);
    tileSel->show();

    ResetTools(SceneViewer::TOOL_MOUSE);

    viewer->statusLabel = ui->statusLabel;
    viewer->objProp     = objProp;

    ui->showParallax->setIcon(Utils::getColoredIcon(":/icons/ic_horizontal_split_48px.svg"));
    ui->showTileGrid->setIcon(Utils::getColoredIcon(":/icons/ic_grid_48px.svg"));

    scnProp->gridX->setValue(viewer->gridSize.x);
    scnProp->gridY->setValue(viewer->gridSize.y);

    connect(viewer, &SceneViewer::callGameEventv5,
            [this](QString objName, byte eventID, SceneEntity *entity) {
                CallGameEvent(objName, eventID, entity);
            });

    connect(ui->tileFlipX, &QCheckBox::toggled, [this](bool c) { viewer->tileFlip.x = c; });
    connect(ui->tileFlipY, &QCheckBox::toggled, [this](bool c) { viewer->tileFlip.y = c; });
    connect(ui->tileSolidTopA, &QCheckBox::toggled, [this](bool c) { viewer->tileSolidA.x = c; });
    connect(ui->tileSolidLRBA, &QCheckBox::toggled, [this](bool c) { viewer->tileSolidA.y = c; });
    connect(ui->tileSolidTopB, &QCheckBox::toggled, [this](bool c) { viewer->tileSolidB.x = c; });
    connect(ui->tileSolidLRBB, &QCheckBox::toggled, [this](bool c) { viewer->tileSolidB.y = c; });

    ui->toolBox->setCurrentIndex(0);
    ui->propertiesBox->setCurrentIndex(0);

    connect(ui->horizontalScrollBar, &QScrollBar::valueChanged, [this](int v) {
        viewer->cameraPos.x         = v;
        viewer->screens->position.x = v;
    });

    connect(ui->verticalScrollBar, &QScrollBar::valueChanged, [this](int v) {
        viewer->cameraPos.y         = v;
        viewer->screens->position.y = v;
    });


    QCheckBox *filterToggles[] = { ui->filterBox1, ui->filterBox2, ui->filterBox3, ui->filterBox4,
                                   ui->filterBox5, ui->filterBox6, ui->filterBox7, ui->filterBox8 };

    for (int f = 0; f < 8; ++f) {
        filterToggles[f]->setDisabled(viewer->engineRevision == 1);
        filterToggles[f]->setChecked(viewer->engineRevision != 1
                                     || Utils::getBit(viewer->sceneFilter, f));
        connect(filterToggles[f], &QCheckBox::toggled, [this, f] { viewer->sceneFilter ^= (1 << f); });
    }

#ifndef Q_NO_PROCESS
    connect(ui->filterSelection, QOverload<int>::of(&QComboBox::currentIndexChanged),
            [this](int v) {
                if (v < 0)
                    return;
                initGameFilter = (byte)v;
            });

    connect(ui->runGame, &QPushButton::clicked, [this]{
        QString gamePath = appConfig.gameManager[ENGINE_v5].exePath;
        QString dataPath = appConfig.baseDataManager[ENGINE_v5].dataPath;
        if (QFile::exists(gamePath)) {
            QStringList args;
            byte filterVal = 0;
            switch (initGameFilter){
                case 0: filterVal = 255;                 break;
                case 1: filterVal = 3;                   break;
                case 2: filterVal = 5;                   break;
                case 3: filterVal = viewer->sceneFilter; break;
            }
            args << "console=true;" << QString("stage=%1;").arg(argInitStage)
                 << QString("scene=%1;").arg(argInitScene) << QString("filter=%1;").arg(filterVal);
            QProcess proc;
            proc.setProgram(gamePath);
            proc.setWorkingDirectory(QFileInfo(dataPath).absolutePath());
            proc.setArguments(args);
            proc.startDetached();
            proc.waitForStarted();
        }
    });
#else
    ui->runningFilters->setVisible(false);
    ui->filterSelection->setVisible(false);
    ui->runGame->setVisible(false);
#endif

    connect(ui->useGizmos, &QPushButton::clicked, [this] {
        viewer->sceneInfo.effectGizmo ^= 1;
        viewer->sceneInfoV1.effectGizmo = viewer->sceneInfo.effectGizmo;
    });

    connect(ui->layerList, &QListWidget::currentRowChanged, [this](int c) {
        ui->rmLayer->setDisabled(c == -1);
        ui->upLayer->setDisabled(c == -1);
        ui->downLayer->setDisabled(c == -1);

        if (c == -1)
            return;

        ui->addLayer->setDisabled(viewer->layers.count() >= 8);
        ui->rmLayer->setDisabled(viewer->layers.count() < 0);
        ui->upLayer->setDisabled(c == 0);
        ui->downLayer->setDisabled(c == viewer->layers.count() - 1);

        viewer->selectedLayer = c;
        lyrProp->setupUI(viewer, viewer->selectedLayer);

        ui->propertiesBox->setCurrentWidget(ui->layerPropPage);

        bool disabled = true;
        if (c != -1)
            disabled = viewer->layers[c].type == SceneHelpers::TileLayer::LAYER_ROTOZOOM
                       || viewer->layers[c].type == SceneHelpers::TileLayer::LAYER_BASIC;

        CreateScrollList();
        ui->addScr->setDisabled(disabled);
        ui->rmScr->setDisabled(disabled);
        ui->impScr->setDisabled(disabled);
        ui->expScr->setDisabled(disabled);
    });

    connect(ui->addLayer, &QToolButton::clicked, [this] {
        uint c = ui->layerList->currentRow() + 1;
        SceneHelpers::TileLayer layer;
        layer.name = "New Layer " + QString::number(c);
        layer.type = SceneHelpers::TileLayer::LAYER_HSCROLL;
        viewer->layers.append(layer);

        QListWidgetItem *item = new QListWidgetItem(layer.name, ui->layerList);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Checked);

        ui->layerList->setCurrentItem(item);
        DoAction("Add Layer: " + layer.name);

        ui->addLayer->setDisabled(viewer->layers.count() >= 8);
        ui->rmLayer->setDisabled(viewer->layers.count() < 0);
    });

    connect(ui->rmLayer, &QToolButton::clicked, [this] {
        int c = ui->layerList->currentRow();
        int n = ui->layerList->currentRow() == ui->layerList->count() - 1 ? c - 1 : c;
        delete ui->layerList->item(c);
        viewer->layers.removeAt(c);
        ui->layerList->blockSignals(true);
        ui->layerList->setCurrentRow(n);
        ui->layerList->blockSignals(false);

        DoAction("Remove Layer: " + QString::number(c));

        ui->addLayer->setDisabled(viewer->layers.count() >= 8);
        ui->rmLayer->setDisabled(viewer->layers.count() < 0);
    });

    connect(ui->upLayer, &QToolButton::clicked, [this] {
        uint c     = ui->layerList->currentRow();
        auto *item = ui->layerList->takeItem(c);
        viewer->layers.move(c, c - 1);
        ui->layerList->insertItem(c - 1, item);
        ui->layerList->setCurrentRow(c - 1);
    });

    connect(ui->downLayer, &QToolButton::clicked, [this] {
        uint c     = ui->layerList->currentRow();
        auto *item = ui->layerList->takeItem(c);
        viewer->layers.move(c, c + 1);
        ui->layerList->insertItem(c + 1, item);
        ui->layerList->setCurrentRow(c + 1);
    });

    connect(ui->layerList, &QListWidget::itemChanged, [this](QListWidgetItem *item) {
        int c = ui->layerList->row(item);
        if ((uint)c < (uint)viewer->layers.count())
            viewer->layers[c].visible = item->checkState() == Qt::Checked;
    });

    connect(tileProp, &SceneTilePropertiesv5::updateTileFlags, [this](bool c, byte pos){
        Utils::setBit(viewer->selectedTile, c, pos);
    });
    connect(tileProp, &SceneTilePropertiesv5::updateTileColMap, viewer, &SceneViewer::updateTileColMap);

    connect(ui->stampList, &QListWidget::currentRowChanged, [this](int c) {
        ui->rmStp->setDisabled(c == -1);

        if (c == -1 || c >= viewer->stamps.stampList.count())
            return;

        viewer->selectedStamp = c;
        stampProp->setupUI(&viewer->stamps, c);
        ui->propertiesBox->setCurrentWidget(ui->stampPropPage);
    });

    connect(ui->addStp, &QToolButton::clicked, [this] {
        uint c = ui->stampList->currentRow() + 1;
        RSDKv5::Stamps::StampEntry stamp;
        stamp.name = "New Stamp";
        viewer->stamps.stampList.insert(c, stamp);

        ui->stampList->blockSignals(true);

        auto *item = new QListWidgetItem();
        item->setText("New Stamp");
        ui->stampList->addItem(item);
        item->setFlags(item->flags());

        ui->stampList->blockSignals(false);
        ui->stampList->setCurrentItem(item);
        // DoAction("Add Stamp: " + stamp.name);
    });

    connect(ui->rmStp, &QToolButton::clicked, [this] {
        int c        = ui->stampList->currentRow();
        int n        = ui->stampList->currentRow() == ui->objectList->count() - 1 ? c : c - 1;
        QString name = viewer->stamps.stampList.at(c).name;
        delete ui->stampList->item(c);
        viewer->stamps.stampList.removeAt(c);

        ui->stampList->setCurrentRow(n);
        viewer->selectedStamp = n;

        // DoAction("Remove Stamp: " + name);
    });

    connect(lyrProp, &SceneLayerPropertiesv5::layerNameChanged, this, &SceneEditorv5::updateLayerName);
    connect(stampProp, &SceneStampPropertiesv5::stampNameChanged, this, &SceneEditorv5::updateStampName);

    // MAKE SURE YOU ADD YOUR OBJECT TO THE VIEWER'S LIST BEFORE CALLING THIS
    auto linkGameObject = [this](int objectID, GameObjectInfo *info, bool useLoadEvent = true,
                                 bool updateTypes = true) {
        if (!info)
            return;

        if (updateTypes) {
            switch (viewer->engineRevision) {
                case 1:
                    for (auto &e : viewer->entities) {
                        if (e.type >= objectID) {
                            e.type++;
                            AS_ENTITY(e.gameEntity, GameEntityv1)->classID = e.type;
                        }
                    }
                    break;

                case 2:
                    for (auto &e : viewer->entities) {
                        if (e.type >= objectID) {
                            e.type++;
                            AS_ENTITY(e.gameEntity, GameEntityv2)->classID = e.type;
                        }
                    }
                    break;

                default:
                case 3:
                    for (auto &e : viewer->entities) {
                        if (e.type >= objectID) {
                            e.type++;
                            AS_ENTITY(e.gameEntity, GameEntityvU)->classID = e.type;
                        }
                    }
                    break;
            }

            for (int i = 0; i < viewer->objects.count(); ++i) {
                GameObjectInfo *info = GetObjectInfo(viewer->objects[i].name);
                if (info && info->sVars) {
                    GameObject *sVars = *info->sVars;
                    if (sVars) {
                        if (sVars->objectID >= objectID) {
                            sVars->objectID++;
                        }
                    }
                }
            }
        }
        allocateStorage(v5Editor->dataStorage, info->staticClassSize, (void **)info->sVars, DATASET_STG,
                        true);

        if (info->sVars && *info->sVars) {
            CallGameEvent(info->name, SceneViewer::EVENT_STATICLOAD, NULL);

            GameObject *sVars = *info->sVars;
            sVars->objectID   = objectID;
            sVars->active     = ACTIVE_NORMAL;
        }

        if (gameConfig.readFilter) {
            switch (viewer->engineRevision) {
                case 1: break;

                case 2:
                    FunctionTable::SetEditableVar(VAR_UINT8, "filter", objectID,
                                                  offsetof(GameEntityv2, filter));
                    break;

                default:
                case 3:
                    FunctionTable::SetEditableVar(VAR_UINT8, "filter", objectID,
                                                  offsetof(GameEntityvU, filter));
                    break;
            }
        }

        CallGameEvent(info->name, SceneViewer::EVENT_SERIALIZE, NULL);


        if (useLoadEvent){
            // idk why do i need to call every object again but otherwise references to the added object are ignored
            // (Fixes LSelect visual bugs)
            for (int i = 0; i <= objectID; ++i)
                CallGameEvent(viewer->objects[i].name, SceneViewer::EVENT_LOAD, NULL);
        }
    };

    auto unlinkGameObject = [this](int objectID, GameObjectInfo *info) {

        for (int o = viewer->entities.count() - 1; o >= 0; --o) {
            SceneEntity &e = viewer->entities[o];

            if (e.type > objectID) {
                e.type--;
                if (e.gameEntity){
                    switch (viewer->engineRevision) {
                        case 1: AS_ENTITY(e.gameEntity, GameEntityv1)->classID = e.type; break;

                        case 2: AS_ENTITY(e.gameEntity, GameEntityv2)->classID = e.type; break;

                        default:
                        case 3: AS_ENTITY(e.gameEntity, GameEntityvU)->classID = e.type; break;
                    }
                }
            }
            else if (e.type == objectID) {
                DeleteEntity(o, false);
            }
        }
        objProp->unsetUI();
        CreateEntityList();
        viewer->objects.removeAt(objectID);

        for (int i = 0; i < viewer->objects.count(); ++i) {
            GameObjectInfo *info = GetObjectInfo(viewer->objects[i].name);
            if (info && info->sVars) {
                GameObject *sVars = *info->sVars;
                if (sVars) {
                    if (sVars->objectID >= objectID) {
                        sVars->objectID--;
                    }
                }
            }
        }
        if (!info)
            return;

        if (info->sVars && *info->sVars)
            *info->sVars = NULL;
    };

    auto reSyncGameObject = [this, linkGameObject](bool syncGC, int oldListCount) {

        QList<QString> oldObjs;
        QList<SceneObject> oldSceneObjs;
        QList<int> removeFlags;
        int objOffset = 0;
        int id;
        if (syncGC == false)
            objOffset = stageConfig.loadGlobalObjects ? gameConfig.objects.count() : 0;

        // get info about old objs
        for (int i = objOffset; i <= oldListCount; ++i) {
            oldObjs.append(viewer->objects[i].name);
            oldSceneObjs.append(viewer->objects[i]);
            removeFlags.append(-1);
        }

        removeFlags[0] = 0; // keep blank obj

        if (syncGC){
            // add stage object IDs
            int idChange = gameConfig.objects.count() - oldListCount;
            for (int i = oldListCount + 1; i < viewer->objects.count(); ++i) {
                removeFlags.append(i + idChange);
            }

            // remove old global objs
            for (int i = oldListCount; i > 0; --i) {
                viewer->objects.removeAt(i);
            }

            id = 1;
            for (QString &global : gameConfig.objects) {
                int index = oldObjs.indexOf(global);
                if (index < 0)
                    index = oldObjs.indexOf(Utils::getMd5HashString(global));

                SceneObject objInfo;
                objInfo.name = global;

                if (index >= 0) {
                    removeFlags[index] = id;
                    objInfo.variables.clear();
                    for (auto &var : oldSceneObjs[index].variables) {
                        var.offset = -1;
                        objInfo.variables.append(var);
                    }
                }

                viewer->objects.insert(id, objInfo);

                ++id;
            }

        } else {

            // remove old stage objs
            for (int i = oldListCount; i > objOffset; --i) {
                viewer->objects.removeAt(i);
            }

            id = objOffset + 1;
            for (QString &stageObj : stageConfig.objects) {
                int index = oldObjs.indexOf(stageObj);
                if (index < 0)
                    index = oldObjs.indexOf(Utils::getMd5HashString(stageObj));

                SceneObject objInfo;
                objInfo.name = stageObj;

                if (index >= 0) {
                    removeFlags[index] = id;
                    objInfo.variables.clear();
                    for (auto &var : oldSceneObjs[index].variables) {
                        var.offset = -1;
                        objInfo.variables.append(var);
                    }
                }

                viewer->objects.insert(id, objInfo);

                ++id;
            }
        }
        // start by removing objects we dont need
        for (int o = viewer->entities.count() - 1; o >= 0; --o) {
            SceneEntity &entity = viewer->entities[o];
            if (entity.type >= objOffset){
                int newType = removeFlags[entity.type - objOffset];
                if (newType < 0) {
                    DeleteEntity(o, false);
                }
                else {
                    entity.type = newType;
                    if (entity.gameEntity){
                        switch (viewer->engineRevision) {
                            case 1: AS_ENTITY(entity.gameEntity, GameEntityv1)->classID = newType; break;
                            case 2: AS_ENTITY(entity.gameEntity, GameEntityv2)->classID = newType; break;
                            default:
                            case 3: AS_ENTITY(entity.gameEntity, GameEntityvU)->classID = newType; break;
                        }
                    }
                }
            }
        }

        for (int i = objOffset; i < viewer->objects.count(); ++i) {
            GameObjectInfo *info = GetObjectInfo(viewer->objects[i].name);
            if (info && info->sVars) {
                GameObject *sVars = *info->sVars;
                if (sVars) {
                    sVars->objectID = i;
                }
            }
        }

        int maxObjCount;
        if (syncGC)
            maxObjCount = gameConfig.objects.count() - 1 + stageConfig.objects.count();
        else
            maxObjCount = objOffset + stageConfig.objects.count();

        // load assets for new objects
        for (int i = objOffset; i <= maxObjCount; ++i) {
            linkGameObject(i, GetObjectInfo(viewer->objects[i].name), true, false);
            // clean up and remove unused vars
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
    };

    connect(ui->objectFilter, &QLineEdit::textChanged, [this](QString s) { FilterObjectList(s.toUpper()); });

    connect(ui->objectList, &QListWidget::currentRowChanged, [this](int c) {
        ui->rmObj->setDisabled(c == -1 || c >= viewer->objects.count());

        if (c == -1 || c >= viewer->objects.count())
            return;

        bool global = c == 0;
        if (stageConfig.loadGlobalObjects) {
            for (auto &obj : gameConfig.objects) {
                if (viewer->objects[c].name == obj) {
                    global = true;
                    break;
                }
            }
        }

        viewer->selectedObject = c;

        switch (viewer->engineRevision) {
            case 1:
                memset((void *)&createGameEntityv1, 0, sizeof(GameEntityBasev1));
                createGameEntityv1.position.x = 0;
                createGameEntityv1.position.y = 0;
                createGameEntityv1.classID    = viewer->selectedObject;

                createTempEntity.gameEntity = &createGameEntityv1;
                break;

            case 2:
                memset((void *)&createGameEntityv2, 0, sizeof(GameEntityBasev2));
                createGameEntityv2.position.x = 0;
                createGameEntityv2.position.y = 0;
                createGameEntityv2.classID    = viewer->selectedObject;

                createTempEntity.gameEntity = &createGameEntityv2;
                break;

            default:
            case 3:
                memset((void *)&createGameEntityvU, 0, sizeof(GameEntityBasevU));
                createGameEntityvU.position.x = 0;
                createGameEntityvU.position.y = 0;
                createGameEntityvU.classID    = viewer->selectedObject;

                createTempEntity.gameEntity = &createGameEntityvU;
                break;
        }

        createTempEntity.type   = viewer->selectedObject;
        createTempEntity.pos.x  = 0;
        createTempEntity.pos.y  = 0;
        createTempEntity.slotID = 0xFFFF;
        createTempEntity.box    = Rect<int>(0, 0, 0, 0);

        viewer->activeDrawEntity = &createTempEntity;
        CallGameEvent(viewer->objects[viewer->selectedObject].name, SceneViewer::EVENT_CREATE,
                      &createTempEntity);

        ui->rmObj->setDisabled(c == -1 || global);
    });

    connect(ui->objectList, &QListWidget::itemChanged, [this](QListWidgetItem *item) {
        int c = ui->objectList->row(item);
        if ((uint)c < (uint)viewer->objects.count())
            viewer->objects[c].visible = item->checkState() == Qt::Checked;
    });

    connect(ui->addObj, &QToolButton::clicked, [this, linkGameObject] {
        QList<QString> list;
        for (int o = 0; o < viewer->objects.count(); ++o) {
            list.append(viewer->objects[o].name);
        }
        QList<GameObjectInfo> objList;
        for (auto &link : gameLinks) {
            for (int o = 0; o < link.gameObjectList.count(); ++o) {
                objList.append(link.gameObjectList[o]);
            }
        }

        ObjectSelectorv5 *selector = new ObjectSelectorv5(list, objList);
        if (selector->exec() == QDialog::Accepted) {
            // uint c          = ui->objectList->currentRow() + 1;
            // int globalCount = 1;
            // if (stageConfig.loadGlobalObjects)
            //     globalCount = gameConfig.objects.count() + 1;
            //
            // if (c < (uint)globalCount)
            //     c = globalCount;

            for (int i = 0; i < selector->objAddList.count(); ++i) {
                if (selector->objAddList[i]) {
                    SceneObject obj;
                    auto &objInfo = objList[selector->objIDList[i]];

                    obj.name = objInfo.name;

                    viewer->objects.append(obj);
                    stageConfig.objects.append(obj.name);

                    auto *item = new QListWidgetItem();
                    item->setText(obj.name);
                    item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
                    item->setCheckState(Qt::Checked);
                    ui->objectList->addItem(item);

                    linkGameObject(viewer->objects.count() - 1, &objInfo);
                }
            }

            DoAction("Add Object" + QString(selector->objAddList.count() ? "s" : ""));

            CreateEntityList();
        }
    });

    connect(ui->rmObj, &QToolButton::clicked, [this, unlinkGameObject] {
        int c = ui->objectList->currentRow();
        int n = ui->objectList->currentRow() == ui->objectList->count() - 1 ? c - 1 : c;
        delete ui->objectList->item(c);

        unlinkGameObject(c, GetObjectInfo(viewer->objects[c].name));

        int globalCount = 1;
        if (stageConfig.loadGlobalObjects)
            globalCount = gameConfig.objects.count() + 1;
        stageConfig.objects.removeAt(c - globalCount);

        ui->objectList->blockSignals(true);
        ui->objectList->setCurrentRow(n);
        ui->objectList->blockSignals(false);

        CreateEntityList(c);
        DoAction("Remove Object: " + QString::number(c));
    });

    connect(ui->entityFilter, &QLineEdit::textChanged, [this](QString s) { FilterEntityList(s.toUpper()); });

    connect(ui->entityList, &QListWidget::currentRowChanged, [this](int c) {
        ui->upEnt->setDisabled(c == -1);
        ui->downEnt->setDisabled(c == -1);
        ui->rmEnt->setDisabled(c == -1);

        if (c == -1)
            return;

        ui->downEnt->setDisabled(c == viewer->entities.count() - 1);
        ui->upEnt->setDisabled(c == 0);

        viewer->selectedEntity      = c;
        viewer->sceneInfo.listPos   = viewer->entities[viewer->selectedEntity].slotID;
        viewer->sceneInfoV1.listPos = viewer->sceneInfo.listPos;

        viewer->cameraPos.x = viewer->entities[c].pos.x - ((viewer->storedW / 2) * viewer->invZoom());
        viewer->cameraPos.y = viewer->entities[c].pos.y - ((viewer->storedH / 2) * viewer->invZoom());

        viewer->screens->position.x = viewer->cameraPos.x;
        viewer->screens->position.y = viewer->cameraPos.y;

        ui->horizontalScrollBar->blockSignals(true);
        ui->horizontalScrollBar->setValue(viewer->cameraPos.x);
        ui->horizontalScrollBar->blockSignals(false);

        ui->verticalScrollBar->blockSignals(true);
        ui->verticalScrollBar->setValue(viewer->cameraPos.y);
        ui->verticalScrollBar->blockSignals(false);

        // THIS IS REALLY *REALLY* SLOW, TODO: FIX/SPEED UP
        objProp->setupUI(&viewer->entities[viewer->selectedEntity]);
        ui->propertiesBox->setCurrentWidget(ui->objPropPage);
    });
    connect(objProp, &SceneObjectPropertiesv5::typeChanged, this, &SceneEditorv5::updateType);

    connect(ui->addEnt, &QToolButton::clicked, [this] {
        uint c = viewer->entities.count();
        uint entType = (viewer->selectedObject > -1 ? viewer->selectedObject : 0);

        int xpos = viewer->cameraPos.x + ((viewer->storedW / 2) * viewer->invZoom());
        int ypos = viewer->cameraPos.y + ((viewer->storedH / 2) * viewer->invZoom());

        AddEntity(entType, xpos, ypos);

        ui->entityList->setCurrentRow(c);
        viewer->selectedEntity = c;

        ui->addEnt->setDisabled(viewer->activeEntityCount() >= SCENEENTITY_COUNT_v5);
        DoAction("Add Entity: " + QString::number(viewer->entities.count() - 1));
    });

    connect(ui->rmEnt, &QToolButton::clicked, [this] {
        int c = ui->entityList->currentRow();
        int n = ui->entityList->currentRow() == ui->entityList->count() - 1 ? c - 1 : c;

        DeleteEntity(c);

        ui->entityList->blockSignals(true);
        ui->entityList->setCurrentRow(n);
        ui->entityList->blockSignals(false);

        viewer->selectedEntity = n;

        if (n != -1){
            viewer->cameraPos.x = viewer->entities[n].pos.x - ((viewer->storedW / 2) * viewer->invZoom());
            viewer->cameraPos.y = viewer->entities[n].pos.y - ((viewer->storedH / 2) * viewer->invZoom());
            objProp->setupUI(&viewer->entities[n]);
        }

        ui->horizontalScrollBar->blockSignals(true);
        ui->horizontalScrollBar->setValue(viewer->cameraPos.x);
        ui->horizontalScrollBar->blockSignals(false);

        ui->verticalScrollBar->blockSignals(true);
        ui->verticalScrollBar->setValue(viewer->cameraPos.y);
        ui->verticalScrollBar->blockSignals(false);

        ui->rmEnt->setDisabled(viewer->entities.count() <= 0);
        ui->addEnt->setDisabled(viewer->activeEntityCount() >= 0x800);
        DoAction("Remove Entity: " + QString::number(c));
    });

    connect(ui->upEnt, &QToolButton::clicked, [this] {
        ui->entityList->blockSignals(true);
        uint c     = ui->entityList->currentRow();
        auto *item = ui->entityList->takeItem(c);

        int slot                       = viewer->entities[c].slotID;
        viewer->entities[c].slotID     = viewer->entities[c - 1].slotID;
        viewer->entities[c].prevSlot   = viewer->entities[c].slotID;

        viewer->entities[c - 1].slotID   = slot;
        viewer->entities[c - 1].prevSlot = slot;

        viewer->entities.move(c, c - 1);

        ui->entityList->insertItem(c - 1, item);
        ui->entityList->setCurrentRow(c - 1);
        ui->entityList->blockSignals(false);

        ui->entityList->item(c)->setText(QString::number(viewer->entities[c].slotID) + ": "
                                         + viewer->objects[viewer->entities[c].type].name);

        ui->entityList->item(c - 1)->setText(QString::number(viewer->entities[c - 1].slotID) + ": "
                                             + viewer->objects[viewer->entities[c - 1].type].name);

        if (viewer->selectedEntity == (int)c)
            viewer->selectedEntity = c - 1;

        for (int s = 0; s < viewer->selectedEntities.count(); ++s) {
            if (viewer->selectedEntities[s] == (int)c)
                viewer->selectedEntities[s] = c - 1;
        }
        objProp->updateUI();
        ui->upEnt->setDisabled(ui->entityList->currentRow() == 0);
        ui->downEnt->setDisabled(false);
    });

    connect(ui->downEnt, &QToolButton::clicked, [this] {
        ui->entityList->blockSignals(true);
        uint c     = ui->entityList->currentRow();
        auto *item = ui->entityList->takeItem(c);

        int slot                       = viewer->entities[c].slotID;
        viewer->entities[c].slotID     = viewer->entities[c + 1].slotID;
        viewer->entities[c].prevSlot   = viewer->entities[c].slotID;

        viewer->entities[c + 1].slotID   = slot;
        viewer->entities[c + 1].prevSlot = slot;

        viewer->entities.move(c, c + 1);

        ui->entityList->insertItem(c + 1, item);
        ui->entityList->setCurrentRow(c + 1);
        ui->entityList->blockSignals(false);

        ui->entityList->item(c)->setText(QString::number(viewer->entities[c].slotID) + ": "
                                         + viewer->objects[viewer->entities[c].type].name);

        ui->entityList->item(c + 1)->setText(QString::number(viewer->entities[c + 1].slotID) + ": "
                                             + viewer->objects[viewer->entities[c + 1].type].name);

        if (viewer->selectedEntity == (int)c)
            viewer->selectedEntity = c + 1;

        for (int s = 0; s < viewer->selectedEntities.count(); ++s) {
            if (viewer->selectedEntities[s] == (int)c)
                viewer->selectedEntities[s] = c + 1;
        }
        objProp->updateUI();
        ui->downEnt->setDisabled(ui->entityList->currentRow() + 1 >= ui->entityList->count());
        ui->upEnt->setDisabled(false);
    });

    connect(ui->scrollList, &QListWidget::currentRowChanged, [this](int c) {
        ui->rmScr->setDisabled(c == -1);

        if (c == -1)
            return;

        viewer->selectedHScrollInfo = c;

        scrProp->setupUI(&viewer->layers[viewer->selectedLayer].scrollInfos[c]);
        ui->propertiesBox->setCurrentWidget(ui->scrollPropPage);
    });

    connect(ui->addScr, &QToolButton::clicked, [this] {
        auto &layer = viewer->layers[viewer->selectedLayer];

        SceneHelpers::TileLayer::ScrollIndexInfo scr;
        SceneHelpers::TileLayer::ScrollInstance instance;

        instance.startLine = 0;
        instance.length    = 1;
        scr.instances.append(instance);

        layer.scrollInfos.append(scr);

        CreateScrollList();
        DoAction("Add Scroll");
    });

    connect(ui->rmScr, &QToolButton::clicked, [this] {
        int c = ui->scrollList->currentRow();
        int n = ui->scrollList->currentRow() == ui->scrollList->count() - 1 ? c - 1 : c;
        delete ui->scrollList->item(c);
        viewer->layers[viewer->selectedLayer].scrollInfos.removeAt(c);
        ui->scrollList->blockSignals(true);
        ui->scrollList->setCurrentRow(n);
        ui->scrollList->blockSignals(false);
        DoAction("Remove Scroll: " + QString::number(c));
    });

    connect(ui->impScr, &QToolButton::clicked, [this] {
        QFileDialog filedialog(this, tr("Import RSDK Scroll File"), "",
                               tr("RSDK Scroll Files (*.bin)"));
        filedialog.setAcceptMode(QFileDialog::AcceptOpen);
        if (filedialog.exec() == QDialog::Accepted) {
            Reader reader(filedialog.selectedFiles()[0]);
            SceneHelpers::TileLayer &layer = viewer->layers[ui->layerList->currentRow()];
            layer.scrollInfos.clear();
            ushort count = reader.read<ushort>();
            for (int i = 0; i < count; ++i) {
                SceneHelpers::TileLayer::ScrollIndexInfo info;
                info.parallaxFactor = reader.read<float>();
                info.scrollSpeed    = reader.read<float>();
                info.deform         = reader.read<byte>();
                info.unknown        = reader.read<byte>();

                byte instanceCount = reader.read<byte>();
                for (int c = 0; c < instanceCount; ++c) {
                    SceneHelpers::TileLayer::ScrollInstance instance;
                    instance.startLine = reader.read<int>();
                    instance.length    = reader.read<int>();
                    info.instances.append(instance);
                }
                layer.scrollInfos.append(info);
            }
            CreateScrollList();
            DoAction("Import Scroll Info for layer: " + layer.name);
        }
    });

    connect(ui->expScr, &QToolButton::clicked, [this] {
        QFileDialog filedialog(this, tr("Export RSDK Scroll File"), "",
                               tr("RSDK Scroll Files (*.bin)"));
        filedialog.setAcceptMode(QFileDialog::AcceptSave);
        if (filedialog.exec() == QDialog::Accepted) {
            Writer writer(filedialog.selectedFiles()[0]);
            SceneHelpers::TileLayer &layer = viewer->layers[ui->layerList->currentRow()];

            writer.write<ushort>(layer.scrollInfos.count());
            for (int i = 0; i < layer.scrollInfos.count(); ++i) {
                SceneHelpers::TileLayer::ScrollIndexInfo &info = layer.scrollInfos[i];
                writer.write<float>(info.parallaxFactor);
                writer.write<float>(info.scrollSpeed);
                writer.write<byte>(info.deform);
                writer.write<byte>(info.unknown);

                writer.write<byte>(info.instances.count());
                for (auto &instance : info.instances) {
                    writer.write<int>(instance.startLine);
                    writer.write<int>(instance.length);
                }
            }
            writer.flush();
        }
    });

    connect(ui->panTool, &QToolButton::clicked, [this] { ResetTools(SceneViewer::TOOL_MOUSE); });
    connect(ui->selectTool, &QToolButton::clicked, [this] { ResetTools(SceneViewer::TOOL_SELECT); });
    connect(ui->pencilTool, &QToolButton::clicked, [this] { ResetTools(SceneViewer::TOOL_PENCIL); });
    connect(ui->stampTool, &QToolButton::clicked, [this] { ResetTools(SceneViewer::TOOL_STAMP); });
    connect(ui->eraserTool, &QToolButton::clicked, [this] { ResetTools(SceneViewer::TOOL_ERASER); });
    connect(ui->entityTool, &QToolButton::clicked, [this] { ResetTools(SceneViewer::TOOL_ENTITY); });
    connect(ui->stampCopy, &QToolButton::clicked, [this] { ResetTools(SceneViewer::TOOL_STAMP_MAKER); });

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

    connect(scnProp->unknown1, QOverload<int>::of(&QSpinBox::valueChanged),
            [this](int v) { viewer->metadata.unknown1 = (byte)v; });

    connect(scnProp->unknown2, QOverload<int>::of(&QSpinBox::valueChanged),
            [this](int v) { viewer->metadata.unknown2 = (byte)v; });

    connect(scnProp->unknown3, QOverload<int>::of(&QSpinBox::valueChanged),
            [this](int v) { viewer->metadata.unknown3 = (byte)v; });

    connect(scnProp->unknown4, QOverload<int>::of(&QSpinBox::valueChanged),
            [this](int v) { viewer->metadata.unknown4 = (byte)v; });

    connect(scnProp->unknown5, QOverload<int>::of(&QSpinBox::valueChanged),
            [this](int v) { viewer->metadata.unknown5 = (byte)v; });

    connect(scnProp->unknown6, QOverload<int>::of(&QSpinBox::valueChanged),
            [this](int v) { viewer->metadata.unknown6 = (byte)v; });

    connect(scnProp->unknown7, QOverload<int>::of(&QSpinBox::valueChanged),
            [this](int v) { viewer->metadata.unknown7 = (byte)v; });

    connect(scnProp->unknown8, QOverload<int>::of(&QSpinBox::valueChanged),
            [this](int v) { viewer->metadata.unknown8 = (byte)v; });

    connect(scnProp->unknown9, QOverload<int>::of(&QSpinBox::valueChanged),
            [this](int v) { viewer->metadata.unknown9 = (byte)v; });

    connect(scnProp->loadGlobalCB, &QCheckBox::toggled, [this](bool b) {
        if (!b){
            QMessageBox::StandardButton warning = QMessageBox::question(this,
                "Unload Globals", "WARNING: this will remove all global objects from the scene,"
                                  " some of which may be required for stage objects to work, continue?", QMessageBox::Yes|QMessageBox::No);
            if (warning == QMessageBox::No){
                scnProp->loadGlobalCB->setChecked(true);
                return;
            }
        }

        stageConfig.loadGlobalObjects = b;

        if (stageConfig.loadGlobalObjects) { // assume we had no globals & are now adding em

            for (int o = viewer->entities.count() - 1; o >= 0; --o) {
                if (viewer->entities[o].type >= 1)
                    viewer->entities[o].type += gameConfig.objects.count();
            }

            for (int t = gameConfig.objects.count() - 1; t >= 0; --t) {

                SceneObject obj;
                obj.name = gameConfig.objects[t];
                viewer->objects.insert(1, obj);
            }
        }
        else { // assume we had globals & are now removing em
            for (int o = viewer->entities.count() - 1; o >= 0; --o) {
                SceneEntity &entity = viewer->entities[o];
                if (entity.type > gameConfig.objects.count()){
                    entity.type -= gameConfig.objects.count();
                }
                else if (viewer->entities[o].type >= 1)
                    DeleteEntity(o, false);
            }

            for (int t = gameConfig.objects.count() - 1; t >= 0; --t) {
                viewer->objects.removeAt(t + 1);
            }

            objProp->unsetUI();
        }

        ui->objectList->blockSignals(true);
        ui->objectList->clear();
        for (int o = 0; o < viewer->objects.count(); ++o) {
            QListWidgetItem *item = new QListWidgetItem(viewer->objects[o].name, ui->objectList);
            item->setCheckState(viewer->objects[o].visible ? Qt::Checked : Qt::Unchecked);
            item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        }
        ui->objectList->blockSignals(false);

        // Maybe lazy and there's an actual way of finding out the vars, buuuuuuuuuut...
        LoadGameLinks();
        InitGameLink();

        CreateEntityList();
        scnProp->syncGC->setDisabled(!stageConfig.loadGlobalObjects);
        DoAction(stageConfig.loadGlobalObjects ? "Added Global Objects" : "Removed Global Objects");
    });

    connect(ui->showParallax, &QPushButton::clicked, [this] {
        viewer->showParallax ^= 1;
        DoAction("Toggle Parallax Viewer");
    });

    connect(scnProp->editSCF, &QPushButton::clicked, [this, reSyncGameObject] {
        StageConfigEditorv5 *edit = new StageConfigEditorv5(&stageConfig, this);
        edit->exec();

        int oldListCount = ui->objectList->count() - 1;
        reSyncGameObject(false, oldListCount);

        ui->objectList->blockSignals(true);
        ui->objectList->clear();
        for (int o = 0; o < viewer->objects.count(); ++o) {
            QListWidgetItem *item = new QListWidgetItem(viewer->objects[o].name, ui->objectList);
            item->setCheckState(viewer->objects[o].visible ? Qt::Checked : Qt::Unchecked);
            item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        }
        ui->objectList->blockSignals(false);

        objProp->unsetUI();
        CreateEntityList();
        DoAction("Edited StageConfig");
    });

    connect(scnProp->editTSet, &QPushButton::clicked, [this] {
        TilesetEditor *edit = new TilesetEditor(viewer->tiles, viewer->tilePalette);
        edit->setWindowTitle("Edit Tileset");
        edit->exec();

        viewer->stopTimer();
        viewer->disableDrawScene = true;

        SetStatus("Rebuilding tiles...", true);
        viewer->gfxSurface[0].texturePtr = nullptr;

        QImage tileset(0x10, 0x400 * 0x10, QImage::Format_Indexed8);

        QVector<QRgb> pal;
        for (PaletteColor &col : viewer->tilePalette) pal.append(col.toQColor().rgb());
        tileset.setColorTable(pal);
        AddStatusProgress(1. / 4); // finished setup

        uchar *pixels = tileset.bits();
        for (int i = 0; i < 0x400; ++i) {
            uchar *src = viewer->tiles[i].bits();
            for (int y = 0; y < 16; ++y) {
                for (int x = 0; x < 16; ++x) {
                    *pixels++ = *src++;
                }
            }
        }
        AddStatusProgress(1. / 4); // finished copying tiles

        viewer->gfxSurface[0].texturePtr = viewer->createTexture(tileset, QOpenGLTexture::Target2D);

        for (int i = 0; i < viewer->layers.count(); ++i) {
            auto &layer = viewer->layers[i];
            for (int y = 0; y < layer.height; ++y) {
                for (int x = 0; x < layer.width; ++x) {
                    ushort tile        = layer.layout[y][x];
                    layer.layout[y][x] = edit->tileIDs.indexOf(tile & 0x3FF);
                    layer.layout[y][x] |= (tile >> 10) << 10;
                }
            }
        }
        AddStatusProgress(1. / 4); // finished updating layout

        RSDKv5::TileConfig configStore = tileconfig;
        for (int i = 0; i < 0x400; ++i) {
            int id                           = edit->tileIDs.indexOf(i);
            tileconfig.collisionPaths[0][id] = configStore.collisionPaths[0][i];
            tileconfig.collisionPaths[1][id] = configStore.collisionPaths[1][i];
        }

        tileSel->RefreshList();

        DoAction("Edited Tiles");
        SetStatus("Finished rebuilding tiles!"); // done !

        viewer->startTimer();
        viewer->disableDrawScene = false;
    });

    connect(scnProp->editPAL, &QPushButton::clicked, [this] {
        RSDKv5::Palette *configPal = nullptr;
        RSDKv5::Palette *editPal   = nullptr;

        PaletteEditor *edit = new PaletteEditor(stageConfig.filePath, PALTYPE_STAGECONFIGv5);        edit->palette.clear();
        for (int b = 0; b < 8; ++b){
            configPal = &stageConfig.palettes[b];
            for (int r = 0; r < 16; ++r){
                edit->stageConfigv5.palettes[b].activeRows[r] = configPal->activeRows[r];
                for (int c = 0; c < 16; ++c){
                    edit->palette.append(QColor(configPal->colors[r][c]));
                    edit->stageConfigv5.palettes[b].colors[r][c] = QColor(configPal->colors[r][c]);
                }
            }
        }
        edit->mainWindow = false;
        edit->setWindowTitle("Edit StageConfig Palette");
        edit->exec();

        for(int b = 0; b < 8; ++b){
            editPal = &edit->stageConfigv5.palettes[b];
            configPal = &stageConfig.palettes[b];

            for (int r = 0; r < 16; ++r) {
                configPal->activeRows[r] = editPal->activeRows[r];
                if (editPal->activeRows[r]) {
                    for (int c = 0; c < 16; ++c) {
                        for (int c = 0; c < 16; ++c) {
                            if (b == edit->bankID)
                                configPal->colors[r][c] =
                                    QColor(edit->palette[(r << 4) + c].r, edit->palette[(r << 4) + c].g,
                                            edit->palette[(r << 4) + c].b);
                            else
                                configPal->colors[r][c] = editPal->colors[r][c];
                        }
                    }
                }
                else {
                    for (int c = 0; c < 16; ++c) {
                        configPal->colors[r][c] = QColor(0xFF, 0x00, 0xFF);
                    };
                }
            }
        }
        DoAction("Edited Palette");
    });

    connect(scnProp->stampNameEdit, &QLineEdit::textChanged, [this](QString s) {
        viewer->metadata.stampName = s;
    });

    connect(scnProp->loadStamps, &QPushButton::clicked, [this] {
        QString path = QFileInfo(viewer->stamps.filePath).absolutePath() + "/"+ viewer->metadata.stampName;
        if (QFile::exists(path)) {
            SetStatus("File found, Loading stamps...");

            viewer->stamps.read(path);
            ui->stampList->blockSignals(true);
            ui->stampList->clear();
            for (auto &stamp : viewer->stamps.stampList) ui->stampList->addItem(stamp.name);
            ui->stampList->blockSignals(false);
            ui->stampList->setCurrentRow(-1);
            viewer->selectedStamp = 0xFFFF;

            SetStatus("Loaded stamps from " + QFile(viewer->stamps.filePath).fileName());
        }
        else {
            QFileDialog filedialog(this, tr("Load Stamps"), "", tr("RSDKv5 Stamps (*.bin)"));
            filedialog.setAcceptMode(QFileDialog::AcceptOpen);
            if (filedialog.exec() == QDialog::Accepted) {
                SetStatus("File found, Loading stamps...");
                viewer->stamps.read(QFile(filedialog.selectedFiles()[0]).fileName());
                viewer->stamps.filePath = QFile(filedialog.selectedFiles()[0]).fileName();
                viewer->metadata.stampName = QFileInfo(viewer->stamps.filePath).fileName();

                scnProp->stampNameEdit->blockSignals(true);
                scnProp->stampNameEdit->setText(viewer->metadata.stampName);
                scnProp->stampNameEdit->update();
                scnProp->stampNameEdit->blockSignals(false);

                ui->stampList->blockSignals(true);
                ui->stampList->clear();
                for (auto &stamp : viewer->stamps.stampList) ui->stampList->addItem(stamp.name);
                ui->stampList->blockSignals(false);
                ui->stampList->setCurrentRow(-1);
                viewer->selectedStamp = 0xFFFF;


                SetStatus("Loaded stamps from " + QFile(viewer->stamps.filePath).fileName());
            }
        }
    });

    connect(scnProp->saveStamps, &QPushButton::clicked, [this] {
        QString path = QFileInfo(viewer->stamps.filePath).absolutePath() + "/"+ viewer->metadata.stampName;

        if (QFile::exists(path)) {
            SetStatus("Saving stamps...");

            viewer->stamps.write(path);

            SetStatus("Saved stamps to " + QFile(viewer->stamps.filePath).fileName());
        }
        else {
            QFileDialog filedialog(this, tr("Save Stamps"), "", tr("RSDKv5 Stamps (*.bin)"));
            filedialog.setAcceptMode(QFileDialog::AcceptSave);
            if (filedialog.exec() == QDialog::Accepted) {
                SetStatus("Saving stamps...");
                QString path = filedialog.selectedFiles()[0];

                viewer->stamps.write(path);
                SetStatus("Saved stamps to " + QFile(viewer->stamps.filePath).fileName());
            }
        }
    });

    connect(scnProp->syncGC, &QPushButton::clicked, [this, reSyncGameObject] {
        int scnGCObjCount = ui->objectList->count() - stageConfig.objects.count() - 1;
        reSyncGameObject(true, scnGCObjCount);

        ui->objectList->blockSignals(true);
        ui->objectList->clear();
        for (int o = 0; o < viewer->objects.count(); ++o) {
            QListWidgetItem *item = new QListWidgetItem(viewer->objects[o].name, ui->objectList);
            item->setCheckState(viewer->objects[o].visible ? Qt::Checked : Qt::Unchecked);
            item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        }
        ui->objectList->blockSignals(false);

        objProp->unsetUI();
        CreateEntityList();
        SetStatus("Synced Game Config!");
        DoAction("Synced Game Config");
    });
    scnProp->syncGC->setDisabled(true);

    connect(scnProp->syncSC, &QPushButton::clicked, [this, reSyncGameObject] {
        int oldListCount = ui->objectList->count() - 1;
        reSyncGameObject(false, oldListCount);

        ui->objectList->blockSignals(true);
        ui->objectList->clear();
        for (int o = 0; o < viewer->objects.count(); ++o) {
            QListWidgetItem *item = new QListWidgetItem(viewer->objects[o].name, ui->objectList);
            item->setCheckState(viewer->objects[o].visible ? Qt::Checked : Qt::Unchecked);
            item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        }
        ui->objectList->blockSignals(false);

        objProp->unsetUI();
        CreateEntityList();
        SetStatus("Synced Stage Config!");
        DoAction("Synced Stage Config");
    });

    scnProp->bgSel->setColor(viewer->metadata.backgroundColor1);
    connect(scnProp->bgSel, &color_widgets::ColorPreview::clicked, [this] {
        RSDKColorDialog dlg(viewer->metadata.backgroundColor1);
        if (dlg.exec() == QDialog::Accepted) {
            viewer->metadata.backgroundColor1 = dlg.color().toQColor();
            scnProp->bgSel->setColor(viewer->metadata.backgroundColor1);
        }
    });

    scnProp->altBGSel->setColor(viewer->metadata.backgroundColor2);
    connect(scnProp->altBGSel, &color_widgets::ColorPreview::clicked, [this] {
        RSDKColorDialog dlg(viewer->metadata.backgroundColor2);
        if (dlg.exec() == QDialog::Accepted) {
            viewer->metadata.backgroundColor2 = dlg.color().toQColor();
            scnProp->altBGSel->setColor(viewer->metadata.backgroundColor2);
        }
    });

    connect(scnProp->reloadLink, &QPushButton::clicked, [this] {
        SetStatus("Reloading Game Link...");
        viewer->stopTimer();

        UnloadGameLinks();

        ConfirmGameLink *confirmDlg = new ConfirmGameLink;
        confirmDlg->exec();

        LoadGameLinks();
        InitGameLink();

        viewer->startTimer();
        SetStatus("Game Link reloaded successfully!");
    });

    connect(scnProp->replaceTile, &QPushButton::clicked, [this] {
        TileReplaceOptions *dlg = new TileReplaceOptions;
        dlg->replaceVisualPlane->setDisabled(true);
        dlg->hasVisualPlane->setDisabled(true);

        if (!dlg->exec())
            return;

        viewer->stopTimer();

        SetStatus("Replacing Tile Info...", true);

        ushort dstTile = dlg->dstTile->value();
        ushort srcTile = dlg->srcTile->value();

        bool replaceIndex     = dlg->replaceTileIndex->checkState() == Qt::Checked;
        bool replaceFlipX     = dlg->replaceFlipX->checkState() == Qt::Checked;
        bool replaceFlipY     = dlg->replaceFlipY->checkState() == Qt::Checked;
        bool replaceSolidATop = dlg->replaceSolidATop->checkState() == Qt::Checked;
        bool replaceSolidALRB = dlg->replaceSolidALRB->checkState() == Qt::Checked;
        bool replaceSolidBTop = dlg->replaceSolidBTop->checkState() == Qt::Checked;
        bool replaceSolidBLRB = dlg->replaceSolidBLRB->checkState() == Qt::Checked;
        bool replaceCollision = dlg->replaceCollision->checkState() == Qt::Checked;
        bool replaceGraphics  = dlg->replaceGraphics->checkState() == Qt::Checked;

        bool hasFlipX     = dlg->hasFlipX->checkState() == Qt::Checked;
        bool hasFlipY     = dlg->hasFlipY->checkState() == Qt::Checked;
        bool hasSolidATop = dlg->hasSolidATop->checkState() == Qt::Checked;
        bool hasSolidALRB = dlg->hasSolidALRB->checkState() == Qt::Checked;
        bool hasSolidBTop = dlg->hasSolidBTop->checkState() == Qt::Checked;
        bool hasSolidBLRB = dlg->hasSolidBLRB->checkState() == Qt::Checked;

        // Replace Tile Layer info
        if (replaceIndex || replaceFlipX || replaceFlipY || replaceSolidATop || replaceSolidALRB
            || replaceSolidBTop || replaceSolidBLRB) {
            for (int i = 0; i < viewer->layers.count(); ++i) {
                auto &layer = viewer->layers[i];
                for (int y = 0; y < layer.height; ++y) {
                    for (int x = 0; x < layer.width; ++x) {
                        ushort tile = layer.layout[y][x];

                        if ((tile & 0x3FF) == dstTile) {
                            bool flipX     = replaceFlipX ? hasFlipX : Utils::getBit(tile, 10);
                            bool flipY     = replaceFlipY ? hasFlipY : Utils::getBit(tile, 11);
                            bool solidATop = replaceSolidATop ? hasSolidATop : Utils::getBit(tile, 12);
                            bool solidALRB = replaceSolidALRB ? hasSolidALRB : Utils::getBit(tile, 13);
                            bool solidBTop = replaceSolidBTop ? hasSolidBTop : Utils::getBit(tile, 14);
                            bool solidBLRB = replaceSolidBLRB ? hasSolidBLRB : Utils::getBit(tile, 15);

                            ushort newTile = replaceIndex ? srcTile : dstTile;

                            Utils::setBit(newTile, flipX, 10);
                            Utils::setBit(newTile, flipY, 11);
                            Utils::setBit(newTile, solidATop, 12);
                            Utils::setBit(newTile, solidALRB, 13);
                            Utils::setBit(newTile, solidBTop, 14);
                            Utils::setBit(newTile, solidBLRB, 15);

                            layer.layout[y][x] = newTile;
                        }
                    }
                }
            }
        }
        AddStatusProgress(1. / 3); // finished updating layers

        if (replaceCollision) {
            auto &dstA = viewer->tileconfig.collisionPaths[0][dstTile];
            auto &srcA = viewer->tileconfig.collisionPaths[0][srcTile];

            dstA.flags      = srcA.flags;
            dstA.floorAngle = srcA.floorAngle;
            dstA.lWallAngle = srcA.lWallAngle;
            dstA.roofAngle  = srcA.roofAngle;
            dstA.rWallAngle = srcA.rWallAngle;

            for (int c = 0; c < 16; ++c) {
                dstA.collision[c].height = srcA.collision[c].height;
                dstA.collision[c].solid  = srcA.collision[c].solid;
            }

            auto &dstB = viewer->tileconfig.collisionPaths[1][dstTile];
            auto &srcB = viewer->tileconfig.collisionPaths[1][srcTile];

            dstB.flags      = srcB.flags;
            dstB.floorAngle = srcB.floorAngle;
            dstB.lWallAngle = srcB.lWallAngle;
            dstB.roofAngle  = srcB.roofAngle;
            dstB.rWallAngle = srcB.rWallAngle;

            for (int c = 0; c < 16; ++c) {
                dstB.collision[c].height = srcB.collision[c].height;
                dstB.collision[c].solid  = srcB.collision[c].solid;
            }
        }
        AddStatusProgress(1. / 3); // finished updating collision

        if (replaceGraphics) {
            auto &dstTileImg = viewer->tiles[dstTile];
            auto &srcTileImg = viewer->tiles[srcTile];

            uchar *dstPixels = dstTileImg.bits();
            uchar *srcPixels = srcTileImg.bits();
            for (int y = 0; y < 16; ++y) {
                for (int x = 0; x < 16; ++x) {
                    *dstPixels++ = *srcPixels++;
                }
            }
        }
        SetStatus("Finished replacing Tile Info!"); // finished updating graphics

        viewer->startTimer();
    });

    connect(ui->exportSceneImg, &QPushButton::clicked, [this] {
        QFileDialog filedialog(this, tr("Save Image"), "", tr("PNG Files (*.png)"));
        filedialog.setAcceptMode(QFileDialog::AcceptSave);
        if (filedialog.exec() == QDialog::Accepted) {
            SetStatus("Rendering image...", true);
            viewer->queueRender(filedialog.selectedFiles()[0]);
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

            QList<QString> types      = { "uint8", "uint16", "uint32", "int8",    "int16", "int32",
                                     "enum",  "bool",   "string", "vector2", "float", "color" };
            QList<QString> layerTypes = { "HScroll", "VScroll", "RotoZoom", "Basic" };

            writer.writeLine(QString("\t<metadata libraryFile=\"%1\" bgColor1=\"%2\" "
                                     "bgColor2=\"%3\"> </metadata>")
                                 .arg(viewer->metadata.stampName)
                                 .arg(viewer->metadata.backgroundColor1.rgba())
                                 .arg(viewer->metadata.backgroundColor2.rgba()));

            if (viewer->layers.count()) {
                writer.writeLine();
                writer.writeLine("\t<layers>");

                for (auto &layer : viewer->layers) {
                    writer.writeText(
                        QString(
                            "\t\t<layer name=\"%1\" type=\"%2\" drawGroup=\"%3\" width=\"%4\" "
                            "height=\"%5\" parallaxFactor=\"%6\" scrollSpeed=\"%7\" visible=\"%8\">")
                            .arg(layer.name)
                            .arg(layerTypes[layer.type])
                            .arg(layer.drawGroup)
                            .arg(layer.width)
                            .arg(layer.height)
                            .arg(layer.parallaxFactor)
                            .arg(layer.scrollSpeed)
                            .arg(layer.visible ? "true" : "false"));

                    if (layer.scrollInfos.count()) {
                        writer.writeLine();
                        writer.writeLine("\t\t\t<scrollingInfo>");
                        for (auto &scroll : layer.scrollInfos) {
                            writer.writeLine(QString("\t\t\t\t<scrollInfo parallaxFactor=\"%1\" "
                                                     "scrollSpeed=\"%2\" deform=\"%3\">")
                                                 .arg(scroll.parallaxFactor)
                                                 .arg(scroll.scrollSpeed)
                                                 .arg(scroll.deform ? "true" : "false"));
                            for (auto instance : scroll.instances) {
                                writer.writeLine(QString("\t\t\t\t\t<scrollInstance startLine=\"%1\" "
                                                         "length=\"%2\"></scrollInstance>")
                                                     .arg(instance.startLine)
                                                     .arg(instance.length));
                            }
                            writer.writeLine(QString("\t\t\t\t</scrollInfo>"));
                        }
                        writer.writeLine("\t\t\t</scrollingInfo>");
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
                    writer.writeLine();

                    writer.writeLine("\t\t</layer>");
                    writer.writeLine();
                }

                writer.writeLine("\t</layers>");
                writer.writeLine();
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
                writer.writeLine();
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
                                case VAR_FLOAT:
                                    writer.writeText(QString::number(variable.value_float));
                                    break;
                                case VAR_COLOR:
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
                writer.writeLine();
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

    UnloadGameLinks();
    releaseStorage(dataStorage);
}

void SceneEditorv5::updateType(SceneEntity *entity, byte type)
{
    int c = viewer->selectedEntity;
    ui->entityList->item(c)->setText(QString::number(viewer->entities[c].slotID) + ": "
                                     + viewer->objects[viewer->entities[c].type].name);
    entity->variables.clear();
    for (int v = 0; v < viewer->objects[type].variables.length(); ++v) {
        RSDKv5::Scene::VariableValue var;
        var.type = viewer->objects[type].variables[v].type;
        entity->variables.append(var);
    }

    for (int v = 0; v < viewer->objects[type].variables.count(); ++v) {
        if (viewer->objects[type].variables[v].name == "filter") {
            if (v < entity->variables.count())
                entity->variables[v].value_uint8 = viewer->sceneFilter;
            break;
        }
    }
    objProp->setupUI(&viewer->entities[c]); // maybe make it use updateUI?
}

void SceneEditorv5::updateTileSel(){
    ushort tile = viewer->selectedTile;
    Utils::setBit(tile, viewer->tileFlip.x, 10);
    Utils::setBit(tile, viewer->tileFlip.y, 11);
    Utils::setBit(tile, viewer->tileSolidA.x, 12);
    Utils::setBit(tile, viewer->tileSolidA.y, 13);
    Utils::setBit(tile, viewer->tileSolidB.x, 14);
    Utils::setBit(tile, viewer->tileSolidB.y, 15);

    tileProp->setupUI(&tileconfig.collisionPaths[0][tile & 0x3FF], &tileconfig.collisionPaths[1][tile & 0x3FF],
            &tile, viewer->tiles[tile & 0x3FF]);
    copiedTile = false;
}
void SceneEditorv5::updateStampName(QString name){
    ui->stampList->currentItem()->setText(name);
}
void SceneEditorv5::updateLayerName(QString name){
    ui->layerList->currentItem()->setText(name);
}


QString lastSelected = "";

bool SceneEditorv5::event(QEvent *event)
{

    switch ((int)event->type()) {
        case RE_EVENT_NEW: {
            QList<QString> types = {
                "RSDKv5 Scenes",
                "RSDKv5 Scenes (Pre-Plus)",
            };

            CreateScene *cScene = new CreateScene();
            cScene->LoadSceneVer(types);
            if (cScene->exec() == QDialog::Accepted) {
                bool prePlus = cScene->sceneVer;
                CreateNewScene(cScene->scenePath, prePlus, cScene->loadGC, cScene->gcPath);
            }

            break;
        }

        case RE_EVENT_OPEN: {
            QList<QString> types = {
                "RSDKv5 Scenes (Scene*.bin)",
                "RSDKv5 Scenes (Pre-Plus) (Scene*.bin)",
            };

            QFileDialog filedialog(this, tr("Open Scene"), "");
            filedialog.setNameFilters(types);
            filedialog.setAcceptMode(QFileDialog::AcceptOpen);

            if (!lastSelected.isEmpty())
                filedialog.selectNameFilter(lastSelected);

            if (filedialog.exec() == QDialog::Accepted) {
                lastSelected   = filedialog.selectedNameFilter();
                int filter     = types.indexOf(lastSelected);
                QString gcPath = "";

                QString filePath = QFileInfo(filedialog.selectedFiles()[0]).absolutePath();
                QDir dir(filePath); // Data/Stages/SCENE
                dir.cdUp();         // Data/Stages
                dir.cdUp();         // Data

                if (viewer->dataPath.isEmpty() || viewer->dataPath != dir.path()) {
                    QFileDialog gcdialog(
                        this, tr("Open GameConfig"), dir.path(),
                        tr("RSDKv5 GameConfig (GameConfig*.bin);;RSDK Game.xml Files (Game*.xml)"));
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
                else {
                    // Same data path, reuse game config
                    gcPath = gameConfig.filePath;
                }

                LoadScene(filedialog.selectedFiles()[0], gcPath, filter);
                return true;
            }
            break;
        }

        case RE_EVENT_SAVE:
            if (QFile::exists(scene.filepath)) {
                SaveScene(scene.filepath);
                return true;
            }
            else {
                QFileDialog filedialog(this, tr("Save Scene"), "", tr("RSDKv5 Scenes (Scene*.bin)"));
                filedialog.setAcceptMode(QFileDialog::AcceptSave);
                if (filedialog.exec() == QDialog::Accepted) {
                    QString path = filedialog.selectedFiles()[0];

                    SaveScene(path);
                    return true;
                }
            }
            break;

        case RE_EVENT_SAVE_AS: {
            QFileDialog filedialog(this, tr("Save Scene"), "", tr("RSDKv5 Scenes (Scene*.bin)"));
            filedialog.setAcceptMode(QFileDialog::AcceptSave);
            if (filedialog.exec() == QDialog::Accepted) {
                QString path = filedialog.selectedFiles()[0];

                SaveScene(path);
                return true;
            }
            break;
        }

        case RE_EVENT_UNDO: UndoAction(); return true;
        case RE_EVENT_REDO: RedoAction(); return true;
        // TODO: find the actual reason the editor refuses to update w/
        // the proper texture without resize or hiding when changing to certain tools
        case RE_EVENT_TAB_GAIN_FOCUS:
            LoadGameLinks();
            InitGameLink();
            viewer->startTimer();
            viewer->show(); //hack
            break;
        case RE_EVENT_TAB_LOSE_FOCUS:
            viewer->stopTimer();
            UnloadGameLinks();
            viewer->hide(); //hack
            break;

        case QEvent::KeyPress: viewerActive = false; HandleKeyPress(static_cast<QKeyEvent *>(event)); break;
        case QEvent::KeyRelease: viewerActive = false; HandleKeyRelease(static_cast<QKeyEvent *>(event)); break;

        case QEvent::Close:
            if (modified) {
                bool cancelled = false;
                if (MainWindow::ShowCloseWarning(this, &cancelled)) {
                    QString path = scene.filepath;

                    if (QFile::exists(path)) {
                        SaveScene(path);
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
                            QString path = filedialog.selectedFiles()[0];

                            SaveScene(path);
                            return true;
                        }
                    }
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

bool SceneEditorv5::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::FocusIn || event->type() == QEvent::FocusOut) {
        QApplication::sendEvent(this, event);
        return false;
    }

    if (object != viewer)
        return false;

    switch ((int)event->type()) {
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

            if (mouseDownM || (mouseDownL && viewer->curTool == SceneViewer::TOOL_MOUSE))
                setCursor(Qt::ClosedHandCursor);

            if (mouseDownL) {
                switch (viewer->curTool) {
                    case SceneViewer::TOOL_MOUSE: break;
                    case SceneViewer::TOOL_SELECT:
                        if (viewer->selectedEntities.count() == 0) {
                            viewer->isSelecting  = true;
                            viewer->selectPos.x  = sceneMousePos.x;
                            viewer->selectPos.y  = sceneMousePos.y;
                            viewer->selectSize.x = 0;
                            viewer->selectSize.y = 0;
                            viewer->selectedEntities.clear();
                        } else {
                            Rect<float> box;
                            int selectedEntity = -1;
                            int entPos = 0;
                            for (auto o : viewer->selectedEntities) {
                                int left   = viewer->entities[o].pos.x + viewer->entities[o].box.x;
                                int top    = viewer->entities[o].pos.y + viewer->entities[o].box.y;
                                int right  = viewer->entities[o].pos.x + viewer->entities[o].box.w;
                                int bottom = viewer->entities[o].pos.y + viewer->entities[o].box.h;
                                box = Rect<float>(left, top, abs(right - left), abs(bottom - top));

                                Vector2<float> pos = Vector2<float>(
                                    (mEvent->pos().x() * viewer->invZoom()) + viewer->cameraPos.x,
                                    (mEvent->pos().y() * viewer->invZoom()) + viewer->cameraPos.y);


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

                                if (box.contains(pos) && selectedEntity < o && filterFlag) {
                                    selectedEntity = o; // future proofing?
                                    if (viewer->selectedEntity != o) {
                                        waitForRelease         = true;
                                        viewer->selectedEntity = o;
                                        viewer->centerEntity   = entPos;
                                        viewer->sceneInfo.listPos =
                                            viewer->entities[viewer->selectedEntity].slotID;
                                        viewer->sceneInfoV1.listPos = viewer->sceneInfo.listPos;

                                        selectionOffset.x = pos.x - viewer->entities[o].pos.x;
                                        selectionOffset.y = pos.y - viewer->entities[o].pos.y;

                                        objProp->setupUI(&viewer->entities[viewer->selectedEntity]);
                                        ui->propertiesBox->setCurrentWidget(ui->objPropPage);
                                    }
                                    break;
                                }
                                entPos++;
                            }

                            if (selectedEntity == -1) {
                                viewer->selectedEntity = -1;
                                viewer->isSelecting  = true;
                                viewer->selectPos.x  = sceneMousePos.x;
                                viewer->selectPos.y  = sceneMousePos.y;
                                viewer->selectSize.x = 0;
                                viewer->selectSize.y = 0;
                                viewer->selectedEntities.clear();
                                viewer->selectedEntitiesXPos.clear();
                                viewer->selectedEntitiesYPos.clear();
                            }
                        }
                        break;

                    case SceneViewer::TOOL_PENCIL: {
                        if (viewer->selectedTile != 0xFFFF && viewer->isSelecting) {
                            SetTile(mEvent->pos().x(), mEvent->pos().y());
                        }
                        else {
                            // click tile to move it or change properties
                            if (viewer->selectedLayer >= 0) {
                                Rect<float> box;

                                for (int y = 0; y < viewer->layers[viewer->selectedLayer].height; ++y) {
                                    for (int x = 0; x < viewer->layers[viewer->selectedLayer].width;
                                         ++x) {
                                        box = Rect<float>(x * 0x10, y * 0x10, 0x10, 0x10);

                                        Vector2<float> pos =
                                            Vector2<float>((mEvent->pos().x() * viewer->invZoom())
                                                               + viewer->cameraPos.x,
                                                           (mEvent->pos().y() * viewer->invZoom())
                                                               + viewer->cameraPos.y);
                                        if (box.contains(pos)) {
                                            // idk but we're in the gaming zone now bitch!
                                            // ushort tile =
                                            // viewer->layers[viewer->selectedLayer].layout[y][x];
                                            break;
                                        }
                                    }
                                }
                            }
                        }
                        break;
                    }

                    case SceneViewer::TOOL_ERASER: {
                        if (viewer->isSelecting) {
                            int sel              = viewer->selectedTile;
                            viewer->selectedTile = 0xFFFF;
                            SetTile(mEvent->pos().x(), mEvent->pos().y());
                            viewer->selectedTile = sel;
                        }
                        break;
                    }
                    case SceneViewer::TOOL_ENTITY: {
                        if (!viewer->isSelecting || viewer->selectedObject < 0) {
                            Rect<float> box;
                            int firstSel = -1;
                            Vector2<float> firstPos;

                            int selectedEntity = -1;
                            viewer->sceneInfo.listPos   = -1;
                            viewer->sceneInfoV1.listPos = viewer->sceneInfo.listPos;
                            for (int o = 0; o < viewer->entities.count(); ++o) {
                                int left   = viewer->entities[o].pos.x + viewer->entities[o].box.x;
                                int top    = viewer->entities[o].pos.y + viewer->entities[o].box.y;
                                int right  = viewer->entities[o].pos.x + viewer->entities[o].box.w;
                                int bottom = viewer->entities[o].pos.y + viewer->entities[o].box.h;
                                box = Rect<float>(left, top, abs(right - left), abs(bottom - top));

                                Vector2<float> pos = Vector2<float>(
                                    (mEvent->pos().x() * viewer->invZoom()) + viewer->cameraPos.x,
                                    (mEvent->pos().y() * viewer->invZoom()) + viewer->cameraPos.y);

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

                                if (box.contains(pos) && selectedEntity < o && filterFlag) {
                                    selectedEntity = o; // future proofing?
                                    if (viewer->selectedEntity != o) {
                                        waitForRelease         = true;
                                        viewer->selectedEntity = o;
                                        viewer->sceneInfo.listPos =
                                            viewer->entities[viewer->selectedEntity].slotID;
                                        viewer->sceneInfoV1.listPos = viewer->sceneInfo.listPos;

                                        selectionOffset.x = pos.x - viewer->entities[o].pos.x;
                                        selectionOffset.y = pos.y - viewer->entities[o].pos.y;

                                        objProp->setupUI(&viewer->entities[viewer->selectedEntity]);
                                        ui->propertiesBox->setCurrentWidget(ui->objPropPage);
                                    }
                                    // DoAction(QString("Selected Entity: %1 (%2, %3)")
                                    //              .arg(o)
                                    //              .arg(viewer->entities[o].pos.x)
                                    //              .arg(viewer->entities[o].pos.y));
                                    break;
                                }
                            }

                            if (selectedEntity == -1)
                                viewer->selectedEntity = -1;

                            if (viewer->selectedEntity == -1) {
                                if (firstSel >= 0) {
                                    viewer->selectedEntity = firstSel;
                                    viewer->sceneInfo.listPos =
                                        viewer->entities[viewer->selectedEntity].slotID;
                                    viewer->sceneInfoV1.listPos = viewer->sceneInfo.listPos;

                                    selectionOffset.x =
                                        firstPos.x - viewer->entities[viewer->selectedEntity].pos.x;
                                    selectionOffset.y =
                                        firstPos.y - viewer->entities[viewer->selectedEntity].pos.y;
                                    objProp->setupUI(&viewer->entities[viewer->selectedEntity]);
                                    ui->propertiesBox->setCurrentWidget(ui->objPropPage);

                                    // DoAction(QString("Selected Entity: %1 (%2, %3)")
                                    //              .arg(firstSel)
                                    //              .arg(viewer->entities[firstSel].pos.x)
                                    //              .arg(viewer->entities[firstSel].pos.y));
                                }
                                else {
                                    selectionOffset.x = 0;
                                    selectionOffset.y = 0;
                                }
                            }

                            ui->entityList->blockSignals(true);
                            ui->entityList->setCurrentRow(viewer->selectedEntity);
                            ui->entityList->blockSignals(false);
                        }
                        else {
                            if (viewer->selectedObject >= 0
                                && viewer->activeEntityCount() < SCENEENTITY_COUNT_v5) {
                                float x =
                                    ((mEvent->pos().x() * viewer->invZoom()) + viewer->cameraPos.x);
                                float y =
                                    ((mEvent->pos().y() * viewer->invZoom()) + viewer->cameraPos.y);

                                if (ctrlDownL) {
                                    x = (x - fmodf(x, viewer->gridSize.x));
                                    y = (y - fmodf(y, viewer->gridSize.y));
                                }

                                AddEntity(viewer->selectedObject, x, y);

                                DoAction(QString("Created Entity: %1 (%2, %3)")
                                             .arg(viewer->objects[viewer->selectedObject].name)
                                             .arg(x)
                                             .arg(y));
                            }
                            else if (viewer->activeEntityCount() >= SCENEENTITY_COUNT_v5) {
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
                    case SceneViewer::TOOL_STAMP_MAKER: {
                        viewer->isSelecting  = true;
                        viewer->selectSize.x = 0;
                        viewer->selectSize.y = 0;
                        viewer->selectedEntities.clear();
                        break;
                    }
                    case SceneViewer::TOOL_STAMP: {
                        if (viewer->selectedStamp != 0xFFFF) {
                            SetStamp(mEvent->pos().x(), mEvent->pos().y());
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
                            Rect<float> box;

                            Vector2<float> pos = Vector2<float>(
                                (mEvent->pos().x() * viewer->invZoom()) + viewer->cameraPos.x,
                                (mEvent->pos().y() * viewer->invZoom()) + viewer->cameraPos.y);

                            for (int y = 0; y < viewer->layers[viewer->selectedLayer].height; ++y) {
                                for (int x = 0; x < viewer->layers[viewer->selectedLayer].width; ++x) {
                                    box = Rect<float>(x * 0x10, y * 0x10, 0x10, 0x10);
                                    if (box.contains(pos)) {
                                        ushort tile =
                                            viewer->layers[viewer->selectedLayer].layout[y][x];

                                        viewer->selectedTile = tile;
                                        copiedTile = true;
                                        ui->propertiesBox->setCurrentWidget(ui->tilePropPage);
                                        tileProp->setupUI(&tileconfig.collisionPaths[0][tile & 0x3FF], &tileconfig.collisionPaths[1][tile & 0x3FF],
                                                &tile, viewer->tiles[tile & 0x3FF]);
                                        //ui->toolBox->setCurrentWidget(ui->tilesPage); // why does this lag so much wth
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
                                (mEvent->pos().x() * viewer->invZoom()) + viewer->cameraPos.x,
                                (mEvent->pos().y() * viewer->invZoom()) + +viewer->cameraPos.y);

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
                                viewer->selectedEntity = o;
                                viewer->sceneInfo.listPos =
                                    viewer->entities[viewer->selectedEntity].slotID;
                                viewer->sceneInfoV1.listPos = viewer->sceneInfo.listPos;

                                selectionOffset.x = pos.x - viewer->entities[o].pos.x;
                                selectionOffset.y = pos.y - viewer->entities[o].pos.y;
                                found             = true;

                                objProp->setupUI(&viewer->entities[viewer->selectedEntity]);
                                ui->propertiesBox->setCurrentWidget(ui->objPropPage);
                                break;
                            }
                        }

                        if (!found) {
                            if (firstSel >= 0) {
                                viewer->selectedEntity = firstSel;
                                viewer->sceneInfo.listPos =
                                    viewer->entities[viewer->selectedEntity].slotID;
                                viewer->sceneInfoV1.listPos = viewer->sceneInfo.listPos;

                                selectionOffset.x =
                                    firstPos.x - viewer->entities[viewer->selectedEntity].pos.x;
                                selectionOffset.y =
                                    firstPos.y - viewer->entities[viewer->selectedEntity].pos.y;
                                objProp->setupUI(&viewer->entities[viewer->selectedEntity]);
                                ui->propertiesBox->setCurrentWidget(ui->objPropPage);
                            }
                            else {
                                viewer->selectedEntity      = -1;
                                viewer->sceneInfo.listPos   = -1;
                                viewer->sceneInfoV1.listPos = viewer->sceneInfo.listPos;

                                viewer->selectedObject = -1;
                                selectionOffset.x      = 0;
                                selectionOffset.y      = 0;
                                ui->objectList->setCurrentRow(-1);
                                ui->entityList->setCurrentRow(-1);

                                objProp->unsetUI();
                                // DoAction();
                            }
                        }

                        ui->entityList->blockSignals(true);
                        ui->entityList->setCurrentRow(viewer->selectedEntity);
                        ui->entityList->blockSignals(false);
                        break;
                    }

                    default: break;
                }
                break;
            }

            break;
        }

        case QEvent::MouseMove: {
            bool status = false;

            auto mPos = viewer->mapFromGlobal(QCursor::pos());

            viewer->mousePos.x = mPos.x();
            viewer->mousePos.y = mPos.y();

            Vector2<float> sceneMousePos((viewer->mousePos.x * viewer->invZoom()) + viewer->cameraPos.x,
                                         (viewer->mousePos.y * viewer->invZoom())
                                             + viewer->cameraPos.y);

            if (mouseDownM || (mouseDownL && viewer->curTool == SceneViewer::TOOL_MOUSE)) {
                float moveX = viewer->mousePos.x - viewer->reference.x();
                float moveY = viewer->mousePos.y - viewer->reference.y();

                QPoint cursorPos = QApplication::desktop()->cursor().pos();
                cursorPos -= QPoint(moveX, moveY);

                QScreen *screen = QGuiApplication::screenAt(cursorPos);
                cursorPos += QPoint(moveX, moveY);
                if (!screen) {
                    QGuiApplication::screenAt(cursorPos);
                }

                QRect screenRect = screen->availableGeometry().adjusted(20, 20, -20, -20);

                if (!screenRect.contains(cursorPos)) {
                    if (cursorPos.x() < screenRect.x()) {
                        cursorPos.setX(cursorPos.x() + (screenRect.width() - 5));
                        viewer->reference.setX(viewer->reference.x() + (screenRect.width() - 5));
                    }
                    if (cursorPos.x() > screenRect.x() + screenRect.width()) {
                        cursorPos.setX(cursorPos.x() - (screenRect.width() - 5));
                        viewer->reference.setX(viewer->reference.x() - (screenRect.width() - 5));
                    }

                    if (cursorPos.y() < screenRect.y()) {
                        cursorPos.setY(cursorPos.y() + (screenRect.height() - 5));
                        viewer->reference.setY(viewer->reference.y() + (screenRect.height() - 5));
                    }
                    if (cursorPos.y() > screenRect.y() + screenRect.height()) {
                        cursorPos.setY(cursorPos.y() - (screenRect.height() - 5));
                        viewer->reference.setY(viewer->reference.y() - (screenRect.height() - 5));
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
                ui->horizontalScrollBar->setMaximum(viewer->sceneBoundsR - viewer->storedW);
                ui->horizontalScrollBar->setValue(viewer->cameraPos.x);
                ui->horizontalScrollBar->blockSignals(false);

                ui->verticalScrollBar->blockSignals(true);
                ui->verticalScrollBar->setMaximum(viewer->sceneBoundsB - viewer->storedH);
                ui->verticalScrollBar->setValue(viewer->cameraPos.y);
                ui->verticalScrollBar->blockSignals(false);

                viewer->screens->position.x = viewer->cameraPos.x;
                viewer->screens->position.y = viewer->cameraPos.y;
            }

            if (viewer->curTool == SceneViewer::TOOL_PENCIL
                || viewer->curTool == SceneViewer::TOOL_ERASER
                || viewer->curTool == SceneViewer::TOOL_ENTITY
                || viewer->curTool == SceneViewer::TOOL_STAMP) {
                viewer->tilePos.x = viewer->mousePos.x;
                viewer->tilePos.y = viewer->mousePos.y;

                if (ctrlDownL) {
                    if (viewer->curTool == SceneViewer::TOOL_PENCIL
                        || viewer->curTool == SceneViewer::TOOL_ERASER) {
                        viewer->tilePos.x -= fmodf(viewer->tilePos.x + (0x10 / 2), 0x10);
                        viewer->tilePos.y -= fmodf(viewer->tilePos.y + (0x10 / 2), 0x10);
                    }
                    else {
                        float camOffX = fmodf(viewer->cameraPos.x, viewer->gridSize.x);
                        float camOffY = fmodf(viewer->cameraPos.y, viewer->gridSize.y);

                        viewer->tilePos.x -= fmodf(viewer->tilePos.x + camOffX, viewer->gridSize.x);
                        viewer->tilePos.y -= fmodf(viewer->tilePos.y + camOffY, viewer->gridSize.y);
                    }
                }
            }

            if (viewer->curTool == SceneViewer::TOOL_STAMP_MAKER && !viewer->isSelecting)
            {
                viewer->selectPos.x  = viewer->mousePos.x;
                viewer->selectPos.y  = viewer->mousePos.y;
            }

            // Hover
            switch (viewer->curTool) {
                default: break;
                case SceneViewer::TOOL_MOUSE: break;
                case SceneViewer::TOOL_SELECT: break;
                case SceneViewer::TOOL_PENCIL: break;
                case SceneViewer::TOOL_ERASER: break;
                case SceneViewer::TOOL_ENTITY: break;
            }

            if (mouseDownL) {
                switch (viewer->curTool) {
                    default: break;
                    case SceneViewer::TOOL_MOUSE: break;
                    case SceneViewer::TOOL_SELECT: {
                    if (viewer->isSelecting){
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
                        viewer->selectedEntitiesXPos.clear();
                        viewer->selectedEntitiesYPos.clear();
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
                                viewer->selectedEntitiesXPos.append(entity.pos.x);
                                viewer->selectedEntitiesYPos.append(entity.pos.y);
                            }
                        }
                    } else {
                        for (int o = 0; o < viewer->selectedEntities.count(); ++o) {
                            int pos = viewer->selectedEntities[o];
                            SceneEntity &entity = viewer->entities[pos];
                            int xOffset = ((viewer->selectedEntitiesXPos[viewer->centerEntity] - viewer->selectedEntitiesXPos[o]));
                            int yOffset = ((viewer->selectedEntitiesYPos[viewer->centerEntity] - viewer->selectedEntitiesYPos[o]));

                            entity.pos.x = (viewer->mousePos.x * viewer->invZoom()) + viewer->cameraPos.x;
                            entity.pos.y = (viewer->mousePos.y * viewer->invZoom()) + viewer->cameraPos.y;

                            if (ctrlDownL) {
                                entity.pos.x = (entity.pos.x - fmodf(entity.pos.x, viewer->gridSize.x)) - xOffset;
                                entity.pos.y = (entity.pos.y - fmodf(entity.pos.y, viewer->gridSize.y)) - yOffset;
                            }
                            else {
                                entity.pos.x -= xOffset + selectionOffset.x;
                                entity.pos.y -= yOffset + selectionOffset.y;
                            }
                            if (viewer->selectedEntity == pos)
                                objProp->updateUI();
                        }

                    }
                        break;
                    }
                    case SceneViewer::TOOL_PENCIL: {
                        if (viewer->selectedTile != 0xFFFF && viewer->isSelecting) {
                            SetTile(viewer->mousePos.x, viewer->mousePos.y);
                        }
                        break;
                    }
                    case SceneViewer::TOOL_ERASER: {
                        if (viewer->isSelecting) {
                            int sel              = viewer->selectedTile;
                            viewer->selectedTile = 0xFFFF;
                            SetTile(viewer->mousePos.x, viewer->mousePos.y);
                            viewer->selectedTile = sel;
                        }
                        break;
                    }
                    case SceneViewer::TOOL_ENTITY: {
                        if (viewer->selectedObject < 0 && viewer->selectedEntity >= 0
                            && !waitForRelease) {

                            SceneEntity &entity = viewer->entities[viewer->selectedEntity];

                            entity.pos.x =
                                (viewer->mousePos.x * viewer->invZoom()) + viewer->cameraPos.x;
                            entity.pos.y =
                                (viewer->mousePos.y * viewer->invZoom()) + viewer->cameraPos.y;

                            if (ctrlDownL) {
                                entity.pos.x = (entity.pos.x - fmodf(entity.pos.x, viewer->gridSize.x));
                                entity.pos.y = (entity.pos.y - fmodf(entity.pos.y, viewer->gridSize.y));
                            }
                            else {
                                entity.pos.x -= selectionOffset.x;
                                entity.pos.y -= selectionOffset.y;
                            }

                            objProp->updateUI();
                        }
                        break;
                    }
                    case SceneViewer::TOOL_STAMP_MAKER: {
                        viewer->isSelecting  = true;
                        viewer->selectSize.x = viewer->mousePos.x - viewer->selectPos.x;
                        viewer->selectSize.y = viewer->mousePos.y - viewer->selectPos.y;

                        viewer->selectSize.x += (viewer->selectSize.x >= 0 ? 0x10 : -0x10) / viewer->invZoom();
                        viewer->selectSize.y += (viewer->selectSize.y >= 0 ? 0x10 : -0x10) / viewer->invZoom();

                        viewer->selectSize.x *= viewer->invZoom();
                        viewer->selectSize.y *= viewer->invZoom();
                        break;
                    }
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

            if (mouseDownL)
                switch (viewer->curTool) {
                    case SceneViewer::TOOL_MOUSE: break;
                    case SceneViewer::TOOL_SELECT:{
                        if (!viewer->isSelecting)
                            DoAction();
                        viewer->isSelecting = false;
                        break;
                    }
                    case SceneViewer::TOOL_STAMP_MAKER: {
                        viewer->isSelecting = false;
                        AddStamp(viewer->selectPos.x, viewer->selectPos.y);
                        viewer->selectSize.x = 0;
                        viewer->selectSize.y = 0;
                        viewer->selectPos.x = viewer->mousePos.x;
                        viewer->selectPos.y = viewer->mousePos.y;
                        break;
                    }
                    case SceneViewer::TOOL_ENTITY:{
                        if (!waitForRelease)
                            DoAction();
                        break;
                    }
                    case SceneViewer::TOOL_PENCIL: {
                        if (viewer->selectedTile != 0xFFFF && viewer->isSelecting) {
                            DoAction(QString("Placed Tile(s): (%1, %2)")
                                         .arg(mEvent->pos().x())
                                         .arg(mEvent->pos().y()));
                        }
                        break;
                    }
                    case SceneViewer::TOOL_ERASER: {
                        if (viewer->isSelecting) {
                            DoAction(QString("Erased Tile(s): (%1, %2)")
                                         .arg(mEvent->pos().x())
                                         .arg(mEvent->pos().y()));
                        }
                        break;
                    }
                    default: break;
                }

            if ((mEvent->button() & Qt::LeftButton) == Qt::LeftButton) {
                if (waitForRelease)
                    waitForRelease = false;
                mouseDownL = false;
            }
            if ((mEvent->button() & Qt::MiddleButton) == Qt::MiddleButton)
                mouseDownM = false;
            if ((mEvent->button() & Qt::RightButton) == Qt::RightButton)
                mouseDownR = false;

            unsetCursor();
            break;
        }

        case QEvent::Wheel: {
            QWheelEvent *wEvent = static_cast<QWheelEvent *>(event);

            if (ctrlDownL) {
                float zoomFactor = 0;
                if (wEvent->angleDelta().y() > 0 && viewer->zoom < 20)
                    zoomFactor = 2.0f;
                else if (wEvent->angleDelta().y() < 0 && viewer->zoom > 0.5)
                    zoomFactor = 0.5f;

                if (zoomFactor != 0) {
                    Vector2<float> worldMousePos = (viewer->mousePos * viewer->invZoom()) + viewer->cameraPos;
                    viewer->zoom *= zoomFactor;
                    viewer->cameraPos = worldMousePos - (viewer->mousePos * viewer->invZoom());
                    // Truncate
                    viewer->cameraPos = Vector2<float>((int)viewer->cameraPos.x, (int)viewer->cameraPos.y);

                    viewer->screens->position.x = viewer->cameraPos.x;
                    viewer->screens->position.y = viewer->cameraPos.y;
                }

                ui->horizontalScrollBar->setMaximum(viewer->sceneBoundsR - (viewer->storedW / viewer->zoom));
                ui->verticalScrollBar->setMaximum(viewer->sceneBoundsB - (viewer->storedH / viewer->zoom));
                ui->horizontalScrollBar->setValue(viewer->cameraPos.x);
                ui->verticalScrollBar->setValue(viewer->cameraPos.y);
                return true;
            }

            if (shiftDownL) {
                viewer->cameraPos.x -= wEvent->angleDelta().y() / 8;
                viewer->cameraPos.y -= wEvent->angleDelta().x() / 8;
            }
            else {
                viewer->cameraPos.x -= wEvent->angleDelta().x() / 8;
                viewer->cameraPos.y -= wEvent->angleDelta().y() / 8;
            }

            ui->horizontalScrollBar->blockSignals(true);
            ui->horizontalScrollBar->setMaximum(viewer->sceneBoundsR
                                                - (viewer->storedW * viewer->invZoom()));
            ui->horizontalScrollBar->setValue(viewer->cameraPos.x);
            ui->horizontalScrollBar->blockSignals(false);

            ui->verticalScrollBar->blockSignals(true);
            ui->verticalScrollBar->setMaximum(viewer->sceneBoundsB
                                              - (viewer->storedH * viewer->invZoom()));
            ui->verticalScrollBar->setValue(viewer->cameraPos.y);
            ui->verticalScrollBar->blockSignals(false);

            viewer->screens->position.x = viewer->cameraPos.x;
            viewer->screens->position.y = viewer->cameraPos.y;

            break;
        }

        // You cant have a focus here & viewer, so copy code :]
        case QEvent::KeyPress: viewerActive = true; HandleKeyPress(static_cast<QKeyEvent *>(event)); break;
        case QEvent::KeyRelease: viewerActive = true; HandleKeyRelease(static_cast<QKeyEvent *>(event)); break;

        default: return false;
    }

    return false;
}

int SceneEditorv5::AddEntity(int type, float x, float y)
{
    if (x == 0xFFFF)
        x = viewer->cameraPos.x;

    if (y == 0xFFFF)
        y = viewer->cameraPos.y;

    SceneEntity entity;
    entity.slotID   = viewer->entities.count();
    entity.prevSlot = entity.slotID;
    entity.type     = type;
    entity.pos.x    = x;
    entity.pos.y    = y;

    entity.type = viewer->selectedObject;
    if (viewer->selectedObject < 0)
        entity.type = 0; // backup

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

    int cnt         = viewer->entities.count();
    entity.slotID   = cnt;
    entity.prevSlot = entity.slotID;

    CallGameEvent(viewer->objects[entity.type].name, SceneViewer::EVENT_CREATE, &entity);

    viewer->entities.append(entity);
    viewer->selectedEntity      = cnt;
    viewer->sceneInfo.listPos   = viewer->entities[viewer->selectedEntity].slotID;
    viewer->sceneInfoV1.listPos = viewer->sceneInfo.listPos;

    ui->entityList->blockSignals(true);
    ui->entityList->setCurrentRow(viewer->selectedEntity);
    ui->entityList->blockSignals(false);

    QString name = "Unknown Object " + QString::number(entity.type);
    if (entity.type < viewer->objects.count())
        name = viewer->objects[entity.type].name;

    ui->entityList->addItem(QString::number(entity.slotID) + ": " + name);

    objProp->setupUI(&viewer->entities[viewer->selectedEntity]);
    ui->propertiesBox->setCurrentWidget(ui->objPropPage);

    return entity.slotID;
}

void SceneEditorv5::PasteEntity(SceneEntity *copy, float x, float y)
{
    if (x == 0xFFFF)
        x = viewer->cameraPos.x;

    if (y == 0xFFFF)
        y = viewer->cameraPos.y;

    SceneEntity entity;
    entity.slotID   = viewer->entities.count();
    entity.prevSlot = entity.slotID;
    entity.type     = copy->type;
    entity.pos.x    = x;
    entity.pos.y    = y;

    entity.variables.clear();
    for (int v = 0; v < viewer->objects[entity.type].variables.length(); ++v) {
        RSDKv5::Scene::VariableValue var;
        var.type = viewer->objects[entity.type].variables[v].type;
        entity.variables.append(var);
    }

    for (int v = 0; v < viewer->objects[entity.type].variables.count(); ++v)
        entity.variables[v] = copy->variables[v];

    int cnt         = viewer->entities.count();
    entity.slotID   = cnt;
    entity.prevSlot = entity.slotID;

    CallGameEvent(viewer->objects[entity.type].name, SceneViewer::EVENT_CREATE, &entity);

    viewer->entities.append(entity);
    viewer->selectedEntity      = cnt;
    viewer->sceneInfo.listPos   = viewer->entities[viewer->selectedEntity].slotID;
    viewer->sceneInfoV1.listPos = viewer->sceneInfo.listPos;

    ui->entityList->blockSignals(true);
    ui->entityList->setCurrentRow(viewer->selectedEntity);
    ui->entityList->blockSignals(false);

    QString name = "Unknown Object " + QString::number(entity.type);
    if (entity.type < viewer->objects.count())
        name = viewer->objects[entity.type].name;

    ui->entityList->addItem(QString::number(entity.slotID) + ": " + name);

    objProp->setupUI(&viewer->entities[viewer->selectedEntity]);
    ui->propertiesBox->setCurrentWidget(ui->objPropPage);
}

void SceneEditorv5::DeleteEntity(int slot, bool updateUI)
{
    const SceneEntity &entity = viewer->entities.takeAt(slot);

    if (entity.gameEntity) {
        switch (viewer->engineRevision) {
            case 1: AS_ENTITY(entity.gameEntity, GameEntityv1)->classID = 0; break;

            case 2: AS_ENTITY(entity.gameEntity, GameEntityv2)->classID = 0; break;

            default:
            case 3: AS_ENTITY(entity.gameEntity, GameEntityvU)->classID = 0; break;
        }
    }

    if (viewer->entities.count() <= 0) {
        viewer->selectedEntity      = -1;
        viewer->sceneInfo.listPos   = -1;
        viewer->sceneInfoV1.listPos = viewer->sceneInfo.listPos;
    }

    if (viewer->selectedEntity == slot) {
        viewer->selectedEntity      = -1;
        viewer->sceneInfo.listPos   = -1;
        viewer->sceneInfoV1.listPos = viewer->sceneInfo.listPos;
    }

    ui->entityList->blockSignals(true);
    delete ui->entityList->item(slot);
    ui->entityList->blockSignals(false);
    if (updateUI) {
        objProp->unsetUI();

        ui->entityList->blockSignals(true);
        ui->entityList->setCurrentRow(viewer->selectedEntity);
        ui->entityList->blockSignals(false);
    }
}

void SceneEditorv5::CreateNewScene(QString scnPath, bool prePlus, bool loadGC, QString gcPath)
{
    SetStatus("Creating new scene...", true);

    viewer->disableDrawScene = true;
    viewer->stopTimer();
    viewer->unloadScene();

    releaseStorage(dataStorage);
    initStorage(dataStorage);
    AddStatusProgress(0.2); // finish unloading

    if (loadGC){
        if (QFileInfo(gcPath).suffix().toLower().contains("xml"))
            ParseGameXML(gcPath);
        else
            gameConfig.read(gcPath, prePlus);
    }

    dataPath = QFileInfo(gcPath).absolutePath();
    QDir dir(dataPath);
    dir.cdUp();
    dataPath = dir.path();
    viewer->dataPath = dataPath;

    // load the base data folder for game launch / game.dll failsafe
    if (!appConfig.baseDataManager[ENGINE_v5].dataPath.isEmpty())
        WorkingDirManager::workingDir = appConfig.baseDataManager[ENGINE_v5].dataPath + "/";

    gameLinkPath = dataPath + "/../";
    LoadGameLinks();

    if (gameLinks.count() == 0) {
        // As a fail-safe, try loading a game.dll from the game manager path
        gameLinkPath = WorkingDirManager::workingDir + "/../";
        LoadGameLinks();
        // Can't load objects properly on a new scene without Game.dll, so we REALLY need one bud.
        if (gameLinks.count() == 0){
            QMessageBox dllMessage(QMessageBox::Information, "RetroED",
                                   QString("Game.dll not found, select the location of the file"),
                                   QMessageBox::Ok);
            dllMessage.exec();

            QFileDialog gdlldialog(this, tr("Locate Game.dll Folder"), "",
                        tr("RSDKv5 Game Library (Game*.dll)"));
            gdlldialog.setFileMode(QFileDialog::Directory);
            gdlldialog.setAcceptMode(QFileDialog::AcceptOpen);
            if (gdlldialog.exec() == QDialog::Accepted) {
                gameLinkPath = gdlldialog.selectedFiles()[0];
            } else{
                SetStatus("Scene Creation Cancelled, no Game.dll found");
                AddStatusProgress(1);
                return;
            }
            LoadGameLinks();
        }
    }

    // loading
    QString basePath = scnPath + "/";
    scene.filepath = "";

    viewer->currentFolder  = QDir(basePath).dirName();
    viewer->currentSceneID = "X";

    viewer->metadata = RSDKv5::Scene::SceneEditorMetadata();;
    viewer->stamps   = RSDKv5::Stamps();

    viewer->useLayerScrollInfo = true; // use v5 style scroll Info

    scene.layers.clear();
    viewer->layers.clear();

    SceneHelpers::TileLayer layer;
    layer.name = "New Layer 0";
    layer.type = SceneHelpers::TileLayer::LAYER_HSCROLL - 1;
    layer.drawGroup = 2;
    layer.parallaxFactor = 0;
    layer.scrollSpeed = 0;
    viewer->layers.append(layer);


    QString pathTCF = WorkingDirManager::GetPath("Stages/" + viewer->currentFolder + "/TileConfig.bin",
                                                 basePath + "TileConfig.bin");
    QString pathSCF = WorkingDirManager::GetPath("Stages/" + viewer->currentFolder + "/StageConfig.bin",
                                                 basePath + "StageConfig.bin");

    QString pathSTP =
        WorkingDirManager::GetPath("Stages/" + viewer->currentFolder + "/" + viewer->metadata.stampName,
                                   basePath + viewer->metadata.stampName);

    if (QFile::exists(pathSTP))
        viewer->stamps.read(pathSTP);
    else
        viewer->stamps = RSDKv5::Stamps();

    if (QFile::exists(pathTCF))
        tileconfig.read(pathTCF);
    else
        tileconfig = RSDKv5::TileConfig();
    viewer->tileconfig = tileconfig;

    if (QFile::exists(pathSCF))
        stageConfig.read(pathSCF);
    else
        stageConfig = RSDKv5::StageConfig();
    stageConfig.loadGlobalObjects = loadGC;

    AddStatusProgress(1. / 6); // finish scene loading

    QImage tileset(16, 0x400 * 16, QImage::Format_Indexed8);
    for (int i = 0; i < 256; ++i)
        tileset.setColor(i, QRgb(0xFFFF00FF));
    tileset.fill(0);
    if (QFile::exists(basePath + "16x16Tiles.gif")) {
        QGifImage tilesetGif(basePath + "16x16Tiles.gif");
        tileset = tilesetGif.frame(0);
    }

    viewer->initScene(tileset);

    AddStatusProgress(1. / 6); // finish tileset loading

    scene.objects.clear();

    RSDKv5::Scene::SceneObject blankObj;
    blankObj.name = RSDKv5::Scene::NameIdentifier("Blank Object");
    scene.objects.insert(0, blankObj);

    int id = 1;

    if (stageConfig.loadGlobalObjects){
        for (QString &global : gameConfig.objects){
            RSDKv5::Scene::SceneObject objInfo;
            objInfo.name = global;
            objInfo.variables.clear();
            scene.objects.insert(id, objInfo);
            ++id;
        }
    }

    if (QFile::exists(pathSCF)){
        for (QString &stage : stageConfig.objects){
            RSDKv5::Scene::SceneObject objInfo;
            objInfo.name = stage;
            objInfo.variables.clear();
            scene.objects.insert(id, objInfo);
            ++id;
        }
    }

    InitGameLink();

    SetupObjects();

    ui->layerList->blockSignals(true);
    ui->layerList->clear();
    QListWidgetItem *item = new QListWidgetItem(viewer->layers[0].name, ui->layerList);
    item->setCheckState(viewer->layers[0].visible ? Qt::Checked : Qt::Unchecked);
    item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
    ui->layerList->blockSignals(false);

    ui->objectList->blockSignals(true);
    ui->objectList->clear();
    for (int o = 0; o < viewer->objects.count(); ++o) {
        QListWidgetItem *item = new QListWidgetItem(viewer->objects[o].name, ui->objectList);
        item->setCheckState(viewer->objects[o].visible ? Qt::Checked : Qt::Unchecked);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
    }
    ui->objectList->blockSignals(false);

    CreateEntityList();

    viewer->sceneBoundsL = 0;
    viewer->sceneBoundsT = 0;
    viewer->sceneBoundsR = 0;
    viewer->sceneBoundsB = 0;
    for (int l = 0; l < viewer->layers.count(); ++l) {
        if (viewer->sceneBoundsR < viewer->layers[l].width)
            viewer->sceneBoundsR = viewer->layers[l].width;
        if (viewer->sceneBoundsB < viewer->layers[l].height)
            viewer->sceneBoundsB = viewer->layers[l].height;
    }

    viewer->refreshResize();

    for (int c = 0; c < 2; c++){
        viewer->colTex[c] = new QImage(viewer->sceneBoundsR * viewer->tileSize,
                                    viewer->sceneBoundsB * viewer->tileSize, QImage::Format_RGB888);
        viewer->colTex[c]->setColorTable(
            { qRgb(0, 0, 0), qRgb(255, 255, 0), qRgb(255, 0, 0), qRgb(255, 255, 255) });
    }

    ui->horizontalScrollBar->setMaximum(viewer->storedW);
    ui->verticalScrollBar->setMaximum(viewer->storedH);
    ui->horizontalScrollBar->setPageStep(0x10);
    ui->verticalScrollBar->setPageStep(0x10);

    scnProp->setupUI(&scene, &stageConfig);

    scnProp->loadGlobalCB->blockSignals(true);
    scnProp->loadGlobalCB->setChecked(stageConfig.loadGlobalObjects);
    scnProp->loadGlobalCB->blockSignals(false);

    scnProp->stampNameEdit->blockSignals(true);
    scnProp->stampNameEdit->setText(viewer->metadata.stampName);
    scnProp->stampNameEdit->blockSignals(false);

    // TEMP!!!!
    scnProp->unknown1->blockSignals(true);
    scnProp->unknown1->setValue(viewer->metadata.unknown1);
    scnProp->unknown1->blockSignals(false);

    scnProp->unknown2->blockSignals(true);
    scnProp->unknown2->setValue(viewer->metadata.unknown2);
    scnProp->unknown2->blockSignals(false);

    scnProp->unknown3->blockSignals(true);
    scnProp->unknown3->setValue(viewer->metadata.unknown3);
    scnProp->unknown3->blockSignals(false);

    scnProp->unknown4->blockSignals(true);
    scnProp->unknown4->setValue(viewer->metadata.unknown4);
    scnProp->unknown4->blockSignals(false);

    scnProp->unknown5->blockSignals(true);
    scnProp->unknown5->setValue(viewer->metadata.unknown5);
    scnProp->unknown5->blockSignals(false);

    scnProp->unknown6->blockSignals(true);
    scnProp->unknown6->setValue(viewer->metadata.unknown6);
    scnProp->unknown6->blockSignals(false);

    scnProp->unknown7->blockSignals(true);
    scnProp->unknown7->setValue(viewer->metadata.unknown7);
    scnProp->unknown7->blockSignals(false);

    scnProp->unknown8->blockSignals(true);
    scnProp->unknown8->setValue(viewer->metadata.unknown8);
    scnProp->unknown8->blockSignals(false);

    scnProp->unknown9->blockSignals(true);
    scnProp->unknown9->setValue(viewer->metadata.unknown9);
    scnProp->unknown9->blockSignals(false);

    scnProp->syncGC->setDisabled(!stageConfig.loadGlobalObjects);

    scnProp->bgSel->setColor(viewer->metadata.backgroundColor1);
    scnProp->altBGSel->setColor(viewer->metadata.backgroundColor2);

    if (tileSel) {
        ui->tileListFrame->layout()->removeWidget(tileSel);
        delete tileSel;
    }

    tileSel = new TileSelector(this);
    tileSel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->tileListFrame->layout()->addWidget(tileSel);
    tileSel->show();

    ui->layerList->setCurrentRow(0);
    viewer->selectedLayer = 0;

    ui->toolBox->setCurrentIndex(0);
    ui->propertiesBox->setCurrentIndex(0);

    ui->showCollisionA->blockSignals(true);
    ui->showCollisionB->blockSignals(true);

    ui->showCollisionA->setChecked(false);
    ui->showCollisionB->setChecked(false);
    viewer->showPlaneA = false;
    viewer->showPlaneB = false;

    ui->showCollisionA->blockSignals(false);
    ui->showCollisionB->blockSignals(false);

    scnProp->setupUI(&scene, &stageConfig);
    lyrProp->setupUI(viewer, 0);
    tileProp->setupUI(&tileconfig.collisionPaths[0][0], &tileconfig.collisionPaths[1][0],
                      0, viewer->tiles[0]);
    tileProp->unsetUI();
    objProp->unsetUI();
    scrProp->unsetUI();

    AddStatusProgress(1. / 6); // finish setting up UI stuff

    InitGameLink();

    QCheckBox *filterToggles[] = { ui->filterBox1, ui->filterBox2, ui->filterBox3, ui->filterBox4,
                                   ui->filterBox5, ui->filterBox6, ui->filterBox7, ui->filterBox8 };

    for (int f = 0; f < 8; ++f) {
        filterToggles[f]->setDisabled(viewer->engineRevision == 1);
        filterToggles[f]->setChecked(viewer->engineRevision != 1
                                     || Utils::getBit(viewer->sceneFilter, f));
    }

    tabTitle = viewer->currentFolder + "/Scene" + viewer->currentSceneID;

    ClearActions();
    UpdateTitle(true);
    SetStatus("Created new scene!"); // done!

    viewer->disableDrawScene = false;
    viewer->disableObjects   = false;

    viewer->startTimer();
}

void SceneEditorv5::LoadScene(QString scnPath, QString gcfPath, byte sceneVer)
{
    viewer->disableDrawScene = true;

    SetStatus("Loading Scene...", true);

    viewer->stopTimer();
    viewer->unloadScene();

    releaseStorage(dataStorage);
    initStorage(dataStorage);
    AddStatusProgress(1. / 6); // finish unloading

    if (gcfPath != gameConfig.filePath) {
        if (QFileInfo(gcfPath).suffix().toLower().contains("xml"))
            ParseGameXML(gcfPath);
        else
            gameConfig.read(gcfPath, sceneVer == 1);
        dataPath = QFileInfo(gcfPath).absolutePath();
        QDir dir(dataPath);
        dir.cdUp();
        dataPath = dir.path();
    }

    viewer->dataPath = dataPath;

    gameLinkPath = dataPath + "/../";

    // load the base data folder for game launch / game.dll failsafe
    if (!appConfig.baseDataManager[ENGINE_v5].dataPath.isEmpty())
        WorkingDirManager::workingDir = appConfig.baseDataManager[ENGINE_v5].dataPath + "/";

    LoadGameLinks();

    // As a fail-safe, try loading a game.dll from the game manager path
    if (gameLinks.count() == 0 && !appConfig.baseDataManager[ENGINE_v5].dataPath.isEmpty()) {
        gameLinkPath = WorkingDirManager::workingDir + "/../";
        LoadGameLinks();
    }

    // loading
    QString pth      = scnPath;
    QString basePath = pth.replace(QFileInfo(pth).fileName(), "");

    viewer->currentFolder  = QDir(basePath).dirName();
    viewer->currentSceneID = QFileInfo(scnPath).baseName().toLower().replace("scene", "");


    AddStatusProgress(1. / 6); // finish initial setup

    scene.read(scnPath);
    viewer->metadata = scene.editorMetadata;

    viewer->useLayerScrollInfo = true; // use v5 style scroll Info

    viewer->layers.clear();
    for (auto &layer : scene.layers) {
        SceneHelpers::TileLayer viewLayer;

        viewLayer.name           = layer.name;
        viewLayer.width          = layer.width;
        viewLayer.height         = layer.height;
        viewLayer.drawGroup      = layer.drawGroup;
        viewLayer.visible        = layer.visible;
        viewLayer.parallaxFactor = layer.parallaxFactor / 256.0f;
        viewLayer.scrollSpeed    = layer.scrollSpeed / 256.0f;

        viewLayer.type = SceneHelpers::TileLayer::LAYER_NONE;
        switch (layer.type) {
            case 0: viewLayer.type = SceneHelpers::TileLayer::LAYER_HSCROLL; break;
            case 1: viewLayer.type = SceneHelpers::TileLayer::LAYER_VSCROLL; break;
            case 2: viewLayer.type = SceneHelpers::TileLayer::LAYER_ROTOZOOM; break;
            case 3: viewLayer.type = SceneHelpers::TileLayer::LAYER_BASIC; break;
        }

        viewLayer.scrollInfos.clear();
        for (auto &info : layer.scrollInfos) {
            SceneHelpers::TileLayer::ScrollIndexInfo scroll;

            scroll.parallaxFactor = info.parallaxFactor;
            scroll.scrollSpeed    = info.scrollSpeed;
            scroll.deform         = info.deform;
            scroll.unknown        = info.unknown;

            for (auto &instance : info.instances) {
                SceneHelpers::TileLayer::ScrollInstance inst;

                inst.startLine = instance.startLine;
                inst.length    = instance.length;
                inst.layerID   = 0xFF; // for v5, we don't care about layer ID
                scroll.instances.append(inst);
            }

            viewLayer.scrollInfos.append(scroll);
        }

        viewLayer.layout.clear();
        for (int y = 0; y < layer.height; ++y) {
            viewLayer.layout.append(QList<ushort>());
            for (int x = 0; x < layer.width; ++x) viewLayer.layout[y].append(layer.layout[y][x]);
        }

        viewer->layers.append(viewLayer);
    }

    QString pathTCF = WorkingDirManager::GetPath("Stages/" + viewer->currentFolder + "/TileConfig.bin",
                                                 basePath + "TileConfig.bin");
    QString pathSCF = WorkingDirManager::GetPath("Stages/" + viewer->currentFolder + "/StageConfig.bin",
                                                 basePath + "StageConfig.bin");

    tileconfig.read(pathTCF);
    viewer->tileconfig = tileconfig;

    stageConfig.read(pathSCF);

    if (viewer->metadata.stampName == "")
        viewer->metadata.stampName = "StampList.bin";

    QString pathSTP = WorkingDirManager::GetPath("Stages/" + viewer->currentFolder + "/" + viewer->metadata.stampName,
                                                 basePath + viewer->metadata.stampName);

    if (QFile::exists(pathSTP))
        viewer->stamps.read(pathSTP);
    else
        viewer->stamps = RSDKv5::Stamps();

    AddStatusProgress(1. / 6); // finish scene loading

    // Tile Texture
    QImage tileset(16, 0x400 * 16, QImage::Format_Indexed8);
    for (int i = 0; i < 256; ++i)
        tileset.setColor(i, QRgb(0xFFFF00FF));
    tileset.fill(0);
    if (QFile::exists(basePath + "16x16Tiles.gif")) {
        QGifImage tilesetGif(basePath + "16x16Tiles.gif");
        tileset = tilesetGif.frame(0);
    }
    viewer->initScene(tileset);

    AddStatusProgress(1. / 6); // finish tileset loading

    ClearActions();
    SetupObjects();

    ui->layerList->blockSignals(true);
    ui->layerList->clear();
    for (int l = 0; l < viewer->layers.count(); ++l) {
        QListWidgetItem *item = new QListWidgetItem(viewer->layers[l].name, ui->layerList);
        item->setCheckState(viewer->layers[l].visible ? Qt::Checked : Qt::Unchecked);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
    }
    ui->layerList->blockSignals(false);

    ui->objectList->blockSignals(true);
    ui->objectList->clear();
    for (int o = 0; o < viewer->objects.count(); ++o) {
        QListWidgetItem *item = new QListWidgetItem(viewer->objects[o].name, ui->objectList);
        item->setCheckState(viewer->objects[o].visible ? Qt::Checked : Qt::Unchecked);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
    }
    ui->objectList->blockSignals(false);

    CreateEntityList();

    viewer->sceneBoundsL = 0;
    viewer->sceneBoundsT = 0;
    viewer->sceneBoundsR = 0;
    viewer->sceneBoundsB = 0;
    for (int l = 0; l < viewer->layers.count(); ++l) {
        if (viewer->sceneBoundsR < viewer->layers[l].width)
            viewer->sceneBoundsR = viewer->layers[l].width;
        if (viewer->sceneBoundsB < viewer->layers[l].height)
            viewer->sceneBoundsB = viewer->layers[l].height;
    }

    viewer->refreshResize();

    for (int c = 0; c < 2; c++){
        viewer->colTex[c] = new QImage(viewer->sceneBoundsR * viewer->tileSize,
                                    viewer->sceneBoundsB * viewer->tileSize, QImage::Format_RGB888);
        viewer->colTex[c]->setColorTable(
            { qRgb(0, 0, 0), qRgb(255, 255, 0), qRgb(255, 0, 0), qRgb(255, 255, 255) });
    }


    ui->horizontalScrollBar->setMaximum((viewer->sceneBoundsR * viewer->tileSize) - viewer->storedW);
    ui->verticalScrollBar->setMaximum((viewer->sceneBoundsB * viewer->tileSize) - viewer->storedH);
    ui->horizontalScrollBar->setPageStep(0x10);
    ui->verticalScrollBar->setPageStep(0x10);

    scnProp->setupUI(&scene, &stageConfig);

    scnProp->loadGlobalCB->blockSignals(true);
    scnProp->loadGlobalCB->setChecked(stageConfig.loadGlobalObjects);
    scnProp->loadGlobalCB->blockSignals(false);

    scnProp->stampNameEdit->blockSignals(true);
    scnProp->stampNameEdit->setText(viewer->metadata.stampName);
    scnProp->stampNameEdit->blockSignals(false);

    // TEMP!!!!
    scnProp->unknown1->blockSignals(true);
    scnProp->unknown1->setValue(viewer->metadata.unknown1);
    scnProp->unknown1->blockSignals(false);

    scnProp->unknown2->blockSignals(true);
    scnProp->unknown2->setValue(viewer->metadata.unknown2);
    scnProp->unknown2->blockSignals(false);

    scnProp->unknown3->blockSignals(true);
    scnProp->unknown3->setValue(viewer->metadata.unknown3);
    scnProp->unknown3->blockSignals(false);

    scnProp->unknown4->blockSignals(true);
    scnProp->unknown4->setValue(viewer->metadata.unknown4);
    scnProp->unknown4->blockSignals(false);

    scnProp->unknown5->blockSignals(true);
    scnProp->unknown5->setValue(viewer->metadata.unknown5);
    scnProp->unknown5->blockSignals(false);

    scnProp->unknown6->blockSignals(true);
    scnProp->unknown6->setValue(viewer->metadata.unknown6);
    scnProp->unknown6->blockSignals(false);

    scnProp->unknown7->blockSignals(true);
    scnProp->unknown7->setValue(viewer->metadata.unknown7);
    scnProp->unknown7->blockSignals(false);

    scnProp->unknown8->blockSignals(true);
    scnProp->unknown8->setValue(viewer->metadata.unknown8);
    scnProp->unknown8->blockSignals(false);

    scnProp->unknown9->blockSignals(true);
    scnProp->unknown9->setValue(viewer->metadata.unknown9);
    scnProp->unknown9->blockSignals(false);

    scnProp->syncGC->setDisabled(!stageConfig.loadGlobalObjects);

    scnProp->bgSel->setColor(viewer->metadata.backgroundColor1);
    scnProp->altBGSel->setColor(viewer->metadata.backgroundColor2);

    if (tileSel) {
        ui->tileListFrame->layout()->removeWidget(tileSel);
        delete tileSel;
    }

    tileSel = new TileSelector(this);
    tileSel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->tileListFrame->layout()->addWidget(tileSel);
    tileSel->show();

    ui->layerList->setCurrentRow(0);
    viewer->selectedLayer = 0;

    ui->toolBox->setCurrentIndex(0);
    ui->propertiesBox->setCurrentIndex(0);

    ui->showCollisionA->blockSignals(true);
    ui->showCollisionB->blockSignals(true);

    ui->showCollisionA->setChecked(false);
    ui->showCollisionB->setChecked(false);
    viewer->showPlaneA = false;
    viewer->showPlaneB = false;

    ui->showCollisionA->blockSignals(false);
    ui->showCollisionB->blockSignals(false);

    scnProp->setupUI(&scene, &stageConfig);
    lyrProp->setupUI(viewer, 0);
    tileProp->setupUI(&tileconfig.collisionPaths[0][0], &tileconfig.collisionPaths[1][0],
                      0, viewer->tiles[0]);
    tileProp->unsetUI();
    objProp->unsetUI();
    scrProp->unsetUI();

    AddStatusProgress(1. / 6); // finish setting up UI stuff

    InitGameLink();

    QCheckBox *filterToggles[] = { ui->filterBox1, ui->filterBox2, ui->filterBox3, ui->filterBox4,
                                   ui->filterBox5, ui->filterBox6, ui->filterBox7, ui->filterBox8 };

    for (int f = 0; f < 8; ++f) {
        filterToggles[f]->setDisabled(viewer->engineRevision == 1);
        filterToggles[f]->setChecked(viewer->engineRevision != 1
                                     || Utils::getBit(viewer->sceneFilter, f));
    }

    ui->addEnt->setDisabled(viewer->activeEntityCount() >= 0x800);

    tabTitle = Utils::getFilenameAndFolder(scnPath);

    appConfig.addRecentFile(ENGINE_v5, TOOL_SCENEEDITOR, scnPath,
                            QList<QString>{ gcfPath, gameConfig.readFilter ? "2" : "1" });
    SetStatus("Loaded scene " + QFileInfo(scnPath).fileName()); // done!

    viewer->disableDrawScene = false;
    viewer->disableObjects   = false;
    viewer->startTimer();
}

void SceneEditorv5::SaveScene(QString path)
{
    // saving
    viewer->disableDrawScene = true;
    SetStatus("Saving scene...", true);

    QString pth      = path;
    QString basePath = pth.replace(QFileInfo(pth).fileName(), "");

    scene.editorMetadata = viewer->metadata;

    scene.layers.clear();
    for (auto &viewLayer : viewer->layers) {
        RSDKv5::Scene::SceneLayer layer;

        layer.name           = viewLayer.name;
        layer.width          = viewLayer.width;
        layer.height         = viewLayer.height;
        layer.drawGroup      = viewLayer.drawGroup;
        layer.visible        = viewLayer.visible;
        layer.parallaxFactor = viewLayer.parallaxFactor * 256;
        layer.scrollSpeed    = viewLayer.scrollSpeed * 256;

        layer.type = 0;
        switch (viewLayer.type) {
            case SceneHelpers::TileLayer::LAYER_HSCROLL:  layer.type = 0; break;
            case SceneHelpers::TileLayer::LAYER_VSCROLL:  layer.type = 1; break;
            case SceneHelpers::TileLayer::LAYER_ROTOZOOM: layer.type = 2; break;
            case SceneHelpers::TileLayer::LAYER_BASIC:    layer.type = 3; break;
        }

        layer.scrollInfos.clear();
        for (auto &info : viewLayer.scrollInfos) {
            RSDKv5::Scene::ScrollIndexInfo scroll;

            scroll.parallaxFactor = info.parallaxFactor;
            scroll.scrollSpeed    = info.scrollSpeed;
            scroll.deform         = info.deform;
            scroll.unknown        = info.unknown;

            for (auto &instance : info.instances) {
                RSDKv5::Scene::ScrollInstance inst;

                inst.startLine = instance.startLine;
                inst.length    = instance.length;
                scroll.instances.append(inst);
            }

            layer.scrollInfos.append(scroll);
        }

        layer.layout.clear();
        for (int y = 0; y < viewLayer.height; ++y) {
            layer.layout.append(QVector<ushort>());
            for (int x = 0; x < viewLayer.width; ++x) layer.layout[y].append(viewLayer.layout[y][x]);
        }

        scene.layers.append(layer);
    }
    AddStatusProgress(1.f / 5); // created tile layers

    scene.objects.clear();
    for (SceneObject &obj : viewer->objects) {
        RSDKv5::Scene::SceneObject object;
        object.name = RSDKv5::Scene::NameIdentifier(obj.name);
        object.variables.clear();
        object.entities.clear();
        for (int v = 0; v < obj.variables.count(); ++v) {
            RSDKv5::Scene::VariableInfo variable;
            variable.name = RSDKv5::Scene::NameIdentifier(obj.variables[v].name);
            variable.type = (RSDKv5::VariableTypes)obj.variables[v].type;
            object.variables.append(variable);
        }
        scene.objects.append(object);
    }
    AddStatusProgress(1.f / 5); // created object definitions

    for (SceneEntity &entity : viewer->entities) {
        if (entity.type < viewer->objects.count()) {
            RSDKv5::Scene::SceneEntity ent;
            ent.slotID     = entity.slotID;
            ent.position.x = entity.pos.x * 65536.0f;
            ent.position.y = entity.pos.y * 65536.0f;

            ent.variables.clear();
            for (int v = 0; v < entity.variables.count(); ++v) {
                ent.variables.append(entity.variables[v]);
            }

            ent.parent = &scene.objects[entity.type];
            scene.objects[entity.type].entities.append(ent);
        }
        else {
            // what...?
        }
    }
    AddStatusProgress(1.f / 5); // created entity list

    FormatHelpers::Gif tileset(16, 0x400 * 16);

    int c = 0;
    for (PaletteColor &col : viewer->tilePalette) tileset.palette[c++] = col.toQColor();

    int pos = 0;
    for (int i = 0; i < 0x400; ++i) {
        uchar *src = viewer->tiles[i].bits();
        for (int y = 0; y < 16; ++y) {
            for (int x = 0; x < 16; ++x) tileset.pixels[pos++] = *src++;
        }
    }
    AddStatusProgress(1.f / 5); // generated tileset

    scene.write(path);
    tileconfig.write(basePath + "TileConfig.bin");
    stageConfig.write(basePath + "StageConfig.bin");
    viewer->stamps.write(basePath + viewer->metadata.stampName);
    tileset.write(basePath + "16x16Tiles.gif");

    tabTitle = Utils::getFilenameAndFolder(path);
    ClearActions();
    appConfig.addRecentFile(ENGINE_v5, TOOL_SCENEEDITOR, path, QList<QString>{ gameConfig.filePath });
    SetStatus("Saved scene to " + Utils::getFilenameAndFolder(scene.filepath)); // written scene
    viewer->disableDrawScene = false;
}

void SceneEditorv5::FilterObjectList(QString filter)
{
    bool showAll = filter.length() == 0;

    for (int row = 0; row < ui->objectList->count(); ++row) {
        auto *item = ui->objectList->item(row);
        item->setHidden(!showAll && !item->text().toUpper().contains(filter));
    }
}

void SceneEditorv5::FilterEntityList(QString filter)
{
    bool showAll = filter.length() == 0;

    for (int row = 0; row < ui->entityList->count(); ++row) {
        auto *item = ui->entityList->item(row);
        item->setHidden(!showAll && !item->text().toUpper().contains(filter));
    }
}

void SceneEditorv5::CreateEntityList(int startSlot)
{
    ui->entityList->blockSignals(true);

    if (startSlot <= 0) {
        ui->entityList->clear();
        startSlot = 0;
    }
    else {
        for (int i = ui->entityList->count(); i >= startSlot; --i) delete ui->entityList->item(i);
    }

    std::sort(viewer->entities.begin(), viewer->entities.end(),
              [](const SceneEntity &a, const SceneEntity &b) -> bool { return a.slotID < b.slotID; });
    for (int i = startSlot; i < viewer->entities.count(); ++i) {
        QString name = "Unknown Object " + QString::number(viewer->entities[i].type);
        if (viewer->entities[i].type < viewer->objects.count())
            name = viewer->objects[viewer->entities[i].type].name;

        ui->entityList->addItem(QString::number(viewer->entities[i].slotID) + ": " + name);
    }
    ui->entityList->blockSignals(false);
}

void SceneEditorv5::CreateScrollList(bool update)
{
    if (update) {
        if (viewer->selectedLayer < 0 || viewer->selectedLayer >= viewer->layers.count())
            return;

        if (viewer->selectedHScrollInfo < 0
            || viewer->selectedHScrollInfo >= viewer->layers[viewer->selectedLayer].scrollInfos.count())
            return;

        ui->scrollList->blockSignals(true);

        SceneHelpers::TileLayer::ScrollIndexInfo &info =
            viewer->layers[viewer->selectedLayer].scrollInfos[viewer->selectedHScrollInfo];
        ui->scrollList->item(viewer->selectedHScrollInfo)
            ->setText(
                QString("Parallax: %1, Speed: %2").arg(info.parallaxFactor).arg(info.scrollSpeed));

        ui->scrollList->blockSignals(false);
    }
    else {
        ui->scrollList->clear();

        if (viewer->selectedLayer < 0 || viewer->selectedLayer >= viewer->layers.count())
            return;

        ui->scrollList->blockSignals(true);
        for (int i = 0; i < viewer->layers[viewer->selectedLayer].scrollInfos.count(); ++i) {
            SceneHelpers::TileLayer::ScrollIndexInfo &info =
                viewer->layers[viewer->selectedLayer].scrollInfos[i];
            ui->scrollList->addItem(
                QString("Parallax: %1, Speed: %2").arg(info.parallaxFactor).arg(info.scrollSpeed));
        }

        ui->scrollList->blockSignals(false);
        ui->scrollList->setCurrentRow(-1);
    }
}

void SceneEditorv5::SetupObjects()
{
    viewer->objects.clear();
    viewer->entities.clear();

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

        bool repeatedObj = false;
        int repeatObjID = -1;
        // Check for repeated objects
        for (int i = 0; i < viewer->objects.count(); i++){
            if (viewer->objects[i].name == object.name){
                repeatedObj = true;
                repeatObjID = i;
            }
        }

        if (!repeatedObj){
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

                viewer->entities.append(entity);
            }
            viewer->objects.append(object);
            ++type;
        } else {
            for (RSDKv5::Scene::SceneEntity &ent : obj.entities) {
                SceneEntity entity;
                entity.slotID   = ent.slotID;
                entity.prevSlot = entity.slotID;
                entity.type     = repeatObjID;
                entity.pos.x    = Utils::fixedToFloat(ent.position.x);
                entity.pos.y    = Utils::fixedToFloat(ent.position.y);

                for (int v = 0; v < ent.variables.count(); ++v) {
                    entity.variables.append(ent.variables[v]);
                }

                viewer->entities.append(entity);
            }
        }
    }

    // obj type 0 should always be blank obj
    if (viewer->objects[0].name != "Blank Object"){
        bool blankFlag = false;
        int blkObjPos = 0;
        // check if the object exists later on the list
        for (SceneObject &obj : viewer->objects) {
            if (obj.name == "Blank Object") {
                blankFlag = true;
                break;
            }
            blkObjPos++;
        }

        if (blankFlag && blkObjPos != 0){
            // exist in the wrong position, let's deal with that
            viewer->objects.removeAt(blkObjPos);
            SceneObject blankObj;
            blankObj.name = "Blank Object";
            viewer->objects.insert(0, blankObj);
            for (SceneEntity &ent : viewer->entities) {
                if (ent.type == blkObjPos)
                    ent.type = 0;
                else if (ent.type < blkObjPos)
                    ent.type++;
            }
        } else if (!blankFlag){
            // doesn't exist at all, add it
            SceneObject blankObj;
            blankObj.name = "Blank Object";
            viewer->objects.insert(0, blankObj);
            for (SceneEntity &ent : viewer->entities) { ent.type++; }
        }
    }

    // Make super duper sure the objects on the list match the order from the GC/SC list
    // Hi pre-RE2 edited stages, it's not your fault
    objNames.removeAt(1);
    bool sortedList = true;
    for (int n = 0; n < viewer->objects.count(); n++){
        if (n >= objNames.count() || viewer->objects[n].name != objNames[n]){
            sortedList = false;
            break;
        }
    }

    if (!sortedList){
        QApplication::beep();
        QMessageBox::StandardButton warning = QMessageBox::question(this,
            "Object List Error", "The object list of the Scene differs from the intended order, re-sort?\n"
                              "(invalid objects and entities may be lost)", QMessageBox::Yes|QMessageBox::No);
        if (warning == QMessageBox::No)
            return;
        QList<QString> preSortNames;
        QList<SceneObject> preSortObjs;
        QList<int> preSortID;
        for (int i = 0; i < viewer->objects.count(); ++i) {
            preSortNames.append(viewer->objects[i].name);
            preSortObjs.append(viewer->objects[i]);
            preSortID.append(-1);
        }
        preSortID[0] = 0;

        viewer->objects.clear();


        int objTypeID = 0;

        for (QString &obj : objNames){
            int index = preSortNames.indexOf(obj);
            if (index < 0)
                index = preSortNames.indexOf(Utils::getMd5HashString(obj));

            SceneObject objInfo;
            objInfo.name = obj;

            if (index >= 0) {
                preSortID[index] = objTypeID;
                objInfo.variables.clear();
                for (auto &var : preSortObjs[index].variables)
                    objInfo.variables.append(var);
            }

            viewer->objects.insert(objTypeID, objInfo);

            ++objTypeID;
        }

        for (int o = viewer->entities.count() - 1; o >= 0; --o) {
            SceneEntity &entity = viewer->entities[o];
            if (entity.type >= 0){
                int newType = preSortID[entity.type];
                if (newType < 0)
                    DeleteEntity(o, false);
                else
                    entity.type = preSortID[entity.type];
            }
        }

        DoAction("Object List re-sorted");
    }
}

void SceneEditorv5::UnloadGameLinks()
{
    for (int o = 4; o < v5_SURFACE_MAX; ++o) {
        if (viewer->gfxSurface[o].scope == SCOPE_STAGE) {
            if (viewer->gfxSurface[o].texturePtr)
                delete viewer->gfxSurface[o].texturePtr;
            viewer->gfxSurface[o].texturePtr = nullptr;
            viewer->gfxSurface[o].scope      = SCOPE_NONE;
        }
    }

    for (int a = 0; a < v5_SPRFILE_COUNT; ++a) {
        viewer->spriteAnimationList[a].scope      = SCOPE_NONE;
        viewer->spriteAnimationList[a].animations = NULL;
        viewer->spriteAnimationList[a].frames     = NULL;
    }

    for (int l = 0; l < gameLinks.count(); ++l) {
        gameLinks[l].unload();
    }
    gameLinks.clear();

    dataStorage[DATASET_STG].usedStorage = 0;
    dataStorage[DATASET_STG].entryCount  = 0;
    dataStorage[DATASET_STG].clearCount  = 0;
    memset(dataStorage[DATASET_STG].dataEntries, 0, sizeof(dataStorage[DATASET_STG].dataEntries));
    memset(dataStorage[DATASET_STG].storageEntries, 0, sizeof(dataStorage[DATASET_STG].storageEntries));

    dataStorage[DATASET_STR].usedStorage = 0;
    dataStorage[DATASET_STR].entryCount  = 0;
    dataStorage[DATASET_STR].clearCount  = 0;
    memset(dataStorage[DATASET_STR].dataEntries, 0, sizeof(dataStorage[DATASET_STR].dataEntries));
    memset(dataStorage[DATASET_STR].storageEntries, 0, sizeof(dataStorage[DATASET_STR].storageEntries));

    dataStorage[DATASET_TMP].usedStorage = 0;
    dataStorage[DATASET_TMP].entryCount  = 0;
    dataStorage[DATASET_TMP].clearCount  = 0;
    memset(dataStorage[DATASET_TMP].dataEntries, 0, sizeof(dataStorage[DATASET_TMP].dataEntries));
    memset(dataStorage[DATASET_TMP].storageEntries, 0, sizeof(dataStorage[DATASET_TMP].storageEntries));
}

void SceneEditorv5::LoadGameLinks()
{
    UnloadGameLinks();

    QDirIterator it(gameLinkPath, QStringList() << "*", QDir::Files,
                    QDirIterator::NoIteratorFlags);
    while (it.hasNext()) {
        QString filePath = it.next();

        if (QLibrary::isLibrary(filePath)) {
            GameLink link;
            gameLinks.append(link);
            gameLinks.last().LinkGameObjects(filePath);
        }
    }
}

void SceneEditorv5::InitGameLink()
{
    viewer->gameEntityList = NULL;
    viewer->engineRevision = 3;

    if (gameLinks.count()) {
        GameLink &link         = gameLinks.first();
        viewer->engineRevision = link.revision;
    }

    switch (viewer->engineRevision) {
        case 1: viewer->gameEntityList = viewer->gameEntityListv1; break;

        case 2: viewer->gameEntityList = viewer->gameEntityListv2; break;

        default:
        case 3: viewer->gameEntityList = viewer->gameEntityListvU; break;
    }

    for (int i = 0; i < viewer->objects.count(); ++i) {
        GameObjectInfo *info = GetObjectInfo(viewer->objects[i].name);
        if (info) {
            allocateStorage(dataStorage, info->staticClassSize, (void **)info->sVars, DATASET_STG,
                            true);

            CallGameEvent(info->name, SceneViewer::EVENT_STATICLOAD, NULL);

            GameObject *sVars = *info->sVars;
            sVars->objectID   = i;
            sVars->active     = ACTIVE_NORMAL;
        }
    }

    switch (viewer->engineRevision) {
        case 1: break;

        case 2:
            for (int i = 0; i < viewer->objects.count(); ++i) {
                if (gameConfig.readFilter)
                    FunctionTable::SetEditableVar(VAR_UINT8, "filter", i,
                                                  offsetof(GameEntityv2, filter));
            }
            break;

        default:
        case 3:
            for (int i = 0; i < viewer->objects.count(); ++i) {
                if (gameConfig.readFilter)
                    FunctionTable::SetEditableVar(VAR_UINT8, "filter", i,
                                                  offsetof(GameEntityvU, filter));
            }
            break;
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

    for (int i = 0; i < viewer->objects.count() && gameLinks.count(); ++i) {
        bool called = CallGameEvent(viewer->objects[i].name, SceneViewer::EVENT_SERIALIZE, NULL);

        if (called) { // only erase vars if serialize was called, otherwise its likely we just dont have
                      // a game link
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
    }

    // EditorLoad should have all the info before being called
    for (int i = 0; i < viewer->objects.count() && gameLinks.count(); ++i) {
        CallGameEvent(viewer->objects[i].name, SceneViewer::EVENT_LOAD, NULL);
    }

    for (int i = 0; i < viewer->entities.count() && gameLinks.count(); ++i) {
        SceneEntity *entity = &viewer->entities[i];
        CallGameEvent(viewer->objects[entity->type].name, SceneViewer::EVENT_CREATE, entity);
    }
}

void SceneEditorv5::SetTile(float x, float y)
{
    if (viewer->selectedLayer < 0)
        return;

    float tx = x;
    float ty = y;

    tx *= viewer->invZoom();
    ty *= viewer->invZoom();

    float tx2 = tx + fmodf(viewer->cameraPos.x, 0x10);
    float ty2 = ty + fmodf(viewer->cameraPos.y, 0x10);

    // clip to grid
    tx -= fmodf(tx2, 0x10);
    ty -= fmodf(ty2, 0x10);

    float xpos = tx + viewer->cameraPos.x;
    float ypos = ty + viewer->cameraPos.y;

    xpos /= 0x10;
    ypos /= 0x10;

    ushort tile = viewer->selectedTile;
    if (!copiedTile) {  // only use this if the tile is not copied from the stage
        Utils::setBit(tile, viewer->tileFlip.x, 10);
        Utils::setBit(tile, viewer->tileFlip.y, 11);
        Utils::setBit(tile, viewer->tileSolidA.x, 12);
        Utils::setBit(tile, viewer->tileSolidA.y, 13);
        Utils::setBit(tile, viewer->tileSolidB.x, 14);
        Utils::setBit(tile, viewer->tileSolidB.y, 15);
    }

    if (viewer->selectedTile == 0xFFFF)
        tile = 0xFFFF;

    if (ypos >= 0 && ypos < viewer->layers[viewer->selectedLayer].height) {
        if (xpos >= 0 && xpos < viewer->layers[viewer->selectedLayer].width) {
            viewer->layers[viewer->selectedLayer].layout[ypos][xpos] = tile;
        }
    }

}

void SceneEditorv5::SetStamp(float x, float y)
{
    if (viewer->selectedLayer < 0)
        return;
    if (viewer->selectedStamp >= 0x400 && viewer->selectedStamp != 0xFFFF)
        return;

    int tx = x;
    int ty = y;

    tx *= viewer->invZoom();
    ty *= viewer->invZoom();

    int tx2 = tx + fmodf(viewer->cameraPos.x, 0x10);
    int ty2 = ty + fmodf(viewer->cameraPos.y, 0x10);

    // clip to grid
    tx -= fmodf(tx2, 0x10);
    ty -= fmodf(ty2, 0x10);

    int xpos = tx + viewer->cameraPos.x;
    int ypos = ty + viewer->cameraPos.y;

    xpos /= 0x10;
    ypos /= 0x10;

    auto stamp = viewer->stamps.stampList[viewer->selectedStamp];

    // Invalid stamp position prevention
    if (0 > stamp.pos.x || stamp.pos.x + stamp.size.x > viewer->layers[viewer->selectedLayer].width ||
        0 > stamp.pos.y || stamp.pos.y + stamp.size.y > viewer->layers[viewer->selectedLayer].height){
        SetStatus("Invalid stamp position in current layer");
        return;
    }

    for(int y = 0; y < stamp.size.y; y++){
        for(int x = 0; x < stamp.size.x; x++){
            int tileXPos = stamp.pos.x + x;
            int tileYPos = stamp.pos.y + y;

            ushort tile = viewer->layers[viewer->selectedLayer].layout[tileYPos][tileXPos];

            if (tile != 0xFFFF) {
                if (ypos + y >= 0 && ypos + y < viewer->layers[viewer->selectedLayer].height) {
                    if (xpos + x >= 0 && xpos + x < viewer->layers[viewer->selectedLayer].width) {
                        viewer->layers[viewer->selectedLayer].layout[ypos + y][xpos + x] = tile;
                    }
                }
            }
        }
    }
}

void SceneEditorv5::AddStamp(float x, float y)
{
    if (viewer->selectedLayer < 0)
        return;
    if (viewer->selectedStamp >= 0x400 && viewer->selectedStamp != 0xFFFF)
        return;

    int tx = x;
    int ty = y;

    tx *= viewer->invZoom();
    ty *= viewer->invZoom();

    int tx2 = tx + fmodf(viewer->cameraPos.x, 0x10);
    int ty2 = ty + fmodf(viewer->cameraPos.y, 0x10);

    // clip to grid
    tx -= fmodf(tx2, 0x10);
    ty -= fmodf(ty2, 0x10);

    int xpos = tx + viewer->cameraPos.x;
    int ypos = ty + viewer->cameraPos.y;

    xpos /= 0x10;
    ypos /= 0x10;

    int w = ((int)viewer->selectSize.x / 0x10);
    int h = ((int)viewer->selectSize.y / 0x10);

    RSDKv5::Stamps::StampEntry stamp;
    stamp.name = "New Stamp";
    if (w > 0) {
        stamp.pos.x = xpos;
        stamp.size.x = w;
    } else {
        stamp.pos.x = xpos + w;
        stamp.size.x = -w;
    }

    if (h > 0) {
        stamp.pos.y = ypos;
        stamp.size.y = h;
    } else {
        stamp.pos.y = ypos + h;
        stamp.size.y = -h;
    }
    uint c = ui->stampList->count();
    viewer->stamps.stampList.insert(c, stamp);

    ui->stampList->blockSignals(true);

    auto *item = new QListWidgetItem();
    item->setText("New Stamp");
    ui->stampList->addItem(item);
    item->setFlags(item->flags());

    ui->stampList->blockSignals(false);
    ui->stampList->setCurrentItem(item);
}


void SceneEditorv5::ResetTools(byte tool)
{
    if (tool == 0xFF)
        tool = SceneViewer::TOOL_MOUSE;
    viewer->curTool = tool;

    ui->panTool->blockSignals(true);
    ui->selectTool->blockSignals(true);
    ui->pencilTool->blockSignals(true);
    ui->stampTool->blockSignals(true);
    ui->eraserTool->blockSignals(true);
    ui->entityTool->blockSignals(true);
    ui->stampCopy->blockSignals(true);

    ui->panTool->setDown(false);
    ui->selectTool->setDown(false);
    ui->pencilTool->setDown(false);
    ui->stampTool->setDown(false);
    ui->eraserTool->setDown(false);
    ui->entityTool->setDown(false);
    ui->stampCopy->setDown(false);

    switch (tool) {
        default: break; // what
        case SceneViewer::TOOL_MOUSE: ui->panTool->setDown(true); break;
        case SceneViewer::TOOL_SELECT: ui->selectTool->setDown(true); break;
        case SceneViewer::TOOL_PENCIL: ui->pencilTool->setDown(true); break;
        case SceneViewer::TOOL_STAMP: ui->stampTool->setDown(true); break;
        case SceneViewer::TOOL_ERASER: ui->eraserTool->setDown(true); break;
        case SceneViewer::TOOL_ENTITY: ui->entityTool->setDown(true); break;
        case SceneViewer::TOOL_STAMP_MAKER: ui->stampCopy->setDown(true); break;
    }

    ui->panTool->blockSignals(false);
    ui->selectTool->blockSignals(false);
    ui->pencilTool->blockSignals(false);
    ui->stampTool->blockSignals(false);
    ui->eraserTool->blockSignals(false);
    ui->entityTool->blockSignals(false);
    ui->stampCopy->blockSignals(false);

    // Reset
    viewer->selectedObject      = -1;
    viewer->selectedEntity      = -1;
    viewer->sceneInfo.listPos   = -1;
    viewer->sceneInfoV1.listPos = viewer->sceneInfo.listPos;

    objProp->unsetUI();
    viewer->isSelecting  = false;
    viewer->selectPos.x  = 0;
    viewer->selectPos.y  = 0;
    viewer->selectSize.x = 0;
    viewer->selectSize.y = 0;
    viewer->selectedEntities.clear();

    ui->entityList->blockSignals(true);
    ui->entityList->setCurrentRow(viewer->selectedEntity);
    ui->entityList->blockSignals(false);

    unsetCursor();

    switch (tool) {
        default: break; // what
        case SceneViewer::TOOL_MOUSE: break;
        case SceneViewer::TOOL_SELECT: break;
        case SceneViewer::TOOL_PENCIL: viewer->isSelecting = true; break;
        case SceneViewer::TOOL_STAMP: break;
        case SceneViewer::TOOL_ERASER: viewer->isSelecting = true; break;
        case SceneViewer::TOOL_ENTITY: viewer->isSelecting = true; break;
    }
}

bool SceneEditorv5::HandleKeyPress(QKeyEvent *event)
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
        && event->key() == Qt::Key_V && !event->isAutoRepeat() && viewerActive) {
        if (clipboard) {
            switch (clipboardType) {
                default: break;
                case COPY_ENTITY: {
                    if (viewer->activeEntityCount() < 0x800) {
                        SceneEntity *entity = (SceneEntity *)clipboard;

                        PasteEntity(entity, sceneMousePos.x, sceneMousePos.y);

                        DoAction(QString("Pasted Entity: %1 (%2, %3)")
                                     .arg(viewer->objects[entity->type].name)
                                     .arg(sceneMousePos.x)
                                     .arg(sceneMousePos.y));
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

    if ((event->modifiers() & Qt::ControlModifier) == Qt::ControlModifier
        && event->key() == Qt::Key_G && viewerActive) {
        auto *sel = new GoToPos(viewer->layers[viewer->selectedLayer].width * 16, viewer->layers[viewer->selectedLayer].height * 16, viewer->layers[viewer->selectedLayer].name, this);
        if (sel->exec() == QDialog::Accepted) {
            viewer->cameraPos.x = sel->posX - ((viewer->storedW / 2) * viewer->invZoom());
            viewer->cameraPos.y = sel->posY - ((viewer->storedH / 2) * viewer->invZoom());
            if (viewer->cameraPos.x < 0) viewer->cameraPos.x = 0;
            if (viewer->cameraPos.y < 0) viewer->cameraPos.y = 0;
            viewer->screens->position.x = viewer->cameraPos.x;
            viewer->screens->position.y = viewer->cameraPos.y;

            ui->horizontalScrollBar->blockSignals(true);
            ui->horizontalScrollBar->setValue(viewer->cameraPos.x);
            ui->horizontalScrollBar->blockSignals(false);

            ui->verticalScrollBar->blockSignals(true);
            ui->verticalScrollBar->setValue(viewer->cameraPos.y);
            ui->verticalScrollBar->blockSignals(false);
        }
    }

    byte prevTool = viewer->curTool;
    byte tool     = viewer->curTool;
    if (!ctrlDownL && !altDownL && !shiftDownL){
        if (event->key() == Qt::Key_S)
            tool = SceneViewer::TOOL_MOUSE;
        if (event->key() == Qt::Key_W)
            tool = SceneViewer::TOOL_SELECT;
        if (event->key() == Qt::Key_A)
            tool = SceneViewer::TOOL_PENCIL;
        if (event->key() == Qt::Key_K)
            tool = SceneViewer::TOOL_STAMP;
        if (event->key() == Qt::Key_L)
            tool = SceneViewer::TOOL_STAMP_MAKER;
        if (event->key() == Qt::Key_R)
            tool = SceneViewer::TOOL_ERASER;
        if (event->key() == Qt::Key_E)
            tool = SceneViewer::TOOL_ENTITY;
    }

    if (tool != prevTool)
        ResetTools(tool);

    switch (viewer->curTool) {
        case SceneViewer::TOOL_MOUSE: break;

        case SceneViewer::TOOL_SELECT:
            if (viewer->selectedEntities.count()) {
                if (event->key() == Qt::Key_Delete || event->key() == Qt::Key_Backspace) {

                    std::sort(viewer->selectedEntities.begin(), viewer->selectedEntities.end(),
                              [](const int &a, const int &b) -> bool { return a < b; });

                    for (int s = viewer->selectedEntities.count() - 1; s >= 0; --s) {
                        DeleteEntity(viewer->selectedEntities[s], false);
                    }

                    objProp->unsetUI();
                    CreateEntityList();
                    viewer->selectedEntities.clear();
                }

                for (auto o : viewer->selectedEntities){
                    int move = 4;
                    if (ctrlDownL)
                        move *= 4;
                    if (shiftDownL)
                        move /= 4;

                    if (event->key() == Qt::Key_Up) {
                        viewer->entities[o].pos.y -= move;
                    }
                    if (event->key() == Qt::Key_Down) {
                        viewer->entities[o].pos.y += move;
                    }
                    if (event->key() == Qt::Key_Left) {
                        viewer->entities[o].pos.x -= move;
                    }
                    if (event->key() == Qt::Key_Right) {
                        viewer->entities[o].pos.x += move;
                    }
                    if ((event->key() == Qt::Key_Up || event->key() == Qt::Key_Down
                        || event->key() == Qt::Key_Left || event->key() == Qt::Key_Right) && o == viewer->selectedEntity)
                        objProp->updateUI();
                }
            }
            break;

        case SceneViewer::TOOL_PENCIL:
            if (viewerActive){
                if (event->key() == Qt::Key_Z){
                    bool flipX  = Utils::getBit(viewer->selectedTile, 10) ^ true;
                    Utils::setBit(viewer->selectedTile, flipX, 10);
                    tileProp->updatePropFlags(flipX, 10);
                }
                if (event->key() == Qt::Key_X){
                    bool flipY  = Utils::getBit(viewer->selectedTile, 11) ^ true;
                    Utils::setBit(viewer->selectedTile, flipY, 11);
                    tileProp->updatePropFlags(flipY, 11);
                }
            }

            if (event->key() == Qt::Key_Delete || event->key() == Qt::Key_Backspace) {
                if (viewer->selectedLayer >= 0) {
                    Rect<float> box;

                    for (int y = 0; y < viewer->layers[viewer->selectedLayer].height; ++y) {
                        for (int x = 0; x < viewer->layers[viewer->selectedLayer].width; ++x) {
                            box = Rect<float>(x * 0x10, y * 0x10, 0x10, 0x10);

                            Vector2<float> pos = Vector2<float>(
                                (viewer->tilePos.x * viewer->invZoom()) + viewer->cameraPos.x,
                                (viewer->tilePos.y * viewer->invZoom()) + viewer->cameraPos.y);
                            if (box.contains(pos)) {
                                viewer->layers[viewer->selectedLayer].layout[y][x] = 0xFFFF;

                                DoAction(QString("Erased Tile: (%1, %2)").arg(pos.x).arg(pos.y));

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
            if (viewer->selectedEntity > -1) {
                if (event->key() == Qt::Key_Delete || event->key() == Qt::Key_Backspace) {
                    DeleteEntity(viewer->selectedEntity);
                    viewer->selectedEntity = -1;

                    DoAction();
                }

                int move = 4;
                if (ctrlDownL)
                    move *= 4;
                if (shiftDownL)
                    move /= 4;

                if (event->key() == Qt::Key_Up) {
                    viewer->entities[viewer->selectedEntity].pos.y -= move;
                    objProp->updateUI();
                }
                if (event->key() == Qt::Key_Down) {
                    viewer->entities[viewer->selectedEntity].pos.y += move;
                    objProp->updateUI();
                }
                if (event->key() == Qt::Key_Left) {
                    viewer->entities[viewer->selectedEntity].pos.x -= move;
                    objProp->updateUI();
                }
                if (event->key() == Qt::Key_Right) {
                    viewer->entities[viewer->selectedEntity].pos.x += move;
                    objProp->updateUI();
                }

                if ((event->modifiers() & Qt::ControlModifier) == Qt::ControlModifier
                    && event->key() == Qt::Key_C) {
                    clipboard     = &viewer->entities[viewer->selectedEntity];
                    clipboardType = COPY_ENTITY;
                    clipboardInfo = viewer->selectedEntity;
                }
            }
            break;
    }

    return false;
}
bool SceneEditorv5::HandleKeyRelease(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Control)
        ctrlDownL = false;
    if (event->key() == Qt::Key_Alt)
        altDownL = false;
    if (event->key() == Qt::Key_Shift)
        shiftDownL = false;

    return false;
}

GameObjectInfo *SceneEditorv5::GetObjectInfo(QString name)
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

void SceneEditorv5::SetGameEntityVariables(SceneEntity *entity, void *gameEntity)
{
    byte *entityBytes = (byte *)gameEntity;

    for (int o = 0; o < entity->variables.length(); o++) {
        auto var    = viewer->objects[entity->type].variables[o];
        auto val    = entity->variables[o];
        auto offset = &entityBytes[var.offset];

        switch (var.type) {
            case VAR_UINT8: memcpy(offset, &val.value_uint8, sizeof(byte)); break;
            case VAR_INT8: memcpy(offset, &val.value_int8, sizeof(sbyte)); break;
            case VAR_UINT16: memcpy(offset, &val.value_uint16, sizeof(ushort)); break;
            case VAR_INT16: memcpy(offset, &val.value_int16, sizeof(short)); break;
            case VAR_UINT32: memcpy(offset, &val.value_uint32, sizeof(uint)); break;
            case VAR_INT32: memcpy(offset, &val.value_int32, sizeof(int)); break;
            case VAR_ENUM: memcpy(offset, &val.value_enum, sizeof(int)); break;
            case VAR_STRING: {
                // FunctionTable::setText((TextInfo *)offset,
                //                        (char *)val.value_string.toStdString().c_str(), false);
                break;
            }
            case VAR_VECTOR2: memcpy(offset, &val.value_vector2.x, sizeof(Vector2<int>)); break;
            case VAR_FLOAT: memcpy(offset, &val.value_float, sizeof(float)); break;
            case VAR_BOOL: {
                bool32 value = val.value_bool;
                memcpy(offset, &value, sizeof(bool32));
                break;
            }
            case VAR_COLOR: {
                auto c   = val.value_color;
                uint clr = (c.red() << 16) | (c.green() << 8) | (c.blue());
                memcpy(offset, &clr, sizeof(uint));
                break;
            }
        }
    }
}

void SceneEditorv5::GetGameEntityVariables(SceneEntity *entity, void *gameEntity)
{
    byte *entityBytes = (byte *)gameEntity;

    for (int o = 0; o < entity->variables.length(); o++) {
        auto var    = viewer->objects[entity->type].variables[o];
        auto &val   = entity->variables[o];
        auto offset = &entityBytes[var.offset];
        switch (var.type) {
            case VAR_UINT8: memcpy(&val.value_uint8, offset, sizeof(byte)); break;
            case VAR_INT8: memcpy(&val.value_int8, offset, sizeof(sbyte)); break;
            case VAR_UINT16: memcpy(&val.value_uint16, offset, sizeof(ushort)); break;
            case VAR_INT16: memcpy(&val.value_int16, offset, sizeof(short)); break;
            case VAR_UINT32: memcpy(&val.value_uint32, offset, sizeof(uint)); break;
            case VAR_INT32: memcpy(&val.value_int32, offset, sizeof(int)); break;
            case VAR_ENUM: memcpy(&val.value_enum, offset, sizeof(int)); break;
            case VAR_STRING: {
                // char buffer[0x100];
                // FunctionTable::getCString(buffer, (TextInfo *)offset);
                // val.value_string = buffer;
                break;
            }
            case VAR_VECTOR2:
                memcpy(&val.value_vector2.x, offset, sizeof(Vector2<int>));
                val.value_vector2f.x = Utils::fixedToFloat(val.value_vector2.x);
                val.value_vector2f.y = Utils::fixedToFloat(val.value_vector2.y);
                break;
            case VAR_FLOAT: memcpy(&val.value_float, offset, sizeof(float)); break;
            case VAR_BOOL: {
                bool32 value = false;
                memcpy(&value, offset, sizeof(bool32));
                val.value_bool = value;
                break;
            }
            case VAR_COLOR: {
                uint clr = 0;
                memcpy(&clr, offset, sizeof(uint));
                val.value_color = QColor(clr);
                break;
            }
        }
    }
}

bool SceneEditorv5::CallGameEvent(QString objName, byte eventID, SceneEntity *entity)
{
    bool called = false;

    GameObjectInfo *info = GetObjectInfo(objName);

    if (!info)
        return called;

    viewer->foreachStackPtr = viewer->foreachStackList;
    switch (eventID) {
        default: break;

        case SceneViewer::EVENT_STATICLOAD:
            if (info->staticLoad) {
                info->staticLoad(*info->sVars);
                called = true;
            }
            break;

        case SceneViewer::EVENT_LOAD:
            if (info->editorLoad) {
                info->editorLoad();
                called = true;
            }
            break;

        case SceneViewer::EVENT_CREATE: {
            if (!entity)
                return called;

            switch (viewer->engineRevision) {
                case 1: {
                    GameEntityv1 *entityPtr =
                        entity->slotID == 0xFFFF
                            ? AS_ENTITY(entity->gameEntity, GameEntityBasev1)
                            : &AS_ENTITY(viewer->gameEntityList, GameEntityBasev1)[entity->slotID];
                    memset((void *)entityPtr, 0, sizeof(GameEntityBasev1));
                    entityPtr->position.x    = Utils::floatToFixed(entity->pos.x);
                    entityPtr->position.y    = Utils::floatToFixed(entity->pos.y);
                    entityPtr->interaction   = true;
                    entityPtr->active        = ACTIVE_BOUNDS;
                    entityPtr->updateRange.x = 0x80 << 16;
                    entityPtr->updateRange.y = 0x80 << 16;
                    entityPtr->scale.x       = 0x200;
                    entityPtr->scale.y       = 0x200;
                    entityPtr->classID       = entity->type;

                    entity->gameEntity = (void *)entityPtr;
                    break;
                }

                case 2: {
                    GameEntityv2 *entityPtr =
                        entity->slotID == 0xFFFF
                            ? AS_ENTITY(entity->gameEntity, GameEntityBasev2)
                            : &AS_ENTITY(viewer->gameEntityList, GameEntityBasev2)[entity->slotID];
                    memset((void *)entityPtr, 0, sizeof(GameEntityBasev2));
                    entityPtr->position.x    = Utils::floatToFixed(entity->pos.x);
                    entityPtr->position.y    = Utils::floatToFixed(entity->pos.y);
                    entityPtr->interaction   = true;
                    entityPtr->active        = ACTIVE_BOUNDS;
                    entityPtr->updateRange.x = 0x80 << 16;
                    entityPtr->updateRange.y = 0x80 << 16;
                    entityPtr->scale.x       = 0x200;
                    entityPtr->scale.y       = 0x200;
                    entityPtr->classID       = entity->type;

                    entity->gameEntity = (void *)entityPtr;
                    break;
                }

                default:
                case 3: {
                    GameEntityvU *entityPtr =
                        entity->slotID == 0xFFFF
                            ? AS_ENTITY(entity->gameEntity, GameEntityBasevU)
                            : &AS_ENTITY(viewer->gameEntityList, GameEntityBasevU)[entity->slotID];
                    memset((void *)entityPtr, 0, sizeof(GameEntityBasevU));
                    entityPtr->position.x    = Utils::floatToFixed(entity->pos.x);
                    entityPtr->position.y    = Utils::floatToFixed(entity->pos.y);
                    entityPtr->interaction   = true;
                    entityPtr->active        = ACTIVE_BOUNDS;
                    entityPtr->updateRange.x = 0x80 << 16;
                    entityPtr->updateRange.y = 0x80 << 16;
                    entityPtr->scale.x       = 0x200;
                    entityPtr->scale.y       = 0x200;
                    entityPtr->classID       = entity->type;

                    entity->gameEntity = (void *)entityPtr;
                    break;
                }
            }

            SetGameEntityVariables(entity, entity->gameEntity);

            viewer->sceneInfo.entity       = entity->gameEntity;
            viewer->sceneInfo.entitySlot   = entity->slotID;
            viewer->sceneInfoV1.entity     = entity->gameEntity;
            viewer->sceneInfoV1.entitySlot = entity->slotID;

            if (info->create && entity->gameEntity) {
                info->create(NULL);
                called = true;
            }

            viewer->sceneInfo.entity       = NULL;
            viewer->sceneInfo.entitySlot   = 0;
            viewer->sceneInfoV1.entity     = NULL;
            viewer->sceneInfoV1.entitySlot = 0;

            // editor defaults!
            // getGameEntityVariables(entity, entity->gameEntity);
            break;
        }

        case SceneViewer::EVENT_UPDATE:
            if (!entity)
                return called;

            // TODO: this(?)
            viewer->sceneInfo.entity       = entity->gameEntity;
            viewer->sceneInfo.entitySlot   = entity->slotID;
            viewer->sceneInfoV1.entity     = entity->gameEntity;
            viewer->sceneInfoV1.entitySlot = entity->slotID;

            if (info->update && entity->gameEntity) {
                info->update();
                called = true;
            }

            viewer->sceneInfo.entity       = NULL;
            viewer->sceneInfo.entitySlot   = 0;
            viewer->sceneInfoV1.entity     = NULL;
            viewer->sceneInfoV1.entitySlot = 0;
            break;

        case SceneViewer::EVENT_DRAW:
            if (!entity) {
                float ex = viewer->tilePos.x;
                float ey = viewer->tilePos.y;

                ex *= viewer->invZoom();
                ey *= viewer->invZoom();

                validDraw = false;
                switch (viewer->engineRevision) {
                    case 1:
                        createGameEntityv1.position.x = (ex + viewer->cameraPos.x) * 65536.0f;
                        createGameEntityv1.position.y = (ey + viewer->cameraPos.y) * 65536.0f;
                        createGameEntityv1.classID    = viewer->selectedObject;

                        createTempEntity.gameEntity = &createGameEntityv1;
                        break;

                    case 2:
                        createGameEntityv2.position.x = (ex + viewer->cameraPos.x) * 65536.0f;
                        createGameEntityv2.position.y = (ey + viewer->cameraPos.y) * 65536.0f;
                        createGameEntityv2.classID    = viewer->selectedObject;

                        createTempEntity.gameEntity = &createGameEntityv2;
                        break;

                    default:
                    case 3:
                        createGameEntityvU.position.x = (ex + viewer->cameraPos.x) * 65536.0f;
                        createGameEntityvU.position.y = (ey + viewer->cameraPos.y) * 65536.0f;
                        createGameEntityvU.classID    = viewer->selectedObject;

                        createTempEntity.gameEntity = &createGameEntityvU;
                        break;
                }

                createTempEntity.type   = viewer->selectedObject;
                createTempEntity.pos.x  = ex + viewer->cameraPos.x;
                createTempEntity.pos.y  = ey + viewer->cameraPos.y;
                createTempEntity.slotID = 0xFFFF;
                createTempEntity.box    = Rect<int>(0, 0, 0, 0);

                viewer->activeDrawEntity = &createTempEntity;

                entity = &createTempEntity;
            }

            if (entity->gameEntity) {
                switch (viewer->engineRevision) {
                    case 1:
                        AS_ENTITY(entity->gameEntity, GameEntityv1)->classID = entity->type;
                        AS_ENTITY(entity->gameEntity, GameEntityv1)->position.x =
                            Utils::floatToFixed(entity->pos.x);
                        AS_ENTITY(entity->gameEntity, GameEntityv1)->position.y =
                            Utils::floatToFixed(entity->pos.y);
                        break;

                    case 2:
                        AS_ENTITY(entity->gameEntity, GameEntityv2)->classID = entity->type;
                        AS_ENTITY(entity->gameEntity, GameEntityv2)->position.x =
                            Utils::floatToFixed(entity->pos.x);
                        AS_ENTITY(entity->gameEntity, GameEntityv2)->position.y =
                            Utils::floatToFixed(entity->pos.y);
                        break;

                    default:
                    case 3:
                        AS_ENTITY(entity->gameEntity, GameEntityvU)->classID = entity->type;
                        AS_ENTITY(entity->gameEntity, GameEntityvU)->position.x =
                            Utils::floatToFixed(entity->pos.x);
                        AS_ENTITY(entity->gameEntity, GameEntityvU)->position.y =
                            Utils::floatToFixed(entity->pos.y);
                        break;
                }

                SetGameEntityVariables(entity, entity->gameEntity);
            }

            viewer->sceneInfo.currentScreenID  = 0;
            viewer->sceneInfo.currentDrawGroup = 0; // TODO

            viewer->sceneInfo.debugMode  = false; // always start with overlay mode off
            viewer->sceneInfo.entity     = entity->gameEntity;
            viewer->sceneInfo.entitySlot = entity->slotID;

            viewer->sceneInfoV1.currentScreenID  = 0;
            viewer->sceneInfoV1.currentDrawGroup = 0; // TODO

            viewer->sceneInfoV1.debugMode  = false; // always start with overlay mode off
            viewer->sceneInfoV1.entity     = entity->gameEntity;
            viewer->sceneInfoV1.entitySlot = entity->slotID;

            if (info->editorDraw && entity->gameEntity) {
                info->editorDraw();
                called = true;
            }

            viewer->sceneInfo.entity       = NULL;
            viewer->sceneInfo.entitySlot   = 0;
            viewer->sceneInfoV1.entity     = NULL;
            viewer->sceneInfoV1.entitySlot = 0;
            break;

        case SceneViewer::EVENT_SERIALIZE:
            if (info->serialize) {
                info->serialize();
                called = true;
            }
            break;
    }

    return called;
}

void SceneEditorv5::ParseGameXML(QString path)
{
    gameConfig          = RSDKv5::GameConfig();
    gameConfig.filePath = path;

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
                gameConfig.objects.append(objName);
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
                list           = gameConfig.categories.count();
                gameConfig.categories.append(stageList);
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
                gameConfig.categories[list].scenes.append(stage);
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

void SceneEditorv5::UndoAction()
{
    if (actionIndex > 0) {
        actionIndex--;
        ResetAction();
    }
}
void SceneEditorv5::RedoAction()
{
    if (actionIndex + 1 < actions.count()) {
        actionIndex++;
        ResetAction();
    }
}

void SceneEditorv5::ResetAction()
{
#if RE_USE_UNSTABLE
    viewer->tilePalette = actions[actionIndex].tilePalette;
    viewer->tiles       = actions[actionIndex].tiles;

    gameConfig = actions[actionIndex].gameConfig;

    scene       = actions[actionIndex].scene;
    stageConfig = actions[actionIndex].stageConfig;
    tileconfig  = actions[actionIndex].tileconfig;

    // Objects & Entities
    viewer->objects  = actions[actionIndex].objects;
    viewer->entities = actions[actionIndex].entities;

    // General Editing
    // viewer->curTool     = actions[actionIndex].curTool;
    // viewer->isSelecting = actions[actionIndex].selecting;
    // viewer->mousePos    = actions[actionIndex].mousePos;

    // Layer Editing
    viewer->tilePos       = actions[actionIndex].tilePos;
    viewer->tileFlip      = actions[actionIndex].tileFlip;
    viewer->selectedLayer = actions[actionIndex].selectedLayer;

    // Collision
    // viewer->showPlaneA = actions[actionIndex].showPlaneA;
    // viewer->showPlaneB = actions[actionIndex].showPlaneB;

    // Entity Editing
    viewer->selectedObject      = actions[actionIndex].selectedObject; // placing
    viewer->selectedEntity      = actions[actionIndex].selectedEntity; // viewing
    viewer->sceneInfo.listPos   = viewer->selectedEntity;
    viewer->sceneInfoV1.listPos = viewer->selectedEntity;

    // Parallax Editing
    // viewer->showParallax       = actions[actionIndex].showParallax;
    viewer->selectedHScrollInfo = actions[actionIndex].selectedScrollInfo;

    // Camera
    // viewer->showGrid = actions[actionIndex].showGrid;
    // viewer->gridSize = actions[actionIndex].gridSize;

    // updating UI

    ui->layerList->blockSignals(true);
    ui->layerList->clear();
    for (int l = 0; l < viewer->layers.count(); ++l) {
        QListWidgetItem *item = new QListWidgetItem(viewer->layers[l].name, ui->layerList);
        item->setCheckState(viewer->layers[l].visible ? Qt::Checked : Qt::Unchecked);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
    }
    ui->layerList->blockSignals(false);

    ui->objectList->blockSignals(true);
    ui->objectList->clear();
    for (int o = 0; o < viewer->objects.count(); ++o) {
        QListWidgetItem *item = new QListWidgetItem(viewer->objects[o].name, ui->objectList);
        item->setCheckState(viewer->objects[o].visible ? Qt::Checked : Qt::Unchecked);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
    }
    ui->objectList->blockSignals(false);

    createEntityList();

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
#endif

    UpdateTitle(actionIndex > 0);

    // setStatus("redid Action: " + actions[actionIndex].name);
}

void SceneEditorv5::DoAction(QString name, bool setModified)
{
    // TODO: THIS IS VERY SLOW!! FIX IT!!
    ActionState action;

    action.name = name;

#if RE_USE_UNSTABLE
    action.tilePalette = viewer->tilePalette;
    action.tiles       = viewer->tiles;

    action.gameConfig = gameConfig;

    action.scene       = scene;
    action.stageConfig = stageConfig;
    action.tileconfig  = tileconfig;

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
    action.selectedScrollInfo = viewer->selectedHScrollInfo;
#endif

    // Actions
    for (int i = actions.count() - 1; i > actionIndex; --i) {
        actions.removeAt(i);
    }

    actions.append(action);
    actionIndex = actions.count() - 1;

    UpdateTitle(setModified);

    // setStatus("Did Action: " + name);
}
void SceneEditorv5::ClearActions()
{
    actions.clear();
    actionIndex = 0;
    DoAction("Action Setup", false); // first action, cant be undone
}

#include "moc_sceneeditorv5.cpp"
