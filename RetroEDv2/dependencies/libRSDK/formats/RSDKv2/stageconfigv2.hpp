#pragma once

namespace RSDKv2
{

class StageConfig
{
public:
    StageConfig() {}
    StageConfig(QString filename) { read(filename); }
    StageConfig(Reader &reader) { read(reader); }

    inline void read(QString filename)
    {
        Reader reader(filename);
        read(reader);
    }
    void read(Reader &reader);

    inline void write(QString filename)
    {
        if (filename == "")
            filename = filePath;
        if (filename == "")
            return;
        Writer writer(filename);
        write(writer);
    }
    void write(Writer &writer);

    Palette palette;
    QList<QString> soundFX;
    QList<QString> objects;
    bool loadGlobalScripts = true;

    QString filePath = "";
};

} // namespace RSDKv2


