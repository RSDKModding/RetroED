#pragma once

namespace RSDKv1
{

class Animation
{
public:
    class Frame
    {
    public:
        struct Hitbox {
        public:
            Hitbox() {}
            Hitbox(Reader &reader) { read(reader); }

            inline void read(Reader &reader)
            {
                left   = readSByte(reader);
                top    = readSByte(reader);
                right  = readSByte(reader);
                bottom = readSByte(reader);
            }

            inline void write(Writer &writer)
            {
                writeSByte(writer, left);
                writeSByte(writer, top);
                writeSByte(writer, right);
                writeSByte(writer, bottom);
            }

            sbyte left   = 0;
            sbyte top    = 0;
            sbyte right  = 0;
            sbyte bottom = 0;

        private:
            inline sbyte readSByte(Reader &reader)
            {
                int val = reader.read<byte>();
                if (val > 127)
                    return (sbyte)(0x80 - val);
                else
                    return (sbyte)val;
            }
            inline void writeSByte(Writer &writer, int val)
            {
                if (val < 0)
                    writer.write((byte)(0x80 - val));
                else
                    writer.write((byte)val);
            }
        };

        Frame() {}
        Frame(Reader &reader) { read(reader); }
        inline void read(Reader &reader)
        {
            sprX   = reader.read<byte>();
            sprY   = reader.read<byte>();
            width  = reader.read<byte>();
            height = reader.read<byte>();
            sheet  = reader.read<byte>();
            hitbox.read(reader);
            pivotX = -reader.read<byte>();
            pivotY = -reader.read<byte>();
        }

        inline void write(Writer &writer)
        {
            writer.write(sprX);
            writer.write(sprY);
            writer.write(width);
            writer.write(height);
            writer.write(sheet);
            hitbox.write(writer);
            writer.write((byte)-pivotX);
            writer.write((byte)-pivotY);
        }

        byte sheet    = 0;
        Hitbox hitbox = Hitbox();
        byte sprX     = 0;
        byte sprY     = 0;
        byte width    = 0;
        byte height   = 0;
        sbyte pivotX  = 0;
        sbyte pivotY  = 0;
    };

    class AnimationEntry
    {
    public:
        AnimationEntry() {}
        AnimationEntry(Reader &reader) { read(reader); }

        inline void read(Reader &reader)
        {
            byte frameCount = reader.read<byte>();
            speed           = reader.read<byte>();
            loopIndex       = reader.read<byte>();

            frames.clear();
            for (int f = 0; f < frameCount; ++f) frames.append(Frame(reader));
        }

        inline void write(Writer &writer)
        {
            writer.write((byte)frames.count());
            writer.write(speed / 4);
            writer.write(loopIndex);

            for (int f = 0; f < frames.count(); ++f) frames[f].write(writer);
        }

        QString name = "Retro Sonic Animation #";
        QList<Frame> frames;
        byte loopIndex = 0;
        byte speed     = 0;
    };

    Animation() {}
    Animation(QString filename, bool dcVer = false) { read(filename, dcVer); }
    Animation(Reader &reader, bool dcVer = false) { read(reader, dcVer); }

    inline void read(QString filename, bool dcVer = false)
    {
        Reader reader(filename);
        read(reader, dcVer);
    }
    void read(Reader &reader, bool dcVer = false);

    inline void write(QString filename, bool dcVer = false)
    {
        if (filename == "")
            filename = filename;
        if (filename == "")
            return;
        Writer writer(filename);
        write(writer, dcVer);
    }
    void write(Writer &writer, bool dcVer = false);

    byte unknown    = 0;
    byte playerType = 0;

    QList<QString> sheets;
    QList<AnimationEntry> animations;

    QString filePath = "";
};

} // namespace RSDKv1


