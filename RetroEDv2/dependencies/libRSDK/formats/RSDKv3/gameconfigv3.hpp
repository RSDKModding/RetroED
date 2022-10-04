#ifndef GAMECONFIG_V3_H
#define GAMECONFIG_V3_H

namespace RSDKv3
{

class GameConfig
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
            id          = reader.readString();
            name        = reader.readString();
            highlighted = reader.read<bool>();
        }

        inline void write(Writer &writer)
        {
            writer.write(folder);
            writer.write(id);
            writer.write(name);
            writer.write(highlighted);
        }

        QString name     = "Scene";
        QString folder   = "Folder";
        QString id       = "1";
        bool highlighted = false;
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
            name             = reader.readString();
            QByteArray bytes = reader.readByteArray(4);
            value            = ((byte)bytes[0] << 24) + ((byte)bytes[1] << 16) + ((byte)bytes[2] << 8)
                    + ((byte)bytes[3] << 0);
        }
        inline void write(Writer &writer)
        {
            writer.write(name);
            writer.write((byte)((value >> 0x18) & 0xFF));
            writer.write((byte)((value >> 0x10) & 0xFF));
            writer.write((byte)((value >> 0x08) & 0xFF));
            writer.write((byte)((value >> 0x00) & 0xFF));
        }

        QString name = "Variable";
        int value    = 0;
    };

    class ObjectInfo
    {
    public:
        ObjectInfo() {}

        QString name   = "Object";
        QString script = "Folder/Script.txt";
    };

    GameConfig()
    {
        stageLists.clear();
        for (int c = 0; c < 4; ++c) {
            stageLists.append(Category());
        }
    }
    GameConfig(QString filename) { read(filename); }
    GameConfig(Reader &reader) { read(reader); }

    inline void read(QString filename)
    {
        Reader reader(filename);
        read(reader);
    }
    void read(Reader &reader);

    inline void write(QString filename)
    {
        if (filename == "")
            filename = filePath;
        if (filename == "")
            return;
        Writer writer(filename);
        write(writer);
    }
    void write(Writer &writer);

    QString gameWindowText;
    QString unknown;
    QString gameDescriptionText;

    QList<ObjectInfo> objects;
    QList<QString> soundFX;
    QList<GlobalVariable> globalVariables;
    QList<QString> players;
    QList<Category> stageLists;

    QString filePath = "";
};

} // namespace RSDKv3

#endif // GAMECONFIG_V3_H
