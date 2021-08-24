#include "includes.hpp"
#include "ui_gameconfigeditorv5.h"

GameconfigEditorv5::GameconfigEditorv5(QString configPath, byte type, bool oldVer, QWidget *parent)
    : QWidget(parent), ui(new Ui::GameconfigEditorv5)
{
    ui->setupUi(this);

    icon_up   = QIcon(Utils::getColouredIcon(":/icons/ic_arrow_upward_48px.svg"));
    icon_down = QIcon(Utils::getColouredIcon(":/icons/ic_arrow_downward_48px.svg"));
    icon_add  = QIcon(Utils::getColouredIcon(":/icons/ic_add_circle_48px.svg"));
    icon_rm   = QIcon(Utils::getColouredIcon(":/icons/ic_cancel_48px.svg"));

    ui->gcSectionList->blockSignals(true);
    ui->gcSectionList->setCurrentRow(-1);
    ui->gcSectionList->blockSignals(false);
    ui->gcSectionList->setCurrentRow(0);

    ui->scSectionList->blockSignals(true);
    ui->scSectionList->setCurrentRow(-1);
    ui->scSectionList->blockSignals(false);
    ui->scSectionList->setCurrentRow(0);

    ui->gcAddObj->setIcon(icon_add);
    ui->gcUpObj->setIcon(icon_up);
    ui->gcDownObj->setIcon(icon_down);
    ui->gcRmObj->setIcon(icon_rm);

    ui->gcAddSfx->setIcon(icon_add);
    ui->gcUpSfx->setIcon(icon_up);
    ui->gcDownSfx->setIcon(icon_down);
    ui->gcRmSfx->setIcon(icon_rm);

    ui->gcAddScn->setIcon(icon_add);
    ui->gcUpScn->setIcon(icon_up);
    ui->gcDownScn->setIcon(icon_down);
    ui->gcRmScn->setIcon(icon_rm);

    ui->scAddObj->setIcon(icon_add);
    ui->scUpObj->setIcon(icon_up);
    ui->scDownObj->setIcon(icon_down);
    ui->scRmObj->setIcon(icon_rm);

    ui->scAddSfx->setIcon(icon_add);
    ui->scUpSfx->setIcon(icon_up);
    ui->scDownSfx->setIcon(icon_down);
    ui->scRmSfx->setIcon(icon_rm);

    connect(ui->gcSectionList, &QListWidget::currentRowChanged, [this](int v) {
        ui->gcDetailsWidget->setCurrentIndex(v);
        ui->gcSectionLabel->setText(ui->gcSectionList->item(v)->text());
    });

    connect(ui->scSectionList, &QListWidget::currentRowChanged, [this](int v) {
        ui->scDetailsWidget->setCurrentIndex(v);
        ui->scSectionLabel->setText(ui->scSectionList->item(v)->text());
    });

    auto setupInitScenes = [this] {
        ui->gcStartingScene->clear();

        if (ui->gcStartingCategory->currentIndex() >= 0) {
            for (int i = 0;
                 i < gameConfig.sceneCategories[ui->gcStartingCategory->currentIndex()].scenes.count();
                 ++i) {
                ui->gcStartingScene->addItem(
                    gameConfig.sceneCategories[ui->gcStartingCategory->currentIndex()]
                        .scenes[i]
                        .m_name);
            }
        }
    };

    auto setupInitCategories = [this, setupInitScenes] {
        ui->gcStartingCategory->clear();
        for (int i = 0; i < gameConfig.sceneCategories.count(); ++i)
            ui->gcStartingCategory->addItem(gameConfig.sceneCategories[i].m_name);

        setupInitScenes();
    };

    m_sceneModel = new QStandardItemModel(ui->gcScnTree);

    QMenu *scnAddMenu = new QMenu(ui->gcAddScn);

    auto addCat = [this, setupInitCategories] {
        const QModelIndex &index = ui->gcScnTree->currentIndex();
        bool scnSelected         = index.parent().isValid();
        uint c                   = scnSelected ? index.parent().row() + 1 : index.row() + 1;
        gameConfig.sceneCategories.insert(c, RSDKv5::GameConfig::Category());
        auto *catItem = new QStandardItem(gameConfig.sceneCategories[c].m_name);
        auto *scnItem = new QStandardItem(gameConfig.sceneCategories[c].scenes.first().m_name);
        catItem->appendRow(scnItem);
        catItem->setFlags(catItem->flags() | Qt::ItemIsEditable);
        m_sceneModel->insertRow(c, catItem);
        ui->gcScnTree->setCurrentIndex(m_sceneModel->indexFromItem(catItem));

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
        gameConfig.sceneCategories[cat].scenes.insert(scn, RSDKv5::GameConfig::SceneInfo());
        auto *scnItem = new QStandardItem(gameConfig.sceneCategories[cat].scenes[scn].m_name);
        if (scnSelected)
            m_sceneModel->itemFromIndex(index.parent())->insertRow(scn, scnItem);
        else
            m_sceneModel->itemFromIndex(index)->insertRow(scn, scnItem);
        scnItem->setFlags(scnItem->flags() | Qt::ItemIsEditable);
        ui->gcScnTree->setCurrentIndex(m_sceneModel->indexFromItem(scnItem));

        setupInitScenes();
    });

    connect(m_sceneModel, &QStandardItemModel::itemChanged, [this](QStandardItem *item) {
        const QModelIndex &index = m_sceneModel->indexFromItem(item);
        if (index.parent().isValid()) { // Scene
            gameConfig.sceneCategories[index.parent().row()].scenes[index.row()].m_name = item->text();
            return;
        }
        gameConfig.sceneCategories[index.row()].m_name = item->text();
    });

    ui->gcAddScn->setMenu(scnAddMenu);

    connect(ui->gcRmScn, &QToolButton::clicked, [this, setupInitCategories, setupInitScenes] {
        const QModelIndex &index = ui->gcScnTree->currentIndex();

        if (index.parent().isValid()) { // Scene
            gameConfig.sceneCategories[index.parent().row()].scenes.removeAt(index.row());
            m_sceneModel->itemFromIndex(index.parent())->removeRow(index.row());
            setupInitScenes();
            return;
        }
        gameConfig.sceneCategories.removeAt(index.row());
        m_sceneModel->removeRow(index.row());

        setupInitCategories();
    });

    auto moveScn = [this, setupInitCategories, setupInitScenes](char translation) {
        bool isScn = ui->gcScnTree->currentIndex().parent().isValid();
        int c      = ui->gcScnTree->currentIndex().row();
        QList<QStandardItem *> item;
        if (isScn) {
            gameConfig.sceneCategories[ui->gcScnTree->currentIndex().parent().row()].scenes.move(
                c, c + translation);
            QStandardItem *parentItem =
                m_sceneModel->itemFromIndex(ui->gcScnTree->currentIndex().parent());
            item = parentItem->takeRow(c);
            parentItem->insertRow(c + translation, item);
            setupInitScenes();
        }
        else {
            gameConfig.sceneCategories.move(c, c + translation);
            bool expand = ui->gcScnTree->isExpanded(ui->gcScnTree->currentIndex());
            item        = m_sceneModel->takeRow(c), m_sceneModel->insertRow(c + translation, item);
            ui->gcScnTree->setExpanded(m_sceneModel->indexFromItem(item.first()), expand);
            setupInitCategories();
        }
        ui->gcScnTree->setCurrentIndex(m_sceneModel->indexFromItem(item.first()));
    };

    connect(ui->gcUpScn, &QToolButton::clicked, [moveScn] { moveScn(-1); });

    connect(ui->gcDownScn, &QToolButton::clicked, [moveScn] { moveScn(1); });

    ui->gcScnTree->setModel(m_sceneModel);

    connect(ui->gcScnTree->selectionModel(), &QItemSelectionModel::currentChanged,
            [this](const QModelIndex &c) {
                ui->gcUpScn->setDisabled(!c.isValid());
                ui->gcDownScn->setDisabled(!c.isValid());
                ui->gcRmScn->setDisabled(!c.isValid());

                disconnect(ui->gcScnMode, nullptr, nullptr, nullptr);
                disconnect(ui->gcScnID, nullptr, nullptr, nullptr);
                disconnect(ui->gcScnFolder, nullptr, nullptr, nullptr);
                disconnect(ui->gcScnName, nullptr, nullptr, nullptr);

                ui->gcScnID->setDisabled(!c.parent().isValid());
                ui->gcScnFolder->setDisabled(!c.parent().isValid());
                ui->gcScnName->setDisabled(!c.parent().isValid());
                ui->gcScnMode->setDisabled(!c.parent().isValid());

                if (!c.isValid())
                    return;

                if (c.parent().isValid()) {
                    ui->gcScnID->setDisabled(
                        c.row() >= gameConfig.sceneCategories[c.parent().row()].scenes.count());
                    ui->gcScnFolder->setDisabled(
                        c.row() >= gameConfig.sceneCategories[c.parent().row()].scenes.count());
                    ui->gcScnName->setDisabled(
                        c.row() >= gameConfig.sceneCategories[c.parent().row()].scenes.count());
                    ui->gcScnMode->setDisabled(
                        c.row() >= gameConfig.sceneCategories[c.parent().row()].scenes.count());

                    if (c.row() >= gameConfig.sceneCategories[c.parent().row()].scenes.count())
                        return;

                    ui->gcScnID->setText(
                        gameConfig.sceneCategories[c.parent().row()].scenes[c.row()].sceneID);
                    ui->gcScnMode->setValue(
                        gameConfig.sceneCategories[c.parent().row()].scenes[c.row()].modeFilter);
                    ui->gcScnFolder->setText(
                        gameConfig.sceneCategories[c.parent().row()].scenes[c.row()].folder);
                    ui->gcScnName->setText(
                        gameConfig.sceneCategories[c.parent().row()].scenes[c.row()].m_name);

                    connect(ui->gcScnName, &QLineEdit::textEdited, [this, c](QString s) {
                        gameConfig.sceneCategories[c.parent().row()].scenes[c.row()].m_name = s;

                        // TODO: edit text
                    });

                    connect(ui->gcScnFolder, &QLineEdit::textEdited, [this, c](QString s) {
                        gameConfig.sceneCategories[c.parent().row()].scenes[c.row()].folder = s;
                    });

                    connect(ui->gcScnID, &QLineEdit::textEdited, [this, c](QString s) {
                        gameConfig.sceneCategories[c.parent().row()].scenes[c.row()].folder = s;
                    });

                    connect(
                        ui->gcScnMode, QOverload<int>::of(&QSpinBox::valueChanged), [this, c](int v) {
                            gameConfig.sceneCategories[c.parent().row()].scenes[c.row()].modeFilter = v;
                        });
                }

                ui->gcDownScn->setDisabled(
                    c.parent().isValid()
                        ? c.row() == gameConfig.sceneCategories[c.parent().row()].scenes.count() - 1
                        : c.row() == gameConfig.sceneCategories.count() - 1);
                ui->gcUpScn->setDisabled(c.row() == 0);
            });

    ui->stackedWidget->setCurrentIndex(type);
    this->oldVer = oldVer;
    if (configPath != "") {
        if (type == 0) {
            gameConfig.read(configPath, oldVer);
            if (QFile::exists(QFileInfo(configPath).absolutePath() + "/RSDKConfig.bin")) {
                rsdkconfig.read(QFileInfo(configPath).absolutePath() + "/RSDKConfig.bin");
            }
            else
                rsdkconfig = RSDKv5::RSDKConfig();
            stageconfig = RSDKv5::StageConfig();

            appConfig.addRecentFile(ENGINE_v5, TOOL_GAMECONFIGEDITOR, configPath,
                                    QList<QString>{
                                        "GameConfig",
                                        oldVer ? "rev01" : "rev02",
                                    });
        }
        else {
            stageconfig.read(configPath);
            gameConfig = RSDKv5::GameConfig();
            rsdkconfig = RSDKv5::RSDKConfig();
            appConfig.addRecentFile(ENGINE_v5, TOOL_GAMECONFIGEDITOR, configPath,
                                    QList<QString>{ "StageConfig", "rev02" });
        }
        setupUI();
    }
}

GameconfigEditorv5::~GameconfigEditorv5() { delete ui; }

void GameconfigEditorv5::setupUI()
{
    auto setupInitScenes = [this] {
        ui->gcStartingScene->clear();

        if (ui->gcStartingCategory->currentIndex() >= 0) {
            for (int i = 0;
                 i < gameConfig.sceneCategories[ui->gcStartingCategory->currentIndex()].scenes.count();
                 ++i) {
                ui->gcStartingScene->addItem(
                    gameConfig.sceneCategories[ui->gcStartingCategory->currentIndex()]
                        .scenes[i]
                        .m_name);
            }
        }
    };

    auto setupInitCategories = [this, setupInitScenes] {
        ui->gcStartingCategory->clear();
        for (int i = 0; i < gameConfig.sceneCategories.count(); ++i)
            ui->gcStartingCategory->addItem(gameConfig.sceneCategories[i].m_name);

        setupInitScenes();
    };

    ui->gcSectionList->blockSignals(true);
    ui->gcSectionList->setCurrentRow(-1);
    ui->gcSectionList->blockSignals(false);
    ui->gcSectionList->setCurrentRow(0);

    ui->scSectionList->blockSignals(true);
    ui->scSectionList->setCurrentRow(-1);
    ui->scSectionList->blockSignals(false);
    ui->scSectionList->setCurrentRow(0);

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
    disconnect(ui->gcScnMode, nullptr, nullptr, nullptr);

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

    ui->gcObjList->clear();
    for (auto &o : gameConfig.objects) ui->gcObjList->addItem(o);
    ui->gcObjList->setCurrentRow(0x00);
    ui->gcObjName->setText("");

    ui->gcSfxList->clear();
    for (auto &s : gameConfig.soundFX) ui->gcSfxList->addItem(s.m_name);
    ui->gcSfxList->setCurrentRow(0x00);
    ui->gcSfxName->setText("");
    ui->gcMaxConcurrentPlays->setValue(0x00);

    m_sceneModel->clear();
    for (int c = 0; c < gameConfig.sceneCategories.count(); ++c) {
        auto *catItem = new QStandardItem(gameConfig.sceneCategories[c].m_name);

        for (auto &scn : gameConfig.sceneCategories[c].scenes) {
            auto *scnItem = new QStandardItem(scn.m_name);
            catItem->appendRow(scnItem);

            scnItem->setFlags(scnItem->flags() | Qt::ItemIsEditable);
        }

        catItem->setFlags(catItem->flags() | Qt::ItemIsEditable);
        m_sceneModel->appendRow(catItem);
    }
    ui->gcScnName->setText("");
    ui->gcScnFolder->setText("");
    ui->gcScnID->setText("");
    ui->gcScnMode->setValue(0x00);

    ui->gcVarList->clear();
    if (rsdkconfig.variables.count() > 0 && gameConfig.globalVariables.count() > 0) {
        for (auto &v : rsdkconfig.variables) {
            ui->gcVarList->addItem(v.m_name);
        }
    }
    else {
        for (int v = 0; v < gameConfig.globalVariables.count(); ++v)
            ui->gcVarList->addItem("Unnamed Variable " + QString::number(v));
    }
    ui->gcVarList->setCurrentRow(0x00);
    ui->gcVarValue->setValue(0x00);

    ui->scLoadGlobals->setChecked(stageconfig.loadGlobalObjects);

    ui->scObjList->setCurrentRow(0x00);
    ui->scObjList->clear();
    for (auto &o : stageconfig.objects) ui->scObjList->addItem(o);
    ui->scObjName->setText("");

    ui->scSfxList->setCurrentRow(0x00);
    ui->scSfxList->clear();
    for (auto &s : stageconfig.soundFX) ui->scSfxList->addItem(s.path);
    ui->scSfxName->setText("");
    ui->scMaxConcurrentPlays->setValue(0x00);

    // ----------------
    // GENERAL
    // ----------------
    connect(ui->gcGameName, &QLineEdit::textChanged, [this](QString s) { gameConfig.gameTitle = s; });
    connect(ui->gcGameSubname, &QLineEdit::textChanged,
            [this](QString s) { gameConfig.gameSubtitle = s; });
    connect(ui->gcGameVersion, &QLineEdit::textChanged, [this](QString s) { gameConfig.version = s; });
    connect(ui->gcStartingCategory, QOverload<int>::of(&QComboBox::currentIndexChanged),
            [this, setupInitScenes](int i) {
                gameConfig.startSceneCategoryIndex = i;
                setupInitScenes();
            });
    connect(ui->gcStartingScene, QOverload<int>::of(&QComboBox::currentIndexChanged),
            [this](int i) { gameConfig.startSceneIndex = i; });

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
    });

    connect(ui->gcUpObj, &QToolButton::clicked, [this] {
        uint c     = ui->gcObjList->currentRow();
        auto *item = ui->gcObjList->takeItem(c);
        gameConfig.objects.move(c, c - 1);
        ui->gcObjList->insertItem(c - 1, item);
        ui->gcObjList->setCurrentRow(c - 1);
    });

    connect(ui->gcDownObj, &QToolButton::clicked, [this] {
        uint c     = ui->gcObjList->currentRow();
        auto *item = ui->gcObjList->takeItem(c);
        gameConfig.objects.move(c, c + 1);
        ui->gcObjList->insertItem(c + 1, item);
        ui->gcObjList->setCurrentRow(c + 1);
    });

    connect(ui->gcRmObj, &QToolButton::clicked, [this] {
        int c = ui->gcObjList->currentRow();
        int n = ui->gcObjList->currentRow() == ui->gcObjList->count() - 1 ? c - 1 : c;
        delete ui->gcObjList->item(c);
        gameConfig.objects.removeAt(c);
        ui->gcObjList->blockSignals(true);
        ui->gcObjList->setCurrentRow(n);
        ui->gcObjList->blockSignals(false);
    });
    connect(ui->gcObjName, &QLineEdit::textChanged, [this](QString s) {
        gameConfig.objects[ui->gcObjList->currentRow()] = s;

        ui->gcObjList->item(ui->gcObjList->currentRow())
            ->setText(gameConfig.objects[ui->gcObjList->currentRow()]);
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

        ui->gcSfxName->setText(gameConfig.soundFX[c].m_name);
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
        item->setText(gameConfig.soundFX[c].m_name);
        ui->gcSfxList->insertItem(c, item);

        item->setFlags(item->flags() | Qt::ItemIsEditable);
        ui->gcSfxList->setCurrentItem(item);
    });

    connect(ui->gcUpSfx, &QToolButton::clicked, [this] {
        uint c     = ui->gcSfxList->currentRow();
        auto *item = ui->gcSfxList->takeItem(c);
        gameConfig.soundFX.move(c, c - 1);
        ui->gcSfxList->insertItem(c - 1, item);
        ui->gcSfxList->setCurrentRow(c - 1);
    });

    connect(ui->gcDownSfx, &QToolButton::clicked, [this] {
        uint c     = ui->gcSfxList->currentRow();
        auto *item = ui->gcSfxList->takeItem(c);
        gameConfig.soundFX.move(c, c + 1);
        ui->gcSfxList->insertItem(c + 1, item);
        ui->gcSfxList->setCurrentRow(c + 1);
    });

    connect(ui->gcRmSfx, &QToolButton::clicked, [this] {
        int c = ui->gcSfxList->currentRow();
        int n = ui->gcSfxList->currentRow() == ui->gcSfxList->count() - 1 ? c - 1 : c;
        delete ui->gcSfxList->item(c);
        gameConfig.soundFX.removeAt(c);
        ui->gcSfxList->blockSignals(true);
        ui->gcSfxList->setCurrentRow(n);
        ui->gcSfxList->blockSignals(false);
    });
    connect(ui->gcSfxName, &QLineEdit::textChanged, [this](QString s) {
        gameConfig.soundFX[ui->gcSfxList->currentRow()].m_name = s;

        ui->gcSfxList->item(ui->gcSfxList->currentRow())
            ->setText(gameConfig.soundFX[ui->gcSfxList->currentRow()].m_name);
    });
    connect(ui->gcMaxConcurrentPlays, QOverload<int>::of(&QSpinBox::valueChanged),
            [this](int v) { gameConfig.soundFX[ui->gcSfxList->currentRow()].maxConcurrentPlay = v; });

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

        if (c < rsdkconfig.variables.count())
            ui->gcVarName->setText(rsdkconfig.variables[c].m_name);
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
        if (gameConfig.globalVariables[ui->gcVarList->currentRow()].values.count() > 0)
            gameConfig.globalVariables[ui->gcVarList->currentRow()].values[ui->gcVarIndex->value()] = v;
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

            for (auto &var : rsdkconfig.variables) {
                writer.writeLine(QString("\t%1 %2;").arg(var.type).arg(var.m_name));
            }

            writer.writeLine("} GlobalVariables;");

            writer.flush();
        }
    });

    // ----------------
    // GENERAL
    // ----------------
    connect(ui->scLoadGlobals, &QCheckBox::clicked,
            [this](int c) { stageconfig.loadGlobalObjects = c; });

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

        ui->scObjName->setText(stageconfig.objects[c]);

        ui->scObjName->blockSignals(false);

        if (ui->scDownObj)
            ui->scDownObj->setDisabled(c == ui->scObjList->count() - 1);
        if (ui->scUpObj)
            ui->scUpObj->setDisabled(c == 0);
    });

    connect(ui->scAddObj, &QToolButton::clicked, [this] {
        uint c = ui->scObjList->currentRow() + 1;
        stageconfig.objects.insert(c, "Object");
        auto *item = new QListWidgetItem();
        item->setText(stageconfig.objects[c]);
        ui->scObjList->insertItem(c, item);

        item->setFlags(item->flags() | Qt::ItemIsEditable);
        ui->scObjList->setCurrentItem(item);
    });

    connect(ui->scUpObj, &QToolButton::clicked, [this] {
        uint c     = ui->scObjList->currentRow();
        auto *item = ui->scObjList->takeItem(c);
        stageconfig.objects.move(c, c - 1);
        ui->scObjList->insertItem(c - 1, item);
        ui->scObjList->setCurrentRow(c - 1);
    });

    connect(ui->scDownObj, &QToolButton::clicked, [this] {
        uint c     = ui->scObjList->currentRow();
        auto *item = ui->scObjList->takeItem(c);
        stageconfig.objects.move(c, c + 1);
        ui->scObjList->insertItem(c + 1, item);
        ui->scObjList->setCurrentRow(c + 1);
    });

    connect(ui->scRmObj, &QToolButton::clicked, [this] {
        int c = ui->scObjList->currentRow();
        int n = ui->scObjList->currentRow() == ui->scObjList->count() - 1 ? c - 1 : c;
        delete ui->scObjList->item(c);
        stageconfig.objects.removeAt(c);
        ui->scObjList->blockSignals(true);
        ui->scObjList->setCurrentRow(n);
        ui->scObjList->blockSignals(false);
    });
    connect(ui->scObjName, &QLineEdit::textChanged, [this](QString s) {
        stageconfig.objects[ui->scObjList->currentRow()] = s;

        ui->scObjList->item(ui->scObjList->currentRow())
            ->setText(stageconfig.objects[ui->scObjList->currentRow()]);
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

        ui->scSfxName->setText(stageconfig.soundFX[c].path);
        ui->scMaxConcurrentPlays->setValue(stageconfig.soundFX[c].maxConcurrentPlay);

        ui->scSfxName->blockSignals(false);
        ui->scMaxConcurrentPlays->blockSignals(false);

        if (ui->scDownSfx)
            ui->scDownSfx->setDisabled(c == ui->scSfxList->count() - 1);
        if (ui->scUpSfx)
            ui->scUpSfx->setDisabled(c == 0);
    });

    connect(ui->scAddSfx, &QToolButton::clicked, [this] {
        uint c = ui->scSfxList->currentRow() + 1;
        stageconfig.soundFX.insert(c, RSDKv5::StageConfig::WAVConfiguration());
        auto *item = new QListWidgetItem();
        item->setText(stageconfig.soundFX[c].path);
        ui->scSfxList->insertItem(c, item);

        item->setFlags(item->flags() | Qt::ItemIsEditable);
        ui->scSfxList->setCurrentItem(item);
    });

    connect(ui->scUpSfx, &QToolButton::clicked, [this] {
        uint c     = ui->scSfxList->currentRow();
        auto *item = ui->scSfxList->takeItem(c);
        stageconfig.soundFX.move(c, c - 1);
        ui->scSfxList->insertItem(c - 1, item);
        ui->scSfxList->setCurrentRow(c - 1);
    });

    connect(ui->scDownSfx, &QToolButton::clicked, [this] {
        uint c     = ui->scSfxList->currentRow();
        auto *item = ui->scSfxList->takeItem(c);
        stageconfig.soundFX.move(c, c + 1);
        ui->scSfxList->insertItem(c + 1, item);
        ui->scSfxList->setCurrentRow(c + 1);
    });

    connect(ui->scRmSfx, &QToolButton::clicked, [this] {
        int c = ui->scSfxList->currentRow();
        int n = ui->scSfxList->currentRow() == ui->scSfxList->count() - 1 ? c - 1 : c;
        delete ui->scSfxList->item(c);
        stageconfig.soundFX.removeAt(c);
        ui->scSfxList->blockSignals(true);
        ui->scSfxList->setCurrentRow(n);
        ui->scSfxList->blockSignals(false);
    });
    connect(ui->scSfxName, &QLineEdit::textChanged, [this](QString s) {
        stageconfig.soundFX[ui->scSfxList->currentRow()].path = s;

        ui->scSfxList->item(ui->scSfxList->currentRow())
            ->setText(stageconfig.soundFX[ui->scSfxList->currentRow()].path);
    });
    connect(ui->scMaxConcurrentPlays, QOverload<int>::of(&QSpinBox::valueChanged),
            [this](int v) { stageconfig.soundFX[ui->scSfxList->currentRow()].maxConcurrentPlay = v; });
}

bool GameconfigEditorv5::event(QEvent *event)
{
    if (event->type() == (QEvent::Type)RE_EVENT_NEW) {
        if (!ui->stackedWidget->currentIndex()) {
            gameConfig  = RSDKv5::GameConfig();
            rsdkconfig  = RSDKv5::RSDKConfig();
            stageconfig = RSDKv5::StageConfig();
            ui->stackedWidget->setCurrentIndex(0);
            setupUI();
        }
        else {
            stageconfig = RSDKv5::StageConfig();
            gameConfig  = RSDKv5::GameConfig();
            rsdkconfig  = RSDKv5::RSDKConfig();
            ui->stackedWidget->setCurrentIndex(1);
            setupUI();
        }
        return true;
    }
    if (event->type() == (QEvent::Type)RE_EVENT_OPEN) {
        QList<QString> filters = { "rev02 (plus) RSDKv5 GameConfig files (*GameConfig*.bin);;rev01 "
                                   "(pre-plus) RSDKv5 GameConfig files (*GameConfig*.bin)",
                                   "StageConfig files (*StageConfig*.bin)" };

        QFileDialog filedialog(this, tr("Open RSDKv5 Config"), "",
                               tr(filters[ui->stackedWidget->currentIndex()].toStdString().c_str()));
        filedialog.setAcceptMode(QFileDialog::AcceptOpen);
        if (filedialog.exec() == QDialog::Accepted) {
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
                    rsdkconfig.read(QFileInfo(filedialog.selectedFiles()[0]).absolutePath()
                                    + "/RSDKConfig.bin");
                }
                else
                    rsdkconfig = RSDKv5::RSDKConfig();
                stageconfig = RSDKv5::StageConfig();

                appConfig.addRecentFile(ENGINE_v5, TOOL_GAMECONFIGEDITOR, filedialog.selectedFiles()[0],
                                        QList<QString>{ "GameConfig", oldVer ? "rev01" : "rev02" });
            }
            else {
                ui->stackedWidget->setCurrentIndex(1);

                stageconfig.read(filedialog.selectedFiles()[0]);
                gameConfig = RSDKv5::GameConfig();
                rsdkconfig = RSDKv5::RSDKConfig();
                appConfig.addRecentFile(ENGINE_v5, TOOL_GAMECONFIGEDITOR, filedialog.selectedFiles()[0],
                                        QList<QString>{ "StageConfig", "rev02" });
            }
            setupUI();
        }
        return true;
    }

    if (event->type() == (QEvent::Type)RE_EVENT_SAVE) {
        if (!ui->stackedWidget->currentIndex()) {
            if (gameConfig.m_filename != "") {
                gameConfig.write("");
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
                gameConfig.readMode = !oldVer;
                gameConfig.write(filedialog.selectedFiles()[0]);
                appConfig.addRecentFile(ENGINE_v5, TOOL_GAMECONFIGEDITOR, filedialog.selectedFiles()[0],
                                        QList<QString>{ "GameConfig", oldVer ? "rev01" : "rev02" });
            }
        }
        else {
            if (stageconfig.m_filename != "") {
                stageconfig.write("");
                return true;
            }

            QFileDialog filedialog(
                this, tr("Save RSDKv5 StageConfig"), "",
                tr(QString("RSDKv5 StageConfig files (*StageConfig*.bin)").toStdString().c_str()));
            filedialog.setAcceptMode(QFileDialog::AcceptSave);
            if (filedialog.exec() == QDialog::Accepted) {
                stageconfig.write(filedialog.selectedFiles()[0]);
                appConfig.addRecentFile(ENGINE_v5, TOOL_GAMECONFIGEDITOR, filedialog.selectedFiles()[0],
                                        QList<QString>{ "StageConfig", "rev02" });
            }
        }
    }

    if (event->type() == (QEvent::Type)RE_EVENT_SAVE_AS) {
        if (!ui->stackedWidget->currentIndex()) {
            QList<QString> types = {
                "RSDKv5 rev02 (plus) GameConfig files (GameConfig*.bin)",
                "RSDKv5 rev01 (pre-plus) GameConfig files (GameConfig*.bin)",
                "RSDKv2 GameConfig files (GameConfig*.bin)",
                "RSDKv3 GameConfig files (GameConfig*.bin)",
                "RSDKv4 GameConfig files (GameConfig*.bin)",
            };
            QFileDialog filedialog(this, tr("Save GameConfig"), "",
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

                switch (filter) {
                    case 0:
                    case 1: {
                        setStatus("Saving GameConfig: " + filedialog.selectedFiles()[0]);

                        gameConfig.readMode = filter == 3;
                        appConfig.addRecentFile(
                            ENGINE_v5, TOOL_GAMECONFIGEDITOR, filedialog.selectedFiles()[0],
                            QList<QString>{ "GameConfig", filter == 1 ? "rev01" : "rev02" });

                        gameConfig.write(filedialog.selectedFiles()[0]);
                        break;
                    }
                    case 2: {
                        setStatus("Saving (v2) GameConfig: " + filedialog.selectedFiles()[0]);
                        RSDKv2::Gameconfig config;

                        config.gameWindowText      = gameConfig.gameTitle;
                        config.gameDescriptionText = gameConfig.gameSubtitle;

                        config.globalVariables.clear();
                        int varID = 0;
                        for (auto &var : gameConfig.globalVariables) {
                            RSDKv2::Gameconfig::GlobalVariable variable;
                            variable.m_name = rsdkconfig.variables[varID++].m_name;
                            variable.value  = var.values.count() >= 1 ? var.values[0] : 0;
                            config.globalVariables.append(variable);
                        }

                        config.scripts.clear();
                        for (auto &obj : gameConfig.objects) {
                            config.scripts.append(obj + ".txt");
                        }

                        config.soundFX.clear();
                        for (auto &sfx : gameConfig.soundFX) {
                            config.soundFX.append(sfx.m_name);
                        }

                        config.players.clear();

                        config.categories.clear();
                        int id = 0;
                        for (auto &cat : gameConfig.sceneCategories) {
                            RSDKv2::Gameconfig::Category category;
                            category.scenes.clear();

                            for (auto &scn : cat.scenes) {
                                RSDKv2::Gameconfig::SceneInfo scene;
                                scene.name        = scn.m_name;
                                scene.folder      = scn.folder;
                                scene.actID       = scn.sceneID;
                                scene.highlighted = false;
                                category.scenes.append(scene);
                            }
                            config.categories.append(category);
                            if (++id >= 4)
                                break;
                        }

                        appConfig.addRecentFile(ENGINE_v2, TOOL_GAMECONFIGEDITOR,
                                                filedialog.selectedFiles()[0], QList<QString>{ /**/ });
                        config.write(filedialog.selectedFiles()[0]);
                        break;
                    }
                    case 3: {
                        setStatus("Saving (v3) GameConfig: " + filedialog.selectedFiles()[0]);
                        RSDKv3::Gameconfig config;

                        config.gameWindowText      = gameConfig.gameTitle;
                        config.gameDescriptionText = gameConfig.gameSubtitle;

                        config.globalVariables.clear();
                        int varID = 0;
                        for (auto &var : gameConfig.globalVariables) {
                            RSDKv3::Gameconfig::GlobalVariable variable;
                            variable.m_name = rsdkconfig.variables[varID++].m_name;
                            variable.value  = var.values.count() >= 1 ? var.values[0] : 0;
                            config.globalVariables.append(variable);
                        }

                        config.objects.clear();
                        for (auto &obj : gameConfig.objects) {
                            RSDKv3::Gameconfig::ObjectInfo object;
                            object.name   = obj;
                            object.script = obj + ".txt";
                            config.objects.append(object);
                        }

                        config.soundFX.clear();
                        for (auto &sfx : gameConfig.soundFX) {
                            config.soundFX.append(sfx.m_name);
                        }

                        config.players.clear();

                        config.categories.clear();
                        int id = 0;
                        for (auto &cat : gameConfig.sceneCategories) {
                            RSDKv3::Gameconfig::Category category;
                            category.scenes.clear();

                            for (auto &scn : cat.scenes) {
                                RSDKv3::Gameconfig::SceneInfo scene;
                                scene.name        = scn.m_name;
                                scene.folder      = scn.folder;
                                scene.actID       = scn.sceneID;
                                scene.highlighted = false;
                                category.scenes.append(scene);
                            }
                            config.categories.append(category);
                            if (++id >= 4)
                                break;
                        }

                        appConfig.addRecentFile(ENGINE_v3, TOOL_GAMECONFIGEDITOR,
                                                filedialog.selectedFiles()[0], QList<QString>{ /**/ });
                        config.write(filedialog.selectedFiles()[0]);
                        break;
                    }
                    case 4: {
                        setStatus("Saving (v4) GameConfig: " + filedialog.selectedFiles()[0]);
                        RSDKv4::Gameconfig config;

                        config.gameWindowText      = gameConfig.gameTitle;
                        config.gameDescriptionText = gameConfig.gameSubtitle;

                        config.globalVariables.clear();
                        int varID = 0;
                        for (auto &var : gameConfig.globalVariables) {
                            RSDKv4::Gameconfig::GlobalVariable variable;
                            variable.m_name = rsdkconfig.variables[varID++].m_name;
                            variable.value  = var.values.count() >= 1 ? var.values[0] : 0;
                            config.globalVariables.append(variable);
                        }

                        config.objects.clear();
                        for (auto &obj : gameConfig.objects) {
                            RSDKv4::Gameconfig::ObjectInfo object;
                            object.name   = obj;
                            object.script = obj + ".txt";
                            config.objects.append(object);
                        }

                        config.soundFX.clear();
                        for (auto &sfx : gameConfig.soundFX) {
                            RSDKv4::Gameconfig::SoundInfo soundFX;
                            soundFX.m_name = QFileInfo(sfx.m_name).baseName();
                            soundFX.path   = sfx.m_name;
                            config.soundFX.append(soundFX);
                        }

                        config.players.clear();

                        config.palette.colours.clear();
                        for (int r = 0; r < 6; ++r) {
                            if (gameConfig.palettes[0].activeRows[r]) {
                                for (int c = 0; c < 16; ++c) {
                                    config.palette.colours.append(
                                        Colour(gameConfig.palettes[0].colours[r][c].red(),
                                               gameConfig.palettes[0].colours[r][c].green(),
                                               gameConfig.palettes[0].colours[r][c].blue()));
                                }
                            }
                            else {
                                for (int c = 0; c < 16; ++c) {
                                    config.palette.colours.append(Colour(0x00, 0x00, 0x00));
                                }
                            }
                        }

                        config.categories.clear();
                        int id = 0;
                        for (auto &cat : gameConfig.sceneCategories) {
                            RSDKv4::Gameconfig::Category category;
                            category.scenes.clear();

                            for (auto &scn : cat.scenes) {
                                RSDKv4::Gameconfig::SceneInfo scene;
                                scene.name        = scn.m_name;
                                scene.folder      = scn.folder;
                                scene.actID       = scn.sceneID;
                                scene.highlighted = false;
                                category.scenes.append(scene);
                            }
                            config.categories.append(category);
                            if (++id >= 4)
                                break;
                        }

                        appConfig.addRecentFile(ENGINE_v4, TOOL_GAMECONFIGEDITOR,
                                                filedialog.selectedFiles()[0], QList<QString>{ /**/ });
                        config.write(filedialog.selectedFiles()[0]);
                        break;
                    }
                }
            }
        }
        else {
            QFileDialog filedialog(
                this, tr("Save RSDKv5 StageConfig"), "",
                tr(QString("RSDKv5 StageConfig files (*StageConfig*.bin)").toStdString().c_str()));
            filedialog.setAcceptMode(QFileDialog::AcceptSave);
            if (filedialog.exec() == QDialog::Accepted) {
                stageconfig.write(filedialog.selectedFiles()[0]);
                appConfig.addRecentFile(ENGINE_v5, TOOL_GAMECONFIGEDITOR, filedialog.selectedFiles()[0],
                                        QList<QString>{ "StageConfig", "rev02" });
            }
        }
    }
    return QWidget::event(event);
}

#include "moc_gameconfigeditorv5.cpp"
