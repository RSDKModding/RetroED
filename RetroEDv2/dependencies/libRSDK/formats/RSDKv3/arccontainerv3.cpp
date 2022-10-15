#include "libRSDK.hpp"

int decryptByte(int &key)
{
    int val = 48271 * (key % 44488) - 3399 * (key / 44488);
    if (val <= 0)
        key = val + 0x7FFFFFFF;
    else
        key = 48271 * (key % 44488) - 3399 * (key / 44488);
    return key;
}

void RSDKv3::ArcContainer::read(Reader &reader)
{
    filePath = reader.filePath;

    if (!reader.matchesSignature(signature, 4))
        return;

    ushort filecount = reader.read<ushort>();
    key              = filecount;

    files.clear();
    for (int f = 0; f < filecount; ++f) {
        FileInfo info = FileInfo();
        Utils::intBytes offset;
        Utils::intBytes fsize;
        for (int i = 0; i < 0x28; ++i) {
            byte buffer        = reader.read<byte>();
            byte decryptedByte = (byte)((decryptByte(key) % 0xFF) ^ buffer);
            if (i < 0x20) {
                if (decryptedByte != 0)
                    info.fileName += (char)decryptedByte;
            }
            else if (i >= 0x20 && i < 0x24)
                offset.bytes[i - 0x20] = decryptedByte;
            else
                fsize.bytes[i - 0x24] = decryptedByte;
        }
        info.offset   = offset.val;
        info.fileSize = fsize.val;
        long readPos  = reader.tell();
        reader.seek(info.offset);
        info.fileData = reader.readByteArray(info.fileSize);
        reader.seek(readPos);
        files.append(info);
    }
}

void RSDKv3::ArcContainer::write(Writer &writer)
{
    filePath = writer.filePath;

    writer.write(signature, 4);

    writer.write((ushort)files.count());
    key = (ushort)files.count();

    for (int f = 0; f < (ushort)files.count(); ++f) {
        FileInfo &file = files[f];
        for (int i = 0; i < 0x20; ++i)
            writer.write((byte)(i >= file.fileName.length() ? 0 : file.fileName[i].toLatin1()));
        writer.write(0x00); // offset
        writer.write(file.fileData.count());
    }

    for (int f = 0; f < (ushort)files.count(); ++f) {
        FileInfo &file = files[f];
        file.offset    = writer.tell();
        file.fileSize  = file.fileData.count();
        writer.write(file.fileData);
    }

    writer.seek(0);
    writer.write(signature, 4);
    writer.write((ushort)files.count());
    key = (ushort)files.count();

    for (int f = 0; f < (ushort)files.count(); ++f) {
        FileInfo &file = files[f];
        Utils::intBytes offset(file.offset);
        Utils::intBytes fsize(file.fileSize);
        for (int i = 0; i < 0x28; ++i) {
            byte buffer = 0;
            if (i < 0x20)
                buffer = (byte)(i >= file.fileName.length() ? 0 : file.fileName[i].toLatin1());
            else if (i >= 0x20 && i < 0x24)
                buffer = offset.bytes[i - 32];
            else
                buffer = fsize.bytes[i - 36];
            writer.write((byte)(decryptByte(key) % 0xFF ^ buffer));
        }
    }

    for (int f = 0; f < (ushort)files.count(); ++f) writer.write(files[f].fileData);

    writer.flush();
}
