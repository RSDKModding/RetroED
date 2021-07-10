#ifndef GAMECONFIG_V4_H
#define GAMECONFIG_V4_H

namespace RSDKv4
{

class Gameconfig
{
public:
    class SceneInfo
    {
    public:
        SceneInfo() {}
        SceneInfo(Reader &reader) { read(reader); }

        inline void read(Reader &reader)
        {
            folder      = reader.readString();
            m_actID       = reader.readString();
            name        = reader.readString();
            m_highlighted = reader.read<bool>();
        }

        inline void write(Writer &writer)
        {
            writer.write(folder);
            writer.write(m_actID);
            writer.write(name);
            writer.write(m_highlighted);
        }

        QString name     = "Scene";
        QString folder   = "Folder";
        QString m_actID    = "1";
        bool m_highlighted = false;
    };

    class Category
    {
    public:
        Category() {}
        Category(Reader &reader) { read(reader); }

        inline void read(Reader &reader)
        {
            byte sceneCount = reader.read<byte>();

            scenes.clear();
            for (int i = 0; i < sceneCount; ++i) {
                scenes.append(SceneInfo(reader));
            }
        }

        inline void write(Writer &writer)
        {
            writer.write((byte)scenes.count());
            for (SceneInfo &scn : scenes) {
                scn.write(writer);
            }
        }

        QList<SceneInfo> scenes;
    };

    class GlobalVariable
    {
    public:
        GlobalVariable() {}
        GlobalVariable(Reader &reader) { read(reader); }

        inline void read(Reader &reader)
        {
            m_name           = reader.readString();
            QByteArray bytes = reader.readByteArray(4);
            m_value          = ((byte)bytes[3] << 24) + ((byte)bytes[2] << 16) + ((byte)bytes[1] << 8)
                      + ((byte)bytes[0] << 0);
        }
        inline void write(Writer &writer)
        {
            writer.write(m_name);
            writer.write((byte)((m_value >> 0x00) & 0xFF));
            writer.write((byte)((m_value >> 0x08) & 0xFF));
            writer.write((byte)((m_value >> 0x10) & 0xFF));
            writer.write((byte)((m_value >> 0x18) & 0xFF));
        }

        QString m_name = "Variable";
        int m_value    = 0;
    };

    class ObjectInfo
    {
    public:
        ObjectInfo() {}

        QString name   = "Object";
        QString script = "Folder/Script.txt";
    };

    class SoundInfo
    {
    public:
        SoundInfo() {}

        QString m_name = "Sound";
        QString m_path = "Folder/Sound.wav";
    };

    Gameconfig()
    {
        categories.clear();
        for (int c = 0; c < 4; ++c) {
            categories.append(Category());
        }
    }
    Gameconfig(QString filename) { read(filename); }
    Gameconfig(Reader &reader) { read(reader); }

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

    QString m_gameWindowText;
    QString m_gameDescriptionText;

    Palette m_masterPalette;
    QList<ObjectInfo> objects;
    QList<SoundInfo> soundFX;
    QList<GlobalVariable> globalVariables;
    QList<QString> m_players;
    QList<Category> categories;

    QString m_filename = "";
};

} // namespace RSDKv4

#endif // GAMECONFIG_V4_H
