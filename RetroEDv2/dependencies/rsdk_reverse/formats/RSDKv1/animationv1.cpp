#include "include.hpp"

void RSDKv1::Animation::read(Reader &reader, bool dcVer)
{
    m_filename = reader.m_filepath;

    m_unknown    = reader.read<byte>();
    m_playerType = reader.read<byte>();

    int sheetCount = dcVer ? 2 : 3;
    int animCount  = reader.read<byte>();

    for (int s = 0; s < sheetCount; ++s) m_sheets.append(reader.readString());

    for (int a = 0; a < animCount; ++a) m_animations.append(AnimationEntry(reader));
}

void RSDKv1::Animation::write(Writer &writer, bool dcVer)
{
    m_filename = writer.m_filename;

    writer.write(m_unknown);
    writer.write(m_playerType);

    writer.write((byte)m_animations.count());

    for (int s = 0; s < (dcVer ? 2 : 3); ++s) writer.write(m_sheets[s]);

    for (int a = 0; a < m_animations.count(); ++a) m_animations[a].write(writer);

    writer.flush();
}
