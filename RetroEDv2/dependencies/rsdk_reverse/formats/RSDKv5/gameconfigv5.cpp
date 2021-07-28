#include "include.hpp"

bool interpretVersion(QString ver)
{
    return !(ver.contains("1.00.") || ver.contains("1.01.") || ver.contains("1.02.")
             || ver.contains("1.03."));
}

void RSDKv5::GameConfig::read(Reader &reader, bool oldVer)
{
    m_filename = reader.filepath;

    if (!reader.matchesSignature(m_signature, 4))
        return;

    gameTitle    = reader.readString();
    gameSubtitle = reader.readString();
    version      = reader.readString();

    // readMode = interpretVersion(m_version);

    startSceneCategoryIndex = reader.read<byte>();
    startSceneIndex         = reader.read<ushort>();

    byte objCnt = reader.read<byte>();
    objects.clear();
    for (int i = 0; i < objCnt; ++i) objects.append(reader.readString());

    for (int i = 0; i < 8; ++i) palettes[i].read(reader);

    byte sfxCnt = reader.read<byte>();
    sfx.clear();
    for (int i = 0; i < sfxCnt; ++i) sfx.append(WAVConfiguration(reader));

    ushort totalScenes = reader.read<ushort>();
    Q_UNUSED(totalScenes);
    byte categoryCount = reader.read<byte>();

    sceneCategories.clear();
    for (int i = 0; i < categoryCount; ++i) sceneCategories.append(Category(reader, readMode));

    try {
        byte cfmCount = reader.read<byte>();

        globalVariables.clear();
        for (int i = 0; i < cfmCount; ++i) globalVariables.append(ConfigurableMemoryEntry(reader));
    } catch (const std::exception &) {
        qDebug() << "Error reading config memory! you potentially have a bad gameconfig!";
    }
}

void RSDKv5::GameConfig::write(Writer &writer)
{
    m_filename = writer.filePath;
    writer.write(m_signature, 4);

    writer.write(gameTitle);
    writer.write(gameSubtitle);
    writer.write(version);

    writer.write(startSceneCategoryIndex);
    writer.write(startSceneIndex);

    writer.write((byte)objects.count());
    for (int i = 0; i < (byte)objects.count(); ++i) writer.write(objects[i]);

    for (int i = 0; i < 8; ++i) palettes[i].write(writer);

    writer.write((byte)sfx.count());
    for (int i = 0; i < (byte)sfx.count(); ++i) sfx[i].write(writer);

    ushort totScn = 0;
    for (Category &cat : sceneCategories) totScn += cat.m_scenes.count();
    writer.write(totScn);

    writer.write((byte)sceneCategories.count());
    for (Category &cat : sceneCategories) cat.write(writer, readMode);

    writer.write((byte)globalVariables.count());
    for (ConfigurableMemoryEntry &c : globalVariables) c.write(writer);

    writer.flush();
}
