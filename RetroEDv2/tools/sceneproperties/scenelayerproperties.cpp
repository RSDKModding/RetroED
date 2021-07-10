#include "includes.hpp"
#include "ui_scenelayerproperties.h"

SceneLayerProperties::SceneLayerProperties(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SceneLayerProperties)
{
    ui->setupUi(this);
}

SceneLayerProperties::~SceneLayerProperties()
{
    delete ui;
}

void SceneLayerProperties::setupUI(FormatHelpers::Scene *scn, FormatHelpers::Background *bg, byte lID, byte ver)
{
    unsetUI();

    if (lID == 0) {
        ui->behaviour->setDisabled(true);
        ui->relSpeed->setDisabled(true);
        ui->constSpeed->setDisabled(true);

        ui->width->setValue(scn->width);
        ui->height->setValue(scn->height);
        ui->behaviour->setCurrentIndex(1);
        ui->relSpeed->setValue(1.0f);
        ui->constSpeed->setValue(0.0f);


        connect(ui->width, QOverload<int>::of(&QSpinBox::valueChanged),
                [scn](int v) {
            if (v > scn->width) {
                for (int h = 0; h < scn->height; ++h) {
                    for (int l = scn->width - 1; l < v; ++l)
                        scn->layout[h].append(QList<ushort>());
                }
            }
            else if (v < scn->width) {
                for (int h = 0; h < scn->height; ++h) {
                    for (int l = scn->width - 1; l >= v; --l)
                        scn->layout[h].removeAt(l);
                }
            }

            scn->width = (short)v;
        });

        connect(ui->height, QOverload<int>::of(&QSpinBox::valueChanged),
                [scn](int v) {
            if (v > scn->height) {
                for (int l = scn->height - 1; l < v; ++l)
                    scn->layout.append(QList<ushort>());
            }
            else if (v < scn->height) {
                for (int l = scn->height - 1; l >= v; --l)
                    scn->layout.removeAt(l);
            }

            scn->height = (short)v;
        });
    }
    else {
        ui->behaviour->setDisabled(false);
        ui->relSpeed->setDisabled(false);
        ui->constSpeed->setDisabled(false);

        ui->width->setValue(bg->layers[lID - 1].width);
        ui->height->setValue(bg->layers[lID - 1].height);
        ui->behaviour->setCurrentIndex(bg->layers[lID - 1].behaviour);
        ui->relSpeed->setValue(bg->layers[lID - 1].m_relativeSpeed);
        ui->constSpeed->setValue(bg->layers[lID - 1].m_constantSpeed);


        connect(ui->width, QOverload<int>::of(&QSpinBox::valueChanged),
                [bg, lID](int v) {
            if (v > bg->layers[lID - 1].width) {
                for (int h = 0; h < bg->layers[lID - 1].height; ++h) {
                    for (int l = bg->layers[lID - 1].width - 1; l < v; ++l)
                        bg->layers[lID - 1].layout[h].append(QList<ushort>());
                }
            }
            else if (v < bg->layers[lID - 1].width) {
                for (int h = 0; h < bg->layers[lID - 1].height; ++h) {
                    for (int l = bg->layers[lID - 1].width - 1; l >= v; --l)
                        bg->layers[lID - 1].layout[h].removeAt(l);
                }
            }

            if (bg->layers[lID - 1].behaviour == 2)
                bg->layers[lID - 1].m_lineIndexes.resize(v * 0x80);

            bg->layers[lID - 1].width = (short)v;
         });

        connect(ui->height, QOverload<int>::of(&QSpinBox::valueChanged),
                [bg, lID](int v) {
            if (v > bg->layers[lID - 1].height) {
                for (int l = bg->layers[lID - 1].height - 1; l < v; ++l)
                    bg->layers[lID - 1].layout.append(QList<ushort>());
            }
            else if (v < bg->layers[lID - 1].height) {
                for (int l = bg->layers[lID - 1].height - 1; l >= v; --l)
                    bg->layers[lID - 1].layout.removeAt(l);
            }

            if (bg->layers[lID - 1].behaviour == 1)
                bg->layers[lID - 1].m_lineIndexes.resize(v * 0x80);

            bg->layers[lID - 1].height = (short)v;
         });

        connect(ui->behaviour, QOverload<int>::of(&QComboBox::currentIndexChanged),
                [bg, lID](int v) { bg->layers[lID - 1].behaviour = (byte)v; });

        connect(ui->relSpeed, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
                [bg, lID](double v) { bg->layers[lID - 1].m_relativeSpeed = v; });

        connect(ui->constSpeed, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
                [bg, lID](double v) { bg->layers[lID - 1].m_constantSpeed = v; });
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
