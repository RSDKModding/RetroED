#pragma once

namespace RSDKv3
{

class Animation
{
public:
    class Hitbox
    {
    public:
        struct HitboxInfo {
        public:
            HitboxInfo() {}
            HitboxInfo(Reader &reader) { read(reader); }

            inline void read(Reader &reader)
            {
                left   = reader.read<sbyte>();
                top    = reader.read<sbyte>();
                right  = reader.read<sbyte>();
                bottom = reader.read<sbyte>();
            }

            inline void write(Writer &writer)
            {
                writer.write(left);
                writer.write(top);
                writer.write(right);
                writer.write(bottom);
            }

            sbyte left   = 0;
            sbyte top    = 0;
            sbyte right  = 0;
            sbyte bottom = 0;
        };

        Hitbox() {}
        Hitbox(Reader &reader) { read(reader); }

        inline void read(Reader &reader)
        {
            for (int h = 0; h < 8; ++h) hitboxes[h].read(reader);
        }

        inline void write(Writer &writer)
        {
            for (int h = 0; h < 8; ++h) hitboxes[h].write(writer);
        }

        HitboxInfo hitboxes[8];
    };

    class Frame
    {
    public:
        Frame() {}
        Frame(Reader &reader) { read(reader); }

        inline void read(Reader &reader)
        {
            sheet        = reader.read<byte>();
            collisionBox = reader.read<byte>();
            sprX         = reader.read<byte>();
            sprY         = reader.read<byte>();
            width        = reader.read<byte>();
            height       = reader.read<byte>();
            pivotX       = reader.read<sbyte>();
            pivotY       = reader.read<sbyte>();
        }

        inline void write(Writer &writer)
        {
            writer.write(sheet);
            writer.write(collisionBox);
            writer.write(sprX);
            writer.write(sprY);
            writer.write(width);
            writer.write(height);
            writer.write(pivotX);
            writer.write(pivotY);
        }

        byte sheet        = 0;
        byte collisionBox = 0;
        byte sprX         = 0;
        byte sprY         = 0;
        byte width        = 0;
        byte height       = 0;
        sbyte pivotX      = 0;
        sbyte pivotY      = 0;
    };

    class AnimationEntry
    {
    public:
        AnimationEntry() {}
        AnimationEntry(Reader &reader) { read(reader); }

        inline void read(Reader &reader)
        {
            name            = reader.readString();
            byte frameCount = reader.read<byte>();
            speed           = reader.read<byte>();
            loopIndex       = reader.read<byte>();
            rotationStyle   = reader.read<byte>();

            frames.clear();
            for (int f = 0; f < frameCount; ++f) frames.append(Frame(reader));
        }

        inline void write(Writer &writer)
        {
            writer.write(name);
            writer.write((byte)frames.count());
            writer.write(speed);
            writer.write(loopIndex);
            writer.write(rotationStyle);
            for (int f = 0; f < frames.count(); ++f) frames[f].write(writer);
        }

        QString name = "New Animation";
        QList<Frame> frames;
        byte loopIndex     = 0;
        byte speed         = 0;
        byte rotationStyle = 0;
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

    QList<QString> sheets;
    QList<AnimationEntry> animations;
    QList<Hitbox> hitboxes;

    QString filePath = "";
};

} // namespace RSDKv3


