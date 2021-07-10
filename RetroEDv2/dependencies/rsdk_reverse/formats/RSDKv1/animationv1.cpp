#include "include.hpp"

void RSDKv1::Animation::read(Reader &reader, bool dcVer)
{
    filepath = reader.filepath;

    unknown    = reader.read<byte>();
    playerType = reader.read<byte>();

    int sheetCount = dcVer ? 2 : 3;
    int animCount  = reader.read<byte>();

    for (int s = 0; s < sheetCount; ++s) sheets.append(reader.readString());

    for (int a = 0; a < animCount; ++a) animations.append(AnimationEntry(reader));
}

void RSDKv1::Animation::write(Writer &writer, bool dcVer)
{
    filepath = writer.filePath;

    writer.write(unknown);
    writer.write(playerType);

    writer.write((byte)animations.count());

    for (int s = 0; s < (dcVer ? 2 : 3); ++s) writer.write(sheets[s]);

    for (int a = 0; a < animations.count(); ++a) animations[a].write(writer);

    writer.flush();
}
