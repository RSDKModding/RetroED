#include "rsdkreverse.hpp"

QList<QString> RSDKv4::objectVariableNames = { "state",          "direction", "scale",     "rotation",
                                               "drawOrder",      "priority",  "alpha",     "animation",
                                               "animationSpeed", "frame",     "inkEffect", "value0",
                                               "value1",         "value2",    "value3" };

QList<QString> RSDKv4::objectVariableTypes = { "int32", "uint8", "int32", "int32", "uint8",
                                               "uint8", "uint8", "uint8", "int32", "uint8",
                                               "uint8", "int32", "int32", "int32", "int32" };

void RSDKv4::Scene::read(Reader &reader)
{
    filePath = reader.filePath;

    title = reader.readString();

    for (int i = 0; i < 4; ++i) activeLayers[i] = reader.read<byte>();
    midpoint = reader.read<byte>();

    // Map width in chunk units
    // In RSDKv4, it's one byte long, no idea why it reads 2 bytes
    width = reader.read<byte>();
    reader.read<byte>();

    height = reader.read<byte>();
    reader.read<byte>();

    layout.reserve(height);
    for (int y = 0; y < height; ++y) {
        layout.append(QList<ushort>());
        layout[y].reserve(width);
        for (int x = 0; x < width; ++x) {
            // 128x128 Block number is 16-bit
            // Big-Endian in RSDKv4
            byte b0 = 0, b1 = 0;
            b0 = reader.read<byte>();
            b1 = reader.read<byte>();
            layout[y].append((ushort)(b0 + (b1 << 8)));
        }
    }

    // Read object data
    int objCount = 0;

    // 2 bytes, big-endian, unsigned
    objCount = reader.read<byte>();
    objCount |= reader.read<byte>() << 8;

    for (int n = 0; n < objCount; ++n) entities.append(Entity(reader, n));
}

void RSDKv4::Scene::write(Writer &writer)
{
    filePath = writer.filePath;

    // Write zone name
    writer.write(title);

    // Write the five display bytes
    for (int i = 0; i < 4; ++i) writer.write(activeLayers[i]);
    writer.write(midpoint);

    // Write width and height
    writer.write((byte)width);
    writer.write((byte)0x00);

    writer.write((byte)height);
    writer.write((byte)0x00);

    // Write tile map
    for (int h = 0; h < height; ++h) {
        for (int w = 0; w < width; ++w) {
            writer.write((byte)(layout[h][w] & 0xFF));
            writer.write((byte)(layout[h][w] >> 8));
        }
    }

    // Write number of objects
    writer.write((byte)(entities.count() & 0xFF));
    writer.write((byte)(entities.count() >> 8));

    std::sort(entities.begin(), entities.end(),
              [](const Entity &a, const Entity &b) -> bool { return a.slotID < b.slotID; });

    // Write object data
    for (Entity &obj : entities) obj.write(writer);
    writer.flush();
}

void RSDKv4::Scene::Entity::read(Reader &reader, int id)
{

    QByteArray buf;

    // Attribute bits, 2 bytes, unsigned
    buf          = reader.readByteArray(2);
    ushort flags = (ushort)((byte)buf[1] << 8) | ((byte)buf[0] << 0);
    for (int i = 0; i < 0x0F; ++i) variables[i].active = Utils::getBit(flags, i);

    // Object type, 1 byte, unsigned
    type = reader.read<byte>();
    // Object propertyValue, 1 byte, unsigned
    propertyValue = reader.read<byte>();

    buf  = reader.readByteArray(4);
    posX = ((byte)buf[3] << 24) | ((byte)buf[2] << 16) | ((byte)buf[1] << 8) | ((byte)buf[0] << 0);

    buf  = reader.readByteArray(4);
    posY = ((byte)buf[3] << 24) | ((byte)buf[2] << 16) | ((byte)buf[1] << 8) | ((byte)buf[0] << 0);

    bool varInt[] = {
        true, false, true, true, false, false, false, false, true, false, false, true, true, true, true,
    };

    for (int i = 0; i < 0x0F; ++i) {
        if (variables[i].active) {
            if (!varInt[i]) {
                variables[i].value = reader.read<byte>();
            }
            else {
                buf                = reader.readByteArray(4);
                variables[i].value = ((byte)buf[3] << 24) | ((byte)buf[2] << 16) | ((byte)buf[1] << 8)
                                     | ((byte)buf[0] << 0);
            }
        }
    }

    slotID = id;
}

void RSDKv4::Scene::Entity::write(Writer &writer)
{
    ushort flags = 0;
    for (int a = 0; a < 0x0F; ++a) {
        Utils::setBit(flags, variables[a].active, a);
    }

    writer.write((byte)(flags & 0xFF));
    writer.write((byte)(flags >> 8));
    // writer.write((ushort)flags);

    writer.write(type);
    writer.write(propertyValue);

    writer.write((byte)(posX & 0xFF));
    writer.write((byte)(posX >> 8));
    writer.write((byte)(posX >> 16));
    writer.write((byte)(posX >> 24));

    writer.write((byte)(posY & 0xFF));
    writer.write((byte)(posY >> 8));
    writer.write((byte)(posY >> 16));
    writer.write((byte)(posY >> 24));

    bool varInt[] = {
        true, false, true, true, false, false, false, false, true, false, false, true, true, true, true,
    };

    for (int i = 0; i < 0x0F; ++i) {
        if (variables[i].active) {
            if (!varInt[i]) {
                writer.write((byte)variables[i].value);
            }
            else {
                writer.write((byte)(variables[i].value & 0xFF));
                writer.write((byte)(variables[i].value >> 8));
                writer.write((byte)(variables[i].value >> 16));
                writer.write((byte)(variables[i].value >> 24));
            }
        }
    }
}
