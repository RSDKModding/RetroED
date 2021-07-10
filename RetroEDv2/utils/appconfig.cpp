#include "includes.hpp"

void AppConfig::read(Reader &reader)
{
    m_filename = reader.filepath;

    byte fVer = reader.read<byte>();

    switch (fVer) {
        default:
        case 1: break;
    }

    byte recentFiles = reader.read<byte>();
    m_recentFiles.clear();
    for (int e = 0; e < recentFiles; ++e) m_recentFiles.append(RecentFileInfo(reader));
}

void AppConfig::write(Writer &writer)
{
    m_filename = writer.filePath;

    writer.write(m_fileVer);

    writer.write((byte)m_recentFiles.count());
    for (auto &rf : m_recentFiles) rf.write(writer);

    writer.flush();
}

void AppConfig::addRecentFile(byte gameVer, byte tool, QString path, QList<QString> extra)
{
    RecentFileInfo rf;
    rf.m_gameVer = gameVer;
    rf.m_tool    = tool;
    rf.m_path    = path;
    rf.m_extra   = extra;

    int index = -1;
    for (int i = 0; i < m_recentFiles.count(); ++i) {
        if (m_recentFiles[i].m_path == path) {
            index = i;
            break;
        }
    }

    if (index >= 0)
        m_recentFiles.removeAt(index);
    m_recentFiles.prepend(rf);
    while (m_recentFiles.count() > 15) m_recentFiles.removeAt(15);
    write(appDir + "appConfig.bin");
}

void AppConfig::RecentFileInfo::read(Reader &reader)
{
    m_gameVer = reader.read<byte>();
    m_tool    = reader.read<byte>();
    m_path    = reader.readString();

    byte extraCount = reader.read<byte>();
    m_extra.clear();
    for (int e = 0; e < extraCount; ++e) m_extra.append(reader.readString());
}

void AppConfig::RecentFileInfo::write(Writer &writer)
{
    writer.write(m_gameVer);
    writer.write(m_tool);
    writer.write(m_path);

    writer.write((byte)m_extra.count());
    for (auto &s : m_extra) writer.write(s);
}
