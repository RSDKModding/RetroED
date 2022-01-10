#include "rsdkreverse.hpp"

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
        hash[y + 3] = reader.read<byte>();
        hash[y + 2] = reader.read<byte>();
        hash[y + 1] = reader.read<byte>();
        hash[y]     = reader.read<byte>();
    }
    filenameHash = QByteArray((const char *)hash, 0x10).toHex();

    fileName = "File " + QString::number(cnt + 1) + ".bin"; // Make a base name

    for (int i = 0; i < fileList.count(); ++i) {
        // RSDKv5 hashes all strings as lower case
        QString fp = fileList[i];
        fp         = fp.toLower();

        bool match = true;

        for (int z = 0; z < 16; ++z) {
            match = (byte)calculateMD5Hash(fp)[z] == hash[z];
            if (!match)
                break;
        }

        if (match) {
            fileName = fileList[i];
            break;
        }
    }

    fileOffset = reader.read<uint>();
    uint buf   = reader.read<uint>();

    encrypted = (buf & 0x80000000) != 0;
    fileSize  = (buf & 0x7FFFFFFF);

    long tmp = reader.tell();
    reader.seek(fileOffset);

    fileData = reader.readByteArray(fileSize);

    // Decrypt File if Encrypted
    if (encrypted)
        fileData = decrypt(fileData, false);

    reader.seek(tmp);
}

void RSDKv5::Datafile::FileInfo::writeHeader(Writer &writer)
{
    fileName = fileName.replace('\\', '/');

    QString fn     = fileName;
    QByteArray md5 = calculateMD5Hash(fn.toLower());

    for (int y = 0; y < 16; y += 4) {
        hash[y + 3] = md5[y + 0];
        hash[y + 2] = md5[y + 1];
        hash[y + 1] = md5[y + 2];
        hash[y + 0] = md5[y + 3];
    }

    writer.write(hash, 0x10);
    writer.write(fileOffset);
    encrypted = false; // temp
    writer.write(fileSize | (encrypted ? 0x80000000 : 0));
}

void RSDKv5::Datafile::FileInfo::writeData(Writer &writer)
{
    if (encrypted)
        writer.write(decrypt(fileData, true));
    else
        writer.write(fileData);
}

void RSDKv5::Datafile::FileInfo::generateELoadKeys(QString filename, uint size)
{
    QByteArray md5Buf;

    QString filenameCaps = filename.toUpper();
    md5Buf               = calculateMD5Hash(filenameCaps);
    encryptionKeyA.resize(0x10);
    encryptionKeyB.resize(0x10);

    for (int y = 0; y < 16; y += 4) {
        // convert every 32-bit word to Little Endian
        encryptionKeyA[y + 3] = (byte)md5Buf[y + 0];
        encryptionKeyA[y + 2] = (byte)md5Buf[y + 1];
        encryptionKeyA[y + 1] = (byte)md5Buf[y + 2];
        encryptionKeyA[y + 0] = (byte)md5Buf[y + 3];
    }

    QString fsize = QString::number(size);
    md5Buf        = calculateMD5Hash(fsize);

    for (int y = 0; y < 16; y += 4) {
        // convert every 32-bit word to Little Endian
        encryptionKeyB[y + 3] = (byte)md5Buf[y + 0];
        encryptionKeyB[y + 2] = (byte)md5Buf[y + 1];
        encryptionKeyB[y + 1] = (byte)md5Buf[y + 2];
        encryptionKeyB[y + 0] = (byte)md5Buf[y + 3];
    }

    eKeyNo      = (int)(size / 4) & 0x7F;
    eKeyPosA    = 0;
    eKeyPosB    = 8;
    eNybbleSwap = 0;
}

QByteArray RSDKv5::Datafile::FileInfo::decrypt(QByteArray data, bool encrypting)
{
    generateELoadKeys(fileName, fileSize);

    for (int i = 0; i < data.count(); ++i) {
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
                eNybbleSwap = 0;

                eKeyPosA = eKeyNo % 7;
                eKeyPosB = (eKeyNo % 0xC) + 2;
            }
            else {
                eNybbleSwap = 1;

                eKeyPosA = (eKeyNo % 0xC) + 3;
                eKeyPosB = eKeyNo % 7;
            }
        }
    }

    return data;
}

void RSDKv5::Datafile::read(Reader &reader, QList<QString> fileList)
{
    filePath = reader.filePath;

    if (QByteArray((const char *)signature, 6) != reader.readByteArray(6))
        return;

    files.clear();
    int fileCount = reader.read<ushort>(); // read the header data
    for (int i = 0; i < fileCount; ++i)
        files.append(FileInfo(reader, fileList, i)); // read each file's header
}

void RSDKv5::Datafile::write(Writer &writer)
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
