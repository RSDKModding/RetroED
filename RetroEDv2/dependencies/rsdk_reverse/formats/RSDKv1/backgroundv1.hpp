#ifndef BACKGROUND_VRS_H
#define BACKGROUND_VRS_H

namespace RSDKv1
{

class Background
{
public:
    class ScrollInfo
    {
    public:
        ScrollInfo() {}
        ScrollInfo(Reader &reader) { read(reader); }

        void read(Reader &reader)
        {
            m_relativeSpeed = reader.read<byte>();
            m_constantSpeed = reader.read<byte>();
            m_behaviour     = reader.read<byte>();
        }
        void write(Writer &writer)
        {
            writer.write(m_relativeSpeed);
            writer.write(m_constantSpeed);
            writer.write(m_behaviour);
        }

        byte m_relativeSpeed = 0;
        byte m_constantSpeed = 0;
        byte m_behaviour     = 0;
    };

    class Layer
    {
    public:
        Layer() {}
        Layer(Reader &reader) { read(reader); }

        void read(Reader &reader);
        void write(Writer &writer);

        QList<QList<byte>> m_layout;

        byte m_width         = 0;
        byte m_height        = 0;
        byte m_behaviour     = 0;
        byte m_relativeSpeed = 0;
        byte m_constantSpeed = 0;
        QByteArray m_lineIndexes;
    };

    Background() {}
    Background(QString filename) { read(filename); }
    Background(Reader &reader) { read(reader); }

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

    QList<Layer> m_layers;
    QList<ScrollInfo> m_hScroll;
    QList<ScrollInfo> m_vScroll;

    QString m_filename = "";
};

} // namespace RSDKv1

#endif // BACKGROUND_VRS_H
