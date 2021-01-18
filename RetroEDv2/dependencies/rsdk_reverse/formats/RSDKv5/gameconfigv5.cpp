#include "include.hpp"

bool interpretVersion(QString ver)
{
    return !(ver.contains("1.00.") || ver.contains("1.01.") || ver.contains("1.02.")
             || ver.contains("1.03."));
}

void RSDKv5::GameConfig::read(Reader &reader)
{
    m_filename = reader.m_filepath;

    if (!reader.matchesSignature(m_signature, 4))
        return;

    m_gameName    = reader.readString();
    m_gameSubname = reader.readString();
    m_version     = reader.readString();

    m_readMode = interpretVersion(m_version);

    m_startSceneCategoryIndex = reader.read<byte>();
    m_startSceneIndex         = reader.read<ushort>();

    byte objCnt = reader.read<byte>();
    m_objects.clear();
    for (int i = 0; i < objCnt; ++i) m_objects.append(reader.readString());

    for (int i = 0; i < 8; ++i) m_palettes[i].read(reader);

    byte sfxCnt = reader.read<byte>();
    m_sfx.clear();
    for (int i = 0; i < sfxCnt; ++i) m_sfx.append(WAVConfiguration(reader));

    ushort totalScenes = reader.read<ushort>();
    Q_UNUSED(totalScenes);
    byte categoryCount = reader.read<byte>();

    m_sceneCategories.clear();
    for (int i = 0; i < categoryCount; ++i) m_sceneCategories.append(Category(reader, m_readMode));

    try {
        byte cfmCount = reader.read<byte>();

        m_gameOptions.clear();
        for (int i = 0; i < cfmCount; ++i) m_gameOptions.append(ConfigurableMemoryEntry(reader));
    } catch (const std::exception &) {
        qDebug() << "Error reading config memory! you potentially have a bad gameconfig!";
    }
}

void RSDKv5::GameConfig::write(Writer &writer)
{
    m_filename = writer.m_filename;
    writer.write(m_signature, 4);

    writer.write(m_gameName);
    writer.write(m_gameSubname);
    writer.write(m_version);

    writer.write(m_startSceneCategoryIndex);
    writer.write(m_startSceneIndex);

    writer.write((byte)m_objects.count());
    for (int i = 0; i < (byte)m_objects.count(); ++i) writer.write(m_objects[i]);

    for (int i = 0; i < 8; ++i) m_palettes[i].write(writer);

    writer.write((byte)m_sfx.count());
    for (int i = 0; i < (byte)m_sfx.count(); ++i) m_sfx[i].write(writer);

    ushort totScn = 0;
    for (Category &cat : m_sceneCategories) totScn += cat.m_scenes.count();
    writer.write(totScn);

    writer.write((byte)m_sceneCategories.count());
    for (Category &cat : m_sceneCategories) cat.write(writer, m_readMode);

    writer.write((byte)m_gameOptions.count());
    for (ConfigurableMemoryEntry &c : m_gameOptions) c.write(writer);

    writer.flush();
}
