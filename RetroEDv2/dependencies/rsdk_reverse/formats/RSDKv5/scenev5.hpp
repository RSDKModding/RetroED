#ifndef SCENE_V5_H
#define SCENE_V5_H

namespace RSDKv5
{

enum AttributeTypes {
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
    UNKNOWN = 10, // A wild guess, but nonetheless likely
    COLOR   = 11
};

class Scene
{
public:
    class SceneEditorMetadata
    {
    public:
        byte unknown1           = 3; // usually 2,3 or 4
        QColor backgroundColor1 = QColor(0xFF, 0, 0xFF);
        QColor backgroundColor2 = QColor(0, 0xFF, 0);
        QByteArray unknown2; // Const: 01010400010400
        QString stampName = "Stamps.bin";
        byte unknown3;

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
        byte m_drawOrder     = 0;

        ScrollInfo() {}
        ScrollInfo(Reader &reader) { read(reader); }

        inline void read(Reader &reader)
        {
            parallaxFactor = reader.read<short>(); // << 0
            scrollSpeed    = reader.read<short>(); // << 8

            deform      = reader.read<byte>();
            m_drawOrder = reader.read<byte>();
        }

        inline void write(Writer &writer)
        {
            writer.write(parallaxFactor);
            writer.write(scrollSpeed);

            writer.write(deform);
            writer.write(m_drawOrder);
        }
    };

    class ScrollIndexInfo
    {
    public:
        int startLine = 0;
        int length    = 1;

        float m_scrollPos    = 0.0f; // not written, for scene viewer only
        float parallaxFactor = 1.0f;
        float scrollSpeed    = 0.0f;
        byte deform          = 0;

        bool operator==(const ScrollIndexInfo &other) const
        {
            return startLine == other.startLine && length == other.length
                   && m_scrollPos == other.m_scrollPos && parallaxFactor == other.parallaxFactor
                   && scrollSpeed == other.scrollSpeed && deform == other.deform;
        }
    };

    class SceneLayer
    {
    public:
        byte unknown = 0;

        QString m_name = "";

        byte type      = 0;
        byte drawOrder = 2;
        ushort width   = 16;
        ushort height  = 16;

        short parallaxFactor = 1 << 8;
        short scrollSpeed    = 0 << 8;

        QList<ScrollInfo> scrollingInfo;
        QByteArray lineIndexes;

        QList<ScrollIndexInfo> scrollInfos;

        QVector<QVector<ushort>> layout;

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
        QString m_name = "";

        NameIdentifier(QString name)
        {
            hash   = Utils::getMd5HashByteArray(name);
            m_name = name;
        }

        NameIdentifier(Reader &reader) { read(reader); }

        inline void read(Reader &reader) { hash = reader.readByteArray(0x10); }
        inline void write(Writer &writer) { writer.write(hash); }

        QString hashString() { return Utils::getMd5HashString(hash); }

        QString toString()
        {
            if (m_name != "")
                return m_name;
            return hashString();
        }
    };

    class AttributeInfo
    {
    public:
        NameIdentifier m_name = NameIdentifier(QString("attribute"));
        AttributeTypes type   = (AttributeTypes)0;

        AttributeInfo() {}
        AttributeInfo(Reader &reader) { read(reader); }

        inline void read(Reader &reader)
        {
            m_name.read(reader);
            type = (AttributeTypes)reader.read<byte>();
        }

        inline void write(Writer &writer)
        {
            m_name.write(writer);
            writer.write((byte)type);
        }
    };

    class AttributeValue
    {
    public:
        byte value_uint8     = 0;
        ushort value_uint16  = 0;
        uint value_uint32    = 0;
        char value_int8      = 0;
        short value_int16    = 0;
        int value_int32      = 0;
        int value_enum       = 0;
        bool value_bool      = false;
        QString value_string = "";
        Position value_vector2;
        Position value_vector3;
        QColor value_color;

        AttributeTypes m_type;

        AttributeValue() {}
        AttributeValue(Reader &reader, AttributeTypes type)
        {
            m_type = type;
            read(reader);
        }

        void read(Reader &reader);
        void write(Writer &writer);
    };

    class SceneObject;

    class SceneEntity
    {
    public:
        ushort m_slotID = 0;
        Position position;
        SceneObject *m_parent = nullptr;

        QList<AttributeValue> variables;
        SceneEntity() {}

        SceneEntity(Reader &reader, SceneObject *obj, QList<AttributeInfo> &attributes)
        {
            read(reader, obj, attributes);
        }
        inline void read(Reader &reader, SceneObject *obj, QList<AttributeInfo> &attributes)
        {
            m_parent = obj;
            m_slotID = reader.read<ushort>();
            position.read(reader);

            variables.clear();
            for (AttributeInfo &attribute : attributes)
                variables.append(AttributeValue(reader, attribute.type));
        }

        inline void write(Writer &writer)
        {
            writer.write(m_slotID);
            position.write(writer);

            for (AttributeValue &attribute : variables) attribute.write(writer);
        }
    };

    class SceneObject
    {
    public:
        NameIdentifier m_name = NameIdentifier(QString("Object"));
        QList<AttributeInfo> variables;
        QList<SceneEntity> entities;

        SceneObject() {}

        SceneObject(NameIdentifier name, QList<AttributeInfo> attributes)
        {
            m_name    = name;
            variables = attributes;
        }

        SceneObject(Reader &reader) { read(reader); }
        inline void read(Reader &reader)
        {
            m_name.read(reader);

            byte attribCount = reader.read<byte>();
            variables.clear();
            for (int i = 1; i < attribCount; ++i) variables.append(AttributeInfo(reader));

            ushort entityCount = reader.read<ushort>();
            entities.clear();
            for (int i = 0; i < entityCount; ++i) entities.append(SceneEntity(reader, this, variables));
        }

        inline void write(Writer &writer)
        {
            m_name.write(writer);

            writer.write((byte)(variables.count() + 1));
            for (AttributeInfo &attribute : variables) attribute.write(writer);

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
        filepath = reader.filepath;

        if (!reader.matchesSignature(m_signature, 4))
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
        writer.write(m_signature, 4);

        editorMetadata.write(writer);

        writer.write((byte)layers.count());
        for (SceneLayer &layer : layers) layer.write(writer);

        writer.write((byte)objects.count());
        for (SceneObject &obj : objects) obj.write(writer);

        writer.flush();
    }

    byte m_signature[4] = { 'S', 'C', 'N', 0 };

    SceneEditorMetadata editorMetadata;
    QList<SceneLayer> layers;
    QList<SceneObject> objects;

    QString filepath = "";
};

} // namespace RSDKv5

#endif // SCENE_V5_H
