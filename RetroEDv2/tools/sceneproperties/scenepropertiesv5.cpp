#include "includes.hpp"
#include "ui_scenepropertiesv5.h"

ScenePropertiesv5::ScenePropertiesv5(QWidget *parent) : QWidget(parent), ui(new Ui::ScenePropertiesv5)
{
    ui->setupUi(this);

    loadGlobalCB  = ui->loadGlobals;
    stampNameEdit = ui->stampName;

    editTIL  = ui->editChunks;
    editSCF  = ui->editStageConfig;
    editPAL  = ui->editPalette;
    editTSet = ui->editTSet;

    loadStamps = ui->loadStamps;
    saveStamps = ui->saveStamps;

    layerCnt   = ui->layerCnt;
    objCnt     = ui->objCnt;
    entCntTot  = ui->entCntTot;
    entCntAct  = ui->entCntAct;
    entCntInac = ui->entCntInac;

    gridX = ui->gridX;
    gridY = ui->gridY;

    syncGC     = ui->syncGC;
    syncSC     = ui->syncSC;
    reloadLink = ui->reloadLink;

    replaceTile = ui->replaceTile;

    bgSel = new color_widgets::ColorPreview(this);
    ui->bgClrLayout->addWidget(bgSel);

    altBGSel = new color_widgets::ColorPreview(this);
    ui->altBGClrLayout->addWidget(altBGSel);

    unknown1 = ui->unknown1;
    unknown2 = ui->unknown2;
    unknown3 = ui->unknown3;
    unknown4 = ui->unknown4;
    unknown5 = ui->unknown5;
    unknown6 = ui->unknown6;
    unknown7 = ui->unknown7;
    unknown8 = ui->unknown8;
    unknown9 = ui->unknown9;
}

ScenePropertiesv5::~ScenePropertiesv5()
{
    delete ui;

    if (bgSel)
        delete bgSel;
    bgSel = nullptr;

    if (altBGSel)
        delete altBGSel;
    altBGSel = nullptr;
}

void ScenePropertiesv5::setupUI(RSDKv5::Scene *scn, RSDKv5::StageConfig *config)
{
    unsetUI();

    if (!scn || !config)
        return;

    ui->loadGlobals->blockSignals(true);
    ui->loadGlobals->setChecked(config->loadGlobalObjects);
    ui->loadGlobals->blockSignals(false);

    // ui->stampName->blockSignals(true);
    // ui->stampName->setText(scn->editorMetadata.stampName);
    // ui->stampName->blockSignals(false);
}

void ScenePropertiesv5::unsetUI() {}

#include "moc_scenepropertiesv5.cpp"
