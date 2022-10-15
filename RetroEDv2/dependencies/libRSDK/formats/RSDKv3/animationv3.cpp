#include "libRSDK.hpp"

void RSDKv3::Animation::read(Reader &reader)
{
    filePath = reader.filePath;

    int sheetCount = reader.read<byte>();
    for (int s = 0; s < sheetCount; ++s) sheets.append(reader.readString());

    int animCount = reader.read<byte>();
    for (int a = 0; a < animCount; ++a) animations.append(AnimationEntry(reader));

    int collisionBoxCount = reader.read<byte>();
    for (int h = 0; h < collisionBoxCount; ++h) hitboxes.append(Hitbox(reader));
}

void RSDKv3::Animation::write(Writer &writer)
{
    filePath = writer.filePath;

    writer.write((byte)sheets.count());
    for (int s = 0; s < sheets.count(); ++s) writer.write(sheets[s]);

    writer.write((byte)animations.count());
    for (int a = 0; a < animations.count(); ++a) animations[a].write(writer);

    writer.write((byte)hitboxes.count());
    for (int h = 0; h < hitboxes.count(); ++h) hitboxes[h].write(writer);

    writer.flush();
}
