#ifndef RSDK_STAGECONFIG_H
#define RSDK_STAGECONFIG_H

namespace FormatHelpers
{

class Stageconfig
{
public:
    class ObjectInfo
    {
    public:
        ObjectInfo() {}

        QString m_name   = "Object";
        QString script = "Folder/Script.txt";
        byte sheetID   = 0;
    };

    class SoundInfo
    {
    public:
        SoundInfo() {}

        QString m_name = "Sound";
        QString path = "Folder/Sound.wav";
    };

    Stageconfig() {}
    Stageconfig(byte ver, QString filepath) { read(ver, filepath); }

    void read(byte ver, QString filename);
    void write(byte ver, QString filename);

    Palette palette;
    QList<SoundInfo> soundFX;
    QList<ObjectInfo> objects;
    bool loadGlobalScripts = true;
    QList<QString> spriteSheets;
    QList<QString> music;

    QString filePath = "";
};

} // namespace FormatHelpers

#endif // RSDK_STAGECONFIG_H
