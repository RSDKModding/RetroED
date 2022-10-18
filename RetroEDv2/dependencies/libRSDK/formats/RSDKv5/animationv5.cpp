#include "libRSDK.hpp"

#include "animationv5.hpp"

void RSDKv5::Animation::read(Reader &reader)
{
    filePath = reader.filePath;

    if (!reader.matchesSignature(signature, 4)) {
        return;
    }

    uint totalFrameCount = reader.read<uint>();
    Q_UNUSED(totalFrameCount);

    byte sheetCount = reader.read<byte>();
    for (int s = 0; s < sheetCount; ++s) sheets.append(reader.readString());

    byte hitboxCount = reader.read<byte>();
    for (int h = 0; h < hitboxCount; ++h) hitboxTypes.append(reader.readString());

    ushort animCount = reader.read<ushort>();
    for (int a = 0; a < animCount; ++a) animations.append(AnimationEntry(reader, this));
}

void RSDKv5::Animation::write(Writer &writer)
{
    filePath = writer.filePath;

    writer.write(signature, 4);

    uint totalFrameCount = 0;
    for (auto &a : animations) totalFrameCount += a.frames.count();
    writer.write(totalFrameCount);

    writer.write((byte)sheets.count());
    for (int s = 0; s < sheets.count(); ++s) writer.write(sheets[s]);

    writer.write((byte)hitboxTypes.count());
    for (int h = 0; h < hitboxTypes.count(); ++h) writer.write(hitboxTypes[h]);

    writer.write((ushort)animations.count());
    for (int a = 0; a < animations.count(); ++a) animations[a].write(writer, this);

    writer.flush();
}
