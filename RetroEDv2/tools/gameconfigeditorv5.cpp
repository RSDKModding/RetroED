#include "includes.hpp"
#include "ui_gameconfigeditorv5.h"

#include "tools/gameconfigeditorv5.hpp"

#include <RSDKv2/gameconfigv2.hpp>
#include <RSDKv3/gameconfigv3.hpp>
#include <RSDKv4/gameconfigv4.hpp>
#include <RSDKv5/gameconfigv5.hpp>

#include "paletteeditor.hpp"

GameConfigEditorv5::GameConfigEditorv5(QString configPath, byte type, bool oldVer, QWidget *parent)
    : QWidget(parent), ui(new Ui::GameConfigEditorv5)
{
    ui->setupUi(this);

    ui->gcSectionList->blockSignals(true);
    ui->gcSectionList->setCurrentRow(-1);
    ui->gcSectionList->blockSignals(false);
    ui->gcSectionList->setCurrentRow(0);

    ui->scSectionList->blockSignals(true);
    ui->scSectionList->setCurrentRow(-1);
    ui->scSectionList->blockSignals(false);
    ui->scSectionList->setCurrentRow(0);

    connect(ui->gcSectionList, &QListWidget::currentRowChanged, [this](int v) {
        ui->gcObjList->blockSignals(true);
        ui->gcObjList->setCurrentRow(0);
        ui->gcObjList->blockSignals(false);
        ui->gcObjList->setCurrentRow(-1);

        ui->gcSfxList->blockSignals(true);
        ui->gcSfxList->setCurrentRow(0);
        ui->gcSfxList->blockSignals(false);
        ui->gcSfxList->setCurrentRow(-1);

        ui->gcVarList->blockSignals(true);
        ui->gcVarList->setCurrentRow(0);
        ui->gcVarList->blockSignals(false);
        ui->gcVarList->setCurrentRow(-1);

        ui->gcDetailsWidget->setCurrentIndex(v);
        ui->gcSectionLabel->setText(ui->gcSectionList->item(v)->text());
    });

    connect(ui->scSectionList, &QListWidget::currentRowChanged, [this](int v) {
        ui->scObjList->blockSignals(true);
        ui->scObjList->setCurrentRow(0);
        ui->scObjList->blockSignals(false);
        ui->scObjList->setCurrentRow(-1);

        ui->scSfxList->blockSignals(true);
        ui->scSfxList->setCurrentRow(0);
        ui->scSfxList->blockSignals(false);
        ui->scSfxList->setCurrentRow(-1);

        ui->scDetailsWidget->setCurrentIndex(v);
        ui->scSectionLabel->setText(ui->scSectionList->item(v)->text());
    });

    auto setupInitScenes = [this] {
        ui->gcStartingScene->blockSignals(true);
        ui->gcStartingScene->clear();

        if (ui->gcStartingCategory->currentIndex() >= 0) {
            for (int i = 0;
                 i < gameConfig.categories[ui->gcStartingCategory->currentIndex()].scenes.count();
                 ++i) {
                ui->gcStartingScene->addItem(
                    gameConfig.categories[ui->gcStartingCategory->currentIndex()].scenes[i].name);
            }
        }
        ui->gcStartingScene->blockSignals(false);
    };

    auto setupInitCategories = [this, setupInitScenes] {
        ui->gcStartingCategory->blockSignals(true);
        ui->gcStartingCategory->clear();
        for (int i = 0; i < gameConfig.categories.count(); ++i)
            ui->gcStartingCategory->addItem(gameConfig.categories[i].name);

        setupInitScenes();
        ui->gcStartingCategory->blockSignals(false);
    };

    QCheckBox *filterToggles[] = { ui->gcScnFilter1, ui->gcScnFilter2, ui->gcScnFilter3,
                                   ui->gcScnFilter4, ui->gcScnFilter5, ui->gcScnFilter6,
                                   ui->gcScnFilter7, ui->gcScnFilter8 };

    sceneModel = new QStandardItemModel(ui->gcScnTree);

    QMenu *scnAddMenu = new QMenu(ui->gcAddScn);

    auto addCat = [this, setupInitCategories] {
        const QModelIndex &index = ui->gcScnTree->currentIndex();
        bool scnSelected         = index.parent().isValid();
        uint c                   = scnSelected ? index.parent().row() + 1 : index.row() + 1;
        gameConfig.categories.insert(c, RSDKv5::GameConfig::Category());
        auto *catItem = new QStandardItem(gameConfig.categories[c].name);
        auto *scnItem = new QStandardItem(gameConfig.categories[c].scenes.first().name);
        catItem->appendRow(scnItem);
        catItem->setFlags(catItem->flags() | Qt::ItemIsEditable);
        sceneModel->insertRow(c, catItem);
        ui->gcScnTree->setCurrentIndex(sceneModel->indexFromItem(catItem));
        ui->gcScnTree->setExpanded(QModelIndex(ui->gcScnTree->currentIndex()), true);

        DoAction("Added Category");
        setupInitCategories();
    };

    scnAddMenu->addAction("Category", addCat);

    scnAddMenu->addAction("Scene", [this, addCat, setupInitScenes] {
        const QModelIndex &index = ui->gcScnTree->currentIndex();
        bool scnSelected         = index.parent().isValid();
        if (!index.isValid()) {
            addCat();
            return;
        }
        uint cat = scnSelected ? index.parent().row() : index.row();
        uint scn = scnSelected ? index.row() + 1 : 0;
        gameConfig.categories[cat].scenes.insert(scn, RSDKv5::GameConfig::SceneInfo());
        auto *scnItem = new QStandardItem(gameConfig.categories[cat].scenes[scn].name);
        if (scnSelected)
            sceneModel->itemFromIndex(index.parent())->insertRow(scn, scnItem);
        else
            sceneModel->itemFromIndex(index)->insertRow(scn, scnItem);
        scnItem->setFlags(scnItem->flags() | Qt::ItemIsEditable);
        ui->gcScnTree->setCurrentIndex(sceneModel->indexFromItem(scnItem));

        DoAction("Added Scene");
        setupInitScenes();
    });

    connect(sceneModel, &QStandardItemModel::itemChanged, [this](QStandardItem *item) {
        const QModelIndex &index = sceneModel->indexFromItem(item);
        if (index.parent().isValid()) { // Scene
            gameConfig.categories[index.parent().row()].scenes[index.row()].name = item->text();
            DoAction("Changed Scene Name");
            return;
        }
        gameConfig.categories[index.row()].name = item->text();
        DoAction("Changed Category Name");
    });

    ui->gcAddScn->setMenu(scnAddMenu);

    connect(ui->gcRmScn, &QToolButton::clicked, [this, setupInitCategories, setupInitScenes] {
        const QModelIndex &index = ui->gcScnTree->currentIndex();

        if (index.parent().isValid()) { // Scene
            gameConfig.categories[index.parent().row()].scenes.removeAt(index.row());
            sceneModel->itemFromIndex(index.parent())->removeRow(index.row());
            setupInitScenes();
            DoAction("Removed Scene");
            return;
        }
        gameConfig.categories.removeAt(index.row());
        sceneModel->removeRow(index.row());

        setupInitCategories();
        DoAction("Removed Category");
    });

    auto moveScn = [this, setupInitCategories, setupInitScenes](char translation) {
        bool isScn = ui->gcScnTree->currentIndex().parent().isValid();
        int c      = ui->gcScnTree->currentIndex().row();
        QList<QStandardItem *> item;
        if (isScn) {
            gameConfig.categories[ui->gcScnTree->currentIndex().parent().row()].scenes.move(
                c, c + translation);
            QStandardItem *parentItem =
                sceneModel->itemFromIndex(ui->gcScnTree->currentIndex().parent());
            item = parentItem->takeRow(c);
            parentItem->insertRow(c + translation, item);
            setupInitScenes();
        }
        else {
            gameConfig.categories.move(c, c + translation);
            bool expand = ui->gcScnTree->isExpanded(ui->gcScnTree->currentIndex());
            item        = sceneModel->takeRow(c), sceneModel->insertRow(c + translation, item);
            ui->gcScnTree->setExpanded(sceneModel->indexFromItem(item.first()), expand);
            setupInitCategories();
        }
        DoAction(isScn ? "Moved Scene" : "Moved Category");
        ui->gcScnTree->setCurrentIndex(sceneModel->indexFromItem(item.first()));
    };

    connect(ui->gcUpScn, &QToolButton::clicked, [moveScn] { moveScn(-1); });

    connect(ui->gcDownScn, &QToolButton::clicked, [moveScn] { moveScn(1); });

    ui->gcScnTree->setModel(sceneModel);

    connect(
        ui->gcScnTree->selectionModel(), &QItemSelectionModel::currentChanged,
        [this, filterToggles](const QModelIndex &c) {
            ui->gcUpScn->setDisabled(!c.isValid());
            ui->gcDownScn->setDisabled(!c.isValid());
            ui->gcRmScn->setDisabled(!c.isValid());

            disconnect(ui->gcScnID, nullptr, nullptr, nullptr);
            disconnect(ui->gcScnFolder, nullptr, nullptr, nullptr);
            disconnect(ui->gcScnName, nullptr, nullptr, nullptr);

            disconnect(ui->gcScnFilterValue, nullptr, nullptr, nullptr);
            for (int f = 0; f < 8; ++f) disconnect(filterToggles[f], nullptr, nullptr, nullptr);

            ui->gcScnID->setDisabled(!c.parent().isValid());
            ui->gcScnFolder->setDisabled(!c.parent().isValid());
            ui->gcScnName->setDisabled(!c.parent().isValid());

            ui->gcScnFilterValue->setDisabled(!c.parent().isValid());
            for (int f = 0; f < 8; ++f) filterToggles[f]->setDisabled(!c.parent().isValid());

            if (!c.isValid())
                return;

            if (c.parent().isValid()) {
                ui->gcScnID->setDisabled(c.row()
                                         >= gameConfig.categories[c.parent().row()].scenes.count());
                ui->gcScnFolder->setDisabled(c.row()
                                             >= gameConfig.categories[c.parent().row()].scenes.count());
                ui->gcScnName->setDisabled(c.row()
                                           >= gameConfig.categories[c.parent().row()].scenes.count());

                ui->gcScnFilterValue->setDisabled(
                    c.row() >= gameConfig.categories[c.parent().row()].scenes.count());

                for (int f = 0; f < 8; ++f) {
                    filterToggles[f]->setDisabled(
                        c.row() >= gameConfig.categories[c.parent().row()].scenes.count());
                }

                if (c.row() >= gameConfig.categories[c.parent().row()].scenes.count())
                    return;

                ui->gcScnID->setText(gameConfig.categories[c.parent().row()].scenes[c.row()].id);
                ui->gcScnFolder->setText(
                    gameConfig.categories[c.parent().row()].scenes[c.row()].folder);
                ui->gcScnName->setText(gameConfig.categories[c.parent().row()].scenes[c.row()].name);

                ui->gcScnFilterValue->setText(
                    QString("Filter Value: %1")
                        .arg(gameConfig.categories[c.parent().row()].scenes[c.row()].filter));

                for (int f = 0; f < 8; ++f) {
                    filterToggles[f]->setChecked(Utils::getBit(
                        gameConfig.categories[c.parent().row()].scenes[c.row()].filter, f));
                }

                connect(ui->gcScnName, &QLineEdit::editingFinished, [this, c]() {
                    gameConfig.categories[c.parent().row()].scenes[c.row()].name =
                        ui->gcScnName->text();
                    sceneModel->itemFromIndex(c)->setText(ui->gcScnName->text());
                    DoAction("Changed Scene Name");
                });

                connect(ui->gcScnFolder, &QLineEdit::editingFinished, [this, c]() {
                    gameConfig.categories[c.parent().row()].scenes[c.row()].folder =
                        ui->gcScnFolder->text();
                    DoAction("Changed Scene Folder");
                });

                connect(ui->gcScnID, &QLineEdit::editingFinished, [this, c]() {
                    gameConfig.categories[c.parent().row()].scenes[c.row()].id = ui->gcScnID->text();
                    DoAction("Changed Scene ID");
                });

                for (int f = 0; f < 8; ++f) {
                    connect(filterToggles[f], &QCheckBox::toggled, [this, c, f] {
                        gameConfig.categories[c.parent().row()].scenes[c.row()].filter ^= (1 << f);

                        ui->gcScnFilterValue->setText(
                            QString("Filter Value: %1")
                                .arg(gameConfig.categories[c.parent().row()].scenes[c.row()].filter));

                        DoAction(QString("Changed Scene Filter (%1)").arg(f));
                    });
                }
            }

            ui->gcDownScn->setDisabled(
                c.parent().isValid()
                    ? c.row() == gameConfig.categories[c.parent().row()].scenes.count() - 1
                    : c.row() == gameConfig.categories.count() - 1);
            ui->gcUpScn->setDisabled(c.row() == 0);
        });

    ui->stackedWidget->setCurrentIndex(type);
    this->oldVer = oldVer;
    if (configPath != "") {
        if (type == 0) {
            gameConfig.read(configPath, oldVer);
            if (QFile::exists(QFileInfo(configPath).absolutePath() + "/RSDKConfig.bin"))
                rsdkConfig.read(QFileInfo(configPath).absolutePath() + "/RSDKConfig.bin");
            else
                rsdkConfig = RSDKv5::RSDKConfig();
            stageConfig = RSDKv5::StageConfig();

            tabTitle = Utils::getFilenameAndFolder(gameConfig.filePath);
            tabPath  = configPath;
            ClearActions();

            appConfig.addRecentFile(ENGINE_v5, TOOL_GAMECONFIGEDITOR, configPath,
                                    QList<QString>{
                                        "GameConfig",
                                        oldVer ? "rev01" : "rev02",
                                    });
            SetStatus("Loaded GameConfig " + tabTitle);
        }
        else {
            stageConfig.read(configPath);
            gameConfig = RSDKv5::GameConfig();
            rsdkConfig = RSDKv5::RSDKConfig();

            tabTitle = Utils::getFilenameAndFolder(stageConfig.filePath);
            tabPath  = configPath;
            ClearActions();

            appConfig.addRecentFile(ENGINE_v5, TOOL_GAMECONFIGEDITOR, configPath,
                                    QList<QString>{ "StageConfig", "rev02" });
        }
        setupUI();
    } else {
        gameConfig  = RSDKv5::GameConfig();
        rsdkConfig  = RSDKv5::RSDKConfig();
        stageConfig = RSDKv5::StageConfig();
        tabTitle    = "GameConfig Editor";
        tabPath     = "";
        ui->stackedWidget->setCurrentIndex(0);
        setupUI();
    }
}

GameConfigEditorv5::~GameConfigEditorv5() { delete ui; }

void GameConfigEditorv5::setupUI(bool allowRowChange)
{
    auto setupInitScenes = [this] {
        ui->gcStartingScene->clear();

        if (ui->gcStartingCategory->currentIndex() >= 0) {
            for (int i = 0;
                 i < gameConfig.categories[ui->gcStartingCategory->currentIndex()].scenes.count();
                 ++i) {
                ui->gcStartingScene->addItem(
                    gameConfig.categories[ui->gcStartingCategory->currentIndex()].scenes[i].name);
            }
        }
    };

    auto setupInitCategories = [this, setupInitScenes] {
        ui->gcStartingCategory->clear();
        for (int i = 0; i < gameConfig.categories.count(); ++i)
            ui->gcStartingCategory->addItem(gameConfig.categories[i].name);

        setupInitScenes();
    };

    if (allowRowChange) {
        ui->gcSectionList->blockSignals(true);
        ui->gcSectionList->setCurrentRow(-1);
        ui->gcSectionList->blockSignals(false);
        ui->gcSectionList->setCurrentRow(0);

        ui->scSectionList->blockSignals(true);
        ui->scSectionList->setCurrentRow(-1);
        ui->scSectionList->blockSignals(false);
        ui->scSectionList->setCurrentRow(0);
    }

    // Disconnect
    disconnect(ui->gcGameName, nullptr, nullptr, nullptr);
    disconnect(ui->gcGameSubname, nullptr, nullptr, nullptr);
    disconnect(ui->gcGameVersion, nullptr, nullptr, nullptr);
    disconnect(ui->gcStartingCategory, nullptr, nullptr, nullptr);
    disconnect(ui->gcStartingScene, nullptr, nullptr, nullptr);

    disconnect(ui->gcObjList, nullptr, nullptr, nullptr);
    disconnect(ui->gcObjName, nullptr, nullptr, nullptr);
    disconnect(ui->gcUpObj, nullptr, nullptr, nullptr);
    disconnect(ui->gcDownObj, nullptr, nullptr, nullptr);
    disconnect(ui->gcAddObj, nullptr, nullptr, nullptr);
    disconnect(ui->gcRmObj, nullptr, nullptr, nullptr);

    disconnect(ui->gcSfxList, nullptr, nullptr, nullptr);
    disconnect(ui->gcSfxName, nullptr, nullptr, nullptr);
    disconnect(ui->gcMaxConcurrentPlays, nullptr, nullptr, nullptr);
    disconnect(ui->gcUpSfx, nullptr, nullptr, nullptr);
    disconnect(ui->gcDownSfx, nullptr, nullptr, nullptr);
    disconnect(ui->gcAddSfx, nullptr, nullptr, nullptr);
    disconnect(ui->gcRmSfx, nullptr, nullptr, nullptr);

    disconnect(ui->gcScnTree, nullptr, nullptr, nullptr);
    disconnect(ui->gcScnName, nullptr, nullptr, nullptr);
    disconnect(ui->gcScnFolder, nullptr, nullptr, nullptr);
    disconnect(ui->gcScnID, nullptr, nullptr, nullptr);

    QCheckBox *filterToggles[] = { ui->gcScnFilter1, ui->gcScnFilter2, ui->gcScnFilter3,
                                   ui->gcScnFilter4, ui->gcScnFilter5, ui->gcScnFilter6,
                                   ui->gcScnFilter7, ui->gcScnFilter8 };

    disconnect(ui->gcScnFilterValue, nullptr, nullptr, nullptr);
    for (int f = 0; f < 8; ++f) disconnect(filterToggles[f], nullptr, nullptr, nullptr);

    disconnect(ui->gcVarList, nullptr, nullptr, nullptr);
    disconnect(ui->gcVarValue, nullptr, nullptr, nullptr);

    disconnect(ui->scLoadGlobals, nullptr, nullptr, nullptr);

    disconnect(ui->scObjList, nullptr, nullptr, nullptr);
    disconnect(ui->scObjName, nullptr, nullptr, nullptr);
    disconnect(ui->scUpObj, nullptr, nullptr, nullptr);
    disconnect(ui->scDownObj, nullptr, nullptr, nullptr);
    disconnect(ui->scAddObj, nullptr, nullptr, nullptr);
    disconnect(ui->scRmObj, nullptr, nullptr, nullptr);

    disconnect(ui->scSfxList, nullptr, nullptr, nullptr);
    disconnect(ui->scSfxName, nullptr, nullptr, nullptr);
    disconnect(ui->scMaxConcurrentPlays, nullptr, nullptr, nullptr);
    disconnect(ui->scUpSfx, nullptr, nullptr, nullptr);
    disconnect(ui->scDownSfx, nullptr, nullptr, nullptr);
    disconnect(ui->scAddSfx, nullptr, nullptr, nullptr);
    disconnect(ui->scRmSfx, nullptr, nullptr, nullptr);

    // Set Values
    ui->gcGameName->setText(gameConfig.gameTitle);
    ui->gcGameSubname->setText(gameConfig.gameSubtitle);
    ui->gcGameVersion->setText(gameConfig.version);

    setupInitCategories();
    ui->gcStartingCategory->setCurrentIndex(gameConfig.startSceneCategoryIndex);
    ui->gcStartingScene->setCurrentIndex(gameConfig.startSceneIndex);

    int prevIndex = ui->gcObjList->currentRow();
    ui->gcObjList->clear();
    int id = 0;
    for (auto &o : gameConfig.objects) {
        ui->gcObjList->addItem(o);
        ui->gcObjList->item(id)->setFlags(ui->gcObjList->item(id)->flags() | Qt::ItemIsEditable);
        id++;
    };
    ui->gcObjList->setCurrentRow(0x00);
    ui->gcObjName->setText("");
    ui->gcObjList->setCurrentRow(prevIndex >= ui->gcObjList->count() ? -1 : prevIndex);

    prevIndex = ui->gcSfxList->currentRow();
    ui->gcSfxList->clear();
    id = 0;
    for (auto &s : gameConfig.soundFX) {
        ui->gcSfxList->addItem(s.path);
        ui->gcSfxList->item(id)->setFlags(ui->gcSfxList->item(id)->flags() | Qt::ItemIsEditable);
        id++;
    };
    ui->gcSfxList->setCurrentRow(0x00);
    ui->gcSfxName->setText("");
    ui->gcMaxConcurrentPlays->setValue(0x00);
    ui->gcSfxList->setCurrentRow(prevIndex >= ui->gcSfxList->count() ? -1 : prevIndex);

    sceneModel->clear();
    for (int c = 0; c < gameConfig.categories.count(); ++c) {
        auto *catItem = new QStandardItem(gameConfig.categories[c].name);

        for (auto &scn : gameConfig.categories[c].scenes) {
            auto *scnItem = new QStandardItem(scn.name);
            catItem->appendRow(scnItem);

            scnItem->setFlags(scnItem->flags() | Qt::ItemIsEditable);
        }

        catItem->setFlags(catItem->flags() | Qt::ItemIsEditable);
        sceneModel->appendRow(catItem);
    }
    ui->gcScnName->setText("");
    ui->gcScnFolder->setText("");
    ui->gcScnID->setText("");

    ui->gcScnFilterValue->setText("Filter Value: 0");
    for (int f = 0; f < 8; ++f) filterToggles[f]->setChecked(false);

    prevIndex = ui->gcVarList->currentRow();
    ui->gcVarList->clear();
    if (rsdkConfig.variables.count() > 0 && gameConfig.globalVariables.count() > 0) {
        for (auto &v : rsdkConfig.variables) {
            ui->gcVarList->addItem(v.name);
        }
    }
    else {
        for (int v = 0; v < gameConfig.globalVariables.count(); ++v)
            ui->gcVarList->addItem("Unnamed Variable " + QString::number(v));
    }
    ui->gcVarList->setCurrentRow(0x00);
    ui->gcVarValue->setValue(0x00);
    ui->gcVarList->setCurrentRow(prevIndex >= ui->gcVarList->count() ? -1 : prevIndex);

    ui->scLoadGlobals->setChecked(stageConfig.loadGlobalObjects);

    prevIndex = ui->scObjList->currentRow();
    ui->scObjList->setCurrentRow(0x00);
    ui->scObjList->clear();
    for (auto &o : stageConfig.objects) ui->scObjList->addItem(o);
    ui->scObjName->setText("");
    ui->scObjList->setCurrentRow(prevIndex >= ui->scObjList->count() ? -1 : prevIndex);

    prevIndex = ui->scSfxList->currentRow();
    ui->scSfxList->setCurrentRow(0x00);
    ui->scSfxList->clear();
    for (auto &s : stageConfig.soundFX) ui->scSfxList->addItem(s.path);
    ui->scSfxName->setText("");
    ui->scMaxConcurrentPlays->setValue(0x00);
    ui->scSfxList->setCurrentRow(prevIndex >= ui->scSfxList->count() ? -1 : prevIndex);

    // ----------------
    // GENERAL
    // ----------------
    connect(ui->gcGameName, &QLineEdit::textChanged, [this](QString s) {
        gameConfig.gameTitle = s;
        DoAction("Changed Game Title");
    });
    connect(ui->gcGameSubname, &QLineEdit::textChanged, [this](QString s) {
        gameConfig.gameSubtitle = s;
        DoAction("Changed Game SubTitle");
    });
    connect(ui->gcGameVersion, &QLineEdit::textChanged, [this](QString s) {
        gameConfig.version = s;
        DoAction("Changed Game Version");
    });
    connect(ui->gcStartingCategory, QOverload<int>::of(&QComboBox::currentIndexChanged),
            [this, setupInitScenes](int i) {
                gameConfig.startSceneCategoryIndex = i;
                DoAction("Changed Start Category");
                setupInitScenes();
            });
    connect(ui->gcStartingScene, QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int i) {
        gameConfig.startSceneIndex = i;
        DoAction("Changed Start Scene");
    });

    connect(ui->editPalette, &QPushButton::clicked, [this] {
        RSDKv5::Palette *configPal = nullptr;
        RSDKv5::Palette *editPal   = nullptr;

        PaletteEditor *edit = new PaletteEditor(gameConfig.filePath, (oldVer ? PALTYPE_GAMECONFIGv5_rev01 : PALTYPE_GAMECONFIGv5), true);
        edit->palette.clear();
        for (int b = 0; b < 8; ++b){
            configPal = &gameConfig.palettes[b];
            for (int r = 0; r < 16; ++r){
                edit->gameConfigv5.palettes[b].activeRows[r] = configPal->activeRows[r];
                for (int c = 0; c < 16; ++c){
                    edit->palette.append(QColor(configPal->colors[r][c]));
                    edit->gameConfigv5.palettes[b].colors[r][c] = QColor(configPal->colors[r][c]);
                }
            }
        }
        edit->setWindowTitle("Edit GameConfig Palette");
        edit->exec();

        for(int b = 0; b < 8; ++b){
            editPal = &edit->gameConfigv5.palettes[b];
            configPal = &gameConfig.palettes[b];

            for (int r = 0; r < 16; ++r) {
                configPal->activeRows[r] = editPal->activeRows[r];
                if (editPal->activeRows[r]) {
                    for (int c = 0; c < 16; ++c) {
                        if (b == edit->bankID)
                            configPal->colors[r][c] =
                                QColor(edit->palette[(r << 4) + c].r, edit->palette[(r << 4) + c].g,
                                        edit->palette[(r << 4) + c].b);
                        else
                            configPal->colors[r][c] = editPal->colors[r][c];
                    }
                }
                else {
                    for (int c = 0; c < 16; ++c)
                        configPal->colors[r][c] = QColor(0xFF, 0x00, 0xFF);
                }
            }
        }
        delete edit;
        DoAction("Edited Palette");
    });

    // ----------------
    // OBJECTS
    // ----------------
    connect(ui->gcObjList, &QListWidget::currentRowChanged, [this](int c) {
        if (ui->gcUpObj)
            ui->gcUpObj->setDisabled(c == -1);
        if (ui->gcDownObj)
            ui->gcDownObj->setDisabled(c == -1);
        if (ui->gcRmObj)
            ui->gcRmObj->setDisabled(c == -1);

        ui->gcObjName->setDisabled(c == -1);

        if (c == -1)
            return;

        ui->gcObjName->blockSignals(true);

        ui->gcObjName->setText(gameConfig.objects[c]);
        ui->labelObjID->setText(QString("Object ID: %1").arg(c));

        ui->gcObjName->blockSignals(false);

        if (ui->gcDownObj)
            ui->gcDownObj->setDisabled(c == ui->gcObjList->count() - 1);
        if (ui->gcUpObj)
            ui->gcUpObj->setDisabled(c == 0);
    });

    connect(ui->gcAddObj, &QToolButton::clicked, [this] {
        uint c = ui->gcObjList->currentRow() + 1;
        gameConfig.objects.insert(c, "Object");
        auto *item = new QListWidgetItem();
        item->setText(gameConfig.objects[c]);
        ui->gcObjList->insertItem(c, item);

        item->setFlags(item->flags() | Qt::ItemIsEditable);
        ui->gcObjList->setCurrentItem(item);
        DoAction("Added Object");
    });

    connect(ui->gcUpObj, &QToolButton::clicked, [this] {
        uint c     = ui->gcObjList->currentRow();
        auto *item = ui->gcObjList->takeItem(c);
        gameConfig.objects.move(c, c - 1);
        ui->gcObjList->insertItem(c - 1, item);
        ui->gcObjList->setCurrentRow(c - 1);
        DoAction("Moved Object Up");
    });

    connect(ui->gcDownObj, &QToolButton::clicked, [this] {
        uint c     = ui->gcObjList->currentRow();
        auto *item = ui->gcObjList->takeItem(c);
        gameConfig.objects.move(c, c + 1);
        ui->gcObjList->insertItem(c + 1, item);
        ui->gcObjList->setCurrentRow(c + 1);
        DoAction("Moved Object Down");
    });

    connect(ui->gcRmObj, &QToolButton::clicked, [this] {
        int c = ui->gcObjList->currentRow();
        int n = ui->gcObjList->currentRow() == ui->gcObjList->count() - 1 ? c - 1 : c;
        delete ui->gcObjList->item(c);
        gameConfig.objects.removeAt(c);
        ui->gcObjList->blockSignals(true);
        ui->gcObjList->setCurrentRow(n);
        ui->gcObjList->blockSignals(false);
        DoAction("Removed Object");
    });

    connect(ui->gcObjList, &QListWidget::itemChanged, [this](QListWidgetItem *item) {
        gameConfig.objects[ui->gcObjList->row(item)] = item->text();

        ui->gcObjName->blockSignals(true);
        ui->gcObjName->setText(gameConfig.objects[ui->gcObjList->row(item)]);
        ui->gcObjName->blockSignals(false);
        DoAction("Changed Object Name");
    });

    connect(ui->gcObjName, &QLineEdit::textChanged, [this](QString s) {
        gameConfig.objects[ui->gcObjList->currentRow()] = s;

        ui->gcObjList->item(ui->gcObjList->currentRow())
            ->setText(gameConfig.objects[ui->gcObjList->currentRow()]);
        DoAction("Changed Object Name");
    });

    // ----------------
    // SOUND FX
    // ----------------
    connect(ui->gcSfxList, &QListWidget::currentRowChanged, [this](int c) {
        if (ui->gcUpSfx)
            ui->gcUpSfx->setDisabled(c == -1);
        if (ui->gcDownSfx)
            ui->gcDownSfx->setDisabled(c == -1);
        if (ui->gcRmSfx)
            ui->gcRmSfx->setDisabled(c == -1);

        ui->gcSfxName->setDisabled(c == -1);

        if (c == -1)
            return;

        ui->gcSfxName->blockSignals(true);
        ui->gcMaxConcurrentPlays->blockSignals(true);

        ui->gcSfxName->setText(gameConfig.soundFX[c].path);
        ui->gcMaxConcurrentPlays->setValue(gameConfig.soundFX[c].maxConcurrentPlay);

        ui->gcSfxName->blockSignals(false);
        ui->gcMaxConcurrentPlays->blockSignals(false);

        if (ui->gcDownSfx)
            ui->gcDownSfx->setDisabled(c == ui->gcSfxList->count() - 1);
        if (ui->gcUpSfx)
            ui->gcUpSfx->setDisabled(c == 0);
    });

    connect(ui->gcAddSfx, &QToolButton::clicked, [this] {
        uint c = ui->gcSfxList->currentRow() + 1;
        gameConfig.soundFX.insert(c, RSDKv5::GameConfig::SoundInfo());
        auto *item = new QListWidgetItem();
        item->setText(gameConfig.soundFX[c].path);
        ui->gcSfxList->insertItem(c, item);

        item->setFlags(item->flags() | Qt::ItemIsEditable);
        ui->gcSfxList->setCurrentItem(item);
        DoAction("Added SoundFX");
    });

    connect(ui->gcUpSfx, &QToolButton::clicked, [this] {
        uint c     = ui->gcSfxList->currentRow();
        auto *item = ui->gcSfxList->takeItem(c);
        gameConfig.soundFX.move(c, c - 1);
        ui->gcSfxList->insertItem(c - 1, item);
        ui->gcSfxList->setCurrentRow(c - 1);
        DoAction("Moved SoundFX Up");
    });

    connect(ui->gcDownSfx, &QToolButton::clicked, [this] {
        uint c     = ui->gcSfxList->currentRow();
        auto *item = ui->gcSfxList->takeItem(c);
        gameConfig.soundFX.move(c, c + 1);
        ui->gcSfxList->insertItem(c + 1, item);
        ui->gcSfxList->setCurrentRow(c + 1);
        DoAction("Moved SoundFX Down");
    });

    connect(ui->gcRmSfx, &QToolButton::clicked, [this] {
        int c = ui->gcSfxList->currentRow();
        int n = ui->gcSfxList->currentRow() == ui->gcSfxList->count() - 1 ? c - 1 : c;
        delete ui->gcSfxList->item(c);
        gameConfig.soundFX.removeAt(c);
        ui->gcSfxList->blockSignals(true);
        ui->gcSfxList->setCurrentRow(n);
        ui->gcSfxList->blockSignals(false);
        DoAction("Removed SoundFX");
    });

    connect(ui->gcSfxList, &QListWidget::itemChanged, [this](QListWidgetItem *item) {
        gameConfig.soundFX[ui->gcSfxList->row(item)].path = item->text();

        ui->gcSfxName->blockSignals(true);
        ui->gcSfxName->setText(gameConfig.soundFX[ui->gcSfxList->row(item)].path);
        ui->gcSfxName->blockSignals(false);
        DoAction("Changed SoundFX Path");
    });

    connect(ui->gcSfxName, &QLineEdit::textChanged, [this](QString s) {
        gameConfig.soundFX[ui->gcSfxList->currentRow()].path = s;

        ui->gcSfxList->item(ui->gcSfxList->currentRow())
            ->setText(gameConfig.soundFX[ui->gcSfxList->currentRow()].path);
        DoAction("Changed SoundFX Path");
    });

    connect(ui->gcMaxConcurrentPlays, QOverload<int>::of(&QSpinBox::valueChanged), [this](int v) {
        gameConfig.soundFX[ui->gcSfxList->currentRow()].maxConcurrentPlay = v;
        DoAction("Changed SoundFX Plays");
    });

    // ------------------
    // GLOBAL VARIABLES
    // ------------------
    connect(ui->gcVarList, &QListWidget::currentRowChanged, [this](int c) {
        ui->gcVarName->setDisabled(c == -1);
        ui->gcVarCount->setDisabled(c == -1);
        ui->gcVarIndex->setDisabled(c == -1);
        ui->gcVarValue->setDisabled(c == -1);

        if (c == -1)
            return;

        ui->gcVarName->blockSignals(true);
        ui->gcVarCount->blockSignals(true);
        ui->gcVarIndex->blockSignals(true);
        ui->gcVarValue->blockSignals(true);

        if (c < rsdkConfig.variables.count())
            ui->gcVarName->setText(rsdkConfig.variables[c].name);
        else
            ui->gcVarName->setText("Unnamed Variable " + QString::number(c));
        ui->gcVarCount->setValue(gameConfig.globalVariables[c].values.count());
        ui->gcVarIndex->setValue(0);
        if (gameConfig.globalVariables[c].values.count() > 0)
            ui->gcVarValue->setValue(gameConfig.globalVariables[c].values[0]);
        else
            ui->gcVarValue->setValue(0);

        ui->gcVarName->blockSignals(false);
        ui->gcVarCount->blockSignals(false);
        if (gameConfig.globalVariables[c].values.count() > 0)
            ui->gcVarIndex->blockSignals(false);
        if (gameConfig.globalVariables[c].values.count() > 0)
            ui->gcVarValue->blockSignals(false);
    });
    connect(ui->gcVarIndex, QOverload<int>::of(&QSpinBox::valueChanged), [this](int v) {
        if (gameConfig.globalVariables[ui->gcVarList->currentRow()].values.count() == 0)
            ui->gcVarIndex->setValue(0);
        else if (v >= gameConfig.globalVariables[ui->gcVarList->currentRow()].values.count())
            ui->gcVarIndex->setValue(
                gameConfig.globalVariables[ui->gcVarList->currentRow()].values.count() - 1);

        if (gameConfig.globalVariables[ui->gcVarList->currentRow()].values.count() > 0) {
            ui->gcVarValue->blockSignals(true);
            ui->gcVarValue->setValue(gameConfig.globalVariables[ui->gcVarList->currentRow()]
                                         .values[ui->gcVarIndex->value()]);
            ui->gcVarValue->blockSignals(false);
        }
    });
    connect(ui->gcVarValue, QOverload<int>::of(&QSpinBox::valueChanged), [this](int v) {
        if (gameConfig.globalVariables[ui->gcVarList->currentRow()].values.count() > 0) {
            gameConfig.globalVariables[ui->gcVarList->currentRow()].values[ui->gcVarIndex->value()] = v;
            DoAction("Changed Variable Value");
        }
        else
            ui->gcVarValue->setValue(0);
    });

    connect(ui->exportGlobals, &QPushButton::pressed, [this] {
        QFileDialog filedialog(
            this, tr("Save Globals"), "",
            tr(QString("RSDKv5 Global Variables (GlobalVars.bin)").toStdString().c_str()));
        filedialog.setAcceptMode(QFileDialog::AcceptSave);
        if (filedialog.exec() == QDialog::Accepted) {
            QString selFile = filedialog.selectedFiles()[0];

            Writer writer(selFile);

            writer.write((byte)gameConfig.globalVariables.count());
            for (RSDKv5::GameConfig::GlobalVariable &c : gameConfig.globalVariables) c.write(writer);

            writer.flush();
        }
    });

    connect(ui->genCStruct, &QPushButton::pressed, [this] {
        QFileDialog filedialog(this, tr("Save File"), "",
                               tr(QString("C Headers (*.h)").toStdString().c_str()));
        filedialog.setAcceptMode(QFileDialog::AcceptSave);
        if (filedialog.exec() == QDialog::Accepted) {
            QString selFile = filedialog.selectedFiles()[0];

            Writer writer(selFile);
            writer.writeLine("typedef struct {");

            for (auto &var : rsdkConfig.variables) {
                writer.writeLine(QString("\t%1 %2;").arg(var.type).arg(var.name));
            }

            writer.writeLine("} GlobalVariables;");

            writer.flush();
        }
    });

    // ----------------
    // GENERAL
    // ----------------
    connect(ui->scLoadGlobals, &QCheckBox::clicked, [this](int c) {
        stageConfig.loadGlobalObjects = c;
        DoAction("Changed Load Globals");
    });

    // ----------------
    // OBJECTS
    // ----------------
    connect(ui->scObjList, &QListWidget::currentRowChanged, [this](int c) {
        if (ui->scUpObj)
            ui->scUpObj->setDisabled(c == -1);
        if (ui->scDownObj)
            ui->scDownObj->setDisabled(c == -1);
        if (ui->scRmObj)
            ui->scRmObj->setDisabled(c == -1);

        ui->scObjName->setDisabled(c == -1);

        if (c == -1)
            return;

        ui->scObjName->blockSignals(true);

        ui->scObjName->setText(stageConfig.objects[c]);

        ui->scObjName->blockSignals(false);

        if (ui->scDownObj)
            ui->scDownObj->setDisabled(c == ui->scObjList->count() - 1);
        if (ui->scUpObj)
            ui->scUpObj->setDisabled(c == 0);
    });

    connect(ui->scAddObj, &QToolButton::clicked, [this] {
        uint c = ui->scObjList->currentRow() + 1;
        stageConfig.objects.insert(c, "Object");
        auto *item = new QListWidgetItem();
        item->setText(stageConfig.objects[c]);
        ui->scObjList->insertItem(c, item);

        item->setFlags(item->flags() | Qt::ItemIsEditable);
        ui->scObjList->setCurrentItem(item);
        DoAction("Added Object");
    });

    connect(ui->scUpObj, &QToolButton::clicked, [this] {
        uint c     = ui->scObjList->currentRow();
        auto *item = ui->scObjList->takeItem(c);
        stageConfig.objects.move(c, c - 1);
        ui->scObjList->insertItem(c - 1, item);
        ui->scObjList->setCurrentRow(c - 1);
        DoAction("Moved Object Up");
    });

    connect(ui->scDownObj, &QToolButton::clicked, [this] {
        uint c     = ui->scObjList->currentRow();
        auto *item = ui->scObjList->takeItem(c);
        stageConfig.objects.move(c, c + 1);
        ui->scObjList->insertItem(c + 1, item);
        ui->scObjList->setCurrentRow(c + 1);
        DoAction("Moved Object Down");
    });

    connect(ui->scRmObj, &QToolButton::clicked, [this] {
        int c = ui->scObjList->currentRow();
        int n = ui->scObjList->currentRow() == ui->scObjList->count() - 1 ? c - 1 : c;
        delete ui->scObjList->item(c);
        stageConfig.objects.removeAt(c);
        ui->scObjList->blockSignals(true);
        ui->scObjList->setCurrentRow(n);
        ui->scObjList->blockSignals(false);
        DoAction("Removed Object");
    });
    connect(ui->scObjName, &QLineEdit::textChanged, [this](QString s) {
        stageConfig.objects[ui->scObjList->currentRow()] = s;

        ui->scObjList->item(ui->scObjList->currentRow())
            ->setText(stageConfig.objects[ui->scObjList->currentRow()]);
        DoAction("Changed Object Name");
    });

    // ----------------
    // SOUND FX
    // ----------------
    connect(ui->scSfxList, &QListWidget::currentRowChanged, [this](int c) {
        if (ui->scUpSfx)
            ui->scUpSfx->setDisabled(c == -1);
        if (ui->scDownSfx)
            ui->scDownSfx->setDisabled(c == -1);
        if (ui->scRmSfx)
            ui->scRmSfx->setDisabled(c == -1);

        ui->scSfxName->setDisabled(c == -1);

        if (c == -1)
            return;

        ui->scSfxName->blockSignals(true);
        ui->scMaxConcurrentPlays->blockSignals(true);

        ui->scSfxName->setText(stageConfig.soundFX[c].path);
        ui->scMaxConcurrentPlays->setValue(stageConfig.soundFX[c].maxConcurrentPlay);

        ui->scSfxName->blockSignals(false);
        ui->scMaxConcurrentPlays->blockSignals(false);

        if (ui->scDownSfx)
            ui->scDownSfx->setDisabled(c == ui->scSfxList->count() - 1);
        if (ui->scUpSfx)
            ui->scUpSfx->setDisabled(c == 0);
    });

    connect(ui->scAddSfx, &QToolButton::clicked, [this] {
        uint c = ui->scSfxList->currentRow() + 1;
        stageConfig.soundFX.insert(c, RSDKv5::StageConfig::WAVConfiguration());
        auto *item = new QListWidgetItem();
        item->setText(stageConfig.soundFX[c].path);
        ui->scSfxList->insertItem(c, item);

        item->setFlags(item->flags() | Qt::ItemIsEditable);
        ui->scSfxList->setCurrentItem(item);
        DoAction("Added SoundFX");
    });

    connect(ui->scUpSfx, &QToolButton::clicked, [this] {
        uint c     = ui->scSfxList->currentRow();
        auto *item = ui->scSfxList->takeItem(c);
        stageConfig.soundFX.move(c, c - 1);
        ui->scSfxList->insertItem(c - 1, item);
        ui->scSfxList->setCurrentRow(c - 1);
        DoAction("Moved SoundFX Up");
    });

    connect(ui->scDownSfx, &QToolButton::clicked, [this] {
        uint c     = ui->scSfxList->currentRow();
        auto *item = ui->scSfxList->takeItem(c);
        stageConfig.soundFX.move(c, c + 1);
        ui->scSfxList->insertItem(c + 1, item);
        ui->scSfxList->setCurrentRow(c + 1);
        DoAction("Moved SoundFX Down");
    });

    connect(ui->scRmSfx, &QToolButton::clicked, [this] {
        int c = ui->scSfxList->currentRow();
        int n = ui->scSfxList->currentRow() == ui->scSfxList->count() - 1 ? c - 1 : c;
        delete ui->scSfxList->item(c);
        stageConfig.soundFX.removeAt(c);
        ui->scSfxList->blockSignals(true);
        ui->scSfxList->setCurrentRow(n);
        ui->scSfxList->blockSignals(false);
        DoAction("Removed SoundFX");
    });
    connect(ui->scSfxName, &QLineEdit::textChanged, [this](QString s) {
        stageConfig.soundFX[ui->scSfxList->currentRow()].path = s;

        ui->scSfxList->item(ui->scSfxList->currentRow())
            ->setText(stageConfig.soundFX[ui->scSfxList->currentRow()].path);
        DoAction("Changed SoundFX Name");
    });
    connect(ui->scMaxConcurrentPlays, QOverload<int>::of(&QSpinBox::valueChanged), [this](int v) {
        stageConfig.soundFX[ui->scSfxList->currentRow()].maxConcurrentPlay = v;
        DoAction("Changed SoundFX Plays");
    });
}

bool GameConfigEditorv5::event(QEvent *event)
{

    switch ((int)event->type()) {
        default: break;
        case RE_EVENT_NEW:
            if (!ui->stackedWidget->currentIndex()) {
                gameConfig  = RSDKv5::GameConfig();
                rsdkConfig  = RSDKv5::RSDKConfig();
                stageConfig = RSDKv5::StageConfig();
                tabTitle    = "GameConfig Editor";
                tabPath     = "";
                ui->stackedWidget->setCurrentIndex(0);
                setupUI();
            }
            else {
                stageConfig = RSDKv5::StageConfig();
                gameConfig  = RSDKv5::GameConfig();
                rsdkConfig  = RSDKv5::RSDKConfig();
                tabTitle    = "StageConfig Editor";
                tabPath     = "";
                ui->stackedWidget->setCurrentIndex(1);
                setupUI();
            }
            ClearActions();
            return true;
        case RE_EVENT_OPEN: {
            QList<QString> filters = { "rev02 (plus) RSDKv5 GameConfig files (*GameConfig*.bin);;rev01 "
                                       "(pre-plus) RSDKv5 GameConfig files (*GameConfig*.bin)" };

            QFileDialog filedialog(
                this, tr("Open RSDKv5 Config"), "",
                tr(filters[ui->stackedWidget->currentIndex()].toStdString().c_str()));
            filedialog.setAcceptMode(QFileDialog::AcceptOpen);
            if (filedialog.exec() == QDialog::Accepted) {
                SetStatus("Loading GameConfig...", true);

                if (filedialog.selectedNameFilter()
                        == "rev02 (plus) RSDKv5 GameConfig files (*GameConfig*.bin)"
                    || filedialog.selectedNameFilter()
                           == "rev01 (pre-plus) RSDKv5 GameConfig files (*GameConfig*.bin)") {
                    ui->stackedWidget->setCurrentIndex(0);

                    oldVer = filedialog.selectedNameFilter()
                             == "rev01 (pre-plus) RSDKv5 GameConfig files (*GameConfig*.bin)";
                    gameConfig.read(filedialog.selectedFiles()[0], oldVer);
                    if (QFile::exists(QFileInfo(filedialog.selectedFiles()[0]).absolutePath()
                                      + "/RSDKConfig.bin")) {
                        rsdkConfig.read(QFileInfo(filedialog.selectedFiles()[0]).absolutePath()
                                        + "/RSDKConfig.bin");
                    }
                    else
                        rsdkConfig = RSDKv5::RSDKConfig();
                    stageConfig = RSDKv5::StageConfig();

                    tabTitle    = Utils::getFilenameAndFolder(gameConfig.filePath);
                    tabPath     = filedialog.selectedFiles()[0];
                    ClearActions();

                    appConfig.addRecentFile(ENGINE_v5, TOOL_GAMECONFIGEDITOR,
                                            filedialog.selectedFiles()[0],
                                            QList<QString>{ "GameConfig", oldVer ? "rev01" : "rev02" });
                    SetStatus("Loaded GameConfig " + tabTitle);
                }
                else {
                    ui->stackedWidget->setCurrentIndex(1);

                    stageConfig.read(filedialog.selectedFiles()[0]);
                    gameConfig = RSDKv5::GameConfig();
                    rsdkConfig = RSDKv5::RSDKConfig();
                    tabTitle   = Utils::getFilenameAndFolder(stageConfig.filePath);
                    tabPath    = filedialog.selectedFiles()[0];
                    ClearActions();

                    appConfig.addRecentFile(ENGINE_v5, TOOL_GAMECONFIGEDITOR,
                                            filedialog.selectedFiles()[0],
                                            QList<QString>{ "StageConfig", "rev02" });
                    SetStatus("Loaded GameConfig " + tabTitle);
                }
                setupUI();
            }
            return true;
        }
        case RE_EVENT_SAVE: {
            if (!ui->stackedWidget->currentIndex()) {
                if (gameConfig.filePath != "") {
                    SetStatus("Saving GameConfig...", true);
                    gameConfig.write("");
                    ClearActions();
                    SetStatus("Saved GameConfig to " + gameConfig.filePath);

                    return true;
                }

                QFileDialog filedialog(
                    this, tr("Save RSDKv5 GameConfig"), "",
                    tr(QString("rev02 (plus) RSDKv5 GameConfig files (*GameConfig*.bin);;rev01 "
                               "(pre-plus) RSDKv5 GameConfig files (*GameConfig*.bin)")
                           .toStdString()
                           .c_str()));
                filedialog.setAcceptMode(QFileDialog::AcceptSave);
                if (filedialog.exec() == QDialog::Accepted) {
                    oldVer = filedialog.selectedNameFilter()
                             == "rev01 (pre-plus) RSDKv5 GameConfig files (*GameConfig*.bin)";
                    gameConfig.readFilter = !oldVer;
                    gameConfig.write(filedialog.selectedFiles()[0]);
                    tabTitle = Utils::getFilenameAndFolder(gameConfig.filePath);
                    tabPath  = filedialog.selectedFiles()[0];
                    ClearActions();
                    appConfig.addRecentFile(ENGINE_v5, TOOL_GAMECONFIGEDITOR,
                                            filedialog.selectedFiles()[0],
                                            QList<QString>{ "GameConfig", oldVer ? "rev01" : "rev02" });
                }
            }
            else {
                if (stageConfig.filePath != "") {
                    SetStatus("Saving StageConfig...", true);
                    stageConfig.write("");
                    ClearActions();
                    SetStatus("Saved StageConfig to " + stageConfig.filePath);

                    return true;
                }

                QFileDialog filedialog(
                    this, tr("Save RSDKv5 StageConfig"), "",
                    tr(QString("RSDKv5 StageConfig files (*StageConfig*.bin)").toStdString().c_str()));
                filedialog.setAcceptMode(QFileDialog::AcceptSave);
                if (filedialog.exec() == QDialog::Accepted) {
                    SetStatus("Saving StageConfig...", true);

                    stageConfig.write(filedialog.selectedFiles()[0]);
                    tabTitle = Utils::getFilenameAndFolder(gameConfig.filePath);
                    tabPath  = filedialog.selectedFiles()[0];
                    ClearActions();
                    appConfig.addRecentFile(ENGINE_v5, TOOL_GAMECONFIGEDITOR,
                                            filedialog.selectedFiles()[0],
                                            QList<QString>{ "StageConfig", "rev02" });
                    SetStatus("Saved StageConfig to " + filedialog.selectedFiles()[0]);
                }
            }
            break;
        }
        case RE_EVENT_SAVE_AS:
            if (!ui->stackedWidget->currentIndex()) {
                QList<QString> types = {
                    "RSDKv5 rev02 (plus) GameConfig files (GameConfig*.bin)",
                    "RSDKv5 rev01 (pre-plus) GameConfig files (GameConfig*.bin)",
                    "RSDKv2 GameConfig files (GameConfig*.bin)",
                    "RSDKv3 GameConfig files (GameConfig*.bin)",
                    "RSDKv4 GameConfig files (GameConfig*.bin)",
                    "RSDK Game XMLs (Game*.xml)",
                };
                QFileDialog filedialog(this, tr("Save GameConfig"), "",
                                       tr(QString("%1;;%2;;%3;;%4;;%5;;%6")
                                              .arg(types[0])
                                              .arg(types[1])
                                              .arg(types[2])
                                              .arg(types[3])
                                              .arg(types[4])
                                              .arg(types[5])
                                              .toStdString()
                                              .c_str()));
                filedialog.setAcceptMode(QFileDialog::AcceptSave);
                if (filedialog.exec() == QDialog::Accepted) {
                    int filter = types.indexOf(filedialog.selectedNameFilter());

                    switch (filter) {
                        case 0:
                        case 1: {
                            SetStatus("Saving GameConfig...", true);

                            gameConfig.readFilter = filter == 3;
                            appConfig.addRecentFile(
                                ENGINE_v5, TOOL_GAMECONFIGEDITOR, filedialog.selectedFiles()[0],
                                QList<QString>{ "GameConfig", filter == 1 ? "rev01" : "rev02" });

                            gameConfig.write(filedialog.selectedFiles()[0]);
                            tabTitle = Utils::getFilenameAndFolder(gameConfig.filePath);
                            SetStatus("Saved GameConfig to " + filedialog.selectedFiles()[0]);

                            break;
                        }
                        case 2: {
                            SetStatus("Converting GameConfig to v2...", true);
                            RSDKv2::GameConfig config;

                            config.gameWindowText      = gameConfig.gameTitle;
                            config.gameDescriptionText = gameConfig.gameSubtitle;

                            config.globalVariables.clear();
                            int varID = 0;
                            for (auto &var : gameConfig.globalVariables) {
                                RSDKv2::GameConfig::GlobalVariable variable;
                                variable.name  = rsdkConfig.variables[varID++].name;
                                variable.value = var.values.count() >= 1 ? var.values[0] : 0;
                                config.globalVariables.append(variable);
                            }
                            AddStatusProgress(1.f / 4);

                            config.objects.clear();
                            for (auto &obj : gameConfig.objects) {
                                config.objects.append(obj + ".txt");
                            }
                            AddStatusProgress(1.f / 4);

                            config.soundFX.clear();
                            for (auto &sfx : gameConfig.soundFX) {
                                config.soundFX.append(sfx.path);
                            }
                            AddStatusProgress(1.f / 4);

                            config.players.clear();

                            config.stageLists.clear();
                            int id = 0;
                            for (auto &cat : gameConfig.categories) {
                                RSDKv2::GameConfig::Category category;
                                category.scenes.clear();

                                for (auto &scn : cat.scenes) {
                                    RSDKv2::GameConfig::SceneInfo scene;
                                    scene.name        = scn.name;
                                    scene.folder      = scn.folder;
                                    scene.id          = scn.id;
                                    scene.highlighted = false;
                                    category.scenes.append(scene);
                                }
                                config.stageLists.append(category);
                                if (++id >= 4)
                                    break;
                            }
                            SetStatus("Saving GameConfig...", true);

                            appConfig.addRecentFile(ENGINE_v2, TOOL_GAMECONFIGEDITOR,
                                                    filedialog.selectedFiles()[0],
                                                    QList<QString>{ /**/ });
                            config.write(filedialog.selectedFiles()[0]);
                            SetStatus("Saved v2 GameConfig to " + filedialog.selectedFiles()[0]);

                            break;
                        }
                        case 3: {
                            SetStatus("Converting GameConfig to v3...", true);
                            RSDKv3::GameConfig config;

                            config.gameWindowText      = gameConfig.gameTitle;
                            config.gameDescriptionText = gameConfig.gameSubtitle;

                            config.globalVariables.clear();
                            int varID = 0;
                            for (auto &var : gameConfig.globalVariables) {
                                RSDKv3::GameConfig::GlobalVariable variable;
                                variable.name  = rsdkConfig.variables[varID++].name;
                                variable.value = var.values.count() >= 1 ? var.values[0] : 0;
                                config.globalVariables.append(variable);
                            }
                            AddStatusProgress(1.f / 4);

                            config.objects.clear();
                            for (auto &obj : gameConfig.objects) {
                                RSDKv3::GameConfig::ObjectInfo object;
                                object.name   = obj;
                                object.script = obj + ".txt";
                                config.objects.append(object);
                            }
                            AddStatusProgress(1.f / 4);

                            config.soundFX.clear();
                            for (auto &sfx : gameConfig.soundFX) {
                                config.soundFX.append(sfx.path);
                            }
                            AddStatusProgress(1.f / 4);

                            config.players.clear();

                            config.stageLists.clear();
                            int id = 0;
                            for (auto &cat : gameConfig.categories) {
                                RSDKv3::GameConfig::Category category;
                                category.scenes.clear();

                                for (auto &scn : cat.scenes) {
                                    RSDKv3::GameConfig::SceneInfo scene;
                                    scene.name        = scn.name;
                                    scene.folder      = scn.folder;
                                    scene.id          = scn.id;
                                    scene.highlighted = false;
                                    category.scenes.append(scene);
                                }
                                config.stageLists.append(category);
                                if (++id >= 4)
                                    break;
                            }

                            SetStatus("Saving GameConfig...", true);

                            appConfig.addRecentFile(ENGINE_v3, TOOL_GAMECONFIGEDITOR,
                                                    filedialog.selectedFiles()[0],
                                                    QList<QString>{ /**/ });
                            config.write(filedialog.selectedFiles()[0]);
                            SetStatus("Saved v3 GameConfig to " + filedialog.selectedFiles()[0]);
                            break;
                        }
                        case 4: {
                            SetStatus("Converting GameConfig to v4...", true);
                            RSDKv4::GameConfig config;

                            config.gameWindowText      = gameConfig.gameTitle;
                            config.gameDescriptionText = gameConfig.gameSubtitle;

                            config.globalVariables.clear();
                            int varID = 0;
                            for (auto &var : gameConfig.globalVariables) {
                                RSDKv4::GameConfig::GlobalVariable variable;
                                variable.name  = rsdkConfig.variables[varID++].name;
                                variable.value = var.values.count() >= 1 ? var.values[0] : 0;
                                config.globalVariables.append(variable);
                            }
                            AddStatusProgress(1.f / 5);

                            config.objects.clear();
                            for (auto &obj : gameConfig.objects) {
                                RSDKv4::GameConfig::ObjectInfo object;
                                object.name   = obj;
                                object.script = obj + ".txt";
                                config.objects.append(object);
                            }
                            AddStatusProgress(1.f / 5);

                            config.soundFX.clear();
                            for (auto &sfx : gameConfig.soundFX) {
                                RSDKv4::GameConfig::SoundInfo soundFX;
                                soundFX.name = QFileInfo(sfx.path).baseName();
                                soundFX.path = sfx.path;
                                config.soundFX.append(soundFX);
                            }
                            AddStatusProgress(1.f / 5);

                            config.players.clear();

                            config.palette.colors.clear();
                            for (int r = 0; r < 6; ++r) {
                                if (gameConfig.palettes[0].activeRows[r]) {
                                    for (int c = 0; c < 16; ++c) {
                                        config.palette.colors.append(
                                            Color(gameConfig.palettes[0].colors[r][c].red(),
                                                  gameConfig.palettes[0].colors[r][c].green(),
                                                  gameConfig.palettes[0].colors[r][c].blue()));
                                    }
                                }
                                else {
                                    for (int c = 0; c < 16; ++c) {
                                        config.palette.colors.append(Color(0x00, 0x00, 0x00));
                                    }
                                }
                            }
                            AddStatusProgress(1.f / 5);

                            config.stageLists.clear();
                            int id = 0;
                            for (auto &cat : gameConfig.categories) {
                                RSDKv4::GameConfig::Category category;
                                category.scenes.clear();

                                for (auto &scn : cat.scenes) {
                                    RSDKv4::GameConfig::SceneInfo scene;
                                    scene.name        = scn.name;
                                    scene.folder      = scn.folder;
                                    scene.id          = scn.id;
                                    scene.highlighted = false;
                                    category.scenes.append(scene);
                                }
                                config.stageLists.append(category);
                                if (++id >= 4)
                                    break;
                            }

                            SetStatus("Saving GameConfig...", true);

                            appConfig.addRecentFile(ENGINE_v4, TOOL_GAMECONFIGEDITOR,
                                                    filedialog.selectedFiles()[0],
                                                    QList<QString>{ /**/ });
                            config.write(filedialog.selectedFiles()[0]);
                            SetStatus("Saved v4 GameConfig to " + filedialog.selectedFiles()[0]);
                            break;
                        }
                        case 5: {
                            SetStatus("Saving Game.xml...", true);
                            Writer writer(filedialog.selectedFiles()[0]);

                            writer.writeLine("<?xml version=\"1.0\"?>");

                            writer.writeLine();
                            writer.writeLine("<game>");
                            if (gameConfig.globalVariables.count()) {
                                /*writer.writeLine();
                                writer.writeLine("\t<variables>");

                                int varID = 0;
                                for (auto &var : gameConfig.globalVariables) {
                                    writer.write(QString("\t<variable name=\"%1\"")
                                                     .arg(rsdkconfig.variables[varID++].name));
                                    for (int v = 0; v < var.values.count(); ++v) {
                                        writer.write(QString("
                                value%1=\"%2\"").arg(v).arg(var.values[v]));
                                    }
                                    writer.writeLine("> </variable>");
                                }

                                writer.writeLine("\t</variables>");*/
                            }
                            AddStatusProgress(1.f / 4);

                            if (gameConfig.objects.count()) {
                                writer.writeLine();
                                writer.writeLine("\t<objects>");
                                for (auto &obj : gameConfig.objects) {
                                    writer.writeLine(
                                        QString("\t\t<object name=\"%1\"> </object>").arg(obj));
                                }
                                writer.writeLine("\t</objects>");
                            }
                            AddStatusProgress(1.f / 4);

                            if (gameConfig.soundFX.count()) {
                                writer.writeLine();
                                writer.writeLine("\t<sounds>");
                                for (auto &sfx : gameConfig.soundFX) {
                                    writer.writeLine(QString("\t\t<soundfx path=\"%1\" "
                                                             "maxConcurrentPlays=\"%2\"> </soundfx>")
                                                         .arg(sfx.path)
                                                         .arg(sfx.maxConcurrentPlay));
                                }
                                writer.writeLine("\t</sounds>");
                            }
                            AddStatusProgress(1.f / 4);

                            for (int i = 0; i < gameConfig.categories.count(); ++i) {
                                if (gameConfig.categories[i].scenes.count()) {
                                    writer.writeLine();
                                    writer.writeLine(QString("\t<category name=\"%1\">")
                                                         .arg(gameConfig.categories[i].name));
                                    for (auto &stg : gameConfig.categories[i].scenes) {
                                        writer.writeLine(QString("\t\t<stage name=\"%1\" folder=\"%2\" "
                                                                 "id=\"%3\" filter=\"%4\"> </stage>")
                                                             .arg(stg.name)
                                                             .arg(stg.folder)
                                                             .arg(stg.id)
                                                             .arg(stg.filter));
                                    }
                                    writer.writeLine("\t</category>");
                                }
                            }
                            writer.writeLine("</game>");

                            writer.flush();
                            SetStatus("Saved Game.xml to " + filedialog.selectedFiles()[0]);

                            break;
                        }
                    }

                    tabPath = filedialog.selectedFiles()[0];
                    ClearActions();
                }
            }
            else {
                QFileDialog filedialog(
                    this, tr("Save RSDKv5 StageConfig"), "",
                    tr(QString("RSDKv5 StageConfig files (*StageConfig*.bin)").toStdString().c_str()));
                filedialog.setAcceptMode(QFileDialog::AcceptSave);
                if (filedialog.exec() == QDialog::Accepted) {
                    SetStatus("Saving StageConfig...", true);
                    stageConfig.write(filedialog.selectedFiles()[0]);
                    SetStatus("Saved StageConfig to " + filedialog.selectedFiles()[0]);
                    tabTitle = Utils::getFilenameAndFolder(gameConfig.filePath);
                    tabPath  = filedialog.selectedFiles()[0];
                    ClearActions();
                    appConfig.addRecentFile(ENGINE_v5, TOOL_GAMECONFIGEDITOR,
                                            filedialog.selectedFiles()[0],
                                            QList<QString>{ "StageConfig", "rev02" });
                }
            }
            break;

        case RE_EVENT_UNDO: UndoAction(); break;
        case RE_EVENT_REDO: RedoAction(); break;

        case QEvent::Close:
            if (modified) {
                bool cancelled = false;
                if (MainWindow::ShowCloseWarning(this, &cancelled)) {
                    if (!ui->stackedWidget->currentIndex()) {
                        if (gameConfig.filePath != "") {
                            SetStatus("Saving GameConfig...", true);
                            gameConfig.write("");
                            SetStatus("Saved GameConfig to " + gameConfig.filePath);

                            ClearActions();
                            return true;
                        }

                        QFileDialog filedialog(
                            this, tr("Save RSDKv5 GameConfig"), "",
                            tr(QString("rev02 (plus) RSDKv5 GameConfig files (*GameConfig*.bin);;rev01 "
                                       "(pre-plus) RSDKv5 GameConfig files (*GameConfig*.bin)")
                                   .toStdString()
                                   .c_str()));
                        filedialog.setAcceptMode(QFileDialog::AcceptSave);
                        if (filedialog.exec() == QDialog::Accepted) {
                            oldVer = filedialog.selectedNameFilter()
                                     == "rev01 (pre-plus) RSDKv5 GameConfig files (*GameConfig*.bin)";
                            gameConfig.readFilter = !oldVer;
                            SetStatus("Saving GameConfig...", true);

                            gameConfig.write(filedialog.selectedFiles()[0]);
                            SetStatus("Saved GameConfig to " + filedialog.selectedFiles()[0]);

                            tabTitle = Utils::getFilenameAndFolder(gameConfig.filePath);
                            ClearActions();
                            appConfig.addRecentFile(
                                ENGINE_v5, TOOL_GAMECONFIGEDITOR, filedialog.selectedFiles()[0],
                                QList<QString>{ "GameConfig", oldVer ? "rev01" : "rev02" });
                        }
                    }
                    else {
                        if (stageConfig.filePath != "") {
                            SetStatus("Saving StageConfig...", true);
                            stageConfig.write("");
                            ClearActions();
                            SetStatus("Saved StageConfig to " + stageConfig.filePath);

                            return true;
                        }

                        QFileDialog filedialog(
                            this, tr("Save RSDKv5 StageConfig"), "",
                            tr(QString("RSDKv5 StageConfig files (*StageConfig*.bin)")
                                   .toStdString()
                                   .c_str()));
                        filedialog.setAcceptMode(QFileDialog::AcceptSave);
                        if (filedialog.exec() == QDialog::Accepted) {
                            SetStatus("Saving StageConfig...", true);

                            stageConfig.write(filedialog.selectedFiles()[0]);
                            tabTitle = Utils::getFilenameAndFolder(gameConfig.filePath);
                            ClearActions();
                            appConfig.addRecentFile(ENGINE_v5, TOOL_GAMECONFIGEDITOR,
                                                    filedialog.selectedFiles()[0],
                                                    QList<QString>{ "StageConfig", "rev02" });
                            SetStatus("Saved StageConfig to " + filedialog.selectedFiles()[0]);
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

void GameConfigEditorv5::UndoAction()
{
    if (actionIndex > 0) {
        // setStatus("Undid Action: " + actions[actionIndex].name);
        actionIndex--;
        ResetAction();
    }
}
void GameConfigEditorv5::RedoAction()
{
    if (actionIndex + 1 < actions.count()) {
        // setStatus("Redid Action: " + actions[actionIndex].name);
        actionIndex++;
        ResetAction();
    }
}
void GameConfigEditorv5::ResetAction()
{
    copyConfig(NULL, &actions[actionIndex]);

    setupUI(false);

    UpdateTitle(actionIndex > 0);
}
void GameConfigEditorv5::DoAction(QString name, bool setModified)
{
    ActionState action;

    action.name = name;

#if RE_USE_UNSTABLE
    copyConfig(&action, NULL);
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
void GameConfigEditorv5::ClearActions()
{
    actions.clear();
    actionIndex = 0;
    DoAction("Action Setup", false); // first action, cant be undone
}

void GameConfigEditorv5::copyConfig(ActionState *stateDst, ActionState *stateSrc)
{
    RSDKv5::GameConfig *src = stateSrc ? &stateSrc->gameConfig : &gameConfig;
    RSDKv5::GameConfig *dst = stateDst ? &stateDst->gameConfig : &gameConfig;

    dst->filePath   = src->filePath;
    dst->readFilter = src->readFilter;

    dst->gameTitle               = src->gameTitle;
    dst->gameSubtitle            = src->gameSubtitle;
    dst->version                 = src->version;
    dst->startSceneCategoryIndex = src->startSceneCategoryIndex;
    dst->startSceneIndex         = src->startSceneIndex;

    for (int p = 0; p < 8; ++p) {
        for (int r = 0; r < 16; ++r) {
            dst->palettes[p].activeRows[r] = src->palettes[p].activeRows[r];
            for (int c = 0; c < 16; ++c) {
                dst->palettes[p].colors[r][c] = src->palettes[p].colors[r][c];
            }
        }
    }

    dst->objects.clear();
    for (auto obj : src->objects) dst->objects.append(obj);

    dst->soundFX.clear();
    for (auto sfx : src->soundFX) dst->soundFX.append(sfx);

    dst->categories.clear();
    for (auto cat : src->categories) {
        RSDKv5::GameConfig::Category category;

        category.name = cat.name;
        category.scenes.clear();
        for (auto scn : cat.scenes) category.scenes.append(scn);
        dst->categories.append(category);
    }

    dst->globalVariables.clear();
    for (auto var : src->globalVariables) dst->globalVariables.append(var);
}

#include "moc_gameconfigeditorv5.cpp"
