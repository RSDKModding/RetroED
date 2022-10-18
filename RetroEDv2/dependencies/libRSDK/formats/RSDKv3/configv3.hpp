#pragma once

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
        filePath = reader.m_filepath;

        width    = reader.read<int>();
        height   = reader.read<int>();
        fps      = reader.read<int>();
        windowed = reader.read<bool>();
    }

    inline void write(QString filename)
    {
        if (filename == "")
            filename = filePath;
        if (filename == "")
            return;
        Writer writer(filename);
        write(writer);
    }
    inline void write(Writer &writer)
    {
        filePath = writer.filePath;
        writer.write(width);
        writer.write(height);
        writer.write(fps);
        writer.write(windowed);

        writer.flush();
    }

    int width     = 400;
    int height    = 240;
    int fps       = 60;
    bool windowed = true;

    QString filePath = "";
};

} // namespace RSDKv3


