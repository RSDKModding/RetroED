#include "include.hpp"

void RSDKv3::Stageconfig::read(Reader &reader)
{
    m_filename = reader.filepath;

    // Global Objects Flag
    loadGlobalScripts = reader.read<byte>();

    // Stage Palette
    m_stagePalette.read(reader, 2 * 16);

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

    // Stage SoundFX
    byte sfxCount = reader.read<byte>();
    soundFX.clear();

    for (int s = 0; s < sfxCount; ++s) {
        soundFX.append(reader.readString());
    }
}

void RSDKv3::Stageconfig::write(Writer &writer)
{
    m_filename = writer.filePath;

    // Global Objects Flag
    writer.write(loadGlobalScripts);

    // Stage Palette
    m_stagePalette.write(writer);

    // Stage Objects
    writer.write((byte)m_objects.count());
    for (ObjectInfo &obj : m_objects) {
        writer.write(obj.name);
    }

    for (ObjectInfo &obj : m_objects) {
        writer.write(obj.script);
    }

    // Stage SoundFX
    writer.write((byte)soundFX.count());
    for (QString &sfx : soundFX) {
        writer.write(sfx);
    }

    writer.flush();
}
