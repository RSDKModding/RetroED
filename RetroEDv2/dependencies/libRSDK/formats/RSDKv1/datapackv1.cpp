#include "libRSDK.hpp"

#include "datapackv1.hpp"

void RSDKv1::Datapack::read(Reader &reader)
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
            while (reader.tell() - headerSize < directories[d + 1].startOffset && !reader.isEOF()) {
                FileInfo f     = FileInfo(reader);
                f.fullFileName = directories[d].directory + f.fileName;
                f.dirID        = d;
                files.append(f);
            }
        }
        else {
            while (!reader.isEOF()) {
                FileInfo f     = FileInfo(reader);
                f.fullFileName = directories[d].directory + f.fileName;
                f.dirID        = d;
                files.append(f);
            }
        }
    }
}

void RSDKv1::Datapack::write(Writer &writer)
{
    filePath = writer.filePath;

    int dirHeaderSize = 0;

    writer.write(dirHeaderSize);
    writer.write((byte)directories.count());

    std::sort(directories.begin(), directories.end(), [](const DirInfo &a, const DirInfo &b) ->
              bool { return a.directory < b.directory; });

    for (int i = 0; i < directories.count(); ++i) {
        directories[i].write(writer);
    }

    dirHeaderSize = (int)writer.tell();

    std::sort(files.begin(), files.end(), [](const FileInfo &a, const FileInfo &b) -> bool {
        return a.dirID < b.dirID && a.fileName < b.fileName;
    });

    int dir                      = 0;
    directories[dir].startOffset = 0;
    for (int i = 0; i < files.count(); ++i) {
        files[i].fileName = QFileInfo(files[i].fileName).fileName();
        if (files[i].dirID != dir) {
            ++dir;
            directories[dir].startOffset = (int)writer.tell() - dirHeaderSize;
        }

        files[i].write(writer);
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
        if (files[i].dirID != dir)
            ++dir;

        files[i].write(writer);
    }

    writer.flush();
}

void RSDKv1::Datapack::FileInfo::read(Reader &reader)
{
    fileName = reader.readString();
    fileSize = reader.read<uint>();
    fileData = reader.readByteArray(fileSize);
}

void RSDKv1::Datapack::FileInfo::write(Writer &writer)
{
    fileName = fileName.replace('\\', '/');

    writer.write(fileName);
    writer.write((uint)fileSize);
    writer.write(fileData);
}

void RSDKv1::Datapack::DirInfo::read(Reader &reader)
{
    directory   = reader.readString();
    startOffset = reader.read<int>();
}

void RSDKv1::Datapack::DirInfo::write(Writer &writer)
{
    if (!directory.endsWith('/'))
        directory += "/";

    writer.write(directory);
    writer.write(startOffset);
}
