#include "include.hpp"

void RSDKv5::Stamps::read(Reader &reader)
{
    filePath = reader.filepath;
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
    width  = reader.read<ushort>();
    height = reader.read<ushort>();

    layout.resize(height);
    for (int y = 0; y < height; ++y) {
        layout[y].resize(width);
        for (int x = 0; x < width; ++x) layout[y][x] = reader.read<ushort>();
    }
}

void RSDKv5::Stamps::StampEntry::write(Writer &writer)
{
    writer.write(name);
    writer.write(width);
    writer.write(height);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) writer.write(layout[y][x]);
    }
}
