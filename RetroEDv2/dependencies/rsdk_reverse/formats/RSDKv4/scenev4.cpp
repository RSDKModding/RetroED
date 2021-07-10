#include "include.hpp"

QList<QString> RSDKv4::objectAttributeNames = { "State",          "Direction", "Scale",     "Rotation",
                                                "DrawOrder",      "Priority",  "Alpha",     "Animation",
                                                "AnimationSpeed", "Frame",     "InkEffect", "Value0",
                                                "Value1",         "Value2",    "Value3" };

QList<QString> RSDKv4::objectAttributeTypes = { "int",   "uint8", "int",   "int", "uint8",
                                                "uint8", "uint8", "uint8", "int", "uint8",
                                                "uint8", "int",   "int",   "int", "int" };

void RSDKv4::Scene::read(Reader &reader)
{
    m_filename = reader.filepath;

    m_title = reader.readString();

    for (int i = 0; i < 4; ++i) m_activeLayer[i] = reader.read<byte>();
    m_midpoint = reader.read<byte>();

    // Map width in chunk units
    // In RSDKv4, it's one byte long, no idea why it reads 2 bytes
    m_width = reader.read<byte>();
    reader.read<byte>();

    m_height = reader.read<byte>();
    reader.read<byte>();

    m_layout.reserve(m_height);
    for (int y = 0; y < m_height; ++y) {
        m_layout.append(QList<ushort>());
        m_layout[y].reserve(m_width);
        for (int x = 0; x < m_width; ++x) {
            // 128x128 Block number is 16-bit
            // Big-Endian in RSDKv4
            byte b0 = 0, b1 = 0;
            b0 = reader.read<byte>();
            b1 = reader.read<byte>();
            m_layout[y].append((ushort)(b0 + (b1 << 8)));
        }
    }

    // Read object data
    int objCount = 0;

    // 2 bytes, big-endian, unsigned
    objCount = reader.read<byte>();
    objCount |= reader.read<byte>() << 8;

    for (int n = 0; n < objCount; ++n) m_objects.append(Object(reader, n));
}

void RSDKv4::Scene::write(Writer &writer)
{
    m_filename = writer.filePath;

    // Write zone name
    writer.write(m_title);

    // Write the five display bytes
    for (int i = 0; i < 4; ++i) writer.write(m_activeLayer[i]);
    writer.write(m_midpoint);

    // Write width and height
    writer.write((byte)m_width);
    writer.write((byte)0x00);

    writer.write((byte)m_height);
    writer.write((byte)0x00);

    // Write tile map
    for (int h = 0; h < m_height; ++h) {
        for (int w = 0; w < m_width; ++w) {
            writer.write((byte)(m_layout[h][w] & 0xFF));
            writer.write((byte)(m_layout[h][w] >> 8));
        }
    }

    // Write number of objects
    writer.write((byte)(m_objects.count() & 0xFF));
    writer.write((byte)(m_objects.count() >> 8));

    std::sort(m_objects.begin(), m_objects.end(),
              [](const Object &a, const Object &b) -> bool { return a.m_id < b.m_id; });

    // Write object data
    for (Object &obj : m_objects) {
        obj.write(writer);
    }
    writer.flush();
}

void RSDKv4::Scene::Object::read(Reader &reader, int id)
{

    QByteArray buf;

    // Attribute bits, 2 bytes, unsigned
    buf          = reader.readByteArray(2);
    ushort flags = (ushort)((buf[1] << 8) + buf[0]);
    for (int i = 0; i < 0x0F; ++i) {
        m_attributes[i].m_active = Utils::getBit(flags, i);
    }

    // Object type, 1 byte, unsigned
    m_type = reader.read<byte>();
    // Object subtype, 1 byte, unsigned
    m_subtype = reader.read<byte>();

    buf    = reader.readByteArray(4);
    m_xPos = ((byte)buf[3] << 24) | ((byte)buf[2] << 16) | ((byte)buf[1] << 8) | ((byte)buf[0] << 0);

    buf    = reader.readByteArray(4);
    m_yPos = ((byte)buf[3] << 24) | ((byte)buf[2] << 16) | ((byte)buf[1] << 8) | ((byte)buf[0] << 0);

    bool attribInt[] = {
        true, false, true, true, false, false, false, false, true, false, false, true, true, true, true,
    };

    for (int i = 0; i < 0x0F; ++i) {
        if (m_attributes[i].m_active) {
            if (!attribInt[i]) {
                m_attributes[i].m_value = reader.read<byte>();
            }
            else {
                buf                     = reader.readByteArray(4);
                m_attributes[i].m_value = ((byte)buf[3] << 24) | ((byte)buf[2] << 16)
                                          | ((byte)buf[1] << 8) | ((byte)buf[0] << 0);
            }
        }
    }

    m_id = id;
}

void RSDKv4::Scene::Object::write(Writer &writer)
{
    ushort flags = 0;
    for (int a = 0; a < 0x0F; ++a) {
        Utils::setBit(flags, m_attributes[a].m_active, a);
    }

    writer.write((byte)(flags & 0xFF));
    writer.write((byte)(flags >> 8));
    // writer.write((ushort)flags);

    writer.write(m_type);
    writer.write(m_subtype);

    writer.write((byte)(m_xPos & 0xFF));
    writer.write((byte)(m_xPos >> 8));
    writer.write((byte)(m_xPos >> 16));
    writer.write((byte)(m_xPos >> 24));

    writer.write((byte)(m_yPos & 0xFF));
    writer.write((byte)(m_yPos >> 8));
    writer.write((byte)(m_yPos >> 16));
    writer.write((byte)(m_yPos >> 24));

    bool attribInt[] = {
        true, false, true,  true, false, false, false, false,
        true, false, false, true, true,  true,  true,  true,
    };

    for (int i = 0; i < 0x0F; ++i) {
        if (m_attributes[i].m_active) {
            if (!attribInt[i]) {
                writer.write((byte)m_attributes[i].m_value);
            }
            else {
                writer.write((byte)(m_attributes[i].m_value & 0xFF));
                writer.write((byte)(m_attributes[i].m_value >> 8));
                writer.write((byte)(m_attributes[i].m_value >> 16));
                writer.write((byte)(m_attributes[i].m_value >> 24));
            }
        }
    }
}
