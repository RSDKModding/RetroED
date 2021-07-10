#include "include.hpp"

void RSDKv3::Gameconfig::read(Reader &reader)
{
    m_filename = reader.filepath;

    // Game Text
    m_gameWindowText      = reader.readString();
    m_unknown             = reader.readString();
    m_gameDescriptionText = reader.readString();

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
    globalVariables.clear();

    for (int v = 0; v < varCount; ++v) {
        globalVariables.append(GlobalVariable(reader));
    }

    // Global SFX
    byte sfxCount = reader.read<byte>();
    m_soundFX.clear();

    for (int s = 0; s < sfxCount; ++s) {
        m_soundFX.append(reader.readString());
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

void RSDKv3::Gameconfig::write(Writer &writer)
{
    m_filename = writer.filePath;

    writer.write(m_gameWindowText);
    writer.write(m_unknown);
    writer.write(m_gameDescriptionText);

    // Objects
    writer.write((byte)m_objects.count());
    for (ObjectInfo &obj : m_objects) {
        writer.write(obj.m_name);
    }

    for (ObjectInfo &obj : m_objects) {
        writer.write(obj.m_script);
    }

    // Global Variables
    writer.write((byte)globalVariables.count());
    for (GlobalVariable &var : globalVariables) {
        var.write(writer);
    }

    // Global SFX
    writer.write((byte)m_soundFX.count());
    for (QString &sfx : m_soundFX) {
        writer.write(sfx);
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
