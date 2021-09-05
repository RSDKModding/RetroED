#include "include.hpp"

void RSDKv3::StageConfig::read(Reader &reader)
{
    m_filename = reader.filepath;

    // Global Objects Flag
    loadGlobalScripts = reader.read<byte>();

    // Stage Palette
    palette.read(reader, 2 * 16);

    // Stage Objects
    byte objectCount = reader.read<byte>();
    objects.clear();

    for (int o = 0; o < objectCount; ++o) {
        ObjectInfo obj;
        obj.name = reader.readString();
        objects.append(obj);
    }

    for (ObjectInfo &obj : objects) {
        obj.script = reader.readString();
    }

    // Stage SoundFX
    byte sfxCount = reader.read<byte>();
    soundFX.clear();

    for (int s = 0; s < sfxCount; ++s) {
        soundFX.append(reader.readString());
    }
}

void RSDKv3::StageConfig::write(Writer &writer)
{
    m_filename = writer.filePath;

    // Global Objects Flag
    writer.write(loadGlobalScripts);

    // Stage Palette
    palette.write(writer);

    // Stage Objects
    writer.write((byte)objects.count());
    for (ObjectInfo &obj : objects) {
        writer.write(obj.name);
    }

    for (ObjectInfo &obj : objects) {
        writer.write(obj.script);
    }

    // Stage SoundFX
    writer.write((byte)soundFX.count());
    for (QString &sfx : soundFX) {
        writer.write(sfx);
    }

    writer.flush();
}
