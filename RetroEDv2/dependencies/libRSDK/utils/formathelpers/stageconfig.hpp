#pragma once

namespace FormatHelpers
{

class StageConfig
{
public:
    class ObjectInfo
    {
    public:
        ObjectInfo() {}

        QString name   = "Object";
        QString script = "Folder/Script.txt";
        byte sheetID   = 0;
    };

    class SoundInfo
    {
    public:
        SoundInfo() {}

        QString name = "Sound";
        QString path = "Folder/Sound.wav";
    };

    StageConfig() {}
    StageConfig(byte ver, QString filepath) { read(ver, filepath); }

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


