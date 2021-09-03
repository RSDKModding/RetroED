#include "includes.hpp"
#include "ui_stageconfigeditorv2.h"

StageconfigEditorv2::StageconfigEditorv2(FormatHelpers::Stageconfig *scf, int gcCount,
                                         int globalSfxCount, QWidget *parent)
    : stageConfig(scf), QDialog(parent), ui(new Ui::StageconfigEditorv2)
{
    ui->setupUi(this);
    globalObjectCount = gcCount;
    globalSFXCount    = globalSfxCount;

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

        ui->objScript->setDisabled(c == -1);
        ui->objType->setText("Object Type ID: ");

        if (c == -1)
            return;

        ui->objName->blockSignals(true);
        ui->objName->setText(stageConfig->objects[c].m_name);
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

    connect(ui->objScript, &QLineEdit::textEdited, [this](QString s) {
        stageConfig->objects[ui->objList->currentRow()].script = s;
        stageConfig->objects[ui->objList->currentRow()].m_name = QFileInfo(s).baseName();

        ui->objList->item(ui->objList->currentRow())
            ->setText(stageConfig->objects[ui->objList->currentRow()].m_name);

        ui->objName->blockSignals(true);
        ui->objName->setText(stageConfig->objects[ui->objList->currentRow()].m_name);
        ui->objName->blockSignals(false);
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
        ui->sfxID->setText("SoundFX ID: " + QString::number(globalSFXCount + c));

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
}

StageconfigEditorv2::~StageconfigEditorv2() { delete ui; }

void StageconfigEditorv2::setupUI()
{
    ui->sectionList->blockSignals(true);
    ui->sectionList->setCurrentRow(-1);
    ui->sectionList->blockSignals(false);
    ui->sectionList->setCurrentRow(0);

    ui->objList->setCurrentRow(0);
    ui->objList->setCurrentRow(-1);

    ui->sfxList->setCurrentRow(0);
    ui->sfxList->setCurrentRow(-1);

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
}

#include "moc_stageconfigeditorv2.cpp"
