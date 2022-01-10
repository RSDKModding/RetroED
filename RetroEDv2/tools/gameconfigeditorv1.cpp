#include "includes.hpp"
#include "ui_gameconfigeditorv1.h"

GameconfigEditorv1::GameconfigEditorv1(QString path, QWidget *parent)
    : QWidget(parent), ui(new Ui::GameconfigEditorv1)
{
    ui->setupUi(this);

    ui->sectionList->blockSignals(true);
    ui->sectionList->setCurrentRow(-1);
    ui->sectionList->blockSignals(false);
    ui->sectionList->setCurrentRow(0);

    ui->addPlr->setIcon(icon_add);
    ui->upPlr->setIcon(icon_up);
    ui->downPlr->setIcon(icon_down);
    ui->rmPlr->setIcon(icon_rm);

    ui->addScn->setIcon(icon_add);
    ui->upScn->setIcon(icon_up);
    ui->downScn->setIcon(icon_down);
    ui->rmScn->setIcon(icon_rm);

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
        ui->plrName->setText(characters.players[c].displayName);
        ui->plrName->blockSignals(false);

        ui->plrAnim->blockSignals(true);
        ui->plrAnim->setText(characters.players[c].player1Anim);
        ui->plrAnim->blockSignals(false);

        ui->hasP2->blockSignals(true);
        ui->hasP2->setChecked(characters.players[c].hasP2);
        ui->hasP2->blockSignals(false);

        ui->plr2Anim->blockSignals(true);
        ui->plr2Anim->setReadOnly(characters.players[c].hasP2);
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
        updateTitle(true);
    });

    connect(ui->upPlr, &QToolButton::clicked, [this] {
        uint c     = ui->plrList->currentRow();
        auto *item = ui->plrList->takeItem(c);
        characters.players.move(c, c - 1);
        ui->plrList->insertItem(c - 1, item);
        ui->plrList->setCurrentRow(c - 1);
        updateTitle(true);
    });

    connect(ui->downPlr, &QToolButton::clicked, [this] {
        uint c     = ui->plrList->currentRow();
        auto *item = ui->plrList->takeItem(c);
        characters.players.move(c, c + 1);
        ui->plrList->insertItem(c + 1, item);
        ui->plrList->setCurrentRow(c + 1);
        updateTitle(true);
    });

    connect(ui->rmPlr, &QToolButton::clicked, [this] {
        int c = ui->plrList->currentRow();
        int n = ui->plrList->currentRow() == ui->plrList->count() - 1 ? c - 1 : c;
        delete ui->plrList->item(c);
        characters.players.removeAt(c);
        ui->plrList->blockSignals(true);
        ui->plrList->setCurrentRow(n);
        ui->plrList->blockSignals(false);
        updateTitle(true);
    });

    connect(ui->plrList, &QListWidget::itemChanged, [this](QListWidgetItem *item) {
        characters.players[ui->plrList->row(item)].displayName = item->text().toUpper();

        ui->plrNameDisplay->blockSignals(true);
        ui->plrNameDisplay->setText(characters.players[ui->plrList->row(item)].displayName);
        ui->plrNameDisplay->blockSignals(false);
        updateTitle(true);
    });

    connect(ui->plrNameDisplay, &QLineEdit::textEdited, [this](QString s) {
        characters.players[ui->plrList->currentRow()].displayName = s.toUpper();

        ui->plrList->item(ui->plrList->currentRow())
            ->setText(characters.players[ui->plrList->currentRow()].displayName);
        updateTitle(true);
    });

    connect(ui->plrName, &QLineEdit::textEdited, [this](QString s) {
        characters.players[ui->plrList->currentRow()].playerName = s.toUpper();
        updateTitle(true);
    });

    connect(ui->plrAnim, &QLineEdit::textEdited, [this](QString s) {
        characters.players[ui->plrList->currentRow()].player1Anim = s;
        updateTitle(true);
    });

    connect(ui->plr2Anim, &QLineEdit::textEdited, [this](QString s) {
        characters.players[ui->plrList->currentRow()].player2Anim = s;
        updateTitle(true);
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
            updateTitle(true);
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
        updateTitle(true);
    });

    connect(ui->rmScn, &QToolButton::clicked, [this] {
        const QModelIndex &index = ui->scnTree->currentIndex();

        if (index.parent().isValid()) { // Scene
            stageList[index.parent().row()].stages.removeAt(index.row());
            sceneModel->itemFromIndex(index.parent())->removeRow(index.row());
            updateTitle(true);
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
            updateTitle(true);
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

            ui->addScn->setDisabled(!c.parent().isValid());
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
                    updateTitle(true);
                });

                connect(ui->scnFolder, &QLineEdit::textEdited, [this, c](QString s) {
                    stageList[c.parent().row()].stages[c.row()].folder = s;
                    updateTitle(true);
                });

                connect(ui->scnID, &QLineEdit::textEdited, [this, c](QString s) {
                    stageList[c.parent().row()].stages[c.row()].folder = s;
                    updateTitle(true);
                });

                connect(ui->scnHighlighted, &QCheckBox::toggled, [this, c](bool v) {
                    stageList[c.parent().row()].stages[c.row()].highlighted = v;
                    updateTitle(true);
                });
            }

            ui->downScn->setDisabled(c.parent().isValid()
                                         ? c.row() == stageList[c.parent().row()].stages.count() - 1
                                         : c.row() == 3);
            ui->upScn->setDisabled(c.row() == 0);
        });

    load(path);
}

GameconfigEditorv1::~GameconfigEditorv1() { delete ui; }

void GameconfigEditorv1::load(QString filename)
{
    ui->sectionList->blockSignals(true);
    ui->sectionList->setCurrentRow(-1);
    ui->sectionList->blockSignals(false);
    ui->sectionList->setCurrentRow(0);

    if (filename != "") {
        Reader reader = Reader(filename);
        characters.read(reader);
        tabTitle = Utils::getFilenameAndFolder(characters.filePath);
    }
    else {
        characters = RSDKv1::CharacterList();
        tabTitle   = "GameConfig Editor";
    }
    updateTitle(false);

    ui->plrList->setCurrentRow(0);
    ui->plrList->setCurrentRow(-1);

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
}

bool GameconfigEditorv1::event(QEvent *event)
{
    if (event->type() == (QEvent::Type)RE_EVENT_NEW) {
        load("");
        return true;
    }
    if (event->type() == (QEvent::Type)RE_EVENT_OPEN) {
        QFileDialog filedialog(this, tr("Open Game Configuration"), "",
                               tr("RSDKv1 Character List files (Characters*.mdf)"));
        filedialog.setAcceptMode(QFileDialog::AcceptOpen);
        if (filedialog.exec() == QDialog::Accepted) {
            QString basePath = filedialog.selectedFiles()[0].replace(
                QFileInfo(filedialog.selectedFiles()[0]).fileName(), "");
            load(filedialog.selectedFiles()[0]);

            appConfig.addRecentFile(ENGINE_v1, TOOL_GAMECONFIGEDITOR, filedialog.selectedFiles()[0],
                                    QList<QString>{ /**/ });
            setStatus("Loading GameConfig: " + QFileInfo(filedialog.selectedFiles()[0]).fileName());

            return true;
        }
    }

    if (event->type() == (QEvent::Type)RE_EVENT_SAVE) {
        if (!QFile(characters.filePath).exists()) {
            QFileDialog filedialog(this, tr("Open Game Configuration"), "",
                                   tr("RSDKv1 Character List files (Characters*.mdf)"));
            filedialog.setAcceptMode(QFileDialog::AcceptSave);
            if (filedialog.exec() == QDialog::Accepted) {

                setStatus("Saving GameConfig: " + filedialog.selectedFiles()[0]);

                appConfig.addRecentFile(ENGINE_v1, TOOL_GAMECONFIGEDITOR, filedialog.selectedFiles()[0],
                                        QList<QString>{ /**/ });
                Writer writer(filedialog.selectedFiles()[0]);
                characters.write(writer);
                updateTitle(false);

                return true;
            }
        }
        else {
            characters.write("");
            updateTitle(false);
            return true;
        }
    }

    if (event->type() == (QEvent::Type)RE_EVENT_SAVE_AS) {
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
                    setStatus("Saving Game Configuration: " + filedialog.selectedFiles()[0]);

                    appConfig.addRecentFile(ENGINE_v1, TOOL_GAMECONFIGEDITOR,
                                            filedialog.selectedFiles()[0], QList<QString>{ /**/ });

                    characters.write(filedialog.selectedFiles()[0]);
                    break;
                }
            }

            updateTitle(false);
            return true;
        }
    }

    if (event->type() == QEvent::Close && modified) {
        bool cancelled = false;
        if (MainWindow::showCloseWarning(this, &cancelled)) {
            if (!QFile(characters.filePath).exists()) {
                QFileDialog filedialog(this, tr("Open Game Configuration"), "",
                                       tr("RSDKv1 Character List files (Characters*.mdf)"));
                filedialog.setAcceptMode(QFileDialog::AcceptSave);
                if (filedialog.exec() == QDialog::Accepted) {

                    setStatus("Saving Game Configuration: " + filedialog.selectedFiles()[0]);

                    appConfig.addRecentFile(ENGINE_v1, TOOL_GAMECONFIGEDITOR,
                                            filedialog.selectedFiles()[0], QList<QString>{ /**/ });
                    Writer writer(filedialog.selectedFiles()[0]);
                    characters.write(writer);
                    updateTitle(false);
                }
            }
            else {
                characters.write("");
                updateTitle(false);
            }
        }
        else if (cancelled) {
            event->ignore();
            return true;
        }
    }

    return QWidget::event(event);
}

#include "moc_gameconfigeditorv1.cpp"
