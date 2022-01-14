#include "includes.hpp"
#include "ui_sceneobjectproperties.h"

SceneObjectProperties::SceneObjectProperties(QWidget *parent)
    : QWidget(parent), ui(new Ui::SceneObjectProperties)
{
    ui->setupUi(this);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, QOverload<>::of(&SceneObjectProperties::updateUI));
    timer->start(1000.0f / 60.0f);

    properties = new PropertyBrowser;
    ui->gridLayout->addWidget(properties);
}

SceneObjectProperties::~SceneObjectProperties() { delete ui; }

void SceneObjectProperties::setupUI(SceneViewer::EntityInfo *entity, int entityID,
                                    Compilerv2::Entity *entityv2, Compilerv3::Entity *entityv3,
                                    Compilerv4::Entity *entityv4, byte ver)
{
    unsetUI();

    SceneViewer::ObjectInfo &object = scnEditor->viewer->objects[entity->type];

    QList<PropertyValue> objNames;
    for (int o = 0; o < scnEditor->viewer->objects.count(); ++o) {
        PropertyValue value;
        value.name  = scnEditor->viewer->objects[o].name;
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
        new Property(object.variablesAliases[SceneViewer::VAR_ALIAS_PROPVAL], &entity->propertyValue),
    };

    connect(infoGroup[0], &Property::changed, [infoGroup, entity, entityv2, entityv3, entityv4] {
        byte type    = *(byte *)infoGroup[0]->valuePtr;
        entity->type = type;
        if (entityv2)
            entityv2->type = type;
        if (entityv3)
            entityv3->type = type;
        if (entityv4)
            entityv4->type = type;
    });
    connect(infoGroup[1], &Property::changed, [this, entity, infoGroup] {
        bool flag = false;
        if (entity->slotID != entity->prevSlotID) {
            for (auto &entityRef : scnEditor->viewer->entities) {
                if (entity->slotID == entityRef.slotID) {
                    msgBox = new QMessageBox(
                        QMessageBox::Information, "RetroED",
                        QString("An entity already exists with slotID %1.").arg(entity->slotID),
                        QMessageBox::NoButton, this);
                    msgBox->open();
                    entity->slotID = entity->prevSlotID;
                    flag           = true;

                    infoGroup[1]->updateValue();
                    break;
                }
            }
        }
        if (!flag) {
            entity->prevSlotID = entity->slotID;
        }
    });
    connect(infoGroup[2], &Property::changed,
            [infoGroup, entity, entityv2, entityv3, entityv4, entityID] {
                byte propVal          = *(byte *)infoGroup[2]->valuePtr;
                entity->propertyValue = propVal;
                if (entityv2)
                    entityv2->propertyValue = propVal;
                if (entityv3)
                    entityv3->propertyValue = propVal;
                if (entityv4)
                    entityv4->propertyValue = propVal;

                scnEditor->viewer->variableID    = -1;      // prop val
                scnEditor->viewer->variableValue = propVal; // prop val
                scnEditor->viewer->callGameEvent(SceneViewer::EVENT_RSDKEDIT, entityID);
            });

    QList<Property *> posGroup = { new Property("x", &entity->pos.x),
                                   new Property("y", &entity->pos.y) };

    connect(posGroup[0], &Property::changed, [entity, entityv2, entityv3, entityv4] {
        if (entityv2)
            entityv2->XPos = entity->pos.x * 65536.0f;
        if (entityv3)
            entityv3->XPos = entity->pos.x * 65536.0f;
        if (entityv4)
            entityv4->XPos = entity->pos.x * 65536.0f;
    });

    connect(posGroup[1], &Property::changed, [entity, entityv2, entityv3, entityv4] {
        if (entityv2)
            entityv2->YPos = entity->pos.y * 65536.0f;
        if (entityv3)
            entityv3->YPos = entity->pos.y * 65536.0f;
        if (entityv4)
            entityv4->YPos = entity->pos.y * 65536.0f;
    });

    if (ver == ENGINE_v4) {

        QList<PropertyValue> flipFlags     = { PropertyValue("No Flip", 0), PropertyValue("Flip X", 1),
                                           PropertyValue("Flip Y", 2), PropertyValue("Flip XY", 3) };
        QList<PropertyValue> inkEffects    = { PropertyValue("No Ink", 0), PropertyValue("Blended", 1),
                                            PropertyValue("Alpha", 2), PropertyValue("Additive", 3),
                                            PropertyValue("Subtractive", 4) };
        QList<PropertyValue> priorityFlags = { PropertyValue("Bounds", 0),
                                               PropertyValue("Active", 1),
                                               PropertyValue("Active (Paused)", 2),
                                               PropertyValue("X Bounds", 3),
                                               PropertyValue("X Bounds (Remove)", 4),
                                               PropertyValue("Inactive", 5),
                                               PropertyValue("Bounds (Small)", 6),
                                               PropertyValue("Unknown", 7) };

        QList<Property *> varGroup = {};

        int *values[] = { &entityv4->state,
                          NULL,
                          &entityv4->scale,
                          &entityv4->rotation,
                          NULL,
                          NULL,
                          &entityv4->alpha,
                          NULL,
                          &entityv4->animationSpeed,
                          NULL,
                          NULL,
                          &entityv4->values[0],
                          &entityv4->values[1],
                          &entityv4->values[2],
                          &entityv4->values[3] };

        byte *valuesB[] = { NULL,
                            &entityv4->direction,
                            NULL,
                            NULL,
                            (byte *)&entityv4->drawOrder,
                            &entityv4->priority,
                            NULL,
                            &entityv4->animation,
                            NULL,
                            &entityv4->frame,
                            &entityv4->inkEffect,
                            NULL,
                            NULL,
                            NULL,
                            NULL };

        // Variables
        for (int v = 0; v < 0xF; ++v) {
            Property *group = new Property(RSDKv4::objectVariableNames[v]);

            Property *variable = NULL;

            switch (v) {
                default: {
                    QString name = RSDKv4::objectVariableTypes[v];
                    if (v >= 11)
                        name = object.variablesAliases[SceneViewer::VAR_ALIAS_VAL0 + (v - 11)];
                    variable = new Property(name, &entity->variables[v].value);
                    break;
                }
                case 1:
                    variable = new Property(RSDKv4::objectVariableTypes[v], flipFlags,
                                            &entity->variables[v].value, Property::BYTE_MANAGER);
                    break;
                case 5:
                    variable = new Property(RSDKv4::objectVariableTypes[v], priorityFlags,
                                            &entity->variables[v].value, Property::BYTE_MANAGER);
                    break;
                case 10:
                    variable = new Property(RSDKv4::objectVariableTypes[v], inkEffects,
                                            &entity->variables[v].value, Property::BYTE_MANAGER);
                    break;
            }

            QList<Property *> valGroup = { variable };

            disconnect(variable, nullptr, nullptr, nullptr);
            connect(variable, &Property::changed, [=] {
                entity->variables[v].value = *(int *)variable->valuePtr;
                if (values[v])
                    *values[v] = entity->variables[v].value;
                else
                    *valuesB[v] = entity->variables[v].value;

                entity->variables[v].active = entity->variables[v].value != 0;
            });

            group->setSubProperties(valGroup);
            varGroup.append(group);
        }

        entityGroup.append(new Property("Variables"));
        entityGroup[2]->setSubProperties(varGroup);
    }

    for (int v = 0; v < entity->customVars.count(); ++v) {
        auto &var = entity->customVars[v];

        SceneViewer::ObjectInfo *object = &scnEditor->viewer->objects[entity->type];
        QString varName                 = scnEditor->viewer->objects[entity->type].variables[v].name;
        Property *group                 = new Property(varName);
        QList<Property *> valGroup;

        QList<PropertyValue> aliases;
        if (object) {
            for (auto &value : object->variables[v].values) {
                PropertyValue val;
                val.name  = value.name;
                val.value = value.value;
                aliases.append(val);
            }
        }

        scnEditor->viewer->variableID                       = v; // var
        scnEditor->viewer->variableValue                    = 0;
        scnEditor->viewer->returnVariable                   = true;
        scnEditor->viewer->compilerv4.scriptEng.checkResult = -1;
        scnEditor->viewer->callGameEvent(SceneViewer::EVENT_RSDKEDIT, entityID);
        var.value_int32                   = scnEditor->viewer->compilerv4.scriptEng.checkResult;
        scnEditor->viewer->returnVariable = false;

        if (aliases.count()) {
            valGroup.append(new Property("enum", aliases, &var.value_int32, Property::INT_MANAGER));
        }
        else {
            valGroup.append(new Property("int32", &var.value_int32));
        }

        Property *prop = valGroup.last();
        disconnect(prop, nullptr, nullptr, nullptr);
        connect(prop, &Property::changed,
                [prop, &var, entityID, v, infoGroup, entity, entityv2, entityv3, entityv4, ver] {
                    var.value_int32                   = *(byte *)prop->valuePtr;
                    scnEditor->viewer->variableID     = v;
                    scnEditor->viewer->variableValue  = var.value_int32;
                    scnEditor->viewer->returnVariable = false;

                    if (entityv2)
                        entityv2->propertyValue = entity->propertyValue;
                    if (entityv3)
                        entityv3->propertyValue = entity->propertyValue;
                    if (entityv4)
                        entityv4->propertyValue = entity->propertyValue;

                    scnEditor->viewer->callGameEvent(SceneViewer::EVENT_RSDKEDIT, entityID);

                    switch (ver) {
                        case ENGINE_v2: entity->propertyValue = entityv2->propertyValue; break;
                        case ENGINE_v3: entity->propertyValue = entityv3->propertyValue; break;
                        case ENGINE_v4: entity->propertyValue = entityv4->propertyValue; break;
                    }

                    infoGroup[2]->updateValue();
                });

        group->setSubProperties(valGroup);
        entityGroup.append(group);
    }

    entityGroup[0]->setSubProperties(infoGroup);
    entityGroup[1]->setSubProperties(posGroup);
    properties->setPropertySet(entityGroup);

    entityPtr = entity;
}

void SceneObjectProperties::unsetUI()
{
    properties->clear();
    entityPtr = nullptr;
}

void SceneObjectProperties::updateUI()
{
    if (!entityPtr)
        return;
}

#include "moc_sceneobjectproperties.cpp"
