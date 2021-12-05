#include "includes.hpp"
#include "ui_scenestamppropertiesv5.h"

SceneStampPropertiesv5::SceneStampPropertiesv5(QWidget *parent)
    : QWidget(parent), ui(new Ui::SceneStampPropertiesv5)
{
    ui->setupUi(this);
}

SceneStampPropertiesv5::~SceneStampPropertiesv5() { delete ui; }

void SceneStampPropertiesv5::setupUI(RSDKv5::Stamps *stamps, int id)
{
    unsetUI();

    if (!stamps || id < 0)
        return;

    stampsPtr = stamps;

    ui->stampName->setText(stamps->stampList[id].name);

    ui->stampX->setValue(stamps->stampList[id].pos.x);
    ui->stampY->setValue(stamps->stampList[id].pos.y);
    ui->stampSizeX->setValue(stamps->stampList[id].size.x);
    ui->stampSizeY->setValue(stamps->stampList[id].size.y);

    ui->stampID->setText("Stamp ID: " + QString::number(id));

    connect(ui->stampName, &QLineEdit::textChanged,
            [stamps, id](QString s) { stamps->stampList[id].name = s; });

    connect(ui->stampX, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            [stamps, id](double v) { stamps->stampList[id].pos.x = v; });

    connect(ui->stampY, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            [stamps, id](double v) { stamps->stampList[id].pos.y = v; });

    connect(ui->stampSizeX, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            [stamps, id](double v) { stamps->stampList[id].size.x = v; });

    connect(ui->stampSizeY, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            [stamps, id](double v) { stamps->stampList[id].size.y = v; });
}
void SceneStampPropertiesv5::unsetUI()
{
    if (!stampsPtr) // already unset
        return;

    ui->stampID->setText("Stamp ID: ");

    disconnect(ui->stampName, nullptr, nullptr, nullptr);
    disconnect(ui->stampX, nullptr, nullptr, nullptr);
    disconnect(ui->stampY, nullptr, nullptr, nullptr);
    disconnect(ui->stampSizeX, nullptr, nullptr, nullptr);
    disconnect(ui->stampSizeY, nullptr, nullptr, nullptr);

    stampsPtr = nullptr;
}

#include "moc_scenestamppropertiesv5.cpp"
