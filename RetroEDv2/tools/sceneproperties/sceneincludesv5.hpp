#ifndef SCENEINCLUDES_V5_H
#define SCENEINCLUDES_V5_H

namespace SceneHelpers
{

class TileLayer
{
public:
    TileLayer() {}

    struct ScrollInstance {
        int startLine = 0;
        int length    = 1;

        byte layerID = 0;
    };

    struct ScrollIndexInfo {
        float scrollPos      = 0.0f; // not written, for scene viewer only
        float parallaxFactor = 1.0f;
        float scrollSpeed    = 0.0f;
        bool deform          = false;
        byte unknown         = 0;

        QList<ScrollInstance> instances;

        bool operator==(const ScrollIndexInfo &other) const
        {
            if (instances.count() != other.instances.count())
                return false;

            bool instanceMatch = false;
            for (int i = 0; i < instances.count(); ++i) {
                instanceMatch = instanceMatch && instances[i].startLine == other.instances[i].startLine
                                && instances[i].length == other.instances[i].length
                                && instances[i].layerID == other.instances[i].layerID;
            }

            return instanceMatch && scrollPos == other.scrollPos
                   && parallaxFactor == other.parallaxFactor && scrollSpeed == other.scrollSpeed
                   && deform == other.deform && unknown == other.unknown;
        }
    };

    enum LayerTypes {
        LAYER_NONE,
        LAYER_HSCROLL,
        LAYER_VSCROLL,
        LAYER_ROTOZOOM,
        LAYER_BASIC,
        LAYER_SKY3D,
        LAYER_FLOOR3D
    };

    QString name = "TileLayer";

    byte type            = LAYER_NONE;
    byte drawOrder       = 2;
    ushort width         = 0;
    ushort height        = 0;
    bool visible         = true;
    float parallaxFactor = 1.0f;
    float scrollSpeed    = 0.0f;

    QList<ScrollIndexInfo> scrollInfos;

    QList<QList<ushort>> layout;
};

} // namespace SceneHelpers

enum ObjectAliases {
    VAR_ALIAS_PROPVAL,
    VAR_ALIAS_VAL0,
    VAR_ALIAS_VAL1,
    VAR_ALIAS_VAL2,
    VAR_ALIAS_VAL3,
    VAR_ALIAS_COUNT
};

class VariableValue
{
public:
    QString name = "Variable";
    int value    = 0;

    VariableValue() {}
};

class VariableInfo
{
public:
    QString name = "Variable";
    byte type    = 0;
    int offset   = -1;
    QByteArray hash;
    QList<VariableValue> values;

    VariableInfo()
    {
        hash = Utils::getMd5HashByteArray(name);
        values.clear();
    }
    VariableInfo(QString n, byte t, int o) : name(n), type(t), offset(o)
    {
        hash = Utils::getMd5HashByteArray(name);
        values.clear();
    }
};

struct SceneEntity {
    ushort slotID      = 0;
    ushort prevSlot    = 0;
    byte type          = 0;
    byte propertyValue = 0;
    Vector2<float> pos = Vector2<float>(0, 0);
    void *gameEntity   = nullptr; // for v5
    int gameEntitySlot = -1;      // for v4 and below
    QList<RSDKv5::Scene::VariableValue> variables;
    Rect<int> box = Rect<int>(-0x10, -0x10, 0x10, 0x10); // selection box

    // for <= v4
    FormatHelpers::Scene::Object::VariableInfo sceneVariables[0x0F];

    SceneEntity() {}

    bool operator==(const SceneEntity &other) const
    {
        return slotID == other.slotID && type == other.type && propertyValue == other.propertyValue
               && pos.x == other.pos.x && pos.y == other.pos.y /*&& variables == other.variables*/;
    }
};

struct SceneObject {
    QString name = "";
    QList<VariableInfo> variables;

    // for <= v4
    QString variablesAliases[VAR_ALIAS_COUNT] = { "propertyValue", "value0", "value1", "value2",
                                                  "value3" };

    // EDITOR ONLY
    bool visible = true;

    SceneObject() {}

    bool operator==(const SceneObject &other) const
    {
        return name == other.name /*&& variables == other.variables*/;
    }
};

#endif // SCENEINCLUDES_V5_H
