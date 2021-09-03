#include "includes.hpp"
#include "ui_gameconfigeditorv4.h"

GameconfigEditorv4::GameconfigEditorv4(QString path, QWidget *parent)
    : QWidget(parent), ui(new Ui::GameconfigEditorv4)
{
    ui->setupUi(this);

    ui->sectionList->blockSignals(true);
    ui->sectionList->setCurrentRow(-1);
    ui->sectionList->blockSignals(false);
    ui->sectionList->setCurrentRow(0);

    ui->addObj->setIcon(icon_add);
    ui->upObj->setIcon(icon_up);
    ui->downObj->setIcon(icon_down);
    ui->rmObj->setIcon(icon_rm);

    ui->addSfx->setIcon(icon_add);
    ui->upSfx->setIcon(icon_up);
    ui->downSfx->setIcon(icon_down);
    ui->rmSfx->setIcon(icon_rm);

    ui->addVar->setIcon(icon_add);
    ui->upVar->setIcon(icon_up);
    ui->downVar->setIcon(icon_down);
    ui->rmVar->setIcon(icon_rm);

    ui->addPlr->setIcon(icon_add);
    ui->upPlr->setIcon(icon_up);
    ui->downPlr->setIcon(icon_down);
    ui->rmPlr->setIcon(icon_rm);

    ui->addScn->setIcon(icon_add);
    ui->upScn->setIcon(icon_up);
    ui->downScn->setIcon(icon_down);
    ui->rmScn->setIcon(icon_rm);

    // ----------------
    // DETAILS
    // ----------------

    connect(ui->gameName, &QLineEdit::textEdited, [this](QString s) { gameConfig.gameWindowText = s; });
    connect(ui->gameDescription, &QPlainTextEdit::textChanged,
            [this] { gameConfig.gameDescriptionText = ui->gameDescription->toPlainText(); });

    connect(ui->sectionList, &QListWidget::currentRowChanged, [this](int v) {
        ui->detailsWidget->setCurrentIndex(v);
        ui->sectionLabel->setText(ui->sectionList->item(v)->text());
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
        gameConfig.objects.insert(c, RSDKv4::Gameconfig::ObjectInfo());
        auto *item = new QListWidgetItem();
        item->setText(gameConfig.objects[c].name);
        ui->objList->insertItem(c, item);

        item->setFlags(item->flags() | Qt::ItemIsEditable);
        ui->objList->setCurrentItem(item);
    });

    connect(ui->upObj, &QToolButton::clicked, [this] {
        uint c     = ui->objList->currentRow();
        auto *item = ui->objList->takeItem(c);
        gameConfig.objects.move(c, c - 1);
        ui->objList->insertItem(c - 1, item);
        ui->objList->setCurrentRow(c - 1);
    });

    connect(ui->downObj, &QToolButton::clicked, [this] {
        uint c     = ui->objList->currentRow();
        auto *item = ui->objList->takeItem(c);
        gameConfig.objects.move(c, c + 1);
        ui->objList->insertItem(c + 1, item);
        ui->objList->setCurrentRow(c + 1);
    });

    connect(ui->rmObj, &QToolButton::clicked, [this] {
        int c = ui->objList->currentRow();
        int n = ui->objList->currentRow() == ui->objList->count() - 1 ? c - 1 : c;
        delete ui->objList->item(c);
        gameConfig.objects.removeAt(c);
        ui->objList->blockSignals(true);
        ui->objList->setCurrentRow(n);
        ui->objList->blockSignals(false);
    });

    connect(ui->objList, &QListWidget::itemChanged, [this](QListWidgetItem *item) {
        gameConfig.objects[ui->objList->row(item)].name = item->text();

        ui->objName->blockSignals(true);
        ui->objName->setText(gameConfig.objects[ui->objList->row(item)].name);
        ui->objName->blockSignals(false);
    });

    connect(ui->objName, &QLineEdit::textEdited, [this](QString s) {
        gameConfig.objects[ui->objList->currentRow()].name = s;

        ui->objList->item(ui->objList->currentRow())
            ->setText(gameConfig.objects[ui->objList->currentRow()].name);
    });
    connect(ui->objScript, &QLineEdit::textEdited,
            [this](QString s) { gameConfig.objects[ui->objList->currentRow()].script = s; });

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
        ui->sfxPath->setDisabled(c == -1);

        ui->sfxID->setText("SoundFX ID: ");

        if (c == -1)
            return;

        ui->sfxName->blockSignals(true);
        ui->sfxName->setText(gameConfig.soundFX[c].name);
        ui->sfxName->blockSignals(false);

        ui->sfxPath->blockSignals(true);
        ui->sfxPath->setText(gameConfig.soundFX[c].path);
        ui->sfxPath->blockSignals(false);

        ui->sfxID->setText("SoundFX ID: " + QString::number(c));

        if (ui->downSfx)
            ui->downSfx->setDisabled(c == ui->sfxList->count() - 1);
        if (ui->upSfx)
            ui->upSfx->setDisabled(c == 0);
    });

    connect(ui->addSfx, &QToolButton::clicked, [this] {
        uint c = ui->sfxList->currentRow() + 1;
        RSDKv4::Gameconfig::SoundInfo sfx;
        gameConfig.soundFX.insert(c, sfx);
        auto *item = new QListWidgetItem();
        item->setText(gameConfig.soundFX[c].name);
        ui->sfxList->insertItem(c, item);

        item->setFlags(item->flags() | Qt::ItemIsEditable);
        ui->sfxList->setCurrentItem(item);
    });

    connect(ui->upSfx, &QToolButton::clicked, [this] {
        uint c     = ui->sfxList->currentRow();
        auto *item = ui->sfxList->takeItem(c);
        gameConfig.soundFX.move(c, c - 1);
        ui->sfxList->insertItem(c - 1, item);
        ui->sfxList->setCurrentRow(c - 1);
    });

    connect(ui->downSfx, &QToolButton::clicked, [this] {
        uint c     = ui->sfxList->currentRow();
        auto *item = ui->sfxList->takeItem(c);
        gameConfig.soundFX.move(c, c + 1);
        ui->sfxList->insertItem(c + 1, item);
        ui->sfxList->setCurrentRow(c + 1);
    });

    connect(ui->rmSfx, &QToolButton::clicked, [this] {
        int c = ui->sfxList->currentRow();
        int n = ui->sfxList->currentRow() == ui->sfxList->count() - 1 ? c - 1 : c;
        delete ui->sfxList->item(c);
        gameConfig.soundFX.removeAt(c);
        ui->sfxList->blockSignals(true);
        ui->sfxList->setCurrentRow(n);
        ui->sfxList->blockSignals(false);
    });

    connect(ui->sfxList, &QListWidget::itemChanged, [this](QListWidgetItem *item) {
        gameConfig.soundFX[ui->sfxList->row(item)].name = item->text();

        ui->sfxName->blockSignals(true);
        ui->sfxName->setText(gameConfig.soundFX[ui->sfxList->row(item)].name);
        ui->sfxName->blockSignals(false);
    });

    connect(ui->sfxName, &QLineEdit::textEdited, [this](QString s) {
        gameConfig.soundFX[ui->sfxList->currentRow()].name = s;

        ui->sfxList->item(ui->sfxList->currentRow())
            ->setText(gameConfig.soundFX[ui->sfxList->currentRow()].name);
    });

    connect(ui->sfxPath, &QLineEdit::textEdited,
            [this](QString s) { gameConfig.soundFX[ui->sfxList->currentRow()].path = s; });

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
        gameConfig.globalVariables.insert(c, RSDKv4::Gameconfig::GlobalVariable());
        auto *item = new QListWidgetItem();
        item->setText(gameConfig.globalVariables[c].name);
        ui->varList->insertItem(c, item);

        item->setFlags(item->flags() | Qt::ItemIsEditable);
        ui->varList->setCurrentItem(item);
    });

    connect(ui->upVar, &QToolButton::clicked, [this] {
        uint c     = ui->varList->currentRow();
        auto *item = ui->varList->takeItem(c);
        gameConfig.globalVariables.move(c, c - 1);
        ui->varList->insertItem(c - 1, item);
        ui->varList->setCurrentRow(c - 1);
    });

    connect(ui->downVar, &QToolButton::clicked, [this] {
        uint c     = ui->varList->currentRow();
        auto *item = ui->varList->takeItem(c);
        gameConfig.globalVariables.move(c, c + 1);
        ui->varList->insertItem(c + 1, item);
        ui->varList->setCurrentRow(c + 1);
    });

    connect(ui->rmVar, &QToolButton::clicked, [this] {
        int c = ui->varList->currentRow();
        int n = ui->varList->currentRow() == ui->varList->count() - 1 ? c - 1 : c;
        delete ui->varList->item(c);
        gameConfig.globalVariables.removeAt(c);
        ui->varList->blockSignals(true);
        ui->varList->setCurrentRow(n);
        ui->varList->blockSignals(false);
    });

    connect(ui->varList, &QListWidget::itemChanged, [this](QListWidgetItem *item) {
        gameConfig.globalVariables[ui->varList->row(item)].name = item->text();

        ui->varName->blockSignals(true);
        ui->varName->setText(gameConfig.globalVariables[ui->varList->row(item)].name);
        ui->varName->blockSignals(false);
    });

    connect(ui->varName, &QLineEdit::textEdited, [this](QString s) {
        gameConfig.globalVariables[ui->varList->currentRow()].name = s;

        ui->varList->item(ui->varList->currentRow())
            ->setText(gameConfig.globalVariables[ui->varList->currentRow()].name);
    });

    connect(ui->varValue, QOverload<int>::of(&QSpinBox::valueChanged),
            [this](int v) { gameConfig.globalVariables[ui->varList->currentRow()].value = v; });

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
    });

    connect(ui->upPlr, &QToolButton::clicked, [this] {
        uint c     = ui->plrList->currentRow();
        auto *item = ui->plrList->takeItem(c);
        gameConfig.players.move(c, c - 1);
        ui->plrList->insertItem(c - 1, item);
        ui->plrList->setCurrentRow(c - 1);
    });

    connect(ui->downPlr, &QToolButton::clicked, [this] {
        uint c     = ui->plrList->currentRow();
        auto *item = ui->plrList->takeItem(c);
        gameConfig.players.move(c, c + 1);
        ui->plrList->insertItem(c + 1, item);
        ui->plrList->setCurrentRow(c + 1);
    });

    connect(ui->rmPlr, &QToolButton::clicked, [this] {
        int c = ui->plrList->currentRow();
        int n = ui->plrList->currentRow() == ui->plrList->count() - 1 ? c - 1 : c;
        delete ui->plrList->item(c);
        gameConfig.players.removeAt(c);
        ui->plrList->blockSignals(true);
        ui->plrList->setCurrentRow(n);
        ui->plrList->blockSignals(false);
    });

    connect(ui->plrList, &QListWidget::itemChanged, [this](QListWidgetItem *item) {
        gameConfig.players[ui->plrList->row(item)] = item->text().toUpper();

        ui->plrName->blockSignals(true);
        ui->plrName->setText(gameConfig.players[ui->plrList->row(item)]);
        ui->plrName->blockSignals(false);
    });

    connect(ui->plrName, &QLineEdit::textEdited, [this](QString s) {
        gameConfig.players[ui->plrList->currentRow()] = s.toUpper();

        ui->plrList->item(ui->plrList->currentRow())
            ->setText(gameConfig.players[ui->plrList->currentRow()]);
    });

    // ----------------
    // SCENES
    // ----------------
    if (m_sceneModel) {
        delete m_sceneModel;
        m_sceneModel = nullptr;
    }
    m_sceneModel = new QStandardItemModel(ui->scnTree);

    connect(m_sceneModel, &QStandardItemModel::itemChanged, [this](QStandardItem *item) {
        const QModelIndex &index = m_sceneModel->indexFromItem(item);
        if (index.parent().isValid()) { // Scene
            gameConfig.categories[index.parent().row()].scenes[index.row()].name = item->text();
            return;
        }
        // m_gameconfig.m_categories[index.row()].m_name = item->text();
    });

    connect(ui->addScn, &QToolButton::clicked, [this] {
        const QModelIndex &index = ui->scnTree->currentIndex();
        bool scnSelected         = index.parent().isValid();
        if (!index.isValid()) {
            return;
        }
        uint cat = scnSelected ? index.parent().row() : index.row();
        uint scn = scnSelected ? index.row() + 1 : 0;
        gameConfig.categories[cat].scenes.insert(scn, RSDKv4::Gameconfig::SceneInfo());
        auto *scnItem = new QStandardItem(gameConfig.categories[cat].scenes[scn].name);
        if (scnSelected)
            m_sceneModel->itemFromIndex(index.parent())->insertRow(scn, scnItem);
        else
            m_sceneModel->itemFromIndex(index)->insertRow(scn, scnItem);
        scnItem->setFlags(scnItem->flags() | Qt::ItemIsEditable);
        ui->scnTree->setCurrentIndex(m_sceneModel->indexFromItem(scnItem));
    });

    connect(ui->rmScn, &QToolButton::clicked, [this] {
        const QModelIndex &index = ui->scnTree->currentIndex();

        if (index.parent().isValid()) { // Scene
            gameConfig.categories[index.parent().row()].scenes.removeAt(index.row());
            m_sceneModel->itemFromIndex(index.parent())->removeRow(index.row());
            return;
        }
    });

    auto moveScn = [this](char translation) {
        bool isScn = ui->scnTree->currentIndex().parent().isValid();
        int c      = ui->scnTree->currentIndex().row();
        QList<QStandardItem *> item;
        if (isScn) {
            gameConfig.categories[ui->scnTree->currentIndex().parent().row()].scenes.move(
                c, c + translation);
            QStandardItem *parentItem =
                m_sceneModel->itemFromIndex(ui->scnTree->currentIndex().parent());
            item = parentItem->takeRow(c);
            parentItem->insertRow(c + translation, item);
        }
        else
            return;
        ui->scnTree->setCurrentIndex(m_sceneModel->indexFromItem(item.first()));
    };

    connect(ui->upScn, &QToolButton::clicked, [moveScn] { moveScn(-1); });

    connect(ui->downScn, &QToolButton::clicked, [moveScn] { moveScn(1); });

    ui->scnTree->setModel(m_sceneModel);

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

            ui->addScn->setDisabled(!c.parent().isValid());
            ui->upScn->setDisabled(!c.parent().isValid());
            ui->downScn->setDisabled(!c.parent().isValid());
            ui->rmScn->setDisabled(!c.parent().isValid());

            if (!c.isValid())
                return;

            if (c.parent().isValid()) {
                ui->scnID->setDisabled(c.row()
                                       >= gameConfig.categories[c.parent().row()].scenes.count());
                ui->scnFolder->setDisabled(c.row()
                                           >= gameConfig.categories[c.parent().row()].scenes.count());
                ui->scnName->setDisabled(c.row()
                                         >= gameConfig.categories[c.parent().row()].scenes.count());
                ui->scnHighlighted->setDisabled(
                    c.row() >= gameConfig.categories[c.parent().row()].scenes.count());

                if (c.row() >= gameConfig.categories[c.parent().row()].scenes.count())
                    return;

                ui->scnID->setText(gameConfig.categories[c.parent().row()].scenes[c.row()].id);
                ui->scnHighlighted->setChecked(
                    gameConfig.categories[c.parent().row()].scenes[c.row()].highlighted);
                ui->scnFolder->setText(gameConfig.categories[c.parent().row()].scenes[c.row()].folder);
                ui->scnName->setText(gameConfig.categories[c.parent().row()].scenes[c.row()].name);

                connect(ui->scnName, &QLineEdit::textEdited, [this, c](QString s) {
                    gameConfig.categories[c.parent().row()].scenes[c.row()].name = s;

                    // TODO: edit text
                });

                connect(ui->scnFolder, &QLineEdit::textEdited, [this, c](QString s) {
                    gameConfig.categories[c.parent().row()].scenes[c.row()].folder = s;
                });

                connect(ui->scnID, &QLineEdit::textEdited, [this, c](QString s) {
                    gameConfig.categories[c.parent().row()].scenes[c.row()].folder = s;
                });

                connect(ui->scnHighlighted, &QCheckBox::toggled, [this, c](bool v) {
                    gameConfig.categories[c.parent().row()].scenes[c.row()].highlighted = v;
                });
            }

            ui->downScn->setDisabled(
                c.parent().isValid()
                    ? c.row() == gameConfig.categories[c.parent().row()].scenes.count() - 1
                    : c.row() == gameConfig.categories.count() - 1);
            ui->upScn->setDisabled(c.row() == 0);
        });

    load(path);
}

GameconfigEditorv4::~GameconfigEditorv4() { delete ui; }

void GameconfigEditorv4::load(QString filename)
{
    ui->sectionList->blockSignals(true);
    ui->sectionList->setCurrentRow(-1);
    ui->sectionList->blockSignals(false);
    ui->sectionList->setCurrentRow(0);

    if (filename != "") {
        Reader reader = Reader(filename);
        gameConfig.read(reader);
    }
    else {
        gameConfig = RSDKv4::Gameconfig();
    }

    ui->gameName->setText(gameConfig.gameWindowText);
    ui->gameDescription->setPlainText(gameConfig.gameDescriptionText);

    ui->objList->setCurrentRow(0);
    ui->objList->setCurrentRow(-1);

    ui->sfxList->setCurrentRow(0);
    ui->sfxList->setCurrentRow(-1);

    ui->varList->setCurrentRow(0);
    ui->varList->setCurrentRow(-1);

    ui->plrList->setCurrentRow(0);
    ui->plrList->setCurrentRow(-1);

    ui->objList->clear();
    int id = 0;
    for (RSDKv4::Gameconfig::ObjectInfo &obj : gameConfig.objects) {
        ui->objList->addItem(obj.name);
        ui->objList->item(id)->setFlags(ui->objList->item(id)->flags() | Qt::ItemIsEditable);
        id++;
    }

    ui->sfxList->clear();
    id = 0;
    for (RSDKv4::Gameconfig::SoundInfo &sfx : gameConfig.soundFX) {
        ui->sfxList->addItem(sfx.name);
        ui->sfxList->item(id)->setFlags(ui->sfxList->item(id)->flags() | Qt::ItemIsEditable);
        id++;
    }

    ui->varList->clear();
    id = 0;
    for (RSDKv4::Gameconfig::GlobalVariable &var : gameConfig.globalVariables) {
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

    m_sceneModel->clear();
    QList<QString> names = {
        "Presentation Stages",
        "Regular Stages",
        "Special Stages",
        "Bonus Stages",
    };
    for (int c = 0; c < 4; ++c) {
        auto *catItem = new QStandardItem(names[c]);

        for (auto &scn : gameConfig.categories[c].scenes) {
            auto *scnItem = new QStandardItem(scn.name);
            catItem->appendRow(scnItem);

            scnItem->setFlags(scnItem->flags() | Qt::ItemIsEditable);
        }

        catItem->setFlags(catItem->flags() & ~Qt::ItemIsEditable);
        m_sceneModel->appendRow(catItem);
    }
}

bool GameconfigEditorv4::event(QEvent *event)
{
    if (event->type() == (QEvent::Type)RE_EVENT_NEW) {
        load("");
        return true;
    }
    if (event->type() == (QEvent::Type)RE_EVENT_OPEN) {
        QFileDialog filedialog(this, tr("Open GameConfig"), "",
                               tr("RSDKv4 GameConfig files (GameConfig*.bin)"));
        filedialog.setAcceptMode(QFileDialog::AcceptOpen);
        if (filedialog.exec() == QDialog::Accepted) {
            QString basePath = filedialog.selectedFiles()[0].replace(
                QFileInfo(filedialog.selectedFiles()[0]).fileName(), "");
            load(filedialog.selectedFiles()[0]);

            appConfig.addRecentFile(ENGINE_v4, TOOL_GAMECONFIGEDITOR, filedialog.selectedFiles()[0],
                                    QList<QString>{ /**/ });
            setStatus("Loading GameConfig: " + QFileInfo(filedialog.selectedFiles()[0]).fileName());

            return true;
        }
    }

    if (event->type() == (QEvent::Type)RE_EVENT_SAVE) {
        if (!QFile(gameConfig.m_filename).exists()) {
            QFileDialog filedialog(this, tr("Save GameConfig"), "",
                                   tr("RSDKv4 GameConfig files (GameConfig*.bin)"));
            filedialog.setAcceptMode(QFileDialog::AcceptSave);
            if (filedialog.exec() == QDialog::Accepted) {

                setStatus("Saving GameConfig: " + filedialog.selectedFiles()[0]);

                appConfig.addRecentFile(ENGINE_v4, TOOL_GAMECONFIGEDITOR, filedialog.selectedFiles()[0],
                                        QList<QString>{ /**/ });
                Writer writer(filedialog.selectedFiles()[0]);
                gameConfig.write(writer);

                return true;
            }
        }
        else {
            gameConfig.write("");
            return true;
        }
    }

    if (event->type() == (QEvent::Type)RE_EVENT_SAVE_AS) {
        QList<QString> types = {
            "RSDKv4 GameConfig files (GameConfig*.bin)",
            "RSDKv2 GameConfig files (GameConfig*.bin)",
            "RSDKv3 GameConfig files (GameConfig*.bin)",
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
                    setStatus("Saving GameConfig: " + filedialog.selectedFiles()[0]);

                    appConfig.addRecentFile(ENGINE_v4, TOOL_GAMECONFIGEDITOR,
                                            filedialog.selectedFiles()[0], QList<QString>{ /**/ });

                    gameConfig.write(filedialog.selectedFiles()[0]);
                    break;
                }
                case 1: {
                    setStatus("Saving (v2) GameConfig: " + filedialog.selectedFiles()[0]);
                    RSDKv2::Gameconfig config;

                    config.gameWindowText      = gameConfig.gameWindowText;
                    config.gameDescriptionText = gameConfig.gameDescriptionText;

                    config.globalVariables.clear();
                    for (auto &var : gameConfig.globalVariables) {
                        RSDKv2::Gameconfig::GlobalVariable variable;
                        variable.name  = var.name;
                        variable.value = var.value;
                        config.globalVariables.append(variable);
                    }

                    config.scripts.clear();
                    for (auto &obj : gameConfig.objects) {
                        config.scripts.append(obj.script);
                    }

                    config.soundFX.clear();
                    for (auto &sfx : gameConfig.soundFX) {
                        config.soundFX.append(sfx.name);
                    }

                    config.players.clear();
                    for (auto &plr : gameConfig.players) {
                        RSDKv2::Gameconfig::PlayerInfo player;
                        player.name = plr;
                        config.players.append(player);
                    }

                    config.categories.clear();
                    for (auto &cat : gameConfig.categories) {
                        RSDKv2::Gameconfig::Category category;
                        category.scenes.clear();

                        for (auto &scn : cat.scenes) {
                            RSDKv2::Gameconfig::SceneInfo scene;
                            scene.name        = scn.name;
                            scene.folder      = scn.folder;
                            scene.id          = scn.id;
                            scene.highlighted = scn.highlighted;
                            category.scenes.append(scene);
                        }
                        config.categories.append(category);
                    }

                    appConfig.addRecentFile(ENGINE_v2, TOOL_GAMECONFIGEDITOR,
                                            filedialog.selectedFiles()[0], QList<QString>{ /**/ });
                    config.write(filedialog.selectedFiles()[0]);
                    break;
                }
                case 2: {
                    setStatus("Saving (v3) GameConfig: " + filedialog.selectedFiles()[0]);
                    RSDKv3::Gameconfig config;

                    config.gameWindowText      = gameConfig.gameWindowText;
                    config.gameDescriptionText = gameConfig.gameDescriptionText;

                    config.globalVariables.clear();
                    for (auto &var : gameConfig.globalVariables) {
                        RSDKv3::Gameconfig::GlobalVariable variable;
                        variable.name  = var.name;
                        variable.value = var.value;
                        config.globalVariables.append(variable);
                    }

                    config.objects.clear();
                    for (auto &obj : gameConfig.objects) {
                        RSDKv3::Gameconfig::ObjectInfo object;
                        object.name   = obj.name;
                        object.script = obj.script;
                        config.objects.append(object);
                    }

                    config.soundFX.clear();
                    for (auto &sfx : gameConfig.soundFX) {
                        config.soundFX.append(sfx.name);
                    }

                    config.players.clear();
                    for (auto &plr : gameConfig.players) {
                        config.players.append(plr);
                    }

                    config.categories.clear();
                    for (auto &cat : gameConfig.categories) {
                        RSDKv3::Gameconfig::Category category;
                        category.scenes.clear();

                        for (auto &scn : cat.scenes) {
                            RSDKv3::Gameconfig::SceneInfo scene;
                            scene.name        = scn.name;
                            scene.folder      = scn.folder;
                            scene.id          = scn.id;
                            scene.highlighted = scn.highlighted;
                            category.scenes.append(scene);
                        }
                        config.categories.append(category);
                    }

                    appConfig.addRecentFile(ENGINE_v3, TOOL_GAMECONFIGEDITOR,
                                            filedialog.selectedFiles()[0], QList<QString>{ /**/ });
                    config.write(filedialog.selectedFiles()[0]);
                    break;
                }
                case 3:
                case 4: {
                    setStatus("Saving (v5) GameConfig: " + filedialog.selectedFiles()[0]);
                    RSDKv5::GameConfig config;
                    RSDKv5::RSDKConfig rsdkConfig;

                    config.gameTitle = gameConfig.gameWindowText;
                    config.readMode  = true;

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

                    config.objects.clear();
                    for (auto &obj : gameConfig.objects) {
                        QString name = obj.name;
                        config.objects.append(name.replace(" ", ""));
                    }

                    config.soundFX.clear();
                    for (auto &sfx : gameConfig.soundFX) {
                        RSDKv5::GameConfig::SoundInfo soundFX;
                        soundFX.path              = sfx.path;
                        soundFX.maxConcurrentPlay = 1;
                        config.soundFX.append(soundFX);
                    }

                    for (int i = 0; i < 6; ++i) {
                        config.palettes[0].activeRows[i] = true;
                        for (int c = 0; c < 16; ++c) {
                            QColor clr(gameConfig.palette.colours[(i * 16) + c].r,
                                       gameConfig.palette.colours[(i * 16) + c].g,
                                       gameConfig.palette.colours[(i * 16) + c].b);
                            config.palettes[0].colours[i][c] = clr;
                        }
                    }
                    config.palettes[0].activeRows[6]  = false;
                    config.palettes[0].activeRows[7]  = false;
                    config.palettes[0].activeRows[8]  = false;
                    config.palettes[0].activeRows[9]  = false;
                    config.palettes[0].activeRows[10] = false;
                    config.palettes[0].activeRows[11] = false;
                    config.palettes[0].activeRows[12] = false;
                    config.palettes[0].activeRows[13] = false;
                    config.palettes[0].activeRows[14] = false;
                    config.palettes[0].activeRows[15] = false;

                    for (int i = 1; i < 8; ++i) {
                        for (int r = 0; r < 16; ++r) config.palettes[i].activeRows[r] = false;
                    }

                    config.categories.clear();
                    QList<QString> catNames = { "Presentation", "Regular Stages", "Bonus Stages",
                                                "Special Stages" };
                    int catID               = 0;
                    for (auto &cat : gameConfig.categories) {
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

                    config.readMode = filter == 3;
                    appConfig.addRecentFile(
                        ENGINE_v5, TOOL_GAMECONFIGEDITOR, filedialog.selectedFiles()[0],
                        QList<QString>{ "GameConfig", filter == 4 ? "rev01" : "rev02" });
                    config.write(filedialog.selectedFiles()[0]);
                    rsdkConfig.write(
                        filedialog.selectedFiles()[0].toLower().replace("gameconfig", "rsdkconfig"));
                    break;
                }
                case 5: {
                    setStatus("Saving Game.xml: " + filedialog.selectedFiles()[0]);
                    Writer writer(filedialog.selectedFiles()[0]);

                    writer.writeLine("<?xml version=\"1.0\"?>");

                    if (gameConfig.globalVariables.count()) {
                        writer.writeLine();
                        writer.writeLine("<variables>");
                        for (auto &var : gameConfig.globalVariables) {
                            writer.writeLine(
                                QString("\t<variable name=\"%1\" value=\"%2\"> </variable>")
                                    .arg(var.name)
                                    .arg(var.value));
                        }
                        writer.writeLine("</variables>");
                    }

                    if (gameConfig.objects.count()) {
                        writer.writeLine();
                        writer.writeLine("<objects>");
                        for (auto &obj : gameConfig.objects) {
                            writer.writeLine(QString("\t<object name=\"%1\" script=\"%2\" "
                                                     "forceLoad=\"false\"> </object>")
                                                 .arg(obj.name)
                                                 .arg(obj.script));
                        }
                        writer.writeLine("</objects>");
                    }

                    if (gameConfig.soundFX.count()) {
                        writer.writeLine();
                        writer.writeLine("<sounds>");
                        for (auto &sfx : gameConfig.soundFX) {
                            writer.writeLine(QString("\t<soundfx name=\"%1\" path=\"%2\"> </soundfx>")
                                                 .arg(sfx.name)
                                                 .arg(sfx.path));
                        }
                        writer.writeLine("</sounds>");
                    }

                    if (gameConfig.players.count()) {
                        writer.writeLine();
                        writer.writeLine("<players>");
                        for (auto &plr : gameConfig.players) {
                            writer.writeLine(QString("\t<player name=\"%1\"> </player>").arg(plr));
                        }
                        writer.writeLine("</players>");
                    }

                    QString elementNames[] = { "presentationStages", "regularStages", "specialStages",
                                               "bonusStages" };
                    for (int i = 0; i < 4; ++i) {
                        if (gameConfig.categories[i].scenes.count()) {
                            writer.writeLine();
                            writer.writeLine(QString("<%1>").arg(elementNames[i]));
                            for (auto &stg : gameConfig.categories[i].scenes) {
                                writer.writeLine(QString("\t<stage name=\"%1\" folder=\"%2\" "
                                                         "id=\"%3\" highlight=\"%4\"> </stage>")
                                                     .arg(stg.name)
                                                     .arg(stg.folder)
                                                     .arg(stg.id)
                                                     .arg(stg.highlighted ? "true" : "false"));
                            }
                            writer.writeLine(QString("</%1>").arg(elementNames[i]));
                        }
                    }

                    writer.flush();
                    break;
                }
            }

            return true;
        }
    }
    return QWidget::event(event);
}

#include "moc_gameconfigeditorv4.cpp"
