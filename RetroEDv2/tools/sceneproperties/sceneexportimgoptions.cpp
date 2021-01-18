#include "includes.hpp"
#include "ui_sceneexportimgoptions.h"

SceneExportImgOptions::SceneExportImgOptions(QWidget *parent)
    : QDialog(parent), ui(new Ui::SceneExportImgOptions)
{
    ui->setupUi(this);

    memset(m_exportFG, 0, 2 * sizeof(bool));
    memset(m_exportBG, 0, 8 * 2 * sizeof(bool));

    m_exportFG[0] = true;
    m_exportFG[1] = true;

    QCheckBox *bgLow[]  = { ui->bg1Low, ui->bg2Low, ui->bg3Low, ui->bg4Low,
                           ui->bg5Low, ui->bg6Low, ui->bg7Low, ui->bg8Low };
    QCheckBox *bgHigh[] = { ui->bg1High, ui->bg2High, ui->bg3High, ui->bg4High,
                            ui->bg5High, ui->bg6High, ui->bg7High, ui->bg8High };

    connect(ui->fgLow, &QCheckBox::toggled, [this](bool c) { m_exportFG[0] = c; });
    connect(ui->fgHigh, &QCheckBox::toggled, [this](bool c) { m_exportFG[1] = c; });

    for (int i = 0; i < 8; ++i) {
        connect(bgLow[i], &QCheckBox::toggled, [this, &i](bool c) { m_exportBG[i][0] = c; });
    }

    for (int i = 0; i < 8; ++i) {
        connect(bgHigh[i], &QCheckBox::toggled, [this, &i](bool c) { m_exportBG[i][1] = c; });
    }

    connect(ui->objects, &QCheckBox::toggled, [this](bool c) { m_exportObjects = c; });
    connect(ui->objInfo, &QCheckBox::toggled, [this](bool c) { m_exportObjInfo = c; });
}

SceneExportImgOptions::~SceneExportImgOptions() { delete ui; }
