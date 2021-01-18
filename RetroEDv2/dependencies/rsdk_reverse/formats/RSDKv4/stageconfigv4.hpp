#ifndef STAGECONFIG_V4_H
#define STAGECONFIG_V4_H

namespace RSDKv4
{

class Stageconfig
{
public:
    class ObjectInfo
    {
    public:
        ObjectInfo() {}

        QString m_name   = "Object";
        QString m_script = "Folder/Script.txt";
    };

    class SoundInfo
    {
    public:
        SoundInfo() {}

        QString m_name = "Sound";
        QString m_path = "Folder/Sound.wav";
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
    QList<SoundInfo> m_soundFX;
    QList<ObjectInfo> m_objects;
    bool m_loadGlobalScripts = true;

    QString m_filename = "";
};

} // namespace RSDKv4

#endif // STAGECONFIG_V4_H
