#ifndef STAGECONFIG_V1_H
#define STAGECONFIG_V1_H

namespace RSDKv2
{

class Stageconfig
{
public:
    Stageconfig() {}
    Stageconfig(QString filename) { read(filename); }
    Stageconfig(Reader &reader) { read(reader); }

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

    Palette palette;
    QList<QString> soundFX;
    QList<QString> scripts;
    bool loadGlobalScripts = true;

    QString m_filename = "";
};

} // namespace RSDKv2

#endif // STAGECONFIG_V1_H
