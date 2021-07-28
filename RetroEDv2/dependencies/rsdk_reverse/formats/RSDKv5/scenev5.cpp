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
    unknown = reader.read<byte>();

    m_name = reader.readString();

    type      = reader.read<byte>();
    drawOrder = reader.read<byte>();

    width  = reader.read<ushort>();
    height = reader.read<ushort>();

    parallaxFactor = reader.read<short>(); // << 0
    scrollSpeed    = reader.read<short>(); // << 8

    ushort scrollInfoCount = reader.read<ushort>();
    for (int i = 0; i < scrollInfoCount; ++i) scrollingInfo.append(ScrollInfo(reader));

    lineIndexes = reader.readZLib();

    Reader creader = reader.getCReader();
    layout.resize(height);
    for (int y = 0; y < height; ++y) {
        layout[y].resize(width);
        for (int x = 0; x < width; ++x) layout[y][x] = creader.read<ushort>();
    }

    scrollInfoFromIndices();
}

void RSDKv5::Scene::SceneLayer::write(Writer &writer)
{
    scrollIndicesFromInfo();

    writer.write(unknown);

    writer.write(m_name + '\0');

    writer.write(type);
    writer.write(drawOrder);

    writer.write(width);
    writer.write(height);

    writer.write(parallaxFactor);
    writer.write(scrollSpeed);

    writer.write((ushort)scrollingInfo.count());
    for (ScrollInfo &info : scrollingInfo) info.write(writer);

    writer.writeCompressed(lineIndexes);

    QByteArray bytes;
    QBuffer buffer(&bytes);
    buffer.open(QIODevice::WriteOnly);
    QDataStream *mem = new QDataStream(&buffer);
    Writer cwriter(mem);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) cwriter.write(layout[y][x]);
    }
    writer.writeCompressed(bytes);
}

void RSDKv5::Scene::SceneLayer::resize(ushort width, ushort height)
{
    ushort oldWidth  = width;
    ushort oldHeight = height;
    this->width      = width;
    this->height     = height;
    lineIndexes.resize(height * 0x10);

    layout.resize(height);
    for (int y = oldHeight; y < height; ++y) {
        layout[y].resize(oldWidth);
        for (int x = 0; x < oldWidth; ++x) {
            layout[y][x] = 0;
        }
    }

    for (int y = 0; y < height; ++y) {
        layout[y].resize(width);
        for (ushort x = oldWidth; x < width; ++x) layout[y][x] = 0;
    }
}

void RSDKv5::Scene::SceneLayer::scrollInfoFromIndices()
{
    scrollInfos.clear();
    QList<ScrollInfo> infos;
    if (type == 0 || type == 1)
        infos = scrollingInfo;
    else
        return;

    int prev  = lineIndexes.count() > 0 ? lineIndexes[0] : -1;
    int start = 0;
    int h     = 0;

    for (; h < lineIndexes.count(); ++h) {
        if ((byte)lineIndexes[h] != prev) {
            ScrollIndexInfo info;

            info.startLine      = start;
            info.length         = (h - start);
            info.parallaxFactor = infos[prev].parallaxFactor / 256.0f;
            info.scrollSpeed    = infos[prev].scrollSpeed / 256.0f;
            info.m_scrollPos    = 0.0f;
            info.deform         = infos[prev].deform;

            scrollInfos.append(info);
            start = h;
        }

        prev = (byte)lineIndexes[h];
    }

    {
        ScrollIndexInfo info;

        info.startLine      = start;
        info.length         = (h - start);
        info.parallaxFactor = infos[0].parallaxFactor / 256.0f;
        info.scrollSpeed    = infos[0].scrollSpeed / 256.0f;
        info.m_scrollPos    = 0.0f;
        info.deform         = infos[0].deform;

        scrollInfos.append(info);
    }
}

void RSDKv5::Scene::SceneLayer::scrollIndicesFromInfo()
{
    bool hScroll = type == 1;
    lineIndexes.clear();

    if (type != 0 && type != 1) {
        // other layers dont need any scrolling, TODO: check this works
        return;
    }
    if (width == 0 || height == 0)
        return; // basically invalid layers, dont write em

    if (hScroll) {
        lineIndexes.resize(height * 0x10);
    }
    else {
        lineIndexes.resize(width * 0x10);
    }

    int id = 0;
    for (ScrollIndexInfo &info : scrollInfos) {
        int infoID = id;
        ScrollInfo sInfo;
        sInfo.deform         = info.deform;
        sInfo.parallaxFactor = info.parallaxFactor * 256;
        sInfo.scrollSpeed    = info.scrollSpeed * 256;

        int scrollID = 0;
        for (ScrollInfo &info : scrollingInfo) {
            if (info.parallaxFactor == sInfo.parallaxFactor && info.scrollSpeed == sInfo.scrollSpeed
                && info.deform == sInfo.deform) {
                infoID = scrollID;
                break;
            }
            ++scrollID;
        }

        for (int i = info.startLine; i < info.startLine + info.length; ++i) {
            lineIndexes[i] = (byte)infoID;
        }

        // New Info needs to be added
        if (infoID == id) {
            scrollingInfo.append(sInfo);
            ++id;
        }
    }
}

void RSDKv5::Scene::SceneEditorMetadata::read(Reader &reader)
{
    unknown1         = reader.read<byte>();
    byte b           = reader.read<byte>();
    byte g           = reader.read<byte>();
    byte r           = reader.read<byte>();
    byte a           = reader.read<byte>();
    backgroundColor1 = QColor(r, g, b, a);

    b                = reader.read<byte>();
    g                = reader.read<byte>();
    r                = reader.read<byte>();
    a                = reader.read<byte>();
    backgroundColor2 = QColor(r, g, b, a);

    unknown2  = reader.readByteArray(7);
    stampName = reader.readString();
    unknown3  = reader.read<byte>();
}

void RSDKv5::Scene::SceneEditorMetadata::write(Writer &writer)
{
    writer.write(unknown1);
    writer.write((byte)backgroundColor1.red());
    writer.write((byte)backgroundColor1.green());
    writer.write((byte)backgroundColor1.blue());
    writer.write((byte)backgroundColor1.alpha());
    writer.write((byte)backgroundColor2.red());
    writer.write((byte)backgroundColor2.green());
    writer.write((byte)backgroundColor2.blue());
    writer.write((byte)backgroundColor2.alpha());
    writer.write(unknown2);
    writer.write(stampName + '\0');
    writer.write(unknown3);
}
