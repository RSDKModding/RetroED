#include "libRSDK.hpp"

#include "stageconfigv2.hpp"

void RSDKv2::StageConfig::read(Reader &reader)
{
    filePath = reader.filePath;

    // Global Objects Flag
    loadGlobalScripts = reader.read<byte>();

    // Stage Palette
    palette.read(reader, 2 * 16);

    // Stage Scripts
    byte scrCount = reader.read<byte>();
    objects.clear();

    for (int s = 0; s < scrCount; ++s) {
        objects.append(reader.readString());
    }

    // Stage SoundFX
    byte sfxCount = reader.read<byte>();
    soundFX.clear();

    for (int s = 0; s < sfxCount; ++s) {
        soundFX.append(reader.readString());
    }
}

void RSDKv2::StageConfig::write(Writer &writer)
{
    filePath = writer.filePath;

    // Global Objects Flag
    writer.write(loadGlobalScripts);

    // Stage Palette
    palette.write(writer);

    // Stage Scripts
    writer.write((byte)objects.count());
    for (QString &scr : objects) {
        writer.write(scr);
    }

    // Stage SoundFX
    writer.write((byte)soundFX.count());
    for (QString &sfx : soundFX) {
        writer.write(sfx);
    }

    writer.flush();
}
