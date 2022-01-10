#include "rsdkreverse.hpp"

void RSDKv2::Datafile::read(Reader &reader)
{
    filePath = reader.filePath;

    int headerSize = reader.read<uint>();
    int dircount   = reader.read<byte>();

    directories.clear();
    for (int d = 0; d < dircount; ++d) {
        directories.append(DirInfo(reader));
    }

    files.clear();
    for (int d = 0; d < dircount; ++d) {
        if ((d + 1) < directories.count()) {
            while (reader.tell() - headerSize < directories[d + 1].startOffset) {
                FileInfo f       = FileInfo(reader);
                f.fullFileName = directories[d].directory + f.fileName;
                f.dirID        = d;
                files.append(f);
            }
        }
        else {
            while (!reader.isEOF()) {
                FileInfo f       = FileInfo(reader);
                f.fullFileName = directories[d].directory + f.fileName;
                f.dirID        = d;
                files.append(f);
            }
        }
    }
}

void RSDKv2::Datafile::write(Writer &writer)
{
    filePath = writer.filePath;

    int dirHeaderSize = 0;

    writer.write(dirHeaderSize);
    writer.write((byte)directories.count());

    // TODO: sort dirs by name
    // std::sort(m_directories.begin(), m_directories.end(), [](const DirInfo &a, const DirInfo &b) ->
    // bool { return a.m_directory < b.m_directory; });

    for (int i = 0; i < directories.count(); ++i) {
        directories[i].write(writer);
    }

    dirHeaderSize = (int)writer.tell();
    // std::sort(m_files.begin(), m_files.end(), [](const FileInfo &a, const FileInfo &b) -> bool {
    //    return a.m_dirID < b.m_dirID && a.m_filename < b.m_filename;
    //});

    int dir                      = 0;
    directories[dir].startOffset = 0;
    for (int i = 0; i < files.count(); ++i) {
        if (files[i].dirID == dir) {
            files[i].write(writer);
        }
        else {
            ++dir;
            directories[dir].startOffset = (int)writer.tell() - dirHeaderSize;
            files[i].write(writer);
        }
    }

    // 2nd pass
    writer.seek(0);

    writer.write(dirHeaderSize);
    writer.write((byte)directories.count());

    for (int i = 0; i < directories.count(); ++i) {
        directories[i].write(writer);
    }

    dir = 0;
    for (int i = 0; i < files.count(); ++i) {
        if (files[i].dirID == dir) {
            files[i].write(writer);
        }
        else {
            ++dir;
            files[i].write(writer);
        }
    }

    writer.flush();
}

void RSDKv2::Datafile::FileInfo::read(Reader &reader)
{
    fileName = reader.readString();
    fileSize = reader.read<uint>();
    fileData = reader.readByteArray(fileSize);

    if (QFileInfo(fileName).suffix() != "ogg" && QFileInfo(fileName).suffix() != "wav") {
        for (int i = 0; i < fileData.length(); ++i) fileData[i] = (byte)fileData[i] ^ 0xFF;
    }
}

void RSDKv2::Datafile::FileInfo::write(Writer &writer)
{
    fileName = fileName.replace('\\', '/');

    QByteArray data = fileData;
    if (QFileInfo(fileName).suffix() != "ogg" && QFileInfo(fileName).suffix() != "wav") {
        for (int i = 0; i < data.length(); ++i) data[i] = (byte)data[i] ^ 0xFF;
    }

    writer.write(fileName);
    writer.write((uint)fileData.count());
    writer.write(data);
}

void RSDKv2::Datafile::DirInfo::read(Reader &reader)
{
    directory = reader.readString();
    startOffset   = reader.read<int>();
}

void RSDKv2::Datafile::DirInfo::write(Writer &writer)
{
    if (!directory.endsWith('/'))
        directory += "/";

    writer.write(directory);
    writer.write(startOffset);
}
