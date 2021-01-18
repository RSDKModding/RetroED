#ifndef CONFIG_V3_H
#define CONFIG_V3_H

namespace RSDKv3
{

class Config
{
public:
    Config() {}
    Config(QString filename) { read(filename); }
    Config(Reader &reader) { read(reader); }

    inline void read(QString filename)
    {
        Reader reader(filename);
        read(reader);
    }
    inline void read(Reader &reader)
    {
        m_filename = reader.m_filepath;

        m_gameWidth  = reader.read<int>();
        m_gameHeight = reader.read<int>();
        m_gameFPS    = reader.read<int>();
        m_windowed   = reader.read<bool>();
    }

    inline void write(QString filename)
    {
        if (filename == "")
            filename = m_filename;
        if (filename == "")
            return;
        Writer writer(filename);
        write(writer);
    }
    inline void write(Writer &writer)
    {
        m_filename = writer.m_filename;
        writer.write(m_gameWidth);
        writer.write(m_gameHeight);
        writer.write(m_gameFPS);
        writer.write(m_windowed);

        writer.flush();
    }

    int m_gameWidth  = 1280;
    int m_gameHeight = 720;
    int m_gameFPS    = 60;
    bool m_windowed  = true;

    QString m_filename = "";
};

} // namespace RSDKv3

#endif // CONFIG_V3_H
