#include "include.hpp"

void RSDKv3::Animation::read(Reader &reader)
{
    m_filename = reader.filepath;

    int sheetCount = reader.read<byte>();
    for (int s = 0; s < sheetCount; ++s) m_sheets.append(reader.readString());

    int animCount = reader.read<byte>();
    for (int a = 0; a < animCount; ++a) m_animations.append(AnimationEntry(reader));

    int collisionBoxCount = reader.read<byte>();
    for (int h = 0; h < collisionBoxCount; ++h) m_hitboxes.append(Hitbox(reader));
}

void RSDKv3::Animation::write(Writer &writer)
{
    m_filename = writer.filePath;

    writer.write((byte)m_sheets.count());
    for (int s = 0; s < m_sheets.count(); ++s) writer.write(m_sheets[s]);

    writer.write((byte)m_animations.count());
    for (int a = 0; a < m_animations.count(); ++a) m_animations[a].write(writer);

    writer.write((byte)m_hitboxes.count());
    for (int h = 0; h < m_hitboxes.count(); ++h) m_hitboxes[h].write(writer);

    writer.flush();
}
