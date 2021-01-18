#ifndef ANIMATION_VRS_H
#define ANIMATION_VRS_H

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

        Frame() {}
        Frame(Reader &reader) { read(reader); }

        inline void read(Reader &reader)
        {
            m_sprX   = reader.read<byte>();
            m_sprY   = reader.read<byte>();
            m_width  = reader.read<byte>();
            m_height = reader.read<byte>();
            m_sheet  = reader.read<byte>();

            m_hitbox.m_left   = reader.read<char>();
            m_hitbox.m_top    = reader.read<char>();
            m_hitbox.m_right  = reader.read<char>();
            m_hitbox.m_bottom = reader.read<char>();

            byte vals[2];
            vals[0] = (byte)-reader.read<byte>();
            vals[1] = (byte)-reader.read<byte>();

            m_pivotX = (char)vals[0];
            m_pivotY = (char)vals[1];
        }

        inline void write(Writer &writer)
        {
            writer.write(m_sprX);
            writer.write(m_sprY);
            writer.write(m_width);
            writer.write(m_height);
            writer.write(m_sheet);

            writer.write(m_hitbox.m_left);
            writer.write(m_hitbox.m_top);
            writer.write(m_hitbox.m_right);
            writer.write(m_hitbox.m_bottom);

            byte px = (byte)m_pivotX;
            byte py = (byte)m_pivotY;
            writer.write((byte)-px);
            writer.write((byte)-py);
        }

        byte m_sheet = 0;
        Hitbox m_hitbox;
        byte m_sprX   = 0;
        byte m_sprY   = 0;
        byte m_width  = 0;
        byte m_height = 0;
        char m_pivotX = 0;
        char m_pivotY = 0;
    };

    class AnimationEntry
    {
    public:
        AnimationEntry() {}
        AnimationEntry(Reader &reader) { read(reader); }

        inline void read(Reader &reader)
        {
            byte frameCount   = reader.read<byte>();
            m_speedMultiplyer = (byte)(reader.read<byte>() * 4);
            m_loopIndex       = reader.read<byte>();

            m_frames.clear();
            for (int f = 0; f < frameCount; ++f) m_frames.append(Frame(reader));
        }

        inline void write(Writer &writer)
        {
            writer.write((byte)m_frames.count());
            writer.write((byte)(m_speedMultiplyer / 4));
            writer.write(m_loopIndex);

            for (int f = 0; f < m_frames.count(); ++f) m_frames[f].write(writer);
        }

        QList<Frame> m_frames;
        byte m_loopIndex      = 0;
        int m_speedMultiplyer = 0;
    };

    Animation();
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
            filename = m_filename;
        if (filename == "")
            return;
        Writer writer(filename);
        write(writer, dcVer);
    }
    void write(Writer &writer, bool dcVer = false);

    byte m_unknown    = 0;
    byte m_playerType = 0;

    QList<QString> m_sheets;
    QList<AnimationEntry> m_animations;

    QString m_filename = "";
};

} // namespace RSDKv1

#endif // ANIMATION_VRS_H
