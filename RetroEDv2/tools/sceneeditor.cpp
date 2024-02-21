#include "includes.hpp"
#include "ui_sceneeditor.h"
#include "qgifimage.h"
#include "sceneeditor.hpp"

#include "sceneproperties/sceneincludesv5.hpp"

#include "sceneproperties/sceneproperties.hpp"
#include "sceneproperties/scenelayerproperties.hpp"
#include "sceneproperties/sceneobjectproperties.hpp"
#include "sceneproperties/scenescrollproperties.hpp"
#include "sceneproperties/scenetileproperties.hpp"
#include "sceneproperties/tilereplaceoptions.hpp"
#include "sceneproperties/chunkreplaceoptions.hpp"

#include "sceneproperties/stageconfigeditorv1.hpp"
#include "sceneproperties/stageconfigeditorv2.hpp"
#include "sceneproperties/stageconfigeditorv3.hpp"
#include "sceneproperties/stageconfigeditorv4.hpp"

#include "paletteeditor.hpp"
#include "paletteeditor/colourdialog.hpp"

#include "sceneproperties/chunkeditor.hpp"
#include "sceneproperties/tileseteditor.hpp"
#include "sceneproperties/copyplane.hpp"
#include "sceneproperties/gotopos.hpp"

#include <RSDKv1/gfxv1.hpp>
#include <RSDKv1/tileconfigv1.hpp>
#include <RSDKv4/tileconfigv4.hpp>
#include <RSDKv4/scenev4.hpp>

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

ChunkSelector::ChunkSelector(QWidget *parent) : QWidget(parent), parentWidget((SceneEditor *)parent)
{
    memset(labels, 0, sizeof(labels));

    scrollArea = new QScrollArea(this);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scrollArea->setGeometry(10, 10, 200, 200);

    QWidget *chunkArea = new QWidget();

    QGridLayout *layout = new QGridLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    int i = 0;
    for (auto &&chunk : parentWidget->viewer->chunks) {
        auto *label = new ChunkLabel(&parentWidget->viewer->selectedChunk, i, chunkArea);
        label->setPixmap(QPixmap::fromImage(chunk).scaled(chunk.width(), chunk.height()));
        label->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        label->resize(chunk.width(), chunk.height());
        label->setFixedSize(chunk.width(), chunk.height() * 1.1f);
        layout->addWidget(label, i, 1);
        connect(label, &ChunkLabel::requestRepaint, chunkArea, QOverload<>::of(&QWidget::update));
        labels[i++] = label;
    }

    chunkArea->setLayout(layout);
    scrollArea->setWidget(chunkArea);
    QVBoxLayout *l = new QVBoxLayout(this);
    l->addWidget(scrollArea);
    setLayout(l);
}

void ChunkSelector::RefreshList()
{
    int i = 0;
    for (auto &&chunk : parentWidget->viewer->chunks) {
        labels[i++]->setPixmap(QPixmap::fromImage(chunk).scaled(chunk.width(), chunk.height()));
    }
}

void ChunkSelector::SetCurrentChunk(int chunkID)
{
    if (parentWidget->viewer->selectedChunk >= 0 && parentWidget->viewer->selectedChunk < 0x200
        && labels[parentWidget->viewer->selectedChunk]) {
        labels[parentWidget->viewer->selectedChunk]->update();
    }

    if (chunkID >= 0 && chunkID < 0x200 && labels[chunkID]) {
        scrollArea->ensureWidgetVisible(labels[chunkID]);
        labels[chunkID]->update();
    }
}

SceneEditor::SceneEditor(QWidget *parent) : QWidget(parent), ui(new Ui::SceneEditor)
{
    ui->setupUi(this);

    compilerv2 = new Compilerv2;
    compilerv3 = new Compilerv3;
    compilerv4 = new Compilerv4;

    viewer = new SceneViewer(ENGINE_NONE, this);
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

    ResetTools(SceneViewer::TOOL_MOUSE);

    viewer->statusLabel = ui->statusLabel;

    ui->toolBox->setCurrentIndex(0);
    ui->propertiesBox->setCurrentIndex(0);

    scnProp->gridX->setValue(viewer->gridSize.x);
    scnProp->gridY->setValue(viewer->gridSize.y);

    connect(viewer, &SceneViewer::callGameEvent,
            [this](byte eventID, int entityID) { CallGameEvent(eventID, entityID); });

    connect(ui->horizontalScrollBar, &QScrollBar::valueChanged,
            [this](int v) { viewer->cameraPos.x = v; });

    connect(ui->verticalScrollBar, &QScrollBar::valueChanged,
            [this](int v) { viewer->cameraPos.y = v; });

#ifndef Q_NO_PROCESS
    connect(ui->runGame, &QPushButton::clicked, [this]{
        QString gamePath = appConfig.gameManager[viewer->gameType].exePath;
        if (QFile::exists(gamePath)) {
            QStringList args;
            args << "console=true;" << QString("stage=%1;").arg(argInitStage) << QString("scene=%1;").arg(argInitScene);
            QProcess proc;
            proc.setProgram(gamePath);
            proc.setWorkingDirectory(QFileInfo(gamePath).absolutePath());
            proc.setArguments(args);
            proc.startDetached();
            proc.waitForStarted();
        }
    });
#else
    ui->runGame->setVisible(false);
#endif

    connect(ui->useGizmos, &QPushButton::clicked, [this] { viewer->sceneInfo.effectGizmo ^= 1; });

    connect(ui->layerList, &QListWidget::currentRowChanged, [this](int c) {
        if (c == -1)
            return;

        viewer->selectedLayer = c;

        lyrProp->setupUI(viewer, c);
        ui->propertiesBox->setCurrentWidget(ui->layerPropPage);

        CreateScrollList();
        ui->rmScrH->setDisabled(c < 1);
        ui->rmScrV->setDisabled(c < 1);
    });

    connect(ui->layerList, &QListWidget::itemChanged, [this](QListWidgetItem *item) {
        int c = ui->layerList->row(item);
        if ((uint)c < (uint)9)
            viewer->layers[c].visible = item->checkState() == Qt::Checked;
    });

    connect(ui->objectFilter, &QLineEdit::textChanged, [this](QString s) { FilterObjectList(s.toUpper()); });

    connect(ui->objectList, &QListWidget::currentRowChanged, [this](int c) {
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

        viewer->selectedObject = c;
        ui->rmObj->setDisabled(c == -1 || global);
    });

    connect(ui->objectList, &QListWidget::itemChanged, [this](QListWidgetItem *item) {
        int c = ui->objectList->row(item);
        if ((uint)c < (uint)viewer->objects.count())
            viewer->objects[c].visible = item->checkState() == Qt::Checked;
    });

    connect(ui->addObj, &QToolButton::clicked, [this] {
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
        DoAction();
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

        CreateEntityList();
        DoAction();
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

        viewer->selectedEntity = c;

        viewer->cameraPos.x = viewer->entities[c].pos.x - ((viewer->storedW / 2) * viewer->invZoom());
        viewer->cameraPos.y = viewer->entities[c].pos.y - ((viewer->storedH / 2) * viewer->invZoom());

        ui->horizontalScrollBar->blockSignals(true);
        ui->horizontalScrollBar->setValue(viewer->cameraPos.x);
        ui->horizontalScrollBar->blockSignals(false);

        ui->verticalScrollBar->blockSignals(true);
        ui->verticalScrollBar->setValue(viewer->cameraPos.y);
        ui->verticalScrollBar->blockSignals(false);

        // THIS IS REALLY *REALLY* SLOW, TODO: FIX/SPEED UP
        auto *entity = &viewer->entities[viewer->selectedEntity];
        objProp->setupUI(entity, viewer->selectedEntity,
                         &compilerv2->objectEntityList[entity->gameEntitySlot],
                         &compilerv3->objectEntityList[entity->gameEntitySlot],
                         &compilerv4->objectEntityList[entity->gameEntitySlot], viewer->gameType);
        ui->propertiesBox->setCurrentWidget(ui->objPropPage);
    });
    connect(objProp, &SceneObjectProperties::typeChanged, this, &SceneEditor::updateType);

    connect(ui->addEnt, &QToolButton::clicked, [this] {
        uint c = viewer->entities.count();
        uint entType = (viewer->selectedObject > -1 ? viewer->selectedObject : 0);

        AddEntity(entType, viewer->cameraPos.x + ((viewer->storedW / 2) * viewer->invZoom()), viewer->cameraPos.y + ((viewer->storedH / 2) * viewer->invZoom()));
        ui->entityList->setCurrentRow(c);
        viewer->selectedEntity = c;

        ui->addEnt->setDisabled(viewer->entities.count() >= FormatHelpers::Scene::entityLimit);
        DoAction("Added Entity " + QString::number(viewer->entities.count() - 1));
    });

    connect(ui->upEnt, &QToolButton::clicked, [this] {
            ui->entityList->blockSignals(true);
            uint c     = ui->entityList->currentRow();
            auto *item = ui->entityList->takeItem(c);

            int slot                       = viewer->entities[c].slotID;
            int gameSlot                   = viewer->entities[c].gameEntitySlot; // isn't this the exact same thing as slotID?

            viewer->entities[c].slotID         = viewer->entities[c - 1].slotID;
            viewer->entities[c].gameEntitySlot = viewer->entities[c - 1].gameEntitySlot;
            viewer->entities[c].prevSlot       = viewer->entities[c - 1].slotID;

            viewer->entities[c - 1].slotID         = slot;
            viewer->entities[c - 1].prevSlot       = slot;
            viewer->entities[c - 1].gameEntitySlot = gameSlot;


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
            int slot                        = viewer->entities[c].slotID;
            int gameSlot                    = viewer->entities[c].gameEntitySlot;

            viewer->entities[c].slotID         = viewer->entities[c + 1].slotID;
            viewer->entities[c].gameEntitySlot = viewer->entities[c + 1].gameEntitySlot;
            viewer->entities[c].prevSlot       = viewer->entities[c + 1].slotID;

            viewer->entities[c + 1].slotID         = slot;
            viewer->entities[c + 1].prevSlot       = slot;
            viewer->entities[c + 1].gameEntitySlot = gameSlot;

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

    connect(ui->rmEnt, &QToolButton::clicked, [this] {
        int c = ui->entityList->currentRow();
        if (c == -1)
            return;
        int n = ui->entityList->currentRow() == ui->entityList->count() - 1 ? c - 1 : c;

        DeleteEntity(c);

        ui->entityList->blockSignals(true);
        ui->entityList->setCurrentRow(n);
        ui->entityList->blockSignals(false);

        viewer->selectedEntity = n;

        viewer->cameraPos.x = viewer->entities[n].pos.x - ((viewer->storedW / 2) * viewer->invZoom());
        viewer->cameraPos.y = viewer->entities[n].pos.y - ((viewer->storedH / 2) * viewer->invZoom());

        ui->horizontalScrollBar->blockSignals(true);
        ui->horizontalScrollBar->setValue(viewer->cameraPos.x);
        ui->horizontalScrollBar->blockSignals(false);

        ui->verticalScrollBar->blockSignals(true);
        ui->verticalScrollBar->setValue(viewer->cameraPos.y);
        ui->verticalScrollBar->blockSignals(false);

        auto *entity = &viewer->entities[viewer->selectedEntity];
        objProp->setupUI(entity, viewer->selectedEntity,
                         &compilerv2->objectEntityList[entity->gameEntitySlot],
                         &compilerv3->objectEntityList[entity->gameEntitySlot],
                         &compilerv4->objectEntityList[entity->gameEntitySlot], viewer->gameType);

        ui->propertiesBox->setCurrentWidget(ui->objPropPage);

        for (int s = n; s < viewer->selectedEntities.count(); ++s) {
            if (viewer->selectedEntities[s] == (int)c)
                viewer->selectedEntities[s] = c - 1;
            viewer->entities[s].slotID     = viewer->entities[s - 1].slotID;
        }

        ui->rmEnt->setDisabled(viewer->entities.count() <= 0);
        ui->addEnt->setDisabled(viewer->entities.count() >= FormatHelpers::Scene::entityLimit);
        DoAction();
    });

    connect(ui->hScrollList, &QListWidget::currentRowChanged, [this](int c) {
        ui->rmScrH->setDisabled(c == -1);
        ui->impScrH->setDisabled(c == -1);
        ui->expScrH->setDisabled(c == -1);

        if (c == -1)
            return;

        viewer->selectedHScrollInfo = c;

        scrProp->setupUI(&viewer->hScroll[c]);
        ui->propertiesBox->setCurrentWidget(ui->scrollPropPage);
    });

    connect(ui->addScrH, &QToolButton::clicked, [this] {
        SceneHelpers::TileLayer::ScrollIndexInfo scr;
        SceneHelpers::TileLayer::ScrollInstance instance;

        instance.startLine = 0;
        instance.length    = 1;
        scr.instances.append(instance);

        viewer->hScroll.append(scr);

        CreateScrollList();
        DoAction("Added HScroll");
    });

    connect(ui->rmScrH, &QToolButton::clicked, [this] {
        int c = ui->hScrollList->currentRow();
        int n = ui->hScrollList->currentRow() == ui->hScrollList->count() - 1 ? c - 1 : c;
        delete ui->hScrollList->item(c);
        viewer->hScroll.removeAt(c);
        ui->hScrollList->blockSignals(true);
        ui->hScrollList->setCurrentRow(n);
        ui->hScrollList->blockSignals(false);
        DoAction("Removed HScroll");
    });

    connect(ui->impScrH, &QToolButton::clicked, [this] {
        QFileDialog filedialog(this, tr("Import RSDK Scroll File"), "",
                               tr("RSDK Scroll Files (*.xml)"));
        filedialog.setAcceptMode(QFileDialog::AcceptOpen);
        if (filedialog.exec() == QDialog::Accepted) {
            if (ui->layerList->currentRow() == 0){
                QMessageBox::critical(this, "Parallax Import", "Parallax Import Error, Background layer not selected",QMessageBox::Ok);
                return;
            }
            Reader reader(filedialog.selectedFiles()[0]);
            QByteArray bytes = reader.readByteArray(reader.filesize, false);

            QXmlStreamReader xmlReader = QXmlStreamReader(bytes);

            ReadXMLScrollInfo(xmlReader, ui->layerList->currentRow(), 0);

            if (xmlReader.hasError()) {
                QMessageBox::critical(this, "Scroll.xml Parse Error", xmlReader.errorString(),
                                      QMessageBox::Ok);
                return;
            }

            // close reader and flush file
            xmlReader.clear();

            CreateScrollList();
            DoAction("Imported HScroll");
        }
    });

    connect(ui->expScrH, &QToolButton::clicked, [this] {
        QFileDialog filedialog(this, tr("Export RSDK Scroll File"), "",
                               tr("RSDK Scroll Files (*.xml)"));
        filedialog.setAcceptMode(QFileDialog::AcceptSave);
        if (filedialog.exec() == QDialog::Accepted) {
            if (ui->layerList->currentRow() == 0){
                QMessageBox::critical(this, "Parallax Export", "Parallax Export Error, Background layer not selected",QMessageBox::Ok);
                return;
            }
            Writer writer(filedialog.selectedFiles()[0]);
            writer.writeLine("<?xml version=\"1.0\"?>");
            writer.writeLine("");
            WriteXMLScrollInfo(writer, ui->layerList->currentRow(), 0);

            writer.flush();
            SetStatus("Exported Horizontal Parallax Info to " + filedialog.selectedFiles()[0]);
        }
    });

    connect(ui->vScrollList, &QListWidget::currentRowChanged, [this](int c) {
        ui->rmScrV->setDisabled(c == -1);
        ui->impScrV->setDisabled(c == -1);
        ui->expScrV->setDisabled(c == -1);

        if (c == -1)
            return;

        viewer->selectedVScrollInfo = c;

        scrProp->setupUI(&viewer->vScroll[c]);
        ui->propertiesBox->setCurrentWidget(ui->scrollPropPage);
    });

    connect(ui->addScrV, &QToolButton::clicked, [this] {
        SceneHelpers::TileLayer::ScrollIndexInfo scr;
        SceneHelpers::TileLayer::ScrollInstance instance;

        instance.startLine = 0;
        instance.length    = 1;
        scr.instances.append(instance);

        viewer->vScroll.append(scr);

        CreateScrollList();
        DoAction("Added VScroll");
    });

    connect(ui->rmScrV, &QToolButton::clicked, [this] {
        int c = ui->vScrollList->currentRow();
        int n = ui->vScrollList->currentRow() == ui->vScrollList->count() - 1 ? c - 1 : c;
        delete ui->vScrollList->item(c);
        viewer->vScroll.removeAt(c);
        ui->vScrollList->blockSignals(true);
        ui->vScrollList->setCurrentRow(n);
        ui->vScrollList->blockSignals(false);
        DoAction("Removed VScroll");
    });

    connect(ui->impScrV, &QToolButton::clicked, [this] {
        QFileDialog filedialog(this, tr("Import RSDK Scroll File"), "",
                               tr("RSDK Scroll Files (*.xml)"));
        filedialog.setAcceptMode(QFileDialog::AcceptOpen);
        if (filedialog.exec() == QDialog::Accepted) {
            if (ui->layerList->currentRow() == 0){
                QMessageBox::critical(this, "Parallax Import", "Parallax Import Error, Background layer not selected",QMessageBox::Ok);
                return;
            }
            Reader reader(filedialog.selectedFiles()[0]);
            QByteArray bytes = reader.readByteArray(reader.filesize, false);

            QXmlStreamReader xmlReader = QXmlStreamReader(bytes);

            ReadXMLScrollInfo(xmlReader, ui->layerList->currentRow(), 1);

            if (xmlReader.hasError()) {
                QMessageBox::critical(this, "Scroll.xml Parse Error", xmlReader.errorString(),
                                      QMessageBox::Ok);
                return;
            }

            // close reader and flush file
            xmlReader.clear();

            CreateScrollList();
            DoAction("Imported VScroll");
        }
    });

    connect(ui->expScrV, &QToolButton::clicked, [this] {
        QFileDialog filedialog(this, tr("Export RSDK Scroll File"), "",
                               tr("RSDK Scroll Files (*.xml)"));
        filedialog.setAcceptMode(QFileDialog::AcceptSave);
        if (filedialog.exec() == QDialog::Accepted) {
            if (ui->layerList->currentRow() == 0){
                QMessageBox::critical(this, "Parallax Export", "Parallax Export Error, Background layer not selected",QMessageBox::Ok);
                return;
            }
            Writer writer(filedialog.selectedFiles()[0]);
            writer.writeLine("<?xml version=\"1.0\"?>");
            writer.writeLine("");
            WriteXMLScrollInfo(writer, ui->layerList->currentRow(), 0);

            writer.flush();
            SetStatus("Exported Vertical Parallax Info to " + filedialog.selectedFiles()[0]);
        }
    });

    connect(ui->panTool, &QToolButton::clicked, [this] { ResetTools(SceneViewer::TOOL_MOUSE); });
    connect(ui->selectTool, &QToolButton::clicked, [this] { ResetTools(SceneViewer::TOOL_SELECT); });
    connect(ui->pencilTool, &QToolButton::clicked, [this] { ResetTools(SceneViewer::TOOL_PENCIL); });
    connect(ui->eraserTool, &QToolButton::clicked, [this] { ResetTools(SceneViewer::TOOL_ERASER); });
    connect(ui->entityTool, &QToolButton::clicked, [this] { ResetTools(SceneViewer::TOOL_ENTITY); });

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
                    DeleteEntity(o, false);
            }

            for (int t = gameConfig.objects.count() - 1; t >= 0; --t) {
                viewer->objects.removeAt(t);
            }
        }

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
        DoAction();
    });

    connect(ui->showParallax, &QPushButton::clicked, [this] { viewer->showParallax ^= 1; });

    connect(scnProp->editTIL, &QPushButton::clicked, [this] {
        if (chunkEdit == nullptr) {
            chunkEdit = new ChunkEditor(&viewer->chunkset, viewer->chunks, viewer->tiles,
                                        viewer->gameType, this);
            chunkEdit->show();
        }

        connect(chunkEdit, &QDialog::finished, [this] {
            auto chunkStore = viewer->chunkset;
            for (int c = 0; c < (viewer->gameType == ENGINE_v1 ? 0x100 : 0x200); ++c) {
                int chunkID                = chunkEdit->chunkIDs.indexOf(c);
                viewer->chunkset.chunks[c] = chunkStore.chunks[chunkID];
            }

            for (int i = 0; i < viewer->layers.count(); ++i) {
                auto &layer = viewer->layers[i];
                for (int y = 0; y < layer.height; ++y) {
                    for (int x = 0; x < layer.width; ++x) {
                        ushort chunk       = layer.layout[y][x];
                        layer.layout[y][x] = chunkEdit->chunkIDs.indexOf(chunk);
                    }
                }
            }

            chkProp->RefreshList();
            DoAction();
            chunkEdit = nullptr;
        });
    });

    connect(scnProp->editSCF, &QPushButton::clicked, [this] {
        QList<int> newTypes;

        int count = stageConfig.loadGlobalScripts ? gameConfig.objects.count() : 0;
        switch (viewer->gameType) {
            case ENGINE_v4: {
                StageConfigEditorv4 *edit =
                    new StageConfigEditorv4(&stageConfig, 1 + count, gameConfig.soundFX.count(), this);
                edit->exec();
                break;
            }
            case ENGINE_v3: {
                StageConfigEditorv3 *edit =
                    new StageConfigEditorv3(&stageConfig, 1 + count, gameConfig.soundFX.count(), this);
                edit->exec();
                break;
            }
            case ENGINE_v2: {
                StageConfigEditorv2 *edit =
                    new StageConfigEditorv2(&stageConfig, 2 + count, gameConfig.soundFX.count(), this);
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

                int defaultCount = newTypes.count();
                for (; o < gameConfig.objects.count(); ++o) {
                    newTypes.append(defaultCount + o);
                }
            }
        }
        else {
            for (; o < globalsRS.count(); ++o) {
                newTypes.append(1 + o);
            }
        }

        int globalCount = newTypes.count();
        for (o = globalCount; o < viewer->objects.count(); ++o) {
            int index = names.indexOf(viewer->objects[o].name);
            if (index >= 0)
                index += globalCount;
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
            item->setCheckState(viewer->objects[o].visible ? Qt::Checked : Qt::Unchecked);
            item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        }
        ui->objectList->blockSignals(false);

        CreateEntityList();

        if (viewer->gameType == ENGINE_v1) {
            scnProp->musBox->blockSignals(true);
            scnProp->musBox->clear();
            for (int m = 0; m < stageConfig.music.count(); ++m)
                scnProp->musBox->addItem(stageConfig.music[m]);
            scnProp->musBox->blockSignals(false);
        }
        DoAction();
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
        AddStatusProgress(1. / 5); // finished setup

        uchar *pixels = tileset.bits();
        for (int i = 0; i < 0x400; ++i) {
            uchar *src = viewer->tiles[i].bits();
            for (int y = 0; y < 16; ++y) {
                for (int x = 0; x < 16; ++x) {
                    *pixels++ = *src++;
                }
            }
        }
        AddStatusProgress(1. / 5); // finished copying tiles

        viewer->gfxSurface[0].texturePtr = viewer->createTexture(tileset, QOpenGLTexture::Target2D);

        for (int i = 0; i < 0x200; ++i) {
            for (int y = 0; y < 8; ++y) {
                for (int x = 0; x < 8; ++x) {
                    ushort tile = viewer->chunkset.chunks[i].tiles[y][x].tileIndex;
                    viewer->chunkset.chunks[i].tiles[y][x].tileIndex = edit->tileIDs.indexOf(tile);
                }
            }
        }
        AddStatusProgress(1. / 5); // finished updating layout

        RSDKv5::TileConfig configStore = viewer->tileconfig;
        for (int i = 0; i < 0x400; ++i) {
            int id                                   = edit->tileIDs.indexOf(i);
            viewer->tileconfig.collisionPaths[0][id] = configStore.collisionPaths[0][i];
            viewer->tileconfig.collisionPaths[1][id] = configStore.collisionPaths[1][i];
        }
        AddStatusProgress(1. / 5); // finished updating collision masks

        viewer->chunks.clear();
        for (FormatHelpers::Chunks::Chunk &c : viewer->chunkset.chunks)
            viewer->chunks.append(c.getImage(viewer->tiles));

        chkProp->RefreshList();

        DoAction("Edited Tiles");
        SetStatus("Finished rebuilding tiles!"); // done!

        viewer->startTimer();
        viewer->disableDrawScene = false;
    });

    connect(scnProp->editPAL, &QPushButton::clicked, [this] {
        PaletteEditor *edit =
            new PaletteEditor(stageConfig.filePath, viewer->gameType + PALTYPE_STAGECONFIGv4);
        edit->setWindowTitle("Edit StageConfig Palette");
        edit->show();
        DoAction();
    });

    connect(scnProp->copyPlane, &QPushButton::clicked, [this] {
        CopyPlane *sel = new CopyPlane(this);
        if (sel->exec() == QDialog::Accepted) {
            float progress = 1.6;
            if (sel->copyTilePlanes ){
                SetStatus("Copying tile collision....", true);
                RSDKv5::TileConfig configStore = viewer->tileconfig;
                for (int i = 0; i < 0x400; ++i) { viewer->tileconfig.collisionPaths[1][i] = configStore.collisionPaths[0][i]; };
                AddStatusProgress(progress / 5); // finished copying tile planes
                progress = 3.2;
            }

            if (sel->copyChunkPlane){
                for (int i = 0; i < 0x200; ++i) {
                    for(int y = 0; y < 8; ++y){
                        for(int x = 0; x < 8; ++x){
                            viewer->chunkset.chunks[i].tiles[y][x].solidityB = viewer->chunkset.chunks[i].tiles[y][x].solidityA;
                        }
                    }
                    AddStatusProgress(progress / 5); // finished copying tile planes

                };
            }
        }
        if (sel->copyTilePlanes || sel->copyChunkPlane) {
            AddStatusProgress(5 / 5); // finished copying chunks planes
            DoAction();
        }
    });

    connect(ui->exportScn, &QPushButton::clicked, [this] {
        QFileDialog dlg(this, tr("Save Scene XML"), "", tr("RSDK Scene XML Files (*.xml)"));
        dlg.setAcceptMode(QFileDialog::AcceptSave);
        if (dlg.exec() == QDialog::Accepted) {
            Writer writer(dlg.selectedFiles()[0]);

            writer.writeLine("<?xml version=\"1.0\"?>");
            writer.writeLine("");
            WriteXMLScene(writer);

            writer.flush();
        }
    });

    connect(scnProp->syncGC, &QPushButton::clicked, [] {});
    scnProp->syncGC->setDisabled(true);

    connect(scnProp->syncSC, &QPushButton::clicked, [] {});

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
        SetStatus("Reloading Game Link...", true);
        viewer->stopTimer();
        objProp->unsetUI();

        InitGameLink();

        viewer->startTimer();
        SetStatus("Game Link reloaded successfully!");
    });

    connect(scnProp->replaceChunk, &QPushButton::clicked, [this] {

        if (chunkRpl == nullptr) {
            chunkRpl = new ChunkReplaceOptions(viewer->gameType, &viewer->chunkset, viewer->chunks, viewer->tiles, this);

            chunkRpl->show();
        }

        connect(chunkRpl, &QDialog::finished, [this] {
            if (chunkRpl->modified){
                chkProp->RefreshList();
                DoAction();
            }
            chunkRpl = nullptr;
        });
    });

    connect(scnProp->replaceTile, &QPushButton::clicked, [this] {
        TileReplaceOptions *dlg = new TileReplaceOptions;
        if (!dlg->exec())
            return;

        viewer->stopTimer();

        SetStatus("Replacing Tile Info...", true);

        ushort dstTile = dlg->dstTile->value();
        ushort srcTile = dlg->srcTile->value();

        bool replaceIndex       = dlg->replaceTileIndex->checkState() == Qt::Checked;
        bool replaceVisualPlane = dlg->replaceVisualPlane->checkState() == Qt::Checked;
        bool replaceFlipX       = dlg->replaceFlipX->checkState() == Qt::Checked;
        bool replaceFlipY       = dlg->replaceFlipY->checkState() == Qt::Checked;
        bool replaceSolidATop   = dlg->replaceSolidATop->checkState() == Qt::Checked;
        bool replaceSolidALRB   = dlg->replaceSolidALRB->checkState() == Qt::Checked;
        bool replaceSolidBTop   = dlg->replaceSolidBTop->checkState() == Qt::Checked;
        bool replaceSolidBLRB   = dlg->replaceSolidBLRB->checkState() == Qt::Checked;
        bool replaceCollision   = dlg->replaceCollision->checkState() == Qt::Checked;
        bool replaceGraphics    = dlg->replaceGraphics->checkState() == Qt::Checked;

        bool hasVisualPlane = dlg->hasVisualPlane->checkState() == Qt::Checked;
        bool hasFlipX       = dlg->hasFlipX->checkState() == Qt::Checked;
        bool hasFlipY       = dlg->hasFlipY->checkState() == Qt::Checked;
        bool hasSolidATop   = dlg->hasSolidATop->checkState() == Qt::Checked;
        bool hasSolidALRB   = dlg->hasSolidALRB->checkState() == Qt::Checked;
        bool hasSolidBTop   = dlg->hasSolidBTop->checkState() == Qt::Checked;
        bool hasSolidBLRB   = dlg->hasSolidBLRB->checkState() == Qt::Checked;

        byte newSolidA = 3;
        if (hasSolidATop && hasSolidALRB)
            newSolidA = 0;
        if (hasSolidATop && !hasSolidALRB)
            newSolidA = 1;
        if (!hasSolidATop && hasSolidALRB)
            newSolidA = 2;

        byte newSolidB = 3;
        if (hasSolidBTop && hasSolidBLRB)
            newSolidB = 0;
        if (hasSolidBTop && !hasSolidBLRB)
            newSolidB = 1;
        if (!hasSolidBTop && hasSolidBLRB)
            newSolidB = 2;

        // Replace Tile Layer info
        if (replaceIndex || replaceFlipX || replaceFlipY || replaceSolidATop || replaceSolidALRB
            || replaceSolidBTop || replaceSolidBLRB) {

            for (int c = 0; c < 0x200; ++c) {
                auto &chunk = viewer->chunkset.chunks[c];

                for (int y = 0; y < 8; ++y) {
                    for (int x = 0; x < 8; ++x) {
                        auto &tile = chunk.tiles[y][x];

                        if (tile.tileIndex == dstTile) {
                            bool flipX = replaceFlipX ? hasFlipX : Utils::getBit(tile.direction, 0);
                            bool flipY = replaceFlipY ? hasFlipY : Utils::getBit(tile.direction, 1);
                            byte dir   = (byte)flipX | ((byte)flipY << 1);

                            tile.tileIndex   = replaceIndex ? srcTile : dstTile;
                            tile.direction   = dir;
                            tile.visualPlane = replaceVisualPlane ? hasVisualPlane : tile.visualPlane;
                            if (replaceSolidATop || replaceSolidALRB)
                                tile.solidityA = newSolidA;
                            if (replaceSolidBTop || replaceSolidBLRB)
                                tile.solidityB = newSolidB;
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

    ClearActions();
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

void SceneEditor::updateType(SceneEntity *entity, byte type)
{
    int c = viewer->selectedEntity;
    ui->entityList->item(c)->setText(QString::number(viewer->entities[c].slotID) + ": "
                                     + viewer->objects[viewer->entities[c].type].name);
    entity->propertyValue = 0;
    entity->variables.clear();
    for (int v = 0; v < viewer->objects[type].variables.count(); ++v) {
        RSDKv5::Scene::VariableValue val;
        val.type        = VAR_UINT8;
        val.value_uint8 = 0;
        entity->variables.append(val);
    }
    // maybe make it use updateUI?
    objProp->setupUI(entity, viewer->selectedEntity,
                     &compilerv2->objectEntityList[entity->gameEntitySlot],
                     &compilerv3->objectEntityList[entity->gameEntitySlot],
                     &compilerv4->objectEntityList[entity->gameEntitySlot], viewer->gameType);
}

bool SceneEditor::event(QEvent *event)
{

    switch ((int)event->type()) {
        default: break;

        case RE_EVENT_NEW: CreateNewScene(); break;

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

            QFileDialog filedialog(this, tr("Open Scene"), viewer->dataPath + "/Stages/");
            filedialog.setNameFilters(types);
            filedialog.setAcceptMode(QFileDialog::AcceptOpen);

            if (viewer->gameType != ENGINE_NONE) {
                filedialog.selectNameFilter(types[viewer->gameType - 1]);
            }

            if (filedialog.exec() == QDialog::Accepted) {
                int filter     = types.indexOf(filedialog.selectedNameFilter());
                QString gcPath = "";

                QString filePath = QFileInfo(filedialog.selectedFiles()[0]).absolutePath();
                QDir dir(filePath); // Data/Stages/SCENE/
                dir.cdUp();         // Data/Stages/
                dir.cdUp();         // Data/

                if ((viewer->gameType != filter + 1 || viewer->dataPath != dir.path())
                    && filter < ENGINE_v2) {
                    QFileDialog gcdialog(this, tr("Open GameConfig"), dir.path() + "/Data/");
                    gcdialog.setNameFilters({ gcTypes[filter], "RSDK Game.xml Files (Game*.xml)" });
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
                    gcPath = gameConfig.filePath;
                }

                LoadScene(filedialog.selectedFiles()[0], gcPath, filter + 1);
                return true;
            }
            break;
        }

        case RE_EVENT_SAVE: {
            if (SaveScene())
                return true;
            break;
        }

        case RE_EVENT_SAVE_AS: {
            if (SaveScene(true))
                return true;
            break;
        }

        case RE_EVENT_UNDO: UndoAction(); return true;
        case RE_EVENT_REDO: RedoAction(); return true;

        case RE_EVENT_TAB_GAIN_FOCUS: viewer->startTimer(); break;
        case RE_EVENT_TAB_LOSE_FOCUS: viewer->stopTimer(); break;

        case QEvent::Close:
            if (modified) {
                bool cancelled = false;
                if (MainWindow::ShowCloseWarning(this, &cancelled)) {
                    if (SaveScene())
                        return true;
                }
                else if (cancelled) {
                    event->ignore();
                    return true;
                }
            }
            break;

            // You cant have a focus here & viewer, so copy code :]
        case QEvent::KeyPress: viewerActive = false; HandleKeyPress(static_cast<QKeyEvent *>(event)); break;
        case QEvent::KeyRelease: viewerActive = false; HandleKeyRelease(static_cast<QKeyEvent *>(event)); break;
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

            if (mouseDownM || (mouseDownL && viewer->curTool == SceneViewer::TOOL_MOUSE))
                setCursor(Qt::ClosedHandCursor);

            if (mouseDownL) {
                switch (viewer->curTool) {
                    case SceneViewer::TOOL_MOUSE: break;
                    case SceneViewer::TOOL_SELECT:
                        viewer->isSelecting  = true;
                        viewer->selectPos.x  = sceneMousePos.x;
                        viewer->selectPos.y  = sceneMousePos.y;
                        viewer->selectSize.x = 0;
                        viewer->selectSize.y = 0;
                        viewer->selectedEntities.clear();
                        break;

                    case SceneViewer::TOOL_PENCIL: {
                        if (viewer->selectedChunk != 0xFFFF && viewer->isSelecting) {
                            SetChunk(mEvent->pos().x(), mEvent->pos().y());
                            // DoAction();
                        }
                        break;
                    }
                    case SceneViewer::TOOL_ERASER: {
                        if (viewer->isSelecting) {
                            viewer->selectedChunk = 0x00;
                            SetChunk(mEvent->pos().x(), mEvent->pos().y());
                            // DoAction();
                        }
                        break;
                    }
                    case SceneViewer::TOOL_ENTITY: {
                        if (!viewer->isSelecting || viewer->selectedObject < 0) {
                            Rect<float> box;
                            int firstSel = -1;
                            Vector2<float> firstPos;

                            int selectedEntity = -1;
                            // viewer->selectedEntity = -1;
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

                                if (box.contains(pos) && selectedEntity < o) {
                                    selectedEntity = o; // future proofing?
                                    if (viewer->selectedEntity != o) {
                                        waitForRelease         = true;
                                        viewer->selectedEntity = o;

                                        selectionOffset.x = pos.x - viewer->entities[o].pos.x;
                                        selectionOffset.y = pos.y - viewer->entities[o].pos.y;

                                        auto *entity = &viewer->entities[viewer->selectedEntity];
                                        objProp->setupUI(
                                            entity, viewer->selectedEntity,
                                            &compilerv2->objectEntityList[entity->gameEntitySlot],
                                            &compilerv3->objectEntityList[entity->gameEntitySlot],
                                            &compilerv4->objectEntityList[entity->gameEntitySlot],
                                            viewer->gameType);
                                        ui->propertiesBox->setCurrentWidget(ui->objPropPage);
                                    }
                                    // DoAction();
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

                                    auto *entity = &viewer->entities[viewer->selectedEntity];
                                    objProp->setupUI(
                                        entity, viewer->selectedEntity,
                                        &compilerv2->objectEntityList[entity->gameEntitySlot],
                                        &compilerv3->objectEntityList[entity->gameEntitySlot],
                                        &compilerv4->objectEntityList[entity->gameEntitySlot],
                                        viewer->gameType);
                                    ui->propertiesBox->setCurrentWidget(ui->objPropPage);
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
                                && viewer->entities.count() < FormatHelpers::Scene::entityLimit) {
                                float x =
                                    ((mEvent->pos().x() * viewer->invZoom()) + viewer->cameraPos.x);
                                float y =
                                    ((mEvent->pos().y() * viewer->invZoom()) + viewer->cameraPos.y);

                                if (ctrlDownL) {
                                    x = (x - fmodf(x, viewer->gridSize.x));
                                    y = (y - fmodf(y, viewer->gridSize.y));
                                }

                                AddEntity(viewer->selectedObject, x, y);

                                DoAction("Added Entity");
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
                            Rect<float> box;

                            for (int y = 0; y < viewer->sceneBoundsB / 0x80; ++y) {
                                for (int x = 0; x < viewer->sceneBoundsR / 0x80; ++x) {
                                    box = Rect<float>(x * 0x80, y * 0x80, 0x80, 0x80);

                                    Vector2<float> pos = Vector2<float>(
                                        (mEvent->pos().x() * viewer->invZoom()) + viewer->cameraPos.x,
                                        (mEvent->pos().y() * viewer->invZoom()) + viewer->cameraPos.y);
                                    if (box.contains(pos)) {
                                        ushort chunk =
                                            viewer->layers[viewer->selectedLayer].layout[y][x];

                                        chkProp->SetCurrentChunk(chunk);

                                        viewer->selectedChunk = chunk;
                                        ui->toolBox->setCurrentWidget(ui->chunksPage);
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

                                auto *entity = &viewer->entities[viewer->selectedEntity];
                                objProp->setupUI(entity, viewer->selectedEntity,
                                                 &compilerv2->objectEntityList[entity->gameEntitySlot],
                                                 &compilerv3->objectEntityList[entity->gameEntitySlot],
                                                 &compilerv4->objectEntityList[entity->gameEntitySlot],
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

                                auto *entity = &viewer->entities[viewer->selectedEntity];
                                objProp->setupUI(entity, viewer->selectedEntity,
                                                 &compilerv2->objectEntityList[entity->gameEntitySlot],
                                                 &compilerv3->objectEntityList[entity->gameEntitySlot],
                                                 &compilerv4->objectEntityList[entity->gameEntitySlot],
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
            // QMouseEvent *mEvent = static_cast<QMouseEvent *>(event);

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
                || viewer->curTool == SceneViewer::TOOL_ENTITY) {
                viewer->tilePos.x = viewer->mousePos.x;
                viewer->tilePos.y = viewer->mousePos.y;

                if (ctrlDownL) {
                    if (viewer->curTool == SceneViewer::TOOL_PENCIL
                        || viewer->curTool == SceneViewer::TOOL_ERASER) {
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

                    case SceneViewer::TOOL_PENCIL: {
                        if (viewer->selectedChunk != 0xFFFF && viewer->isSelecting) {
                            SetChunk(viewer->mousePos.x, viewer->mousePos.y);
                            // DoAction();
                        }
                        break;
                    }

                    case SceneViewer::TOOL_ERASER: {
                        if (viewer->isSelecting) {
                            viewer->selectedChunk = 0x0;
                            SetChunk(viewer->mousePos.x, viewer->mousePos.y);
                            // DoAction();
                        }
                        break;
                    }

                    case SceneViewer::TOOL_ENTITY: {
                        if (viewer->selectedObject < 0 && viewer->selectedEntity >= 0
                            && !waitForRelease) {
                            UpdateTitle(true);
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

                            compilerv2->objectEntityList[entity.gameEntitySlot].XPos =
                                entity.pos.x * 65536;
                            compilerv2->objectEntityList[entity.gameEntitySlot].YPos =
                                entity.pos.y * 65536;

                            compilerv3->objectEntityList[entity.gameEntitySlot].XPos =
                                entity.pos.x * 65536;
                            compilerv3->objectEntityList[entity.gameEntitySlot].YPos =
                                entity.pos.y * 65536;

                            compilerv4->objectEntityList[entity.gameEntitySlot].XPos =
                                entity.pos.x * 65536;
                            compilerv4->objectEntityList[entity.gameEntitySlot].YPos =
                                entity.pos.y * 65536;

                            objProp->updateUI();
                            // DoAction();
                        }
                        break;
                    }
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

            if (mouseDownL)
                switch (viewer->curTool) {
                    case SceneViewer::TOOL_MOUSE: break;
                    case SceneViewer::TOOL_SELECT: viewer->isSelecting = false; break;
                    case SceneViewer::TOOL_PENCIL: {
                        if (viewer->selectedChunk != 0xFFFF && viewer->isSelecting) {
                            DoAction(QString("Placed Chunk(s): (%1, %2)")
                                         .arg(mEvent->pos().x())
                                         .arg(mEvent->pos().y()));
                        }
                        break;
                    }
                    case SceneViewer::TOOL_ERASER: {
                        if (viewer->isSelecting) {
                            DoAction(QString("Erased Chunk(s): (%1, %2)")
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

            if (wEvent->modifiers() & Qt::ControlModifier) {
                if (wEvent->angleDelta().y() > 0 && viewer->zoom < 20) {
                    viewer->cameraPos.x += (int)((viewer->storedW / 4) / viewer->zoom);
                    viewer->cameraPos.y += (int)((viewer->storedH / 4) / viewer->zoom);
                    viewer->zoom *= 2;
                }
                else if (wEvent->angleDelta().y() < 0 && viewer->zoom > 1) {
                    viewer->zoom /= 2;
                    viewer->cameraPos.x -= (int)((viewer->storedW / 4) / viewer->zoom);
                    viewer->cameraPos.y -= (int)((viewer->storedH / 4) / viewer->zoom);
                }

                ui->horizontalScrollBar->setMaximum(viewer->sceneBoundsR
                                                    - viewer->storedW / viewer->zoom);
                ui->verticalScrollBar->setMaximum(viewer->sceneBoundsB
                                                  - viewer->storedH / viewer->zoom);
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
            break;
        }


        case QEvent::KeyPress: viewerActive = true; HandleKeyPress(static_cast<QKeyEvent *>(event)); break;
        case QEvent::KeyRelease: viewerActive = true; HandleKeyRelease(static_cast<QKeyEvent *>(event)); break;

        default: break;
    }

    return false;
}

void SceneEditor::CreateNewScene()
{
    SetStatus("Creating new scene...", true);

    viewer->objectsLoaded = false;
    viewer->stopTimer();
    viewer->unloadScene();

    AddStatusProgress(0.2); // finish unloading

    // TODO: lol actually make a new scene

    tabTitle = "Scene Editor";

    ClearActions();
    SetStatus("Created new scene!"); // done!

    viewer->startTimer();
}

void SceneEditor::LoadScene(QString scnPath, QString gcfPath, byte gameType)
{
    viewer->disableDrawScene = true;

    SetStatus("Loading scene...", true);

    viewer->objectsLoaded = false;

    viewer->stopTimer();
    viewer->unloadScene();

    if (gcfPath != gameConfig.filePath) {
        if (QFileInfo(gcfPath).suffix().toLower().contains("xml"))
            ParseGameXML(gcfPath);
        else
            gameConfig.read(gameType, gcfPath);
    }
    QString dataPath = QFileInfo(gcfPath).absolutePath();
    QDir dir(dataPath);
    dir.cdUp();
    viewer->dataPath = dir.path();

    AddStatusProgress(1. / 7); // finish initial setup

    // loading
    QString pth      = scnPath;
    QString basePath = pth.replace(QFileInfo(pth).fileName(), "");

    viewer->currentFolder  = QDir(basePath).dirName();
    viewer->currentSceneID = QFileInfo(scnPath).baseName().toLower().replace("act", "");

    scene.read(gameType, scnPath);

    viewer->metadata    = RSDKv5::Scene::SceneEditorMetadata();
    viewer->playerPos.x = scene.playerX;
    viewer->playerPos.y = scene.playerY;

    if (!appConfig.baseDataManager[gameType].dataPath.isEmpty())
        WorkingDirManager::workingDir = appConfig.baseDataManager[gameType].dataPath + "/";

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

    AddStatusProgress(1. / 7); // finish reading files

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
        viewLayer.drawGroup      = layerID--;
        viewLayer.visible        = false;
        viewLayer.parallaxFactor = 1.0f;
        viewLayer.scrollSpeed    = 0.0f;
        viewLayer.type           = SceneHelpers::TileLayer::LAYER_HSCROLL;

        viewLayer.scrollInfos.clear();
        viewLayer.layout.clear();
        for (int y = 0; y < scene.height; ++y) {
            QList<ushort> tileRow;
            for (int x = 0; x < scene.width; ++x) tileRow.append(scene.layout[y][x]);
            viewLayer.layout.append(tileRow);
        }

        viewer->layers.append(viewLayer);
    }

    // Background
    for (auto &layer : background.layers) {
        SceneHelpers::TileLayer viewLayer;

        viewLayer.name           = "Background " + QString::number(8 - layerID);
        viewLayer.width          = layer.width;
        viewLayer.height         = layer.height;
        viewLayer.drawGroup      = layerID--;
        viewLayer.visible        = false;
        viewLayer.parallaxFactor = layer.parallaxFactor;
        viewLayer.scrollSpeed    = layer.scrollSpeed;

        viewLayer.type = SceneHelpers::TileLayer::LAYER_NONE;
        switch (layer.type) {
            default: break;
            case 1: viewLayer.type = SceneHelpers::TileLayer::LAYER_HSCROLL; break;
            case 2: viewLayer.type = SceneHelpers::TileLayer::LAYER_VSCROLL; break;
            case 3: viewLayer.type = SceneHelpers::TileLayer::LAYER_FLOOR3D; break;
            case 4: viewLayer.type = SceneHelpers::TileLayer::LAYER_SKY3D; break;
        }

        viewLayer.scrollInfos.clear();
        viewLayer.layout.clear();
        for (int y = 0; y < layer.height; ++y) {
            viewLayer.layout.append(QList<ushort>());
            for (int x = 0; x < layer.width; ++x) viewLayer.layout[y].append(layer.layout[y][x]);
        }

        viewer->layers.append(viewLayer);
    }

    viewer->useLayerScrollInfo = false; // use <= v4 style scroll Info

    viewer->hScroll.clear();
    for (auto &info : background.hScrollInfo) {
        SceneHelpers::TileLayer::ScrollIndexInfo scroll;

        scroll.parallaxFactor = info.parallaxFactor;
        scroll.scrollSpeed    = info.scrollSpeed;
        scroll.deform         = info.deform;
        scroll.unknown        = 0;

        for (auto &instance : info.instances) {
            SceneHelpers::TileLayer::ScrollInstance inst;

            inst.startLine = instance.startLine;
            inst.length    = instance.length;
            inst.layerID   = instance.layerID; // for v5, we don't care about layer ID
            scroll.instances.append(inst);
        }

        viewer->hScroll.append(scroll);
    }

    viewer->vScroll.clear();
    for (auto &info : background.vScrollInfo) {
        SceneHelpers::TileLayer::ScrollIndexInfo scroll;

        scroll.parallaxFactor = info.parallaxFactor;
        scroll.scrollSpeed    = info.scrollSpeed;
        scroll.deform         = info.deform;
        scroll.unknown        = 0;

        for (auto &instance : info.instances) {
            SceneHelpers::TileLayer::ScrollInstance inst;

            inst.startLine = instance.startLine;
            inst.length    = instance.length;
            inst.layerID   = instance.layerID; // for v5, we don't care about layer ID
            scroll.instances.append(inst);
        }

        viewer->vScroll.append(scroll);
    }

    for (int a = 0; a < 4; ++a) {
        byte id = scene.activeLayer[a];
        if (id < 9)
            viewer->layers[id].visible = true;
    }

    AddStatusProgress(1. / 7); // finish setting up layers

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
        info.slotID         = scene.objects[i].slotID;
        info.prevSlot       = info.slotID;
        info.gameEntitySlot = info.slotID;
        info.type           = scene.objects[i].type;
        info.propertyValue  = scene.objects[i].propertyValue;
        info.pos.x          = scene.objects[i].getX();
        info.pos.y          = scene.objects[i].getY();

        if (gameType == ENGINE_v4) {
            for (int v = 0; v < 0xF; ++v) {
                info.sceneVariables[v] = scene.objects[i].variables[v];
            }
        }

        viewer->entities.append(info);
    }

    AddStatusProgress(1. / 7); // finish objects & entities

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

    AddStatusProgress(1. / 7); // finish building tiles & chunks

    ui->layerList->blockSignals(true);
    ui->layerList->clear();
    QListWidgetItem *itemFG = new QListWidgetItem("Foreground", ui->layerList);
    itemFG->setCheckState(viewer->layers[0].visible ? Qt::Checked : Qt::Unchecked);
    itemFG->setFlags(itemFG->flags() | Qt::ItemIsUserCheckable);

    for (int l = 0; l < background.layers.count(); ++l) {
        QListWidgetItem *item =
            new QListWidgetItem("Background " + QString::number(l + 1), ui->layerList);
        item->setCheckState(viewer->layers[l + 1].visible ? Qt::Checked : Qt::Unchecked);
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

    ui->horizontalScrollBar->setMaximum(viewer->sceneBoundsR - viewer->storedW);
    ui->verticalScrollBar->setMaximum(viewer->sceneBoundsB - viewer->storedH);
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

    scnProp->syncGC->setDisabled(!stageConfig.loadGlobalScripts);

    scnProp->bgSel->setColor(viewer->metadata.backgroundColor1);
    scnProp->altBGSel->setColor(viewer->metadata.backgroundColor2);

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
    lyrProp->setupUI(viewer, 0);
    tileProp->setupUI(&viewer->tileconfig.collisionPaths[0][0], &viewer->tileconfig.collisionPaths[1][0], 0, viewer->tiles, viewer);

    objProp->unsetUI();
    scrProp->unsetUI();

    AddStatusProgress(1. / 7); // finish setting up UI stuff

    InitGameLink();

    ui->addEnt->setDisabled(viewer->entities.count() >= FormatHelpers::Scene::entityLimit);

    tabTitle = Utils::getFilenameAndFolder(scnPath);

    viewer->disableDrawScene = false;
    viewer->disableObjects   = false;
    viewer->objectsLoaded    = true;
    viewer->startTimer();

    ClearActions();
    appConfig.addRecentFile(viewer->gameType, TOOL_SCENEEDITOR, scnPath, QList<QString>{ gcfPath });
    SetStatus("Loaded scene " + QFileInfo(scnPath).fileName()); // done!
}

bool SceneEditor::SaveScene(bool forceSaveAs)
{
    viewer->disableDrawScene = true;
    viewer->disableObjects   = true;

    byte saveVer     = viewer->gameType;
    QString savePath = "";
    if (!forceSaveAs && QFile::exists(scene.filepath)) {
        savePath = scene.filepath;
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
            savePath = filedialog.selectedFiles()[0];
            saveVer  = ENGINE_v4 + types.indexOf(filedialog.selectedNameFilter());
        }
        else {
            viewer->disableDrawScene = false;
            viewer->disableObjects   = false;
            return false;
        }
    }

    QString basePath = savePath;
    basePath         = basePath.replace(QFileInfo(savePath).fileName(), "");

    SetStatus("Saving scene...", true);
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
    AddStatusProgress(1.f / 5); // created tileset

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
        switch (viewLayer.type) {
            default: break;
            case SceneHelpers::TileLayer::LAYER_HSCROLL: layer.type = 1; break;
            case SceneHelpers::TileLayer::LAYER_VSCROLL: layer.type = 2; break;
            case SceneHelpers::TileLayer::LAYER_FLOOR3D: layer.type = 3; break;
            case SceneHelpers::TileLayer::LAYER_SKY3D: layer.type = 4; break;
        }

        layer.layout.clear();
        for (int y = 0; y < viewLayer.height; ++y) {
            layer.layout.append(QList<ushort>());
            for (int x = 0; x < viewLayer.width; ++x) layer.layout[y].append(viewLayer.layout[y][x]);
        }

        background.layers.append(layer);
    }

    background.hScrollInfo.clear();
    for (auto &hScroll : viewer->hScroll) {
        FormatHelpers::Background::ScrollIndexInfo scroll;

        scroll.parallaxFactor = hScroll.parallaxFactor;
        scroll.scrollSpeed    = hScroll.scrollSpeed;
        scroll.deform         = hScroll.deform;

        scroll.instances.clear();
        for (auto &instance : hScroll.instances) {
            FormatHelpers::Background::ScrollInstance inst;

            inst.startLine = instance.startLine;
            inst.length    = instance.length;
            inst.layerID   = instance.layerID;

            scroll.instances.append(inst);
        }
        background.hScrollInfo.append(scroll);
    }

    background.vScrollInfo.clear();
    for (auto &vScroll : viewer->vScroll) {
        FormatHelpers::Background::ScrollIndexInfo scroll;

        scroll.parallaxFactor = vScroll.parallaxFactor;
        scroll.scrollSpeed    = vScroll.scrollSpeed;
        scroll.deform         = vScroll.deform;

        scroll.instances.clear();
        for (auto &instance : vScroll.instances) {
            FormatHelpers::Background::ScrollInstance inst;

            inst.startLine = instance.startLine;
            inst.length    = instance.length;
            inst.layerID   = instance.layerID;

            scroll.instances.append(inst);
        }
        background.vScrollInfo.append(scroll);
    }

    AddStatusProgress(1.f / 5); // created tile layers

    scene.objects.clear();
    scene.objectTypeNames.clear();

    int globalObjCount = 1;
    if (stageConfig.loadGlobalScripts) {
        if (saveVer == ENGINE_v2)
            globalObjCount += 1; // player

        globalObjCount += gameConfig.objects.count();
    }

    int objID = 0;
    for (auto &obj : viewer->objects) {
        if (objID >= globalObjCount)
            scene.objectTypeNames.append(obj.name);

        objID++;
    }
    AddStatusProgress(1.f / 5); // created object list

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
    AddStatusProgress(1.f / 5); // created entity list

    if (saveVer != ENGINE_v1) {
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

        scene.write(saveVer, savePath);
        background.write(saveVer, basePath + "Backgrounds.bin");
        viewer->chunkset.write(saveVer, basePath + "128x128Tiles.bin");
        tileconfig.write(basePath + "CollisionMasks.bin");
        stageConfig.write(saveVer, basePath + "StageConfig.bin");
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

        scene.write(saveVer, savePath);
        background.write(saveVer, basePath + "ZoneBG.map");
        viewer->chunkset.write(saveVer, basePath + "Zone.til");
        tileconfig.write(basePath + "Zone.tcf");
        stageConfig.write(saveVer, basePath + "Zone.zcf");

        RSDKv1::GFX *gfx = new RSDKv1::GFX;
        gfx->importImage(tileset);
        gfx->write(basePath + "Zone.gfx");

        delete gfx;
    }

    tabTitle = Utils::getFilenameAndFolder(savePath);
    ClearActions();
    appConfig.addRecentFile(saveVer, TOOL_SCENEEDITOR, savePath, QList<QString>{ gameConfig.filePath });
    SetStatus("Saved scene to " + Utils::getFilenameAndFolder(savePath));

    viewer->disableDrawScene = false;
    viewer->disableObjects   = false;
    return true;
}

void SceneEditor::UnloadGameLinks()
{
    for (int o = 2; o < v5_SURFACE_MAX; ++o) {
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

    compilerv2->ClearScriptData();
    compilerv3->ClearScriptData();
    compilerv4->ClearScriptData();
}

void SceneEditor::InitGameLink()
{
    UnloadGameLinks();

    compilerv2->ClearScriptData();
    compilerv3->ClearScriptData();
    compilerv4->ClearScriptData();

    for (int i = 0; i < ENTITY_COUNT_v4; ++i) { // v2 == v3 == v4
        memset((void *)&compilerv2->objectEntityList[i], 0, sizeof(compilerv2->objectEntityList[i]));
        memset((void *)&compilerv3->objectEntityList[i], 0, sizeof(compilerv3->objectEntityList[i]));
        memset((void *)&compilerv4->objectEntityList[i], 0, sizeof(compilerv4->objectEntityList[i]));

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

    compilerv2->ClearScriptData();
    compilerv3->ClearScriptData();
    compilerv4->ClearScriptData();
    int id = 0;
    sprintf(compilerv3->typeNames[id], "%s", "BlankObject");
    sprintf(compilerv4->typeNames[id++], "%s", "BlankObject");

    if (stageConfig.loadGlobalScripts) {
        for (int o = 0; o < gameConfig.objects.count(); ++o) {
            SetScriptTypeName(gameConfig.objects[o].name.toStdString().c_str(),
                              compilerv3->typeNames[id]);

            SetScriptTypeName(gameConfig.objects[o].name.toStdString().c_str(),
                              compilerv4->typeNames[id]);

            id++;
        }
    }

    for (int o = 0; o < stageConfig.objects.count(); ++o) {
        SetScriptTypeName(stageConfig.objects[o].name.toStdString().c_str(), compilerv3->typeNames[id]);
        SetScriptTypeName(stageConfig.objects[o].name.toStdString().c_str(), compilerv4->typeNames[id]);

        id++;
    }

    for (int v = 0; v < gameConfig.globalVariables.count(); ++v) {
        compilerv2->globalVariables.append(gameConfig.globalVariables[v].name);
        compilerv3->globalVariables.append(gameConfig.globalVariables[v].name);
        compilerv4->globalVariables.append(gameConfig.globalVariables[v].name);
    }

    scriptError = false;
    switch (viewer->gameType) {
        case ENGINE_v1: break; // read the editor stuff from this somehow (idk how to parse it lol)
        case ENGINE_v2: {      // parse the RSDK sub and use that data to know what to draw
            int scrID          = 1;
            compilerv2->editor = this;

            if (stageConfig.loadGlobalScripts) {
                for (int i = 0; i < gameConfig.objects.count() && !scriptError; ++i) {
                    QString scriptPath = viewer->dataPath + "/Scripts/" + gameConfig.objects[i].script;
                    scriptPath = WorkingDirManager::GetPath("/Scripts/" + gameConfig.objects[i].script,
                                                            scriptPath);

                    compilerv2->ParseScriptFile(scriptPath, scrID++);

                    if (compilerv2->scriptError) {
                        PrintLog(compilerv2->errorMsg);
                        PrintLog(compilerv2->errorPos);
                        PrintLog(QString::number(compilerv2->errorLine));

                        QFileInfo info(compilerv2->errorScr);
                        QDir dir(info.dir());
                        dir.cdUp();
                        QString dirFile = dir.relativeFilePath(compilerv2->errorScr);

                        SetStatus("Failed to compile script: " + dirFile);
                        compilerv2->objectScriptList[scrID - 1].subRSDK.scriptCodePtr = -1;
                        compilerv2->objectScriptList[scrID - 1].subRSDK.jumpTablePtr  = -1;
                        scriptError                                                   = true;
                    }
                }
            }

            for (int i = 0; i < stageConfig.objects.count() && !scriptError; ++i) {
                QString scriptPath = viewer->dataPath + "/Scripts/" + stageConfig.objects[i].script;
                scriptPath =
                    WorkingDirManager::GetPath("/Scripts/" + stageConfig.objects[i].script, scriptPath);
                compilerv2->ParseScriptFile(scriptPath, scrID++);

                if (compilerv2->scriptError) {
                    PrintLog(compilerv2->errorMsg);
                    PrintLog(compilerv2->errorPos);
                    PrintLog(QString::number(compilerv2->errorLine));

                    QFileInfo info(compilerv2->errorScr);
                    QDir dir(info.dir());
                    dir.cdUp();
                    QString dirFile = dir.relativeFilePath(compilerv2->errorScr);

                    SetStatus("Failed to compile script: " + dirFile);
                    compilerv2->objectScriptList[scrID - 1].subRSDK.scriptCodePtr = -1;
                    compilerv2->objectScriptList[scrID - 1].subRSDK.jumpTablePtr  = -1;
                    scriptError                                                   = true;
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
                for (int i = 0; i < gameConfig.objects.count() && !scriptError; ++i) {
                    QString scriptPath = viewer->dataPath + "/Scripts/" + gameConfig.objects[i].script;
                    scriptPath = WorkingDirManager::GetPath("/Scripts/" + gameConfig.objects[i].script,
                                                            scriptPath);
                    compilerv3->ParseScriptFile(scriptPath, scrID++);

                    if (compilerv3->scriptError) {
                        PrintLog(compilerv3->errorMsg);
                        PrintLog(compilerv3->errorPos);
                        PrintLog(QString::number(compilerv3->errorLine));

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

                        SetStatus("Failed to compile script: " + dirFile);
                        compilerv3->objectScriptList[scrID - 1].subRSDKDraw.scriptCodePtr = -1;
                        compilerv3->objectScriptList[scrID - 1].subRSDKDraw.jumpTablePtr  = -1;
                        compilerv3->objectScriptList[scrID - 1].subRSDKLoad.scriptCodePtr = -1;
                        compilerv3->objectScriptList[scrID - 1].subRSDKLoad.jumpTablePtr  = -1;
                        compilerv3->objectScriptList[scrID - 1].subRSDKEdit.scriptCodePtr = -1;
                        compilerv3->objectScriptList[scrID - 1].subRSDKEdit.jumpTablePtr  = -1;

                        scriptError = true;
                    }
                }
            }

            for (int i = 0; i < stageConfig.objects.count() && !scriptError; ++i) {
                QString scriptPath = viewer->dataPath + "/Scripts/" + stageConfig.objects[i].script;
                scriptPath =
                    WorkingDirManager::GetPath("/Scripts/" + stageConfig.objects[i].script, scriptPath);
                compilerv3->ParseScriptFile(scriptPath, scrID++);

                if (compilerv3->scriptError) {
                    PrintLog(compilerv3->errorMsg);
                    PrintLog(compilerv3->errorPos);
                    PrintLog(QString::number(compilerv3->errorLine));

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

                    SetStatus("Failed to compile script: " + dirFile);
                    compilerv3->objectScriptList[scrID - 1].subRSDKDraw.scriptCodePtr = -1;
                    compilerv3->objectScriptList[scrID - 1].subRSDKDraw.jumpTablePtr  = -1;
                    compilerv3->objectScriptList[scrID - 1].subRSDKLoad.scriptCodePtr = -1;
                    compilerv3->objectScriptList[scrID - 1].subRSDKLoad.jumpTablePtr  = -1;
                    compilerv3->objectScriptList[scrID - 1].subRSDKEdit.scriptCodePtr = -1;
                    compilerv3->objectScriptList[scrID - 1].subRSDKEdit.jumpTablePtr  = -1;

                    scriptError = true;
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
                for (int i = 0; i < gameConfig.objects.count() && !scriptError; ++i) {
                    QString scriptPath = viewer->dataPath + "/Scripts/" + gameConfig.objects[i].script;
                    scriptPath = WorkingDirManager::GetPath("/Scripts/" + gameConfig.objects[i].script,
                                                            scriptPath);
                    compilerv4->ParseScriptFile(scriptPath, scrID++);

                    if (compilerv4->scriptError) {
                        PrintLog(compilerv4->errorMsg);
                        PrintLog(compilerv4->errorPos);
                        PrintLog(QString::number(compilerv4->errorLine));

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

                        SetStatus("Failed to compile script: " + dirFile);
                        compilerv4->objectScriptList[scrID - 1].eventRSDKDraw.scriptCodePtr = -1;
                        compilerv4->objectScriptList[scrID - 1].eventRSDKDraw.jumpTablePtr  = -1;
                        compilerv4->objectScriptList[scrID - 1].eventRSDKLoad.scriptCodePtr = -1;
                        compilerv4->objectScriptList[scrID - 1].eventRSDKLoad.jumpTablePtr  = -1;
                        compilerv4->objectScriptList[scrID - 1].eventRSDKEdit.scriptCodePtr = -1;
                        compilerv4->objectScriptList[scrID - 1].eventRSDKEdit.jumpTablePtr  = -1;

                        scriptError = true;
                    }
                }
            }

            for (int i = 0; i < stageConfig.objects.count() && !scriptError; ++i) {
                QString scriptPath = viewer->dataPath + "/Scripts/" + stageConfig.objects[i].script;
                scriptPath =
                    WorkingDirManager::GetPath("/Scripts/" + stageConfig.objects[i].script, scriptPath);
                compilerv4->ParseScriptFile(scriptPath, scrID++);

                if (compilerv4->scriptError) {
                    PrintLog(compilerv4->errorMsg);
                    PrintLog(compilerv4->errorPos);
                    PrintLog(QString::number(compilerv4->errorLine));

                    QFileInfo info(compilerv4->errorScr);
                    QDir dir(info.dir());
                    dir.cdUp();
                    QString dirFile = dir.relativeFilePath(compilerv4->errorScr);

                    SetStatus("Failed to compile script: " + dirFile);

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

                    scriptError = true;
                }
            }
            break;
        }
    }

    compilerv3->objectLoop      = ENTITY_COUNT_v3 - 1;
    compilerv4->objectEntityPos = ENTITY_COUNT_v4 - 1;
    for (int o = 0; o < viewer->objects.count(); ++o) {
        viewer->objects[o].variables.clear();
        viewer->objects[o].variablesAliases[VAR_ALIAS_PROPVAL] = "propertyValue";
        viewer->objects[o].variablesAliases[VAR_ALIAS_VAL0]    = "value0";
        viewer->objects[o].variablesAliases[VAR_ALIAS_VAL1]    = "value1";
        viewer->objects[o].variablesAliases[VAR_ALIAS_VAL2]    = "value2";
        viewer->objects[o].variablesAliases[VAR_ALIAS_VAL3]    = "value3";

        viewer->activeVarObj = o;
        CallGameEvent(SceneViewer::EVENT_LOAD, o);
    }
    viewer->activeVarObj = -1;

    for (int e = 0; e < viewer->entities.count(); ++e) {
        viewer->entities[e].variables.clear();
        for (int v = 0; v < viewer->objects[viewer->entities[e].type].variables.count(); ++v) {
            RSDKv5::Scene::VariableValue val;
            val.type        = VAR_UINT8;
            val.value_uint8 = 0;
            viewer->entities[e].variables.append(val);
        }
    }

    // Initial property value check (used for capping the value & etc)
    for (int e = 0; e < viewer->entities.count(); ++e) {
        auto *entity   = &viewer->entities[e];
        auto *entityv2 = &compilerv2->objectEntityList[entity->gameEntitySlot];
        auto *entityv3 = &compilerv3->objectEntityList[entity->gameEntitySlot];
        auto *entityv4 = &compilerv4->objectEntityList[entity->gameEntitySlot];

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

void SceneEditor::SetChunk(float x, float y)
{
    if (viewer->selectedChunk >= 0x200 || viewer->selectedLayer < 0)
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

    // Draw Selected Chunk Preview
    float xpos = tx + viewer->cameraPos.x;
    float ypos = ty + viewer->cameraPos.y;

    xpos /= 0x80;
    ypos /= 0x80;
    if (ypos >= 0 && ypos < viewer->layers[viewer->selectedLayer].height) {
        if (xpos >= 0 && xpos < viewer->layers[viewer->selectedLayer].width) {
            viewer->layers[viewer->selectedLayer].layout[ypos][xpos] = viewer->selectedChunk;
        }
    }
}

void SceneEditor::ResetTools(byte tool)
{
    if (tool == 0xFF)
        tool = SceneViewer::TOOL_MOUSE;
    viewer->curTool = tool;

    ui->panTool->blockSignals(true);
    ui->selectTool->blockSignals(true);
    ui->pencilTool->blockSignals(true);
    ui->eraserTool->blockSignals(true);
    ui->entityTool->blockSignals(true);

    ui->panTool->setDown(false);
    ui->selectTool->setDown(false);
    ui->pencilTool->setDown(false);
    ui->eraserTool->setDown(false);
    ui->entityTool->setDown(false);

    switch (tool) {
        default: break; // what
        case SceneViewer::TOOL_MOUSE: ui->panTool->setDown(true); break;
        case SceneViewer::TOOL_SELECT: ui->selectTool->setDown(true); break;
        case SceneViewer::TOOL_PENCIL: ui->pencilTool->setDown(true); break;
        case SceneViewer::TOOL_ERASER: ui->eraserTool->setDown(true); break;
        case SceneViewer::TOOL_ENTITY: ui->entityTool->setDown(true); break;
    }

    ui->panTool->blockSignals(false);
    ui->selectTool->blockSignals(false);
    ui->pencilTool->blockSignals(false);
    ui->eraserTool->blockSignals(false);
    ui->entityTool->blockSignals(false);

    // Reset
    viewer->selectedObject = -1;
    viewer->selectedEntity = -1;
    objProp->unsetUI();
    viewer->isSelecting = false;

    ui->entityList->blockSignals(true);
    ui->entityList->setCurrentRow(viewer->selectedEntity);
    ui->entityList->blockSignals(false);

    unsetCursor();

    switch (tool) {
        default: break; // what
        case SceneViewer::TOOL_MOUSE: break;
        case SceneViewer::TOOL_SELECT: break;
        case SceneViewer::TOOL_PENCIL: viewer->isSelecting = true; break;
        case SceneViewer::TOOL_ERASER: viewer->isSelecting = true; break;
        case SceneViewer::TOOL_ENTITY: viewer->isSelecting = true; break;
    }
};

bool SceneEditor::HandleKeyPress(QKeyEvent *event)
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
                    if (viewer->entities.count() < FormatHelpers::Scene::entityLimit) {
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
        auto *sel = new GoToPos(viewer->layers[viewer->selectedLayer].width * 128, viewer->layers[viewer->selectedLayer].height * 128, viewer->layers[viewer->selectedLayer].name, this);
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
        if (event->key() == Qt::Key_S)
            tool = SceneViewer::TOOL_SELECT;
        if (event->key() == Qt::Key_C)
            tool = SceneViewer::TOOL_PENCIL;
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
            if (event->key() == Qt::Key_Delete || event->key() == Qt::Key_Backspace) {
                if (viewer->selectedEntities.count()) {
                    std::sort(viewer->selectedEntities.begin(), viewer->selectedEntities.end(),
                              [](const int &a, const int &b) -> bool { return a < b; });

                    for (int s = viewer->selectedEntities.count() - 1; s >= 0; --s) {
                        DeleteEntity(viewer->selectedEntities[s], false);
                    }
                    objProp->unsetUI();
                    CreateEntityList();
                    viewer->selectedEntities.clear();
                }
            }
            break;

        case SceneViewer::TOOL_PENCIL:
            if (event->key() == Qt::Key_Z)
                viewer->tileFlip.x = true;

            if (event->key() == Qt::Key_X)
                viewer->tileFlip.y = true;

            if (event->key() == Qt::Key_Delete || event->key() == Qt::Key_Backspace) {
                if (viewer->selectedLayer >= 0) {
                    Rect<float> box;

                    for (int y = 0; y < viewer->sceneBoundsB / 0x80; ++y) {
                        for (int x = 0; x < viewer->sceneBoundsR / 0x80; ++x) {
                            box = Rect<float>(x * 0x80, y * 0x80, 0x80, 0x80);

                            Vector2<float> pos = Vector2<float>(
                                (viewer->tilePos.x * viewer->invZoom()) + viewer->cameraPos.x,
                                (viewer->tilePos.y * viewer->invZoom()) + viewer->cameraPos.y);
                            if (box.contains(pos)) {
                                viewer->layers[viewer->selectedLayer].layout[y][x] = 0;
                                DoAction();

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
bool SceneEditor::HandleKeyRelease(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Control)
        ctrlDownL = false;
    if (event->key() == Qt::Key_Alt)
        altDownL = false;
    if (event->key() == Qt::Key_Shift)
        shiftDownL = false;
    return false;
}

int SceneEditor::AddEntity(int type, float x, float y)
{
    if (x == 0xFFFF)
        x = viewer->cameraPos.x;

    if (y == 0xFFFF)
        y = viewer->cameraPos.y;

    SceneEntity entity;
    entity.type  = type;
    entity.pos.x = x;
    entity.pos.y = y;

    int cnt               = viewer->entities.count();
    entity.slotID         = cnt;
    entity.prevSlot       = entity.slotID;
    entity.gameEntitySlot = entity.slotID;

    entity.variables.clear();
    for (int v = 0; v < viewer->objects[entity.type].variables.count(); ++v) {
        RSDKv5::Scene::VariableValue val;
        val.type        = VAR_UINT8;
        val.value_uint8 = 0;
        entity.variables.append(val);
    }

    viewer->entities.append(entity);
    compilerv2->objectEntityList[entity.gameEntitySlot].type          = viewer->selectedObject;
    compilerv2->objectEntityList[entity.gameEntitySlot].propertyValue = 0;
    compilerv2->objectEntityList[entity.gameEntitySlot].XPos          = x * 65536;
    compilerv2->objectEntityList[entity.gameEntitySlot].YPos          = y * 65536;

    compilerv3->objectEntityList[entity.gameEntitySlot].type          = viewer->selectedObject;
    compilerv3->objectEntityList[entity.gameEntitySlot].propertyValue = 0;
    compilerv3->objectEntityList[entity.gameEntitySlot].XPos          = x * 65536;
    compilerv3->objectEntityList[entity.gameEntitySlot].YPos          = y * 65536;

    compilerv4->objectEntityList[entity.gameEntitySlot].type          = viewer->selectedObject;
    compilerv4->objectEntityList[entity.gameEntitySlot].propertyValue = 0;
    compilerv4->objectEntityList[entity.gameEntitySlot].XPos          = x * 65536;
    compilerv4->objectEntityList[entity.gameEntitySlot].YPos          = y * 65536;

    viewer->selectedEntity = cnt;

    ui->entityList->blockSignals(true);
    ui->entityList->setCurrentRow(viewer->selectedEntity);
    ui->entityList->blockSignals(false);

    QString name = "Unknown Object " + QString::number(entity.type);
    if (entity.type < viewer->objects.count())
        name = viewer->objects[entity.type].name;

    ui->entityList->addItem(QString::number(entity.slotID) + ": " + name);

    auto *entityPtr = &viewer->entities[viewer->selectedEntity];
    objProp->setupUI(entityPtr, viewer->selectedEntity,
                     &compilerv2->objectEntityList[entityPtr->gameEntitySlot],
                     &compilerv3->objectEntityList[entityPtr->gameEntitySlot],
                     &compilerv4->objectEntityList[entityPtr->gameEntitySlot], viewer->gameType);
    ui->propertiesBox->setCurrentWidget(ui->objPropPage);

    return entity.slotID;
}

void SceneEditor::PasteEntity(SceneEntity *copy, float x, float y)
{
    if (x == 0xFFFF)
        x = viewer->cameraPos.x;

    if (y == 0xFFFF)
        y = viewer->cameraPos.y;

    SceneEntity entity;
    entity.type  = copy->type;
    entity.pos.x = x;
    entity.pos.y = y;

    int cnt               = viewer->entities.count();
    entity.slotID         = cnt;
    entity.prevSlot       = entity.slotID;
    entity.gameEntitySlot = entity.slotID;
    entity.propertyValue  = copy->propertyValue;

    entity.variables.clear();
    PrintLog(QString("var count %1").arg(viewer->objects[entity.type].variables.count()));
    for (int v = 0; v < viewer->objects[entity.type].variables.count(); ++v) {
        RSDKv5::Scene::VariableValue val;
        val.type        = VAR_UINT8;
        val.value_uint8 = 0;
        entity.variables.append(val);
    }

    if (viewer->gameType == ENGINE_v4)
        for (int v = 0; v < 0xF; ++v){entity.sceneVariables[v].value = copy->sceneVariables[v].value;}

    viewer->entities.append(entity);
    compilerv2->objectEntityList[entity.gameEntitySlot].type          = copy->type;
    compilerv2->objectEntityList[entity.gameEntitySlot].propertyValue = copy->propertyValue;
    compilerv2->objectEntityList[entity.gameEntitySlot].XPos          = x * 65536;
    compilerv2->objectEntityList[entity.gameEntitySlot].YPos          = y * 65536;

    compilerv3->objectEntityList[entity.gameEntitySlot].type          = copy->type;
    compilerv3->objectEntityList[entity.gameEntitySlot].propertyValue = copy->propertyValue;
    compilerv3->objectEntityList[entity.gameEntitySlot].XPos          = x * 65536;
    compilerv3->objectEntityList[entity.gameEntitySlot].YPos          = y * 65536;

    compilerv4->objectEntityList[entity.gameEntitySlot].type          = copy->type;
    compilerv4->objectEntityList[entity.gameEntitySlot].propertyValue = copy->propertyValue;
    compilerv4->objectEntityList[entity.gameEntitySlot].XPos          = x * 65536;
    compilerv4->objectEntityList[entity.gameEntitySlot].YPos          = y * 65536;

    viewer->selectedEntity = cnt;

    ui->entityList->blockSignals(true);
    ui->entityList->setCurrentRow(viewer->selectedEntity);
    ui->entityList->blockSignals(false);

    QString name = "Unknown Object " + QString::number(entity.type);
    if (entity.type < viewer->objects.count())
        name = viewer->objects[entity.type].name;

    ui->entityList->addItem(QString::number(entity.slotID) + ": " + name);

    auto *entityPtr = &viewer->entities[viewer->selectedEntity];
    objProp->setupUI(entityPtr, viewer->selectedEntity,
                     &compilerv2->objectEntityList[entityPtr->gameEntitySlot],
                     &compilerv3->objectEntityList[entityPtr->gameEntitySlot],
                     &compilerv4->objectEntityList[entityPtr->gameEntitySlot], viewer->gameType);
    ui->propertiesBox->setCurrentWidget(ui->objPropPage);
}

void SceneEditor::DeleteEntity(int slot, bool updateUI)
{
    const SceneEntity &entity = viewer->entities.takeAt(slot);

    compilerv4->objectEntityList[entity.gameEntitySlot].type = 0;
    compilerv3->objectEntityList[entity.gameEntitySlot].type = 0;
    compilerv2->objectEntityList[entity.gameEntitySlot].type = 0;

    if (viewer->entities.count() <= 0) {
        viewer->selectedEntity    = -1;
        viewer->sceneInfo.listPos = -1;
    }

    if (viewer->selectedEntity == slot) {
        viewer->selectedEntity    = -1;
        viewer->sceneInfo.listPos = -1;
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

void SceneEditor::FilterObjectList(QString filter)
{
    bool showAll = filter.length() == 0;

    for (int row = 0; row < ui->objectList->count(); ++row) {
        auto *item = ui->objectList->item(row);
        item->setHidden(!showAll && !item->text().toUpper().contains(filter));
    }
}

void SceneEditor::FilterEntityList(QString filter)
{
    bool showAll = filter.length() == 0;

    for (int row = 0; row < ui->entityList->count(); ++row) {
        auto *item = ui->entityList->item(row);
        item->setHidden(!showAll && !item->text().toUpper().contains(filter));
    }
}

void SceneEditor::CreateEntityList(int startSlot)
{
    ui->entityList->blockSignals(true);

    if (startSlot <= 0) {
        ui->entityList->clear();
        startSlot = 0;
    }
    else {
        for (int i = ui->entityList->count(); i >= startSlot; --i) delete ui->entityList->item(i);
    }

    // C++11 absolute poggers
    std::sort(viewer->entities.begin(), viewer->entities.end(),
              [](const SceneEntity &a, const SceneEntity &b) -> bool { return a.slotID < b.slotID; });

    for (int i = startSlot; i < viewer->entities.count(); ++i) {
        QString name = "Unknown Object " + QString::number(viewer->entities[i].type);
        if (viewer->entities[i].type < viewer->objects.count())
            name = viewer->objects[viewer->entities[i].type].name;

        ui->entityList->addItem(QString::number(viewer->entities[i].slotID) + ": " + name);
    }
    ui->entityList->blockSignals(false);

    for (int i = 0; i < ENTITY_COUNT_v4; ++i) { // v2 == v3 == v4
        memset((void *)&compilerv2->objectEntityList[i], 0, sizeof(compilerv2->objectEntityList[i]));
        memset((void *)&compilerv3->objectEntityList[i], 0, sizeof(compilerv3->objectEntityList[i]));
        memset((void *)&compilerv4->objectEntityList[i], 0, sizeof(compilerv4->objectEntityList[i]));

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
void SceneEditor::CreateScrollList(bool update)
{
    if (update) {
        if (viewer->selectedHScrollInfo >= 0 && viewer->selectedHScrollInfo < viewer->hScroll.count()) {
            ui->hScrollList->blockSignals(true);
            auto &info = viewer->hScroll[viewer->selectedHScrollInfo];
            ui->hScrollList->item(viewer->selectedHScrollInfo)
                ->setText(
                    QString("Parallax: %1, Speed: %2").arg(info.parallaxFactor).arg(info.scrollSpeed));

            ui->hScrollList->blockSignals(false);
        }

        if (viewer->selectedVScrollInfo >= 0 && viewer->selectedVScrollInfo < viewer->hScroll.count()) {
            ui->vScrollList->blockSignals(true);

            auto &info = viewer->vScroll[viewer->selectedVScrollInfo];
            ui->vScrollList->item(viewer->selectedVScrollInfo)
                ->setText(
                    QString("Parallax: %1, Speed: %2").arg(info.parallaxFactor).arg(info.scrollSpeed));

            ui->vScrollList->blockSignals(false);
        }
    }
    else {
        ui->hScrollList->clear();
        ui->vScrollList->clear();

        ui->hScrollList->blockSignals(true);
        for (int i = 0; i < viewer->hScroll.count(); ++i) {
            auto &info = viewer->hScroll[i];
            ui->hScrollList->addItem(
                QString("Parallax: %1, Speed: %2").arg(info.parallaxFactor).arg(info.scrollSpeed));
        }

        ui->hScrollList->blockSignals(false);
        ui->hScrollList->setCurrentRow(-1);

        ui->vScrollList->blockSignals(true);
        for (int i = 0; i < viewer->vScroll.count(); ++i) {
            auto &info = viewer->vScroll[i];
            ui->vScrollList->addItem(
                QString("Parallax: %1, Speed: %2").arg(info.parallaxFactor).arg(info.scrollSpeed));
        }

        ui->vScrollList->blockSignals(false);
        ui->vScrollList->setCurrentRow(-1);
    }
}

bool SceneEditor::CallGameEvent(byte eventID, int id)
{
    if (scriptError)
        return false;

    bool called = false;
    switch (viewer->gameType) {
        case ENGINE_v3:
            switch (eventID) {
                case SceneViewer::EVENT_LOAD: {
                    auto &curObj           = compilerv3->objectScriptList[id];
                    curObj.frameListOffset = compilerv3->scriptFrameCount;
                    curObj.spriteSheetID   = 0;
                    compilerv3->objectEntityList[ENTITY_COUNT_v3 - 1].type = id;

                    auto &curSub = curObj.subRSDKLoad;
                    if (curSub.scriptCodePtr != SCRIPTCODE_COUNT_v3 - 1) {
                        compilerv3->ProcessScript(curSub.scriptCodePtr, curSub.jumpTablePtr,
                                                  Compilerv3::SUB_RSDKLOAD);
                        called = true;
                    }
                    curObj.spriteFrameCount = compilerv3->scriptFrameCount - curObj.frameListOffset;
                    break;
                }
                case SceneViewer::EVENT_DRAW: {
                    SceneEntity *entity = nullptr;
                    if (id == -1) {
                        auto &tmpObj = compilerv3->objectScriptList[viewer->selectedObject];

                        if (tmpObj.subRSDKDraw.scriptCodePtr >= 0 && viewer->selectedObject != 0) {

                            float ex = viewer->tilePos.x;
                            float ey = viewer->tilePos.y;

                            ex *= viewer->invZoom();
                            ey *= viewer->invZoom();

                            compilerv3->objectEntityList[ENTITY_COUNT_v3 - 1].type =
                                viewer->selectedObject;
                            compilerv3->objectEntityList[ENTITY_COUNT_v3 - 1].propertyValue = 0;
                            compilerv3->objectEntityList[ENTITY_COUNT_v3 - 1].XPos =
                                (ex + viewer->cameraPos.x) * 65536.0f;
                            compilerv3->objectEntityList[ENTITY_COUNT_v3 - 1].YPos =
                                (ey + viewer->cameraPos.y) * 65536.0f;
                            compilerv3->objectLoop = ENTITY_COUNT_v3 - 1;
                            id                     = ENTITY_COUNT_v3 - 1;

                            createTempEntity.type           = viewer->selectedObject;
                            createTempEntity.propertyValue  = 0;
                            createTempEntity.pos.x          = (ex + viewer->cameraPos.x) * 65536.0f;
                            createTempEntity.pos.y          = (ey + viewer->cameraPos.y) * 65536.0f;
                            createTempEntity.slotID         = 0xFFFF;
                            createTempEntity.box            = Rect<int>(0, 0, 0, 0);
                            createTempEntity.gameEntitySlot = id;
                            entity = viewer->activeDrawEntity = &createTempEntity;
                        }
                    }
                    else {
                        entity = &viewer->entities[id];

                        compilerv3->objectEntityList[entity->gameEntitySlot].type = entity->type;
                        compilerv3->objectEntityList[entity->gameEntitySlot].propertyValue =
                            entity->propertyValue;
                        compilerv3->objectEntityList[entity->gameEntitySlot].XPos =
                            entity->pos.x * 65536.0f;
                        compilerv3->objectEntityList[entity->gameEntitySlot].YPos =
                            entity->pos.y * 65536.0f;
                    }

                    if (id == -1)
                        return false;

                    auto &curObj = compilerv3->objectScriptList[entity->type];

                    if (curObj.subRSDKDraw.scriptCodePtr != SCRIPTCODE_COUNT_v3 - 1
                        && entity->type != 0) {
                        viewer->sceneInfo.debugMode = false; // always start with overlay mode off
                        compilerv3->objectLoop      = entity->gameEntitySlot;
                        compilerv3->ProcessScript(curObj.subRSDKDraw.scriptCodePtr,
                                                  curObj.subRSDKDraw.jumpTablePtr,
                                                  Compilerv3::SUB_RSDKDRAW);
                        called = true;
                    }
                    break;
                }
                case SceneViewer::EVENT_EDIT: {
                    auto &curObj = compilerv3->objectScriptList[viewer->entities[id].type];

                    if (curObj.subRSDKEdit.scriptCodePtr != SCRIPTCODE_COUNT_v3 - 1
                        && viewer->entities[id].type != 0) {
                        compilerv3->objectLoop = viewer->entities[id].gameEntitySlot;
                        viewer->activeVarObj   = viewer->entities[id].type;

                        compilerv3->ProcessScript(curObj.subRSDKEdit.scriptCodePtr,
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
                case SceneViewer::EVENT_LOAD: {
                    auto &curObj           = compilerv4->objectScriptList[id];
                    curObj.frameListOffset = compilerv4->scriptFrameCount;
                    curObj.spriteSheetID   = 0;
                    compilerv4->objectEntityList[ENTITY_COUNT_v4 - 1].type = id;

                    auto &curSub = curObj.eventRSDKLoad;
                    if (curSub.scriptCodePtr != SCRIPTCODE_COUNT_v3 - 1) {
                        compilerv4->ProcessScript(curSub.scriptCodePtr, curSub.jumpTablePtr,
                                                  Compilerv4::EVENT_RSDKLOAD);
                        called = true;
                    }
                    curObj.spriteFrameCount = compilerv4->scriptFrameCount - curObj.frameListOffset;
                    break;
                }
                case SceneViewer::EVENT_DRAW: {
                    SceneEntity *entity = nullptr;
                    if (id == -1) {
                        auto &tmpObj = compilerv4->objectScriptList[viewer->selectedObject];

                        if (tmpObj.eventRSDKDraw.scriptCodePtr >= 0 && viewer->selectedObject != 0) {

                            float ex = viewer->tilePos.x;
                            float ey = viewer->tilePos.y;

                            ex *= viewer->invZoom();
                            ey *= viewer->invZoom();

                            compilerv4->objectEntityList[ENTITY_COUNT_v4 - 1].type =
                                viewer->selectedObject;
                            compilerv4->objectEntityList[ENTITY_COUNT_v4 - 1].propertyValue = 0;
                            compilerv4->objectEntityList[ENTITY_COUNT_v4 - 1].XPos =
                                (ex + viewer->cameraPos.x) * 65536.0f;
                            compilerv4->objectEntityList[ENTITY_COUNT_v4 - 1].YPos =
                                (ey + viewer->cameraPos.y) * 65536.0f;
                            compilerv4->objectEntityPos = ENTITY_COUNT_v4 - 1;
                            id                          = ENTITY_COUNT_v4 - 1;

                            createTempEntity.type           = viewer->selectedObject;
                            createTempEntity.propertyValue  = 0;
                            createTempEntity.pos.x          = (ex + viewer->cameraPos.x) * 65536.0f;
                            createTempEntity.pos.y          = (ey + viewer->cameraPos.y) * 65536.0f;
                            createTempEntity.slotID         = 0xFFFF;
                            createTempEntity.box            = Rect<int>(0, 0, 0, 0);
                            createTempEntity.gameEntitySlot = id;
                            entity = viewer->activeDrawEntity = &createTempEntity;
                        }
                    }
                    else {
                        entity = &viewer->entities[id];

                        compilerv4->objectEntityList[entity->gameEntitySlot].type = entity->type;
                        compilerv4->objectEntityList[entity->gameEntitySlot].propertyValue =
                            entity->propertyValue;
                        compilerv4->objectEntityList[entity->gameEntitySlot].XPos =
                            entity->pos.x * 65536.0f;
                        compilerv4->objectEntityList[entity->gameEntitySlot].YPos =
                            entity->pos.y * 65536.0f;
                    }

                    if (id == -1)
                        return false;

                    auto &curObj = compilerv4->objectScriptList[entity->type];

                    if (curObj.eventRSDKDraw.scriptCodePtr != SCRIPTCODE_COUNT_v3 - 1
                        && entity->type != 0) {
                        viewer->sceneInfo.debugMode = false; // always start with overlay mode off
                        compilerv4->objectEntityPos = entity->gameEntitySlot;
                        compilerv4->ProcessScript(curObj.eventRSDKDraw.scriptCodePtr,
                                                  curObj.eventRSDKDraw.jumpTablePtr,
                                                  Compilerv4::EVENT_RSDKDRAW);
                        called = true;
                    }
                    break;
                }
                case SceneViewer::EVENT_EDIT: {
                    auto &curObj = compilerv4->objectScriptList[viewer->entities[id].type];

                    if (curObj.eventRSDKEdit.scriptCodePtr != SCRIPTCODE_COUNT_v3 - 1
                        && viewer->entities[id].type != 0) {
                        compilerv4->objectEntityPos = viewer->entities[id].gameEntitySlot;
                        viewer->activeVarObj        = viewer->entities[id].type;

                        compilerv4->ProcessScript(curObj.eventRSDKEdit.scriptCodePtr,
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

void SceneEditor::ParseGameXML(QString path)
{
    gameConfig          = FormatHelpers::GameConfig();
    gameConfig.filePath = path;

    Reader fileReader = Reader(path);
    QByteArray bytes  = fileReader.readByteArray(fileReader.filesize, false);

    QXmlStreamReader xmlReader = QXmlStreamReader(bytes);

    int list          = -1;
    bool objectFlag   = false;
    bool variableFlag = false;
    bool soundFXFlag  = false;
    bool playerFlag   = false;
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
            else if (name == "sounds") {
                soundFXFlag ^= 1;
            }
            else if (soundFXFlag && name == "soundfx") {
                QString sfxName = "";
                QString sfxPath = "";
                for (const QXmlStreamAttribute &attr : xmlReader.attributes()) {
                    if (attr.name().toString() == QLatin1String("name")) {
                        sfxName = attr.value().toString();
                    }
                    if (attr.name().toString() == QLatin1String("path")) {
                        sfxPath = attr.value().toString();
                    }
                }
                FormatHelpers::GameConfig::SoundInfo sfx;
                sfx.name = sfxName;
                sfx.path = sfxPath;
                gameConfig.soundFX.append(sfx);
            }
            else if (name == "variables") {
                variableFlag ^= 1;
            }
            else if (variableFlag && name == "variable") {
                QString varName = "";
                int varValue    = 0;
                for (const QXmlStreamAttribute &attr : xmlReader.attributes()) {
                    if (attr.name().toString() == QLatin1String("name")) {
                        varName = attr.value().toString();
                    }
                    if (attr.name().toString() == QLatin1String("script")) {
                        varValue = attr.value().toInt();
                    }
                }
                FormatHelpers::GameConfig::GlobalVariable var;
                var.name  = varName;
                var.value = varValue;
                gameConfig.globalVariables.append(var);
            }
            else if (name == "players") {
                playerFlag ^= 1;
            }
            else if (playerFlag && name == "player") {
                QString plrName = "";
                for (const QXmlStreamAttribute &attr : xmlReader.attributes()) {
                    if (attr.name().toString() == QLatin1String("name")) {
                        plrName = attr.value().toString();
                    }
                }
                FormatHelpers::GameConfig::PlayerInfo plr;
                plr.name = plrName;
                gameConfig.players.append(plr);
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
                stage.name        = stgName;
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

void SceneEditor::UndoAction()
{
    if (actionIndex > 0) {
        actionIndex--;
        ResetAction();
    }
}
void SceneEditor::RedoAction()
{
    if (actionIndex + 1 < actions.count()) {
        actionIndex++;
        ResetAction();
    }
}
void SceneEditor::ResetAction()
{
#if RE_USE_UNSTABLE
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
    viewer->selectedChunk  = actions[actionIndex].selectedChunk;
    viewer->selectedLayer = actions[actionIndex].selectedLayer;

    // Collision
    // viewer->showPlaneA = actions[actionIndex].showPlaneA;
    // viewer->showPlaneB = actions[actionIndex].showPlaneB;

    // Entity Editing
    viewer->selectedObject = actions[actionIndex].selectedObject; // placing
    viewer->selectedEntity = actions[actionIndex].selectedEntity; // viewing

    // Parallax Editing
    // viewer->showParallax       = actions[actionIndex].showParallax;
    viewer->selectedHScrollInfo = actions[actionIndex].selectedScrollInfo;

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
#endif

    UpdateTitle(actionIndex > 0);
}
void SceneEditor::DoAction(QString name, bool setModified)
{
    ActionState action;

    action.name = name;

#if RE_USE_UNSTABLE
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
    action.selectedChunk  = viewer->selectedChunk;
    action.selectedLayer = viewer->selectedLayer;

    // Collision
    // action.showPlaneA = viewer->showPlaneA;
    // action.showPlaneB = viewer->showPlaneB;

    // Entity Editing
    action.selectedObject = viewer->selectedObject; // placing
    action.selectedEntity = viewer->selectedEntity; // viewing

    // Parallax Editing
    // action.showParallax       = viewer->showParallax;
    action.selectedScrollInfo = viewer->selectedHScrollInfo;

    // Camera
    action.camPos = viewer->cameraPos;

    // action.showTileGrid = viewer->showGrid;

    // action.compilerv2 = viewer->compilerv2;
    // action.compilerv3 = viewer->compilerv3;
    // action.compilerv4 = viewer->compilerv4;
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
void SceneEditor::ClearActions()
{
    actions.clear();
    actionIndex = 0;
    DoAction("Action Setup", false); // first action, cant be undone
}

// XML Management
void SceneEditor::ReadXMLScrollInfo(QXmlStreamReader &xmlReader, int layerID, byte mode)
{
    if (layerID <= 0 || layerID >= 9)
        return;

    auto &layer = viewer->layers[layerID];
    layer.scrollInfos.clear();
    // Parse the XML until we reach end of it
    while (!xmlReader.atEnd() && !xmlReader.hasError() && mode <= 2) {
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
            else if (name == "layers")  // for what is this?
                mode = 2;
            else if (mode && name == "scrollInfo") {
                int startLine        = 0;
                int length           = 0;
                float parallaxFactor = 1.0f;
                float scrollSpeed    = 0.0f;
                bool deform          = false;
                int attachLayer      = 0;
                bool repeatedScroll  = false;
                int c = 0;
                for (const QXmlStreamAttribute &attr : xmlReader.attributes()) {
                    if (attr.name().toString() == QLatin1String("parallaxFactor"))
                        parallaxFactor = attr.value().toFloat();
                    if (attr.name().toString() == QLatin1String("scrollSpeed"))
                        scrollSpeed = attr.value().toFloat();
                    if (attr.name().toString() == QLatin1String("deform"))
                        deform = attr.value().toString() != "false" && attr.value().toString() != "0";
                }

                for (auto &scrollCheck : mode == 1 ? viewer->hScroll : viewer->vScroll){
                    if(scrollCheck.parallaxFactor == parallaxFactor && scrollCheck.scrollSpeed == scrollSpeed && scrollCheck.deform == deform){
                        repeatedScroll = true;
                        PrintLog(QString("Repeated scroll."));
                        break;
                    }
                    c++;
                }
                SceneHelpers::TileLayer::ScrollIndexInfo info;
                // TODO: new parallax may be created due to float imprecision during import?
                info.parallaxFactor = parallaxFactor;
                info.scrollSpeed    = scrollSpeed;
                info.deform         = deform;
                info.instances.clear();

                while (xmlReader.readNextStartElement() && xmlReader.name() == "scrollInstance"){
                    bool repeatedInst    = false;
                    for (const QXmlStreamAttribute &subAttr : xmlReader.attributes()) {
                        if (subAttr.name().toString() == QLatin1String("startLine"))
                            startLine = subAttr.value().toInt();
                        if (subAttr.name().toString() == QLatin1String("length"))
                            length = subAttr.value().toInt();
                        if (subAttr.name().toString() == QLatin1String("attachedLayer"))
                            attachLayer = subAttr.value().toInt();
                    }

                    if (repeatedScroll){
                        auto &test = viewer->hScroll[c];
                        for (auto &instCheck : test.instances){
                            if(instCheck.startLine == startLine && instCheck.length == length && instCheck.layerID == attachLayer){
                                repeatedInst = true;
                                PrintLog(QString("Repeated instance."));
                                break;
                            }
                        }
                    }

                    // remove this check to add all instances regardless of layerID as long as the background is valid
                    if (attachLayer == layerID){
                        SceneHelpers::TileLayer::ScrollInstance instance;
                        instance.startLine      = startLine;
                        instance.length         = length;
                        instance.layerID        = attachLayer;
                        info.instances.append(instance);
                        if (!repeatedInst){
                            layer.scrollInfos.append(info);
                        }
                    }
                    xmlReader.readNext();
                }
                if(!repeatedScroll){
                    if (mode == 1){viewer->hScroll.append(info);}
                    if (mode == 2){viewer->vScroll.append(info);}   // bug: seems unrelated to this function, but the editor will not save vertical instances no matter what
                }
                else{
                    if (mode == 1){viewer->hScroll.removeAt(c);viewer->hScroll.append(info);}
                    if (mode == 2){viewer->vScroll.removeAt(c);viewer->vScroll.append(info);}
                }

            }
        }
    }
}

void SceneEditor::ReadXMLLayout(QXmlStreamReader &xmlReader, int layerID, byte mode)
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

void SceneEditor::ReadXMLLayers(QXmlStreamReader &xmlReader)
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
                    int width = 0, height = 0, drawGroup = 0;
                    float parallaxFactor = 1.0f, scrollSpeed = 0.0f;
                    bool visible = false;
                    for (const QXmlStreamAttribute &attr : xmlReader.attributes()) {
                        if (attr.name().toString() == QLatin1String("name"))
                            name = attr.value().toString();
                        if (attr.name().toString() == QLatin1String("type"))
                            type = attr.value().toString();
                        if (attr.name().toString() == QLatin1String("drawGroup"))
                            drawGroup = attr.value().toInt();
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
                    ReadXMLScrollInfo(xmlReader, layerID, 1);
                else if (name == "layout")
                    ReadXMLLayout(xmlReader, layerID, 1);
            }
        }
    }
}

void SceneEditor::WriteXMLScrollInfo(Writer &writer, int layerID, int indentPos)
{
    if (!layerID)
        return;

    auto &layer = viewer->layers[layerID];

    if (layer.type == SceneHelpers::TileLayer::LAYER_HSCROLL && viewer->hScroll.count()) {
        writeXMLIndentation(writer, indentPos++);
        writer.writeLine("<scrollingInfo>");
        for (auto &scroll : viewer->hScroll) {
            writeXMLIndentation(writer, indentPos);

            writer.writeLine(QString("<scrollInfo parallaxFactor=\"%1\" "
                                     "scrollSpeed=\"%2\" deform=\"%3\">")
                                 .arg(scroll.parallaxFactor, 0, 'f', 8)
                                 .arg(scroll.scrollSpeed, 0, 'f', 8)
                                 .arg(scroll.deform ? "true" : "false"));
            ++indentPos;
            for (auto instance : scroll.instances) {
                writeXMLIndentation(writer, indentPos);
                writer.writeLine(QString("<scrollInstance startLine=\"%1\" "
                                         "length=\"%2\" "
                                         "attachedLayer=\"%3\"></scrollInstance>" )
                                     .arg(instance.startLine)
                                     .arg(instance.length)
                                     .arg(instance.layerID));
            }
            --indentPos;
            writeXMLIndentation(writer, indentPos);
            writer.writeLine(QString("</scrollInfo>"));
        }
        writeXMLIndentation(writer, --indentPos);
        writer.writeLine("</scrollingInfo>");
        writer.writeLine();
    }

    if (layer.type == SceneHelpers::TileLayer::LAYER_VSCROLL && viewer->vScroll.count()) {
        writeXMLIndentation(writer, indentPos++);
        writer.writeLine("<scrollingInfo>");
        for (auto &scroll : viewer->vScroll) {
            writeXMLIndentation(writer, indentPos);

            writer.writeLine(QString("<scrollInfo parallaxFactor=\"%1\" "
                                     "scrollSpeed=\"%2\" deform=\"%3\">")
                                 .arg(scroll.parallaxFactor)
                                 .arg(scroll.scrollSpeed)
                                 .arg(scroll.deform ? "true" : "false"));
            ++indentPos;
            for (auto instance : scroll.instances) {
                writeXMLIndentation(writer, indentPos);
                writer.writeLine(QString("<scrollInstance startLine=\"%1\" "
                                         "length=\"%2\" "
                                         "attachedLayer=\"%3\"></scrollInstance>" )
                                     .arg(instance.startLine)
                                     .arg(instance.length)
                                     .arg(instance.layerID));
            }
            --indentPos;
            writeXMLIndentation(writer, indentPos);
            writer.writeLine(QString("</scrollInfo>"));
        }
        writeXMLIndentation(writer, --indentPos);
        writer.writeLine("</scrollingInfo>");
        writer.writeLine();
    }
}

void SceneEditor::WriteXMLLayout(Writer &writer, int layerID, int indentPos)
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

void SceneEditor::WriteXMLLayer(Writer &writer, int layerID, int indentPos)
{
    QList<QString> layerTypes = { "None", "HScroll", "VScroll", "3D Sky", "3D Floor" };

    auto &layer = viewer->layers[layerID];

    int drawGroup = 16;

    if (viewer->gameType != ENGINE_v4) {
        // Draw Object Groups 0
        if (drawGroup == 16 && layerID == scene.activeLayer[0] && scene.activeLayer[0] < 9)
            drawGroup = 0;

        // Draw Object Groups 1
        if (drawGroup == 16 && layerID == scene.activeLayer[1] && scene.activeLayer[1] < 9)
            drawGroup = 1;

        // Draw Object Groups 2
        if (drawGroup == 16 && layerID == scene.activeLayer[2] && scene.activeLayer[2] < 9)
            drawGroup = 2;

        // Draw Object Groups 3, 4
        if (drawGroup == 16 && layerID == scene.activeLayer[3] && scene.activeLayer[3] < 9)
            drawGroup = 6;

        // Draw Object Groups 5, 6
    }
    else {
        if (scene.midpoint < 6) {
            // Draw Object Groups 0
            if (drawGroup == 16 && layerID == scene.activeLayer[0] && scene.activeLayer[0] < 9)
                drawGroup = 0;

            // Draw Object Groups 1
            if (drawGroup == 16 && layerID == scene.activeLayer[1] && scene.activeLayer[1] < 9)
                drawGroup = 1;
        }

        if (scene.midpoint < 3) {
            // Draw Object Groups 2, 3, 4

            if (drawGroup == 16 && layerID == scene.activeLayer[2] && scene.activeLayer[2] < 9)
                drawGroup = 4;
        }
        else if (scene.midpoint < 6) {
            // Draw Object Groups 2
            if (drawGroup == 16 && layerID == scene.activeLayer[2] && scene.activeLayer[2] < 9)
                drawGroup = 2;

            // Draw Object Groups 3, 4
        }

        if (scene.midpoint < 6) {
            if (drawGroup == 16 && layerID == scene.activeLayer[3] && scene.activeLayer[3] < 9)
                drawGroup = 6;

            // Draw Object Groups 5, 6
        }
    }

    if ((layer.width == 0 || layer.height == 0) && drawGroup == 16)
        return;

    writeXMLIndentation(writer, indentPos);
    writer.writeLine(QString("<layer name=\"%1\" type=\"%2\" drawGroup=\"%3\" width=\"%4\" "
                             "height=\"%5\" parallaxFactor=\"%6\" scrollSpeed=\"%7\" "
                             "visible=\"%8\">")
                         .arg(viewer->layers[layerID].name)
                         .arg(layerTypes[layer.type])
                         .arg(drawGroup)
                         .arg(layer.width)
                         .arg(layer.height)
                         .arg(layer.parallaxFactor)
                         .arg(layer.scrollSpeed)
                         .arg(viewer->layers[layerID].visible ? "true" : "false"));

    WriteXMLScrollInfo(writer, layerID, indentPos + 1);

    WriteXMLLayout(writer, layerID, indentPos + 1);

    writeXMLIndentation(writer, indentPos);
    writer.writeLine("</layer>");
    writer.writeLine();
}

void SceneEditor::WriteXMLObject(Writer &writer, int objID, int indentPos)
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
                                 .arg("enum"));
        }
    }
    writeXMLIndentation(writer, indentPos);
    writer.writeLine("</object>");
}

void SceneEditor::WriteXMLEntity(Writer &writer, int entityID, int indentPos)
{

    QList<QString> types = { "uint8", "uint16", "uint32", "int8",    "int16", "int32",
                             "enum",  "bool",   "string", "vector2", "float", "color" };

    auto &entity = viewer->entities[entityID];

    writeXMLIndentation(writer, indentPos);
    writer.writeLine(QString("<entity name=\"%1\" slotID=\"%2\" x=\"%3\" y=\"%4\">")
                         .arg(viewer->objects[entity.type].name)
                         .arg(entity.slotID)
                         .arg(entity.pos.x)
                         .arg(entity.pos.y));

    writeXMLIndentation(writer, indentPos + 1);
    writer.writeLine(QString("<variable name=\"%1\" type=\"uint8\" value=\"%2\">%2</variable>")
                         .arg(viewer->objects[entity.type].variablesAliases[VAR_ALIAS_PROPVAL])
                         .arg(entity.propertyValue)
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
            writer.writeLine(QString("<variable name=\"%1\" type=\"%2\" value=\"%3\">%3</variable>")
                                 .arg(name)
                                 .arg(RSDKv4::objectVariableTypes[v])
                                 .arg(entity.sceneVariables[v].value));
        }
    }

    for (int v = 0; v < viewer->objects[entity.type].variables.count(); ++v) {
        int value = objProp->callRSDKEdit(this, true, entityID, v, 0);

        writeXMLIndentation(writer, indentPos + 1);
        writer.writeText(QString("<variable name=\"%1\" type=\"%2\" value=\"%3\">")
                             .arg(viewer->objects[entity.type].variables[v].name)
                             .arg("enum")
                             .arg(value));

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

void SceneEditor::WriteXMLChunk(Writer &writer, int chunkID, int indentPos)
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

void SceneEditor::WriteXMLScene(Writer &writer)
{
    int indentPos = 0;

    writer.writeLine("<scene>");

    QList<QString> types = { "uint8", "uint16", "uint32", "int8",    "int16", "int32",
                             "enum",  "bool",   "string", "vector2", "float", "color" };

    indentPos++;
    {
        uint bgClr1 = (viewer->metadata.backgroundColor1.red() << 16)
                      | (viewer->metadata.backgroundColor1.green() << 8)
                      | (viewer->metadata.backgroundColor1.blue() << 0) | (0xFF << 24);
        uint bgClr2 = (viewer->metadata.backgroundColor2.red() << 16)
                      | (viewer->metadata.backgroundColor2.green() << 8)
                      | (viewer->metadata.backgroundColor2.blue() << 0) | (0xFF << 24);
        writer.writeLine(
            QString("\t<metadata bgColor1=\"%1\" "
                    "bgColor2=\"%2\" title=\"%3\" layerMidpoint=\"%4\" activeLayer0=\"%5\" "
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
            (void)layer;

            WriteXMLLayer(writer, id, indentPos + 1);
            ++id;
        }

        writeXMLIndentation(writer, indentPos);
        writer.writeLine("</layers>");
    }
    indentPos--;

    indentPos++;
    if (viewer->objects.count()) {
        writer.writeLine();
        writeXMLIndentation(writer, indentPos);
        writer.writeLine("<objects>");

        for (int o = 0; o < viewer->objects.count(); ++o) WriteXMLObject(writer, o, indentPos + 1);

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

        for (int e = 0; e < viewer->entities.count(); ++e) WriteXMLEntity(writer, e, indentPos + 1);

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
            WriteXMLChunk(writer, c, indentPos + 1);

        writeXMLIndentation(writer, indentPos);
        writer.writeLine("</chunks>");
        writer.writeLine();
    }
    indentPos--;

    writer.writeLine("</scene>");
}

ushort SceneEditor::LoadSpriteSheet(QString filename)
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

void SceneEditor::DrawSpriteFlipped(float XPos, float YPos, float width, float height, float sprX,
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

void SceneEditor::DrawSpriteRotozoom(float XPos, float YPos, float pivotX, float pivotY, float width,
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
