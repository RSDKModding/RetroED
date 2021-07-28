#include "includes.hpp"
#include "ui_scenelayerpropertiesv5.h"

SceneLayerPropertiesv5::SceneLayerPropertiesv5(QWidget *parent)
    : QWidget(parent), ui(new Ui::SceneLayerPropertiesv5)
{
    ui->setupUi(this);
}

SceneLayerPropertiesv5::~SceneLayerPropertiesv5() { delete ui; }

void SceneLayerPropertiesv5::setupUI(RSDKv5::Scene *scn, byte lID)
{
    unsetUI();

    ui->type->setDisabled(false);
    ui->relSpeed->setDisabled(false);
    ui->constSpeed->setDisabled(false);

    ui->width->setValue(scn->layers[lID].width);
    ui->height->setValue(scn->layers[lID].height);
    ui->type->setCurrentIndex(scn->layers[lID].type);
    ui->drawOrder->setValue(scn->layers[lID].drawOrder);
    ui->relSpeed->setValue(scn->layers[lID].parallaxFactor / 256.0f);
    ui->constSpeed->setValue(scn->layers[lID].scrollSpeed / 256.0f);

    connect(ui->width, QOverload<int>::of(&QSpinBox::valueChanged), [scn, lID](int v) {
        if (v > scn->layers[lID].width) {
            for (int h = 0; h < scn->layers[lID].height; ++h) {
                for (int l = scn->layers[lID].width - 1; l < v; ++l)
                    scn->layers[lID].layout[h].append(QVector<ushort>());
            }
        }
        else if (v < scn->layers[lID].width) {
            for (int h = 0; h < scn->layers[lID].height; ++h) {
                for (int l = scn->layers[lID].width - 1; l >= v; --l)
                    scn->layers[lID].layout[h].removeAt(l);
            }
        }

        if (scn->layers[lID].type == 1)
            scn->layers[lID].lineIndexes.resize(v * 0x10);

        scn->layers[lID].width = (short)v;
    });

    connect(ui->height, QOverload<int>::of(&QSpinBox::valueChanged), [scn, lID](int v) {
        if (v > scn->layers[lID].height) {
            for (int l = scn->layers[lID].height - 1; l < v; ++l)
                scn->layers[lID].layout.append(QVector<ushort>());
        }
        else if (v < scn->layers[lID].height) {
            for (int l = scn->layers[lID].height - 1; l >= v; --l) scn->layers[lID].layout.removeAt(l);
        }

        if (scn->layers[lID].type == 0)
            scn->layers[lID].lineIndexes.resize(v * 0x10);

        scn->layers[lID].height = (short)v;
    });

    connect(ui->type, QOverload<int>::of(&QComboBox::currentIndexChanged),
            [scn, lID](int v) { scn->layers[lID].type = (byte)v; });

    connect(ui->drawOrder, QOverload<int>::of(&QSpinBox::valueChanged),
            [scn, lID](int v) { scn->layers[lID].drawOrder = (byte)v; });

    connect(ui->relSpeed, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            [scn, lID](double v) { scn->layers[lID].parallaxFactor = v * 256; });

    connect(ui->constSpeed, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            [scn, lID](double v) { scn->layers[lID].scrollSpeed = v * 256; });
}

void SceneLayerPropertiesv5::unsetUI()
{
    disconnect(ui->width, nullptr, nullptr, nullptr);
    disconnect(ui->height, nullptr, nullptr, nullptr);
    disconnect(ui->type, nullptr, nullptr, nullptr);
    disconnect(ui->drawOrder, nullptr, nullptr, nullptr);
    disconnect(ui->relSpeed, nullptr, nullptr, nullptr);
    disconnect(ui->constSpeed, nullptr, nullptr, nullptr);
}

#include "moc_scenelayerpropertiesv5.cpp"
