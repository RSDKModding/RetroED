#include "include.hpp"

void RSDKv4::Stageconfig::read(Reader &reader)
{
    m_filename = reader.filepath;

    // Global Objects Flag
    loadGlobalScripts = reader.read<byte>();

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
        obj.name = reader.readString();
        m_objects.append(obj);
    }

    for (ObjectInfo &obj : m_objects) {
        obj.script = reader.readString();
    }
}

void RSDKv4::Stageconfig::write(Writer &writer)
{
    m_filename = writer.filePath;

    // Global Objects Flag
    writer.write(loadGlobalScripts);

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
        writer.write(obj.name);
    }

    for (ObjectInfo &obj : m_objects) {
        writer.write(obj.script);
    }

    writer.flush();
}
