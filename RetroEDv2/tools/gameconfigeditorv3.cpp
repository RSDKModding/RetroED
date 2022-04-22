#include "includes.hpp"
#include "ui_gameconfigeditorv3.h"

GameConfigEditorv3::GameConfigEditorv3(QString path, QWidget *parent)
    : QWidget(parent), ui(new Ui::GameConfigEditorv3)
{
    ui->setupUi(this);

    ui->sectionList->blockSignals(true);
    ui->sectionList->setCurrentRow(-1);
    ui->sectionList->blockSignals(false);
    ui->sectionList->setCurrentRow(0);

    connect(ui->sectionList, &QListWidget::currentRowChanged, [this](int v) {
        ui->objList->blockSignals(true);
        ui->objList->setCurrentRow(0);
        ui->objList->blockSignals(false);
        ui->objList->setCurrentRow(-1);

        ui->sfxList->blockSignals(true);
        ui->sfxList->setCurrentRow(0);
        ui->sfxList->blockSignals(false);
        ui->sfxList->setCurrentRow(-1);

        ui->varList->blockSignals(true);
        ui->varList->setCurrentRow(0);
        ui->varList->blockSignals(false);
        ui->varList->setCurrentRow(-1);

        ui->plrList->blockSignals(true);
        ui->plrList->setCurrentRow(0);
        ui->plrList->blockSignals(false);
        ui->plrList->setCurrentRow(-1);

        ui->detailsWidget->setCurrentIndex(v);
        ui->sectionLabel->setText(ui->sectionList->item(v)->text());
    });

    // ----------------
    // DETAILS
    // ----------------

    connect(ui->gameName, &QLineEdit::textEdited, [this](QString s) {
        gameConfig.gameWindowText = s;
        doAction("Changed Game Title");
    });
    connect(ui->gameDescription, &QPlainTextEdit::textChanged, [this] {
        gameConfig.gameDescriptionText = ui->gameDescription->toPlainText();
        doAction("Changed Game Description");
    });

    // ----------------
    // OBJECTS
    // ----------------
    connect(ui->objList, &QListWidget::currentRowChanged, [this](int c) {
        if (ui->upObj)
            ui->upObj->setDisabled(c == -1);
        if (ui->downObj)
            ui->downObj->setDisabled(c == -1);
        if (ui->rmObj)
            ui->rmObj->setDisabled(c == -1);

        ui->objName->setDisabled(c == -1);
        ui->objScript->setDisabled(c == -1);
        ui->objType->setText("Object Type ID: ");

        if (c == -1)
            return;

        ui->objName->blockSignals(true);
        ui->objScript->blockSignals(true);

        ui->objName->setText(gameConfig.objects[c].name);
        ui->objScript->setText(gameConfig.objects[c].script);
        ui->objType->setText("Object Type ID: " + QString::number(c + 1));

        ui->objName->blockSignals(false);
        ui->objScript->blockSignals(false);

        if (ui->downObj)
            ui->downObj->setDisabled(c == ui->objList->count() - 1);
        if (ui->upObj)
            ui->upObj->setDisabled(c == 0);
    });

    connect(ui->addObj, &QToolButton::clicked, [this] {
        uint c = ui->objList->currentRow() + 1;
        gameConfig.objects.insert(c, RSDKv3::GameConfig::ObjectInfo());
        auto *item = new QListWidgetItem();
        item->setText(gameConfig.objects[c].name);
        ui->objList->insertItem(c, item);

        item->setFlags(item->flags() | Qt::ItemIsEditable);
        ui->objList->setCurrentItem(item);
        doAction("Added Object");
    });

    connect(ui->upObj, &QToolButton::clicked, [this] {
        uint c     = ui->objList->currentRow();
        auto *item = ui->objList->takeItem(c);
        gameConfig.objects.move(c, c - 1);
        ui->objList->insertItem(c - 1, item);
        ui->objList->setCurrentRow(c - 1);
        doAction("Moved Object Up");
    });

    connect(ui->downObj, &QToolButton::clicked, [this] {
        uint c     = ui->objList->currentRow();
        auto *item = ui->objList->takeItem(c);
        gameConfig.objects.move(c, c + 1);
        ui->objList->insertItem(c + 1, item);
        ui->objList->setCurrentRow(c + 1);
        doAction("Moved Object Down");
    });

    connect(ui->rmObj, &QToolButton::clicked, [this] {
        int c = ui->objList->currentRow();
        int n = ui->objList->currentRow() == ui->objList->count() - 1 ? c - 1 : c;
        delete ui->objList->item(c);
        gameConfig.objects.removeAt(c);
        ui->objList->blockSignals(true);
        ui->objList->setCurrentRow(n);
        ui->objList->blockSignals(false);
        doAction("Removed Object");
    });

    connect(ui->objList, &QListWidget::itemChanged, [this](QListWidgetItem *item) {
        gameConfig.objects[ui->objList->row(item)].name = item->text();

        ui->objName->blockSignals(true);
        ui->objName->setText(gameConfig.objects[ui->objList->row(item)].name);
        ui->objName->blockSignals(false);
        doAction("Changed Object Name");
    });

    connect(ui->objName, &QLineEdit::textEdited, [this](QString s) {
        gameConfig.objects[ui->objList->currentRow()].name = s;

        ui->objList->item(ui->objList->currentRow())
            ->setText(gameConfig.objects[ui->objList->currentRow()].name);
        doAction("Changed Object Name");
    });
    connect(ui->objScript, &QLineEdit::textEdited, [this](QString s) {
        gameConfig.objects[ui->objList->currentRow()].script = s;
        doAction("Changed Object Script");
    });

    // ----------------
    // SOUNDFX
    // ----------------
    connect(ui->sfxList, &QListWidget::currentRowChanged, [this](int c) {
        if (ui->upSfx)
            ui->upSfx->setDisabled(c == -1);
        if (ui->downSfx)
            ui->downSfx->setDisabled(c == -1);
        if (ui->rmSfx)
            ui->rmSfx->setDisabled(c == -1);

        ui->sfxName->setDisabled(c == -1);

        ui->sfxID->setText("SoundFX ID: ");

        if (c == -1)
            return;

        ui->sfxName->blockSignals(true);
        ui->sfxName->setText(gameConfig.soundFX[c]);
        ui->sfxName->blockSignals(false);

        ui->sfxID->setText("SoundFX ID: " + QString::number(c));

        if (ui->downSfx)
            ui->downSfx->setDisabled(c == ui->sfxList->count() - 1);
        if (ui->upSfx)
            ui->upSfx->setDisabled(c == 0);
    });

    connect(ui->addSfx, &QToolButton::clicked, [this] {
        uint c = ui->sfxList->currentRow() + 1;
        gameConfig.soundFX.insert(c, "Folder/Sound.wav");
        auto *item = new QListWidgetItem();
        item->setText(gameConfig.soundFX[c]);
        ui->sfxList->insertItem(c, item);

        item->setFlags(item->flags() | Qt::ItemIsEditable);
        ui->sfxList->setCurrentItem(item);
        doAction("Added Sfx");
    });

    connect(ui->upSfx, &QToolButton::clicked, [this] {
        uint c     = ui->sfxList->currentRow();
        auto *item = ui->sfxList->takeItem(c);
        gameConfig.soundFX.move(c, c - 1);
        ui->sfxList->insertItem(c - 1, item);
        ui->sfxList->setCurrentRow(c - 1);
        doAction("Moved Sfx Up");
    });

    connect(ui->downSfx, &QToolButton::clicked, [this] {
        uint c     = ui->sfxList->currentRow();
        auto *item = ui->sfxList->takeItem(c);
        gameConfig.soundFX.move(c, c + 1);
        ui->sfxList->insertItem(c + 1, item);
        ui->sfxList->setCurrentRow(c + 1);
        doAction("Moved Sfx Down");
    });

    connect(ui->rmSfx, &QToolButton::clicked, [this] {
        int c = ui->sfxList->currentRow();
        int n = ui->sfxList->currentRow() == ui->sfxList->count() - 1 ? c - 1 : c;
        delete ui->sfxList->item(c);
        gameConfig.soundFX.removeAt(c);
        ui->sfxList->blockSignals(true);
        ui->sfxList->setCurrentRow(n);
        ui->sfxList->blockSignals(false);
        doAction("Removed Sfx");
    });

    connect(ui->sfxList, &QListWidget::itemChanged, [this](QListWidgetItem *item) {
        gameConfig.soundFX[ui->sfxList->row(item)] = item->text();

        ui->sfxName->blockSignals(true);
        ui->sfxName->setText(gameConfig.soundFX[ui->sfxList->row(item)]);
        ui->sfxName->blockSignals(false);
        doAction("Changed Sfx Path");
    });

    connect(ui->sfxName, &QLineEdit::textEdited, [this](QString s) {
        gameConfig.soundFX[ui->sfxList->currentRow()] = s;

        ui->sfxList->item(ui->sfxList->currentRow())
            ->setText(gameConfig.soundFX[ui->sfxList->currentRow()]);
        doAction("Changed Sfx Path");
    });

    // ----------------
    // GLOBAL VARIABLES
    // ----------------
    connect(ui->varList, &QListWidget::currentRowChanged, [this](int c) {
        if (ui->upVar)
            ui->upVar->setDisabled(c == -1);
        if (ui->downVar)
            ui->downVar->setDisabled(c == -1);
        if (ui->rmVar)
            ui->rmVar->setDisabled(c == -1);

        ui->varName->setDisabled(c == -1);
        ui->varValue->setDisabled(c == -1);

        if (c == -1)
            return;

        ui->varName->blockSignals(true);
        ui->varValue->blockSignals(true);

        ui->varName->setText(gameConfig.globalVariables[c].name);
        ui->varValue->setValue(gameConfig.globalVariables[c].value);

        ui->varName->blockSignals(false);
        ui->varValue->blockSignals(false);

        if (ui->downVar)
            ui->downVar->setDisabled(c == ui->varList->count() - 1);
        if (ui->upVar)
            ui->upVar->setDisabled(c == 0);
    });

    connect(ui->addVar, &QToolButton::clicked, [this] {
        uint c = ui->varList->currentRow() + 1;
        gameConfig.globalVariables.insert(c, RSDKv3::GameConfig::GlobalVariable());
        auto *item = new QListWidgetItem();
        item->setText(gameConfig.globalVariables[c].name);
        ui->varList->insertItem(c, item);

        item->setFlags(item->flags() | Qt::ItemIsEditable);
        ui->varList->setCurrentItem(item);
        doAction("Added Variable");
    });

    connect(ui->upVar, &QToolButton::clicked, [this] {
        uint c     = ui->varList->currentRow();
        auto *item = ui->varList->takeItem(c);
        gameConfig.globalVariables.move(c, c - 1);
        ui->varList->insertItem(c - 1, item);
        ui->varList->setCurrentRow(c - 1);
        doAction("Moved Variable Up");
    });

    connect(ui->downVar, &QToolButton::clicked, [this] {
        uint c     = ui->varList->currentRow();
        auto *item = ui->varList->takeItem(c);
        gameConfig.globalVariables.move(c, c + 1);
        ui->varList->insertItem(c + 1, item);
        ui->varList->setCurrentRow(c + 1);
        doAction("Moved Variable Down");
    });

    connect(ui->rmVar, &QToolButton::clicked, [this] {
        int c = ui->varList->currentRow();
        int n = ui->varList->currentRow() == ui->varList->count() - 1 ? c - 1 : c;
        delete ui->varList->item(c);
        gameConfig.globalVariables.removeAt(c);
        ui->varList->blockSignals(true);
        ui->varList->setCurrentRow(n);
        ui->varList->blockSignals(false);
        doAction("Removed Variable");
    });

    connect(ui->varList, &QListWidget::itemChanged, [this](QListWidgetItem *item) {
        gameConfig.globalVariables[ui->varList->row(item)].name = item->text();

        ui->varName->blockSignals(true);
        ui->varName->setText(gameConfig.globalVariables[ui->varList->row(item)].name);
        ui->varName->blockSignals(false);
        doAction("Changed Variable Name");
    });

    connect(ui->varName, &QLineEdit::textEdited, [this](QString s) {
        gameConfig.globalVariables[ui->varList->currentRow()].name = s;

        ui->varList->item(ui->varList->currentRow())
            ->setText(gameConfig.globalVariables[ui->varList->currentRow()].name);
        doAction("Changed Variable Name");
    });

    connect(ui->varValue, QOverload<int>::of(&QSpinBox::valueChanged), [this](int v) {
        gameConfig.globalVariables[ui->varList->currentRow()].value = v;
        doAction("Changed Variable Value");
    });

    // ----------------
    // PLAYERS
    // ----------------
    connect(ui->plrList, &QListWidget::currentRowChanged, [this](int c) {
        if (ui->upPlr)
            ui->upPlr->setDisabled(c == -1);
        if (ui->downPlr)
            ui->downPlr->setDisabled(c == -1);
        if (ui->rmPlr)
            ui->rmPlr->setDisabled(c == -1);

        ui->plrName->setDisabled(c == -1);

        if (c == -1)
            return;

        ui->plrName->blockSignals(true);
        ui->plrName->setText(gameConfig.players[c]);
        ui->plrName->blockSignals(false);

        if (ui->downPlr)
            ui->downPlr->setDisabled(c == ui->plrList->count() - 1);
        if (ui->upPlr)
            ui->upPlr->setDisabled(c == 0);
    });

    connect(ui->addPlr, &QToolButton::clicked, [this] {
        uint c = ui->plrList->currentRow() + 1;
        gameConfig.players.insert(c, "PLAYER");
        auto *item = new QListWidgetItem();
        item->setText(gameConfig.players[c]);
        ui->plrList->insertItem(c, item);

        item->setFlags(item->flags() | Qt::ItemIsEditable);
        ui->plrList->setCurrentItem(item);
        doAction("Added Player");
    });

    connect(ui->upPlr, &QToolButton::clicked, [this] {
        uint c     = ui->plrList->currentRow();
        auto *item = ui->plrList->takeItem(c);
        gameConfig.players.move(c, c - 1);
        ui->plrList->insertItem(c - 1, item);
        ui->plrList->setCurrentRow(c - 1);
        doAction("Moved Player Up");
    });

    connect(ui->downPlr, &QToolButton::clicked, [this] {
        uint c     = ui->plrList->currentRow();
        auto *item = ui->plrList->takeItem(c);
        gameConfig.players.move(c, c + 1);
        ui->plrList->insertItem(c + 1, item);
        ui->plrList->setCurrentRow(c + 1);
        doAction("Moved Player Down");
    });

    connect(ui->rmPlr, &QToolButton::clicked, [this] {
        int c = ui->plrList->currentRow();
        int n = ui->plrList->currentRow() == ui->plrList->count() - 1 ? c - 1 : c;
        delete ui->plrList->item(c);
        gameConfig.players.removeAt(c);
        ui->plrList->blockSignals(true);
        ui->plrList->setCurrentRow(n);
        ui->plrList->blockSignals(false);
        doAction("Removed Player");
    });

    connect(ui->plrList, &QListWidget::itemChanged, [this](QListWidgetItem *item) {
        gameConfig.players[ui->plrList->row(item)] = item->text().toUpper();

        ui->plrName->blockSignals(true);
        ui->plrName->setText(gameConfig.players[ui->plrList->row(item)]);
        ui->plrName->blockSignals(false);
        doAction("Changed Player Name");
    });

    connect(ui->plrName, &QLineEdit::textEdited, [this](QString s) {
        gameConfig.players[ui->plrList->currentRow()] = s.toUpper();

        ui->plrList->item(ui->plrList->currentRow())
            ->setText(gameConfig.players[ui->plrList->currentRow()]);
        doAction("Changed Player Name");
    });

    // ----------------
    // SCENES
    // ----------------
    if (sceneModel) {
        delete sceneModel;
        sceneModel = nullptr;
    }
    sceneModel = new QStandardItemModel(ui->scnTree);

    connect(sceneModel, &QStandardItemModel::itemChanged, [this](QStandardItem *item) {
        const QModelIndex &index = sceneModel->indexFromItem(item);
        if (index.parent().isValid()) { // Scene
            gameConfig.stageLists[index.parent().row()].scenes[index.row()].name = item->text();
            doAction("Changed Scene Name");
            return;
        }
    });

    connect(ui->addScn, &QToolButton::clicked, [this] {
        const QModelIndex &index = ui->scnTree->currentIndex();
        bool scnSelected         = index.parent().isValid();
        if (!index.isValid()) {
            return;
        }
        uint cat = scnSelected ? index.parent().row() : index.row();
        uint scn = scnSelected ? index.row() + 1 : 0;
        gameConfig.stageLists[cat].scenes.insert(scn, RSDKv3::GameConfig::SceneInfo());
        auto *scnItem = new QStandardItem(gameConfig.stageLists[cat].scenes[scn].name);
        if (scnSelected)
            sceneModel->itemFromIndex(index.parent())->insertRow(scn, scnItem);
        else
            sceneModel->itemFromIndex(index)->insertRow(scn, scnItem);
        scnItem->setFlags(scnItem->flags() | Qt::ItemIsEditable);
        ui->scnTree->setCurrentIndex(sceneModel->indexFromItem(scnItem));
        doAction("Added Scene");
    });

    connect(ui->rmScn, &QToolButton::clicked, [this] {
        const QModelIndex &index = ui->scnTree->currentIndex();

        if (index.parent().isValid()) { // Scene
            gameConfig.stageLists[index.parent().row()].scenes.removeAt(index.row());
            sceneModel->itemFromIndex(index.parent())->removeRow(index.row());
            doAction("Removed Scene");
            return;
        }
    });

    auto moveScn = [this](char translation) {
        bool isScn = ui->scnTree->currentIndex().parent().isValid();
        int c      = ui->scnTree->currentIndex().row();
        QList<QStandardItem *> item;
        if (isScn) {
            gameConfig.stageLists[ui->scnTree->currentIndex().parent().row()].scenes.move(
                c, c + translation);
            QStandardItem *parentItem = sceneModel->itemFromIndex(ui->scnTree->currentIndex().parent());
            item                      = parentItem->takeRow(c);
            parentItem->insertRow(c + translation, item);
            doAction("Moved Scene");
        }
        else
            return;
        ui->scnTree->setCurrentIndex(sceneModel->indexFromItem(item.first()));
    };

    connect(ui->upScn, &QToolButton::clicked, [moveScn] { moveScn(-1); });

    connect(ui->downScn, &QToolButton::clicked, [moveScn] { moveScn(1); });

    ui->scnTree->setModel(sceneModel);

    connect(
        ui->scnTree->selectionModel(), &QItemSelectionModel::currentChanged,
        [this](const QModelIndex &c) {
            ui->upScn->setDisabled(!c.isValid());
            ui->downScn->setDisabled(!c.isValid());
            ui->rmScn->setDisabled(!c.isValid());

            disconnect(ui->scnHighlighted, nullptr, nullptr, nullptr);
            disconnect(ui->scnID, nullptr, nullptr, nullptr);
            disconnect(ui->scnFolder, nullptr, nullptr, nullptr);
            disconnect(ui->scnName, nullptr, nullptr, nullptr);

            ui->scnID->setDisabled(!c.parent().isValid());
            ui->scnFolder->setDisabled(!c.parent().isValid());
            ui->scnName->setDisabled(!c.parent().isValid());
            ui->scnHighlighted->setDisabled(!c.parent().isValid());

            // ui->addScn->setDisabled(!c.parent().isValid());
            ui->upScn->setDisabled(!c.parent().isValid());
            ui->downScn->setDisabled(!c.parent().isValid());
            ui->rmScn->setDisabled(!c.parent().isValid());

            if (!c.isValid())
                return;

            if (c.parent().isValid()) {
                ui->scnID->setDisabled(c.row()
                                       >= gameConfig.stageLists[c.parent().row()].scenes.count());
                ui->scnFolder->setDisabled(c.row()
                                           >= gameConfig.stageLists[c.parent().row()].scenes.count());
                ui->scnName->setDisabled(c.row()
                                         >= gameConfig.stageLists[c.parent().row()].scenes.count());
                ui->scnHighlighted->setDisabled(
                    c.row() >= gameConfig.stageLists[c.parent().row()].scenes.count());

                if (c.row() >= gameConfig.stageLists[c.parent().row()].scenes.count())
                    return;

                ui->scnID->setText(gameConfig.stageLists[c.parent().row()].scenes[c.row()].id);
                ui->scnHighlighted->setChecked(
                    gameConfig.stageLists[c.parent().row()].scenes[c.row()].highlighted);
                ui->scnFolder->setText(gameConfig.stageLists[c.parent().row()].scenes[c.row()].folder);
                ui->scnName->setText(gameConfig.stageLists[c.parent().row()].scenes[c.row()].name);

                connect(ui->scnName, &QLineEdit::textEdited, [this, c](QString s) {
                    gameConfig.stageLists[c.parent().row()].scenes[c.row()].name = s;

                    // TODO: edit text
                    doAction("Changed Scene Name");
                });

                connect(ui->scnFolder, &QLineEdit::textEdited, [this, c](QString s) {
                    gameConfig.stageLists[c.parent().row()].scenes[c.row()].folder = s;
                    doAction("Changed Scene Folder");
                });

                connect(ui->scnID, &QLineEdit::textEdited, [this, c](QString s) {
                    gameConfig.stageLists[c.parent().row()].scenes[c.row()].id = s;
                    doAction("Changed Scene ID");
                });

                connect(ui->scnHighlighted, &QCheckBox::toggled, [this, c](bool v) {
                    gameConfig.stageLists[c.parent().row()].scenes[c.row()].highlighted = v;
                    doAction("Changed Scene Highlight");
                });
            }

            ui->downScn->setDisabled(
                c.parent().isValid()
                    ? c.row() == gameConfig.stageLists[c.parent().row()].scenes.count() - 1
                    : c.row() == gameConfig.stageLists.count() - 1);
            ui->upScn->setDisabled(c.row() == 0);
        });

    load(path);
}

GameConfigEditorv3::~GameConfigEditorv3() { delete ui; }

void GameConfigEditorv3::load(QString filename)
{
    if (filename != "") {
        Reader reader = Reader(filename);
        gameConfig.read(reader);
        tabTitle = Utils::getFilenameAndFolder(gameConfig.filePath);
        appConfig.addRecentFile(ENGINE_v3, TOOL_GAMECONFIGEDITOR, filename, QList<QString>{ /**/ });
    }
    else {
        gameConfig = RSDKv3::GameConfig();
        tabTitle   = "GameConfig Editor";
    }
    clearActions();

    setupUI();
}

void GameConfigEditorv3::setupUI(bool allowRowChange)
{
    if (allowRowChange) {
        ui->sectionList->blockSignals(true);
        ui->sectionList->setCurrentRow(-1);
        ui->sectionList->blockSignals(false);
        ui->sectionList->setCurrentRow(0);
    }

    ui->gameName->blockSignals(true);
    ui->gameDescription->blockSignals(true);
    ui->objList->blockSignals(true);
    ui->sfxList->blockSignals(true);
    ui->varList->blockSignals(true);
    ui->plrList->blockSignals(true);

    ui->gameName->setText(gameConfig.gameWindowText);
    ui->gameDescription->setPlainText(gameConfig.gameDescriptionText);

    int prevIndex = ui->objList->currentRow();
    ui->objList->setCurrentRow(0);
    ui->objList->setCurrentRow(prevIndex >= ui->objList->count() ? -1 : prevIndex);

    prevIndex = ui->sfxList->currentRow();
    ui->sfxList->setCurrentRow(0);
    ui->sfxList->setCurrentRow(prevIndex >= ui->sfxList->count() ? -1 : prevIndex);

    prevIndex = ui->varList->currentRow();
    ui->varList->setCurrentRow(0);
    ui->varList->setCurrentRow(prevIndex >= ui->varList->count() ? -1 : prevIndex);

    prevIndex = ui->plrList->currentRow();
    ui->plrList->setCurrentRow(0);
    ui->plrList->setCurrentRow(prevIndex >= ui->plrList->count() ? -1 : prevIndex);

    ui->objList->clear();
    int id = 0;
    for (RSDKv3::GameConfig::ObjectInfo &obj : gameConfig.objects) {
        ui->objList->addItem(obj.name);
        ui->objList->item(id)->setFlags(ui->objList->item(id)->flags() | Qt::ItemIsEditable);
        id++;
    }

    ui->sfxList->clear();
    id = 0;
    for (QString &sfx : gameConfig.soundFX) {
        ui->sfxList->addItem(sfx);
        ui->sfxList->item(id)->setFlags(ui->sfxList->item(id)->flags() | Qt::ItemIsEditable);
        id++;
    }

    ui->varList->clear();
    id = 0;
    for (RSDKv3::GameConfig::GlobalVariable &var : gameConfig.globalVariables) {
        ui->varList->addItem(var.name);
        ui->varList->item(id)->setFlags(ui->varList->item(id)->flags() | Qt::ItemIsEditable);
        id++;
    }

    ui->plrList->clear();
    id = 0;
    for (QString &plr : gameConfig.players) {
        ui->plrList->addItem(plr.toUpper());
        ui->plrList->item(id)->setFlags(ui->plrList->item(id)->flags() | Qt::ItemIsEditable);
        id++;
    }

    sceneModel->clear();
    QList<QString> names = {
        "Presentation Stages",
        "Regular Stages",
        "Special Stages",
        "Bonus Stages",
    };
    for (int c = 0; c < 4; ++c) {
        auto *catItem = new QStandardItem(names[c]);

        for (auto &scn : gameConfig.stageLists[c].scenes) {
            auto *scnItem = new QStandardItem(scn.name);
            catItem->appendRow(scnItem);

            scnItem->setFlags(scnItem->flags() | Qt::ItemIsEditable);
        }

        catItem->setFlags(catItem->flags() & ~Qt::ItemIsEditable);
        sceneModel->appendRow(catItem);
    }

    ui->gameName->blockSignals(false);
    ui->gameDescription->blockSignals(false);
    ui->objList->blockSignals(false);
    ui->sfxList->blockSignals(false);
    ui->varList->blockSignals(false);
    ui->plrList->blockSignals(false);
}

bool GameConfigEditorv3::event(QEvent *event)
{

    switch ((int)event->type()) {
        default: break;
        case RE_EVENT_NEW: load(""); return true;
        case RE_EVENT_OPEN: {
            QFileDialog filedialog(this, tr("Open GameConfig"), "",
                                   tr("RSDKv3 GameConfig files (GameConfig*.bin)"));
            filedialog.setAcceptMode(QFileDialog::AcceptOpen);
            if (filedialog.exec() == QDialog::Accepted) {
                setStatus("Loading GameConfig...", true);
                QString basePath = filedialog.selectedFiles()[0].replace(
                    QFileInfo(filedialog.selectedFiles()[0]).fileName(), "");
                load(filedialog.selectedFiles()[0]);

                setStatus("Loaded GameConfig " + tabTitle);

                return true;
            }
            break;
        }
        case RE_EVENT_SAVE:
            if (gameConfig.filePath.isEmpty()) {
                QFileDialog filedialog(this, tr("Open GameConfig"), "",
                                       tr("RSDKv3 GameConfig files (GameConfig*.bin)"));
                filedialog.setAcceptMode(QFileDialog::AcceptSave);
                if (filedialog.exec() == QDialog::Accepted) {

                    setStatus("Saving GameConfig...", true);

                    appConfig.addRecentFile(ENGINE_v3, TOOL_GAMECONFIGEDITOR,
                                            filedialog.selectedFiles()[0], QList<QString>{ /**/ });
                    Writer writer(filedialog.selectedFiles()[0]);
                    gameConfig.write(writer);
                    clearActions();
                    setStatus("Saved GameConfig to " + filedialog.selectedFiles()[0]);

                    return true;
                }
            }
            else {
                setStatus("Saving GameConfig...", true);
                gameConfig.write("");
                setStatus("Saved GameConfig to " + gameConfig.filePath);
                clearActions();
                return true;
            }
            break;
        case RE_EVENT_SAVE_AS: {
            QList<QString> types = {
                "RSDKv3 GameConfig files (GameConfig*.bin)",
                "RSDKv2 GameConfig files (GameConfig*.bin)",
                "RSDKv4 GameConfig files (GameConfig*.bin)",
                "RSDKv5 rev02 (plus) GameConfig files (GameConfig*.bin)",
                "RSDKv5 rev01 (pre-plus) GameConfig files (GameConfig*.bin)",
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
                    case 0: {
                        setStatus("Saving GameConfig...", true);

                        appConfig.addRecentFile(ENGINE_v4, TOOL_GAMECONFIGEDITOR,
                                                filedialog.selectedFiles()[0], QList<QString>{ /**/ });

                        gameConfig.write(filedialog.selectedFiles()[0]);
                        setStatus("Saved GameConfig to " + filedialog.selectedFiles()[0]);

                        break;
                    }
                    case 1: {
                        setStatus("Converting GameConfig to v2...", true);
                        RSDKv2::GameConfig config;

                        config.gameWindowText      = gameConfig.gameWindowText;
                        config.gameDescriptionText = gameConfig.gameDescriptionText;

                        config.globalVariables.clear();
                        for (auto &var : gameConfig.globalVariables) {
                            RSDKv2::GameConfig::GlobalVariable variable;
                            variable.name  = var.name;
                            variable.value = var.value;
                            config.globalVariables.append(variable);
                        }
                        addStatusProgress(1.f / 5);

                        config.objects.clear();
                        for (auto &obj : gameConfig.objects) {
                            config.objects.append(obj.script);
                        }
                        addStatusProgress(1.f / 5);

                        config.soundFX.clear();
                        for (auto &sfx : gameConfig.soundFX) {
                            config.soundFX.append(sfx);
                        }
                        addStatusProgress(1.f / 5);

                        config.players.clear();
                        for (auto &plr : gameConfig.players) {
                            RSDKv2::GameConfig::PlayerInfo player;
                            player.name = plr;
                            config.players.append(player);
                        }
                        addStatusProgress(1.f / 5);

                        config.stageLists.clear();
                        for (auto &cat : gameConfig.stageLists) {
                            RSDKv2::GameConfig::Category category;
                            category.scenes.clear();

                            for (auto &scn : cat.scenes) {
                                RSDKv2::GameConfig::SceneInfo scene;
                                scene.name        = scn.name;
                                scene.folder      = scn.folder;
                                scene.id          = scn.id;
                                scene.highlighted = scn.highlighted;
                                category.scenes.append(scene);
                            }
                            config.stageLists.append(category);
                        }
                        setStatus("Saving GameConfig...", true);

                        appConfig.addRecentFile(ENGINE_v2, TOOL_GAMECONFIGEDITOR,
                                                filedialog.selectedFiles()[0], QList<QString>{ /**/ });
                        config.write(filedialog.selectedFiles()[0]);
                        setStatus("Saved v2 GameConfig to " + filedialog.selectedFiles()[0]);

                        break;
                    }
                    case 2: {
                        setStatus("Converting GameConfig to v4...", true);
                        RSDKv4::GameConfig config;

                        config.gameWindowText      = gameConfig.gameWindowText;
                        config.gameDescriptionText = gameConfig.gameDescriptionText;

                        config.globalVariables.clear();
                        for (auto &var : gameConfig.globalVariables) {
                            RSDKv4::GameConfig::GlobalVariable variable;
                            variable.name  = var.name;
                            variable.value = var.value;
                            config.globalVariables.append(variable);
                        }
                        addStatusProgress(1.f / 6);

                        config.objects.clear();
                        for (auto &obj : gameConfig.objects) {
                            RSDKv4::GameConfig::ObjectInfo object;
                            object.name   = obj.name;
                            object.script = obj.script;
                            config.objects.append(object);
                        }
                        addStatusProgress(1.f / 6);

                        config.soundFX.clear();
                        for (auto &sfx : gameConfig.soundFX) {
                            RSDKv4::GameConfig::SoundInfo soundFX;
                            soundFX.name = QFileInfo(sfx).baseName();
                            soundFX.path = sfx;
                            config.soundFX.append(soundFX);
                        }
                        addStatusProgress(1.f / 6);

                        config.players.clear();
                        for (auto &plr : gameConfig.players) {
                            config.players.append(plr);
                        }
                        addStatusProgress(1.f / 6);

                        config.palette.colors.clear();
                        for (int i = 0; i < 96; ++i) {
                            config.palette.colors.append(Color(0x00, 0x00, 0x00));
                        }
                        addStatusProgress(1.f / 6);

                        config.stageLists.clear();
                        for (auto &cat : gameConfig.stageLists) {
                            RSDKv4::GameConfig::Category category;
                            category.scenes.clear();

                            for (auto &scn : cat.scenes) {
                                RSDKv4::GameConfig::SceneInfo scene;
                                scene.name        = scn.name;
                                scene.folder      = scn.folder;
                                scene.id          = scn.id;
                                scene.highlighted = scn.highlighted;
                                category.scenes.append(scene);
                            }
                            config.stageLists.append(category);
                        }

                        setStatus("Saving GameConfig...", true);

                        appConfig.addRecentFile(ENGINE_v4, TOOL_GAMECONFIGEDITOR,
                                                filedialog.selectedFiles()[0], QList<QString>{ /**/ });
                        config.write(filedialog.selectedFiles()[0]);
                        setStatus("Saved v4 GameConfig to " + filedialog.selectedFiles()[0]);
                        break;
                    }
                    case 3:
                    case 4: {
                        setStatus("Converting GameConfig to v5 GCF and RSDKConfig...", true);
                        RSDKv5::GameConfig config;
                        RSDKv5::RSDKConfig rsdkConfig;

                        config.gameTitle  = gameConfig.gameWindowText;
                        config.readFilter = true;

                        config.globalVariables.clear();
                        rsdkConfig.variables.clear();
                        int varID = 0;
                        for (auto &var : gameConfig.globalVariables) {
                            RSDKv5::GameConfig::GlobalVariable variable;
                            RSDKv5::RSDKConfig::Variable rsdkVar;

                            rsdkVar.name = var.name;
                            rsdkVar.name.replace(".", "_");
                            rsdkVar.type   = "int";
                            rsdkVar.value  = "-";
                            variable.index = varID;
                            if (var.value) {
                                variable.values.append(var.value);
                                rsdkVar.value = QString::number(var.value);
                            }
                            config.globalVariables.append(variable);
                            rsdkConfig.variables.append(rsdkVar);
                            ++varID;
                        }
                        addStatusProgress(1.f / 4);

                        config.objects.clear();
                        for (auto &obj : gameConfig.objects) {
                            QString name = obj.name;
                            config.objects.append(name.replace(" ", ""));
                        }
                        addStatusProgress(1.f / 4);

                        config.soundFX.clear();
                        for (auto &sfx : gameConfig.soundFX) {
                            RSDKv5::GameConfig::SoundInfo soundFX;
                            soundFX.path              = sfx;
                            soundFX.maxConcurrentPlay = 1;
                            config.soundFX.append(soundFX);
                        }
                        addStatusProgress(1.f / 4);

                        for (int i = 0; i < 8; ++i) {
                            for (int r = 0; r < 16; ++r) config.palettes[i].activeRows[r] = false;
                        }

                        config.categories.clear();
                        QList<QString> catNames = { "Presentation", "Regular Stages", "Bonus Stages",
                                                    "Special Stages" };
                        int catID               = 0;
                        for (auto &cat : gameConfig.stageLists) {
                            RSDKv5::GameConfig::Category category;
                            category.name = catNames[catID];
                            category.scenes.clear();

                            for (auto &scn : cat.scenes) {
                                RSDKv5::GameConfig::SceneInfo scene;
                                scene.name   = scn.name;
                                scene.folder = scn.folder;
                                scene.id     = scn.id;
                                scene.filter = 0;
                                category.scenes.append(scene);
                            }
                            config.categories.append(category);
                            ++catID;
                        }

                        config.readFilter = filter == 3;
                        appConfig.addRecentFile(
                            ENGINE_v5, TOOL_GAMECONFIGEDITOR, filedialog.selectedFiles()[0],
                            QList<QString>{ "GameConfig", filter == 4 ? "rev01" : "rev02" });
                        setStatus("Saving GameConfig...", true);

                        config.write(filedialog.selectedFiles()[0]);
                        setStatus("Saving RSDKConfig...", true);

                        rsdkConfig.write(filedialog.selectedFiles()[0].toLower().replace("gameconfig",
                                                                                         "RSDKConfig"));
                        setStatus("Saved v5 GFC and RSDKConfig to " + filedialog.selectedFiles()[0]
                                      + " and RSDKConfig.bin",
                                  true);

                        break;
                    }
                    case 5: {
                        setStatus("Saving Game.xml...", true);
                        Writer writer(filedialog.selectedFiles()[0]);

                        writer.writeLine("<?xml version=\"1.0\"?>");

                        writer.writeLine();
                        writer.writeLine("<game>");
                        if (gameConfig.globalVariables.count()) {
                            writer.writeLine();
                            writer.writeLine("\t<variables>");
                            for (auto &var : gameConfig.globalVariables) {
                                writer.writeLine(
                                    QString("\t\t<variable name=\"%1\" value=\"%2\"> </variable>")
                                        .arg(var.name)
                                        .arg(var.value));
                            }
                            writer.writeLine("\t</variables>");
                        }
                        addStatusProgress(1.f / 5);

                        if (gameConfig.objects.count()) {
                            writer.writeLine();
                            writer.writeLine("\t<objects>");
                            for (auto &obj : gameConfig.objects) {
                                writer.writeLine(QString("\t<object name=\"%1\" script=\"%2\" "
                                                         "forceLoad=\"false\"> </object>")
                                                     .arg(obj.name)
                                                     .arg(obj.script));
                            }
                            writer.writeLine("\t</objects>");
                        }
                        addStatusProgress(1.f / 5);

                        if (gameConfig.soundFX.count()) {
                            writer.writeLine();
                            writer.writeLine("\t<sounds>");
                            for (auto &sfx : gameConfig.soundFX) {
                                writer.writeLine(
                                    QString("\t\t<soundfx path=\"%1\"> </soundfx>").arg(sfx));
                            }
                            writer.writeLine("\t</sounds>");
                        }
                        addStatusProgress(1.f / 5);

                        if (gameConfig.players.count()) {
                            writer.writeLine();
                            writer.writeLine("\t<players>");
                            for (auto &plr : gameConfig.players) {
                                writer.writeLine(
                                    QString("\t\t<player name=\"%1\"> </player>").arg(plr));
                            }
                            writer.writeLine("\t</players>");
                        }
                        addStatusProgress(1.f / 5);

                        QString elementNames[] = { "presentationStages", "regularStages",
                                                   "specialStages", "bonusStages" };
                        for (int i = 0; i < 4; ++i) {
                            if (gameConfig.stageLists[i].scenes.count()) {
                                writer.writeLine();
                                writer.writeLine(QString("\t<%1>").arg(elementNames[i]));
                                for (auto &stg : gameConfig.stageLists[i].scenes) {
                                    writer.writeLine(QString("\t\t<stage name=\"%1\" folder=\"%2\" "
                                                             "id=\"%3\" highlight=\"%4\"> </stage>")
                                                         .arg(stg.name)
                                                         .arg(stg.folder)
                                                         .arg(stg.id)
                                                         .arg(stg.highlighted ? "true" : "false"));
                                }
                                writer.writeLine(QString("\t</%1>").arg(elementNames[i]));
                            }
                        }
                        writer.writeLine("</game>");

                        writer.flush();
                        setStatus("Saved Game.xml to " + filedialog.selectedFiles()[0]);

                        break;
                    }
                }

                clearActions();
                return true;
            }
            break;
        }
        case RE_EVENT_UNDO: undoAction(); break;
        case RE_EVENT_REDO: redoAction(); break;
        case QEvent::Close:
            if (modified) {
                bool cancelled = false;
                if (MainWindow::showCloseWarning(this, &cancelled)) {
                    if (gameConfig.filePath.isEmpty()) {
                        QFileDialog filedialog(this, tr("Open GameConfig"), "",
                                               tr("RSDKv3 GameConfig files (GameConfig*.bin)"));
                        filedialog.setAcceptMode(QFileDialog::AcceptSave);
                        if (filedialog.exec() == QDialog::Accepted) {

                            setStatus("Saving GameConfig...", true);

                            appConfig.addRecentFile(ENGINE_v3, TOOL_GAMECONFIGEDITOR,
                                                    filedialog.selectedFiles()[0],
                                                    QList<QString>{ /**/ });
                            Writer writer(filedialog.selectedFiles()[0]);
                            gameConfig.write(writer);
                            clearActions();
                            setStatus("Saved GameConfig to " + filedialog.selectedFiles()[0]);
                        }
                    }
                    else {
                        setStatus("Saving GameConfig...", true);
                        gameConfig.write("");
                        setStatus("Saved GameConfig to " + gameConfig.filePath);
                        clearActions();
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

void GameConfigEditorv3::undoAction()
{
    if (actionIndex > 0) {
        // setStatus("Undid Action: " + actions[actionIndex].name);
        actionIndex--;
        resetAction();
    }
}
void GameConfigEditorv3::redoAction()
{
    if (actionIndex + 1 < actions.count()) {
        // setStatus("Redid Action: " + actions[actionIndex].name);
        actionIndex++;
        resetAction();
    }
}
void GameConfigEditorv3::resetAction()
{
    copyConfig(NULL, &actions[actionIndex]);

    setupUI(false);

    updateTitle(actionIndex > 0);
}
void GameConfigEditorv3::doAction(QString name, bool setModified)
{
    ActionState action;

    action.name = name;

    copyConfig(&action, NULL);

    // Actions
    for (int i = actions.count() - 1; i > actionIndex; --i) {
        actions.removeAt(i);
    }

    actions.append(action);
    actionIndex = actions.count() - 1;

    updateTitle(setModified);

    // setStatus("Did Action: " + name);
}
void GameConfigEditorv3::clearActions()
{
    actions.clear();
    actionIndex = 0;
    doAction("Action Setup", false); // first action, cant be undone
}

void GameConfigEditorv3::copyConfig(ActionState *stateDst, ActionState *stateSrc)
{
    RSDKv3::GameConfig *src = stateSrc ? &stateSrc->gameConfig : &gameConfig;
    RSDKv3::GameConfig *dst = stateDst ? &stateDst->gameConfig : &gameConfig;

    dst->filePath = src->filePath;

    dst->gameWindowText      = src->gameWindowText;
    dst->gameDescriptionText = src->gameDescriptionText;
    dst->unknown             = src->unknown;

    dst->objects.clear();
    for (auto obj : src->objects) dst->objects.append(obj);

    dst->soundFX.clear();
    for (auto sfx : src->soundFX) dst->soundFX.append(sfx);

    dst->globalVariables.clear();
    for (auto var : src->globalVariables) dst->globalVariables.append(var);

    dst->players.clear();
    for (auto plr : src->players) dst->players.append(plr);

    dst->stageLists.clear();
    for (auto cat : src->stageLists) {
        RSDKv3::GameConfig::Category category;

        category.scenes.clear();
        for (auto scn : cat.scenes) category.scenes.append(scn);
        dst->stageLists.append(category);
    }
}

#include "moc_gameconfigeditorv3.cpp"
