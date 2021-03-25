#include "includes.hpp"
#include "ui_gameconfigeditorv5.h"

GameconfigEditorv5::GameconfigEditorv5(QString configPath, byte type, QWidget *parent)
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
            for (int i = 0; i < m_gameconfig.m_sceneCategories[ui->gcStartingCategory->currentIndex()]
                                    .m_scenes.count();
                 ++i) {
                ui->gcStartingScene->addItem(
                    m_gameconfig.m_sceneCategories[ui->gcStartingCategory->currentIndex()]
                        .m_scenes[i]
                        .m_name);
            }
        }
    };

    auto setupInitCategories = [this, setupInitScenes] {
        ui->gcStartingCategory->clear();
        for (int i = 0; i < m_gameconfig.m_sceneCategories.count(); ++i)
            ui->gcStartingCategory->addItem(m_gameconfig.m_sceneCategories[i].m_name);

        setupInitScenes();
    };

    m_sceneModel = new QStandardItemModel(ui->gcScnTree);

    QMenu *scnAddMenu = new QMenu(ui->gcAddScn);

    auto addCat = [this, setupInitCategories] {
        const QModelIndex &index = ui->gcScnTree->currentIndex();
        bool scnSelected         = index.parent().isValid();
        uint c                   = scnSelected ? index.parent().row() + 1 : index.row() + 1;
        m_gameconfig.m_sceneCategories.insert(c, RSDKv5::GameConfig::Category());
        auto *catItem = new QStandardItem(m_gameconfig.m_sceneCategories[c].m_name);
        auto *scnItem = new QStandardItem(m_gameconfig.m_sceneCategories[c].m_scenes.first().m_name);
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
        m_gameconfig.m_sceneCategories[cat].m_scenes.insert(scn, RSDKv5::GameConfig::SceneInfo());
        auto *scnItem = new QStandardItem(m_gameconfig.m_sceneCategories[cat].m_scenes[scn].m_name);
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
            m_gameconfig.m_sceneCategories[index.parent().row()].m_scenes[index.row()].m_name =
                item->text();
            return;
        }
        m_gameconfig.m_sceneCategories[index.row()].m_name = item->text();
    });

    ui->gcAddScn->setMenu(scnAddMenu);

    connect(ui->gcRmScn, &QToolButton::clicked, [this, setupInitCategories, setupInitScenes] {
        const QModelIndex &index = ui->gcScnTree->currentIndex();

        if (index.parent().isValid()) { // Scene
            m_gameconfig.m_sceneCategories[index.parent().row()].m_scenes.removeAt(index.row());
            m_sceneModel->itemFromIndex(index.parent())->removeRow(index.row());
            setupInitScenes();
            return;
        }
        m_gameconfig.m_sceneCategories.removeAt(index.row());
        m_sceneModel->removeRow(index.row());

        setupInitCategories();
    });

    auto moveScn = [this, setupInitCategories, setupInitScenes](char translation) {
        bool isScn = ui->gcScnTree->currentIndex().parent().isValid();
        int c      = ui->gcScnTree->currentIndex().row();
        QList<QStandardItem *> item;
        if (isScn) {
            m_gameconfig.m_sceneCategories[ui->gcScnTree->currentIndex().parent().row()].m_scenes.move(
                c, c + translation);
            QStandardItem *parentItem =
                m_sceneModel->itemFromIndex(ui->gcScnTree->currentIndex().parent());
            item = parentItem->takeRow(c);
            parentItem->insertRow(c + translation, item);
            setupInitScenes();
        }
        else {
            m_gameconfig.m_sceneCategories.move(c, c + translation);
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

    connect(
        ui->gcScnTree->selectionModel(), &QItemSelectionModel::currentChanged,
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
                    c.row() >= m_gameconfig.m_sceneCategories[c.parent().row()].m_scenes.count());
                ui->gcScnFolder->setDisabled(
                    c.row() >= m_gameconfig.m_sceneCategories[c.parent().row()].m_scenes.count());
                ui->gcScnName->setDisabled(
                    c.row() >= m_gameconfig.m_sceneCategories[c.parent().row()].m_scenes.count());
                ui->gcScnMode->setDisabled(
                    c.row() >= m_gameconfig.m_sceneCategories[c.parent().row()].m_scenes.count());

                if (c.row() >= m_gameconfig.m_sceneCategories[c.parent().row()].m_scenes.count())
                    return;

                ui->gcScnID->setText(
                    m_gameconfig.m_sceneCategories[c.parent().row()].m_scenes[c.row()].m_sceneID);
                ui->gcScnMode->setValue(
                    m_gameconfig.m_sceneCategories[c.parent().row()].m_scenes[c.row()].m_modeFilter);
                ui->gcScnFolder->setText(
                    m_gameconfig.m_sceneCategories[c.parent().row()].m_scenes[c.row()].m_folder);
                ui->gcScnName->setText(
                    m_gameconfig.m_sceneCategories[c.parent().row()].m_scenes[c.row()].m_name);

                connect(ui->gcScnName, &QLineEdit::textEdited, [this, c](QString s) {
                    m_gameconfig.m_sceneCategories[c.parent().row()].m_scenes[c.row()].m_name = s;

                    // TODO: edit text
                });

                connect(ui->gcScnFolder, &QLineEdit::textEdited, [this, c](QString s) {
                    m_gameconfig.m_sceneCategories[c.parent().row()].m_scenes[c.row()].m_folder = s;
                });

                connect(ui->gcScnID, &QLineEdit::textEdited, [this, c](QString s) {
                    m_gameconfig.m_sceneCategories[c.parent().row()].m_scenes[c.row()].m_folder = s;
                });

                connect(ui->gcScnMode, QOverload<int>::of(&QSpinBox::valueChanged), [this, c](int v) {
                    m_gameconfig.m_sceneCategories[c.parent().row()].m_scenes[c.row()].m_modeFilter = v;
                });
            }

            ui->gcDownScn->setDisabled(
                c.parent().isValid()
                    ? c.row() == m_gameconfig.m_sceneCategories[c.parent().row()].m_scenes.count() - 1
                    : c.row() == m_gameconfig.m_sceneCategories.count() - 1);
            ui->gcUpScn->setDisabled(c.row() == 0);
        });

    ui->stackedWidget->setCurrentIndex(type);
    if (configPath != "") {
        if (type == 0) {
            m_gameconfig.read(configPath);
            if (QFile::exists(QFileInfo(configPath).absolutePath() + "/RSDKConfig.bin")) {
                m_rsdkconfig.read(QFileInfo(configPath).absolutePath() + "/RSDKConfig.bin");
            }
            else
                m_rsdkconfig = RSDKv5::RSDKConfig();
            m_stageconfig = RSDKv5::StageConfig();

            appConfig.addRecentFile(ENGINE_v5, TOOL_GAMECONFIGEDITOR, configPath,
                                    QList<QString>{ "GameConfig" });
        }
        else {
            m_stageconfig.read(configPath);
            m_gameconfig = RSDKv5::GameConfig();
            m_rsdkconfig = RSDKv5::RSDKConfig();
            appConfig.addRecentFile(ENGINE_v5, TOOL_GAMECONFIGEDITOR, configPath,
                                    QList<QString>{ "StageConfig" });
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
            for (int i = 0; i < m_gameconfig.m_sceneCategories[ui->gcStartingCategory->currentIndex()]
                                    .m_scenes.count();
                 ++i) {
                ui->gcStartingScene->addItem(
                    m_gameconfig.m_sceneCategories[ui->gcStartingCategory->currentIndex()]
                        .m_scenes[i]
                        .m_name);
            }
        }
    };

    auto setupInitCategories = [this, setupInitScenes] {
        ui->gcStartingCategory->clear();
        for (int i = 0; i < m_gameconfig.m_sceneCategories.count(); ++i)
            ui->gcStartingCategory->addItem(m_gameconfig.m_sceneCategories[i].m_name);

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
    ui->gcGameName->setText(m_gameconfig.m_gameName);
    ui->gcGameSubname->setText(m_gameconfig.m_gameSubname);
    ui->gcGameVersion->setText(m_gameconfig.m_version);

    setupInitCategories();
    ui->gcStartingCategory->setCurrentIndex(m_gameconfig.m_startSceneCategoryIndex);
    ui->gcStartingScene->setCurrentIndex(m_gameconfig.m_startSceneIndex);

    ui->gcObjList->clear();
    for (auto &o : m_gameconfig.m_objects) ui->gcObjList->addItem(o);
    ui->gcObjList->setCurrentRow(0x00);
    ui->gcObjName->setText("");

    ui->gcSfxList->clear();
    for (auto &s : m_gameconfig.m_sfx) ui->gcSfxList->addItem(s.m_name);
    ui->gcSfxList->setCurrentRow(0x00);
    ui->gcSfxName->setText("");
    ui->gcMaxConcurrentPlays->setValue(0x00);

    m_sceneModel->clear();
    for (int c = 0; c < m_gameconfig.m_sceneCategories.count(); ++c) {
        auto *catItem = new QStandardItem(m_gameconfig.m_sceneCategories[c].m_name);

        for (auto &scn : m_gameconfig.m_sceneCategories[c].m_scenes) {
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
    if (m_rsdkconfig.m_variables.count() > 0 && m_gameconfig.m_gameOptions.count() > 0) {
        for (auto &v : m_rsdkconfig.m_variables) {
            ui->gcVarList->addItem(v.m_name);
        }
    }
    else {
        for (int v = 0; v < m_gameconfig.m_gameOptions.count(); ++v)
            ui->gcVarList->addItem("Unnamed Variable " + QString::number(v));
    }
    ui->gcVarList->setCurrentRow(0x00);
    ui->gcVarValue->setValue(0x00);

    ui->scLoadGlobals->setChecked(m_stageconfig.m_loadGlobalObjects);

    ui->scObjList->setCurrentRow(0x00);
    ui->scObjList->clear();
    for (auto &o : m_stageconfig.m_objects) ui->scObjList->addItem(o);
    ui->scObjName->setText("");

    ui->scSfxList->setCurrentRow(0x00);
    ui->scSfxList->clear();
    for (auto &s : m_stageconfig.m_sfx) ui->scSfxList->addItem(s.m_name);
    ui->scSfxName->setText("");
    ui->scMaxConcurrentPlays->setValue(0x00);

    // ----------------
    // GENERAL
    // ----------------
    connect(ui->gcGameName, &QLineEdit::textChanged,
            [this](QString s) { m_gameconfig.m_gameName = s; });
    connect(ui->gcGameSubname, &QLineEdit::textChanged,
            [this](QString s) { m_gameconfig.m_gameSubname = s; });
    connect(ui->gcGameVersion, &QLineEdit::textChanged,
            [this](QString s) { m_gameconfig.m_version = s; });
    connect(ui->gcStartingCategory, QOverload<int>::of(&QComboBox::currentIndexChanged),
            [this, setupInitScenes](int i) {
                m_gameconfig.m_startSceneCategoryIndex = i;
                setupInitScenes();
            });
    connect(ui->gcStartingScene, QOverload<int>::of(&QComboBox::currentIndexChanged),
            [this](int i) { m_gameconfig.m_startSceneIndex = i; });

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

        ui->gcObjName->setText(m_gameconfig.m_objects[c]);

        ui->gcObjName->blockSignals(false);

        if (ui->gcDownObj)
            ui->gcDownObj->setDisabled(c == ui->gcObjList->count() - 1);
        if (ui->gcUpObj)
            ui->gcUpObj->setDisabled(c == 0);
    });

    connect(ui->gcAddObj, &QToolButton::clicked, [this] {
        uint c = ui->gcObjList->currentRow() + 1;
        m_gameconfig.m_objects.insert(c, "Object");
        auto *item = new QListWidgetItem();
        item->setText(m_gameconfig.m_objects[c]);
        ui->gcObjList->insertItem(c, item);

        item->setFlags(item->flags() | Qt::ItemIsEditable);
        ui->gcObjList->setCurrentItem(item);
    });

    connect(ui->gcUpObj, &QToolButton::clicked, [this] {
        uint c     = ui->gcObjList->currentRow();
        auto *item = ui->gcObjList->takeItem(c);
        m_gameconfig.m_objects.move(c, c - 1);
        ui->gcObjList->insertItem(c - 1, item);
        ui->gcObjList->setCurrentRow(c - 1);
    });

    connect(ui->gcDownObj, &QToolButton::clicked, [this] {
        uint c     = ui->gcObjList->currentRow();
        auto *item = ui->gcObjList->takeItem(c);
        m_gameconfig.m_objects.move(c, c + 1);
        ui->gcObjList->insertItem(c + 1, item);
        ui->gcObjList->setCurrentRow(c + 1);
    });

    connect(ui->gcRmObj, &QToolButton::clicked, [this] {
        int c = ui->gcObjList->currentRow();
        int n = ui->gcObjList->currentRow() == ui->gcObjList->count() - 1 ? c - 1 : c;
        delete ui->gcObjList->item(c);
        m_gameconfig.m_objects.removeAt(c);
        ui->gcObjList->blockSignals(true);
        ui->gcObjList->setCurrentRow(n);
        ui->gcObjList->blockSignals(false);
    });
    connect(ui->gcObjName, &QLineEdit::textChanged, [this](QString s) {
        m_gameconfig.m_objects[ui->gcObjList->currentRow()] = s;

        ui->gcObjList->item(ui->gcObjList->currentRow())
            ->setText(m_gameconfig.m_objects[ui->gcObjList->currentRow()]);
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

        ui->gcSfxName->setText(m_gameconfig.m_sfx[c].m_name);
        ui->gcMaxConcurrentPlays->setValue(m_gameconfig.m_sfx[c].m_maxConcurrentPlay);

        ui->gcSfxName->blockSignals(false);
        ui->gcMaxConcurrentPlays->blockSignals(false);

        if (ui->gcDownSfx)
            ui->gcDownSfx->setDisabled(c == ui->gcSfxList->count() - 1);
        if (ui->gcUpSfx)
            ui->gcUpSfx->setDisabled(c == 0);
    });

    connect(ui->gcAddSfx, &QToolButton::clicked, [this] {
        uint c = ui->gcSfxList->currentRow() + 1;
        m_gameconfig.m_sfx.insert(c, RSDKv5::GameConfig::WAVConfiguration());
        auto *item = new QListWidgetItem();
        item->setText(m_gameconfig.m_sfx[c].m_name);
        ui->gcSfxList->insertItem(c, item);

        item->setFlags(item->flags() | Qt::ItemIsEditable);
        ui->gcSfxList->setCurrentItem(item);
    });

    connect(ui->gcUpSfx, &QToolButton::clicked, [this] {
        uint c     = ui->gcSfxList->currentRow();
        auto *item = ui->gcSfxList->takeItem(c);
        m_gameconfig.m_sfx.move(c, c - 1);
        ui->gcSfxList->insertItem(c - 1, item);
        ui->gcSfxList->setCurrentRow(c - 1);
    });

    connect(ui->gcDownSfx, &QToolButton::clicked, [this] {
        uint c     = ui->gcSfxList->currentRow();
        auto *item = ui->gcSfxList->takeItem(c);
        m_gameconfig.m_sfx.move(c, c + 1);
        ui->gcSfxList->insertItem(c + 1, item);
        ui->gcSfxList->setCurrentRow(c + 1);
    });

    connect(ui->gcRmSfx, &QToolButton::clicked, [this] {
        int c = ui->gcSfxList->currentRow();
        int n = ui->gcSfxList->currentRow() == ui->gcSfxList->count() - 1 ? c - 1 : c;
        delete ui->gcSfxList->item(c);
        m_gameconfig.m_sfx.removeAt(c);
        ui->gcSfxList->blockSignals(true);
        ui->gcSfxList->setCurrentRow(n);
        ui->gcSfxList->blockSignals(false);
    });
    connect(ui->gcSfxName, &QLineEdit::textChanged, [this](QString s) {
        m_gameconfig.m_sfx[ui->gcSfxList->currentRow()].m_name = s;

        ui->gcSfxList->item(ui->gcSfxList->currentRow())
            ->setText(m_gameconfig.m_sfx[ui->gcSfxList->currentRow()].m_name);
    });
    connect(ui->gcMaxConcurrentPlays, QOverload<int>::of(&QSpinBox::valueChanged),
            [this](int v) { m_gameconfig.m_sfx[ui->gcSfxList->currentRow()].m_maxConcurrentPlay = v; });

    // ----------------
    // GAME OPTIONS
    // ----------------
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

        if (c < m_rsdkconfig.m_variables.count())
            ui->gcVarName->setText(m_rsdkconfig.m_variables[c].m_name);
        else
            ui->gcVarName->setText("Unnamed Variable " + QString::number(c));
        ui->gcVarCount->setValue(m_gameconfig.m_gameOptions[c].m_values.count());
        ui->gcVarIndex->setValue(0);
        if (m_gameconfig.m_gameOptions[c].m_values.count() > 0)
            ui->gcVarValue->setValue(m_gameconfig.m_gameOptions[c].m_values[0]);
        else
            ui->gcVarValue->setValue(0);

        ui->gcVarName->blockSignals(false);
        ui->gcVarCount->blockSignals(false);
        if (m_gameconfig.m_gameOptions[c].m_values.count() > 0)
            ui->gcVarIndex->blockSignals(false);
        if (m_gameconfig.m_gameOptions[c].m_values.count() > 0)
            ui->gcVarValue->blockSignals(false);
    });
    connect(ui->gcVarIndex, QOverload<int>::of(&QSpinBox::valueChanged), [this](int v) {
        if (m_gameconfig.m_gameOptions[ui->gcVarList->currentRow()].m_values.count() == 0)
            ui->gcVarIndex->setValue(0);
        else if (v >= m_gameconfig.m_gameOptions[ui->gcVarList->currentRow()].m_values.count())
            ui->gcVarIndex->setValue(
                m_gameconfig.m_gameOptions[ui->gcVarList->currentRow()].m_values.count() - 1);

        if (m_gameconfig.m_gameOptions[ui->gcVarList->currentRow()].m_values.count() > 0) {
            ui->gcVarValue->blockSignals(true);
            ui->gcVarValue->setValue(m_gameconfig.m_gameOptions[ui->gcVarList->currentRow()]
                                         .m_values[ui->gcVarIndex->value()]);
            ui->gcVarValue->blockSignals(false);
        }
    });
    connect(ui->gcVarValue, QOverload<int>::of(&QSpinBox::valueChanged), [this](int v) {
        if (m_gameconfig.m_gameOptions[ui->gcVarList->currentRow()].m_values.count() > 0)
            m_gameconfig.m_gameOptions[ui->gcVarList->currentRow()].m_values[ui->gcVarIndex->value()] =
                v;
        else
            ui->gcVarValue->setValue(0);
    });

    // ----------------
    // GENERAL
    // ----------------
    connect(ui->scLoadGlobals, &QCheckBox::clicked,
            [this](int c) { m_stageconfig.m_loadGlobalObjects = c; });

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

        ui->scObjName->setText(m_stageconfig.m_objects[c]);

        ui->scObjName->blockSignals(false);

        if (ui->scDownObj)
            ui->scDownObj->setDisabled(c == ui->scObjList->count() - 1);
        if (ui->scUpObj)
            ui->scUpObj->setDisabled(c == 0);
    });

    connect(ui->scAddObj, &QToolButton::clicked, [this] {
        uint c = ui->scObjList->currentRow() + 1;
        m_stageconfig.m_objects.insert(c, "Object");
        auto *item = new QListWidgetItem();
        item->setText(m_stageconfig.m_objects[c]);
        ui->scObjList->insertItem(c, item);

        item->setFlags(item->flags() | Qt::ItemIsEditable);
        ui->scObjList->setCurrentItem(item);
    });

    connect(ui->scUpObj, &QToolButton::clicked, [this] {
        uint c     = ui->scObjList->currentRow();
        auto *item = ui->scObjList->takeItem(c);
        m_stageconfig.m_objects.move(c, c - 1);
        ui->scObjList->insertItem(c - 1, item);
        ui->scObjList->setCurrentRow(c - 1);
    });

    connect(ui->scDownObj, &QToolButton::clicked, [this] {
        uint c     = ui->scObjList->currentRow();
        auto *item = ui->scObjList->takeItem(c);
        m_stageconfig.m_objects.move(c, c + 1);
        ui->scObjList->insertItem(c + 1, item);
        ui->scObjList->setCurrentRow(c + 1);
    });

    connect(ui->scRmObj, &QToolButton::clicked, [this] {
        int c = ui->scObjList->currentRow();
        int n = ui->scObjList->currentRow() == ui->scObjList->count() - 1 ? c - 1 : c;
        delete ui->scObjList->item(c);
        m_stageconfig.m_objects.removeAt(c);
        ui->scObjList->blockSignals(true);
        ui->scObjList->setCurrentRow(n);
        ui->scObjList->blockSignals(false);
    });
    connect(ui->scObjName, &QLineEdit::textChanged, [this](QString s) {
        m_stageconfig.m_objects[ui->scObjList->currentRow()] = s;

        ui->scObjList->item(ui->scObjList->currentRow())
            ->setText(m_stageconfig.m_objects[ui->scObjList->currentRow()]);
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

        ui->scSfxName->setText(m_stageconfig.m_sfx[c].m_name);
        ui->scMaxConcurrentPlays->setValue(m_stageconfig.m_sfx[c].m_maxConcurrentPlay);

        ui->scSfxName->blockSignals(false);
        ui->scMaxConcurrentPlays->blockSignals(false);

        if (ui->scDownSfx)
            ui->scDownSfx->setDisabled(c == ui->scSfxList->count() - 1);
        if (ui->scUpSfx)
            ui->scUpSfx->setDisabled(c == 0);
    });

    connect(ui->scAddSfx, &QToolButton::clicked, [this] {
        uint c = ui->scSfxList->currentRow() + 1;
        m_stageconfig.m_sfx.insert(c, RSDKv5::StageConfig::WAVConfiguration());
        auto *item = new QListWidgetItem();
        item->setText(m_stageconfig.m_sfx[c].m_name);
        ui->scSfxList->insertItem(c, item);

        item->setFlags(item->flags() | Qt::ItemIsEditable);
        ui->scSfxList->setCurrentItem(item);
    });

    connect(ui->scUpSfx, &QToolButton::clicked, [this] {
        uint c     = ui->scSfxList->currentRow();
        auto *item = ui->scSfxList->takeItem(c);
        m_stageconfig.m_sfx.move(c, c - 1);
        ui->scSfxList->insertItem(c - 1, item);
        ui->scSfxList->setCurrentRow(c - 1);
    });

    connect(ui->scDownSfx, &QToolButton::clicked, [this] {
        uint c     = ui->scSfxList->currentRow();
        auto *item = ui->scSfxList->takeItem(c);
        m_stageconfig.m_sfx.move(c, c + 1);
        ui->scSfxList->insertItem(c + 1, item);
        ui->scSfxList->setCurrentRow(c + 1);
    });

    connect(ui->scRmSfx, &QToolButton::clicked, [this] {
        int c = ui->scSfxList->currentRow();
        int n = ui->scSfxList->currentRow() == ui->scSfxList->count() - 1 ? c - 1 : c;
        delete ui->scSfxList->item(c);
        m_stageconfig.m_sfx.removeAt(c);
        ui->scSfxList->blockSignals(true);
        ui->scSfxList->setCurrentRow(n);
        ui->scSfxList->blockSignals(false);
    });
    connect(ui->scSfxName, &QLineEdit::textChanged, [this](QString s) {
        m_stageconfig.m_sfx[ui->scSfxList->currentRow()].m_name = s;

        ui->scSfxList->item(ui->scSfxList->currentRow())
            ->setText(m_stageconfig.m_sfx[ui->scSfxList->currentRow()].m_name);
    });
    connect(ui->scMaxConcurrentPlays, QOverload<int>::of(&QSpinBox::valueChanged), [this](int v) {
        m_stageconfig.m_sfx[ui->scSfxList->currentRow()].m_maxConcurrentPlay = v;
    });
}

bool GameconfigEditorv5::event(QEvent *event)
{
    if (event->type() == (QEvent::Type)RE_EVENT_NEW) {
        if (!ui->stackedWidget->currentIndex()) {
            m_gameconfig  = RSDKv5::GameConfig();
            m_rsdkconfig  = RSDKv5::RSDKConfig();
            m_stageconfig = RSDKv5::StageConfig();
            ui->stackedWidget->setCurrentIndex(0);
            setupUI();
        }
        else {
            m_stageconfig = RSDKv5::StageConfig();
            m_gameconfig  = RSDKv5::GameConfig();
            m_rsdkconfig  = RSDKv5::RSDKConfig();
            ui->stackedWidget->setCurrentIndex(1);
            setupUI();
        }
        return true;
    }
    if (event->type() == (QEvent::Type)RE_EVENT_OPEN) {
        QList<QString> filters = { "RSDKv5 GameConfig files (*GameConfig*.bin)",
                                   "StageConfig files (*StageConfig*.bin)" };

        QFileDialog filedialog(this, tr("Open RSDKv5 Config"), "",
                               tr(filters[ui->stackedWidget->currentIndex()].toStdString().c_str()));
        filedialog.setAcceptMode(QFileDialog::AcceptOpen);
        if (filedialog.exec() == QDialog::Accepted) {
            if (filedialog.selectedNameFilter() == filters[0]) {
                ui->stackedWidget->setCurrentIndex(0);

                m_gameconfig.read(filedialog.selectedFiles()[0]);
                if (QFile::exists(QFileInfo(filedialog.selectedFiles()[0]).absolutePath()
                                  + "/RSDKConfig.bin")) {
                    m_rsdkconfig.read(QFileInfo(filedialog.selectedFiles()[0]).absolutePath()
                                      + "/RSDKConfig.bin");
                }
                else
                    m_rsdkconfig = RSDKv5::RSDKConfig();
                m_stageconfig = RSDKv5::StageConfig();

                appConfig.addRecentFile(ENGINE_v5, TOOL_GAMECONFIGEDITOR, filedialog.selectedFiles()[0],
                                        QList<QString>{ "GameConfig" });
            }
            else {
                ui->stackedWidget->setCurrentIndex(1);

                m_stageconfig.read(filedialog.selectedFiles()[0]);
                m_gameconfig = RSDKv5::GameConfig();
                m_rsdkconfig = RSDKv5::RSDKConfig();
                appConfig.addRecentFile(ENGINE_v5, TOOL_GAMECONFIGEDITOR, filedialog.selectedFiles()[0],
                                        QList<QString>{ "StageConfig" });
            }
            setupUI();
        }
        return true;
    }

    if (event->type() == (QEvent::Type)RE_EVENT_SAVE) {
        if (!ui->stackedWidget->currentIndex()) {
            if (m_gameconfig.m_filename != "") {
                m_gameconfig.write("");
                return true;
            }

            QFileDialog filedialog(
                this, tr("Save RSDKv5 GameConfig"), "",
                tr(QString("RSDKv5 GameConfig files (*GameConfig*.bin)").toStdString().c_str()));
            filedialog.setAcceptMode(QFileDialog::AcceptSave);
            if (filedialog.exec() == QDialog::Accepted) {
                m_gameconfig.write(filedialog.selectedFiles()[0]);
                appConfig.addRecentFile(ENGINE_v5, TOOL_GAMECONFIGEDITOR, filedialog.selectedFiles()[0],
                                        QList<QString>{ "GameConfig" });
            }
        }
        else {
            if (m_stageconfig.m_filename != "") {
                m_stageconfig.write("");
                return true;
            }

            QFileDialog filedialog(
                this, tr("Save RSDKv5 StageConfig"), "",
                tr(QString("RSDKv5 StageConfig files (*StageConfig*.bin)").toStdString().c_str()));
            filedialog.setAcceptMode(QFileDialog::AcceptSave);
            if (filedialog.exec() == QDialog::Accepted) {
                m_stageconfig.write(filedialog.selectedFiles()[0]);
                appConfig.addRecentFile(ENGINE_v5, TOOL_GAMECONFIGEDITOR, filedialog.selectedFiles()[0],
                                        QList<QString>{ "StageConfig" });
            }
        }
    }

    if (event->type() == (QEvent::Type)RE_EVENT_SAVE_AS) {
        if (!ui->stackedWidget->currentIndex()) {
            QFileDialog filedialog(
                this, tr("Save RSDKv5 GameConfig"), "",
                tr(QString("RSDKv5 GameConfig files (*GameConfig*.bin)").toStdString().c_str()));
            filedialog.setAcceptMode(QFileDialog::AcceptSave);
            if (filedialog.exec() == QDialog::Accepted) {
                m_gameconfig.write(filedialog.selectedFiles()[0]);
                appConfig.addRecentFile(ENGINE_v5, TOOL_GAMECONFIGEDITOR, filedialog.selectedFiles()[0],
                                        QList<QString>{ "GameConfig" });
            }
        }
        else {
            QFileDialog filedialog(
                this, tr("Save RSDKv5 StageConfig"), "",
                tr(QString("RSDKv5 StageConfig files (*StageConfig*.bin)").toStdString().c_str()));
            filedialog.setAcceptMode(QFileDialog::AcceptSave);
            if (filedialog.exec() == QDialog::Accepted) {
                m_stageconfig.write(filedialog.selectedFiles()[0]);
                appConfig.addRecentFile(ENGINE_v5, TOOL_GAMECONFIGEDITOR, filedialog.selectedFiles()[0],
                                        QList<QString>{ "StageConfig" });
            }
        }
    }
    return QWidget::event(event);
}

#include "moc_gameconfigeditorv5.cpp"
