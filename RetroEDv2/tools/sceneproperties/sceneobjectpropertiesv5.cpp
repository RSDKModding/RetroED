#include "includes.hpp"
#include "ui_sceneobjectpropertiesv5.h"

SceneObjectPropertiesv5::SceneObjectPropertiesv5(QWidget *parent)
    : QWidget(parent), ui(new Ui::SceneObjectPropertiesv5)
{
    ui->setupUi(this);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, QOverload<>::of(&SceneObjectPropertiesv5::updateUI));
    timer->start(1000.0f / 60.0f);

    typeBox = ui->type;
}

SceneObjectPropertiesv5::~SceneObjectPropertiesv5() { delete ui; }

void SceneObjectPropertiesv5::setupUI(SceneEntity *entity)
{
    unsetUI();

    ui->type->setCurrentIndex(entity->type);
    ui->slot->setValue(entity->slotID);

    ui->posX->setValue(entity->pos.x);
    ui->posY->setValue(entity->pos.y);

    connect(ui->type, QOverload<int>::of(&QComboBox::currentIndexChanged),
            [entity](int v) { entity->type = (byte)v; });
    connect(ui->slot, QOverload<int>::of(&QSpinBox::valueChanged),
            [entity](int v) { entity->slotID = v; });

    connect(ui->posX, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            [entity](double v) { entity->pos.x = v; });
    connect(ui->posY, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            [entity](double v) { entity->pos.y = v; });

    entityPtr = entity;
}

void SceneObjectPropertiesv5::unsetUI()
{
    disconnect(ui->type, nullptr, nullptr, nullptr);
    disconnect(ui->slot, nullptr, nullptr, nullptr);
    disconnect(ui->posX, nullptr, nullptr, nullptr);
    disconnect(ui->posY, nullptr, nullptr, nullptr);

    // m_ver = ver;
    entityPtr = nullptr;
}

void SceneObjectPropertiesv5::updateUI()
{
    if (!entityPtr)
        return;

    ui->type->setCurrentIndex(entityPtr->type);
    ui->slot->setValue(entityPtr->slotID);

    ui->posX->setValue(entityPtr->pos.x);
    ui->posY->setValue(entityPtr->pos.y);
}

#include "moc_sceneobjectpropertiesv5.cpp"
