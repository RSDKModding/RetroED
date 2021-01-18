#include "include.hpp"

void RSDKv5::StageConfig::read(Reader &reader)
{
    m_filename = reader.m_filepath;

    if (!reader.matchesSignature(m_signature, 4))
        return;

    m_loadGlobalObjects = reader.read<bool>();

    byte objCnt = reader.read<byte>();
    m_objects.clear();
    for (int i = 0; i < objCnt; ++i) m_objects.append(reader.readString());

    for (int i = 0; i < 8; ++i) m_palettes[i].read(reader);

    byte sfxCnt = reader.read<byte>();
    m_sfx.clear();
    for (int i = 0; i < sfxCnt; ++i) m_sfx.append(WAVConfiguration(reader));
}

void RSDKv5::StageConfig::write(Writer &writer)
{
    m_filename = writer.m_filename;
    writer.write(m_signature, 4);

    writer.write(m_loadGlobalObjects);

    writer.write((byte)m_objects.count());
    for (int i = 0; i < (byte)m_objects.count(); ++i) writer.write(m_objects[i]);

    for (int i = 0; i < 8; ++i) m_palettes[i].write(writer);

    writer.write((byte)m_sfx.count());
    for (int i = 0; i < (byte)m_sfx.count(); ++i) m_sfx[i].write(writer);

    writer.flush();
}
