#ifndef SCENE_V5_H
#define SCENE_V5_H

namespace RSDKv5
{

enum VariableTypes {
    UINT8   = 0,
    UINT16  = 1,
    UINT32  = 2,
    INT8    = 3,
    INT16   = 4,
    INT32   = 5,
    ENUM    = 6,
    BOOL    = 7,
    STRING  = 8,
    VECTOR2 = 9,
    FLOAT   = 10,
    COLOR   = 11
};

class Scene
{
public:
    class SceneEditorMetadata
    {
    public:
        byte unknown1           = 3; // usually 2,3 or 4
        QColor backgroundColor1 = QColor(0x20, 0x20, 0x20);
        QColor backgroundColor2 = QColor(0x30, 0x30, 0x30);
        byte unknown2           = 1;
        byte unknown3           = 1;
        byte unknown4           = 4;
        byte unknown5           = 0;
        byte unknown6           = 1;
        byte unknown7           = 4;
        byte unknown8           = 0;
        QString stampName       = "StampList.bin";
        byte unknown9           = 0;

        SceneEditorMetadata() {}
        SceneEditorMetadata(Reader &reader) { read(reader); }

        void read(Reader &reader);
        void write(Writer &writer);
    };

    class ScrollInfo
    {
    public:
        short parallaxFactor = 1 << 8;
        short scrollSpeed    = 0;
        byte deform          = 0;
        byte unknown         = 0;

        ScrollInfo() {}
        ScrollInfo(Reader &reader) { read(reader); }

        inline void read(Reader &reader)
        {
            parallaxFactor = reader.read<short>(); // << 0
            scrollSpeed    = reader.read<short>(); // << 8

            deform  = reader.read<byte>();
            unknown = reader.read<byte>();
        }

        inline void write(Writer &writer)
        {
            writer.write(parallaxFactor);
            writer.write(scrollSpeed);

            writer.write(deform);
            writer.write(unknown);
        }
    };

    struct ScrollInstance {
        int startLine = 0;
        int length    = 1;
    };

    struct ScrollIndexInfo {
        float scrollPos      = 0.0f; // not written, for scene viewer only
        float parallaxFactor = 1.0f;
        float scrollSpeed    = 0.0f;
        byte deform          = 0;
        byte unknown         = 0;

        QList<ScrollInstance> instances;

        bool operator==(const ScrollIndexInfo &other) const
        {
            if (instances.count() != other.instances.count())
                return false;

            bool instanceMatch = false;
            for (int i = 0; i < instances.count(); ++i) {
                instanceMatch = instanceMatch && instances[i].startLine == other.instances[i].startLine
                                && instances[i].length == other.instances[i].length;
            }

            return instanceMatch && scrollPos == other.scrollPos
                   && parallaxFactor == other.parallaxFactor && scrollSpeed == other.scrollSpeed
                   && deform == other.deform && unknown == other.unknown;
        }
    };

    class SceneLayer
    {
    public:
        QString name = "";

        byte type      = 0;
        byte drawGroup = 2;
        ushort width   = 16;
        ushort height  = 16;

        short parallaxFactor = 1 << 8;
        short scrollSpeed    = 0 << 8;

        QList<ScrollInfo> scrollingInfo;
        QByteArray lineScroll;

        QList<ScrollIndexInfo> scrollInfos;

        QVector<QVector<ushort>> layout;

        // EDITOR-ONLY START
        bool visible = true;

        // EDITOR-ONLY END

        SceneLayer() {}
        SceneLayer(Reader &reader) { read(reader); }

        void read(Reader &reader);
        void write(Writer &writer);

        void resize(ushort width, ushort height);

        void scrollIndicesFromInfo();
        void scrollInfoFromIndices();
    };

    struct Position {
    public:
        int x = 0 << 0x10;
        int y = 0 << 0x10;
        int z = 0 << 0x10;

        Position() {}

        Position(Reader &reader, bool vec3 = false) { read(reader, vec3); }

        inline void read(Reader &reader, bool vec3 = false)
        {
            x = reader.read<int>();
            y = reader.read<int>();

            if (vec3)
                z = reader.read<int>();
        }

        inline void write(Writer &writer, bool vec3 = false)
        {
            writer.write(x);
            writer.write(y);

            if (vec3)
                writer.write(z);
        }

        inline float getX() { return this->x / 65536.0f; }
        inline float getY() { return this->y / 65536.0f; }
        inline float getZ() { return this->z / 65536.0f; }

        inline void setX(float x) { this->x = x * (1 << 0x10); }
        inline void setY(float y) { this->y = y * (1 << 0x10); }
        inline void setZ(float y) { this->z = y * (1 << 0x10); }
    };

    class NameIdentifier
    {
    public:
        QByteArray hash;
        QString name = "";

        NameIdentifier(QString name)
        {
            hash = Utils::getMd5HashByteArray(name);
            name = name;
        }

        NameIdentifier(Reader &reader) { read(reader); }

        inline void read(Reader &reader) { hash = reader.readByteArray(0x10); }
        inline void write(Writer &writer) { writer.write(hash); }

        QString hashString() { return Utils::getMd5HashString(hash); }

        QString toString()
        {
            if (name != "")
                return name;
            return hashString();
        }
    };

    class VariableInfo
    {
    public:
        NameIdentifier name = NameIdentifier(QString("variable"));
        byte type           = 0;

        VariableInfo() {}
        VariableInfo(Reader &reader) { read(reader); }

        inline void read(Reader &reader)
        {
            name.read(reader);
            type = reader.read<byte>();
        }

        inline void write(Writer &writer)
        {
            name.write(writer);
            writer.write(type);
        }
    };

    class VariableValue
    {
    public:
        byte value_uint8     = 0;
        ushort value_uint16  = 0;
        uint value_uint32    = 0;
        sbyte value_int8     = 0;
        short value_int16    = 0;
        int value_int32      = 0;
        int value_enum       = 0;
        bool value_bool      = false;
        QString value_string = "";
        Vector2<int> value_vector2;
        Vector2<float> value_vector2f;
        float value_float = 0;
        QColor value_color;

        byte type;

        VariableValue() {}
        VariableValue(Reader &reader, int type)
        {
            this->type = type;
            read(reader);
        }

        void read(Reader &reader);
        void write(Writer &writer);
    };

    class SceneObject;

    class SceneEntity
    {
    public:
        ushort slotID = 0;
        Position position;
        SceneObject *parent = nullptr;

        QList<VariableValue> variables;
        SceneEntity() {}

        SceneEntity(Reader &reader, SceneObject *obj, QList<VariableInfo> &vars)
        {
            read(reader, obj, vars);
        }
        inline void read(Reader &reader, SceneObject *obj, QList<VariableInfo> &vars)
        {
            parent = obj;
            slotID = reader.read<ushort>();
            position.read(reader);

            variables.clear();
            for (VariableInfo &variable : vars) variables.append(VariableValue(reader, variable.type));
        }

        inline void write(Writer &writer)
        {
            writer.write(slotID);
            position.write(writer);

            for (VariableValue &variable : variables) variable.write(writer);
        }
    };

    class SceneObject
    {
    public:
        NameIdentifier name = NameIdentifier(QString("Object"));
        QList<VariableInfo> variables;
        QList<SceneEntity> entities;

        SceneObject() {}

        SceneObject(NameIdentifier name, QList<VariableInfo> vars)
        {
            name      = name;
            variables = vars;
        }

        SceneObject(Reader &reader) { read(reader); }
        inline void read(Reader &reader)
        {
            name.read(reader);

            byte varCount = reader.read<byte>();
            variables.clear();
            for (int i = 1; i < varCount; ++i) variables.append(VariableInfo(reader));

            ushort entityCount = reader.read<ushort>();
            entities.clear();
            for (int i = 0; i < entityCount; ++i) entities.append(SceneEntity(reader, this, variables));
        }

        inline void write(Writer &writer)
        {
            name.write(writer);

            writer.write((byte)(variables.count() + 1));
            for (VariableInfo &variable : variables) variable.write(writer);

            writer.write((ushort)entities.count());
            for (SceneEntity &entity : entities) entity.write(writer);
        }
    };

    Scene() {}
    Scene(QString filename) { read(filename); }
    Scene(Reader &reader) { read(reader); }

    inline void read(QString filename)
    {
        Reader reader(filename);
        read(reader);
    }
    void read(Reader &reader)
    {
        filepath = reader.filePath;

        if (!reader.matchesSignature(signature, 4))
            return;

        editorMetadata.read(reader);

        byte layerCount = reader.read<byte>();
        layers.clear();
        for (int i = 0; i < layerCount; ++i) layers.append(SceneLayer(reader));

        byte objectCount = reader.read<byte>();
        objects.clear();
        for (int i = 0; i < objectCount; ++i) objects.append(SceneObject(reader));
    }

    inline void write(QString filename)
    {
        if (filename == "")
            filename = filepath;
        if (filename == "")
            return;
        Writer writer(filename);
        write(writer);
    }
    void write(Writer &writer)
    {
        filepath = writer.filePath;
        writer.write(signature, 4);

        editorMetadata.write(writer);

        writer.write((byte)layers.count());
        for (SceneLayer &layer : layers) layer.write(writer);

        writer.write((byte)objects.count());
        for (SceneObject &obj : objects) obj.write(writer);

        writer.flush();
    }

    byte signature[4] = { 'S', 'C', 'N', 0 };

    SceneEditorMetadata editorMetadata;
    QList<SceneLayer> layers;
    QList<SceneObject> objects;

    QString filepath = "";
};

} // namespace RSDKv5

#endif // SCENE_V5_H
