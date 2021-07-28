#include "includes.hpp"
#include "ui_scenepropertiesv5.h"

ScenePropertiesv5::ScenePropertiesv5(QWidget *parent) : QWidget(parent), ui(new Ui::ScenePropertiesv5)
{
    ui->setupUi(this);

    loadGlobalCB = ui->loadGlobals;

    m_editTIL = ui->editChunks;
    m_editSCF = ui->editStageconfig;
    m_editPAL = ui->editPalette;
}

ScenePropertiesv5::~ScenePropertiesv5() { delete ui; }

void ScenePropertiesv5::setupUI(RSDKv5::Scene *scn, RSDKv5::StageConfig *config)
{
    unsetUI();

    if (!scn || !config)
        return;

    ui->loadGlobals->setChecked(config->loadGlobalObjects);
}

void ScenePropertiesv5::unsetUI() { disconnect(ui->loadGlobals, nullptr, nullptr, nullptr); }

#include "moc_scenepropertiesv5.cpp"
