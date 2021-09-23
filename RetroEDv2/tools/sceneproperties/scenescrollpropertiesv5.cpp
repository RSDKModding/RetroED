#include "includes.hpp"
#include "ui_scenescrollpropertiesv5.h"

SceneScrollPropertiesv5::SceneScrollPropertiesv5(QWidget *parent)
    : QWidget(parent), ui(new Ui::SceneScrollPropertiesv5)
{
    ui->setupUi(this);
}

SceneScrollPropertiesv5::~SceneScrollPropertiesv5() { delete ui; }

void SceneScrollPropertiesv5::setupUI(RSDKv5::Scene::ScrollIndexInfo *info)
{
    unsetUI();

    ui->startLine->setValue(info->startLine);
    ui->length->setValue(info->length);
    ui->relSpeed->setValue(info->parallaxFactor);
    ui->constSpeed->setValue(info->scrollSpeed);
    ui->behaviour->setCurrentIndex(info->deform);

    connect(ui->startLine, QOverload<int>::of(&QSpinBox::valueChanged),
            [info](int v) { info->startLine = v; });
    connect(ui->length, QOverload<int>::of(&QSpinBox::valueChanged),
            [info](int v) { info->length = v; });
    connect(ui->relSpeed, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            [info](double v) { info->parallaxFactor = v; });
    connect(ui->constSpeed, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            [info](double v) { info->scrollSpeed = v; });
    connect(ui->behaviour, QOverload<int>::of(&QComboBox::currentIndexChanged),
            [info](int v) { info->deform = (byte)v; });
}

void SceneScrollPropertiesv5::unsetUI()
{
    disconnect(ui->startLine, nullptr, nullptr, nullptr);
    disconnect(ui->length, nullptr, nullptr, nullptr);
    disconnect(ui->relSpeed, nullptr, nullptr, nullptr);
    disconnect(ui->constSpeed, nullptr, nullptr, nullptr);
    disconnect(ui->behaviour, nullptr, nullptr, nullptr);
}

#include "moc_scenescrollpropertiesv5.cpp"
