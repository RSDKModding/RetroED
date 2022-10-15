#include "libRSDK.hpp"

void RSDKv4::Datafile::read(Reader &reader, QList<QString> fileList)
{
    filePath = reader.filePath;

    if (QByteArray((const char *)signature, 6) != reader.readByteArray(6))
        return;

    files.clear();
    int fileCount = reader.read<ushort>(); // read the header data
    for (int i = 0; i < fileCount; ++i)
        files.append(FileInfo(reader, fileList, i)); // read each file's header
}

void RSDKv4::Datafile::write(Writer &writer)
{
    filePath = writer.filePath;

    for (int h = 0; h < 6; ++h) writer.write(signature[h]);

    // std::sort(files.begin(), files.end(), [](const FileInfo &a, const FileInfo &b) -> bool {
    // return a.filename < b.filename; });

    writer.write((ushort)files.count()); // write the header
    for (FileInfo &f : files) {
        f.writeHeader(writer); // write our file header data
    }

    for (FileInfo &f : files) {
        f.fileOffset = (uint)writer.tell();
        QByteArray b;
        b.resize(f.fileSize);
        writer.write(b);
    }

    writer.seek(0); // jump back to the start of the file

    for (int h = 0; h < 6; ++h) writer.write(signature[h]);
    writer.write((ushort)files.count());

    for (FileInfo &f : files) {
        f.writeHeader(writer);
        long tmp = writer.tell();
        writer.seek(f.fileOffset);
        f.writeData(writer);
        writer.seek(tmp);
    }

    writer.flush();
}

void RSDKv4::Datafile::FileInfo::read(Reader &reader, QList<QString> fileList, int cnt)
{
    for (int y = 0; y < 16; y += 4) {
        hash[y + 3] = reader.read<byte>();
        hash[y + 2] = reader.read<byte>();
        hash[y + 1] = reader.read<byte>();
        hash[y + 0] = reader.read<byte>();
    }

    filenameHash = QByteArray((const char *)hash, 0x10).toHex();
    fileName     = QString::number(cnt + 1) + ".bin"; // Make a base name

    for (int i = 0; i < fileList.count(); ++i) {
        // RSDK hashes all strings at lowercase
        QString fp = fileList[i].toLower();

        bool match = true;
        for (int z = 0; z < 16; ++z) {
            if ((byte)calculateMD5Hash(fp)[z] != (byte)hash[z]) {
                match = false;
                break;
            }
        }

        if (match) {
            fileName = fileList[i];
            break;
        }
    }

    fileOffset = reader.read<uint>();
    uint tmp   = reader.read<uint>();

    encrypted = (tmp & 0x80000000) != 0;
    fileSize  = (tmp & 0x7FFFFFFF);

    long tmp2 = reader.tell();
    reader.seek(fileOffset);

    fileData = reader.readByteArray(fileSize);

    // Decrypt File if Encrypted
    if (encrypted)
        fileData = decrypt(fileData, false);

    reader.seek(tmp2);

    extension = getExtensionFromData();
    if (fileName == QString::number(cnt + 1) + ".bin") {
        switch (extension) {
            case ExtensionTypes::GIF: fileName = "Sprite" + QString::number(cnt + 1) + ".gif"; break;
            case ExtensionTypes::R3D: fileName = "Model" + QString::number(cnt + 1) + ".bin"; break;
            case ExtensionTypes::OGG: fileName = "Music" + QString::number(cnt + 1) + ".ogg"; break;
            case ExtensionTypes::PNG: fileName = "Image" + QString::number(cnt + 1) + ".png"; break;
            case ExtensionTypes::WAV:
                fileName = "SoundEffect" + QString::number(cnt + 1) + ".wav";
                break;
            case ExtensionTypes::UNKNOWN:
                fileName = "UnknownFileType" + QString::number(cnt + 1) + ".bin";
                break;
        }
    }
}

void RSDKv4::Datafile::FileInfo::writeHeader(Writer &writer)
{
    fileName       = fileName.replace('\\', '/');
    QString fn     = fileName;
    QByteArray md5 = calculateMD5Hash(fn.toLower());

    for (int y = 0; y < 16; y += 4) {
        hash[y + 3] = md5[y + 0];
        hash[y + 2] = md5[y + 1];
        hash[y + 1] = md5[y + 2];
        hash[y + 0] = md5[y + 3];
    }

    // again, temp
    encrypted = false;

    for (int i = 0; i < 0x10; ++i) writer.write((byte)hash[i]);
    writer.write<uint>(fileOffset);
    writer.write<uint>(fileSize | (encrypted ? 0x80000000 : 0));
}

void RSDKv4::Datafile::FileInfo::writeData(Writer &writer)
{
    if (encrypted)
        writer.write(decrypt(fileData, true));
    else
        writer.write(fileData);
}

QByteArray RSDKv4::Datafile::FileInfo::decrypt(QByteArray data, bool encrypting)
{
    // Note: Since only XOr is used, this function does both,
    //       decryption and encryption.

    generateELoadKeys(fileSize, (fileSize >> 1) + 1, fileSize);

    const uint ENC_KEY_2 = 0x24924925;
    const uint ENC_KEY_1 = 0xAAAAAAAB;

    int key1  = 0;
    int key2  = 0;
    int temp1 = 0;
    int temp2 = 0;

    for (int i = 0; i < data.length(); ++i) {
        byte encByte = data[i];
        if (encrypting) {
            encByte ^= encryptionKeyA[eKeyPosA++];

            if (eNybbleSwap == 1) // swap nibbles: 0xAB <-> 0xBA
                encByte = ((encByte << 4) + (encByte >> 4)) & 0xFF;

            encByte ^= eKeyNo ^ encryptionKeyB[eKeyPosB++];
        }
        else {
            encByte ^= eKeyNo ^ encryptionKeyB[eKeyPosB++];

            if (eNybbleSwap == 1) // swap nibbles: 0xAB <-> 0xBA
                encByte = ((encByte << 4) + (encByte >> 4)) & 0xFF;

            encByte ^= encryptionKeyA[eKeyPosA++];
        }
        data[i] = encByte;

        if (eKeyPosA <= 0x0F) {
            if (eKeyPosB > 0x0C) {
                eKeyPosB = 0;
                eNybbleSwap ^= 0x01;
            }
        }
        else if (eKeyPosB <= 0x08) {
            eKeyPosA = 0;
            eNybbleSwap ^= 0x01;
        }
        else {
            eKeyNo += 2;
            eKeyNo &= 0x7F;

            if (eNybbleSwap != 0) {
                key1        = mulUnsignedHigh(ENC_KEY_1, eKeyNo);
                key2        = mulUnsignedHigh(ENC_KEY_2, eKeyNo);
                eNybbleSwap = 0;

                temp1 = key2 + (eKeyNo - key2) / 2;
                temp2 = key1 / 8 * 3;

                eKeyPosA = eKeyNo - temp1 / 4 * 7;
                eKeyPosB = eKeyNo - temp2 * 4 + 2;
            }
            else {
                key1        = mulUnsignedHigh(ENC_KEY_1, eKeyNo);
                key2        = mulUnsignedHigh(ENC_KEY_2, eKeyNo);
                eNybbleSwap = 1;

                temp1 = key2 + (eKeyNo - key2) / 2;
                temp2 = key1 / 8 * 3;

                eKeyPosB = eKeyNo - temp1 / 4 * 7;
                eKeyPosA = eKeyNo - temp2 * 4 + 3;
            }
        }
    }
    return data;
}

byte RSDKv4::Datafile::FileInfo::getExtensionFromData()
{
    byte header[5];

    for (int i = 0; i < 5; ++i) header[i] = fileData[i];

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
