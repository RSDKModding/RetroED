#include "include.hpp"

void RSDKv2::Animation::read(Reader &reader)
{
    m_filename = reader.m_filepath;

    for (int u = 0; u < 5; ++u) m_unknown[u] = reader.read<byte>();

    for (int s = 0; s < 3; ++s) m_sheets.append(reader.readString());

    m_unknownVal = reader.read<byte>();

    int animCount = reader.read<byte>();
    for (int a = 0; a < animCount; ++a) m_animations.append(AnimationEntry(reader));

    int collisionBoxCount = reader.read<byte>();
    for (int h = 0; h < collisionBoxCount; ++h) m_hitboxes.append(Hitbox(reader));
}

void RSDKv2::Animation::write(Writer &writer)
{
    m_filename = writer.m_filename;

    for (int u = 0; u < 5; ++u) {
        writer.write(m_unknown[u]);
    }

    for (int s = 0; s < 3; ++s) {
        writer.write(m_sheets[s]);
    }

    writer.write(m_unknownVal);

    writer.write((byte)m_animations.count());
    for (int a = 0; a < m_animations.count(); ++a) {
        m_animations[a].write(writer);
    }

    writer.write((byte)m_hitboxes.count());
    for (int h = 0; h < m_hitboxes.count(); ++h) {
        m_hitboxes[h].write(writer);
    }

    writer.flush();
}
