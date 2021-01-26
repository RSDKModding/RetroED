#ifndef APPCONFIG_H
#define APPCONFIG_H

enum TooIDs {
    TOOL_SCENEEDITOR,
    TOOL_SCRIPTUNPACKER,
    TOOL_RSDKUNPACKER,
    TOOL_PALETTEDITOR,
    TOOL_GFXTOOL,
    TOOL_GAMECONFIGEDITOR,
    TOOL_RSVUNPACKER,
    TOOL_STATICOBJECTEDITOR,
    TOOL_SAVEEDITOR,
};

class AppConfig
{
public:
    struct RecentFileInfo {
        RecentFileInfo() {}
        RecentFileInfo(Reader &reader) { read(reader); }

        void read(Reader &reader);
        void write(Writer &writer);

        byte m_gameVer = 0;
        byte m_tool    = 0xFF;
        QString m_path = "";
        QList<QString> m_extra;
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

    void addRecentFile(byte gameVer, byte tool, QString path, QList<QString> extra);

    QList<RecentFileInfo> m_recentFiles;

    QString m_filename;
    const byte m_fileVer = 1;
};

#endif // APPCONFIG_H
