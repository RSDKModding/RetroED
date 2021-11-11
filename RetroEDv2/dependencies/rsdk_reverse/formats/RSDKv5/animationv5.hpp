#ifndef ANIMATION_V5_H
#define ANIMATION_V5_H

namespace RSDKv5
{

class Animation
{
public:
    struct Hitbox {
    public:
        Hitbox() {}
        Hitbox(Reader &reader) { read(reader); }

        inline void read(Reader &reader)
        {
            left   = reader.read<short>();
            top    = reader.read<short>();
            right  = reader.read<short>();
            bottom = reader.read<short>();
        }

        inline void write(Writer &writer)
        {
            writer.write(left);
            writer.write(top);
            writer.write(right);
            writer.write(bottom);
        }

        short left   = 0;
        short top    = 0;
        short right  = 0;
        short bottom = 0;
    };

    class Frame
    {
    public:
        Frame() {}
        Frame(Reader &reader, Animation *parent) { read(reader, parent); }

        inline void read(Reader &reader, Animation *parent)
        {
            sheet  = reader.read<byte>();
            delay  = reader.read<short>();
            id     = reader.read<short>();
            sprX   = reader.read<ushort>();
            sprY   = reader.read<ushort>();
            width  = reader.read<ushort>();
            height = reader.read<ushort>();
            pivotX = reader.read<short>();
            pivotY = reader.read<short>();

            hitboxes.clear();
            for (int i = 0; i < parent->hitboxTypes.count(); ++i) hitboxes.append(Hitbox(reader));
        }

        inline void write(Writer &writer, Animation *parent)
        {
            writer.write(sheet);
            writer.write(delay);
            writer.write(id);
            writer.write(sprX);
            writer.write(sprY);
            writer.write(width);
            writer.write(height);
            writer.write(pivotX);
            writer.write(pivotY);

            for (int i = 0; i < parent->hitboxTypes.count(); ++i) hitboxes[i].write(writer);
        }

        byte sheet    = 0;
        ushort delay  = 0;
        ushort id     = 0;
        ushort sprX   = 0;
        ushort sprY   = 0;
        ushort width  = 0;
        ushort height = 0;
        short pivotX  = 0;
        short pivotY  = 0;

        QList<Hitbox> hitboxes;
    };

    class AnimationEntry
    {
    public:
        AnimationEntry() {}
        AnimationEntry(Reader &reader, Animation *parent) { read(reader, parent); }

        inline void read(Reader &reader, Animation *parent)
        {
            name              = reader.readString();
            ushort frameCount = reader.read<ushort>();
            speedMultiplyer   = reader.read<short>();
            loopIndex         = reader.read<byte>();
            rotationFlags     = reader.read<byte>();

            frames.clear();
            for (int f = 0; f < frameCount; ++f) frames.append(Frame(reader, parent));
        }

        inline void write(Writer &writer, Animation *parent)
        {
            writer.write(name + '\0');
            writer.write((ushort)frames.count());
            writer.write(speedMultiplyer);
            writer.write(loopIndex);
            writer.write(rotationFlags);
            for (int f = 0; f < frames.count(); ++f) frames[f].write(writer, parent);
        }

        QString name = "Animation";
        QList<Frame> frames;
        byte loopIndex        = 0;
        short speedMultiplyer = 0;
        byte rotationFlags    = 0;
    };

    Animation() {}
    Animation(QString filename) { read(filename); }
    Animation(Reader &reader) { read(reader); }

    inline void read(QString filename)
    {
        Reader reader(filename);
        read(reader);
    }
    void read(Reader &reader);

    inline void write(QString filename)
    {
        if (filename == "")
            filename = filename;
        if (filename == "")
            return;
        Writer writer(filename);
        write(writer);
    }
    void write(Writer &writer);

    byte signature[4] = { 'S', 'P', 'R', 0 };

    QList<QString> sheets;
    QList<QString> hitboxTypes;
    QList<AnimationEntry> animations;

    QString filePath = "";
};

} // namespace RSDKv5
#endif // ANIMATION_V5_H
