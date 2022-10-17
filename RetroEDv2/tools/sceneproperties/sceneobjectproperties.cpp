#include "includes.hpp"
#include "ui_sceneobjectproperties.h"

SceneObjectProperties::SceneObjectProperties(QWidget *parent)
    : QWidget(parent), ui(new Ui::SceneObjectProperties)
{
    ui->setupUi(this);

    properties = new PropertyBrowser;
    ui->gridLayout->addWidget(properties);
}

SceneObjectProperties::~SceneObjectProperties() { delete ui; }

void SceneObjectProperties::setupUI(SceneEntity *entity, int entityID, Compilerv2::Entity *entityv2,
                                    Compilerv3::Entity *entityv3, Compilerv4::Entity *entityv4,
                                    byte ver)
{
    unsetUI();

    SceneObject &object = scnEditor->viewer->objects[entity->type];

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
        new Property(object.variablesAliases[VAR_ALIAS_PROPVAL], &entity->propertyValue),
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
        if (entity->slotID != entity->prevSlot) {
            for (auto &entityRef : scnEditor->viewer->entities) {
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

    connect(infoGroup[2], &Property::changed,
            [this, infoGroup, entity, entityv2, entityv3, entityv4, entityID, ver] {
                byte propVal = *(byte *)infoGroup[2]->valuePtr;

                // we set propertyValue via this so the game can run logic on it
                bool called = false;
                callRSDKEdit(scnEditor, false, entityID, -1, propVal, &called);

                if (called) {
                    if (ver == ENGINE_v3)
                        propVal = entityv3->propertyValue;
                    if (ver == ENGINE_v4)
                        propVal = entityv4->propertyValue;
                }

                entity->propertyValue = propVal;

                if (entityv2)
                    entityv2->propertyValue = propVal;
                if (entityv3)
                    entityv3->propertyValue = propVal;
                if (entityv4)
                    entityv4->propertyValue = propVal;

                // in case that changed any custom vars
                int startGroup = ver == ENGINE_v4 ? 3 : 2;
                for (int i = startGroup; i < properties->propertySet.count(); ++i) {
                    auto &var = entity->variables[i - startGroup];
                    var.value_int32 =
                        callRSDKEdit(scnEditor, true, entityID, i - startGroup, var.value_int32);

                    switch (ver) {
                        case ENGINE_v2: entity->propertyValue = entityv2->propertyValue; break;
                        case ENGINE_v3: entity->propertyValue = entityv3->propertyValue; break;
                        case ENGINE_v4: entity->propertyValue = entityv4->propertyValue; break;
                    }

                    properties->propertySet[i]->subProperties[0]->updateValue();
                }

                infoGroup[2]->updateValue(); // in case it changed
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
                                               PropertyValue("Always", 2),
                                               PropertyValue("X Bounds", 3),
                                               PropertyValue("X Bounds (Destroy)", 4),
                                               PropertyValue("Inactive", 5),
                                               PropertyValue("Bounds (Small)", 6),
                                               PropertyValue("Active (Small)", 7) };

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
            QString name = RSDKv4::objectVariableNames[v];
            if (v >= 11)
                name = object.variablesAliases[VAR_ALIAS_VAL0 + (v - 11)];

            Property *group = new Property(name);

            Property *variable = NULL;

            switch (v) {
                default: {
                    variable =
                        new Property(RSDKv4::objectVariableTypes[v], &entity->sceneVariables[v].value);
                    break;
                }

                case 1:
                    variable = new Property(RSDKv4::objectVariableTypes[v], flipFlags,
                                            &entity->sceneVariables[v].value, Property::BYTE_MANAGER);
                    break;
                case 5:
                    variable = new Property(RSDKv4::objectVariableTypes[v], priorityFlags,
                                            &entity->sceneVariables[v].value, Property::BYTE_MANAGER);
                    break;
                case 10:
                    variable = new Property(RSDKv4::objectVariableTypes[v], inkEffects,
                                            &entity->sceneVariables[v].value, Property::BYTE_MANAGER);
                    break;
            }

            QList<Property *> valGroup = { variable };

            disconnect(variable, nullptr, nullptr, nullptr);
            connect(variable, &Property::changed, [=] {
                entity->sceneVariables[v].value = *(int *)variable->valuePtr;
                if (values[v])
                    *values[v] = entity->sceneVariables[v].value;
                else
                    *valuesB[v] = entity->sceneVariables[v].value;

                entity->sceneVariables[v].active = entity->sceneVariables[v].value != 0;
            });

            group->setSubProperties(valGroup);
            varGroup.append(group);
        }

        entityGroup.append(new Property("Variables"));
        entityGroup[2]->setSubProperties(varGroup);
    }

    for (int v = 0; v < entity->variables.count(); ++v) {
        auto &var = entity->variables[v];

        SceneObject *object   = &scnEditor->viewer->objects[entity->type];
        VariableInfo &varInfo = scnEditor->viewer->objects[entity->type].variables[v];
        Property *group       = new Property(varInfo.name);
        QList<Property *> valGroup;

        QList<PropertyValue> aliases;
        if (object) {
            for (auto &value : varInfo.values) {
                PropertyValue val;
                val.name  = value.name;
                val.value = value.value;
                aliases.append(val);
            }
        }

        var.value_int32 = callRSDKEdit(scnEditor, true, entityID, v, 0);

        if (aliases.count()) {
            valGroup.append(new Property("enum", aliases, &var.value_int32, Property::INT_MANAGER));
        }
        else {
            valGroup.append(new Property("int32", &var.value_int32));
        }

        Property *prop = valGroup.last();
        disconnect(prop, nullptr, nullptr, nullptr);
        connect(prop, &Property::changed,
                [this, prop, &var, entityID, v, infoGroup, entity, entityv2, entityv3, entityv4, ver] {
                    var.value_int32 = *(byte *)prop->valuePtr;

                    if (entityv2)
                        entityv2->propertyValue = entity->propertyValue;
                    if (entityv3)
                        entityv3->propertyValue = entity->propertyValue;
                    if (entityv4)
                        entityv4->propertyValue = entity->propertyValue;

                    callRSDKEdit(scnEditor, false, entityID, v, var.value_int32);

                    switch (ver) {
                        case ENGINE_v2: entity->propertyValue = entityv2->propertyValue; break;
                        case ENGINE_v3: entity->propertyValue = entityv3->propertyValue; break;
                        case ENGINE_v4: entity->propertyValue = entityv4->propertyValue; break;
                    }

                    // in case that changed any custom vars
                    int startGroup = ver == ENGINE_v4 ? 3 : 2;
                    for (int i = startGroup; i < properties->propertySet.count(); ++i) {
                        if (i == startGroup + v)
                            continue; // dont update ourselves lol

                        auto &var = entity->variables[i - startGroup];
                        var.value_int32 =
                            callRSDKEdit(scnEditor, true, entityID, i - startGroup, var.value_int32);

                        switch (ver) {
                            case ENGINE_v2: entity->propertyValue = entityv2->propertyValue; break;
                            case ENGINE_v3: entity->propertyValue = entityv3->propertyValue; break;
                            case ENGINE_v4: entity->propertyValue = entityv4->propertyValue; break;
                        }

                        properties->propertySet[i]->subProperties[0]->updateValue();
                    }

                    // Update propertyVal
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

    properties->propertySet[1]->subProperties[0]->updateValue();
    properties->propertySet[1]->subProperties[1]->updateValue();
}

int SceneObjectProperties::callRSDKEdit(void *e, bool shouldReturnVal, int entityID, int variableID,
                                        int variableValue, bool *called)
{
    SceneEditor *editor = (SceneEditor *)e;

    editor->viewer->variableID                = variableID;
    editor->viewer->variableValue             = variableValue;
    editor->viewer->returnVariable            = shouldReturnVal;
    editor->compilerv3->scriptEng.checkResult = -1;
    editor->compilerv4->scriptEng.checkResult = -1;
    bool c                         = editor->callGameEvent(SceneViewer::EVENT_EDIT, entityID);
    editor->viewer->returnVariable = false;

    if (called) {
        *called = c;

        if (variableID == -1 && shouldReturnVal) {
            if (editor->viewer->gameType == ENGINE_v4)
                *called = c && editor->compilerv4->scriptEng.checkResult >= 0;
            else if (editor->viewer->gameType == ENGINE_v3)
                *called = c && editor->compilerv3->scriptEng.checkResult >= 0;
        }
    }

    if (editor->viewer->gameType == ENGINE_v4)
        return editor->compilerv4->scriptEng.checkResult;
    else if (editor->viewer->gameType == ENGINE_v3)
        return editor->compilerv3->scriptEng.checkResult;

    return 0;
}

#include "moc_sceneobjectproperties.cpp"
