#include "include.hpp"

void RSDKv5::Scene::AttributeValue::read(Reader &reader)
{
    switch (m_type) {
        case AttributeTypes::UINT8: value_uint8 = reader.read<byte>(); break;
        case AttributeTypes::UINT16: value_uint16 = reader.read<ushort>(); break;
        case AttributeTypes::UINT32: value_uint32 = reader.read<uint>(); break;
        case AttributeTypes::INT8: value_int8 = reader.read<char>(); break;
        case AttributeTypes::INT16: value_int16 = reader.read<short>(); break;
        case AttributeTypes::INT32: value_int32 = reader.read<int>(); break;
        case AttributeTypes::ENUM: value_enum = reader.read<int>(); break;
        case AttributeTypes::BOOL: value_bool = reader.read<uint>() != 0; break;
        case AttributeTypes::STRING: value_string = reader.readString(1); break;
        case AttributeTypes::VECTOR2: value_vector2 = Position(reader); break;
        case AttributeTypes::UNKNOWN: value_vector3 = Position(reader); break;
        case AttributeTypes::COLOR: {
            byte r      = reader.read<byte>();
            byte g      = reader.read<byte>();
            byte b      = reader.read<byte>();
            byte a      = reader.read<byte>();
            value_color = QColor(r, g, b, a);
            break;
        }
    }
}

void RSDKv5::Scene::AttributeValue::write(Writer &writer)
{
    switch (m_type) {
        case AttributeTypes::UINT8: writer.write(value_uint8); break;
        case AttributeTypes::UINT16: writer.write(value_uint16); break;
        case AttributeTypes::UINT32: writer.write(value_uint32); break;
        case AttributeTypes::INT8: writer.write(value_int8); break;
        case AttributeTypes::INT16: writer.write(value_int16); break;
        case AttributeTypes::INT32: writer.write(value_int32); break;
        case AttributeTypes::ENUM: writer.write(value_enum); break;
        case AttributeTypes::BOOL: writer.write((uint)(value_bool ? 1 : 0)); break;
        case AttributeTypes::STRING: writer.write(value_string, 1); break;
        case AttributeTypes::VECTOR2: value_vector2.write(writer); break;
        case AttributeTypes::UNKNOWN: value_vector3.write(writer, true); break;
        case AttributeTypes::COLOR: {
            writer.write((byte)value_color.red());
            writer.write((byte)value_color.green());
            writer.write((byte)value_color.blue());
            writer.write((byte)value_color.alpha());
            break;
        }
    }
}

void RSDKv5::Scene::SceneLayer::read(Reader &reader)
{
    m_unknownByte = reader.read<byte>();

    m_name = reader.readString();

    m_behaviour    = reader.read<byte>();
    m_drawingOrder = reader.read<byte>();

    m_width  = reader.read<ushort>();
    m_height = reader.read<ushort>();

    m_relativeSpeed = reader.read<short>();
    m_constantSpeed = reader.read<short>();

    ushort scrollInfoCount = reader.read<ushort>();
    for (int i = 0; i < scrollInfoCount; ++i) m_scrollingInfo.append(ScrollInfo(reader));

    m_scrollIndexes = reader.readZLib();

    m_tiles.resize(m_height);
    Reader creader = reader.getCReader();
    for (ushort y = 0; y < m_height; ++y) {
        m_tiles[y].resize(m_width);
        for (ushort x = 0; x < m_width; ++x) m_tiles[y][x] = creader.read<ushort>();
    }
}

void RSDKv5::Scene::SceneLayer::write(Writer &writer)
{
    writer.write(m_unknownByte);

    writer.write(m_name + '\0');

    writer.write(m_behaviour);
    writer.write(m_drawingOrder);

    writer.write(m_width);
    writer.write(m_height);

    writer.write(m_relativeSpeed);
    writer.write(m_constantSpeed);

    writer.write((ushort)m_scrollingInfo.count());
    for (ScrollInfo &info : m_scrollingInfo) info.write(writer);

    writer.writeCompressed(m_scrollIndexes);

    QByteArray bytes;
    QBuffer buffer(&bytes);
    buffer.open(QIODevice::WriteOnly);
    QDataStream *mem = new QDataStream(&buffer);
    Writer cwriter(mem);

    for (int y = 0; y < m_height; ++y) {
        for (int x = 0; x < m_width; ++x) cwriter.write(m_tiles[y][x]);
    }
    writer.writeCompressed(bytes);
}

void RSDKv5::Scene::SceneLayer::resize(ushort width, ushort height)
{
    ushort oldWidth  = m_width;
    ushort oldHeight = m_height;
    m_width          = width;
    m_height         = height;
    m_scrollIndexes.resize(m_height * 0x10);
    m_tiles.resize(height);

    for (ushort y = oldHeight; y < m_height; ++y) {
        m_tiles[y].resize(oldWidth);
        for (int x = 0; x < oldWidth; ++x) {
            m_tiles[y][x] = 0;
        }
    }

    for (ushort y = 0; y < m_height; ++y) {
        m_tiles[y].resize(m_width);
        for (ushort x = oldWidth; x < m_width; ++x) m_tiles[y][x] = 0;
    }
}

void RSDKv5::Scene::SceneEditorMetadata::read(Reader &reader)
{
    m_unknownByte      = reader.read<byte>();
    byte r             = reader.read<byte>();
    byte g             = reader.read<byte>();
    byte b             = reader.read<byte>();
    byte a             = reader.read<byte>();
    m_backgroundColor1 = QColor(r, g, b, a);
    r                  = reader.read<byte>();
    g                  = reader.read<byte>();
    b                  = reader.read<byte>();
    a                  = reader.read<byte>();
    m_backgroundColor2 = QColor(r, g, b, a);
    m_unknownBytes     = reader.readByteArray(7);
    m_stampName        = reader.readString();
    m_unknownByte2     = reader.read<byte>();
}

void RSDKv5::Scene::SceneEditorMetadata::write(Writer &writer)
{
    writer.write(m_unknownByte);
    writer.write((byte)m_backgroundColor1.red());
    writer.write((byte)m_backgroundColor1.green());
    writer.write((byte)m_backgroundColor1.blue());
    writer.write((byte)m_backgroundColor1.alpha());
    writer.write((byte)m_backgroundColor2.red());
    writer.write((byte)m_backgroundColor2.green());
    writer.write((byte)m_backgroundColor2.blue());
    writer.write((byte)m_backgroundColor2.alpha());
    writer.write(m_unknownBytes);
    writer.write(m_stampName + '\0');
    writer.write(m_unknownByte2);
}
