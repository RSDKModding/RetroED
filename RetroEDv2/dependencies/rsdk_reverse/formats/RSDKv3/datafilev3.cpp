#include "include.hpp"

QString readEncString1(Reader &reader)
{
    byte ss     = reader.read<byte>();
    QString str = "";

    for (int i = 0; i < ss; ++i) str = str + (char)(reader.read<byte>() ^ (0xFF - ss));

    return str;
}

QString readEncString2(Reader &reader)
{
    byte ss     = reader.read<byte>();
    QString str = "";

    for (int i = 0; i < ss; ++i) str = str + (char)(reader.read<byte>() ^ 0xFF);

    return str;
}

void writeEncString1(Writer &writer, QString string)
{
    writer.write((byte)string.length());

    for (int i = 0; i < (byte)string.length(); ++i)
        writer.write((byte)((byte)string[i].toLatin1() ^ (0xFF - (byte)string.length())));
}

void writeEncString2(Writer &writer, QString string)
{
    writer.write((byte)string.length());

    for (int i = 0; i < (byte)string.length(); ++i)
        writer.write((byte)((byte)string[i].toLatin1() ^ 0xFF));
}

void RSDKv3::Datafile::read(Reader &reader)
{
    m_filename = reader.filepath;

    int headerSize = reader.read<uint>();
    int dircount   = reader.read<ushort>();

    directories.clear();
    for (int d = 0; d < dircount; ++d) {
        directories.append(DirInfo(reader));
    }

    files.clear();
    for (int d = 0; d < dircount; ++d) {
        if ((d + 1) < directories.count()) {
            while (reader.tell() - headerSize < directories[d + 1].m_address && !reader.isEOF()) {
                FileInfo f       = FileInfo(reader);
                f.fullFileName = directories[d].directory + f.fileName;
                f.m_dirID        = d;
                files.append(f);
            }
        }
        else {
            while (!reader.isEOF()) {
                FileInfo f       = FileInfo(reader);
                f.fullFileName = directories[d].directory + f.fileName;
                f.m_dirID        = d;
                files.append(f);
            }
        }
    }
}

void RSDKv3::Datafile::write(Writer &writer)
{
    m_filename = writer.filePath;

    int dirHeaderSize = 0;

    writer.write(dirHeaderSize);
    writer.write((ushort)directories.count());

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
    directories[dir].m_address = 0;
    for (int i = 0; i < files.count(); ++i) {
        if (files[i].m_dirID == dir) {
            files[i].write(writer);
        }
        else {
            ++dir;
            directories[dir].m_address = (int)writer.tell() - dirHeaderSize;
            files[i].write(writer);
        }
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
        if (files[i].m_dirID == dir) {
            files[i].write(writer);
        }
        else {
            ++dir;
            files[i].write(writer);
        }
    }

    writer.flush();
}

void RSDKv3::Datafile::FileInfo::read(Reader &reader)
{
    fileName = readEncString2(reader);
    fileSize = reader.read<uint>();
    fileData = reader.readByteArray(fileSize);

    m_eKeyNo   = ((int)fileSize & 0x1FC) >> 2;
    m_eKeyPosB = (m_eKeyNo % 9) + 1;
    m_eKeyPosA = (m_eKeyNo % m_eKeyPosB) + 1;

    m_eNybbleSwap = 0;

    for (int i = 0; i < (int)fileSize; ++i) {
        byte buf = (byte)fileData[i];

        buf ^= (byte)m_encryptionKeyB[m_eKeyPosB++] ^ m_eKeyNo;

        if (m_eNybbleSwap == 1) // swap nibbles
            buf = (buf >> 4) | ((buf & 0xf) << 4);

        buf ^= (byte)m_decryptionKeyA[m_eKeyPosA++];

        if ((m_eKeyPosA <= 19) || (m_eKeyPosB <= 11)) {
            if (m_eKeyPosA > 19) {
                m_eKeyPosA = 1;
                m_eNybbleSwap ^= 1;
            }
            if (m_eKeyPosB > 11) {
                m_eKeyPosB = 1;
                m_eNybbleSwap ^= 1;
            }
        }
        else {
            m_eKeyNo++;
            m_eKeyNo &= 0x7F;

            if (m_eNybbleSwap != 0) {
                m_eKeyPosA    = (m_eKeyNo % 12) + 6;
                m_eKeyPosB    = (m_eKeyNo % 5) + 4;
                m_eNybbleSwap = 0;
            }
            else {
                m_eNybbleSwap = 1;
                m_eKeyPosA    = (m_eKeyNo % 15) + 3;
                m_eKeyPosB    = (m_eKeyNo % 7) + 1;
            }
        }

        fileData[i] = buf;
    }
}

void RSDKv3::Datafile::FileInfo::write(Writer &writer)
{
    fileName = fileName.replace('\\', '/');
    fileSize = fileData.count();

    // Encrypt file
    m_eKeyNo   = ((int)fileSize & 0x1FC) >> 2;
    m_eKeyPosB = (m_eKeyNo % 9) + 1;
    m_eKeyPosA = (m_eKeyNo % m_eKeyPosB) + 1;

    m_eNybbleSwap = 0;

    QByteArray data = fileData;
    for (uint i = 0; i < fileSize; ++i) {
        byte buf = (byte)fileData[i];
        buf ^= (byte)m_decryptionKeyA[m_eKeyPosA++];

        if (m_eNybbleSwap == 1) // swap nibbles
            buf = (byte)((buf >> 4) | ((buf & 0xf) << 4));

        buf ^= (byte)((byte)m_encryptionKeyB[m_eKeyPosB++] ^ m_eKeyNo);

        if (m_eKeyPosA <= 19 || m_eKeyPosB <= 11) {
            if (m_eKeyPosA > 19) {
                m_eKeyPosA = 1;
                m_eNybbleSwap ^= 1;
            }
            if (m_eKeyPosB > 11) {
                m_eKeyPosB = 1;
                m_eNybbleSwap ^= 1;
            }
        }
        else {
            ++m_eKeyNo;
            m_eKeyNo &= 0x7F;

            if (m_eNybbleSwap != 0) {
                m_eKeyPosA    = (m_eKeyNo % 12) + 6;
                m_eKeyPosB    = (m_eKeyNo % 5) + 4;
                m_eNybbleSwap = 0;
            }
            else {
                m_eNybbleSwap = 1;
                m_eKeyPosA    = (m_eKeyNo % 15) + 3;
                m_eKeyPosB    = (m_eKeyNo % 7) + 1;
            }
        }

        data[i] = (byte)buf;
    }

    writeEncString2(writer, fileName);
    writer.write(fileSize);
    writer.write(data);
}

void RSDKv3::Datafile::DirInfo::read(Reader &reader)
{
    directory = readEncString1(reader);
    m_address   = reader.read<int>();
}

void RSDKv3::Datafile::DirInfo::write(Writer &writer)
{
    if (!directory.endsWith('/'))
        directory += "/";

    writeEncString1(writer, directory);
    writer.write(m_address);
}
