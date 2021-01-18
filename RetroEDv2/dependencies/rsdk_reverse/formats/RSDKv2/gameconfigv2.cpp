#include "include.hpp"

void RSDKv2::Gameconfig::read(Reader &reader)
{
    m_filename = reader.m_filepath;

    // Game Text
    m_gameWindowText      = reader.readString();
    m_unknown             = reader.readString();
    m_gameDescriptionText = reader.readString();

    // Global Scripts
    byte scrCount = reader.read<byte>();
    m_scripts.clear();
    for (int s = 0; s < scrCount; ++s) m_scripts.append(reader.readString());

    // Global Variables
    byte varCount = reader.read<byte>();
    m_globalVariables.clear();
    for (int v = 0; v < varCount; ++v) m_globalVariables.append(GlobalVariable(reader));

    // Global SFX
    byte sfxCount = reader.read<byte>();
    m_soundFX.clear();
    for (int s = 0; s < sfxCount; ++s) m_soundFX.append(reader.readString());

    // Players
    byte plrCount = reader.read<byte>();
    m_players.clear();
    for (int p = 0; p < plrCount; ++p) m_players.append(PlayerInfo(reader));

    // Categories
    m_categories.clear();
    for (int c = 0; c < 4; ++c) m_categories.append(Category(reader));
}

void RSDKv2::Gameconfig::write(Writer &writer)
{
    m_filename = writer.m_filename;

    writer.write(m_gameWindowText);
    writer.write(m_unknown);
    writer.write(m_gameDescriptionText);

    // Objects
    writer.write((byte)m_scripts.count());
    for (QString &obj : m_scripts) writer.write(obj);

    // Global Variables
    writer.write((byte)m_globalVariables.count());
    for (GlobalVariable &var : m_globalVariables) var.write(writer);

    // Global SFX
    writer.write((byte)m_soundFX.count());
    for (QString &sfx : m_soundFX) writer.write(sfx);

    // Global Variables
    writer.write((byte)m_players.count());
    for (PlayerInfo &plr : m_players) plr.write(writer);

    // Categories
    for (int c = 0; c < 4; ++c) m_categories[c].write(writer);

    writer.flush();
}
