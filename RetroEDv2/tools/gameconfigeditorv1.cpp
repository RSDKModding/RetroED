#include "includes.hpp"
#include "ui_gameconfigeditorv1.h"

GameConfigEditorv1::GameConfigEditorv1(QString path, QWidget *parent)
    : QWidget(parent), ui(new Ui::GameConfigEditorv1)
{
    ui->setupUi(this);

    ui->sectionList->blockSignals(true);
    ui->sectionList->setCurrentRow(-1);
    ui->sectionList->blockSignals(false);
    ui->sectionList->setCurrentRow(0);

    // ----------------
    // DETAILS
    // ----------------
    connect(ui->sectionList, &QListWidget::currentRowChanged, [this](int v) {
        ui->plrList->blockSignals(true);
        ui->plrList->setCurrentRow(0);
        ui->plrList->blockSignals(false);
        ui->plrList->setCurrentRow(-1);

        ui->detailsWidget->setCurrentIndex(v);
        ui->sectionLabel->setText(ui->sectionList->item(v)->text());
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

        ui->plrNameDisplay->blockSignals(true);
        ui->plrNameDisplay->setText(characters.players[c].displayName);
        ui->plrNameDisplay->blockSignals(false);

        ui->plrName->blockSignals(true);
        ui->plrName->setText(characters.players[c].playerName);
        ui->plrName->blockSignals(false);

        ui->plrAnim->blockSignals(true);
        ui->plrAnim->setText(characters.players[c].player1Anim);
        ui->plrAnim->blockSignals(false);

        ui->hasP2->blockSignals(true);
        ui->hasP2->setChecked(characters.players[c].hasP2);
        ui->hasP2->blockSignals(false);

        ui->plr2Anim->blockSignals(true);
        ui->plr2Anim->setDisabled(!characters.players[c].hasP2);
        ui->plr2Anim->setText(characters.players[c].player2Anim);
        ui->plr2Anim->blockSignals(false);

        if (ui->downPlr)
            ui->downPlr->setDisabled(c == ui->plrList->count() - 1);
        if (ui->upPlr)
            ui->upPlr->setDisabled(c == 0);
    });

    connect(ui->addPlr, &QToolButton::clicked, [this] {
        uint c = ui->plrList->currentRow() + 1;
        characters.players.insert(c, RSDKv1::CharacterList::PlayerInfo());
        auto *item = new QListWidgetItem();
        item->setText(characters.players[c].displayName);
        ui->plrList->insertItem(c, item);

        item->setFlags(item->flags() | Qt::ItemIsEditable);
        ui->plrList->setCurrentItem(item);
        DoAction("Added Player");
    });

    connect(ui->upPlr, &QToolButton::clicked, [this] {
        uint c     = ui->plrList->currentRow();
        auto *item = ui->plrList->takeItem(c);
        characters.players.move(c, c - 1);
        ui->plrList->insertItem(c - 1, item);
        ui->plrList->setCurrentRow(c - 1);
        DoAction("Moved Player Up");
    });

    connect(ui->downPlr, &QToolButton::clicked, [this] {
        uint c     = ui->plrList->currentRow();
        auto *item = ui->plrList->takeItem(c);
        characters.players.move(c, c + 1);
        ui->plrList->insertItem(c + 1, item);
        ui->plrList->setCurrentRow(c + 1);
        DoAction("Moved Player Down");
    });

    connect(ui->rmPlr, &QToolButton::clicked, [this] {
        int c = ui->plrList->currentRow();
        int n = ui->plrList->currentRow() == ui->plrList->count() - 1 ? c - 1 : c;
        delete ui->plrList->item(c);
        characters.players.removeAt(c);
        ui->plrList->blockSignals(true);
        ui->plrList->setCurrentRow(n);
        ui->plrList->blockSignals(false);
        DoAction("Removed Player");
    });

    connect(ui->plrList, &QListWidget::itemChanged, [this](QListWidgetItem *item) {
        characters.players[ui->plrList->row(item)].displayName = item->text().toUpper();

        ui->plrNameDisplay->blockSignals(true);
        ui->plrNameDisplay->setText(characters.players[ui->plrList->row(item)].displayName);
        ui->plrNameDisplay->blockSignals(false);
        DoAction("Changed Display Name");
    });

    connect(ui->plrNameDisplay, &QLineEdit::textEdited, [this](QString s) {
        characters.players[ui->plrList->currentRow()].displayName = s.toUpper();

        ui->plrList->item(ui->plrList->currentRow())
            ->setText(characters.players[ui->plrList->currentRow()].displayName);
        DoAction("Changed Display Name");
    });

    connect(ui->plrName, &QLineEdit::textEdited, [this](QString s) {
        characters.players[ui->plrList->currentRow()].playerName = s.toUpper();
        DoAction("Changed Player Name");
    });

    connect(ui->plrAnim, &QLineEdit::textEdited, [this](QString s) {
        characters.players[ui->plrList->currentRow()].player1Anim = s;
        DoAction("Changed Player 1 Anim");
    });

    connect(ui->hasP2, &QCheckBox::toggled, [this](bool c) {
        characters.players[ui->plrList->currentRow()].hasP2 = c;
        ui->plr2Anim->setDisabled(!characters.players[ui->plrList->currentRow()].hasP2);
        DoAction("Changed HasP2");
    });

    connect(ui->plr2Anim, &QLineEdit::textEdited, [this](QString s) {
        characters.players[ui->plrList->currentRow()].player2Anim = s;
        DoAction("Changed Player 2 Anim");
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
            stageList[index.parent().row()].stages[index.row()].name = item->text();
            DoAction("Changed Scene Name");
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
        stageList[cat].stages.insert(scn, RSDKv1::StageList::StageInfo());
        auto *scnItem = new QStandardItem(stageList[cat].stages[scn].name);
        if (scnSelected)
            sceneModel->itemFromIndex(index.parent())->insertRow(scn, scnItem);
        else
            sceneModel->itemFromIndex(index)->insertRow(scn, scnItem);
        scnItem->setFlags(scnItem->flags() | Qt::ItemIsEditable);
        ui->scnTree->setCurrentIndex(sceneModel->indexFromItem(scnItem));
        DoAction("Added Scene");
    });

    connect(ui->rmScn, &QToolButton::clicked, [this] {
        const QModelIndex &index = ui->scnTree->currentIndex();

        if (index.parent().isValid()) { // Scene
            stageList[index.parent().row()].stages.removeAt(index.row());
            sceneModel->itemFromIndex(index.parent())->removeRow(index.row());
            DoAction("Removed Scene");
            return;
        }
    });

    auto moveScn = [this](char translation) {
        bool isScn = ui->scnTree->currentIndex().parent().isValid();

        int c = ui->scnTree->currentIndex().row();
        QList<QStandardItem *> item;
        if (isScn) {
            stageList[ui->scnTree->currentIndex().parent().row()].stages.move(c, c + translation);
            QStandardItem *parentItem = sceneModel->itemFromIndex(ui->scnTree->currentIndex().parent());
            item                      = parentItem->takeRow(c);
            parentItem->insertRow(c + translation, item);
            DoAction("Moved Scene");
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
                ui->scnID->setDisabled(c.row() >= stageList[c.parent().row()].stages.count());
                ui->scnFolder->setDisabled(c.row() >= stageList[c.parent().row()].stages.count());
                ui->scnName->setDisabled(c.row() >= stageList[c.parent().row()].stages.count());
                ui->scnHighlighted->setDisabled(c.row() >= stageList[c.parent().row()].stages.count());

                if (c.row() >= stageList[c.parent().row()].stages.count())
                    return;

                ui->scnID->setText(stageList[c.parent().row()].stages[c.row()].id);
                ui->scnHighlighted->setChecked(stageList[c.parent().row()].stages[c.row()].highlighted);
                ui->scnFolder->setText(stageList[c.parent().row()].stages[c.row()].folder);
                ui->scnName->setText(stageList[c.parent().row()].stages[c.row()].name);

                connect(ui->scnName, &QLineEdit::textEdited, [this, c](QString s) {
                    stageList[c.parent().row()].stages[c.row()].name = s;

                    // TODO: edit text
                    DoAction("Changed Scene Name");
                });

                connect(ui->scnFolder, &QLineEdit::textEdited, [this, c](QString s) {
                    stageList[c.parent().row()].stages[c.row()].folder = s;
                    DoAction("Changed Scene Folder");
                });

                connect(ui->scnID, &QLineEdit::textEdited, [this, c](QString s) {
                    stageList[c.parent().row()].stages[c.row()].id = s;
                    DoAction("Changed Scene ID");
                });

                connect(ui->scnHighlighted, &QCheckBox::toggled, [this, c](bool v) {
                    stageList[c.parent().row()].stages[c.row()].highlighted = v;
                    DoAction("Changed Scene Highlight");
                });
            }

            ui->downScn->setDisabled(c.parent().isValid()
                                         ? c.row() == stageList[c.parent().row()].stages.count() - 1
                                         : c.row() == 3);
            ui->upScn->setDisabled(c.row() == 0);
        });

    load(path);
}

GameConfigEditorv1::~GameConfigEditorv1() { delete ui; }

void GameConfigEditorv1::load(QString filename)
{
    if (filename != "") {
        Reader reader = Reader(filename);
        characters.read(reader);

        QList<QString> listFileNames = { "Zones.mdf", "CZones.mdf", "SStages.mdf", "BStages.mdf" };

        QString baseDir = filename;
        baseDir         = baseDir.replace(QFileInfo(baseDir).fileName(), "");
        for (int l = 0; l < 4; ++l) {
            stageList[l] = RSDKv1::StageList(baseDir + "/" + listFileNames[l]);
        }

        tabTitle = Utils::getFilenameAndFolder(characters.filePath);
        appConfig.addRecentFile(ENGINE_v1, TOOL_GAMECONFIGEDITOR, filename, QList<QString>{ /**/ });
    }
    else {
        characters = RSDKv1::CharacterList();
        tabTitle   = "GameConfig Editor";
    }
    ClearActions();

    setupUI();
}

void GameConfigEditorv1::setupUI(bool allowRowChange)
{
    if (allowRowChange) {
        ui->sectionList->blockSignals(true);
        ui->sectionList->setCurrentRow(-1);
        ui->sectionList->blockSignals(false);
        ui->sectionList->setCurrentRow(0);
    }
    ui->plrList->blockSignals(true);

    int prevIndex = ui->plrList->currentRow();
    ui->plrList->setCurrentRow(0);
    ui->plrList->setCurrentRow(prevIndex >= ui->plrList->count() ? -1 : prevIndex);

    ui->plrList->clear();
    int id = 0;
    for (RSDKv1::CharacterList::PlayerInfo &plr : characters.players) {
        ui->plrList->addItem(plr.displayName.toUpper());
        ui->plrList->item(id)->setFlags(ui->plrList->item(id)->flags() | Qt::ItemIsEditable);
        id++;
    }

    sceneModel->clear();
    for (int c = 0; c < 4; ++c) {
        auto *catItem = new QStandardItem(stageListNames[c]);

        for (auto &scn : stageList[c].stages) {
            auto *scnItem = new QStandardItem(scn.name);
            catItem->appendRow(scnItem);

            scnItem->setFlags(scnItem->flags() | Qt::ItemIsEditable);
        }

        catItem->setFlags(catItem->flags() & ~Qt::ItemIsEditable);
        sceneModel->appendRow(catItem);
    }

    ui->plrList->blockSignals(false);
}

bool GameConfigEditorv1::event(QEvent *event)
{
    QList<QString> listFileNames = { "Zones.mdf", "CZones.mdf", "SStages.mdf", "BStages.mdf" };

    switch ((int)event->type()) {
        default: break;
        case RE_EVENT_NEW: load(""); return true;
        case RE_EVENT_OPEN: {
            QFileDialog filedialog(this, tr("Open Game Configuration"), "",
                                   tr("RSDKv1 Character List files (Characters*.mdf)"));
            filedialog.setAcceptMode(QFileDialog::AcceptOpen);
            if (filedialog.exec() == QDialog::Accepted) {
                QString basePath = filedialog.selectedFiles()[0].replace(
                    QFileInfo(filedialog.selectedFiles()[0]).fileName(), "");
                SetStatus("Loading game configuration...");
                load(filedialog.selectedFiles()[0]);

                return true;
            }
            break;
        }
        case RE_EVENT_SAVE:
            if (!QFile(characters.filePath).exists()) {
                QFileDialog filedialog(this, tr("Open Game Configuration"), "",
                                       tr("RSDKv1 Character List files (Characters*.mdf)"));
                filedialog.setAcceptMode(QFileDialog::AcceptSave);
                if (filedialog.exec() == QDialog::Accepted) {

                    SetStatus("Saving game configuration...", true);

                    appConfig.addRecentFile(ENGINE_v1, TOOL_GAMECONFIGEDITOR,
                                            filedialog.selectedFiles()[0], QList<QString>{ /**/ });
                    Writer writer(filedialog.selectedFiles()[0]);
                    characters.write(writer);
                    AddStatusProgress(.2);

                    QString baseDir = filedialog.selectedFiles()[0];
                    baseDir         = baseDir.replace(QFileInfo(baseDir).fileName(), "");
                    for (int l = 0; l < 4; ++l) {
                        stageList[l].write(baseDir + "/" + listFileNames[l]);
                        AddStatusProgress(.2);
                    }

                    ClearActions();

                    SetStatus("Saved game configuration");

                    return true;
                }
            }
            else {
                characters.write("");

                QString baseDir = characters.filePath;
                baseDir         = baseDir.replace(QFileInfo(baseDir).fileName(), "");
                for (int l = 0; l < 4; ++l) {
                    stageList[l].write(baseDir + "/" + listFileNames[l]);
                }

                ClearActions();
                return true;
            }
            break;
        case RE_EVENT_SAVE_AS: {
            QList<QString> types = {
                "RSDKv1 Character List files (Characters*.mdf)",
            };
            QFileDialog filedialog(this, tr("Save Game Configuration"), "",
                                   tr(QString("%1").arg(types[0]).toStdString().c_str()));
            filedialog.setAcceptMode(QFileDialog::AcceptSave);
            if (filedialog.exec() == QDialog::Accepted) {
                int filter = types.indexOf(filedialog.selectedNameFilter());

                switch (filter) {
                    case 0: {
                        SetStatus("Saving game configuration...", true);

                        appConfig.addRecentFile(ENGINE_v1, TOOL_GAMECONFIGEDITOR,
                                                filedialog.selectedFiles()[0], QList<QString>{ /**/ });

                        characters.write(filedialog.selectedFiles()[0]);
                        AddStatusProgress(.2);

                        QString baseDir = filedialog.selectedFiles()[0];
                        baseDir         = baseDir.replace(QFileInfo(baseDir).fileName(), "");
                        for (int l = 0; l < 4; ++l) {
                            stageList[l].write(baseDir + "/" + listFileNames[l]);
                            AddStatusProgress(.2);
                        }

                        SetStatus("Saved game configuration");

                        break;
                    }
                }

                ClearActions();
                return true;
            }
            break;
        }
        case RE_EVENT_UNDO: UndoAction(); break;
        case RE_EVENT_REDO: RedoAction(); break;
        case QEvent::Close:
            if (modified) {
                bool cancelled = false;
                if (MainWindow::showCloseWarning(this, &cancelled)) {
                    if (!QFile(characters.filePath).exists()) {
                        QFileDialog filedialog(this, tr("Open Game Configuration"), "",
                                               tr("RSDKv1 Character List files (Characters*.mdf)"));
                        filedialog.setAcceptMode(QFileDialog::AcceptSave);
                        if (filedialog.exec() == QDialog::Accepted) {
                            // TODO: you don't save properly here dumbfuck
                            SetStatus("Saving game configuration...");

                            appConfig.addRecentFile(ENGINE_v1, TOOL_GAMECONFIGEDITOR,
                                                    filedialog.selectedFiles()[0],
                                                    QList<QString>{ /**/ });
                            Writer writer(filedialog.selectedFiles()[0]);
                            characters.write(writer);
                            ClearActions();
                        }
                    }
                    else {
                        characters.write("");

                        QString baseDir = characters.filePath;
                        baseDir         = baseDir.replace(QFileInfo(baseDir).fileName(), "");
                        for (int l = 0; l < 4; ++l) {
                            stageList[l].write(baseDir + "/" + listFileNames[l]);
                        }

                        ClearActions();
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

void GameConfigEditorv1::UndoAction()
{
    if (actionIndex > 0) {
        // setStatus("Undid Action: " + actions[actionIndex].name);
        actionIndex--;
        ResetAction();
    }
}
void GameConfigEditorv1::RedoAction()
{
    if (actionIndex + 1 < actions.count()) {
        // setStatus("Redid Action: " + actions[actionIndex].name);
        actionIndex++;
        ResetAction();
    }
}
void GameConfigEditorv1::ResetAction()
{
#if RE_USE_UNSTABLE
    copyConfig(NULL, &actions[actionIndex]);

    setupUI(false);
#endif

    UpdateTitle(actionIndex > 0);
}
void GameConfigEditorv1::DoAction(QString name, bool setModified)
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
void GameConfigEditorv1::ClearActions()
{
    actions.clear();
    actionIndex = 0;
    DoAction("Action Setup", false); // first action, cant be undone
}

void GameConfigEditorv1::copyConfig(ActionState *stateDst, ActionState *stateSrc)
{
    RSDKv1::CharacterList *srcChar = stateSrc ? &stateSrc->characters : &characters;
    RSDKv1::CharacterList *dstChar = stateDst ? &stateDst->characters : &characters;
    RSDKv1::StageList *srcList     = stateSrc ? stateSrc->stageList : stageList;
    RSDKv1::StageList *dstList     = stateDst ? stateDst->stageList : stageList;

    dstChar->filePath = srcChar->filePath;
    for (int l = 0; l < 4; ++l) dstList[l].filePath = srcList[l].filePath;

    dstChar->players.clear();
    for (auto plr : srcChar->players) dstChar->players.append(plr);

    for (int l = 0; l < 4; ++l) {
        dstList[l].stages.clear();
        for (auto scn : srcList[l].stages) dstList[l].stages.append(scn);
    }
}

#include "moc_gameconfigeditorv1.cpp"
