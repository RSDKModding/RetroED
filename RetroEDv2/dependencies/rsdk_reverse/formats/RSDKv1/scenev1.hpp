#ifndef SCENE_VRS_H
#define SCENE_VRS_H

namespace RSDKv1
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
            type = reader.read<byte>();
            // Object subtype, 1 byte, unsigned
            propertyValue = reader.read<byte>();

            // X Position, 2 bytes, big-endian, signed
            XPos = (short)(reader.read<char>() << 8);
            XPos |= (short)reader.read<byte>();

            // Y Position, 2 bytes, big-endian, signed
            YPos = (short)(reader.read<char>() << 8);
            YPos |= (short)reader.read<byte>();

            slotID = id;
        }

        inline void write(Writer &writer)
        {
            writer.write(type);
            writer.write(propertyValue);

            writer.write((byte)(XPos >> 8));
            writer.write((byte)(XPos & 0xFF));

            writer.write((byte)(YPos >> 8));
            writer.write((byte)(YPos & 0xFF));
        }

        byte type          = 0;
        byte propertyValue = 0;
        short XPos         = 0;
        short YPos         = 0;

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
            filename = filepath;
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

    QList<Object> objects;

    byte width  = 0;
    byte height = 0;

    QString filepath = "";
};

} // namespace RSDKv1

#endif // SCENE_VRS_H
