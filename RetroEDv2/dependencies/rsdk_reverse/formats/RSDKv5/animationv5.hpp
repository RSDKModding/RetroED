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
            m_left   = reader.read<short>();
            m_top    = reader.read<short>();
            m_right  = reader.read<short>();
            m_bottom = reader.read<short>();
        }

        inline void write(Writer &writer)
        {
            writer.write(m_left);
            writer.write(m_top);
            writer.write(m_right);
            writer.write(m_bottom);
        }

        short m_left   = 0;
        short m_top    = 0;
        short m_right  = 0;
        short m_bottom = 0;
    };

    class Frame
    {
    public:
        Frame() {}
        Frame(Reader &reader, Animation *parent) { read(reader, parent); }

        inline void read(Reader &reader, Animation *parent)
        {
            m_sheet  = reader.read<byte>();
            m_delay  = reader.read<short>();
            m_id     = reader.read<short>();
            m_sprX   = reader.read<ushort>();
            m_sprY   = reader.read<ushort>();
            m_width  = reader.read<ushort>();
            m_height = reader.read<ushort>();
            m_pivotX = reader.read<short>();
            m_pivotY = reader.read<short>();

            m_hitboxes.clear();
            for (int i = 0; i < parent->m_hitboxTypes.count(); ++i) m_hitboxes.append(Hitbox(reader));
        }

        inline void write(Writer &writer, Animation *parent)
        {
            writer.write(m_sheet);
            writer.write(m_delay);
            writer.write(m_id);
            writer.write(m_sprX);
            writer.write(m_sprY);
            writer.write(m_width);
            writer.write(m_height);
            writer.write(m_pivotX);
            writer.write(m_pivotY);

            for (int i = 0; i < parent->m_hitboxTypes.count(); ++i) m_hitboxes[i].write(writer);
        }

        byte m_sheet    = 0;
        short m_delay   = 0;
        short m_id      = 0;
        ushort m_sprX   = 0;
        ushort m_sprY   = 0;
        ushort m_width  = 0;
        ushort m_height = 0;
        short m_pivotX  = 0;
        short m_pivotY  = 0;

        QList<Hitbox> m_hitboxes;
    };

    class AnimationEntry
    {
    public:
        AnimationEntry() {}
        AnimationEntry(Reader &reader, Animation *parent) { read(reader, parent); }

        inline void read(Reader &reader, Animation *parent)
        {
            m_name            = reader.readString();
            ushort frameCount = reader.read<ushort>();
            m_speedMultiplyer = reader.read<short>();
            m_loopIndex       = reader.read<byte>();
            m_rotationFlags   = reader.read<byte>();

            m_frames.clear();
            for (int f = 0; f < frameCount; ++f) m_frames.append(Frame(reader, parent));
        }

        inline void write(Writer &writer, Animation *parent)
        {
            writer.write(m_name + '\0');
            writer.write((ushort)m_frames.count());
            writer.write(m_speedMultiplyer);
            writer.write(m_loopIndex);
            writer.write(m_rotationFlags);
            for (int f = 0; f < m_frames.count(); ++f) m_frames[f].write(writer, parent);
        }

        QString m_name = "Animation";
        QList<Frame> m_frames;
        byte m_loopIndex        = 0;
        short m_speedMultiplyer = 0;
        byte m_rotationFlags    = 0;
    };

    Animation();
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
            filename = m_filename;
        if (filename == "")
            return;
        Writer writer(filename);
        write(writer);
    }
    void write(Writer &writer);

    byte m_signature[4] = { 'S', 'P', 'R', 0 };

    QList<QString> m_sheets;
    QList<QString> m_hitboxTypes;
    QList<AnimationEntry> m_animations;

    QString m_filename = "";
};

} // namespace RSDKv5
#endif // ANIMATION_V5_H
