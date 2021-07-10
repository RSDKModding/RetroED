#include "includes.hpp"

void StaticObjectInfo::read(Reader &reader)
{
    m_filename = reader.filepath;
    byte fVer  = reader.read<byte>();
    Q_UNUSED(fVer);
    m_name = reader.readString();

    byte arrayCnt = reader.read<byte>();
    m_arrays.clear();
    for (int a = 0; a < arrayCnt; ++a) m_arrays.append(ArrayInfo(reader));
}

void StaticObjectInfo::write(Writer &writer)
{
    m_filename = writer.filePath;
    writer.write(m_fileVer);
    writer.write(m_name);

    writer.write((byte)m_arrays.count());
    for (auto &a : m_arrays) a.write(writer);

    writer.flush();
}
