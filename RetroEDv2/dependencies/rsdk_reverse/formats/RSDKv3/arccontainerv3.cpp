#include "include.hpp"

int decryptARCFileInfo(int &key)
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
    m_filename = reader.m_filepath;

    if (!reader.matchesSignature(m_signature, 4))
        return;

    ushort filecount = reader.read<ushort>();
    m_arcKey         = filecount;

    m_files.clear();
    for (int f = 0; f < filecount; ++f) {
        FileInfo file = FileInfo();
        Utils::intBytes offset;
        Utils::intBytes fsize;
        for (int i = 0; i < 0x28; ++i) {
            byte buffer        = reader.read<byte>();
            byte decryptedByte = (byte)((decryptARCFileInfo(m_arcKey) % 0xFF) ^ buffer);
            if (i < 0x20) {
                if (decryptedByte != 0)
                    file.m_name += (char)decryptedByte;
            }
            else if (i >= 0x20 && i < 0x24)
                offset.m_bytes[i - 0x20] = decryptedByte;
            else
                fsize.m_bytes[i - 0x24] = decryptedByte;
        }
        file.m_offset   = offset.m_val;
        file.m_fileSize = fsize.m_val;
        long readPos    = reader.tell();
        reader.seek(file.m_offset);
        file.m_fileData = reader.readByteArray(file.m_fileSize);
        reader.seek(readPos);
        m_files.append(file);
    }
}

void RSDKv3::ArcContainer::write(Writer &writer)
{
    m_filename = writer.m_filename;

    writer.write(m_signature, 4);

    writer.write((ushort)m_files.count());
    m_arcKey = (ushort)m_files.count();

    for (int f = 0; f < (ushort)m_files.count(); ++f) {
        FileInfo &file = m_files[f];
        for (int i = 0; i < 0x20; ++i)
            writer.write((byte)(i >= file.m_name.length() ? 0 : file.m_name[i].toLatin1()));
        writer.write(0x00); // offset
        writer.write(file.m_fileData.count());
    }

    for (int f = 0; f < (ushort)m_files.count(); ++f) {
        FileInfo &file  = m_files[f];
        file.m_offset   = writer.tell();
        file.m_fileSize = file.m_fileData.count();
        writer.write(file.m_fileData);
    }

    writer.seek(0);
    writer.write(m_signature, 4);
    writer.write((ushort)m_files.count());
    m_arcKey = (ushort)m_files.count();

    for (int f = 0; f < (ushort)m_files.count(); ++f) {
        FileInfo &file = m_files[f];
        Utils::intBytes offset(file.m_offset);
        Utils::intBytes fsize(file.m_fileSize);
        for (int i = 0; i < 0x28; ++i) {
            byte buffer = 0;
            if (i < 0x20)
                buffer = (byte)(i >= file.m_name.length() ? 0 : file.m_name[i].toLatin1());
            else if (i >= 0x20 && i < 0x24)
                buffer = offset.m_bytes[i - 32];
            else
                buffer = fsize.m_bytes[i - 32];
            writer.write((byte)(decryptARCFileInfo(m_arcKey) % 0xFF ^ buffer));
        }
    }

    for (int f = 0; f < (ushort)m_files.count(); ++f) writer.write(m_files[f].m_fileData);

    writer.flush();
}
