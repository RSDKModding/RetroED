#include "includes.hpp"
#include "ui_stageconfigeditorv5.h"

StageconfigEditorv5::StageconfigEditorv5(RSDKv5::StageConfig *scf, QWidget *parent)
    : m_stageconfig(scf), QDialog(parent), ui(new Ui::StageconfigEditorv5)
{
    ui->setupUi(this);

    this->setWindowTitle("Stageconfig Editor");

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

        ui->objName->setDisabled(c == -1);

        if (c == -1)
            return;

        ui->objName->blockSignals(true);
        ui->objName->setText(m_stageconfig->objects[c]);
        ui->objName->blockSignals(false);

        if (ui->downObj)
            ui->downObj->setDisabled(c == ui->objList->count() - 1);
        if (ui->upObj)
            ui->upObj->setDisabled(c == 0);
    });

    connect(ui->addObj, &QToolButton::clicked, [this] {
        ui->objList->blockSignals(true);
        uint c = ui->objList->currentRow() + 1;
        m_stageconfig->objects.insert(c, "New Object");
        auto *item = new QListWidgetItem();
        item->setText(m_stageconfig->objects[c]);
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
        m_stageconfig->objects[ui->objList->row(item)] = item->text();

        ui->objName->blockSignals(true);
        ui->objName->setText(m_stageconfig->objects[ui->objList->row(item)]);
        ui->objName->blockSignals(false);
    });

    connect(ui->objName, &QLineEdit::textEdited, [this](QString s) {
        m_stageconfig->objects[ui->objList->currentRow()] = s;

        ui->objList->item(ui->objList->currentRow())
            ->setText(m_stageconfig->objects[ui->objList->currentRow()]);
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
        ui->sfxPath->setText(m_stageconfig->soundFX[c].path);
        ui->sfxPath->blockSignals(false);

        ui->maxPlays->blockSignals(true);
        ui->maxPlays->setValue(m_stageconfig->soundFX[c].maxConcurrentPlay);
        ui->maxPlays->blockSignals(false);

        if (ui->downSfx)
            ui->downSfx->setDisabled(c == ui->sfxList->count() - 1);
        if (ui->upSfx)
            ui->upSfx->setDisabled(c == 0);
    });

    connect(ui->addSfx, &QToolButton::clicked, [this] {
        ui->sfxList->blockSignals(true);
        uint c = ui->sfxList->currentRow() + 1;
        m_stageconfig->soundFX.insert(c, RSDKv5::StageConfig::WAVConfiguration());
        auto *item = new QListWidgetItem();
        item->setText(m_stageconfig->soundFX[c].path);
        ui->sfxList->insertItem(c, item);

        item->setFlags(item->flags() | Qt::ItemIsEditable);
        ui->sfxList->setCurrentItem(item);
        ui->sfxList->blockSignals(false);
    });

    connect(ui->upSfx, &QToolButton::clicked, [this] {
        uint c     = ui->sfxList->currentRow();
        auto *item = ui->sfxList->takeItem(c);
        m_stageconfig->soundFX.move(c, c - 1);
        ui->sfxList->insertItem(c - 1, item);
        ui->sfxList->setCurrentRow(c - 1);
    });

    connect(ui->downSfx, &QToolButton::clicked, [this] {
        uint c     = ui->sfxList->currentRow();
        auto *item = ui->sfxList->takeItem(c);
        m_stageconfig->soundFX.move(c, c + 1);
        ui->sfxList->insertItem(c + 1, item);
        ui->sfxList->setCurrentRow(c + 1);
    });

    connect(ui->rmSfx, &QToolButton::clicked, [this] {
        int c = ui->sfxList->currentRow();
        int n = ui->sfxList->currentRow() == ui->sfxList->count() - 1 ? c - 1 : c;
        delete ui->sfxList->item(c);
        m_stageconfig->soundFX.removeAt(c);
        ui->sfxList->blockSignals(true);
        ui->sfxList->setCurrentRow(n);
        ui->sfxList->blockSignals(false);
    });

    connect(ui->sfxList, &QListWidget::itemChanged, [this](QListWidgetItem *item) {
        m_stageconfig->soundFX[ui->sfxList->row(item)].path = item->text();

        ui->sfxPath->blockSignals(true);
        ui->sfxPath->setText(m_stageconfig->soundFX[ui->sfxList->currentRow()].path);
        ui->sfxPath->blockSignals(false);
    });

    connect(ui->sfxPath, &QLineEdit::textEdited,
            [this](QString s) { m_stageconfig->soundFX[ui->sfxList->currentRow()].path = s; });

    connect(ui->maxPlays, QOverload<int>::of(&QSpinBox::valueChanged), [this](int v) {
        m_stageconfig->soundFX[ui->sfxList->currentRow()].maxConcurrentPlay = v;

        ui->sfxList->item(ui->sfxList->currentRow())
            ->setText(m_stageconfig->soundFX[ui->sfxList->currentRow()].path);
    });
}

StageconfigEditorv5::~StageconfigEditorv5() { delete ui; }

void StageconfigEditorv5::setupUI()
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
    for (QString &obj : m_stageconfig->objects) {
        ui->objList->addItem(obj);
        ui->objList->item(id)->setFlags(ui->objList->item(id)->flags() | Qt::ItemIsEditable);
        id++;
    }

    ui->sfxList->clear();
    id = 0;
    for (auto &sfx : m_stageconfig->soundFX) {
        ui->sfxList->addItem(sfx.path);
        ui->sfxList->item(id)->setFlags(ui->sfxList->item(id)->flags() | Qt::ItemIsEditable);
        id++;
    }
}

#include "moc_stageconfigeditorv5.cpp"
