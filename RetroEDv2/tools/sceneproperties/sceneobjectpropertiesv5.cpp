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

void SceneObjectPropertiesv5::setupUI(SceneEntity *entity)
{
    unsetUI();

    QList<PropertyValue> objNames;
    for (int o = 0; o < v5Editor->viewer->objects.count(); ++o) {
        PropertyValue value;
        value.name  = v5Editor->viewer->objects[o].name;
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

    connect(infoGroup[0], &Property::changed, [this, infoGroup, entity] {
        byte type = *(byte *)infoGroup[0]->valuePtr;
        emit typeChanged(entity, type);
    });
    connect(infoGroup[1], &Property::changed, [this, entity, infoGroup] {
        bool flag = false;
        if (entity->slotID != entity->prevSlot) {
            for (auto &entityRef : v5Editor->viewer->entities) {
                if (entity->slotID == entityRef.slotID) {
                    msgBox = new QMessageBox(
                        QMessageBox::Information, "RetroED",
                        QString("An entity already exists with slotID %1.").arg(entity->slotID),
                        QMessageBox::NoButton, this);
                    msgBox->open();
                    entity->slotID = entity->prevSlot;
                    flag           = true;

                    infoGroup[1]->updateValue();
                    break;
                }
            }
        }
        if (!flag) {
            entity->prevSlot = entity->slotID;
        }
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

        SceneObject *object = &v5Editor->viewer->objects[entity->type];
        QString varName     = object->variables[v].name;
        Property *group     = new Property(varName);
        QList<Property *> valGroup;

        QList<PropertyValue> aliases;
        if (object) {
            for (int i = 0; i < object->variables.count(); ++i) {
                if (object->variables[i].name == varName) {
                    for (auto &value : object->variables[i].values) {
                        PropertyValue val;
                        val.name  = value.name;
                        val.value = value.value;
                        aliases.append(val);
                    }
                }
            }
        }

        {
            switch ((int)var.type) {
                default: break;
                case VAR_UINT8: {
                    if (aliases.count()) {
                        valGroup.append(new Property("uint8", aliases,
                                                     &entity->variables[v].value_uint8,
                                                     Property::BYTE_MANAGER));
                    }
                    else {
                        valGroup.append(new Property("uint8", &entity->variables[v].value_uint8));
                    }
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
                    if (aliases.count()) {
                        valGroup.append(new Property("uint16", aliases,
                                                     &entity->variables[v].value_uint16,
                                                     Property::USHORT_MANAGER));
                    }
                    else {
                        valGroup.append(new Property("uint16", &entity->variables[v].value_uint16));
                    }
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
                    if (aliases.count()) {
                        valGroup.append(new Property("uint32", aliases,
                                                     &entity->variables[v].value_uint32,
                                                     Property::UINT_MANAGER));
                    }
                    else {
                        valGroup.append(new Property("uint32", &entity->variables[v].value_uint32));
                    }
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
                    if (aliases.count()) {
                        valGroup.append(new Property("int8", aliases, &entity->variables[v].value_int8,
                                                     Property::SBYTE_MANAGER));
                    }
                    else {
                        valGroup.append(new Property("int8", &entity->variables[v].value_int8));
                    }
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
                    if (aliases.count()) {
                        valGroup.append(new Property("int16", aliases,
                                                     &entity->variables[v].value_int16,
                                                     Property::INT_MANAGER));
                    }
                    else {
                        valGroup.append(new Property("int16", &entity->variables[v].value_int16));
                    }
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
                    if (aliases.count()) {
                        valGroup.append(new Property("int32", aliases,
                                                     &entity->variables[v].value_int32,
                                                     Property::INT_MANAGER));
                    }
                    else {
                        valGroup.append(new Property("int32", &entity->variables[v].value_int32));
                    }
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
                    if (aliases.count()) {
                        valGroup.append(new Property("enum", aliases, &entity->variables[v].value_enum,
                                                     Property::INT_MANAGER));
                    }
                    else {
                        valGroup.append(new Property("enum", &entity->variables[v].value_enum));
                    }
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
                    valGroup.append(new Property("x", &entity->variables[v].value_vector2f.x));
                    Property *prop = valGroup.last();
                    disconnect(prop, nullptr, nullptr, nullptr);
                    connect(prop, &Property::changed, [v, entity, object] {
                        entity->variables[v].value_vector2.x =
                            entity->variables[v].value_vector2f.x * 65536.0f;
                        if (object && entity->gameEntity) {
                            int value     = entity->variables[v].value_vector2.x;
                            byte *dataPtr = &((byte *)entity->gameEntity)[object->variables[v].offset];
                            memcpy(dataPtr, &value, sizeof(int));
                        }
                    });

                    valGroup.append(new Property("y", &entity->variables[v].value_vector2f.y));
                    prop = valGroup.last();
                    disconnect(prop, nullptr, nullptr, nullptr);
                    connect(prop, &Property::changed, [v, entity, object] {
                        entity->variables[v].value_vector2.y =
                            entity->variables[v].value_vector2f.y * 65536.0f;
                        if (object && entity->gameEntity) {
                            int value     = entity->variables[v].value_vector2.y;
                            byte *dataPtr = &(
                                (byte *)entity->gameEntity)[object->variables[v].offset + sizeof(int)];
                            memcpy(dataPtr, &value, sizeof(int));
                        }
                    });
                    break;
                }
                case VAR_UNKNOWN: {
                    valGroup.append(new Property("unknown", &entity->variables[v].value_unknown));
                    Property *prop = valGroup.last();
                    disconnect(prop, nullptr, nullptr, nullptr);
                    connect(prop, &Property::changed, [prop, v, entity, object] {
                        if (object && entity->gameEntity) {
                            byte *dataPtr = &((byte *)entity->gameEntity)[object->variables[v].offset];
                            memcpy(dataPtr, &prop->valuePtr, sizeof(int));
                        }
                    });
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
