#include "includes.hpp"
#include "ui_scenelayerpropertiesv5.h"
#include "scenelayerpropertiesv5.hpp"

#include "tools/sceneviewer.hpp"
#include "sceneincludesv5.hpp"

SceneLayerPropertiesv5::SceneLayerPropertiesv5(QWidget *parent)
    : QWidget(parent), ui(new Ui::SceneLayerPropertiesv5)
{
    ui->setupUi(this);
}

SceneLayerPropertiesv5::~SceneLayerPropertiesv5() { delete ui; }

void SceneLayerPropertiesv5::setupUI(SceneViewer *viewer, byte lID)
{
    unsetUI();

    auto *tileLayer = &viewer->layers[lID];

    ui->type->setDisabled(false);
    ui->parallaxFactor->setDisabled(false);
    ui->scrollSpeed->setDisabled(false);

    ui->name->setText(viewer->layers[lID].name);
    ui->width->setValue(viewer->layers[lID].width);
    ui->height->setValue(viewer->layers[lID].height);
    ui->type->setCurrentIndex(viewer->layers[lID].type);
    // Hide "None" layer type, since it doesn't exist in v5
    qobject_cast<QListView *>(ui->type->view())->setRowHidden(SceneHelpers::TileLayer::LAYER_NONE, true);
    ui->drawOrder->setValue(viewer->layers[lID].drawGroup);
    ui->parallaxFactor->setValue(viewer->layers[lID].parallaxFactor / 256.0f);
    ui->scrollSpeed->setValue(viewer->layers[lID].scrollSpeed / 256.0f);

    connect(ui->name, &QLineEdit::textChanged, [this, tileLayer](QString s){
        tileLayer->name = s;
        emit layerNameChanged(s);
    });
    connect(ui->width, QOverload<int>::of(&QSpinBox::valueChanged), [tileLayer](int v) {
        if (v > tileLayer->width) {
            for (int h = 0; h < tileLayer->height; ++h) {
                for (int w = tileLayer->width; w < v; ++w){
                    tileLayer->layout[h].append(0xFFFF);
                }
            }
        }
        else if (v < tileLayer->width) {
            for (int h = 0; h < tileLayer->height; ++h) {
                for (int w = tileLayer->width - 1; w >= v; --w)
                    tileLayer->layout[h].removeAt(w);
            }
        }

        tileLayer->width = (short)v;
    });

    connect(ui->height, QOverload<int>::of(&QSpinBox::valueChanged), [tileLayer](int v) {
        if (v > tileLayer->height) {
            for (int h = tileLayer->height; h < v; ++h) {
                tileLayer->layout.append(QList<ushort>());
                for (int w = 0; w < tileLayer->width; ++w){
                    tileLayer->layout[h].append(0xFFFF);
                }
            }
        }
        else if (v < tileLayer->height) {
            for (int h = tileLayer->height - 1; h >= v; --h)
                tileLayer->layout.removeAt(h);
        }

        tileLayer->height = (short)v;
    });

    connect(ui->type, QOverload<int>::of(&QComboBox::currentIndexChanged),
            [tileLayer](int v) { tileLayer->type = (byte)v; });

    connect(ui->drawOrder, QOverload<int>::of(&QSpinBox::valueChanged),
            [tileLayer](int v) { tileLayer->drawGroup = (byte)v; });

    connect(ui->parallaxFactor, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            [tileLayer](double v) { tileLayer->parallaxFactor = v * 256; });

    connect(ui->scrollSpeed, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            [tileLayer](double v) { tileLayer->scrollSpeed = v * 256; });
}

void SceneLayerPropertiesv5::unsetUI()
{
    disconnect(ui->name, nullptr, nullptr, nullptr);
    disconnect(ui->width, nullptr, nullptr, nullptr);
    disconnect(ui->height, nullptr, nullptr, nullptr);
    disconnect(ui->type, nullptr, nullptr, nullptr);
    disconnect(ui->drawOrder, nullptr, nullptr, nullptr);
    disconnect(ui->parallaxFactor, nullptr, nullptr, nullptr);
    disconnect(ui->scrollSpeed, nullptr, nullptr, nullptr);
}

#include "moc_scenelayerpropertiesv5.cpp"
