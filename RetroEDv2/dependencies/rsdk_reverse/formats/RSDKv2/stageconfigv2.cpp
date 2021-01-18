#include "include.hpp"

void RSDKv2::Stageconfig::read(Reader &reader)
{
    m_filename = reader.m_filepath;

    // Global Objects Flag
    m_loadGlobalScripts = reader.read<byte>();

    // Stage Palette
    m_stagePalette.read(reader, 2 * 16);

    // Stage Scripts
    byte scrCount = reader.read<byte>();
    m_scripts.clear();

    for (int s = 0; s < scrCount; ++s) {
        m_scripts.append(reader.readString());
    }

    // Stage SoundFX
    byte sfxCount = reader.read<byte>();
    m_soundFX.clear();

    for (int s = 0; s < sfxCount; ++s) {
        m_soundFX.append(reader.readString());
    }
}

void RSDKv2::Stageconfig::write(Writer &writer)
{
    m_filename = writer.m_filename;

    // Global Objects Flag
    writer.write(m_loadGlobalScripts);

    // Stage Palette
    m_stagePalette.write(writer);

    // Stage Scripts
    writer.write((byte)m_scripts.count());
    for (QString &scr : m_scripts) {
        writer.write(scr);
    }

    // Stage SoundFX
    writer.write((byte)m_soundFX.count());
    for (QString &sfx : m_soundFX) {
        writer.write(sfx);
    }

    writer.flush();
}
