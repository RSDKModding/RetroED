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

    scene   = scn;
    layerID = lID;

    ui->type->setDisabled(false);
    ui->parallaxFactor->setDisabled(false);
    ui->scrollSpeed->setDisabled(false);

    ui->width->setValue(scene->layers[lID].width);
    ui->height->setValue(scene->layers[lID].height);
    ui->type->setCurrentIndex(scene->layers[lID].type);
    ui->drawOrder->setValue(scene->layers[lID].drawGroup);
    ui->parallaxFactor->setValue(scene->layers[lID].parallaxFactor / 256.0f);
    ui->scrollSpeed->setValue(scene->layers[lID].scrollSpeed / 256.0f);

    connect(ui->width, QOverload<int>::of(&QSpinBox::valueChanged), [this](int v) {
        if (v > scene->layers[layerID].width) {
            for (int h = 0; h < scene->layers[layerID].height; ++h) {
                for (int w = scene->layers[layerID].width; w < v; ++w)
                    scene->layers[layerID].layout[h].append(0);
            }
        }
        else if (v < scene->layers[layerID].width) {
            for (int h = 0; h < scene->layers[layerID].height; ++h) {
                for (int w = scene->layers[layerID].width - 1; w >= v; --w)
                    scene->layers[layerID].layout[h].removeAt(w);
            }
        }

        if (scene->layers[layerID].type == 1)
            scene->layers[layerID].lineScroll.resize(v * 0x10);

        scene->layers[layerID].width = (short)v;
    });

    connect(ui->height, QOverload<int>::of(&QSpinBox::valueChanged), [this](int v) {
        if (v > scene->layers[layerID].height) {
            for (int h = scene->layers[layerID].height; h < v; ++h) {
                scene->layers[layerID].layout.append(QVector<ushort>());
                for (int w = 0; w < scene->layers[layerID].width; ++w)
                    scene->layers[layerID].layout[h].append(0);
            }
        }
        else if (v < scene->layers[layerID].height) {
            for (int h = scene->layers[layerID].height - 1; h >= v; --h)
                scene->layers[layerID].layout.removeAt(h);
        }

        if (scene->layers[layerID].type == 0)
            scene->layers[layerID].lineScroll.resize(v * 0x10);

        scene->layers[layerID].height = (short)v;
    });

    connect(ui->type, QOverload<int>::of(&QComboBox::currentIndexChanged),
            [this](int v) { scene->layers[layerID].type = (byte)v; });

    connect(ui->drawOrder, QOverload<int>::of(&QSpinBox::valueChanged),
            [this](int v) { scene->layers[layerID].drawGroup = (byte)v; });

    connect(ui->parallaxFactor, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            [this](double v) { scene->layers[layerID].parallaxFactor = v * 256; });

    connect(ui->scrollSpeed, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            [this](double v) { scene->layers[layerID].scrollSpeed = v * 256; });
}

void SceneLayerPropertiesv5::unsetUI()
{
    disconnect(ui->width, nullptr, nullptr, nullptr);
    disconnect(ui->height, nullptr, nullptr, nullptr);
    disconnect(ui->type, nullptr, nullptr, nullptr);
    disconnect(ui->drawOrder, nullptr, nullptr, nullptr);
    disconnect(ui->parallaxFactor, nullptr, nullptr, nullptr);
    disconnect(ui->scrollSpeed, nullptr, nullptr, nullptr);
}

#include "moc_scenelayerpropertiesv5.cpp"
