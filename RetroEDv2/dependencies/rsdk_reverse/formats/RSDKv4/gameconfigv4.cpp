#include "include.hpp"

void RSDKv4::Gameconfig::read(Reader &reader)
{
    m_filename = reader.m_filepath;

    // Game Text
    m_gameWindowText      = reader.readString();
    m_gameDescriptionText = reader.readString();

    // Master Palettes
    m_masterPalette.read(reader, 6 * 16);

    // Objects
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

    // Global Variables
    byte varCount = reader.read<byte>();
    m_globalVariables.clear();

    for (int v = 0; v < varCount; ++v) {
        m_globalVariables.append(GlobalVariable(reader));
    }

    // Global SFX
    byte sfxCount = reader.read<byte>();
    m_soundFX.clear();

    for (int s = 0; s < sfxCount; ++s) {
        SoundInfo sfx;
        sfx.m_name = reader.readString();
        m_soundFX.append(sfx);
    }

    for (SoundInfo &sfx : m_soundFX) {
        sfx.m_path = reader.readString();
    }

    // Players
    byte plrCount = reader.read<byte>();
    m_players.clear();

    for (int p = 0; p < plrCount; ++p) {
        m_players.append(reader.readString());
    }

    // Categories
    m_categories.clear();
    for (int c = 0; c < 4; ++c) {
        m_categories.append(Category(reader));
    }
}

void RSDKv4::Gameconfig::write(Writer &writer)
{
    m_filename = writer.m_filename;

    writer.write(m_gameWindowText);
    writer.write(m_gameDescriptionText);

    // Master Palette
    m_masterPalette.write(writer);

    // Objects
    writer.write((byte)m_objects.count());
    for (ObjectInfo &obj : m_objects) {
        writer.write(obj.m_name);
    }

    for (ObjectInfo &obj : m_objects) {
        writer.write(obj.m_script);
    }

    // Global Variables
    writer.write((byte)m_globalVariables.count());
    for (GlobalVariable &var : m_globalVariables) {
        var.write(writer);
    }

    // Global SFX
    writer.write((byte)m_soundFX.count());
    for (SoundInfo &sfx : m_soundFX) {
        writer.write(sfx.m_name);
    }

    for (SoundInfo &sfx : m_soundFX) {
        writer.write(sfx.m_path);
    }

    // Global Variables
    writer.write((byte)m_players.count());
    for (QString &plr : m_players) {
        writer.write(plr);
    }

    // Categories
    for (int c = 0; c < 4; ++c) {
        m_categories[c].write(writer);
    }

    writer.flush();
}
