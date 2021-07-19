#include "include.hpp"

void RSDKv5::StaticObject::read(Reader &reader)
{
    m_filename = reader.filepath;
    if (!reader.matchesSignature(m_signature, 4))
        return;

    uint memPos = 0;
    values.clear();
    while (!reader.isEOF()) {
        int dataType  = reader.read<byte>();
        int arraySize = reader.read<int>();

        if ((dataType & 0x80) != 0) {
            uint dataSize = reader.read<uint>();
            dataType &= 0x7F;

            ArrayInfo array;
            array.type     = (byte)dataType;
            array.size     = arraySize;
            array.dataSize = (int)dataSize;

            switch (dataType) {
                default: break;
                case AttributeTypes::UINT8: {
                    for (uint d = 0; d < dataSize; ++d) array.entries.append(reader.read<byte>());

                    memPos += arraySize;
                    break;
                }
                case AttributeTypes::INT8: {
                    for (uint d = 0; d < dataSize; ++d) array.entries.append(reader.read<char>());

                    memPos += arraySize;
                    break;
                }
                case AttributeTypes::UINT16: {
                    int tmpOffset = (int)((memPos & 0xFFFFFFFE) + 2);
                    if ((memPos & 0xFFFFFFFE) >= memPos)
                        tmpOffset = memPos;
                    memPos = tmpOffset;

                    for (uint d = 0; d < dataSize; ++d) {
                        byte valA = reader.read<byte>();
                        byte valB = reader.read<byte>();
                        array.entries.append((ushort)(valA + (valB << 8)));
                    }

                    memPos += 2 * arraySize;
                    break;
                }
                case AttributeTypes::INT16: {
                    int tmpOffset = (int)((memPos & 0xFFFFFFFE) + 2);
                    if ((memPos & 0xFFFFFFFE) >= memPos)
                        tmpOffset = memPos;
                    memPos = tmpOffset;

                    for (uint d = 0; d < dataSize; ++d) {
                        byte valA = reader.read<byte>();
                        byte valB = reader.read<byte>();
                        array.entries.append((short)(valA + (valB << 8)));
                    }
                    memPos += 2 * arraySize;
                    break;
                }
                case AttributeTypes::UINT32: {
                    int tmpOffset = (int)((memPos & 0xFFFFFFFC) + 4);
                    if ((memPos & 0xFFFFFFFC) >= memPos)
                        tmpOffset = memPos;
                    memPos = tmpOffset;

                    for (uint d = 0; d < dataSize; ++d) {
                        byte valA = reader.read<byte>();
                        byte valB = reader.read<byte>();
                        byte valC = reader.read<byte>();
                        byte valD = reader.read<byte>();
                        array.entries.append(
                            (int)(uint)(valA + (valB << 8) + (valC << 16) + (valD << 24)));
                    }
                    memPos += 4 * arraySize;
                    break;
                }
                case AttributeTypes::INT32: {
                    int tmpOffset = (int)((memPos & 0xFFFFFFFC) + 4);
                    if ((memPos & 0xFFFFFFFC) >= memPos)
                        tmpOffset = memPos;
                    memPos = tmpOffset;

                    for (uint d = 0; d < dataSize; ++d) {
                        byte valA = reader.read<byte>();
                        byte valB = reader.read<byte>();
                        byte valC = reader.read<byte>();
                        byte valD = reader.read<byte>();
                        array.entries.append(valA + (valB << 8) + (valC << 16) + (valD << 24));
                    }
                    memPos += 4 * arraySize;
                    break;
                }
                case AttributeTypes::ENUM: {
                    int tmpOffset = (int)((memPos & 0xFFFFFFFC) + 4);
                    if ((memPos & 0xFFFFFFFC) >= memPos)
                        tmpOffset = memPos;
                    memPos = tmpOffset;

                    for (uint d = 0; d < dataSize; ++d) {
                        byte valA = reader.read<byte>();
                        byte valB = reader.read<byte>();
                        byte valC = reader.read<byte>();
                        byte valD = reader.read<byte>();
                        array.entries.append(valA + (valB << 8) + (valC << 16) + (valD << 24));
                    }
                    memPos += 4 * arraySize;
                    break;
                }
            }
            values.append(array);
        }
        else {
            ArrayInfo array;
            array.type     = (byte)dataType;
            array.size     = arraySize;
            array.dataSize = 0;
            array.entries.clear();
            values.append(array);

            int buffer = 0;
            switch (dataType) {
                case AttributeTypes::UINT8:
                case AttributeTypes::INT8: memPos += arraySize; break;
                case AttributeTypes::UINT16:
                case AttributeTypes::INT16:
                    buffer = (int)((memPos & 0xFFFFFFFE) + 2);
                    if ((memPos & 0xFFFFFFFE) >= memPos)
                        buffer = memPos;
                    memPos = buffer + 2 * arraySize;
                    break;
                case AttributeTypes::UINT32:
                case AttributeTypes::INT32:
                case AttributeTypes::ENUM:
                case AttributeTypes::BOOL:
                    buffer = (int)((memPos & 0xFFFFFFFC) + 4);
                    if ((memPos & 0xFFFFFFFC) >= memPos)
                        buffer = memPos;
                    memPos = buffer + 4 * arraySize;
                    break;
                case AttributeTypes::STRING:
                case AttributeTypes::VECTOR2:
                    buffer = (int)((memPos & 0xFFFFFFFC) + 4);
                    if ((memPos & 0xFFFFFFFC) >= memPos)
                        buffer = memPos;
                    memPos = buffer + 8 * arraySize;
                    break;
                case AttributeTypes::UNKNOWN:
                    buffer = (int)((memPos & 0xFFFFFFFC) + 4);
                    if ((memPos & 0xFFFFFFFC) >= memPos)
                        buffer = memPos;
                    memPos = buffer + 24 * arraySize;
                    break;
                case AttributeTypes::COLOR:
                    buffer = (int)((memPos & 0xFFFFFFFE) + 2);
                    if ((memPos & 0xFFFFFFFE) >= memPos)
                        buffer = memPos;
                    memPos = buffer + 8 * arraySize;
                    break;
                default: break;
            }
        }
    }
}

void RSDKv5::StaticObject::write(Writer &writer)
{
    m_filename = writer.filePath;
    writer.write(m_signature, 4);

    for (ArrayInfo &array : values) {
        writer.write((byte)(array.dataSize > 0 ? (array.type | 0x80) : array.type));
        writer.write(array.size);

        if (array.dataSize > 0) {
            writer.write(array.dataSize);

            switch (array.type) {
                case AttributeTypes::UINT8:
                case AttributeTypes::INT8:
                    for (int i = 0; i < array.dataSize; ++i) writer.write(array.entries[i]);
                    break;
                case AttributeTypes::UINT16:
                case AttributeTypes::INT16:
                    for (int i = 0; i < array.dataSize; ++i) {
                        Utils::intBytes b = Utils::intBytes(array.entries[i]);
                        writer.write(b.m_bytes[0]);
                        writer.write(b.m_bytes[1]);
                    }
                    break;
                case AttributeTypes::UINT32:
                case AttributeTypes::INT32:
                case AttributeTypes::ENUM:
                    for (int i = 0; i < array.dataSize; ++i) {
                        Utils::intBytes b = Utils::intBytes(array.entries[i]);
                        writer.write(b.m_bytes[0]);
                        writer.write(b.m_bytes[1]);
                        writer.write(b.m_bytes[2]);
                        writer.write(b.m_bytes[3]);
                    }
                    break;
            }
        }
    }

    writer.flush();
}

uint RSDKv5::StaticObject::getOffset(int arrayID)
{
    uint memPos = 0;
    for (int a = 0; a < arrayID; ++a) {
        int buffer = 0;
        switch (values[a].type) {
            case AttributeTypes::UINT8:
            case AttributeTypes::INT8: memPos += values[a].size; break;
            case AttributeTypes::UINT16:
            case AttributeTypes::INT16:
                buffer = (int)((memPos & 0xFFFFFFFE) + 2);
                if ((memPos & 0xFFFFFFFE) >= memPos)
                    buffer = memPos;
                memPos = buffer + 2 * values[a].size;
                break;
            case AttributeTypes::UINT32:
            case AttributeTypes::INT32:
            case AttributeTypes::ENUM:
            case AttributeTypes::BOOL:
                buffer = (int)((memPos & 0xFFFFFFFC) + 4);
                if ((memPos & 0xFFFFFFFC) >= memPos)
                    buffer = memPos;
                memPos = buffer + 4 * values[a].size;
                break;
            case AttributeTypes::STRING:
            case AttributeTypes::VECTOR2:
                buffer = (int)((memPos & 0xFFFFFFFC) + 4);
                if ((memPos & 0xFFFFFFFC) >= memPos)
                    buffer = memPos;
                memPos = buffer + 8 * values[a].size;
                break;
            case AttributeTypes::UNKNOWN:
                buffer = (int)((memPos & 0xFFFFFFFC) + 4);
                if ((memPos & 0xFFFFFFFC) >= memPos)
                    buffer = memPos;
                memPos = buffer + 24 * values[a].size;
                break;
            case AttributeTypes::COLOR:
                buffer = (int)((memPos & 0xFFFFFFFE) + 2);
                if ((memPos & 0xFFFFFFFE) >= memPos)
                    buffer = memPos;
                memPos = buffer + 8 * values[a].size;
                break;
            default: break;
        }
    }
    return memPos;
}

int RSDKv5::StaticObject::getDataSize(int type)
{
    switch (type) {
        case AttributeTypes::UINT8:
        case AttributeTypes::INT8: return 1;
        case AttributeTypes::UINT16:
        case AttributeTypes::INT16: return 2;
        case AttributeTypes::UINT32:
        case AttributeTypes::INT32:
        case AttributeTypes::ENUM:
        case AttributeTypes::BOOL: return 4;
        case AttributeTypes::STRING:
        case AttributeTypes::VECTOR2: return 8;
        case AttributeTypes::UNKNOWN: return 24;
        case AttributeTypes::COLOR: return 8;
        default: return 0;
    }
}
