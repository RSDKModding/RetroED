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
        QString m_script = "Folder/Script.txt";
        byte m_sheetID   = 0;
    };

    class SoundInfo
    {
    public:
        SoundInfo() {}

        QString m_name = "Sound";
        QString m_path = "Folder/Sound.wav";
    };

    Stageconfig() {}
    Stageconfig(byte ver, QString filepath) { read(ver, filepath); }

    void read(byte ver, QString filename);
    void write(byte ver, QString filename);

    Palette m_stagePalette;
    QList<SoundInfo> m_soundFX;
    QList<ObjectInfo> m_objects;
    bool m_loadGlobalScripts = true;
    QList<QString> m_objectSpritesheets;
    QList<QString> m_music;

    QString m_filename = "";
};

} // namespace FormatHelpers

#endif // RSDK_STAGECONFIG_H
