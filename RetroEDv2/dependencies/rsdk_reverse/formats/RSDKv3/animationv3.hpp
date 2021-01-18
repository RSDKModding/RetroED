#ifndef ANIMATION_V3_H
#define ANIMATION_V3_H

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
                m_left   = reader.read<char>();
                m_top    = reader.read<char>();
                m_right  = reader.read<char>();
                m_bottom = reader.read<char>();
            }

            inline void write(Writer &writer)
            {
                writer.write(m_left);
                writer.write(m_top);
                writer.write(m_right);
                writer.write(m_bottom);
            }

            char m_left   = 0;
            char m_top    = 0;
            char m_right  = 0;
            char m_bottom = 0;
        };

        Hitbox() {}
        Hitbox(Reader &reader) { read(reader); }

        inline void read(Reader &reader)
        {
            for (int h = 0; h < 8; ++h) m_hitboxes[h].read(reader);
        }

        inline void write(Writer &writer)
        {
            for (int h = 0; h < 8; ++h) m_hitboxes[h].write(writer);
        }

        HitboxInfo m_hitboxes[8];
    };

    class Frame
    {
    public:
        Frame() {}
        Frame(Reader &reader) { read(reader); }

        inline void read(Reader &reader)
        {
            m_sheet        = reader.read<byte>();
            m_collisionBox = reader.read<byte>();
            m_sprX         = reader.read<byte>();
            m_sprY         = reader.read<byte>();
            m_width        = reader.read<byte>();
            m_height       = reader.read<byte>();
            m_pivotX       = reader.read<char>();
            m_pivotY       = reader.read<char>();
        }

        inline void write(Writer &writer)
        {
            writer.write(m_sheet);
            writer.write(m_collisionBox);
            writer.write(m_sprX);
            writer.write(m_sprY);
            writer.write(m_width);
            writer.write(m_height);
            writer.write(m_pivotX);
            writer.write(m_pivotY);
        }

        byte m_sheet        = 0;
        byte m_collisionBox = 0;
        byte m_sprX         = 0;
        byte m_sprY         = 0;
        byte m_width        = 0;
        byte m_height       = 0;
        char m_pivotX       = 0;
        char m_pivotY       = 0;
    };

    class AnimationEntry
    {
    public:
        AnimationEntry() {}
        AnimationEntry(Reader &reader) { read(reader); }

        inline void read(Reader &reader)
        {
            m_name            = reader.readString();
            short frameCount  = reader.read<byte>();
            m_speedMultiplyer = reader.read<byte>();
            m_loopIndex       = reader.read<byte>();
            m_rotationFlags   = reader.read<byte>();

            m_frames.clear();
            for (int f = 0; f < frameCount; ++f) m_frames.append(Frame(reader));
        }

        inline void write(Writer &writer)
        {
            writer.write(m_name);
            writer.write((byte)m_frames.count());
            writer.write(m_speedMultiplyer);
            writer.write(m_loopIndex);
            writer.write(m_rotationFlags);
            for (int f = 0; f < m_frames.count(); ++f) m_frames[f].write(writer);
        }

        QString m_name = "Animation";
        QList<Frame> m_frames;
        byte m_loopIndex       = 0;
        byte m_speedMultiplyer = 0;
        byte m_rotationFlags   = 0;
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

    QList<QString> m_sheets;
    QList<AnimationEntry> m_animations;
    QList<Hitbox> m_hitboxes;

    QString m_filename = "";
};

} // namespace RSDKv3

#endif // ANIMATION_V3_H
