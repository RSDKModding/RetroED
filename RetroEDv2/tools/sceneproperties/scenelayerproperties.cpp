#include "includes.hpp"
#include "ui_scenelayerproperties.h"

SceneLayerProperties::SceneLayerProperties(QWidget *parent)
    : QWidget(parent), ui(new Ui::SceneLayerProperties)
{
    ui->setupUi(this);
}

SceneLayerProperties::~SceneLayerProperties() { delete ui; }

void SceneLayerProperties::setupUI(SceneViewer *viewer, byte layerID)
{
    unsetUI();

    auto *tileLayer = &viewer->layers[layerID];
    if (layerID == 0) {
        ui->type->setDisabled(true);
        ui->parallaxFactor->setDisabled(true);
        ui->scrollSpeed->setDisabled(true);

        ui->width->setValue(tileLayer->width);
        ui->height->setValue(tileLayer->height);
        ui->type->setCurrentIndex(1);
        ui->parallaxFactor->setValue(1.0f);
        ui->scrollSpeed->setValue(0.0f);
    }
    else {
        ui->type->setDisabled(false);
        ui->parallaxFactor->setDisabled(false);
        ui->scrollSpeed->setDisabled(false);

        ui->width->setValue(tileLayer->width);
        ui->height->setValue(tileLayer->height);
        ui->type->setCurrentIndex(tileLayer->type);
        ui->parallaxFactor->setValue(tileLayer->parallaxFactor);
        ui->scrollSpeed->setValue(tileLayer->scrollSpeed);
    }

    connect(ui->width, QOverload<int>::of(&QSpinBox::valueChanged), [tileLayer](int v) {
        if (v > tileLayer->width) {
            for (int h = 0; h < tileLayer->height; ++h) {
                for (int w = tileLayer->width; w < v; ++w) tileLayer->layout[h].append(0);
            }
        }
        else if (v < tileLayer->width) {
            for (int h = 0; h < tileLayer->height; ++h) {
                for (int w = tileLayer->width - 1; w >= v; --w) tileLayer->layout[h].removeAt(w);
            }
        }

        tileLayer->width = (short)v;
    });

    connect(ui->height, QOverload<int>::of(&QSpinBox::valueChanged), [tileLayer](int v) {
        if (v > tileLayer->height) {
            for (int h = tileLayer->height; h < v; ++h) {
                tileLayer->layout.append(QList<ushort>());
                for (int w = 0; w < tileLayer->width; ++w) tileLayer->layout[h].append(0);
            }
        }
        else if (v < tileLayer->height) {
            for (int h = tileLayer->height - 1; h >= v; --h) tileLayer->layout.removeAt(h);
        }

        tileLayer->height = (short)v;
    });

    if (layerID > 0) {
        connect(ui->type, QOverload<int>::of(&QComboBox::currentIndexChanged),
                [tileLayer](int v) { tileLayer->type = (byte)v; });

        connect(ui->parallaxFactor, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
                [tileLayer](double v) { tileLayer->parallaxFactor = v; });

        connect(ui->scrollSpeed, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
                [tileLayer](double v) { tileLayer->scrollSpeed = v; });
    }
}

void SceneLayerProperties::unsetUI()
{
    disconnect(ui->width, nullptr, nullptr, nullptr);
    disconnect(ui->height, nullptr, nullptr, nullptr);
    disconnect(ui->type, nullptr, nullptr, nullptr);
    disconnect(ui->parallaxFactor, nullptr, nullptr, nullptr);
    disconnect(ui->scrollSpeed, nullptr, nullptr, nullptr);
}

#include "moc_scenelayerproperties.cpp"
