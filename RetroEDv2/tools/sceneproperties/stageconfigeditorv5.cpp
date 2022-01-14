#include "includes.hpp"
#include "ui_stageconfigeditorv5.h"

StageConfigEditorv5::StageConfigEditorv5(RSDKv5::StageConfig *scf, QWidget *parent)
    : stageConfig(scf), QDialog(parent), ui(new Ui::StageConfigEditorv5)
{
    ui->setupUi(this);

    this->setWindowTitle("StageConfig Editor");

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

        if (c == -1)
            return;

        ui->objName->blockSignals(true);
        ui->objName->setText(stageConfig->objects[c]);
        ui->objName->blockSignals(false);

        if (ui->downObj)
            ui->downObj->setDisabled(c == ui->objList->count() - 1);
        if (ui->upObj)
            ui->upObj->setDisabled(c == 0);
    });

    connect(ui->addObj, &QToolButton::clicked, [this] {
        ui->objList->blockSignals(true);
        uint c = ui->objList->currentRow() + 1;
        stageConfig->objects.insert(c, "New Object");
        auto *item = new QListWidgetItem();
        item->setText(stageConfig->objects[c]);
        ui->objList->insertItem(c, item);

        item->setFlags(item->flags() | Qt::ItemIsEditable);
        ui->objList->setCurrentItem(item);
        ui->objList->blockSignals(false);
        doAction("Added Object");
    });

    connect(ui->upObj, &QToolButton::clicked, [this] {
        uint c     = ui->objList->currentRow();
        auto *item = ui->objList->takeItem(c);
        stageConfig->objects.move(c, c - 1);
        ui->objList->insertItem(c - 1, item);
        ui->objList->setCurrentRow(c - 1);
        doAction("Moved Object Up");
    });

    connect(ui->downObj, &QToolButton::clicked, [this] {
        uint c     = ui->objList->currentRow();
        auto *item = ui->objList->takeItem(c);
        stageConfig->objects.move(c, c + 1);
        ui->objList->insertItem(c + 1, item);
        ui->objList->setCurrentRow(c + 1);
        doAction("Moved Object Down");
    });

    connect(ui->rmObj, &QToolButton::clicked, [this] {
        int c = ui->objList->currentRow();
        int n = ui->objList->currentRow() == ui->objList->count() - 1 ? c - 1 : c;
        delete ui->objList->item(c);
        stageConfig->objects.removeAt(c);
        ui->objList->blockSignals(true);
        ui->objList->setCurrentRow(n);
        ui->objList->blockSignals(false);
        doAction("Removed Object");
    });

    connect(ui->objList, &QListWidget::itemChanged, [this](QListWidgetItem *item) {
        stageConfig->objects[ui->objList->row(item)] = item->text();

        ui->objName->blockSignals(true);
        ui->objName->setText(stageConfig->objects[ui->objList->row(item)]);
        ui->objName->blockSignals(false);
        doAction("Changed Object Name");
    });

    connect(ui->objName, &QLineEdit::textEdited, [this](QString s) {
        stageConfig->objects[ui->objList->currentRow()] = s;

        ui->objList->item(ui->objList->currentRow())
            ->setText(stageConfig->objects[ui->objList->currentRow()]);
        doAction("Changed Object Name");
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
        ui->maxPlays->setDisabled(c == -1);

        if (c == -1)
            return;

        ui->sfxPath->blockSignals(true);
        ui->sfxPath->setText(stageConfig->soundFX[c].path);
        ui->sfxPath->blockSignals(false);

        ui->maxPlays->blockSignals(true);
        ui->maxPlays->setValue(stageConfig->soundFX[c].maxConcurrentPlay);
        ui->maxPlays->blockSignals(false);

        if (ui->downSfx)
            ui->downSfx->setDisabled(c == ui->sfxList->count() - 1);
        if (ui->upSfx)
            ui->upSfx->setDisabled(c == 0);
    });

    connect(ui->addSfx, &QToolButton::clicked, [this] {
        ui->sfxList->blockSignals(true);
        uint c = ui->sfxList->currentRow() + 1;
        stageConfig->soundFX.insert(c, RSDKv5::StageConfig::WAVConfiguration());
        auto *item = new QListWidgetItem();
        item->setText(stageConfig->soundFX[c].path);
        ui->sfxList->insertItem(c, item);

        item->setFlags(item->flags() | Qt::ItemIsEditable);
        ui->sfxList->setCurrentItem(item);
        ui->sfxList->blockSignals(false);
        doAction("Added Sfx");
    });

    connect(ui->upSfx, &QToolButton::clicked, [this] {
        uint c     = ui->sfxList->currentRow();
        auto *item = ui->sfxList->takeItem(c);
        stageConfig->soundFX.move(c, c - 1);
        ui->sfxList->insertItem(c - 1, item);
        ui->sfxList->setCurrentRow(c - 1);
        doAction("Moved Sfx Up");
    });

    connect(ui->downSfx, &QToolButton::clicked, [this] {
        uint c     = ui->sfxList->currentRow();
        auto *item = ui->sfxList->takeItem(c);
        stageConfig->soundFX.move(c, c + 1);
        ui->sfxList->insertItem(c + 1, item);
        ui->sfxList->setCurrentRow(c + 1);
        doAction("Moved Sfx Down");
    });

    connect(ui->rmSfx, &QToolButton::clicked, [this] {
        int c = ui->sfxList->currentRow();
        int n = ui->sfxList->currentRow() == ui->sfxList->count() - 1 ? c - 1 : c;
        delete ui->sfxList->item(c);
        stageConfig->soundFX.removeAt(c);
        ui->sfxList->blockSignals(true);
        ui->sfxList->setCurrentRow(n);
        ui->sfxList->blockSignals(false);
        doAction("Removed Sfx");
    });

    connect(ui->sfxList, &QListWidget::itemChanged, [this](QListWidgetItem *item) {
        stageConfig->soundFX[ui->sfxList->row(item)].path = item->text();

        ui->sfxPath->blockSignals(true);
        ui->sfxPath->setText(stageConfig->soundFX[ui->sfxList->currentRow()].path);
        ui->sfxPath->blockSignals(false);
        doAction("Changed Sfx Path");
    });

    connect(ui->sfxPath, &QLineEdit::textEdited, [this](QString s) {
        stageConfig->soundFX[ui->sfxList->currentRow()].path = s;
        doAction("Changed Sfx Path");
    });

    connect(ui->maxPlays, QOverload<int>::of(&QSpinBox::valueChanged), [this](int v) {
        stageConfig->soundFX[ui->sfxList->currentRow()].maxConcurrentPlay = v;

        ui->sfxList->item(ui->sfxList->currentRow())
            ->setText(stageConfig->soundFX[ui->sfxList->currentRow()].path);
        doAction("Changed Sfx Plays");
    });
}

StageConfigEditorv5::~StageConfigEditorv5() { delete ui; }

bool StageConfigEditorv5::event(QEvent *event)
{

    switch ((int)event->type()) {
        default: break;
        case RE_EVENT_UNDO: undoAction(); break;
        case RE_EVENT_REDO: undoAction(); break;
    }

    return QWidget::event(event);
}

void StageConfigEditorv5::setupUI(bool allowRowChange)
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
    for (QString &obj : stageConfig->objects) {
        ui->objList->addItem(obj);
        ui->objList->item(id)->setFlags(ui->objList->item(id)->flags() | Qt::ItemIsEditable);
        id++;
    }

    ui->sfxList->clear();
    id = 0;
    for (auto &sfx : stageConfig->soundFX) {
        ui->sfxList->addItem(sfx.path);
        ui->sfxList->item(id)->setFlags(ui->sfxList->item(id)->flags() | Qt::ItemIsEditable);
        id++;
    }

    ui->objList->blockSignals(false);
    ui->sfxList->blockSignals(false);
}

void StageConfigEditorv5::undoAction()
{
    if (actionIndex > 0) {
        // setStatus("Undid Action: " + actions[actionIndex].name);
        actionIndex--;
        resetAction();
    }
}
void StageConfigEditorv5::redoAction()
{
    if (actionIndex + 1 < actions.count()) {
        // setStatus("Redid Action: " + actions[actionIndex].name);
        actionIndex++;
        resetAction();
    }
}
void StageConfigEditorv5::resetAction()
{
    copyConfig(NULL, &actions[actionIndex]);

    setupUI(false);
}
void StageConfigEditorv5::doAction(QString name, bool setModified)
{
    Q_UNUSED(setModified);

    ActionState action;

    action.name = name;

    copyConfig(&action, NULL);

    // Actions
    for (int i = actions.count() - 1; i > actionIndex; --i) {
        actions.removeAt(i);
    }

    actions.append(action);
    actionIndex = actions.count() - 1;

    // setStatus("Did Action: " + name);
}
void StageConfigEditorv5::clearActions()
{
    actions.clear();
    actionIndex = 0;
    doAction("Action Setup", false); // first action, cant be undone
}

void StageConfigEditorv5::copyConfig(ActionState *stateDst, ActionState *stateSrc)
{
    RSDKv5::StageConfig *src = stateSrc ? &stateSrc->stageConfig : stageConfig;
    RSDKv5::StageConfig *dst = stateDst ? &stateDst->stageConfig : stageConfig;

    dst->filePath          = src->filePath;
    dst->loadGlobalObjects = src->loadGlobalObjects;

    for (int i = 0; i < 8; ++i) {
        for (int r = 0; r < 16; ++r) {
            dst->palettes[i].activeRows[r] = src->palettes[i].activeRows[r];
            for (int c = 0; c < 16; ++c)
                dst->palettes[i].colours[r][c] = src->palettes[i].colours[r][c];
        }
    }

    dst->objects.clear();
    for (auto obj : src->objects) dst->objects.append(obj);

    dst->soundFX.clear();
    for (auto sfx : src->soundFX) dst->soundFX.append(sfx);
}

#include "moc_stageconfigeditorv5.cpp"
