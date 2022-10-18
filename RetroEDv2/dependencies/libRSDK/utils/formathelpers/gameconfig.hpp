#pragma once

namespace FormatHelpers
{

class GameConfig
{
public:
    class SceneInfo
    {
    public:
        SceneInfo() {}

        QString m_name   = "Scene";
        QString folder   = "Folder";
        QString id       = "1";
        bool highlighted = false;
    };

    class Category
    {
    public:
        Category() {}

        QList<SceneInfo> scenes;
    };

    class GlobalVariable
    {
    public:
        GlobalVariable() {}

        QString name = "Variable";
        int value      = 0;
    };

    class ObjectInfo
    {
    public:
        ObjectInfo() {}

        QString name = "Object";
        QString script = "Folder/Script.txt";
    };

    class SoundInfo
    {
    public:
        SoundInfo() {}

        QString name = "Sound";
        QString path   = "Folder/Sound.wav";
    };

    class PlayerInfo
    {
    public:
        PlayerInfo() {}

        QString name = "PLAYER";
        QString anim   = "player.ani";
        QString script = "Player/Player.txt";
    };

    GameConfig()
    {
        stageLists.clear();
        for (int i = 0; i < 4; ++i) stageLists.append(Category());
    }
    GameConfig(byte ver, QString filepath) { read(ver, filepath); }

    void read(byte ver, QString filename);
    void write(byte ver, QString filename);

    QString gameWindowText;
    QString gameDescriptionText;
    QString unknown;

    Palette palette;
    QList<ObjectInfo> objects;
    QList<SoundInfo> soundFX;
    QList<GlobalVariable> globalVariables;
    QList<PlayerInfo> players;
    QList<Category> stageLists;

    QString filePath = "";
};

} // namespace FormatHelpers


