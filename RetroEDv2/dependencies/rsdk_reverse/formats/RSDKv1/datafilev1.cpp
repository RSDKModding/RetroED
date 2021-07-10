#include "include.hpp"

void RSDKv1::Datafile::read(Reader &reader)
{
    filepath = reader.filepath;

    int headerSize = reader.read<uint>();
    int dircount   = reader.read<byte>();

    directories.clear();
    for (int d = 0; d < dircount; ++d) {
        directories.append(DirInfo(reader));
    }

    files.clear();
    for (int d = 0; d < dircount; ++d) {
        if ((d + 1) < directories.count()) {
            while (reader.tell() - headerSize < directories[d + 1].offset && !reader.isEOF()) {
                FileInfo f     = FileInfo(reader);
                f.fullFilename = directories[d].directory + f.filename;
                f.dirID        = d;
                files.append(f);
            }
        }
        else {
            while (!reader.isEOF()) {
                FileInfo f     = FileInfo(reader);
                f.fullFilename = directories[d].directory + f.filename;
                f.dirID        = d;
                files.append(f);
            }
        }
    }
}

void RSDKv1::Datafile::write(Writer &writer)
{
    filepath = writer.filePath;

    int dirHeaderSize = 0;

    writer.write(dirHeaderSize);
    writer.write((byte)directories.count());

    // TODO: sort dirs by name
    // std::sort(directories.begin(), directories.end(), [](const DirInfo &a, const DirInfo &b) ->
    // bool { return a.directory < b.directory; });

    for (int i = 0; i < directories.count(); ++i) {
        directories[i].write(writer);
    }

    dirHeaderSize = (int)writer.tell();
    // std::sort(files.begin(), files.end(), [](const FileInfo &a, const FileInfo &b) -> bool {
    //    return a.dirID < b.dirID && a.filename < b.filename;
    //});

    int dir                 = 0;
    directories[dir].offset = 0;
    for (int i = 0; i < files.count(); ++i) {
        if (files[i].dirID == dir) {
            files[i].write(writer);
        }
        else {
            ++dir;
            directories[dir].offset = (int)writer.tell() - dirHeaderSize;
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

void RSDKv1::Datafile::FileInfo::read(Reader &reader)
{
    filename = reader.readString();
    fileSize = reader.read<uint>();
    fileData = reader.readByteArray(fileSize);
}

void RSDKv1::Datafile::FileInfo::write(Writer &writer)
{
    filename = filename.replace('\\', '/');

    writer.write(filename);
    writer.write((uint)fileSize);
    writer.write(fileData);
}

void RSDKv1::Datafile::DirInfo::read(Reader &reader)
{
    directory = reader.readString();
    offset    = reader.read<int>();
}

void RSDKv1::Datafile::DirInfo::write(Writer &writer)
{
    if (!directory.endsWith('/'))
        directory += "/";

    writer.write(directory);
    writer.write(offset);
}
