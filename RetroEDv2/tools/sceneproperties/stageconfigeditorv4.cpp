#include "includes.hpp"
#include "ui_stageconfigeditorv4.h"
#include "stageconfigeditorv4.hpp"

StageConfigEditorv4::StageConfigEditorv4(FormatHelpers::StageConfig *scf, int gcCount,
                                         int globalSfxCount, QWidget *parent)
    : QDialog(parent), ui(new Ui::StageConfigEditorv4), stageConfig(scf)
{
    ui->setupUi(this);
    globalObjectCount = gcCount;
    globalSFXCount    = globalSfxCount;

    this->setWindowTitle("StageConfig Editor");

    // remove question mark from the title bar
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

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
        ui->objName->setText(stageConfig->objects[c].name);
        ui->objName->blockSignals(false);

        ui->objScript->blockSignals(true);
        ui->objScript->setText(stageConfig->objects[c].script);
        ui->objType->setText("Object Type ID: " + QString::number(c + globalObjectCount));
        ui->objScript->blockSignals(false);

        if (ui->downObj)
            ui->downObj->setDisabled(c == ui->objList->count() - 1);
        if (ui->upObj)
            ui->upObj->setDisabled(c == 0);
    });

    connect(ui->addObj, &QToolButton::clicked, [this] {
        ui->objList->blockSignals(true);
        uint c = ui->objList->currentRow() + 1;
        stageConfig->objects.insert(c, FormatHelpers::StageConfig::ObjectInfo());
        auto *item = new QListWidgetItem();
        item->setText(stageConfig->objects[c].name);
        ui->objList->insertItem(c, item);

        item->setFlags(item->flags() | Qt::ItemIsEditable);
        ui->objList->setCurrentItem(item);
        ui->objList->blockSignals(false);
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
        stageConfig->objects[ui->objList->row(item)].name = item->text();

        ui->objName->blockSignals(true);
        ui->objName->setText(stageConfig->objects[ui->objList->row(item)].name);
        ui->objName->blockSignals(false);
        DoAction("Changed Object Name");
    });

    connect(ui->objName, &QLineEdit::textEdited, [this](QString s) {
        stageConfig->objects[ui->objList->currentRow()].name = s;

        ui->objList->blockSignals(true);
        ui->objList->item(ui->objList->currentRow())
            ->setText(stageConfig->objects[ui->objList->currentRow()].name);
        ui->objList->blockSignals(false);

        DoAction("Changed Object Name");
    });
    connect(ui->objScript, &QLineEdit::textEdited, [this](QString s) {
        stageConfig->objects[ui->objList->currentRow()].script = s;
        DoAction("Changed Object Script");
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
        ui->sfxPath->setDisabled(c == -1);
        ui->sfxID->setText("SoundFX ID: ");

        if (c == -1)
            return;

        ui->sfxName->blockSignals(true);
        ui->sfxName->setText(stageConfig->soundFX[c].name);
        ui->sfxName->blockSignals(false);

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
        stageConfig->soundFX.insert(c, FormatHelpers::StageConfig::SoundInfo());
        auto *item = new QListWidgetItem();
        item->setText(stageConfig->soundFX[c].path);
        ui->sfxList->insertItem(c, item);

        item->setFlags(item->flags() | Qt::ItemIsEditable);
        ui->sfxList->setCurrentItem(item);
        ui->sfxList->blockSignals(false);
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
        stageConfig->soundFX[ui->sfxList->row(item)].name = item->text();

        ui->sfxName->blockSignals(true);
        ui->sfxName->setText(stageConfig->soundFX[ui->sfxList->currentRow()].name);
        ui->sfxName->blockSignals(false);
        DoAction("Changed Sfx Name");
    });

    connect(ui->sfxName, &QLineEdit::textEdited, [this](QString s) {
        stageConfig->soundFX[ui->sfxList->currentRow()].name = s;

        ui->sfxList->blockSignals(true);
        ui->sfxList->item(ui->sfxList->currentRow())
            ->setText(stageConfig->soundFX[ui->sfxList->currentRow()].name);
        ui->sfxList->blockSignals(false);

        DoAction("Changed Sfx Name");
    });

    connect(ui->sfxPath, &QLineEdit::textEdited, [this](QString s) {
        stageConfig->soundFX[ui->sfxList->currentRow()].path = s;
        DoAction("Changed Sfx Path");
    });
}

StageConfigEditorv4::~StageConfigEditorv4() { delete ui; }

bool StageConfigEditorv4::event(QEvent *event)
{

    switch ((int)event->type()) {
        default: break;
        case RE_EVENT_UNDO: UndoAction(); break;
        case RE_EVENT_REDO: UndoAction(); break;
    }

    return QWidget::event(event);
}

void StageConfigEditorv4::setupUI(bool allowRowChange)
{
    if (allowRowChange) {
        ui->sectionList->blockSignals(true);
        ui->sectionList->setCurrentRow(-1);
        ui->sectionList->blockSignals(false);
        ui->sectionList->setCurrentRow(0);
    }

    ui->objList->blockSignals(true);
    ui->sfxList->blockSignals(true);

    int prevIndex = ui->objList->currentRow();
    ui->objList->setCurrentRow(0);
    ui->objList->setCurrentRow(prevIndex >= ui->objList->count() ? -1 : prevIndex);

    prevIndex = ui->sfxList->currentRow();
    ui->sfxList->setCurrentRow(0);
    ui->sfxList->setCurrentRow(prevIndex >= ui->sfxList->count() ? -1 : prevIndex);

    ui->objList->clear();
    int id = 0;
    for (FormatHelpers::StageConfig::ObjectInfo &obj : stageConfig->objects) {
        ui->objList->addItem(obj.name);
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

    ui->objList->blockSignals(false);
    ui->sfxList->blockSignals(false);
}

void StageConfigEditorv4::UndoAction()
{
    if (actionIndex > 0) {
        // setStatus("Undid Action: " + actions[actionIndex].name);
        actionIndex--;
        ResetAction();
    }
}
void StageConfigEditorv4::RedoAction()
{
    if (actionIndex + 1 < actions.count()) {
        // setStatus("Redid Action: " + actions[actionIndex].name);
        actionIndex++;
        ResetAction();
    }
}
void StageConfigEditorv4::ResetAction()
{
#if RE_USE_UNSTABLE
    copyConfig(NULL, &actions[actionIndex]);

    setupUI(false);
#endif
}
void StageConfigEditorv4::DoAction(QString name, bool setModified)
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
void StageConfigEditorv4::ClearActions()
{
    actions.clear();
    actionIndex = 0;
    DoAction("Action Setup", false); // first action, cant be undone
}

void StageConfigEditorv4::copyConfig(ActionState *stateDst, ActionState *stateSrc)
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

#include "moc_stageconfigeditorv4.cpp"
