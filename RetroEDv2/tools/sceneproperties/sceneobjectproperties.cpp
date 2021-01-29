#include "includes.hpp"
#include "ui_sceneobjectproperties.h"

SceneObjectProperties::SceneObjectProperties(QWidget *parent)
    : QWidget(parent), ui(new Ui::SceneObjectProperties)
{
    ui->setupUi(this);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, QOverload<>::of(&SceneObjectProperties::updateUI));
    timer->start(1000.0f / 60.0f);

    m_typeBox = ui->type;
}

SceneObjectProperties::~SceneObjectProperties() { delete ui; }

void SceneObjectProperties::setupUI(FormatHelpers::Scene::Object *obj, Compilerv3::Entity *entityv3,
                                    Compilerv4::Entity *entityv4, byte ver)
{
    unsetUI();

    ui->type->setCurrentIndex(obj->m_type);
    ui->subtype->setValue(obj->m_subtype);

    ui->posX->setValue(obj->getX());
    ui->posY->setValue(obj->getY());

    connect(ui->type, QOverload<int>::of(&QComboBox::currentIndexChanged),
            [obj, entityv3, entityv4](int v) {
                obj->m_type    = (byte)v;
                entityv3->type = (byte)v;
                entityv4->type = (byte)v;
            });
    connect(ui->subtype, QOverload<int>::of(&QSpinBox::valueChanged), [obj, entityv3, entityv4](int v) {
        obj->m_subtype          = (byte)v;
        entityv3->propertyValue = (byte)v;
        entityv4->propertyValue = (byte)v;
    });

    connect(ui->posX, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            [obj, entityv3, entityv4](double v) {
                obj->setX(v);
                entityv3->XPos = (int)v * 65536.0f;
                entityv4->XPos = (int)v * 65536.0f;
            });
    connect(ui->posY, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            [obj, entityv3, entityv4](double v) {
                obj->setY(v);
                entityv3->YPos = (int)v * 65536.0f;
                entityv4->YPos = (int)v * 65536.0f;
            });

    if (ver == ENGINE_v4) {
        ui->attribBox->setDisabled(false);

        ui->stateActive->setChecked(obj->m_attributes[0].m_active);
        ui->flipActive->setChecked(obj->m_attributes[1].m_active);
        ui->scaleActive->setChecked(obj->m_attributes[2].m_active);
        ui->rotationActive->setChecked(obj->m_attributes[3].m_active);
        ui->drawOrderActive->setChecked(obj->m_attributes[4].m_active);
        ui->priorityActive->setChecked(obj->m_attributes[5].m_active);
        ui->alphaActive->setChecked(obj->m_attributes[6].m_active);
        ui->animActive->setChecked(obj->m_attributes[7].m_active);
        ui->animSpeedActive->setChecked(obj->m_attributes[8].m_active);
        ui->frameActive->setChecked(obj->m_attributes[9].m_active);
        ui->inkEffectActive->setChecked(obj->m_attributes[10].m_active);
        ui->val1Active->setChecked(obj->m_attributes[11].m_active);
        ui->val2Active->setChecked(obj->m_attributes[12].m_active);
        ui->val3Active->setChecked(obj->m_attributes[13].m_active);
        ui->val4Active->setChecked(obj->m_attributes[14].m_active);

        ui->state->setValue(obj->m_attributes[0].m_value);
        ui->flip->setCurrentIndex(obj->m_attributes[1].m_value);
        ui->scale->setValue(obj->m_attributes[2].m_value / 512.0f);
        ui->rotation->setValue(obj->m_attributes[3].m_value);
        ui->drawOrder->setValue(obj->m_attributes[4].m_value);
        ui->priority->setCurrentIndex(obj->m_attributes[5].m_value);
        ui->alpha->setValue(obj->m_attributes[6].m_value);
        ui->animation->setValue(obj->m_attributes[7].m_value);
        ui->animSpeed->setValue(obj->m_attributes[8].m_value);
        ui->frame->setValue(obj->m_attributes[9].m_value);
        ui->inkEffect->setCurrentIndex(obj->m_attributes[10].m_value);
        ui->value1->setValue(obj->m_attributes[11].m_value);
        ui->value2->setValue(obj->m_attributes[12].m_value);
        ui->value3->setValue(obj->m_attributes[13].m_value);
        ui->value4->setValue(obj->m_attributes[14].m_value);

        connect(ui->stateActive, &QCheckBox::toggled,
                [obj, entityv3, entityv4](bool v) { obj->m_attributes[0].m_active = v; });
        connect(ui->flipActive, &QCheckBox::toggled,
                [obj, entityv3, entityv4](bool v) { obj->m_attributes[1].m_active = v; });
        connect(ui->scaleActive, &QCheckBox::toggled,
                [obj, entityv3, entityv4](bool v) { obj->m_attributes[2].m_active = v; });
        connect(ui->rotationActive, &QCheckBox::toggled,
                [obj, entityv3, entityv4](bool v) { obj->m_attributes[3].m_active = v; });
        connect(ui->drawOrderActive, &QCheckBox::toggled,
                [obj, entityv3, entityv4](bool v) { obj->m_attributes[4].m_active = v; });
        connect(ui->priorityActive, &QCheckBox::toggled,
                [obj, entityv3, entityv4](bool v) { obj->m_attributes[5].m_active = v; });
        connect(ui->alphaActive, &QCheckBox::toggled,
                [obj, entityv3, entityv4](bool v) { obj->m_attributes[6].m_active = v; });
        connect(ui->animActive, &QCheckBox::toggled,
                [obj, entityv3, entityv4](bool v) { obj->m_attributes[7].m_active = v; });
        connect(ui->animSpeedActive, &QCheckBox::toggled,
                [obj, entityv3, entityv4](bool v) { obj->m_attributes[8].m_active = v; });
        connect(ui->frameActive, &QCheckBox::toggled,
                [obj, entityv3, entityv4](bool v) { obj->m_attributes[9].m_active = v; });
        connect(ui->inkEffectActive, &QCheckBox::toggled,
                [obj, entityv3, entityv4](bool v) { obj->m_attributes[10].m_active = v; });
        connect(ui->val1Active, &QCheckBox::toggled,
                [obj, entityv3, entityv4](bool v) { obj->m_attributes[11].m_active = v; });
        connect(ui->val2Active, &QCheckBox::toggled,
                [obj, entityv3, entityv4](bool v) { obj->m_attributes[12].m_active = v; });
        connect(ui->val3Active, &QCheckBox::toggled,
                [obj, entityv3, entityv4](bool v) { obj->m_attributes[13].m_active = v; });
        connect(ui->val4Active, &QCheckBox::toggled,
                [obj, entityv3, entityv4](bool v) { obj->m_attributes[14].m_active = v; });

        connect(ui->state, QOverload<int>::of(&QSpinBox::valueChanged),
                [obj, entityv3, entityv4](int v) {
                    obj->m_attributes[0].m_value = v;
                    entityv3->state              = v;
                    entityv4->state              = v;
                });

        connect(ui->flip, QOverload<int>::of(&QComboBox::currentIndexChanged),
                [obj, entityv3, entityv4](int v) {
                    obj->m_attributes[1].m_value = (byte)v;
                    entityv3->direction          = (byte)v;
                    entityv4->direction          = (byte)v;
                });

        connect(ui->scale, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
                [obj, entityv3, entityv4](double v) {
                    obj->m_attributes[2].m_value = v * 0x200;
                    entityv3->scale              = v * 0x20;
                    entityv4->scale              = v * 0x20;
                });

        connect(ui->rotation, QOverload<int>::of(&QSpinBox::valueChanged),
                [obj, entityv3, entityv4](int v) {
                    obj->m_attributes[3].m_value = (byte)v;
                    entityv3->rotation           = (byte)v;
                    entityv4->rotation           = (byte)v;
                });

        connect(ui->drawOrder, QOverload<int>::of(&QSpinBox::valueChanged),
                [obj, entityv3, entityv4](int v) {
                    obj->m_attributes[4].m_value = (byte)v;
                    entityv3->drawOrder          = (byte)v;
                    entityv4->drawOrder          = (byte)v;
                });

        connect(ui->priority, QOverload<int>::of(&QComboBox::currentIndexChanged),
                [obj, entityv3, entityv4](int v) {
                    obj->m_attributes[5].m_value = (byte)v;
                    entityv3->priority           = (byte)v;
                    entityv4->priority           = (byte)v;
                });

        connect(ui->alpha, QOverload<int>::of(&QSpinBox::valueChanged),
                [obj, entityv3, entityv4](int v) {
                    obj->m_attributes[6].m_value = (byte)v;
                    entityv3->alpha              = (byte)v;
                    entityv4->alpha              = (byte)v;
                });

        connect(ui->animation, QOverload<int>::of(&QSpinBox::valueChanged),
                [obj, entityv3, entityv4](int v) {
                    obj->m_attributes[7].m_value = (byte)v;
                    entityv3->animation          = (byte)v;
                    entityv4->animation          = (byte)v;
                });

        connect(ui->animSpeed, QOverload<int>::of(&QSpinBox::valueChanged),
                [obj, entityv3, entityv4](int v) {
                    obj->m_attributes[8].m_value = (byte)v;
                    entityv3->animationSpeed     = (byte)v;
                    entityv4->animationSpeed     = (byte)v;
                });

        connect(ui->frame, QOverload<int>::of(&QSpinBox::valueChanged),
                [obj, entityv3, entityv4](int v) {
                    obj->m_attributes[9].m_value = (byte)v;
                    entityv3->frame              = (byte)v;
                    entityv4->frame              = (byte)v;
                });

        connect(ui->inkEffect, QOverload<int>::of(&QComboBox::currentIndexChanged),
                [obj, entityv3, entityv4](int v) {
                    obj->m_attributes[10].m_value = (byte)v;
                    entityv3->inkEffect           = (byte)v;
                    entityv4->inkEffect           = (byte)v;
                });

        connect(ui->value1, QOverload<int>::of(&QSpinBox::valueChanged),
                [obj, entityv3, entityv4](int v) {
                    obj->m_attributes[11].m_value = v;
                    entityv3->values[0]           = v;
                    entityv4->values[0]           = v;
                });

        connect(ui->value2, QOverload<int>::of(&QSpinBox::valueChanged),
                [obj, entityv3, entityv4](int v) {
                    obj->m_attributes[12].m_value = v;
                    entityv3->values[1]           = v;
                    entityv4->values[1]           = v;
                });

        connect(ui->value3, QOverload<int>::of(&QSpinBox::valueChanged),
                [obj, entityv3, entityv4](int v) {
                    obj->m_attributes[13].m_value = v;
                    entityv3->values[2]           = v;
                    entityv4->values[2]           = v;
                });

        connect(ui->value4, QOverload<int>::of(&QSpinBox::valueChanged),
                [obj, entityv3, entityv4](int v) {
                    obj->m_attributes[14].m_value = v;
                    entityv3->values[3]           = v;
                    entityv4->values[3]           = v;
                });
    }
    else {
        ui->attribBox->setDisabled(true);
    }

    m_obj = obj;
}

void SceneObjectProperties::unsetUI()
{
    disconnect(ui->type, nullptr, nullptr, nullptr);
    disconnect(ui->subtype, nullptr, nullptr, nullptr);
    disconnect(ui->posX, nullptr, nullptr, nullptr);
    disconnect(ui->posY, nullptr, nullptr, nullptr);

    disconnect(ui->stateActive, nullptr, nullptr, nullptr);
    disconnect(ui->flipActive, nullptr, nullptr, nullptr);
    disconnect(ui->scaleActive, nullptr, nullptr, nullptr);
    disconnect(ui->rotationActive, nullptr, nullptr, nullptr);
    disconnect(ui->drawOrderActive, nullptr, nullptr, nullptr);
    disconnect(ui->priorityActive, nullptr, nullptr, nullptr);
    disconnect(ui->alphaActive, nullptr, nullptr, nullptr);
    disconnect(ui->animActive, nullptr, nullptr, nullptr);
    disconnect(ui->animSpeedActive, nullptr, nullptr, nullptr);
    disconnect(ui->frameActive, nullptr, nullptr, nullptr);
    disconnect(ui->inkEffectActive, nullptr, nullptr, nullptr);
    disconnect(ui->val1Active, nullptr, nullptr, nullptr);
    disconnect(ui->val2Active, nullptr, nullptr, nullptr);
    disconnect(ui->val3Active, nullptr, nullptr, nullptr);
    disconnect(ui->val4Active, nullptr, nullptr, nullptr);

    disconnect(ui->state, nullptr, nullptr, nullptr);
    disconnect(ui->flip, nullptr, nullptr, nullptr);
    disconnect(ui->scale, nullptr, nullptr, nullptr);
    disconnect(ui->rotation, nullptr, nullptr, nullptr);
    disconnect(ui->drawOrder, nullptr, nullptr, nullptr);
    disconnect(ui->priority, nullptr, nullptr, nullptr);
    disconnect(ui->alpha, nullptr, nullptr, nullptr);
    disconnect(ui->animation, nullptr, nullptr, nullptr);
    disconnect(ui->animSpeed, nullptr, nullptr, nullptr);
    disconnect(ui->frame, nullptr, nullptr, nullptr);
    disconnect(ui->inkEffect, nullptr, nullptr, nullptr);
    disconnect(ui->value1, nullptr, nullptr, nullptr);
    disconnect(ui->value2, nullptr, nullptr, nullptr);
    disconnect(ui->value3, nullptr, nullptr, nullptr);
    disconnect(ui->value4, nullptr, nullptr, nullptr);

    // m_ver = ver;
    m_obj = nullptr;
}

void SceneObjectProperties::updateUI()
{
    if (!m_obj)
        return;

    ui->type->setCurrentIndex(m_obj->m_type);
    ui->subtype->setValue(m_obj->m_subtype);

    ui->posX->setValue(m_obj->getX());
    ui->posY->setValue(m_obj->getY());

    // if (ver == ENGINE_v4) {
    // ui->attribBox->setDisabled(false);

    ui->stateActive->setChecked(m_obj->m_attributes[0].m_active);
    ui->flipActive->setChecked(m_obj->m_attributes[1].m_active);
    ui->scaleActive->setChecked(m_obj->m_attributes[2].m_active);
    ui->rotationActive->setChecked(m_obj->m_attributes[3].m_active);
    ui->drawOrderActive->setChecked(m_obj->m_attributes[4].m_active);
    ui->priorityActive->setChecked(m_obj->m_attributes[5].m_active);
    ui->alphaActive->setChecked(m_obj->m_attributes[6].m_active);
    ui->animActive->setChecked(m_obj->m_attributes[7].m_active);
    ui->animSpeedActive->setChecked(m_obj->m_attributes[8].m_active);
    ui->frameActive->setChecked(m_obj->m_attributes[9].m_active);
    ui->inkEffectActive->setChecked(m_obj->m_attributes[10].m_active);
    ui->val1Active->setChecked(m_obj->m_attributes[11].m_active);
    ui->val2Active->setChecked(m_obj->m_attributes[12].m_active);
    ui->val3Active->setChecked(m_obj->m_attributes[13].m_active);
    ui->val4Active->setChecked(m_obj->m_attributes[14].m_active);

    ui->state->setValue(m_obj->m_attributes[0].m_value);
    ui->flip->setCurrentIndex(m_obj->m_attributes[1].m_value);
    ui->scale->setValue(m_obj->m_attributes[2].m_value / 512.0f);
    ui->rotation->setValue(m_obj->m_attributes[3].m_value);
    ui->drawOrder->setValue(m_obj->m_attributes[4].m_value);
    ui->priority->setCurrentIndex(m_obj->m_attributes[5].m_value);
    ui->alpha->setValue(m_obj->m_attributes[6].m_value);
    ui->animation->setValue(m_obj->m_attributes[7].m_value);
    ui->animSpeed->setValue(m_obj->m_attributes[8].m_value);
    ui->frame->setValue(m_obj->m_attributes[9].m_value);
    ui->inkEffect->setCurrentIndex(m_obj->m_attributes[10].m_value);
    ui->value1->setValue(m_obj->m_attributes[11].m_value);
    ui->value2->setValue(m_obj->m_attributes[12].m_value);
    ui->value3->setValue(m_obj->m_attributes[13].m_value);
    ui->value4->setValue(m_obj->m_attributes[14].m_value);
    //}
}

#include "moc_sceneobjectproperties.cpp"
