#include "include.hpp"

enum ExtensionTypes {
    UNKNOWN,
    OGG,
    WAV,
    CFG,
    PNG,
    MDL,
    OBJ,
    SPR,
    GIF,
    SCN,
    TIL,
};

void RSDKv5::Datafile::FileInfo::read(Reader &reader, QList<QString> fileList, int cnt)
{
    for (int y = 0; y < 16; y += 4) {
        m_md5Hash[y + 3] = reader.read<byte>();
        m_md5Hash[y + 2] = reader.read<byte>();
        m_md5Hash[y + 1] = reader.read<byte>();
        m_md5Hash[y]     = reader.read<byte>();
    }
    m_filenameHash = QByteArray((const char *)m_md5Hash, 0x10).toHex();

    fileName = "File " + QString::number(cnt + 1) + ".bin"; // Make a base name

    for (int i = 0; i < fileList.count(); ++i) {
        // RSDKv5 hashes all strings as lower case
        QString fp = fileList[i];
        fp         = fp.toLower();

        bool match = true;

        for (int z = 0; z < 16; ++z) {
            match = (byte)calculateMD5Hash(fp)[z] == m_md5Hash[z];
            if (!match)
                break;
        }

        if (match) {
            fileName = fileList[i];
            break;
        }
    }

    m_dataOffset = reader.read<uint>();
    uint buf     = reader.read<uint>();

    encrypted = (buf & 0x80000000) != 0;
    fileSize  = (buf & 0x7FFFFFFF);

    long tmp = reader.tell();
    reader.seek(m_dataOffset);

    fileData = reader.readByteArray(fileSize);

    // Decrypt File if Encrypted
    if (encrypted)
        fileData = decrypt(fileData);

    reader.seek(tmp);
}

void RSDKv5::Datafile::FileInfo::writeHeader(Writer &writer)
{
    fileName = fileName.replace('\\', '/');

    QString fn     = fileName;
    QByteArray md5 = calculateMD5Hash(fn.toLower());

    for (int y = 0; y < 16; y += 4) {
        m_md5Hash[y + 3] = md5[y + 0];
        m_md5Hash[y + 2] = md5[y + 1];
        m_md5Hash[y + 1] = md5[y + 2];
        m_md5Hash[y + 0] = md5[y + 3];
    }

    writer.write(m_md5Hash, 0x10);
    writer.write(m_dataOffset);
    encrypted = false; // temp
    writer.write(fileSize | (encrypted ? 0x80000000 : 0));
}

void RSDKv5::Datafile::FileInfo::writeData(Writer &writer)
{
    if (encrypted)
        writer.write(decrypt(fileData));
    else
        writer.write(fileData);
}

void RSDKv5::Datafile::FileInfo::generateELoadKeys(QString m_filename, uint size)
{
    QByteArray md5Buf;

    QString m_filenameCaps = m_filename.toUpper();
    md5Buf                 = calculateMD5Hash(m_filenameCaps);
    m_encryptionKeyA.resize(0x10);
    m_encryptionKeyB.resize(0x10);

    for (int y = 0; y < 16; y += 4) {
        // convert every 32-bit word to Little Endian
        m_encryptionKeyA[y + 3] = (byte)md5Buf[y + 0];
        m_encryptionKeyA[y + 2] = (byte)md5Buf[y + 1];
        m_encryptionKeyA[y + 1] = (byte)md5Buf[y + 2];
        m_encryptionKeyA[y + 0] = (byte)md5Buf[y + 3];
    }

    QString fsize = QString::number(size);
    md5Buf        = calculateMD5Hash(fsize);

    for (int y = 0; y < 16; y += 4) {
        // convert every 32-bit word to Little Endian
        m_encryptionKeyB[y + 3] = (byte)md5Buf[y + 0];
        m_encryptionKeyB[y + 2] = (byte)md5Buf[y + 1];
        m_encryptionKeyB[y + 1] = (byte)md5Buf[y + 2];
        m_encryptionKeyB[y + 0] = (byte)md5Buf[y + 3];
    }

    m_eKeyNo      = (int)(size / 4) & 0x7F;
    m_eKeyPosA    = 0;
    m_eKeyPosB    = 8;
    m_eNybbleSwap = 0;
}

QByteArray RSDKv5::Datafile::FileInfo::decrypt(QByteArray data)
{
    generateELoadKeys(fileName, fileSize);

    byte temp = 0;
    for (int i = 0; i < data.count(); ++i) {
        temp = m_eKeyNo ^ m_encryptionKeyB[m_eKeyPosB];
        temp ^= data[i];
        if (m_eNybbleSwap == 1) // swap nibbles: 0xAB <-> 0xBA
            temp = ((temp << 4) + (temp >> 4)) & 0xFF;

        temp ^= m_encryptionKeyA[m_eKeyPosA];
        data[i] = (byte)temp;

        m_eKeyPosA++;
        m_eKeyPosB++;

        if (m_eKeyPosA <= 0x0F) {
            if (m_eKeyPosB > 0x0C) {
                m_eKeyPosB = 0;
                m_eNybbleSwap ^= 0x01;
            }
        }
        else if (m_eKeyPosB <= 0x08) {
            m_eKeyPosA = 0;
            m_eNybbleSwap ^= 0x01;
        }
        else {
            m_eKeyNo += 2;
            m_eKeyNo &= 0x7F;

            if (m_eNybbleSwap != 0) {
                m_eNybbleSwap = 0;

                m_eKeyPosA = m_eKeyNo % 7;
                m_eKeyPosB = (m_eKeyNo % 0xC) + 2;
            }
            else {
                m_eNybbleSwap = 1;

                m_eKeyPosA = (m_eKeyNo % 0xC) + 3;
                m_eKeyPosB = m_eKeyNo % 7;
            }
        }
    }

    return data;
}

void RSDKv5::Datafile::read(Reader &reader, QList<QString> fileList)
{
    m_filename = reader.filepath;

    if (QByteArray((const char *)m_signature, 6) != reader.readByteArray(6))
        return;

    files.clear();
    int fileCount = reader.read<ushort>(); // read the header data
    for (int i = 0; i < fileCount; ++i)
        files.append(FileInfo(reader, fileList, i)); // read each file's header
}

void RSDKv5::Datafile::write(Writer &writer)
{
    m_filename = writer.filePath;

    for (int h = 0; h < 6; ++h) writer.write(m_signature[h]);

    // std::sort(m_files.begin(), m_files.end(), [](const FileInfo &a, const FileInfo &b) -> bool {
    // return a.m_filename < b.m_filename; });

    writer.write((ushort)files.count()); // write the header
    for (FileInfo &f : files) {
        f.writeHeader(writer); // write our file header data
    }

    for (FileInfo &f : files) {
        f.m_dataOffset = (uint)writer.tell();
        QByteArray b;
        b.resize(f.fileSize);
        writer.write(b);
    }

    writer.seek(0); // jump back to the start of the file

    for (int h = 0; h < 6; ++h) writer.write(m_signature[h]);
    writer.write((ushort)files.count());

    for (FileInfo &f : files) {
        f.writeHeader(writer);
        long tmp = writer.tell();
        writer.seek(f.m_dataOffset);
        f.writeData(writer);
        writer.seek(tmp);
    }

    writer.flush();
}
