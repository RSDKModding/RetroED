#include "include.hpp"

void RSDKv1::Stageconfig::read(Reader &reader)
{
    m_filename = reader.m_filepath;

    // Stage Palette
    m_stagePalette.read(reader, 2 * 16);

    // Stage SpriteSheets
    byte sheetCount = reader.read<byte>();
    m_objectSpritesheets.clear();

    for (int o = 0; o < sheetCount; ++o) m_objectSpritesheets.append(reader.readString());

    // Stage Objects
    byte objectCount = reader.read<byte>();
    m_objects.clear();

    for (int o = 0; o < objectCount; ++o) {
        ObjectInfo obj;
        obj.m_script = reader.readString();
        m_objects.append(obj);
    }

    for (ObjectInfo &obj : m_objects) obj.m_sheetID = reader.read<byte>();

    // Stage SoundFX
    byte sfxCount = reader.read<byte>();
    m_soundFX.clear();

    for (int s = 0; s < sfxCount; ++s) m_soundFX.append(reader.readString());

    // Stage Music
    byte musCount = reader.read<byte>();
    m_music.clear();

    for (int m = 0; m < musCount; ++m) m_music.append(reader.readString());
}

void RSDKv1::Stageconfig::write(Writer &writer)
{
    m_filename = writer.m_filename;

    // Stage Palette
    m_stagePalette.write(writer);

    // Stage SpriteSheets
    writer.write((byte)m_objectSpritesheets.count());
    for (QString &obj : m_objectSpritesheets) writer.write(obj);

    // Stage Objects
    writer.write((byte)m_objects.count());
    for (ObjectInfo &obj : m_objects) writer.write(obj.m_script);

    for (ObjectInfo &obj : m_objects) writer.write(obj.m_sheetID);

    // Stage SoundFX
    writer.write((byte)m_soundFX.count());
    for (QString &sfx : m_soundFX) writer.write(sfx);

    // Stage Music
    writer.write((byte)m_music.count());
    for (QString &mus : m_music) writer.write(mus);

    writer.flush();
}
