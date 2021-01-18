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

    ui->relSpeed->setValue(info->m_relativeSpeed);
    ui->constSpeed->setValue(info->m_constantSpeed);
    ui->behaviour->setCurrentIndex(info->m_behaviour);

    connect(ui->relSpeed, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [info](double v) {
        info->m_relativeSpeed = v;
    });
    connect(ui->constSpeed, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [info](double v) {
        info->m_constantSpeed = v;
    });
    connect(ui->behaviour, QOverload<int>::of(&QComboBox::currentIndexChanged),
            [info](int v) { info->m_behaviour = (byte)v; });
}

void SceneScrollProperties::unsetUI()
{
    disconnect(ui->relSpeed, nullptr, nullptr, nullptr);
    disconnect(ui->constSpeed, nullptr, nullptr, nullptr);
    disconnect(ui->behaviour, nullptr, nullptr, nullptr);
}

#include "moc_scenescrollproperties.cpp"
