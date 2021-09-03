#ifndef GAMECONFIG_V5_H
#define GAMECONFIG_V5_H

namespace RSDKv5
{

class GameConfig
{
public:
    class SoundInfo
    {
    public:
        QString path         = "SFX.wav";
        byte maxConcurrentPlay = 1;

        SoundInfo() {}
        SoundInfo(QString name, byte maxPlays)
        {
            path            = name;
            maxConcurrentPlay = maxPlays;
        }
        SoundInfo(Reader &reader) { read(reader); }

        inline void read(Reader reader)
        {
            path            = reader.readString();
            maxConcurrentPlay = reader.read<byte>();
        }

        inline void write(Writer &writer)
        {
            writer.write(path);
            writer.write(maxConcurrentPlay);
        }
    };

    class SceneInfo
    {
    public:
        QString name  = "Scene";
        QString folder  = "";
        QString id = "";
        byte filter = 0;

        SceneInfo() {}
        SceneInfo(Reader &reader, bool readMode = true) { read(reader, readMode); }

        inline void read(Reader &reader, bool readMode = true)
        {
            name  = reader.readString();
            folder  = reader.readString();
            id = reader.readString();
            if (readMode)
                filter = reader.read<byte>();
        }

        inline void write(Writer &writer, bool readMode = true)
        {
            writer.write(name);
            writer.write(folder);
            writer.write(id);

            if (readMode)
                writer.write(filter);
        }
    };

    class Category
    {
    public:
        QString name = "New Category";
        QList<SceneInfo> scenes;

        Category()
        {
            name = "New Category";
            scenes.clear();
            scenes.append(SceneInfo());
        }
        Category(Reader &reader, bool readMode = true) { read(reader, readMode); }

        inline void read(Reader &reader, bool readMode = true)
        {
            name = reader.readString();

            byte sceneCount = reader.read<byte>();
            scenes.clear();
            for (int i = 0; i < sceneCount; ++i) scenes.append(SceneInfo(reader, readMode));
        }

        inline void write(Writer &writer, bool readMode = true)
        {
            writer.write(name);

            writer.write((byte)scenes.count());
            for (SceneInfo &scene : scenes) scene.write(writer, readMode);
        }
    };

    class GlobalVariable
    {
    public:
        int index;
        QList<int> values;

        GlobalVariable() {}
        GlobalVariable(Reader &reader) { read(reader); }

        void read(Reader &reader)
        {
            index     = reader.read<int>();
            int count = reader.read<int>();
            values.clear();
            for (int i = 0; i < count; ++i) values.append(reader.read<int>());
        }

        inline void write(Writer &writer)
        {
            writer.write(index);
            writer.write((int)values.count());
            for (int &val : values) writer.write(val);
        }
    };

    GameConfig() {}
    GameConfig(QString filename, bool oldVer = false) { read(filename, oldVer); }
    GameConfig(Reader &reader, bool oldVer = false) { read(reader, oldVer); }

    inline void read(QString filename, bool oldVer = false)
    {
        Reader reader(filename);
        read(reader, oldVer);
    }
    void read(Reader &reader, bool oldVer = false);

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

    byte signature[4] = { 'C', 'F', 'G', 0 };

    QString gameTitle            = "RSDKv5 Game";
    QString gameSubtitle         = "Powered By the Retro Engine!";
    QString version              = "1.00.0";
    byte startSceneCategoryIndex = 0;
    ushort startSceneIndex       = 0;

    QList<QString> objects;
    RSDKv5::Palette palettes[8];
    QList<SoundInfo> soundFX;

    QList<Category> categories;
    QList<GlobalVariable> globalVariables;

    QString m_filename = "";
    bool readMode      = false;
};

} // namespace RSDKv5

#endif // GAMECONFIG_V5_H
