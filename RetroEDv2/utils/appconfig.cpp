#include "includes.hpp"

void AppConfig::read(Reader &reader)
{
    m_filename = reader.filepath;

    byte fVer = reader.read<byte>();

    switch (fVer) {
        default:
        case 1: break;
    }

    byte recentFileCount = reader.read<byte>();
    recentFiles.clear();
    for (int e = 0; e < recentFileCount; ++e) recentFiles.append(RecentFileInfo(reader));
}

void AppConfig::write(Writer &writer)
{
    m_filename = writer.filePath;

    writer.write(m_fileVer);

    writer.write((byte)recentFiles.count());
    for (auto &rf : recentFiles) rf.write(writer);

    writer.flush();
}

void AppConfig::addRecentFile(byte gameVer, byte tool, QString path, QList<QString> extra)
{
    RecentFileInfo rf;
    rf.gameVer = gameVer;
    rf.tool  = tool;
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
    tool  = reader.read<byte>();
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
