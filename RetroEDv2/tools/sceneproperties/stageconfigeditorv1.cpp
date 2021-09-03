#include "includes.hpp"
#include "ui_stageconfigeditorv1.h"

StageconfigEditorv1::StageconfigEditorv1(FormatHelpers::Stageconfig *scf, int globalSfxCount,
                                         QWidget *parent)
    : stageConfig(scf), QDialog(parent), ui(new Ui::StageconfigEditorv1)
{
    ui->setupUi(this);
    globalSFXCount = globalSfxCount;

    this->setWindowTitle("Stageconfig Editor");

    ui->objName->setDisabled(true);

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
        for (QString &sheet : stageConfig->spriteSheets) {
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
        ui->objName->setText(stageConfig->objects[c].m_name);
        ui->objName->blockSignals(false);

        ui->objScript->blockSignals(true);
        ui->objScript->setText(stageConfig->objects[c].script);
        ui->objScript->blockSignals(false);

        ui->objSheet->blockSignals(true);
        ui->objSheet->setCurrentIndex(stageConfig->objects[c].sheetID);
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
        stageConfig->objects.insert(c, FormatHelpers::Stageconfig::ObjectInfo());
        auto *item = new QListWidgetItem();
        item->setText(stageConfig->objects[c].m_name);
        ui->objList->insertItem(c, item);

        item->setFlags(item->flags() | Qt::ItemIsEditable);
        ui->objList->setCurrentItem(item);
        ui->objList->blockSignals(false);
    });

    connect(ui->upObj, &QToolButton::clicked, [this] {
        uint c     = ui->objList->currentRow();
        auto *item = ui->objList->takeItem(c);
        stageConfig->objects.move(c, c - 1);
        ui->objList->insertItem(c - 1, item);
        ui->objList->setCurrentRow(c - 1);
    });

    connect(ui->downObj, &QToolButton::clicked, [this] {
        uint c     = ui->objList->currentRow();
        auto *item = ui->objList->takeItem(c);
        stageConfig->objects.move(c, c + 1);
        ui->objList->insertItem(c + 1, item);
        ui->objList->setCurrentRow(c + 1);
    });

    connect(ui->rmObj, &QToolButton::clicked, [this] {
        int c = ui->objList->currentRow();
        int n = ui->objList->currentRow() == ui->objList->count() - 1 ? c - 1 : c;
        delete ui->objList->item(c);
        stageConfig->objects.removeAt(c);
        ui->objList->blockSignals(true);
        ui->objList->setCurrentRow(n);
        ui->objList->blockSignals(false);
    });

    connect(ui->objList, &QListWidget::itemChanged, [this](QListWidgetItem *item) {
        stageConfig->objects[ui->objList->row(item)].script = item->text();
        stageConfig->objects[ui->objList->row(item)].m_name = QFileInfo(item->text()).baseName();

        ui->objName->blockSignals(true);
        ui->objName->setText(stageConfig->objects[ui->objList->row(item)].m_name);
        ui->objName->blockSignals(false);

        ui->objScript->blockSignals(true);
        ui->objScript->setText(stageConfig->objects[ui->objList->currentRow()].script);
        ui->objScript->blockSignals(false);
    });

    connect(ui->objScript, &QLineEdit::textEdited,
            [this](QString s) { stageConfig->objects[ui->objList->currentRow()].script = s; });
    connect(ui->objSheet, QOverload<int>::of(&QComboBox::currentIndexChanged),
            [this](int v) { stageConfig->objects[ui->objList->currentRow()].sheetID = (byte)v; });

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

        ui->sfxPath->setDisabled(c == -1);

        ui->sfxID->setText("SoundFX ID: ");

        if (c == -1)
            return;

        ui->sfxPath->blockSignals(true);
        ui->sfxPath->setText(stageConfig->soundFX[c].path);
        ui->sfxPath->blockSignals(false);

        ui->sfxID->setText("SoundFX ID: " + QString::number(globalSFXCount + c));

        if (ui->downSfx)
            ui->downSfx->setDisabled(c == ui->sfxList->count() - 1);
        if (ui->upSfx)
            ui->upSfx->setDisabled(c == 0);
    });

    connect(ui->addSfx, &QToolButton::clicked, [this] {
        ui->sfxList->blockSignals(true);
        uint c = ui->sfxList->currentRow() + 1;
        stageConfig->soundFX.insert(c, FormatHelpers::Stageconfig::SoundInfo());
        auto *item = new QListWidgetItem();
        item->setText(stageConfig->soundFX[c].path);
        ui->sfxList->insertItem(c, item);

        item->setFlags(item->flags() | Qt::ItemIsEditable);
        ui->sfxList->setCurrentItem(item);
        ui->sfxList->blockSignals(false);
    });

    connect(ui->upSfx, &QToolButton::clicked, [this] {
        uint c     = ui->sfxList->currentRow();
        auto *item = ui->sfxList->takeItem(c);
        stageConfig->soundFX.move(c, c - 1);
        ui->sfxList->insertItem(c - 1, item);
        ui->sfxList->setCurrentRow(c - 1);
    });

    connect(ui->downSfx, &QToolButton::clicked, [this] {
        uint c     = ui->sfxList->currentRow();
        auto *item = ui->sfxList->takeItem(c);
        stageConfig->soundFX.move(c, c + 1);
        ui->sfxList->insertItem(c + 1, item);
        ui->sfxList->setCurrentRow(c + 1);
    });

    connect(ui->rmSfx, &QToolButton::clicked, [this] {
        int c = ui->sfxList->currentRow();
        int n = ui->sfxList->currentRow() == ui->sfxList->count() - 1 ? c - 1 : c;
        delete ui->sfxList->item(c);
        stageConfig->soundFX.removeAt(c);
        ui->sfxList->blockSignals(true);
        ui->sfxList->setCurrentRow(n);
        ui->sfxList->blockSignals(false);
    });

    connect(ui->sfxList, &QListWidget::itemChanged, [this](QListWidgetItem *item) {
        stageConfig->soundFX[ui->sfxList->row(item)].path   = item->text();
        stageConfig->soundFX[ui->sfxList->row(item)].m_name = QFileInfo(item->text()).baseName();

        ui->sfxPath->blockSignals(true);
        ui->sfxPath->setText(stageConfig->soundFX[ui->sfxList->row(item)].path);
        ui->sfxPath->blockSignals(false);
    });

    connect(ui->sfxPath, &QLineEdit::textEdited, [this](QString s) {
        stageConfig->soundFX[ui->sfxList->currentRow()].path   = s;
        stageConfig->soundFX[ui->sfxList->currentRow()].m_name = QFileInfo(s).baseName();

        ui->sfxList->item(ui->sfxList->currentRow())
            ->setText(stageConfig->soundFX[ui->sfxList->currentRow()].path);
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
        ui->musPath->setText(stageConfig->music[c]);
        ui->musPath->blockSignals(false);

        if (ui->downMus)
            ui->downMus->setDisabled(c == ui->musList->count() - 1);
        if (ui->upMus)
            ui->upMus->setDisabled(c == 0);
    });

    connect(ui->addMus, &QToolButton::clicked, [this] {
        ui->musList->blockSignals(true);
        uint c = ui->musList->currentRow() + 1;
        stageConfig->music.insert(c, "Music.ogg");
        auto *item = new QListWidgetItem();
        item->setText(stageConfig->music[c]);
        ui->musList->insertItem(c, item);

        item->setFlags(item->flags() | Qt::ItemIsEditable);
        ui->musList->setCurrentItem(item);
        ui->musList->blockSignals(false);
    });

    connect(ui->upMus, &QToolButton::clicked, [this] {
        uint c     = ui->musList->currentRow();
        auto *item = ui->musList->takeItem(c);
        stageConfig->music.move(c, c - 1);
        ui->musList->insertItem(c - 1, item);
        ui->musList->setCurrentRow(c - 1);
    });

    connect(ui->downMus, &QToolButton::clicked, [this] {
        uint c     = ui->musList->currentRow();
        auto *item = ui->musList->takeItem(c);
        stageConfig->music.move(c, c + 1);
        ui->musList->insertItem(c + 1, item);
        ui->musList->setCurrentRow(c + 1);
    });

    connect(ui->rmMus, &QToolButton::clicked, [this] {
        int c = ui->musList->currentRow();
        int n = ui->musList->currentRow() == ui->musList->count() - 1 ? c - 1 : c;
        delete ui->musList->item(c);
        stageConfig->music.removeAt(c);
        ui->musList->blockSignals(true);
        ui->musList->setCurrentRow(n);
        ui->musList->blockSignals(false);
    });

    connect(ui->musList, &QListWidget::itemChanged, [this](QListWidgetItem *item) {
        stageConfig->music[ui->musList->row(item)] = item->text();

        ui->musPath->blockSignals(true);
        ui->musPath->setText(stageConfig->music[ui->musList->row(item)]);
        ui->musPath->blockSignals(false);
    });

    connect(ui->musPath, &QLineEdit::textEdited, [this](QString s) {
        stageConfig->soundFX[ui->musList->currentRow()].path = s;

        ui->musList->item(ui->musList->currentRow())
            ->setText(stageConfig->music[ui->musList->currentRow()]);
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
        ui->shtPath->setText(stageConfig->spriteSheets[c]);
        ui->shtPath->blockSignals(false);

        if (ui->downSht)
            ui->downSht->setDisabled(c == ui->shtList->count() - 1);
        if (ui->upSht)
            ui->upSht->setDisabled(c == 0);
    });

    connect(ui->addSht, &QToolButton::clicked, [this, createSheetList] {
        ui->shtList->blockSignals(true);
        uint c = ui->shtList->currentRow() + 1;
        stageConfig->spriteSheets.insert(c, "Sheet.bmp");
        auto *item = new QListWidgetItem();
        item->setText(stageConfig->spriteSheets[c]);
        ui->shtList->insertItem(c, item);

        item->setFlags(item->flags() | Qt::ItemIsEditable);
        ui->shtList->setCurrentItem(item);

        for (FormatHelpers::Stageconfig::ObjectInfo &obj : stageConfig->objects) {
            if (obj.sheetID >= (c + 6))
                ++obj.sheetID;
        }
        createSheetList();
        ui->shtList->blockSignals(false);
    });

    connect(ui->upSht, &QToolButton::clicked, [this, createSheetList] {
        uint c     = ui->shtList->currentRow();
        auto *item = ui->shtList->takeItem(c);
        stageConfig->spriteSheets.move(c, c - 1);
        ui->shtList->insertItem(c - 1, item);
        ui->shtList->setCurrentRow(c - 1);

        for (FormatHelpers::Stageconfig::ObjectInfo &obj : stageConfig->objects) {
            if (obj.sheetID == (c + 6))
                obj.sheetID = (c + 6) - 1;
            else if (obj.sheetID == (c + 6) - 1)
                obj.sheetID = (c + 6);
        }
        createSheetList();
    });

    connect(ui->downSht, &QToolButton::clicked, [this, createSheetList] {
        uint c     = ui->shtList->currentRow();
        auto *item = ui->shtList->takeItem(c);
        stageConfig->spriteSheets.move(c, c + 1);
        ui->shtList->insertItem(c + 1, item);
        ui->shtList->setCurrentRow(c + 1);

        for (FormatHelpers::Stageconfig::ObjectInfo &obj : stageConfig->objects) {
            if (obj.sheetID == (c + 6))
                obj.sheetID = (c + 6) + 1;
            else if (obj.sheetID == (c + 6) + 1)
                obj.sheetID = (c + 6);
        }
        createSheetList();
    });

    connect(ui->rmSht, &QToolButton::clicked, [this, createSheetList] {
        int c = ui->shtList->currentRow();
        int n = ui->shtList->currentRow() == ui->shtList->count() - 1 ? c - 1 : c;
        delete ui->shtList->item(c);
        stageConfig->spriteSheets.removeAt(c);
        ui->shtList->blockSignals(true);
        ui->shtList->setCurrentRow(n);
        ui->shtList->blockSignals(false);

        for (FormatHelpers::Stageconfig::ObjectInfo &obj : stageConfig->objects) {
            if (obj.sheetID > (c + 6))
                --obj.sheetID;
            else if (obj.sheetID == (c + 6))
                obj.sheetID = 4; // general.gfx
        }
        createSheetList();
    });

    connect(ui->shtList, &QListWidget::itemChanged, [this](QListWidgetItem *item) {
        stageConfig->spriteSheets[ui->shtList->row(item)] = item->text();

        ui->shtPath->blockSignals(true);
        ui->shtPath->setText(stageConfig->spriteSheets[ui->shtList->row(item)]);
        ui->shtPath->blockSignals(false);
    });

    connect(ui->shtPath, &QLineEdit::textEdited, [this](QString s) {
        stageConfig->spriteSheets[ui->shtList->currentRow()] = s;

        ui->shtList->item(ui->shtList->currentRow())
            ->setText(stageConfig->spriteSheets[ui->shtList->currentRow()]);
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
    for (FormatHelpers::Stageconfig::ObjectInfo &obj : stageConfig->objects) {
        ui->objList->addItem(obj.script);
        ui->objList->item(id)->setFlags(ui->objList->item(id)->flags() | Qt::ItemIsEditable);
        id++;
    }

    ui->sfxList->clear();
    id = 0;
    for (FormatHelpers::Stageconfig::SoundInfo &sfx : stageConfig->soundFX) {
        ui->sfxList->addItem(sfx.path);
        ui->sfxList->item(id)->setFlags(ui->sfxList->item(id)->flags() | Qt::ItemIsEditable);
        id++;
    }

    ui->musList->clear();
    id = 0;
    for (QString &mus : stageConfig->music) {
        ui->musList->addItem(mus);
        ui->musList->item(id)->setFlags(ui->musList->item(id)->flags() | Qt::ItemIsEditable);
        id++;
    }

    ui->shtList->clear();
    id = 0;
    for (QString &sheet : stageConfig->spriteSheets) {
        ui->shtList->addItem(sheet);
        ui->shtList->item(id)->setFlags(ui->shtList->item(id)->flags() | Qt::ItemIsEditable);
        id++;
    }
}

#include "moc_stageconfigeditorv1.cpp"
