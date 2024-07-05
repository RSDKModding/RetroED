#include "includes.hpp"

void AppConfig::read(Reader &reader)
{
    filePath = reader.filePath;

    byte fVer = reader.read<byte>();

    switch (fVer) {
        default:
        case 1: break;
    }

    byte recentFileCount = reader.read<byte>();
    recentFiles.clear();
    for (int e = 0; e < recentFileCount; ++e) recentFiles.append(RecentFileInfo(reader));

    if (!reader.isEOF()) {
        for (int v = 0; v <= ENGINE_v1; ++v) {
            gameManager[v].read(reader);
            baseDataManager[v].read(reader);
        };
    }

    if (!reader.isEOF())
        lightMode   = reader.read<byte>();
    if (!reader.isEOF())
        vSync       = reader.read<byte>();
    if (!reader.isEOF())
        windowPos   = reader.read<QPoint>();
    if (!reader.isEOF())
        windowState = reader.read<byte>();
    if (!reader.isEOF())
        windowSize  = reader.read<QSize>();
}

void AppConfig::write(Writer &writer)
{
    filePath = writer.filePath;

    writer.write(fileVer);

    writer.write((byte)recentFiles.count());
    for (auto &rf : recentFiles) rf.write(writer);

    for (int v = 0; v <= ENGINE_v1; ++v) {
        gameManager[v].write(writer);
        baseDataManager[v].write(writer);
    };
    writer.write((byte)lightMode);
    writer.write((byte)vSync);
    writer.write(windowPos);
    writer.write((byte)windowState);
    writer.write((QSize)windowSize);

    writer.flush();
}

void AppConfig::addRecentFile(byte gameVer, byte tool, QString path, QList<QString> extra)
{
    RecentFileInfo rf;
    rf.gameVer = gameVer;
    rf.tool    = tool;
    rf.path    = path;
    rf.extra   = extra;

    int index = -1;
    for (int i = 0; i < recentFiles.count(); ++i) {
        if (recentFiles[i].path == path) {
            index = i;
            break;
        }
    }

    if (index >= 0)
        recentFiles.removeAt(index);
    recentFiles.prepend(rf);
    while (recentFiles.count() > 15) recentFiles.removeAt(15);
    write(appDir + "appConfig.bin");
}

void AppConfig::RecentFileInfo::read(Reader &reader)
{
    gameVer = reader.read<byte>();
    tool    = reader.read<byte>();
    path    = reader.readString();

    byte extraCount = reader.read<byte>();
    extra.clear();
    for (int e = 0; e < extraCount; ++e) extra.append(reader.readString());
}

void AppConfig::RecentFileInfo::write(Writer &writer)
{
    writer.write(gameVer);
    writer.write(tool);
    writer.write(path);

    writer.write((byte)extra.count());
    for (auto &s : extra) writer.write(s);
}

void AppConfig::GameManagerInfo::read(Reader &reader) { exePath = reader.readString(); }

void AppConfig::GameManagerInfo::write(Writer &writer) { writer.write(exePath); }

void AppConfig::BaseDataFolderInfo::read(Reader &reader) { dataPath = reader.readString(); }

void AppConfig::BaseDataFolderInfo::write(Writer &writer) { writer.write(dataPath); }
