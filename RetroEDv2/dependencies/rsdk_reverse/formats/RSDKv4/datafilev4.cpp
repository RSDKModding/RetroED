#include "include.hpp"

void RSDKv4::Datafile::read(Reader &reader, QList<QString> fileList)
{
    m_filename = reader.m_filepath;

    if (QByteArray((const char *)m_signature, 6) != reader.readByteArray(6))
        return;

    m_files.clear();
    int fileCount = reader.read<ushort>(); // read the header data
    for (int i = 0; i < fileCount; ++i)
        m_files.append(FileInfo(reader, fileList, i)); // read each file's header
}

void RSDKv4::Datafile::write(Writer &writer)
{
    m_filename = writer.m_filename;

    for (int h = 0; h < 6; ++h) writer.write(m_signature[h]);

    // std::sort(m_files.begin(), m_files.end(), [](const FileInfo &a, const FileInfo &b) -> bool {
    // return a.m_filename < b.m_filename; });

    writer.write((ushort)m_files.count()); // write the header
    for (FileInfo &f : m_files) {
        f.writeHeader(writer); // write our file header data
    }

    for (FileInfo &f : m_files) {
        f.m_dataOffset = (uint)writer.tell();
        QByteArray b;
        b.resize(f.m_fileSize);
        writer.write(b);
    }

    writer.seek(0); // jump back to the start of the file

    for (int h = 0; h < 6; ++h) writer.write(m_signature[h]);
    writer.write((ushort)m_files.count());

    for (FileInfo &f : m_files) {
        f.writeHeader(writer);
        long tmp = writer.tell();
        writer.seek(f.m_dataOffset);
        f.writeData(writer);
        writer.seek(tmp);
    }

    writer.flush();
}

void RSDKv4::Datafile::FileInfo::read(Reader &reader, QList<QString> fileList, int cnt)
{
    QByteArray hash;
    for (int y = 0; y < 16; y += 4) {
        m_md5Hash[y + 3] = reader.read<byte>();
        m_md5Hash[y + 2] = reader.read<byte>();
        m_md5Hash[y + 1] = reader.read<byte>();
        m_md5Hash[y + 0] = reader.read<byte>();
    }

    for (int i = 0; i < 16; ++i) hash.append(m_md5Hash[i]);

    m_filenameHash = hash.toHex();
    m_filename     = QString::number(cnt + 1) + ".bin"; // Make a base name

    for (int i = 0; i < fileList.count(); ++i) {
        // RSDK hashes all strings at lowercase
        QString fp = fileList[i].toLower();

        bool match = true;
        for (int z = 0; z < 16; ++z) {
            if ((byte)calculateMD5Hash(fp)[z] != (byte)m_md5Hash[z]) {
                match = false;
                break;
            }
        }

        if (match) {
            m_filename = fileList[i];
            break;
        }
    }

    m_dataOffset = reader.read<uint>();
    uint tmp     = reader.read<uint>();

    m_encrypted = (tmp & 0x80000000) != 0;
    m_fileSize  = (tmp & 0x7FFFFFFF);

    long tmp2 = reader.tell();
    reader.seek(m_dataOffset);

    m_filedata = reader.readByteArray(m_fileSize);

    // Decrypt File if Encrypted
    if (m_encrypted)
        m_filedata = decrypt(m_filedata);

    reader.seek(tmp2);

    m_extension = getExtensionFromData();
    if (m_filename == QString::number(cnt + 1) + ".bin") {
        switch (m_extension) {
            case ExtensionTypes::GIF: m_filename = "Sprite" + QString::number(cnt + 1) + ".gif"; break;
            case ExtensionTypes::R3D: m_filename = "Model" + QString::number(cnt + 1) + ".bin"; break;
            case ExtensionTypes::OGG: m_filename = "Music" + QString::number(cnt + 1) + ".ogg"; break;
            case ExtensionTypes::PNG: m_filename = "Image" + QString::number(cnt + 1) + ".png"; break;
            case ExtensionTypes::WAV:
                m_filename = "SoundEffect" + QString::number(cnt + 1) + ".wav";
                break;
            case ExtensionTypes::UNKNOWN:
                m_filename = "UnknownFileType" + QString::number(cnt + 1) + ".bin";
                break;
        }
    }
}

void RSDKv4::Datafile::FileInfo::writeHeader(Writer &writer)
{
    m_filename     = m_filename.replace('\\', '/');
    QString fn     = m_filename;
    QByteArray md5 = calculateMD5Hash(fn.toLower());

    for (int y = 0; y < 16; y += 4) {
        m_md5Hash[y + 3] = md5[y + 0];
        m_md5Hash[y + 2] = md5[y + 1];
        m_md5Hash[y + 1] = md5[y + 2];
        m_md5Hash[y + 0] = md5[y + 3];
    }

    // again, temp
    m_encrypted = false;

    for (int i = 0; i < 0x10; ++i) writer.write((byte)m_md5Hash[i]);
    writer.write<uint>(m_dataOffset);
    writer.write<uint>(m_fileSize | (m_encrypted ? 0x80000000 : 0));
}

void RSDKv4::Datafile::FileInfo::writeData(Writer &writer)
{
    // Too lazy and I dont even fuckin think it works so
    // if (m_encrypted)
    //    writer.write(decrypt(m_filedata));
    // else
    writer.write(m_filedata);
}

QByteArray RSDKv4::Datafile::FileInfo::decrypt(QByteArray data)
{
    // Note: Since only XOr is used, this function does both,
    //       decryption and encryption.

    generateELoadKeys(m_fileSize, (m_fileSize >> 1) + 1, m_fileSize);

    const uint ENC_KEY_2 = 0x24924925;
    const uint ENC_KEY_1 = 0xAAAAAAAB;

    byte tempByt = 0;
    int key1     = 0;
    int key2     = 0;
    int temp1    = 0;
    int temp2    = 0;

    for (int i = 0; i < data.length(); ++i) {
        tempByt = m_eKeyNo ^ (byte)m_encryptionKeyB[m_eKeyPosB];
        tempByt ^= (byte)data[i];
        if (m_eNybbleSwap == 1) // swap nibbles: 0xAB <-> 0xBA
            tempByt = ((tempByt << 4) + (tempByt >> 4)) & 0xFF;
        tempByt ^= (byte)m_encryptionKeyA[m_eKeyPosA];
        data[i] = (byte)tempByt;

        ++m_eKeyPosA;
        ++m_eKeyPosB;

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
                key1          = mulUnsignedHigh(ENC_KEY_1, m_eKeyNo);
                key2          = mulUnsignedHigh(ENC_KEY_2, m_eKeyNo);
                m_eNybbleSwap = 0;

                temp1 = key2 + (m_eKeyNo - key2) / 2;
                temp2 = key1 / 8 * 3;

                m_eKeyPosA = m_eKeyNo - temp1 / 4 * 7;
                m_eKeyPosB = m_eKeyNo - temp2 * 4 + 2;
            }
            else {
                key1          = mulUnsignedHigh(ENC_KEY_1, m_eKeyNo);
                key2          = mulUnsignedHigh(ENC_KEY_2, m_eKeyNo);
                m_eNybbleSwap = 1;

                temp1 = key2 + (m_eKeyNo - key2) / 2;
                temp2 = key1 / 8 * 3;

                m_eKeyPosB = m_eKeyNo - temp1 / 4 * 7;
                m_eKeyPosA = m_eKeyNo - temp2 * 4 + 3;
            }
        }
    }
    return data;
}

byte RSDKv4::Datafile::FileInfo::getExtensionFromData()
{
    byte header[5];

    for (int i = 0; i < 5; ++i) header[i] = m_filedata[i];

    if (header[0] == (byte)'O' && header[1] == (byte)'g' && header[2] == (byte)'g'
        && header[3] == (byte)'S')
        return ExtensionTypes::OGG;

    if (header[0] == (byte)'G' && header[1] == (byte)'I' && header[2] == (byte)'F')
        return ExtensionTypes::GIF;

    if (header[0] == (byte)'R' && header[1] == (byte)'3' && header[2] == (byte)'D'
        && header[3] == (byte)'\0')
        return ExtensionTypes::R3D;

    if (header[1] == (byte)'P' && header[2] == (byte)'N' && header[3] == (byte)'G')
        return ExtensionTypes::PNG;

    if (header[0] == (byte)'R' && header[1] == (byte)'I' && header[2] == (byte)'F'
        && header[3] == (byte)'F')
        return ExtensionTypes::WAV;

    return ExtensionTypes::UNKNOWN;
}
