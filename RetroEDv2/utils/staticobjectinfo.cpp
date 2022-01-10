#include "includes.hpp"

void StaticObjectInfo::read(Reader &reader)
{
    m_filename = reader.filePath;
    byte fVer  = reader.read<byte>();
    Q_UNUSED(fVer);
    m_name = reader.readString();

    byte arrayCnt = reader.read<byte>();
    values.clear();
    for (int a = 0; a < arrayCnt; ++a) values.append(ArrayInfo(reader));
}

void StaticObjectInfo::write(Writer &writer)
{
    m_filename = writer.filePath;
    writer.write(m_fileVer);
    writer.write(m_name);

    writer.write((byte)values.count());
    for (auto &a : values) a.write(writer);

    writer.flush();
}
