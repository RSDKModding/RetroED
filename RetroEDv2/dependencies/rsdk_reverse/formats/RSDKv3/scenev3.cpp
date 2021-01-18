#include "include.hpp"

void RSDKv3::Scene::read(Reader &reader)
{
    m_filename = reader.m_filepath;

    m_title = reader.readString();

    for (int i = 0; i < 4; ++i) m_activeLayer[i] = reader.read<byte>();
    m_midpoint = reader.read<byte>();

    // Map width in 128 pixel units
    // In RSDKv3, it's one byte long
    m_width  = reader.read<byte>();
    m_height = reader.read<byte>();

    m_layout.reserve(m_height);
    for (int y = 0; y < m_height; ++y) {
        m_layout.append(QList<ushort>());
        m_layout[y].reserve(m_width);
        for (int x = 0; x < m_width; ++x) {
            // 128x128 Block number is 16-bit
            // Big-Endian in RSDKv2 and RSDKv3
            byte b0 = 0, b1 = 0;
            b0 = reader.read<byte>();
            b1 = reader.read<byte>();
            m_layout[y].append((ushort)(b1 + (b0 << 8)));
        }
    }

    // Read number of object types, Only RSDKv2 and RSDKv3 support this feature
    int objTypeCount = reader.read<byte>();
    for (int n = 0; n < objTypeCount; ++n) m_objectTypeNames.append(reader.readString());
    // Read object data

    int objCount = 0;

    // 2 bytes, big-endian, unsigned
    objCount = reader.read<byte>() << 8;
    objCount |= reader.read<byte>();

    for (int n = 0; n < objCount; ++n) m_objects.append(Object(reader, n));
}

void RSDKv3::Scene::write(Writer &writer)
{
    m_filename = writer.m_filename;

    // Write zone name
    writer.write(m_title);

    // Write the five "display" bytes
    for (int i = 0; i < 4; ++i) writer.write(m_activeLayer[i]);
    writer.write(m_midpoint);

    // Write width and height
    writer.write((byte)m_width);
    writer.write((byte)m_height);

    // Write tile map

    for (int h = 0; h < m_height; ++h) {
        for (int w = 0; w < m_width; ++w) {
            writer.write((byte)(m_layout[h][w] >> 8));
            writer.write((byte)(m_layout[h][w] & 0xff));
        }
    }

    writer.write((byte)(m_objectTypeNames.count()));

    // Write object type names
    // Ignore first object type "Type zero", it is not stored.
    for (int n = 0; n < m_objectTypeNames.count(); ++n) writer.write(m_objectTypeNames[n]);

    // Write number of objects
    writer.write((byte)(m_objects.count() >> 8));
    writer.write((byte)(m_objects.count() & 0xFF));

    std::sort(m_objects.begin(), m_objects.end(),
              [](const Object &a, const Object &b) -> bool { return a.m_id < b.m_id; });

    // Write object data
    for (Object &obj : m_objects) obj.write(writer);
    writer.flush();
}
