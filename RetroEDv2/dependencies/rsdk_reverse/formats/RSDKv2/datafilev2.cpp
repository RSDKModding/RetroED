#include "include.hpp"

void RSDKv2::Datafile::read(Reader &reader)
{
    m_filename = reader.m_filepath;

    int headerSize = reader.read<uint>();
    int dircount   = reader.read<byte>();

    m_directories.clear();
    for (int d = 0; d < dircount; ++d) {
        m_directories.append(DirInfo(reader));
    }

    m_files.clear();
    for (int d = 0; d < dircount; ++d) {
        if ((d + 1) < m_directories.count()) {
            while (reader.tell() - headerSize < m_directories[d + 1].m_address) {
                FileInfo f       = FileInfo(reader);
                f.m_fullFilename = m_directories[d].m_directory + f.m_filename;
                f.m_dirID        = d;
                m_files.append(f);
            }
        }
        else {
            while (!reader.isEOF()) {
                FileInfo f       = FileInfo(reader);
                f.m_fullFilename = m_directories[d].m_directory + f.m_filename;
                f.m_dirID        = d;
                m_files.append(f);
            }
        }
    }
}

void RSDKv2::Datafile::write(Writer &writer)
{
    m_filename = writer.m_filename;

    int dirHeaderSize = 0;

    writer.write(dirHeaderSize);
    writer.write((byte)m_directories.count());

    // TODO: sort dirs by name
    // std::sort(m_directories.begin(), m_directories.end(), [](const DirInfo &a, const DirInfo &b) ->
    // bool { return a.m_directory < b.m_directory; });

    for (int i = 0; i < m_directories.count(); ++i) {
        m_directories[i].write(writer);
    }

    dirHeaderSize = (int)writer.tell();
    // std::sort(m_files.begin(), m_files.end(), [](const FileInfo &a, const FileInfo &b) -> bool {
    //    return a.m_dirID < b.m_dirID && a.m_filename < b.m_filename;
    //});

    int dir                      = 0;
    m_directories[dir].m_address = 0;
    for (int i = 0; i < m_files.count(); ++i) {
        if (m_files[i].m_dirID == dir) {
            m_files[i].write(writer);
        }
        else {
            ++dir;
            m_directories[dir].m_address = (int)writer.tell() - dirHeaderSize;
            m_files[i].write(writer);
        }
    }

    // 2nd pass
    writer.seek(0);

    writer.write(dirHeaderSize);
    writer.write((byte)m_directories.count());

    for (int i = 0; i < m_directories.count(); ++i) {
        m_directories[i].write(writer);
    }

    dir = 0;
    for (int i = 0; i < m_files.count(); ++i) {
        if (m_files[i].m_dirID == dir) {
            m_files[i].write(writer);
        }
        else {
            ++dir;
            m_files[i].write(writer);
        }
    }

    writer.flush();
}

void RSDKv2::Datafile::FileInfo::read(Reader &reader)
{
    m_filename = reader.readString();
    m_fileSize = reader.read<uint>();
    m_filedata = reader.readByteArray(m_fileSize);

    if (QFileInfo(m_filename).suffix() != "ogg" && QFileInfo(m_filename).suffix() != "wav") {
        for (int i = 0; i < m_filedata.length(); ++i) m_filedata[i] = (byte)m_filedata[i] ^ 0xFF;
    }
}

void RSDKv2::Datafile::FileInfo::write(Writer &writer)
{
    m_filename = m_filename.replace('\\', '/');

    QByteArray data = m_filedata;
    if (QFileInfo(m_filename).suffix() != "ogg" && QFileInfo(m_filename).suffix() != "wav") {
        for (int i = 0; i < data.length(); ++i) data[i] = (byte)data[i] ^ 0xFF;
    }

    writer.write(m_filename);
    writer.write((uint)m_filedata.count());
    writer.write(data);
}

void RSDKv2::Datafile::DirInfo::read(Reader &reader)
{
    m_directory = reader.readString();
    m_address   = reader.read<int>();
}

void RSDKv2::Datafile::DirInfo::write(Writer &writer)
{
    if (!m_directory.endsWith('/'))
        m_directory += "/";

    writer.write(m_directory);
    writer.write(m_address);
}
