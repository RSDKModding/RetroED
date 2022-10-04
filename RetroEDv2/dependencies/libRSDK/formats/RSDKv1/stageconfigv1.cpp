#include "rsdkreverse.hpp"

void RSDKv1::StageConfig::read(Reader &reader)
{
    filePath = reader.filePath;

    // Stage Palette
    palette.read(reader, 2 * 16);

    // Stage SpriteSheets
    byte sheetCount = reader.read<byte>();
    spriteSheets.clear();

    for (int o = 0; o < sheetCount; ++o) spriteSheets.append(reader.readString());

    // Stage Objects
    byte objectCount = reader.read<byte>();
    objects.clear();

    for (int o = 0; o < objectCount; ++o) {
        ObjectInfo obj;
        obj.script = reader.readString();
        objects.append(obj);
    }

    for (ObjectInfo &obj : objects) obj.sheetID = reader.read<byte>();

    // Stage SoundFX
    byte sfxCount = reader.read<byte>();
    soundFX.clear();

    for (int s = 0; s < sfxCount; ++s) soundFX.append(reader.readString());

    // Stage Music
    byte musCount = reader.read<byte>();
    music.clear();

    for (int m = 0; m < musCount; ++m) music.append(reader.readString());
}

void RSDKv1::StageConfig::write(Writer &writer)
{
    filePath = writer.filePath;

    // Stage Palette
    palette.write(writer);

    // Stage SpriteSheets
    writer.write((byte)spriteSheets.count());
    for (QString &obj : spriteSheets) writer.write(obj);

    // Stage Objects
    writer.write((byte)objects.count());
    for (ObjectInfo &obj : objects) writer.write(obj.script);

    for (ObjectInfo &obj : objects) writer.write(obj.sheetID);

    // Stage SoundFX
    writer.write((byte)soundFX.count());
    for (QString &sfx : soundFX) writer.write(sfx);

    // Stage Music
    writer.write((byte)music.count());
    for (QString &mus : music) writer.write(mus);

    writer.flush();
}
