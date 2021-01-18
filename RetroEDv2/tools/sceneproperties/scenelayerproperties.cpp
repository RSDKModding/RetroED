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

        ui->width->setValue(scn->m_width);
        ui->height->setValue(scn->m_height);
        ui->behaviour->setCurrentIndex(1);
        ui->relSpeed->setValue(1.0f);
        ui->constSpeed->setValue(0.0f);


        connect(ui->width, QOverload<int>::of(&QSpinBox::valueChanged),
                [scn](int v) {
            if (v > scn->m_width) {
                for (int h = 0; h < scn->m_height; ++h) {
                    for (int l = scn->m_width - 1; l < v; ++l)
                        scn->m_layout[h].append(QList<ushort>());
                }
            }
            else if (v < scn->m_width) {
                for (int h = 0; h < scn->m_height; ++h) {
                    for (int l = scn->m_width - 1; l >= v; --l)
                        scn->m_layout[h].removeAt(l);
                }
            }

            scn->m_width = (short)v;
        });

        connect(ui->height, QOverload<int>::of(&QSpinBox::valueChanged),
                [scn](int v) {
            if (v > scn->m_height) {
                for (int l = scn->m_height - 1; l < v; ++l)
                    scn->m_layout.append(QList<ushort>());
            }
            else if (v < scn->m_height) {
                for (int l = scn->m_height - 1; l >= v; --l)
                    scn->m_layout.removeAt(l);
            }

            scn->m_height = (short)v;
        });
    }
    else {
        ui->behaviour->setDisabled(false);
        ui->relSpeed->setDisabled(false);
        ui->constSpeed->setDisabled(false);

        ui->width->setValue(bg->m_layers[lID - 1].m_width);
        ui->height->setValue(bg->m_layers[lID - 1].m_height);
        ui->behaviour->setCurrentIndex(bg->m_layers[lID - 1].m_behaviour);
        ui->relSpeed->setValue(bg->m_layers[lID - 1].m_relativeSpeed);
        ui->constSpeed->setValue(bg->m_layers[lID - 1].m_constantSpeed);


        connect(ui->width, QOverload<int>::of(&QSpinBox::valueChanged),
                [bg, lID](int v) {
            if (v > bg->m_layers[lID - 1].m_width) {
                for (int h = 0; h < bg->m_layers[lID - 1].m_height; ++h) {
                    for (int l = bg->m_layers[lID - 1].m_width - 1; l < v; ++l)
                        bg->m_layers[lID - 1].m_layout[h].append(QList<ushort>());
                }
            }
            else if (v < bg->m_layers[lID - 1].m_width) {
                for (int h = 0; h < bg->m_layers[lID - 1].m_height; ++h) {
                    for (int l = bg->m_layers[lID - 1].m_width - 1; l >= v; --l)
                        bg->m_layers[lID - 1].m_layout[h].removeAt(l);
                }
            }

            if (bg->m_layers[lID - 1].m_behaviour == 2)
                bg->m_layers[lID - 1].m_lineIndexes.resize(v * 0x80);

            bg->m_layers[lID - 1].m_width = (short)v;
         });

        connect(ui->height, QOverload<int>::of(&QSpinBox::valueChanged),
                [bg, lID](int v) {
            if (v > bg->m_layers[lID - 1].m_height) {
                for (int l = bg->m_layers[lID - 1].m_height - 1; l < v; ++l)
                    bg->m_layers[lID - 1].m_layout.append(QList<ushort>());
            }
            else if (v < bg->m_layers[lID - 1].m_height) {
                for (int l = bg->m_layers[lID - 1].m_height - 1; l >= v; --l)
                    bg->m_layers[lID - 1].m_layout.removeAt(l);
            }

            if (bg->m_layers[lID - 1].m_behaviour == 1)
                bg->m_layers[lID - 1].m_lineIndexes.resize(v * 0x80);

            bg->m_layers[lID - 1].m_height = (short)v;
         });

        connect(ui->behaviour, QOverload<int>::of(&QComboBox::currentIndexChanged),
                [bg, lID](int v) { bg->m_layers[lID - 1].m_behaviour = (byte)v; });

        connect(ui->relSpeed, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
                [bg, lID](double v) { bg->m_layers[lID - 1].m_relativeSpeed = v; });

        connect(ui->constSpeed, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
                [bg, lID](double v) { bg->m_layers[lID - 1].m_constantSpeed = v; });
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
