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
        byte m_unknownByte        = 3; // 2/3/4
        QColor m_backgroundColor1 = QColor(0xFF, 0, 0xFF);
        QColor m_backgroundColor2 = QColor(0, 0xFF, 0);
        QByteArray m_unknownBytes; // Const: 01010400010400
        QString m_stampName = "Stamps.bin";
        byte m_unknownByte2;

        SceneEditorMetadata() {}
        SceneEditorMetadata(Reader &reader) { read(reader); }

        void read(Reader &reader);
        void write(Writer &writer);
    };

    class ScrollInfo
    {
    public:
        short m_relativeSpeed = 1 << 8;
        short m_constantspeed = 0;
        byte m_behaviour      = 0;
        byte m_drawOrder      = 0;

        ScrollInfo() {}
        ScrollInfo(Reader &reader) { read(reader); }

        inline void read(Reader &reader)
        {
            m_relativeSpeed = reader.read<short>();
            m_constantspeed = reader.read<short>();

            m_behaviour = reader.read<byte>();
            m_drawOrder = reader.read<byte>();
        }

        inline void write(Writer &writer)
        {
            writer.write(m_relativeSpeed);
            writer.write(m_constantspeed);

            writer.write(m_behaviour);
            writer.write(m_drawOrder);
        }
    };

    class SceneLayer
    {
    public:
        byte m_unknownByte = 0;

        QString m_name = "";

        byte m_behaviour    = 0;
        byte m_drawingOrder = 2;
        ushort m_width      = 16;
        ushort m_height     = 16;

        short m_relativeSpeed = 1 << 8;
        short m_constantSpeed = 0 << 8;

        QList<ScrollInfo> m_scrollingInfo;
        QByteArray m_scrollIndexes;

        QVector<QVector<ushort>> m_tiles;

        SceneLayer(Reader &reader) { read(reader); }

        void read(Reader &reader);
        void write(Writer &writer);

        void resize(ushort width, ushort height);
    };

    struct Position {
    public:
        struct Value {
            Value(short high = 0, ushort low = 0)
            {
                m_low  = low;
                m_high = high;
            }

            inline void read(Reader &reader)
            {
                m_low  = reader.read<ushort>();
                m_high = reader.read<short>();
            }

            inline void write(Writer &writer)
            {
                writer.write(m_low);
                writer.write(m_high);
            }

            inline int toInt() { return m_high + (m_low << 16); }
            inline float toFloat() { return m_high + (m_low / (float)(1 << 16)); }

            short m_high = 0;
            ushort m_low = 0;
        };

        Value x = Value();
        Value y = Value();
        Value z = Value();

        Position() {}

        Position(Reader &reader, bool vec3 = false) { read(reader, vec3); }

        inline void read(Reader &reader, bool vec3 = false)
        {
            x.read(reader);
            y.read(reader);

            if (vec3)
                z.read(reader);
        }

        inline void write(Writer &writer, bool vec3 = false)
        {
            x.write(writer);
            y.write(writer);

            if (vec3)
                z.write(writer);
        }
    };

    class NameIdentifier
    {
    public:
        QByteArray m_hash;
        QString m_name = "";

        NameIdentifier(QString name)
        {
            m_hash = Utils::getMd5HashByteArray(name);
            m_name = name;
        }

        NameIdentifier(Reader &reader) { read(reader); }

        inline void read(Reader &reader) { m_hash = reader.readByteArray(0x10); }
        inline void write(Writer &writer) { writer.write(m_hash); }

        QString hashString() { return Utils::getMd5HashString(m_hash); }

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
        AttributeTypes m_type = (AttributeTypes)0;

        AttributeInfo() {}
        AttributeInfo(Reader &reader) { read(reader); }

        inline void read(Reader &reader)
        {
            m_name.read(reader);
            m_type = (AttributeTypes)reader.read<byte>();
        }

        inline void write(Writer &writer)
        {
            m_name.write(writer);
            writer.write((byte)m_type);
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
        Position m_position;
        SceneObject *m_parent = nullptr;

        QList<AttributeValue> m_attributes;
        SceneEntity() {}

        SceneEntity(Reader &reader, SceneObject *obj, QList<AttributeInfo> &attributes)
        {
            read(reader, obj, attributes);
        }
        inline void read(Reader &reader, SceneObject *obj, QList<AttributeInfo> &attributes)
        {
            m_parent = obj;
            m_slotID = reader.read<ushort>();
            m_position.read(reader);

            m_attributes.clear();
            for (AttributeInfo &attribute : attributes)
                m_attributes.append(AttributeValue(reader, attribute.m_type));
        }

        inline void write(Writer &writer)
        {
            writer.write(m_slotID);
            m_position.write(writer);

            for (AttributeValue &attribute : m_attributes) attribute.write(writer);
        }
    };

    class SceneObject
    {
    public:
        NameIdentifier m_name = NameIdentifier(QString("Object"));
        QList<AttributeInfo> m_attributes;
        QList<SceneEntity> m_entities;

        SceneObject() {}

        SceneObject(NameIdentifier name, QList<AttributeInfo> attributes)
        {
            m_name       = name;
            m_attributes = attributes;
        }

        SceneObject(Reader &reader) { read(reader); }
        inline void read(Reader &reader)
        {
            m_name.read(reader);

            byte attribCount = reader.read<byte>();
            m_attributes.clear();
            for (int i = 1; i < attribCount; ++i) m_attributes.append(AttributeInfo(reader));

            ushort entityCount = reader.read<ushort>();
            m_entities.clear();
            for (int i = 0; i < entityCount; ++i)
                m_entities.append(SceneEntity(reader, this, m_attributes));
        }

        inline void write(Writer &writer)
        {
            m_name.write(writer);

            writer.write((byte)(m_attributes.count() + 1));
            for (AttributeInfo &attribute : m_attributes) attribute.write(writer);

            writer.write((ushort)m_entities.count());
            for (SceneEntity &entity : m_entities) entity.write(writer);
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
        m_filename = reader.m_filepath;

        if (!reader.matchesSignature(m_signature, 4))
            return;

        m_editorMetadata.read(reader);

        byte layerCount = reader.read<byte>();
        m_layers.clear();
        for (int i = 0; i < layerCount; ++i) m_layers.append(SceneLayer(reader));

        byte objectCount = reader.read<byte>();
        m_objects.clear();
        for (int i = 0; i < objectCount; ++i) m_objects.append(SceneObject(reader));
    }

    inline void write(QString filename)
    {
        if (filename == "")
            filename = m_filename;
        if (filename == "")
            return;
        Writer writer(filename);
        write(writer);
    }
    void write(Writer &writer)
    {
        m_filename = writer.m_filename;
        writer.write(m_signature, 4);

        m_editorMetadata.write(writer);

        writer.write((byte)m_layers.count());
        for (SceneLayer &layer : m_layers) layer.write(writer);

        writer.write((byte)m_objects.count());
        for (SceneObject &obj : m_objects) obj.write(writer);

        writer.flush();
    }

    byte m_signature[4] = { 'S', 'C', 'N', 0 };

    SceneEditorMetadata m_editorMetadata;
    QList<SceneLayer> m_layers;
    QList<SceneObject> m_objects;

    QString m_filename = "";
};

} // namespace RSDKv5

#endif // SCENE_V5_H
