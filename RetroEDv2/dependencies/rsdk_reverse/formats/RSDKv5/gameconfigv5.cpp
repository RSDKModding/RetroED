#include "include.hpp"

void RSDKv5::GameConfig::read(Reader &reader, bool oldVer)
{
    filePath = reader.filepath;

    if (!reader.matchesSignature(signature, 4))
        return;

    gameTitle    = reader.readString();
    gameSubtitle = reader.readString();
    version      = reader.readString();

    readMode = !oldVer;

    startSceneCategoryIndex = reader.read<byte>();
    startSceneIndex         = reader.read<ushort>();

    byte objCnt = reader.read<byte>();
    objects.clear();
    for (int i = 0; i < objCnt; ++i) objects.append(reader.readString());

    for (int i = 0; i < 8; ++i) palettes[i].read(reader);

    byte sfxCnt = reader.read<byte>();
    soundFX.clear();
    for (int i = 0; i < sfxCnt; ++i) soundFX.append(SoundInfo(reader));

    ushort totalScenes = reader.read<ushort>();
    Q_UNUSED(totalScenes);
    byte categoryCount = reader.read<byte>();

    categories.clear();
    for (int i = 0; i < categoryCount; ++i) categories.append(Category(reader, readMode));

    byte varCount = reader.read<byte>();
    globalVariables.clear();
    for (int i = 0; i < varCount; ++i) globalVariables.append(GlobalVariable(reader));
}

void RSDKv5::GameConfig::write(Writer &writer)
{
    filePath = writer.filePath;
    writer.write(signature, 4);

    writer.write(gameTitle);
    writer.write(gameSubtitle);
    writer.write(version);

    writer.write(startSceneCategoryIndex);
    writer.write(startSceneIndex);

    writer.write((byte)objects.count());
    for (int i = 0; i < (byte)objects.count(); ++i) writer.write(objects[i]);

    for (int i = 0; i < 8; ++i) palettes[i].write(writer, true);

    writer.write((byte)soundFX.count());
    for (int i = 0; i < (byte)soundFX.count(); ++i) soundFX[i].write(writer);

    ushort totScn = 0;
    for (Category &cat : categories) totScn += cat.scenes.count();
    writer.write(totScn);

    writer.write((byte)categories.count());
    for (Category &cat : categories) cat.write(writer, readMode);

    writer.write((byte)globalVariables.count());
    for (GlobalVariable &c : globalVariables) c.write(writer);

    writer.flush();
}
