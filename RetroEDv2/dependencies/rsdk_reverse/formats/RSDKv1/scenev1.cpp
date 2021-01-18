#include "include.hpp"

void RSDKv1::Scene::read(Reader &reader)
{
    m_filename = reader.m_filepath;

    // Map width in 128 pixel units
    // In RSDKv1, it's one byte long
    m_width  = reader.read<byte>();
    m_height = reader.read<byte>();

    m_layout.reserve(m_height);
    for (int y = 0; y < m_height; ++y) {
        m_layout.append(QList<byte>());
        m_layout[y].reserve(m_width);
        for (int x = 0; x < m_width; ++x) {
            // 128x128 Block number is 8-bit in RSDKv1
            m_layout[y].append(reader.read<byte>());
        }
    }

    QString itmpath = QFileInfo(reader.m_filepath).absolutePath() + "/"
                      + QFileInfo(reader.m_filepath).baseName() + ".itm";
    Reader ITMreader(itmpath);

    m_title = ITMreader.readString();

    m_music      = ITMreader.read<byte>();
    m_background = ITMreader.read<byte>();
    m_playerXPos = (short)(ITMreader.read<byte>() << 8);
    m_playerXPos |= ITMreader.read<byte>();
    m_playerYPos = (short)(ITMreader.read<byte>() << 8);
    m_playerYPos |= ITMreader.read<byte>();

    // Read number of object types, Only RSDKv2 and RSDKv3 support this feature
    // 2 bytes, big-endian, unsigned
    int objCount = ITMreader.read<byte>() << 8;
    objCount |= ITMreader.read<byte>();

    for (int n = 0; n < objCount; ++n) {
        m_objects.append(Object(ITMreader, n));
    }
}

void RSDKv1::Scene::write(Writer &writer)
{
    m_filename = writer.m_filename;

    // Write width and height
    writer.write(m_width);
    writer.write(m_height);

    // Write tile map
    for (int h = 0; h < m_height; ++h) {
        for (int w = 0; w < m_width; ++w) {
            writer.write(m_layout[h][w]);
        }
    }
    writer.flush();

    QString itmpath = QFileInfo(writer.m_filename).absolutePath() + "/"
                      + QFileInfo(writer.m_filename).baseName() + ".itm";
    Writer ITMwriter(itmpath);
    // Write zone name
    ITMwriter.write(m_title);

    // Write the Stage Init Data
    ITMwriter.write(m_music);
    ITMwriter.write(m_background);
    ITMwriter.write((byte)(m_playerXPos >> 8));
    ITMwriter.write((byte)(m_playerXPos & 0xFF));
    ITMwriter.write((byte)(m_playerYPos >> 8));
    ITMwriter.write((byte)(m_playerYPos & 0xFF));

    // Write number of objects
    ITMwriter.write((byte)(m_objects.count() >> 8));
    ITMwriter.write((byte)(m_objects.count() & 0xFF));

    std::sort(m_objects.begin(), m_objects.end(),
              [](const Object &a, const Object &b) -> bool { return a.m_id < b.m_id; });

    // Write object data
    for (Object &obj : m_objects) {
        obj.write(ITMwriter);
    }

    ITMwriter.flush();
}
