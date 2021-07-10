#include "include.hpp"

void RSDKv4::Gameconfig::read(Reader &reader)
{
    m_filename = reader.filepath;

    // Game Text
    m_gameWindowText      = reader.readString();
    m_gameDescriptionText = reader.readString();

    // Master Palettes
    m_masterPalette.read(reader, 6 * 16);

    // Objects
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

    // Global Variables
    byte varCount = reader.read<byte>();
    globalVariables.clear();

    for (int v = 0; v < varCount; ++v) {
        globalVariables.append(GlobalVariable(reader));
    }

    // Global SFX
    byte sfxCount = reader.read<byte>();
    soundFX.clear();

    for (int s = 0; s < sfxCount; ++s) {
        SoundInfo sfx;
        sfx.m_name = reader.readString();
        soundFX.append(sfx);
    }

    for (SoundInfo &sfx : soundFX) {
        sfx.m_path = reader.readString();
    }

    // Players
    byte plrCount = reader.read<byte>();
    m_players.clear();

    for (int p = 0; p < plrCount; ++p) {
        m_players.append(reader.readString());
    }

    // Categories
    categories.clear();
    for (int c = 0; c < 4; ++c) {
        categories.append(Category(reader));
    }
}

void RSDKv4::Gameconfig::write(Writer &writer)
{
    m_filename = writer.filePath;

    writer.write(m_gameWindowText);
    writer.write(m_gameDescriptionText);

    // Master Palette
    m_masterPalette.write(writer);

    // Objects
    writer.write((byte)objects.count());
    for (ObjectInfo &obj : objects) {
        writer.write(obj.name);
    }

    for (ObjectInfo &obj : objects) {
        writer.write(obj.script);
    }

    // Global Variables
    writer.write((byte)globalVariables.count());
    for (GlobalVariable &var : globalVariables) {
        var.write(writer);
    }

    // Global SFX
    writer.write((byte)soundFX.count());
    for (SoundInfo &sfx : soundFX) {
        writer.write(sfx.m_name);
    }

    for (SoundInfo &sfx : soundFX) {
        writer.write(sfx.m_path);
    }

    // Global Variables
    writer.write((byte)m_players.count());
    for (QString &plr : m_players) {
        writer.write(plr);
    }

    // Categories
    for (int c = 0; c < 4; ++c) {
        categories[c].write(writer);
    }

    writer.flush();
}
