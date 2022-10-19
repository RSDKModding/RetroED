#pragma once

enum TooIDs {
    TOOL_SCENEEDITOR,
    TOOL_ANIMATIONEDITOR,
    TOOL_RSDKUNPACKER,
    TOOL_PALETTEDITOR,
    TOOL_GAMECONFIGEDITOR,
    TOOL_MODELMANAGER,
};

class AppConfig
{
public:
    struct RecentFileInfo {
        RecentFileInfo() {}
        RecentFileInfo(Reader &reader) { read(reader); }

        friend bool operator==(const RecentFileInfo &a, const RecentFileInfo &b)
        {
            return a.gameVer == b.gameVer && a.path == b.path && a.tool == b.tool;
        }

        void read(Reader &reader);
        void write(Writer &writer);

        byte gameVer = 0;
        byte tool    = 0xFF;
        QString path = "";
        QList<QString> extra;
    };

    struct GameManagerInfo {
        GameManagerInfo() {}
        GameManagerInfo(Reader &reader) { read(reader); }

        void read(Reader &reader);
        void write(Writer &writer);

        QString exePath = "";
    };

    AppConfig() {}
    AppConfig(QString filename) { read(filename); }
    AppConfig(Reader &reader) { read(reader); }

    inline void read(QString filename)
    {
        Reader reader(filename);
        read(reader);
    }
    void read(Reader &reader);

    inline void write(QString filename = "")
    {
        if (filename == "")
            filename = filePath;
        if (filename == "")
            return;
        Writer writer(filename);
        write(writer);
    }
    void write(Writer &writer);

    void addRecentFile(byte gameVer, byte tool, QString path, QList<QString> extra);

    QList<RecentFileInfo> recentFiles;
    GameManagerInfo gameManager[ENGINE_v1 + 1];

    QString filePath;
    const byte fileVer = 1;
};
