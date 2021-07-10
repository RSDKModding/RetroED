#ifndef STAGECONFIG_V3_H
#define STAGECONFIG_V3_H

namespace RSDKv3
{

class Stageconfig
{
public:
    class ObjectInfo
    {
    public:
        ObjectInfo() {}

        QString name   = "Object";
        QString script = "Folder/Script.txt";
    };

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

    Palette m_stagePalette;
    QList<QString> soundFX;
    QList<ObjectInfo> m_objects;
    bool loadGlobalScripts = true;

    QString m_filename = "";
};

} // namespace RSDKv3

#endif // STAGECONFIG_V3_H
