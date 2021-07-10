#ifndef RSDK_GAMECONFIG_H
#define RSDK_GAMECONFIG_H

namespace FormatHelpers
{

class Gameconfig
{
public:
    class SceneInfo
    {
    public:
        SceneInfo() {}

        QString m_name     = "Scene";
        QString m_folder   = "Folder";
        QString m_actID    = "1";
        bool m_highlighted = false;
    };

    class Category
    {
    public:
        Category() {}

        QList<SceneInfo> m_scenes;
    };

    class GlobalVariable
    {
    public:
        GlobalVariable() {}

        QString m_name = "Variable";
        int m_value    = 0;
    };

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

    class PlayerInfo
    {
    public:
        PlayerInfo() {}

        QString m_name   = "PLAYER";
        QString m_anim   = "player.ani";
        QString m_script = "Player/Player.txt";
    };

    Gameconfig() {}
    Gameconfig(byte ver, QString filepath) { read(ver, filepath); }

    void read(byte ver, QString filename);
    void write(byte ver, QString filename);

    QString m_gameWindowText;
    QString m_gameDescriptionText;
    QString m_unknown;

    Palette m_masterPalette;
    QList<ObjectInfo> objects;
    QList<SoundInfo> m_soundFX;
    QList<GlobalVariable> m_globalVariables;
    QList<PlayerInfo> m_players;
    QList<Category> m_categories;

    QString m_filename = "";
};

} // namespace FormatHelpers

#endif // RSDK_GAMECONFIG_H
