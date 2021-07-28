#include "includes.hpp"
#include "ui_sceneobjectpropertiesv5.h"

SceneObjectPropertiesv5::SceneObjectPropertiesv5(QWidget *parent)
    : QWidget(parent), ui(new Ui::SceneObjectPropertiesv5)
{
    ui->setupUi(this);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, QOverload<>::of(&SceneObjectPropertiesv5::updateUI));
    timer->start(1000.0f / 60.0f);

    properties = new PropertyBrowser;
    ui->gridLayout->addWidget(properties);
}

SceneObjectPropertiesv5::~SceneObjectPropertiesv5() { delete ui; }

void SceneObjectPropertiesv5::setupUI(QList<SceneObject> *objects, SceneEntity *entity)
{
    unsetUI();

    QList<PropertyValue> objNames;
    for (int o = 0; o < (*objects).count(); ++o) {
        PropertyValue value;
        value.name  = (*objects)[o].name;
        value.value = o;
        objNames.append(value);
    }

    QList<Property *> entityGroup = {
        new Property("object"),
        new Property("position"),
    };

    QList<Property *> infoGroup = {
        new Property("type", objNames, &entity->type, Property::BYTE_MANAGER),
        new Property("slot", &entity->slotID),
    };

    connect(infoGroup[0], &Property::changed, [] {
        // entity type was changed
        // TODO: clear & reset variables
    });

    QList<Property *> posGroup = { new Property("x", &entity->pos.x),
                                   new Property("y", &entity->pos.y) };

    connect(posGroup[0], &Property::changed, [this] { /*TODO*/ });

    connect(posGroup[1], &Property::changed, [this] { /*TODO*/ });

    for (int v = 0; v < entity->variables.count(); ++v) {
        auto &var = entity->variables[v];

        QString varName = (*objects)[entity->type].variables[v].name;
        Property *group = new Property(varName);
        QList<Property *> valGroup;

        // ushort id           = entity->type;
        // GameObjectInfo *obj = NULL;

        QList<PropertyValue> aliases;

        if (aliases.count()) {
            // TODO
        }
        else {
            switch ((int)var.m_type) {
                default: break;
                case VAR_UINT8: {
                    valGroup.append(new Property("uint8", &entity->variables[v].value_uint8));
                    Property *prop = valGroup.last();
                    disconnect(prop, nullptr, nullptr, nullptr);
                    connect(prop, &Property::changed, [prop, v, entity] {
                        // byte *val                        = (byte *)prop->valuePtr;
                    });
                    break;
                }
                case VAR_UINT16: {
                    valGroup.append(new Property("uint16", &entity->variables[v].value_uint16));
                    Property *prop = valGroup.last();
                    disconnect(prop, nullptr, nullptr, nullptr);
                    connect(prop, &Property::changed, [prop, v, entity] {
                        // ushort *val                       = (ushort *)prop->valuePtr;
                    });
                    break;
                }
                case VAR_UINT32: {
                    valGroup.append(new Property("uint32", &entity->variables[v].value_uint32));
                    Property *prop = valGroup.last();
                    disconnect(prop, nullptr, nullptr, nullptr);
                    connect(prop, &Property::changed, [prop, v, entity] {
                        // uint *val                        = (uint *)prop->valuePtr;
                    });
                    break;
                }
                case VAR_INT8: {
                    valGroup.append(new Property("int8", &entity->variables[v].value_int8));
                    Property *prop = valGroup.last();
                    disconnect(prop, nullptr, nullptr, nullptr);
                    connect(prop, &Property::changed, [prop, v, entity] {
                        // sbyte *val                      = (sbyte *)prop->valuePtr;
                    });
                    break;
                }
                case VAR_INT16: {
                    valGroup.append(new Property("int16", &entity->variables[v].value_int16));
                    Property *prop = valGroup.last();
                    disconnect(prop, nullptr, nullptr, nullptr);
                    connect(prop, &Property::changed, [prop, v, entity] {
                        // short *val                       = (short *)prop->valuePtr;
                    });
                    break;
                }
                case VAR_INT32: {
                    valGroup.append(new Property("int32", &entity->variables[v].value_int32));
                    Property *prop = valGroup.last();
                    disconnect(prop, nullptr, nullptr, nullptr);
                    connect(prop, &Property::changed, [prop, v, entity] {
                        // int *val                         = (int *)prop->valuePtr;
                    });
                    break;
                }
                case VAR_ENUM: {
                    valGroup.append(new Property("enum", &entity->variables[v].value_enum));
                    Property *prop = valGroup.last();
                    disconnect(prop, nullptr, nullptr, nullptr);
                    connect(prop, &Property::changed, [prop, v, entity] {
                        // int *val                        = (int *)prop->valuePtr;/
                    });
                    break;
                }
                case VAR_BOOL: {
                    valGroup.append(new Property("bool", &entity->variables[v].value_bool));
                    Property *prop = valGroup.last();
                    disconnect(prop, nullptr, nullptr, nullptr);
                    connect(prop, &Property::changed, [prop, v, entity] {
                        // int *val                        = (int *)prop->valuePtr;
                    });
                    break;
                }
                case VAR_STRING: {
                    valGroup.append(new Property("string", &entity->variables[v].value_string));
                    Property *prop = valGroup.last();
                    disconnect(prop, nullptr, nullptr, nullptr);
                    connect(prop, &Property::changed, [prop, v, entity] {
                        // QString *val                        = (QString *)prop->valuePtr;
                    });
                    break;
                }
                case VAR_COLOUR: {
                    valGroup.append(new Property("colour", &entity->variables[v].value_color));
                    Property *prop = valGroup.last();
                    disconnect(prop, nullptr, nullptr, nullptr);
                    connect(prop, &Property::changed, [prop, v, entity] {
                        // QColor *val                        = (QColor *)prop->valuePtr;
                    });
                    break;
                }
                case VAR_VECTOR2: {
                    valGroup.append(new Property("x", &entity->variables[v].value_vector2.x));
                    Property *prop = valGroup.last();
                    disconnect(prop, nullptr, nullptr, nullptr);
                    connect(prop, &Property::changed, [prop, v, entity] {
                        // int *val                        = (int *)prop->valuePtr;
                    });

                    valGroup.append(new Property("y", &entity->variables[v].value_vector2.y));
                    prop = valGroup.last();
                    disconnect(prop, nullptr, nullptr, nullptr);
                    connect(prop, &Property::changed, [prop, v, entity] {
                        // int *val                        = (int *)prop->valuePtr;
                    });
                    break;
                }
                case VAR_UNKNOWN: {
                    // no clue
                    break;
                }
            }
        }

        group->setSubProperties(valGroup);
        entityGroup.append(group);
    }

    entityGroup[0]->setSubProperties(infoGroup);
    entityGroup[1]->setSubProperties(posGroup);
    properties->setPropertySet(entityGroup);

    entityPtr = entity;
}

void SceneObjectPropertiesv5::unsetUI()
{
    properties->clear();

    // m_ver = ver;
    entityPtr = nullptr;
}

void SceneObjectPropertiesv5::updateUI()
{
    if (!entityPtr)
        return;
}

#include "moc_sceneobjectpropertiesv5.cpp"
