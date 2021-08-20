#include "includes.hpp"
#include "ui_gameconfigeditorv3.h"

GameconfigEditorv3::GameconfigEditorv3(QString path, QWidget *parent)
    : QWidget(parent), ui(new Ui::GameconfigEditorv3)
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

    connect(ui->gameName, &QLineEdit::textEdited,
            [this](QString s) { m_gameconfig.gameWindowText = s; });
    connect(ui->gameDescription, &QPlainTextEdit::textChanged,
            [this] { m_gameconfig.gameDescriptionText = ui->gameDescription->toPlainText(); });

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

        ui->objName->setText(m_gameconfig.objects[c].name);
        ui->objScript->setText(m_gameconfig.objects[c].script);
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
        m_gameconfig.objects.insert(c, RSDKv3::Gameconfig::ObjectInfo());
        auto *item = new QListWidgetItem();
        item->setText(m_gameconfig.objects[c].name);
        ui->objList->insertItem(c, item);

        item->setFlags(item->flags() | Qt::ItemIsEditable);
        ui->objList->setCurrentItem(item);
    });

    connect(ui->upObj, &QToolButton::clicked, [this] {
        uint c     = ui->objList->currentRow();
        auto *item = ui->objList->takeItem(c);
        m_gameconfig.objects.move(c, c - 1);
        ui->objList->insertItem(c - 1, item);
        ui->objList->setCurrentRow(c - 1);
    });

    connect(ui->downObj, &QToolButton::clicked, [this] {
        uint c     = ui->objList->currentRow();
        auto *item = ui->objList->takeItem(c);
        m_gameconfig.objects.move(c, c + 1);
        ui->objList->insertItem(c + 1, item);
        ui->objList->setCurrentRow(c + 1);
    });

    connect(ui->rmObj, &QToolButton::clicked, [this] {
        int c = ui->objList->currentRow();
        int n = ui->objList->currentRow() == ui->objList->count() - 1 ? c - 1 : c;
        delete ui->objList->item(c);
        m_gameconfig.objects.removeAt(c);
        ui->objList->blockSignals(true);
        ui->objList->setCurrentRow(n);
        ui->objList->blockSignals(false);
    });

    connect(ui->objList, &QListWidget::itemChanged, [this](QListWidgetItem *item) {
        m_gameconfig.objects[ui->objList->row(item)].name = item->text();

        ui->objName->blockSignals(true);
        ui->objName->setText(m_gameconfig.objects[ui->objList->row(item)].name);
        ui->objName->blockSignals(false);
    });

    connect(ui->objName, &QLineEdit::textEdited, [this](QString s) {
        m_gameconfig.objects[ui->objList->currentRow()].name = s;

        ui->objList->item(ui->objList->currentRow())
            ->setText(m_gameconfig.objects[ui->objList->currentRow()].name);
    });
    connect(ui->objScript, &QLineEdit::textEdited,
            [this](QString s) { m_gameconfig.objects[ui->objList->currentRow()].script = s; });

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

        if (c == -1)
            return;

        ui->sfxName->blockSignals(true);
        ui->sfxName->setText(m_gameconfig.soundFX[c]);
        ui->sfxName->blockSignals(false);

        if (ui->downSfx)
            ui->downSfx->setDisabled(c == ui->sfxList->count() - 1);
        if (ui->upSfx)
            ui->upSfx->setDisabled(c == 0);
    });

    connect(ui->addSfx, &QToolButton::clicked, [this] {
        uint c = ui->sfxList->currentRow() + 1;
        m_gameconfig.soundFX.insert(c, "Folder/Sound.wav");
        auto *item = new QListWidgetItem();
        item->setText(m_gameconfig.soundFX[c]);
        ui->sfxList->insertItem(c, item);

        item->setFlags(item->flags() | Qt::ItemIsEditable);
        ui->sfxList->setCurrentItem(item);
    });

    connect(ui->upSfx, &QToolButton::clicked, [this] {
        uint c     = ui->sfxList->currentRow();
        auto *item = ui->sfxList->takeItem(c);
        m_gameconfig.soundFX.move(c, c - 1);
        ui->sfxList->insertItem(c - 1, item);
        ui->sfxList->setCurrentRow(c - 1);
    });

    connect(ui->downSfx, &QToolButton::clicked, [this] {
        uint c     = ui->sfxList->currentRow();
        auto *item = ui->sfxList->takeItem(c);
        m_gameconfig.soundFX.move(c, c + 1);
        ui->sfxList->insertItem(c + 1, item);
        ui->sfxList->setCurrentRow(c + 1);
    });

    connect(ui->rmSfx, &QToolButton::clicked, [this] {
        int c = ui->sfxList->currentRow();
        int n = ui->sfxList->currentRow() == ui->sfxList->count() - 1 ? c - 1 : c;
        delete ui->sfxList->item(c);
        m_gameconfig.soundFX.removeAt(c);
        ui->sfxList->blockSignals(true);
        ui->sfxList->setCurrentRow(n);
        ui->sfxList->blockSignals(false);
    });

    connect(ui->sfxList, &QListWidget::itemChanged, [this](QListWidgetItem *item) {
        m_gameconfig.soundFX[ui->sfxList->row(item)] = item->text();

        ui->sfxName->blockSignals(true);
        ui->sfxName->setText(m_gameconfig.soundFX[ui->sfxList->row(item)]);
        ui->sfxName->blockSignals(false);
    });

    connect(ui->sfxName, &QLineEdit::textEdited, [this](QString s) {
        m_gameconfig.soundFX[ui->sfxList->currentRow()] = s;

        ui->sfxList->item(ui->sfxList->currentRow())
            ->setText(m_gameconfig.soundFX[ui->sfxList->currentRow()]);
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

        ui->varName->setText(m_gameconfig.globalVariables[c].m_name);
        ui->varValue->setValue(m_gameconfig.globalVariables[c].value);

        ui->varName->blockSignals(false);
        ui->varValue->blockSignals(false);

        if (ui->downVar)
            ui->downVar->setDisabled(c == ui->varList->count() - 1);
        if (ui->upVar)
            ui->upVar->setDisabled(c == 0);
    });

    connect(ui->addVar, &QToolButton::clicked, [this] {
        uint c = ui->varList->currentRow() + 1;
        m_gameconfig.globalVariables.insert(c, RSDKv3::Gameconfig::GlobalVariable());
        auto *item = new QListWidgetItem();
        item->setText(m_gameconfig.globalVariables[c].m_name);
        ui->varList->insertItem(c, item);

        item->setFlags(item->flags() | Qt::ItemIsEditable);
        ui->varList->setCurrentItem(item);
    });

    connect(ui->upVar, &QToolButton::clicked, [this] {
        uint c     = ui->varList->currentRow();
        auto *item = ui->varList->takeItem(c);
        m_gameconfig.globalVariables.move(c, c - 1);
        ui->varList->insertItem(c - 1, item);
        ui->varList->setCurrentRow(c - 1);
    });

    connect(ui->downVar, &QToolButton::clicked, [this] {
        uint c     = ui->varList->currentRow();
        auto *item = ui->varList->takeItem(c);
        m_gameconfig.globalVariables.move(c, c + 1);
        ui->varList->insertItem(c + 1, item);
        ui->varList->setCurrentRow(c + 1);
    });

    connect(ui->rmVar, &QToolButton::clicked, [this] {
        int c = ui->varList->currentRow();
        int n = ui->varList->currentRow() == ui->varList->count() - 1 ? c - 1 : c;
        delete ui->varList->item(c);
        m_gameconfig.globalVariables.removeAt(c);
        ui->varList->blockSignals(true);
        ui->varList->setCurrentRow(n);
        ui->varList->blockSignals(false);
    });

    connect(ui->varList, &QListWidget::itemChanged, [this](QListWidgetItem *item) {
        m_gameconfig.globalVariables[ui->varList->row(item)].m_name = item->text();

        ui->varName->blockSignals(true);
        ui->varName->setText(m_gameconfig.globalVariables[ui->varList->row(item)].m_name);
        ui->varName->blockSignals(false);
    });

    connect(ui->varName, &QLineEdit::textEdited, [this](QString s) {
        m_gameconfig.globalVariables[ui->varList->currentRow()].m_name = s;

        ui->varList->item(ui->varList->currentRow())
            ->setText(m_gameconfig.globalVariables[ui->varList->currentRow()].m_name);
    });

    connect(ui->varValue, QOverload<int>::of(&QSpinBox::valueChanged),
            [this](int v) { m_gameconfig.globalVariables[ui->varList->currentRow()].value = v; });

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
        ui->plrName->setText(m_gameconfig.players[c]);
        ui->plrName->blockSignals(false);

        if (ui->downPlr)
            ui->downPlr->setDisabled(c == ui->plrList->count() - 1);
        if (ui->upPlr)
            ui->upPlr->setDisabled(c == 0);
    });

    connect(ui->addPlr, &QToolButton::clicked, [this] {
        uint c = ui->plrList->currentRow() + 1;
        m_gameconfig.players.insert(c, "PLAYER");
        auto *item = new QListWidgetItem();
        item->setText(m_gameconfig.players[c]);
        ui->plrList->insertItem(c, item);

        item->setFlags(item->flags() | Qt::ItemIsEditable);
        ui->plrList->setCurrentItem(item);
    });

    connect(ui->upPlr, &QToolButton::clicked, [this] {
        uint c     = ui->plrList->currentRow();
        auto *item = ui->plrList->takeItem(c);
        m_gameconfig.players.move(c, c - 1);
        ui->plrList->insertItem(c - 1, item);
        ui->plrList->setCurrentRow(c - 1);
    });

    connect(ui->downPlr, &QToolButton::clicked, [this] {
        uint c     = ui->plrList->currentRow();
        auto *item = ui->plrList->takeItem(c);
        m_gameconfig.players.move(c, c + 1);
        ui->plrList->insertItem(c + 1, item);
        ui->plrList->setCurrentRow(c + 1);
    });

    connect(ui->rmPlr, &QToolButton::clicked, [this] {
        int c = ui->plrList->currentRow();
        int n = ui->plrList->currentRow() == ui->plrList->count() - 1 ? c - 1 : c;
        delete ui->plrList->item(c);
        m_gameconfig.players.removeAt(c);
        ui->plrList->blockSignals(true);
        ui->plrList->setCurrentRow(n);
        ui->plrList->blockSignals(false);
    });

    connect(ui->plrList, &QListWidget::itemChanged, [this](QListWidgetItem *item) {
        m_gameconfig.players[ui->plrList->row(item)] = item->text().toUpper();

        ui->plrName->blockSignals(true);
        ui->plrName->setText(m_gameconfig.players[ui->plrList->row(item)]);
        ui->plrName->blockSignals(false);
    });

    connect(ui->plrName, &QLineEdit::textEdited, [this](QString s) {
        m_gameconfig.players[ui->plrList->currentRow()] = s.toUpper();

        ui->plrList->item(ui->plrList->currentRow())
            ->setText(m_gameconfig.players[ui->plrList->currentRow()]);
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
            m_gameconfig.categories[index.parent().row()].scenes[index.row()].name = item->text();
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
        m_gameconfig.categories[cat].scenes.insert(scn, RSDKv3::Gameconfig::SceneInfo());
        auto *scnItem = new QStandardItem(m_gameconfig.categories[cat].scenes[scn].name);
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
            m_gameconfig.categories[index.parent().row()].scenes.removeAt(index.row());
            m_sceneModel->itemFromIndex(index.parent())->removeRow(index.row());
            return;
        }
    });

    auto moveScn = [this](char translation) {
        bool isScn = ui->scnTree->currentIndex().parent().isValid();
        int c      = ui->scnTree->currentIndex().row();
        QList<QStandardItem *> item;
        if (isScn) {
            m_gameconfig.categories[ui->scnTree->currentIndex().parent().row()].scenes.move(
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

    connect(ui->scnTree->selectionModel(), &QItemSelectionModel::currentChanged,
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
                    ui->scnID->setDisabled(
                        c.row() >= m_gameconfig.categories[c.parent().row()].scenes.count());
                    ui->scnFolder->setDisabled(
                        c.row() >= m_gameconfig.categories[c.parent().row()].scenes.count());
                    ui->scnName->setDisabled(
                        c.row() >= m_gameconfig.categories[c.parent().row()].scenes.count());
                    ui->scnHighlighted->setDisabled(
                        c.row() >= m_gameconfig.categories[c.parent().row()].scenes.count());

                    if (c.row() >= m_gameconfig.categories[c.parent().row()].scenes.count())
                        return;

                    ui->scnID->setText(
                        m_gameconfig.categories[c.parent().row()].scenes[c.row()].actID);
                    ui->scnHighlighted->setChecked(
                        m_gameconfig.categories[c.parent().row()].scenes[c.row()].highlighted);
                    ui->scnFolder->setText(
                        m_gameconfig.categories[c.parent().row()].scenes[c.row()].folder);
                    ui->scnName->setText(
                        m_gameconfig.categories[c.parent().row()].scenes[c.row()].name);

                    connect(ui->scnName, &QLineEdit::textEdited, [this, c](QString s) {
                        m_gameconfig.categories[c.parent().row()].scenes[c.row()].name = s;

                        // TODO: edit text
                    });

                    connect(ui->scnFolder, &QLineEdit::textEdited, [this, c](QString s) {
                        m_gameconfig.categories[c.parent().row()].scenes[c.row()].folder = s;
                    });

                    connect(ui->scnID, &QLineEdit::textEdited, [this, c](QString s) {
                        m_gameconfig.categories[c.parent().row()].scenes[c.row()].folder = s;
                    });

                    connect(ui->scnHighlighted, &QCheckBox::toggled, [this, c](bool v) {
                        m_gameconfig.categories[c.parent().row()].scenes[c.row()].highlighted = v;
                    });
                }

                ui->downScn->setDisabled(
                    c.parent().isValid()
                        ? c.row() == m_gameconfig.categories[c.parent().row()].scenes.count() - 1
                        : c.row() == m_gameconfig.categories.count() - 1);
                ui->upScn->setDisabled(c.row() == 0);
            });

    load(path);
}

GameconfigEditorv3::~GameconfigEditorv3() { delete ui; }

void GameconfigEditorv3::load(QString filename)
{
    ui->sectionList->blockSignals(true);
    ui->sectionList->setCurrentRow(-1);
    ui->sectionList->blockSignals(false);
    ui->sectionList->setCurrentRow(0);

    if (filename != "") {
        Reader reader = Reader(filename);
        m_gameconfig.read(reader);
    }
    else {
        m_gameconfig = RSDKv3::Gameconfig();
    }

    ui->gameName->setText(m_gameconfig.gameWindowText);
    ui->gameDescription->setPlainText(m_gameconfig.gameDescriptionText);

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
    for (RSDKv3::Gameconfig::ObjectInfo &obj : m_gameconfig.objects) {
        ui->objList->addItem(obj.name);
        ui->objList->item(id)->setFlags(ui->objList->item(id)->flags() | Qt::ItemIsEditable);
        id++;
    }

    ui->sfxList->clear();
    id = 0;
    for (QString &sfx : m_gameconfig.soundFX) {
        ui->sfxList->addItem(sfx);
        ui->sfxList->item(id)->setFlags(ui->sfxList->item(id)->flags() | Qt::ItemIsEditable);
        id++;
    }

    ui->varList->clear();
    id = 0;
    for (RSDKv3::Gameconfig::GlobalVariable &var : m_gameconfig.globalVariables) {
        ui->varList->addItem(var.m_name);
        ui->varList->item(id)->setFlags(ui->varList->item(id)->flags() | Qt::ItemIsEditable);
        id++;
    }

    ui->plrList->clear();
    id = 0;
    for (QString &plr : m_gameconfig.players) {
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

        for (auto &scn : m_gameconfig.categories[c].scenes) {
            auto *scnItem = new QStandardItem(scn.name);
            catItem->appendRow(scnItem);

            scnItem->setFlags(scnItem->flags() | Qt::ItemIsEditable);
        }

        catItem->setFlags(catItem->flags() & ~Qt::ItemIsEditable);
        m_sceneModel->appendRow(catItem);
    }
}

bool GameconfigEditorv3::event(QEvent *event)
{
    if (event->type() == (QEvent::Type)RE_EVENT_NEW) {
        load("");
        return true;
    }
    if (event->type() == (QEvent::Type)RE_EVENT_OPEN) {
        QFileDialog filedialog(this, tr("Open GameConfig"), "",
                               tr("RSDKv3 GameConfig files (GameConfig*.bin)"));
        filedialog.setAcceptMode(QFileDialog::AcceptOpen);
        if (filedialog.exec() == QDialog::Accepted) {
            QString basePath = filedialog.selectedFiles()[0].replace(
                QFileInfo(filedialog.selectedFiles()[0]).fileName(), "");
            load(filedialog.selectedFiles()[0]);

            appConfig.addRecentFile(ENGINE_v3, TOOL_GAMECONFIGEDITOR, filedialog.selectedFiles()[0],
                                    QList<QString>{ /**/ });
            setStatus("Loading GameConfig: " + QFileInfo(filedialog.selectedFiles()[0]).fileName());

            return true;
        }
    }

    if (event->type() == (QEvent::Type)RE_EVENT_SAVE) {
        if (!QFile(m_gameconfig.m_filename).exists()) {
            QFileDialog filedialog(this, tr("Open GameConfig"), "",
                                   tr("RSDKv3 GameConfig files (GameConfig*.bin)"));
            filedialog.setAcceptMode(QFileDialog::AcceptSave);
            if (filedialog.exec() == QDialog::Accepted) {

                setStatus("Saving GameConfig: " + filedialog.selectedFiles()[0]);

                appConfig.addRecentFile(ENGINE_v3, TOOL_GAMECONFIGEDITOR, filedialog.selectedFiles()[0],
                                        QList<QString>{ /**/ });
                Writer writer(filedialog.selectedFiles()[0]);
                m_gameconfig.write(writer);

                return true;
            }
        }
        else {
            m_gameconfig.write("");
            return true;
        }
    }

    if (event->type() == (QEvent::Type)RE_EVENT_SAVE_AS) {
        QFileDialog filedialog(this, tr("Open GameConfig"), "",
                               tr("RSDKv3 GameConfig files (GameConfig*.bin)"));
        filedialog.setAcceptMode(QFileDialog::AcceptSave);
        if (filedialog.exec() == QDialog::Accepted) {

            setStatus("Saving GameConfig: " + filedialog.selectedFiles()[0]);

            appConfig.addRecentFile(ENGINE_v3, TOOL_GAMECONFIGEDITOR, filedialog.selectedFiles()[0],
                                    QList<QString>{ /**/ });
            Writer writer(filedialog.selectedFiles()[0]);
            m_gameconfig.write(writer);

            return true;
        }
    }
    return QWidget::event(event);
}

#include "moc_gameconfigeditorv3.cpp"
