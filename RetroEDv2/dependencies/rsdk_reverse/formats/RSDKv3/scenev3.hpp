#ifndef SCENE_V3_H
#define SCENE_V3_H

namespace RSDKv3
{

class Scene
{
public:
    class Object
    {
    public:
        Object() {}
        Object(Reader &reader, int id = 0) { read(reader, id); }

        inline void read(Reader &reader, int id = 0)
        {
            // Object type, 1 byte, unsigned
            m_type = reader.read<byte>();
            // Object subtype, 1 byte, unsigned
            m_propertyValue = reader.read<byte>();

            // X Position, 2 bytes, big-endian, signed
            m_xPos = (short)(reader.read<char>() << 8);
            m_xPos |= (short)reader.read<byte>();

            // Y Position, 2 bytes, big-endian, signed
            m_yPos = (short)(reader.read<char>() << 8);
            m_yPos |= (short)reader.read<byte>();

            m_id = id;
        }

        inline void write(Writer &writer)
        {
            writer.write(m_type);
            writer.write(m_propertyValue);

            writer.write((byte)(m_xPos >> 8));
            writer.write((byte)(m_xPos & 0xFF));

            writer.write((byte)(m_yPos >> 8));
            writer.write((byte)(m_yPos & 0xFF));
        }

        byte m_type    = 0;
        byte m_propertyValue = 0;
        short m_xPos   = 0;
        short m_yPos   = 0;

        short m_id = 0;
    };

    Scene() {}
    Scene(QString filename) { read(filename); }
    Scene(Reader &reader) { read(reader); }

    inline void read(QString filename)
    {
        Reader reader(filename);
        read(reader);
    }
    void read(Reader &reader);

    inline void write(QString filename)
    {
        if (filename == "")
            filename = m_filename;
        if (filename == "")
            return;
        Writer writer(filename);
        write(writer);
    }
    void write(Writer &writer);

    QString m_title = "ACT";

    QList<QList<ushort>> m_layout;

    byte m_activeLayer[4];
    byte m_midpoint = 3;

    QList<Object> m_objects;
    QList<QString> m_objectTypeNames;

    byte m_width  = 0;
    byte m_height = 0;

    QString m_filename = "";
};

} // namespace RSDKv3

#endif // SCENE_V3_H
