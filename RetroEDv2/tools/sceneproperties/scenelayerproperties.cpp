#include "includes.hpp"
#include "ui_scenelayerproperties.h"

SceneLayerProperties::SceneLayerProperties(QWidget *parent)
    : QWidget(parent), ui(new Ui::SceneLayerProperties)
{
    ui->setupUi(this);
}

SceneLayerProperties::~SceneLayerProperties() { delete ui; }

void SceneLayerProperties::setupUI(FormatHelpers::Scene *scn, FormatHelpers::Background *bg, byte lID,
                                   byte ver)
{
    unsetUI();

    scene      = scn;
    background = bg;
    layerID    = lID;

    if (lID == 0) {
        ui->behaviour->setDisabled(true);
        ui->relSpeed->setDisabled(true);
        ui->constSpeed->setDisabled(true);

        ui->width->setValue(scene->width);
        ui->height->setValue(scene->height);
        ui->behaviour->setCurrentIndex(1);
        ui->relSpeed->setValue(1.0f);
        ui->constSpeed->setValue(0.0f);

        connect(ui->width, QOverload<int>::of(&QSpinBox::valueChanged), [this](int v) {
            if (v > scene->width) {
                for (int h = 0; h < scene->height; ++h) {
                    for (int w = scene->width; w < v; ++w) scene->layout[h].append(0);
                }
            }
            else if (v < scene->width) {
                for (int h = 0; h < scene->height; ++h) {
                    for (int w = scene->width - 1; w >= v; --w) scene->layout[h].removeAt(w);
                }
            }

            scene->width = (short)v;
        });

        connect(ui->height, QOverload<int>::of(&QSpinBox::valueChanged), [this](int v) {
            if (v > scene->height) {
                for (int h = scene->height; h < v; ++h) {
                    scene->layout.append(QList<ushort>());
                    for (int w = 0; w < scene->width; ++w) scene->layout[h].append(0);
                }
            }
            else if (v < scene->height) {
                for (int h = scene->height - 1; h >= v; --h) scene->layout.removeAt(h);
            }

            scene->height = (short)v;
        });
    }
    else {
        ui->behaviour->setDisabled(false);
        ui->relSpeed->setDisabled(false);
        ui->constSpeed->setDisabled(false);

        ui->width->setValue(background->layers[lID - 1].width);
        ui->height->setValue(background->layers[lID - 1].height);
        ui->behaviour->setCurrentIndex(background->layers[lID - 1].type);
        ui->relSpeed->setValue(background->layers[lID - 1].parallaxFactor);
        ui->constSpeed->setValue(background->layers[lID - 1].scrollSpeed);

        connect(ui->width, QOverload<int>::of(&QSpinBox::valueChanged), [this](int v) {
            if (v > background->layers[layerID - 1].width) {
                for (int h = 0; h < background->layers[layerID - 1].height; ++h) {
                    for (int w = background->layers[layerID - 1].width; w < v; ++w)
                        background->layers[layerID - 1].layout[h].append(0);
                }
            }
            else if (v < background->layers[layerID - 1].width) {
                for (int h = 0; h < background->layers[layerID - 1].height; ++h) {
                    for (int w = background->layers[layerID - 1].width - 1; w >= v; --w)
                        background->layers[layerID - 1].layout[h].removeAt(w);
                }
            }

            if (background->layers[layerID - 1].type == 2)
                background->layers[layerID - 1].lineIndexes.resize(v * 0x80);

            background->layers[layerID - 1].width = (short)v;
        });

        connect(ui->height, QOverload<int>::of(&QSpinBox::valueChanged), [this](int v) {
            if (v > background->layers[layerID - 1].height) {
                for (int h = background->layers[layerID - 1].height; h < v; ++h) {
                    background->layers[layerID - 1].layout.append(QList<ushort>());
                    for (int w = 0; w < background->layers[layerID - 1].width; ++w)
                        background->layers[layerID - 1].layout[h].append(0);
                }
            }
            else if (v < background->layers[layerID - 1].height) {
                for (int h = background->layers[layerID - 1].height - 1; h >= v; --h)
                    background->layers[layerID - 1].layout.removeAt(h);
            }

            if (background->layers[layerID - 1].type == 1)
                background->layers[layerID - 1].lineIndexes.resize(v * 0x80);

            background->layers[layerID - 1].height = (short)v;
        });

        connect(ui->behaviour, QOverload<int>::of(&QComboBox::currentIndexChanged),
                [this](int v) { background->layers[layerID - 1].type = (byte)v; });

        connect(ui->relSpeed, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
                [this](double v) { background->layers[layerID - 1].parallaxFactor = v; });

        connect(ui->constSpeed, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
                [this](double v) { background->layers[layerID - 1].scrollSpeed = v; });
    }
}

void SceneLayerProperties::unsetUI()
{
    disconnect(ui->width, nullptr, nullptr, nullptr);
    disconnect(ui->height, nullptr, nullptr, nullptr);
    disconnect(ui->behaviour, nullptr, nullptr, nullptr);
    disconnect(ui->relSpeed, nullptr, nullptr, nullptr);
    disconnect(ui->constSpeed, nullptr, nullptr, nullptr);
}

#include "moc_scenelayerproperties.cpp"
