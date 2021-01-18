#include "include.hpp"

void RSDKv4::Stageconfig::read(Reader &reader)
{
    m_filename = reader.m_filepath;

    // Global Objects Flag
    m_loadGlobalScripts = reader.read<byte>();

    // Stage Palette
    m_stagePalette.read(reader, 2 * 16);

    // Stage SoundFX
    byte sfxCount = reader.read<byte>();
    m_soundFX.clear();

    for (int s = 0; s < sfxCount; ++s) {
        SoundInfo obj;
        obj.m_name = reader.readString();
        m_soundFX.append(obj);
    }

    for (SoundInfo &sfx : m_soundFX) {
        sfx.m_path = reader.readString();
    }

    // Stage Objects
    byte objectCount = reader.read<byte>();
    m_objects.clear();

    for (int o = 0; o < objectCount; ++o) {
        ObjectInfo obj;
        obj.m_name = reader.readString();
        m_objects.append(obj);
    }

    for (ObjectInfo &obj : m_objects) {
        obj.m_script = reader.readString();
    }
}

void RSDKv4::Stageconfig::write(Writer &writer)
{
    m_filename = writer.m_filename;

    // Global Objects Flag
    writer.write(m_loadGlobalScripts);

    // Stage Palette
    m_stagePalette.write(writer);

    // Stage SoundFX
    writer.write((byte)m_soundFX.count());
    for (SoundInfo &sfx : m_soundFX) {
        writer.write(sfx.m_name);
    }

    for (SoundInfo &sfx : m_soundFX) {
        writer.write(sfx.m_path);
    }

    // Stage Objects
    writer.write((byte)m_objects.count());
    for (ObjectInfo &obj : m_objects) {
        writer.write(obj.m_name);
    }

    for (ObjectInfo &obj : m_objects) {
        writer.write(obj.m_script);
    }

    writer.flush();
}
