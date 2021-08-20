#include "includes.hpp"
#include "ui_scenescrollproperties.h"

SceneScrollProperties::SceneScrollProperties(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SceneScrollProperties)
{
    ui->setupUi(this);
}

SceneScrollProperties::~SceneScrollProperties()
{
    delete ui;
}

void SceneScrollProperties::setupUI(FormatHelpers::Background::ScrollIndexInfo *info)
{
    unsetUI();

    ui->relSpeed->setValue(info->parallaxFactor);
    ui->constSpeed->setValue(info->scrollSpeed);
    ui->behaviour->setCurrentIndex(info->deform);

    connect(ui->relSpeed, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [info](double v) {
        info->parallaxFactor = v;
    });
    connect(ui->constSpeed, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [info](double v) {
        info->scrollSpeed = v;
    });
    connect(ui->behaviour, QOverload<int>::of(&QComboBox::currentIndexChanged),
            [info](int v) { info->deform = (byte)v; });
}

void SceneScrollProperties::unsetUI()
{
    disconnect(ui->relSpeed, nullptr, nullptr, nullptr);
    disconnect(ui->constSpeed, nullptr, nullptr, nullptr);
    disconnect(ui->behaviour, nullptr, nullptr, nullptr);
}

#include "moc_scenescrollproperties.cpp"
