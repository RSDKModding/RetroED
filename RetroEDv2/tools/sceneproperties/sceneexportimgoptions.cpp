#include "includes.hpp"
#include "ui_sceneexportimgoptions.h"

SceneExportImgOptions::SceneExportImgOptions(QWidget *parent)
    : QDialog(parent), ui(new Ui::SceneExportImgOptions)
{
    ui->setupUi(this);

    memset(exportBG, 0, 8 * 2 * sizeof(bool));

    exportFG[0] = true;
    exportFG[1] = true;

    disconnect(ui->fgLow, nullptr, nullptr, nullptr);
    disconnect(ui->fgHigh, nullptr, nullptr, nullptr);
    connect(ui->fgLow, &QCheckBox::toggled, [this](bool c) { exportFG[0] = c; });
    connect(ui->fgHigh, &QCheckBox::toggled, [this](bool c) { exportFG[1] = c; });

    disconnect(ui->bg1Low, nullptr, nullptr, nullptr);
    connect(ui->bg1Low, &QCheckBox::toggled, [this](bool c) { exportBG[0][0] = c; });
    disconnect(ui->bg2Low, nullptr, nullptr, nullptr);
    connect(ui->bg2Low, &QCheckBox::toggled, [this](bool c) { exportBG[1][0] = c; });
    disconnect(ui->bg3Low, nullptr, nullptr, nullptr);
    connect(ui->bg3Low, &QCheckBox::toggled, [this](bool c) { exportBG[2][0] = c; });
    disconnect(ui->bg4Low, nullptr, nullptr, nullptr);
    connect(ui->bg4Low, &QCheckBox::toggled, [this](bool c) { exportBG[3][0] = c; });
    disconnect(ui->bg5Low, nullptr, nullptr, nullptr);
    connect(ui->bg5Low, &QCheckBox::toggled, [this](bool c) { exportBG[4][0] = c; });
    disconnect(ui->bg6Low, nullptr, nullptr, nullptr);
    connect(ui->bg6Low, &QCheckBox::toggled, [this](bool c) { exportBG[5][0] = c; });
    disconnect(ui->bg7Low, nullptr, nullptr, nullptr);
    connect(ui->bg7Low, &QCheckBox::toggled, [this](bool c) { exportBG[6][0] = c; });
    disconnect(ui->bg8Low, nullptr, nullptr, nullptr);
    connect(ui->bg8Low, &QCheckBox::toggled, [this](bool c) { exportBG[7][0] = c; });

    disconnect(ui->bg1High, nullptr, nullptr, nullptr);
    connect(ui->bg1High, &QCheckBox::toggled, [this](bool c) { exportBG[0][1] = c; });
    disconnect(ui->bg2High, nullptr, nullptr, nullptr);
    connect(ui->bg2High, &QCheckBox::toggled, [this](bool c) { exportBG[1][1] = c; });
    disconnect(ui->bg3High, nullptr, nullptr, nullptr);
    connect(ui->bg3High, &QCheckBox::toggled, [this](bool c) { exportBG[2][1] = c; });
    disconnect(ui->bg4High, nullptr, nullptr, nullptr);
    connect(ui->bg4High, &QCheckBox::toggled, [this](bool c) { exportBG[3][1] = c; });
    disconnect(ui->bg5High, nullptr, nullptr, nullptr);
    connect(ui->bg5High, &QCheckBox::toggled, [this](bool c) { exportBG[4][1] = c; });
    disconnect(ui->bg6High, nullptr, nullptr, nullptr);
    connect(ui->bg6High, &QCheckBox::toggled, [this](bool c) { exportBG[5][1] = c; });
    disconnect(ui->bg7High, nullptr, nullptr, nullptr);
    connect(ui->bg7High, &QCheckBox::toggled, [this](bool c) { exportBG[6][1] = c; });
    disconnect(ui->bg8High, nullptr, nullptr, nullptr);
    connect(ui->bg8High, &QCheckBox::toggled, [this](bool c) { exportBG[7][1] = c; });

    connect(ui->objects, &QCheckBox::toggled, [this](bool c) { exportObjects = c; });
    connect(ui->objInfo, &QCheckBox::toggled, [this](bool c) { exportObjInfo = c; });
}

SceneExportImgOptions::~SceneExportImgOptions() { delete ui; }
