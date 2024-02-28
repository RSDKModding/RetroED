#include "includes.hpp"
#include "ui_stageconfigeditorv1.h"
#include "stageconfigeditorv1.hpp"

StageConfigEditorv1::StageConfigEditorv1(FormatHelpers::StageConfig *scf, QWidget *parent)
    : QDialog(parent), ui(new Ui::StageConfigEditorv1), stageConfig(scf)
{
    ui->setupUi(this);

    this->setWindowTitle("StageConfig Editor");

    // remove question mark from the title bar
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    ui->objName->setDisabled(true);

    setupUI();

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

        ui->musList->blockSignals(true);
        ui->musList->setCurrentRow(0);
        ui->musList->blockSignals(false);
        ui->musList->setCurrentRow(-1);

        ui->shtList->blockSignals(true);
        ui->shtList->setCurrentRow(0);
        ui->shtList->blockSignals(false);
        ui->shtList->setCurrentRow(-1);

        ui->detailsWidget->setCurrentIndex(v);
        ui->sectionLabel->setText(ui->sectionList->item(v)->text());
    });

    auto createSheetList = [this] {
        ui->objSheet->blockSignals(true);
        ui->objSheet->clear();

        ui->objSheet->addItem("Data/TitleCard/TitleCard1.gfx");
        ui->objSheet->addItem("[Unused 1]");
        ui->objSheet->addItem("Data/Objects/Shields.gfx");
        ui->objSheet->addItem("[Unused 2]");
        ui->objSheet->addItem("Data/Objects/General.gfx");
        ui->objSheet->addItem("Data/Objects/General2.gfx");
        int sheetID = 0;
        for (QString &sheet : stageConfig->spriteSheets) {
            if (sheetID++ < 5)
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
        ui->objName->setText(stageConfig->objects[c].name);
        ui->objName->blockSignals(false);

        ui->objScript->blockSignals(true);
        ui->objScript->setText(stageConfig->objects[c].script);
        ui->objScript->blockSignals(false);

        ui->objSheet->blockSignals(true);
        ui->objSheet->setCurrentIndex(stageConfig->objects[c].sheetID);
        ui->objSheet->blockSignals(false);

        ui->objType->setText("Object Type ID: " + QString::number(31 + c));

        if (ui->downObj)
            ui->downObj->setDisabled(c == ui->objList->count() - 1);
        if (ui->upObj)
            ui->upObj->setDisabled(c == 0);
    });

    connect(ui->addObj, &QToolButton::clicked, [this] {
        uint c = ui->objList->currentRow() + 1;
        stageConfig->objects.insert(c, FormatHelpers::StageConfig::ObjectInfo());
        auto *item = new QListWidgetItem();
        item->setText(stageConfig->objects[c].name);
        ui->objList->insertItem(c, item);

        item->setFlags(item->flags() | Qt::ItemIsEditable);
        ui->objList->setCurrentItem(item);
        DoAction("Added Object");
    });

    connect(ui->upObj, &QToolButton::clicked, [this] {
        uint c     = ui->objList->currentRow();
        auto *item = ui->objList->takeItem(c);
        stageConfig->objects.move(c, c - 1);
        ui->objList->insertItem(c - 1, item);
        ui->objList->setCurrentRow(c - 1);
        DoAction("Moved Object Up");
    });

    connect(ui->downObj, &QToolButton::clicked, [this] {
        uint c     = ui->objList->currentRow();
        auto *item = ui->objList->takeItem(c);
        stageConfig->objects.move(c, c + 1);
        ui->objList->insertItem(c + 1, item);
        ui->objList->setCurrentRow(c + 1);
        DoAction("Moved Object Down");
    });

    connect(ui->rmObj, &QToolButton::clicked, [this] {
        int c = ui->objList->currentRow();
        int n = ui->objList->currentRow() == ui->objList->count() - 1 ? c - 1 : c;
        delete ui->objList->item(c);
        stageConfig->objects.removeAt(c);
        ui->objList->blockSignals(true);
        ui->objList->setCurrentRow(n);
        ui->objList->blockSignals(false);
        DoAction("Removed Object");
    });

    connect(ui->objList, &QListWidget::itemChanged, [this](QListWidgetItem *item) {
        stageConfig->objects[ui->objList->row(item)].script = item->text();
        stageConfig->objects[ui->objList->row(item)].name   = QFileInfo(item->text()).baseName();

        ui->objName->blockSignals(true);
        ui->objName->setText(stageConfig->objects[ui->objList->row(item)].name);
        ui->objName->blockSignals(false);

        ui->objScript->blockSignals(true);
        ui->objScript->setText(stageConfig->objects[ui->objList->currentRow()].script);
        ui->objScript->blockSignals(false);
        DoAction("Changed Object Script");
    });

    connect(ui->objScript, &QLineEdit::textEdited, [this](QString s) {
        stageConfig->objects[ui->objList->currentRow()].script = s;
        stageConfig->objects[ui->objList->currentRow()].name   = QFileInfo(s).baseName();
        DoAction("Changed Object Script");
    });
    connect(ui->objSheet, QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int v) {
        stageConfig->objects[ui->objList->currentRow()].sheetID = (byte)v;
        DoAction("Changed Object Sheet");
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

        ui->sfxPath->setDisabled(c == -1);

        ui->sfxID->setText("SoundFX ID: ");

        if (c == -1)
            return;

        ui->sfxPath->blockSignals(true);
        ui->sfxPath->setText(stageConfig->soundFX[c].path);
        ui->sfxPath->blockSignals(false);

        ui->sfxID->setText("SoundFX ID: " + QString::number(40 + c));

        if (ui->downSfx)
            ui->downSfx->setDisabled(c == ui->sfxList->count() - 1);
        if (ui->upSfx)
            ui->upSfx->setDisabled(c == 0);
    });

    connect(ui->addSfx, &QToolButton::clicked, [this] {
        uint c = ui->sfxList->currentRow() + 1;
        stageConfig->soundFX.insert(c, FormatHelpers::StageConfig::SoundInfo());
        auto *item = new QListWidgetItem();
        item->setText(stageConfig->soundFX[c].path);
        ui->sfxList->insertItem(c, item);

        item->setFlags(item->flags() | Qt::ItemIsEditable);
        ui->sfxList->setCurrentItem(item);
        DoAction("Added Sfx");
    });

    connect(ui->upSfx, &QToolButton::clicked, [this] {
        uint c     = ui->sfxList->currentRow();
        auto *item = ui->sfxList->takeItem(c);
        stageConfig->soundFX.move(c, c - 1);
        ui->sfxList->insertItem(c - 1, item);
        ui->sfxList->setCurrentRow(c - 1);
        DoAction("Moved Sfx Up");
    });

    connect(ui->downSfx, &QToolButton::clicked, [this] {
        uint c     = ui->sfxList->currentRow();
        auto *item = ui->sfxList->takeItem(c);
        stageConfig->soundFX.move(c, c + 1);
        ui->sfxList->insertItem(c + 1, item);
        ui->sfxList->setCurrentRow(c + 1);
        DoAction("Moved Sfx Down");
    });

    connect(ui->rmSfx, &QToolButton::clicked, [this] {
        int c = ui->sfxList->currentRow();
        int n = ui->sfxList->currentRow() == ui->sfxList->count() - 1 ? c - 1 : c;
        delete ui->sfxList->item(c);
        stageConfig->soundFX.removeAt(c);
        ui->sfxList->blockSignals(true);
        ui->sfxList->setCurrentRow(n);
        ui->sfxList->blockSignals(false);
        DoAction("Removed Sfx");
    });

    connect(ui->sfxList, &QListWidget::itemChanged, [this](QListWidgetItem *item) {
        stageConfig->soundFX[ui->sfxList->row(item)].path = item->text();
        stageConfig->soundFX[ui->sfxList->row(item)].name = QFileInfo(item->text()).baseName();

        ui->sfxPath->blockSignals(true);
        ui->sfxPath->setText(stageConfig->soundFX[ui->sfxList->row(item)].path);
        ui->sfxPath->blockSignals(false);
        DoAction("Changed Sfx Path");
    });

    connect(ui->sfxPath, &QLineEdit::textEdited, [this](QString s) {
        stageConfig->soundFX[ui->sfxList->currentRow()].path = s;
        stageConfig->soundFX[ui->sfxList->currentRow()].name = QFileInfo(s).baseName();

        ui->sfxList->blockSignals(true);
        ui->sfxList->item(ui->sfxList->currentRow())
            ->setText(stageConfig->soundFX[ui->sfxList->currentRow()].path);
        ui->sfxList->blockSignals(false);

        DoAction("Changed Sfx Path");
    });

    // ----------------
    // MUSIC
    // ----------------
    connect(ui->musList, &QListWidget::currentRowChanged, [this](int c) {
        if (ui->addMus)
            ui->addMus->setDisabled(c >= 5);
        if (ui->upMus)
            ui->upMus->setDisabled(c == -1 || c >= 5);
        if (ui->downMus)
            ui->downMus->setDisabled(c == -1 || c >= 5);
        if (ui->rmMus)
            ui->rmMus->setDisabled(c == -1);

        ui->musPath->setDisabled(c == -1 || c >= 5);

        if (c == -1 || c >= 5)
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
        uint c = ui->musList->currentRow() + 1;
        stageConfig->music.insert(c, "Music.ogg");
        auto *item = new QListWidgetItem();
        item->setText(stageConfig->music[c]);
        ui->musList->insertItem(c, item);

        item->setFlags(item->flags() | Qt::ItemIsEditable);
        ui->musList->setCurrentItem(item);
        DoAction("Added Music");
    });

    connect(ui->upMus, &QToolButton::clicked, [this] {
        uint c     = ui->musList->currentRow();
        auto *item = ui->musList->takeItem(c);
        stageConfig->music.move(c, c - 1);
        ui->musList->insertItem(c - 1, item);
        ui->musList->setCurrentRow(c - 1);
        DoAction("Moved Music Up");
    });

    connect(ui->downMus, &QToolButton::clicked, [this] {
        uint c     = ui->musList->currentRow();
        auto *item = ui->musList->takeItem(c);
        stageConfig->music.move(c, c + 1);
        ui->musList->insertItem(c + 1, item);
        ui->musList->setCurrentRow(c + 1);
        DoAction("Moved Music Down");
    });

    connect(ui->rmMus, &QToolButton::clicked, [this] {
        int c = ui->musList->currentRow();
        int n = ui->musList->currentRow() == ui->musList->count() - 1 ? c - 1 : c;
        delete ui->musList->item(c);
        stageConfig->music.removeAt(c);
        ui->musList->blockSignals(true);
        ui->musList->setCurrentRow(n);
        ui->musList->blockSignals(false);
        DoAction("Removed Music");
    });

    connect(ui->musList, &QListWidget::itemChanged, [this](QListWidgetItem *item) {
        stageConfig->music[ui->musList->row(item)] = item->text();

        ui->musPath->blockSignals(true);
        ui->musPath->setText(stageConfig->music[ui->musList->row(item)]);
        ui->musPath->blockSignals(false);
        DoAction("Changed Music Path");
    });

    connect(ui->musPath, &QLineEdit::textEdited, [this](QString s) {
        stageConfig->music[ui->musList->currentRow()] = s;

        ui->musList->blockSignals(true);
        ui->musList->item(ui->musList->currentRow())
            ->setText(stageConfig->music[ui->musList->currentRow()]);
        ui->musList->blockSignals(false);

        DoAction("Changed Music Path");
    });

    // ----------------
    // SPRITE SHEETS
    // ----------------
    connect(ui->shtList, &QListWidget::currentRowChanged, [this](int c) {
        if (ui->addSht)
            ui->addSht->setDisabled(c >= 5);
        if (ui->upSht)
            ui->upSht->setDisabled(c == -1 || c >= 5);
        if (ui->downSht)
            ui->downSht->setDisabled(c == -1 || c >= 5);
        if (ui->rmSht)
            ui->rmSht->setDisabled(c == -1);

        ui->shtPath->setDisabled(c == -1 || c >= 5);

        if (c == -1 || c >= 5)
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
        uint c = ui->shtList->currentRow() + 1;
        stageConfig->spriteSheets.insert(c, "Sheet.bmp");
        auto *item = new QListWidgetItem();
        item->setText(stageConfig->spriteSheets[c]);
        ui->shtList->insertItem(c, item);

        item->setFlags(item->flags() | Qt::ItemIsEditable);
        ui->shtList->setCurrentItem(item);

        for (FormatHelpers::StageConfig::ObjectInfo &obj : stageConfig->objects) {
            if (obj.sheetID >= (c + 6))
                ++obj.sheetID;
        }
        createSheetList();
        DoAction("Added Sheet");
    });

    connect(ui->upSht, &QToolButton::clicked, [this, createSheetList] {
        uint c     = ui->shtList->currentRow();
        auto *item = ui->shtList->takeItem(c);
        stageConfig->spriteSheets.move(c, c - 1);
        ui->shtList->insertItem(c - 1, item);
        ui->shtList->setCurrentRow(c - 1);

        for (FormatHelpers::StageConfig::ObjectInfo &obj : stageConfig->objects) {
            if (obj.sheetID == (c + 6))
                obj.sheetID = (c + 6) - 1;
            else if (obj.sheetID == (c + 6) - 1)
                obj.sheetID = (c + 6);
        }
        createSheetList();
        DoAction("Moved Sheet Up");
    });

    connect(ui->downSht, &QToolButton::clicked, [this, createSheetList] {
        uint c     = ui->shtList->currentRow();
        auto *item = ui->shtList->takeItem(c);
        stageConfig->spriteSheets.move(c, c + 1);
        ui->shtList->insertItem(c + 1, item);
        ui->shtList->setCurrentRow(c + 1);

        for (FormatHelpers::StageConfig::ObjectInfo &obj : stageConfig->objects) {
            if (obj.sheetID == (c + 6))
                obj.sheetID = (c + 6) + 1;
            else if (obj.sheetID == (c + 6) + 1)
                obj.sheetID = (c + 6);
        }
        createSheetList();
        DoAction("Moved Sheet Down");
    });

    connect(ui->rmSht, &QToolButton::clicked, [this, createSheetList] {
        int c = ui->shtList->currentRow();
        int n = ui->shtList->currentRow() == ui->shtList->count() - 1 ? c - 1 : c;
        delete ui->shtList->item(c);
        stageConfig->spriteSheets.removeAt(c);
        ui->shtList->blockSignals(true);
        ui->shtList->setCurrentRow(n);
        ui->shtList->blockSignals(false);

        for (FormatHelpers::StageConfig::ObjectInfo &obj : stageConfig->objects) {
            if (obj.sheetID > (c + 6))
                --obj.sheetID;
            else if (obj.sheetID == (c + 6))
                obj.sheetID = 4; // general.gfx
        }
        createSheetList();
        DoAction("Removed Sheet");
    });

    connect(ui->shtList, &QListWidget::itemChanged, [this](QListWidgetItem *item) {
        stageConfig->spriteSheets[ui->shtList->row(item)] = item->text();

        ui->shtPath->blockSignals(true);
        ui->shtPath->setText(stageConfig->spriteSheets[ui->shtList->row(item)]);
        ui->shtPath->blockSignals(false);
        DoAction("Changed Sheet Path");
    });

    connect(ui->shtPath, &QLineEdit::textEdited, [this](QString s) {
        stageConfig->spriteSheets[ui->shtList->currentRow()] = s;

        ui->shtList->blockSignals(true);
        ui->shtList->item(ui->shtList->currentRow())
            ->setText(stageConfig->spriteSheets[ui->shtList->currentRow()]);
        ui->shtList->blockSignals(false);

        DoAction("Changed Sheet Path");
    });
}

StageConfigEditorv1::~StageConfigEditorv1() { delete ui; }

bool StageConfigEditorv1::event(QEvent *event)
{

    switch ((int)event->type()) {
        default: break;
        case RE_EVENT_UNDO: UndoAction(); break;
        case RE_EVENT_REDO: UndoAction(); break;
    }

    return QWidget::event(event);
}

void StageConfigEditorv1::setupUI(bool allowRowChange)
{
    if (allowRowChange) {
        ui->sectionList->blockSignals(true);
        ui->sectionList->setCurrentRow(-1);
        ui->sectionList->blockSignals(false);
        ui->sectionList->setCurrentRow(0);
    }

    ui->objList->blockSignals(true);
    ui->sfxList->blockSignals(true);
    ui->musList->blockSignals(true);
    ui->shtList->blockSignals(true);

    int prevIndex = ui->objList->currentRow();
    ui->objList->setCurrentRow(0);
    ui->objList->setCurrentRow(prevIndex >= ui->objList->count() ? -1 : prevIndex);

    prevIndex = ui->sfxList->currentRow();
    ui->sfxList->setCurrentRow(0);
    ui->sfxList->setCurrentRow(prevIndex >= ui->sfxList->count() ? -1 : prevIndex);

    prevIndex = ui->musList->currentRow();
    ui->musList->setCurrentRow(0);
    ui->musList->setCurrentRow(prevIndex >= ui->musList->count() ? -1 : prevIndex);

    prevIndex = ui->shtList->currentRow();
    ui->shtList->setCurrentRow(0);
    ui->shtList->setCurrentRow(prevIndex >= ui->shtList->count() ? -1 : prevIndex);

    ui->objList->clear();
    int id = 0;
    for (FormatHelpers::StageConfig::ObjectInfo &obj : stageConfig->objects) {
        ui->objList->addItem(obj.script);
        ui->objList->item(id)->setFlags(ui->objList->item(id)->flags() | Qt::ItemIsEditable);
        id++;
    }

    ui->sfxList->clear();
    id = 0;
    for (FormatHelpers::StageConfig::SoundInfo &sfx : stageConfig->soundFX) {
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

    ui->objList->blockSignals(false);
    ui->sfxList->blockSignals(false);
    ui->musList->blockSignals(false);
    ui->shtList->blockSignals(false);
}

void StageConfigEditorv1::UndoAction()
{
    if (actionIndex > 0) {
        // setStatus("Undid Action: " + actions[actionIndex].name);
        actionIndex--;
        ResetAction();
    }
}
void StageConfigEditorv1::RedoAction()
{
    if (actionIndex + 1 < actions.count()) {
        // setStatus("Redid Action: " + actions[actionIndex].name);
        actionIndex++;
        ResetAction();
    }
}
void StageConfigEditorv1::ResetAction()
{
#if RE_USE_UNSTABLE
    copyConfig(NULL, &actions[actionIndex]);

    setupUI(false);
#endif
}
void StageConfigEditorv1::DoAction(QString name, bool setModified)
{
    Q_UNUSED(setModified);

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

    // setStatus("Did Action: " + name);
}
void StageConfigEditorv1::ClearActions()
{
    actions.clear();
    actionIndex = 0;
    DoAction("Action Setup", false); // first action, cant be undone
}

void StageConfigEditorv1::copyConfig(ActionState *stateDst, ActionState *stateSrc)
{
    FormatHelpers::StageConfig *src = stateSrc ? &stateSrc->stageConfig : stageConfig;
    FormatHelpers::StageConfig *dst = stateDst ? &stateDst->stageConfig : stageConfig;

    dst->filePath          = src->filePath;
    dst->loadGlobalScripts = src->loadGlobalScripts;

    dst->palette.colors.clear();
    for (auto clr : src->palette.colors) dst->palette.colors.append(clr);

    dst->objects.clear();
    for (auto obj : src->objects) dst->objects.append(obj);

    dst->soundFX.clear();
    for (auto sfx : src->soundFX) dst->soundFX.append(sfx);

    dst->music.clear();
    for (auto mus : src->music) dst->music.append(mus);

    dst->spriteSheets.clear();
    for (auto sht : src->spriteSheets) dst->spriteSheets.append(sht);
}

#include "moc_stageconfigeditorv1.cpp"
