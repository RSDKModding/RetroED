#include "include.hpp"

void RSDKv5::Animation::read(Reader &reader)
{
    m_filename = reader.filepath;

    if (!reader.matchesSignature(m_signature, 4)) {
        return;
    }

    uint totalFrameCount = reader.read<uint>();
    Q_UNUSED(totalFrameCount);

    byte sheetCount = reader.read<byte>();
    for (int s = 0; s < sheetCount; ++s) m_sheets.append(reader.readString());

    byte hitboxCount = reader.read<byte>();
    for (int h = 0; h < hitboxCount; ++h) m_hitboxTypes.append(reader.readString());

    ushort animCount = reader.read<ushort>();
    for (int a = 0; a < animCount; ++a) m_animations.append(AnimationEntry(reader, this));
}

void RSDKv5::Animation::write(Writer &writer)
{
    m_filename = writer.filePath;

    writer.write(m_signature, 4);

    uint totalFrameCount = 0;
    for (auto &a : m_animations) totalFrameCount += a.m_frames.count();
    writer.write(totalFrameCount);

    writer.write((byte)m_sheets.count());
    for (int s = 0; s < m_sheets.count(); ++s) writer.write(m_sheets[s] + '\0');

    writer.write((byte)m_hitboxTypes.count());
    for (int h = 0; h < m_hitboxTypes.count(); ++h) writer.write(m_hitboxTypes[h] + '\0');

    writer.write((ushort)m_animations.count());
    for (int a = 0; a < m_animations.count(); ++a) m_animations[a].write(writer, this);

    writer.flush();
}
