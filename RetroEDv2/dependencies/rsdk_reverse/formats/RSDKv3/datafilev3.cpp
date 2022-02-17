#include "rsdkreverse.hpp"

QString readDataFileDirNamev3(Reader &reader)
{
    byte ss     = reader.read<byte>();
    QString str = "";

    for (int i = 0; i < ss; ++i) str = str + (char)(reader.read<byte>() ^ (0xFF - ss));

    return str;
}

QString readDataFileNamev3(Reader &reader)
{
    byte ss     = reader.read<byte>();
    QString str = "";

    for (int i = 0; i < ss; ++i) str = str + (char)(reader.read<byte>() ^ 0xFF);

    return str;
}

void writeDataFileDirNamev3(Writer &writer, QString string)
{
    writer.write((byte)string.length());

    for (int i = 0; i < (byte)string.length(); ++i)
        writer.write((byte)((byte)string[i].toLatin1() ^ (0xFF - (byte)string.length())));
}

void writeDataFileNamev3(Writer &writer, QString string)
{
    writer.write((byte)string.length());

    for (int i = 0; i < (byte)string.length(); ++i)
        writer.write((byte)((byte)string[i].toLatin1() ^ 0xFF));
}

void RSDKv3::Datafile::read(Reader &reader)
{
    filePath = reader.filePath;

    int headerSize = reader.read<uint>();
    int dircount   = reader.read<ushort>();

    directories.clear();
    for (int d = 0; d < dircount; ++d) directories.append(DirInfo(reader));

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

void RSDKv3::Datafile::write(Writer &writer)
{
    filePath = writer.filePath;

    int dirHeaderSize = 0;

    writer.write(dirHeaderSize);
    writer.write((ushort)directories.count());

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

    int dir                      = 0;
    directories[dir].startOffset = 0;
    for (int i = 0; i < files.count(); ++i) {
        if (files[i].dirID != dir) {
            ++dir;
            directories[dir].startOffset = (int)writer.tell() - dirHeaderSize;
        }

        files[i].write(writer);
    }

    // 2nd pass
    writer.seek(0);

    writer.write(dirHeaderSize);
    writer.write((ushort)directories.count());

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

void RSDKv3::Datafile::FileInfo::read(Reader &reader)
{
    fileName = readDataFileNamev3(reader);
    fileSize = reader.read<uint>();
    fileData = decrypt(reader.readByteArray(fileSize), false);
}

void RSDKv3::Datafile::FileInfo::write(Writer &writer)
{
    fileName = fileName.replace('\\', '/');
    fileSize = fileData.count();

    writeDataFileNamev3(writer, fileName);
    writer.write(fileSize);
    writer.write(decrypt(fileData, true));
}

void RSDKv3::Datafile::DirInfo::read(Reader &reader)
{
    directory   = readDataFileDirNamev3(reader);
    startOffset = reader.read<int>();
}

void RSDKv3::Datafile::DirInfo::write(Writer &writer)
{
    if (!directory.endsWith('/'))
        directory += "/";

    writeDataFileDirNamev3(writer, directory);
    writer.write(startOffset);
}

QByteArray RSDKv3::Datafile::FileInfo::decrypt(QByteArray data, bool encrypting)
{
    eKeyNo   = ((int)fileSize & 0x1FC) >> 2;
    eKeyPosB = (eKeyNo % 9) + 1;
    eKeyPosA = (eKeyNo % eKeyPosB) + 1;

    eNybbleSwap = 0;

    for (int i = 0; i < (int)fileSize; ++i) {
        byte encByte = (byte)data[i];

        if (encrypting) {
            encByte ^= (byte)decryptionKeyA[eKeyPosA++];

            if (eNybbleSwap == 1) // swap nibbles
                encByte = (byte)((encByte >> 4) | ((encByte & 0xF) << 4));

            encByte ^= (byte)((byte)encryptionKeyB[eKeyPosB++] ^ eKeyNo);
        }
        else {
            encByte ^= (byte)encryptionKeyB[eKeyPosB++] ^ eKeyNo;

            if (eNybbleSwap == 1) // swap nibbles
                encByte = (encByte >> 4) | ((encByte & 0xF) << 4);

            encByte ^= (byte)decryptionKeyA[eKeyPosA++];
        }
        data[i] = encByte;

        if ((eKeyPosA <= 19) || (eKeyPosB <= 11)) {
            if (eKeyPosA > 19) {
                eKeyPosA = 1;
                eNybbleSwap ^= 1;
            }
            if (eKeyPosB > 11) {
                eKeyPosB = 1;
                eNybbleSwap ^= 1;
            }
        }
        else {
            eKeyNo++;
            eKeyNo &= 0x7F;

            if (eNybbleSwap != 0) {
                eKeyPosA    = (eKeyNo % 12) + 6;
                eKeyPosB    = (eKeyNo % 5) + 4;
                eNybbleSwap = 0;
            }
            else {
                eNybbleSwap = 1;
                eKeyPosA    = (eKeyNo % 15) + 3;
                eKeyPosB    = (eKeyNo % 7) + 1;
            }
        }
    }

    return data;
}
