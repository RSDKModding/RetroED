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
    connect(infoGroup[1], &Property::changed, [] {
        // entity slot was changed
        // TODO:
        // check if slot avaliable
        // if not, dont change slot
        // if it is, reset the linked entity slot and the new entity slot
        // and init the new entity slot's data to suit the type
    });

    QList<Property *> posGroup = { new Property("x", &entity->pos.x),
                                   new Property("y", &entity->pos.y) };

    connect(posGroup[0], &Property::changed, [entity] {
        if (entity->gameEntity) {
            entity->gameEntity->position.x = entity->pos.x * 65536.0f;
        }
    });

    connect(posGroup[1], &Property::changed, [entity] {
        if (entity->gameEntity) {
            entity->gameEntity->position.y = entity->pos.y * 65536.0f;
        }
    });

    for (int v = 0; v < entity->variables.count(); ++v) {
        auto &var = entity->variables[v];

        SceneObject *object = &(*objects)[entity->type];
        QString varName     = (*objects)[entity->type].variables[v].name;
        Property *group     = new Property(varName);
        QList<Property *> valGroup;

        QList<PropertyValue> aliases;

        if (aliases.count()) {
            // TODO
        }
        else {
            switch ((int)var.type) {
                default: break;
                case VAR_UINT8: {
                    valGroup.append(new Property("uint8", &entity->variables[v].value_uint8));
                    Property *prop = valGroup.last();
                    disconnect(prop, nullptr, nullptr, nullptr);
                    connect(prop, &Property::changed, [prop, v, entity, object] {
                        if (object && entity->gameEntity) {
                            byte *dataPtr = &((byte *)entity->gameEntity)[object->variables[v].offset];
                            memcpy(dataPtr, prop->valuePtr, sizeof(byte));
                        }
                    });
                    break;
                }
                case VAR_UINT16: {
                    valGroup.append(new Property("uint16", &entity->variables[v].value_uint16));
                    Property *prop = valGroup.last();
                    disconnect(prop, nullptr, nullptr, nullptr);
                    connect(prop, &Property::changed, [prop, v, entity, object] {
                        if (object && entity->gameEntity) {
                            byte *dataPtr = &((byte *)entity->gameEntity)[object->variables[v].offset];
                            memcpy(dataPtr, prop->valuePtr, sizeof(ushort));
                        }
                    });
                    break;
                }
                case VAR_UINT32: {
                    valGroup.append(new Property("uint32", &entity->variables[v].value_uint32));
                    Property *prop = valGroup.last();
                    disconnect(prop, nullptr, nullptr, nullptr);
                    connect(prop, &Property::changed, [prop, v, entity, object] {
                        if (object && entity->gameEntity) {
                            byte *dataPtr = &((byte *)entity->gameEntity)[object->variables[v].offset];
                            memcpy(dataPtr, prop->valuePtr, sizeof(ushort));
                        }
                    });
                    break;
                }
                case VAR_INT8: {
                    valGroup.append(new Property("int8", &entity->variables[v].value_int8));
                    Property *prop = valGroup.last();
                    disconnect(prop, nullptr, nullptr, nullptr);
                    connect(prop, &Property::changed, [prop, v, entity, object] {
                        if (object && entity->gameEntity) {
                            byte *dataPtr = &((byte *)entity->gameEntity)[object->variables[v].offset];
                            memcpy(dataPtr, prop->valuePtr, sizeof(sbyte));
                        }
                    });
                    break;
                }
                case VAR_INT16: {
                    valGroup.append(new Property("int16", &entity->variables[v].value_int16));
                    Property *prop = valGroup.last();
                    disconnect(prop, nullptr, nullptr, nullptr);
                    connect(prop, &Property::changed, [prop, v, entity, object] {
                        if (object && entity->gameEntity) {
                            byte *dataPtr = &((byte *)entity->gameEntity)[object->variables[v].offset];
                            memcpy(dataPtr, prop->valuePtr, sizeof(short));
                        }
                    });
                    break;
                }
                case VAR_INT32: {
                    valGroup.append(new Property("int32", &entity->variables[v].value_int32));
                    Property *prop = valGroup.last();
                    disconnect(prop, nullptr, nullptr, nullptr);
                    connect(prop, &Property::changed, [prop, v, entity, object] {
                        if (object && entity->gameEntity) {
                            byte *dataPtr = &((byte *)entity->gameEntity)[object->variables[v].offset];
                            memcpy(dataPtr, prop->valuePtr, sizeof(int));
                        }
                    });
                    break;
                }
                case VAR_ENUM: {
                    valGroup.append(new Property("enum", &entity->variables[v].value_enum));
                    Property *prop = valGroup.last();
                    disconnect(prop, nullptr, nullptr, nullptr);
                    connect(prop, &Property::changed, [prop, v, entity, object] {
                        if (object && entity->gameEntity) {
                            byte *dataPtr = &((byte *)entity->gameEntity)[object->variables[v].offset];
                            memcpy(dataPtr, prop->valuePtr, sizeof(int));
                        }
                    });
                    break;
                }
                case VAR_BOOL: {
                    valGroup.append(new Property("bool", &entity->variables[v].value_bool));
                    Property *prop = valGroup.last();
                    disconnect(prop, nullptr, nullptr, nullptr);
                    connect(prop, &Property::changed, [prop, v, entity, object] {
                        if (object && entity->gameEntity) {
                            byte *dataPtr = &((byte *)entity->gameEntity)[object->variables[v].offset];
                            bool32 val    = *((bool *)prop->valuePtr) != false;
                            memcpy(dataPtr, &val, sizeof(bool32));
                        }
                    });
                    break;
                }
                case VAR_STRING: {
                    valGroup.append(new Property("string", &entity->variables[v].value_string));
                    Property *prop = valGroup.last();
                    disconnect(prop, nullptr, nullptr, nullptr);
                    connect(prop, &Property::changed, [prop, v, entity, object] {
                        if (object && entity->gameEntity) {
                            TextInfo *dataPtr =
                                (TextInfo *)&((byte *)entity->gameEntity)[object->variables[v].offset];
                            QString val = *((QString *)prop->valuePtr);
                            FunctionTable::setText(dataPtr, (char *)val.toStdString().c_str(), false);
                        }
                    });
                    break;
                }
                case VAR_COLOUR: {
                    valGroup.append(new Property("colour", &entity->variables[v].value_color));
                    Property *prop = valGroup.last();
                    disconnect(prop, nullptr, nullptr, nullptr);
                    connect(prop, &Property::changed, [prop, v, entity, object] {
                        if (object && entity->gameEntity) {
                            byte *dataPtr = &((byte *)entity->gameEntity)[object->variables[v].offset];
                            QColor *val   = (QColor *)prop->valuePtr;
                            uint clr      = (val->red() << 16) | (val->green() << 8) | (val->blue());
                            memcpy(dataPtr, &clr, sizeof(uint));
                        }
                    });
                    break;
                }
                case VAR_VECTOR2: {
                    valGroup.append(new Property("x", &entity->variables[v].value_vector2.x));
                    Property *prop = valGroup.last();
                    disconnect(prop, nullptr, nullptr, nullptr);
                    connect(prop, &Property::changed, [prop, v, entity, object] {
                        if (object && entity->gameEntity) {
                            byte *dataPtr = &((byte *)entity->gameEntity)[object->variables[v].offset];
                            memcpy(dataPtr, prop->valuePtr, sizeof(int));
                        }
                    });

                    valGroup.append(new Property("y", &entity->variables[v].value_vector2.y));
                    prop = valGroup.last();
                    disconnect(prop, nullptr, nullptr, nullptr);
                    connect(prop, &Property::changed, [prop, v, entity, object] {
                        if (object && entity->gameEntity) {
                            byte *dataPtr = &(
                                (byte *)entity->gameEntity)[object->variables[v].offset + sizeof(int)];
                            memcpy(dataPtr, prop->valuePtr, sizeof(int));
                        }
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
