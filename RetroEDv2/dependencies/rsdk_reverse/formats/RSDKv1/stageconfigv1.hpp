#ifndef STAGECONFIG_VRS_H
#define STAGECONFIG_VRS_H

namespace RSDKv1
{

class Stageconfig
{
public:
    class ObjectInfo
    {
    public:
        ObjectInfo() {}

        byte m_sheetID   = 0;
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
            filename = filepath;
        if (filename == "")
            return;
        Writer writer(filename);
        write(writer);
    }
    void write(Writer &writer);

    Palette palette;
    QList<QString> spriteSheets;
    QList<ObjectInfo> objects;
    QList<QString> music;
    QList<QString> soundFX;

    QString filepath = "";
};

} // namespace RSDKv1

#endif // STAGECONFIG_VRS_H
