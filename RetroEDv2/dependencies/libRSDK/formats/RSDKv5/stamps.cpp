#include "libRSDK.hpp"

#include "stamps.hpp"

void RSDKv5::Stamps::read(Reader &reader)
{
    filePath = reader.filePath;
    if (!reader.matchesSignature(signature, 4))
        return;

    ushort stampCount = reader.read<ushort>();
    for (int i = 0; i < stampCount; ++i) {
        stampList.append(StampEntry(reader));
    }
}

void RSDKv5::Stamps::write(Writer &writer)
{
    filePath = writer.filePath;
    writer.write(signature, 4);

    writer.write((ushort)stampList.count());
    for (int i = 0; i < stampList.count(); ++i) {
        stampList[i].write(writer);
    }

    writer.flush();
}

void RSDKv5::Stamps::StampEntry::read(Reader &reader)
{
    name   = reader.readString();
    size.x = reader.read<int>();
    size.y = reader.read<int>();
    byte tileCount = size.x * size.y;
    for (int tile = 0; tile < tileCount; tile++){
        tiles.append(reader.read<ushort>());
    };
}

void RSDKv5::Stamps::StampEntry::write(Writer &writer)
{
    writer.write(name);
    writer.write(size.x);
    writer.write(size.y);
    byte tileCount = size.x * size.y;
    for (int tile = 0; tile < tileCount; tile++){
        writer.write(tiles[tile]);
    };
}
