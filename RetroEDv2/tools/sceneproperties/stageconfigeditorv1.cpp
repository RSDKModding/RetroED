#include "includes.hpp"
#include "ui_stageconfigeditorv1.h"

StageconfigEditorv1::StageconfigEditorv1(FormatHelpers::Stageconfig *scf, QWidget *parent)
    : m_stageconfig(scf), QDialog(parent), ui(new Ui::StageconfigEditorv1)
{
    ui->setupUi(this);

    this->setWindowTitle("Stageconfig Editor");

    ui->objName->setDisabled(true);
    ui->sfxName->setDisabled(true);

    setupUI();

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

    ui->addMus->setIcon(icon_add);
    ui->upMus->setIcon(icon_up);
    ui->downMus->setIcon(icon_down);
    ui->rmMus->setIcon(icon_rm);

    ui->addSht->setIcon(icon_add);
    ui->upSht->setIcon(icon_up);
    ui->downSht->setIcon(icon_down);
    ui->rmSht->setIcon(icon_rm);

    connect(ui->sectionList, &QListWidget::currentRowChanged, [this](int v) {
        ui->detailsWidget->setCurrentIndex(v);
        ui->sectionLabel->setText(ui->sectionList->item(v)->text());
    });

    auto createSheetList = [this] {
        ui->objSheet->blockSignals(true);
        ui->objSheet->clear();

        ui->objSheet->addItem("Data/TitleCard/TitleCard1.gfx");
        ui->objSheet->addItem("[Unknown]");
        ui->objSheet->addItem("Data/Objects/Shields.gfx");
        ui->objSheet->addItem("[Unknown]");
        ui->objSheet->addItem("Data/Objects/General.gfx");
        ui->objSheet->addItem("Data/Objects/General2.gfx");
        for (QString &sheet : m_stageconfig->m_objectSpritesheets) {
            ui->objSheet->addItem("Data/Objects/Gfx/" + sheet);
        }
        ui->objSheet->blockSignals(false);
    };
    createSheetList();

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

        ui->objScript->setDisabled(c == -1);
        ui->objSheet->setDisabled(c == -1);
        ui->objType->setText("Object Type ID: ");

        if (c == -1)
            return;

        ui->objName->blockSignals(true);
        ui->objName->setText(m_stageconfig->objects[c].m_name);
        ui->objName->blockSignals(false);

        ui->objScript->blockSignals(true);
        ui->objScript->setText(m_stageconfig->objects[c].m_script);
        ui->objScript->blockSignals(false);

        ui->objSheet->blockSignals(true);
        ui->objSheet->setCurrentIndex(m_stageconfig->objects[c].m_sheetID);
        ui->objSheet->blockSignals(false);

        ui->objType->setText("Object Type ID: " + QString::number(c + 31));

        if (ui->downObj)
            ui->downObj->setDisabled(c == ui->objList->count() - 1);
        if (ui->upObj)
            ui->upObj->setDisabled(c == 0);
    });

    connect(ui->addObj, &QToolButton::clicked, [this] {
        ui->objList->blockSignals(true);
        uint c = ui->objList->currentRow() + 1;
        m_stageconfig->objects.insert(c, FormatHelpers::Stageconfig::ObjectInfo());
        auto *item = new QListWidgetItem();
        item->setText(m_stageconfig->objects[c].m_name);
        ui->objList->insertItem(c, item);

        item->setFlags(item->flags() | Qt::ItemIsEditable);
        ui->objList->setCurrentItem(item);
        ui->objList->blockSignals(false);
    });

    connect(ui->upObj, &QToolButton::clicked, [this] {
        uint c     = ui->objList->currentRow();
        auto *item = ui->objList->takeItem(c);
        m_stageconfig->objects.move(c, c - 1);
        ui->objList->insertItem(c - 1, item);
        ui->objList->setCurrentRow(c - 1);
    });

    connect(ui->downObj, &QToolButton::clicked, [this] {
        uint c     = ui->objList->currentRow();
        auto *item = ui->objList->takeItem(c);
        m_stageconfig->objects.move(c, c + 1);
        ui->objList->insertItem(c + 1, item);
        ui->objList->setCurrentRow(c + 1);
    });

    connect(ui->rmObj, &QToolButton::clicked, [this] {
        int c = ui->objList->currentRow();
        int n = ui->objList->currentRow() == ui->objList->count() - 1 ? c - 1 : c;
        delete ui->objList->item(c);
        m_stageconfig->objects.removeAt(c);
        ui->objList->blockSignals(true);
        ui->objList->setCurrentRow(n);
        ui->objList->blockSignals(false);
    });

    connect(ui->objList, &QListWidget::itemChanged, [this](QListWidgetItem *item) {
        m_stageconfig->objects[ui->objList->row(item)].m_script = item->text();
        m_stageconfig->objects[ui->objList->row(item)].m_name   = QFileInfo(item->text()).baseName();

        ui->objName->blockSignals(true);
        ui->objName->setText(m_stageconfig->objects[ui->objList->row(item)].m_name);
        ui->objName->blockSignals(false);

        ui->objScript->blockSignals(true);
        ui->objScript->setText(m_stageconfig->objects[ui->objList->currentRow()].m_script);
        ui->objScript->blockSignals(false);
    });

    connect(ui->objScript, &QLineEdit::textEdited,
            [this](QString s) { m_stageconfig->objects[ui->objList->currentRow()].m_script = s; });
    connect(ui->objSheet, QOverload<int>::of(&QComboBox::currentIndexChanged),
            [this](int v) { m_stageconfig->objects[ui->objList->currentRow()].m_sheetID = (byte)v; });

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

        // ui->sfxName->setDisabled(c == -1);
        ui->sfxPath->setDisabled(c == -1);

        if (c == -1)
            return;

        ui->sfxName->blockSignals(true);
        ui->sfxName->setText(m_stageconfig->m_soundFX[c].m_name);
        ui->sfxName->blockSignals(false);

        ui->sfxPath->blockSignals(true);
        ui->sfxPath->setText(m_stageconfig->m_soundFX[c].m_path);
        ui->sfxPath->blockSignals(false);

        if (ui->downSfx)
            ui->downSfx->setDisabled(c == ui->sfxList->count() - 1);
        if (ui->upSfx)
            ui->upSfx->setDisabled(c == 0);
    });

    connect(ui->addSfx, &QToolButton::clicked, [this] {
        ui->sfxList->blockSignals(true);
        uint c = ui->sfxList->currentRow() + 1;
        m_stageconfig->m_soundFX.insert(c, FormatHelpers::Stageconfig::SoundInfo());
        auto *item = new QListWidgetItem();
        item->setText(m_stageconfig->m_soundFX[c].m_path);
        ui->sfxList->insertItem(c, item);

        item->setFlags(item->flags() | Qt::ItemIsEditable);
        ui->sfxList->setCurrentItem(item);
        ui->sfxList->blockSignals(false);
    });

    connect(ui->upSfx, &QToolButton::clicked, [this] {
        uint c     = ui->sfxList->currentRow();
        auto *item = ui->sfxList->takeItem(c);
        m_stageconfig->m_soundFX.move(c, c - 1);
        ui->sfxList->insertItem(c - 1, item);
        ui->sfxList->setCurrentRow(c - 1);
    });

    connect(ui->downSfx, &QToolButton::clicked, [this] {
        uint c     = ui->sfxList->currentRow();
        auto *item = ui->sfxList->takeItem(c);
        m_stageconfig->m_soundFX.move(c, c + 1);
        ui->sfxList->insertItem(c + 1, item);
        ui->sfxList->setCurrentRow(c + 1);
    });

    connect(ui->rmSfx, &QToolButton::clicked, [this] {
        int c = ui->sfxList->currentRow();
        int n = ui->sfxList->currentRow() == ui->sfxList->count() - 1 ? c - 1 : c;
        delete ui->sfxList->item(c);
        m_stageconfig->m_soundFX.removeAt(c);
        ui->sfxList->blockSignals(true);
        ui->sfxList->setCurrentRow(n);
        ui->sfxList->blockSignals(false);
    });

    connect(ui->sfxList, &QListWidget::itemChanged, [this](QListWidgetItem *item) {
        m_stageconfig->m_soundFX[ui->sfxList->row(item)].m_path = item->text();
        m_stageconfig->m_soundFX[ui->sfxList->row(item)].m_name = QFileInfo(item->text()).baseName();

        ui->sfxPath->blockSignals(true);
        ui->sfxPath->setText(m_stageconfig->m_soundFX[ui->sfxList->row(item)].m_path);
        ui->sfxPath->blockSignals(false);

        ui->sfxName->blockSignals(true);
        ui->sfxName->setText(m_stageconfig->m_soundFX[ui->sfxList->currentRow()].m_name);
        ui->sfxName->blockSignals(false);
    });

    connect(ui->sfxPath, &QLineEdit::textEdited, [this](QString s) {
        m_stageconfig->m_soundFX[ui->sfxList->currentRow()].m_path = s;
        m_stageconfig->m_soundFX[ui->sfxList->currentRow()].m_name = QFileInfo(s).baseName();

        ui->sfxList->item(ui->sfxList->currentRow())
            ->setText(m_stageconfig->m_soundFX[ui->sfxList->currentRow()].m_path);

        ui->sfxName->blockSignals(true);
        ui->sfxName->setText(m_stageconfig->m_soundFX[ui->sfxList->currentRow()].m_name);
        ui->sfxName->blockSignals(false);
    });

    // ----------------
    // MUSIC
    // ----------------
    connect(ui->musList, &QListWidget::currentRowChanged, [this](int c) {
        if (ui->upMus)
            ui->upMus->setDisabled(c == -1);
        if (ui->downMus)
            ui->downMus->setDisabled(c == -1);
        if (ui->rmMus)
            ui->rmMus->setDisabled(c == -1);

        ui->musPath->setDisabled(c == -1);

        if (c == -1)
            return;

        ui->musPath->blockSignals(true);
        ui->musPath->setText(m_stageconfig->m_music[c]);
        ui->musPath->blockSignals(false);

        if (ui->downMus)
            ui->downMus->setDisabled(c == ui->musList->count() - 1);
        if (ui->upMus)
            ui->upMus->setDisabled(c == 0);
    });

    connect(ui->addMus, &QToolButton::clicked, [this] {
        ui->musList->blockSignals(true);
        uint c = ui->musList->currentRow() + 1;
        m_stageconfig->m_music.insert(c, "Music.ogg");
        auto *item = new QListWidgetItem();
        item->setText(m_stageconfig->m_music[c]);
        ui->musList->insertItem(c, item);

        item->setFlags(item->flags() | Qt::ItemIsEditable);
        ui->musList->setCurrentItem(item);
        ui->musList->blockSignals(false);
    });

    connect(ui->upMus, &QToolButton::clicked, [this] {
        uint c     = ui->musList->currentRow();
        auto *item = ui->musList->takeItem(c);
        m_stageconfig->m_music.move(c, c - 1);
        ui->musList->insertItem(c - 1, item);
        ui->musList->setCurrentRow(c - 1);
    });

    connect(ui->downMus, &QToolButton::clicked, [this] {
        uint c     = ui->musList->currentRow();
        auto *item = ui->musList->takeItem(c);
        m_stageconfig->m_music.move(c, c + 1);
        ui->musList->insertItem(c + 1, item);
        ui->musList->setCurrentRow(c + 1);
    });

    connect(ui->rmMus, &QToolButton::clicked, [this] {
        int c = ui->musList->currentRow();
        int n = ui->musList->currentRow() == ui->musList->count() - 1 ? c - 1 : c;
        delete ui->musList->item(c);
        m_stageconfig->m_music.removeAt(c);
        ui->musList->blockSignals(true);
        ui->musList->setCurrentRow(n);
        ui->musList->blockSignals(false);
    });

    connect(ui->musList, &QListWidget::itemChanged, [this](QListWidgetItem *item) {
        m_stageconfig->m_music[ui->musList->row(item)] = item->text();

        ui->musPath->blockSignals(true);
        ui->musPath->setText(m_stageconfig->m_music[ui->musList->row(item)]);
        ui->musPath->blockSignals(false);
    });

    connect(ui->musPath, &QLineEdit::textEdited, [this](QString s) {
        m_stageconfig->m_soundFX[ui->musList->currentRow()].m_path = s;

        ui->musList->item(ui->musList->currentRow())
            ->setText(m_stageconfig->m_music[ui->musList->currentRow()]);
    });

    // ----------------
    // SPRITE SHEETS
    // ----------------
    connect(ui->shtList, &QListWidget::currentRowChanged, [this](int c) {
        if (ui->upSht)
            ui->upSht->setDisabled(c == -1);
        if (ui->downSht)
            ui->downSht->setDisabled(c == -1);
        if (ui->rmSht)
            ui->rmSht->setDisabled(c == -1);

        ui->shtPath->setDisabled(c == -1);

        if (c == -1)
            return;

        ui->shtPath->blockSignals(true);
        ui->shtPath->setText(m_stageconfig->m_objectSpritesheets[c]);
        ui->shtPath->blockSignals(false);

        if (ui->downSht)
            ui->downSht->setDisabled(c == ui->shtList->count() - 1);
        if (ui->upSht)
            ui->upSht->setDisabled(c == 0);
    });

    connect(ui->addSht, &QToolButton::clicked, [this, createSheetList] {
        ui->shtList->blockSignals(true);
        uint c = ui->shtList->currentRow() + 1;
        m_stageconfig->m_objectSpritesheets.insert(c, "Sheet.bmp");
        auto *item = new QListWidgetItem();
        item->setText(m_stageconfig->m_objectSpritesheets[c]);
        ui->shtList->insertItem(c, item);

        item->setFlags(item->flags() | Qt::ItemIsEditable);
        ui->shtList->setCurrentItem(item);

        for (FormatHelpers::Stageconfig::ObjectInfo &obj : m_stageconfig->objects) {
            if (obj.m_sheetID >= (c + 6))
                ++obj.m_sheetID;
        }
        createSheetList();
        ui->shtList->blockSignals(false);
    });

    connect(ui->upSht, &QToolButton::clicked, [this, createSheetList] {
        uint c     = ui->shtList->currentRow();
        auto *item = ui->shtList->takeItem(c);
        m_stageconfig->m_objectSpritesheets.move(c, c - 1);
        ui->shtList->insertItem(c - 1, item);
        ui->shtList->setCurrentRow(c - 1);

        for (FormatHelpers::Stageconfig::ObjectInfo &obj : m_stageconfig->objects) {
            if (obj.m_sheetID == (c + 6))
                obj.m_sheetID = (c + 6) - 1;
            else if (obj.m_sheetID == (c + 6) - 1)
                obj.m_sheetID = (c + 6);
        }
        createSheetList();
    });

    connect(ui->downSht, &QToolButton::clicked, [this, createSheetList] {
        uint c     = ui->shtList->currentRow();
        auto *item = ui->shtList->takeItem(c);
        m_stageconfig->m_objectSpritesheets.move(c, c + 1);
        ui->shtList->insertItem(c + 1, item);
        ui->shtList->setCurrentRow(c + 1);

        for (FormatHelpers::Stageconfig::ObjectInfo &obj : m_stageconfig->objects) {
            if (obj.m_sheetID == (c + 6))
                obj.m_sheetID = (c + 6) + 1;
            else if (obj.m_sheetID == (c + 6) + 1)
                obj.m_sheetID = (c + 6);
        }
        createSheetList();
    });

    connect(ui->rmSht, &QToolButton::clicked, [this, createSheetList] {
        int c = ui->shtList->currentRow();
        int n = ui->shtList->currentRow() == ui->shtList->count() - 1 ? c - 1 : c;
        delete ui->shtList->item(c);
        m_stageconfig->m_objectSpritesheets.removeAt(c);
        ui->shtList->blockSignals(true);
        ui->shtList->setCurrentRow(n);
        ui->shtList->blockSignals(false);

        for (FormatHelpers::Stageconfig::ObjectInfo &obj : m_stageconfig->objects) {
            if (obj.m_sheetID > (c + 6))
                --obj.m_sheetID;
            else if (obj.m_sheetID == (c + 6))
                obj.m_sheetID = 4; // general.gfx
        }
        createSheetList();
    });

    connect(ui->shtList, &QListWidget::itemChanged, [this](QListWidgetItem *item) {
        m_stageconfig->m_objectSpritesheets[ui->shtList->row(item)] = item->text();

        ui->shtPath->blockSignals(true);
        ui->shtPath->setText(m_stageconfig->m_objectSpritesheets[ui->shtList->row(item)]);
        ui->shtPath->blockSignals(false);
    });

    connect(ui->shtPath, &QLineEdit::textEdited, [this](QString s) {
        m_stageconfig->m_objectSpritesheets[ui->shtList->currentRow()] = s;

        ui->shtList->item(ui->shtList->currentRow())
            ->setText(m_stageconfig->m_objectSpritesheets[ui->shtList->currentRow()]);
    });
}

StageconfigEditorv1::~StageconfigEditorv1() { delete ui; }

void StageconfigEditorv1::setupUI()
{
    ui->sectionList->blockSignals(true);
    ui->sectionList->setCurrentRow(-1);
    ui->sectionList->blockSignals(false);
    ui->sectionList->setCurrentRow(0);

    ui->objList->setCurrentRow(0);
    ui->objList->setCurrentRow(-1);

    ui->sfxList->setCurrentRow(0);
    ui->sfxList->setCurrentRow(-1);

    ui->musList->setCurrentRow(0);
    ui->musList->setCurrentRow(-1);

    ui->shtList->setCurrentRow(0);
    ui->shtList->setCurrentRow(-1);

    ui->objList->clear();
    int id = 0;
    for (FormatHelpers::Stageconfig::ObjectInfo &obj : m_stageconfig->objects) {
        ui->objList->addItem(obj.m_script);
        ui->objList->item(id)->setFlags(ui->objList->item(id)->flags() | Qt::ItemIsEditable);
        id++;
    }

    ui->sfxList->clear();
    id = 0;
    for (FormatHelpers::Stageconfig::SoundInfo &sfx : m_stageconfig->m_soundFX) {
        ui->sfxList->addItem(sfx.m_path);
        ui->sfxList->item(id)->setFlags(ui->sfxList->item(id)->flags() | Qt::ItemIsEditable);
        id++;
    }

    ui->musList->clear();
    id = 0;
    for (QString &mus : m_stageconfig->m_music) {
        ui->musList->addItem(mus);
        ui->musList->item(id)->setFlags(ui->musList->item(id)->flags() | Qt::ItemIsEditable);
        id++;
    }

    ui->shtList->clear();
    id = 0;
    for (QString &sheet : m_stageconfig->m_objectSpritesheets) {
        ui->shtList->addItem(sheet);
        ui->shtList->item(id)->setFlags(ui->shtList->item(id)->flags() | Qt::ItemIsEditable);
        id++;
    }
}

#include "moc_stageconfigeditorv1.cpp"
