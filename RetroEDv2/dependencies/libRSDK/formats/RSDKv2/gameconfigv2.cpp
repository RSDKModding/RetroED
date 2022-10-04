#include "rsdkreverse.hpp"

void RSDKv2::GameConfig::read(Reader &reader)
{
    filePath = reader.filePath;

    // Game Text
    gameWindowText      = reader.readString();
    unknown             = reader.readString();
    gameDescriptionText = reader.readString();

    // Global Scripts
    byte scrCount = reader.read<byte>();
    objects.clear();
    for (int s = 0; s < scrCount; ++s) objects.append(reader.readString());

    // Global Variables
    byte varCount = reader.read<byte>();
    globalVariables.clear();
    for (int v = 0; v < varCount; ++v) globalVariables.append(GlobalVariable(reader));

    // Global SFX
    byte sfxCount = reader.read<byte>();
    soundFX.clear();
    for (int s = 0; s < sfxCount; ++s) soundFX.append(reader.readString());

    // Players
    byte plrCount = reader.read<byte>();
    players.clear();
    for (int p = 0; p < plrCount; ++p) players.append(PlayerInfo(reader));

    // Categories
    stageLists.clear();
    for (int c = 0; c < 4; ++c) stageLists.append(Category(reader));
}

void RSDKv2::GameConfig::write(Writer &writer)
{
    filePath = writer.filePath;

    writer.write(gameWindowText);
    writer.write(unknown);
    writer.write(gameDescriptionText);

    // Objects
    writer.write((byte)objects.count());
    for (QString &obj : objects) writer.write(obj);

    // Global Variables
    writer.write((byte)globalVariables.count());
    for (GlobalVariable &var : globalVariables) var.write(writer);

    // Global SFX
    writer.write((byte)soundFX.count());
    for (QString &sfx : soundFX) writer.write(sfx);

    // Global Variables
    writer.write((byte)players.count());
    for (PlayerInfo &plr : players) plr.write(writer);

    // Categories
    for (int c = 0; c < 4; ++c) stageLists[c].write(writer);

    writer.flush();
}
