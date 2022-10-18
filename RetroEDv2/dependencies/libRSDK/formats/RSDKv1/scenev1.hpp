#pragma once

namespace RSDKv1
{

class Scene
{
public:
    class Entity
    {
    public:
        Entity() {}
        Entity(Reader &reader, int id = 0) { read(reader, id); }

        inline void read(Reader &reader, int id = 0)
        {
            // Object type, 1 byte, unsigned
            type = reader.read<byte>();
            // Object subtype, 1 byte, unsigned
            propertyValue = reader.read<byte>();

            // X Position, 2 bytes, big-endian, signed
            posX = (short)(reader.read<char>() << 8);
            posX |= (short)reader.read<byte>();

            // Y Position, 2 bytes, big-endian, signed
            posY = (short)(reader.read<char>() << 8);
            posY |= (short)reader.read<byte>();

            slotID = id;
        }

        inline void write(Writer &writer)
        {
            writer.write(type);
            writer.write(propertyValue);

            writer.write((byte)(posX >> 8));
            writer.write((byte)(posX & 0xFF));

            writer.write((byte)(posY >> 8));
            writer.write((byte)(posY & 0xFF));
        }

        byte type          = 0;
        byte propertyValue = 0;
        short posX         = 0;
        short posY         = 0;

        short slotID = 0;
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
            filename = filePath;
        if (filename == "")
            return;
        Writer writer(filename);
        write(writer);
    }
    void write(Writer &writer);

    QString title = "ACT";

    QList<QList<byte>> layout;

    byte musicID      = 0;
    byte backgroundID = 0;

    short playerXPos = 0;
    short playerYPos = 0;

    QList<Entity> entities;

    byte width  = 0;
    byte height = 0;

    QString filePath = "";
};

} // namespace RSDKv1


