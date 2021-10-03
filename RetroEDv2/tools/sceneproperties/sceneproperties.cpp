#include "includes.hpp"
#include "ui_sceneproperties.h"

SceneProperties::SceneProperties(QWidget *parent) : QWidget(parent), ui(new Ui::SceneProperties)
{
    ui->setupUi(this);

    musBox = ui->music;

    loadGlobalCB = ui->loadGlobals;

    editTIL  = ui->editChunks;
    editSCF  = ui->editStageconfig;
    editPAL  = ui->editPalette;
    editTSet = ui->editTSet;
}

SceneProperties::~SceneProperties() { delete ui; }

void SceneProperties::setupUI(FormatHelpers::Scene *scn, byte ver)
{
    unsetUI();

    if (!scn)
        return;

    ui->title->setText(scn->title);

    connect(ui->title, &QLineEdit::textEdited, [scn](QString s) { scn->title = s; });

    if (ver != ENGINE_v1) {
        ui->background->setDisabled(true);
        ui->music->setDisabled(true);
        ui->plrX->setDisabled(true);
        ui->plrY->setDisabled(true);

        ui->activeLayer0->setDisabled(false);
        ui->activeLayer1->setDisabled(false);
        ui->activeLayer2->setDisabled(false);
        ui->activeLayer3->setDisabled(false);
        ui->layerMidpoint->setDisabled(false);

        ui->activeLayer0->setCurrentIndex(scn->activeLayer[0]);
        ui->activeLayer1->setCurrentIndex(scn->activeLayer[1]);
        ui->activeLayer2->setCurrentIndex(scn->activeLayer[2]);
        ui->activeLayer3->setCurrentIndex(scn->activeLayer[3]);
        ui->layerMidpoint->setCurrentIndex(scn->midpoint);

        connect(ui->activeLayer0, QOverload<int>::of(&QComboBox::currentIndexChanged),
                [scn](int v) { scn->activeLayer[0] = (byte)v; });
        connect(ui->activeLayer1, QOverload<int>::of(&QComboBox::currentIndexChanged),
                [scn](int v) { scn->activeLayer[1] = (byte)v; });
        connect(ui->activeLayer2, QOverload<int>::of(&QComboBox::currentIndexChanged),
                [scn](int v) { scn->activeLayer[2] = (byte)v; });
        connect(ui->activeLayer3, QOverload<int>::of(&QComboBox::currentIndexChanged),
                [scn](int v) { scn->activeLayer[3] = (byte)v; });
        connect(ui->layerMidpoint, QOverload<int>::of(&QComboBox::currentIndexChanged),
                [scn](int v) { scn->midpoint = (byte)v; });
    }
    else {
        ui->background->setDisabled(false);
        ui->music->setDisabled(false);
        ui->plrX->setDisabled(false);
        ui->plrY->setDisabled(false);

        ui->activeLayer0->setDisabled(true);
        ui->activeLayer1->setDisabled(true);
        ui->activeLayer2->setDisabled(true);
        ui->activeLayer3->setDisabled(true);
        ui->layerMidpoint->setDisabled(true);

        ui->background->setCurrentIndex(scn->backgroundID - 1);
        ui->music->setCurrentIndex(scn->musicID);
        ui->plrX->setValue(scn->playerX);
        ui->plrY->setValue(scn->playerY);

        connect(ui->background, QOverload<int>::of(&QComboBox::currentIndexChanged),
                [scn](int v) { scn->backgroundID = (byte)(v + 1); });
        connect(ui->music, QOverload<int>::of(&QComboBox::currentIndexChanged),
                [scn](int v) { scn->musicID = (byte)v; });
        connect(ui->plrX, QOverload<int>::of(&QSpinBox::valueChanged),
                [scn](int v) { scn->playerX = (short)v; });
        connect(ui->plrY, QOverload<int>::of(&QSpinBox::valueChanged),
                [scn](int v) { scn->playerY = (short)v; });
    }
}

void SceneProperties::unsetUI()
{
    disconnect(ui->title, nullptr, nullptr, nullptr);

    disconnect(ui->background, nullptr, nullptr, nullptr);
    disconnect(ui->music, nullptr, nullptr, nullptr);
    disconnect(ui->plrX, nullptr, nullptr, nullptr);
    disconnect(ui->plrY, nullptr, nullptr, nullptr);

    disconnect(ui->activeLayer0, nullptr, nullptr, nullptr);
    disconnect(ui->activeLayer1, nullptr, nullptr, nullptr);
    disconnect(ui->activeLayer2, nullptr, nullptr, nullptr);
    disconnect(ui->activeLayer3, nullptr, nullptr, nullptr);
    disconnect(ui->layerMidpoint, nullptr, nullptr, nullptr);
}

#include "moc_sceneproperties.cpp"
