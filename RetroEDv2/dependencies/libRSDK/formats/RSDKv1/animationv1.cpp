#include "libRSDK.hpp"

#include "animationv1.hpp"

QList<QString> animNamesv1 = QList<QString>{
    "Stopped",       "Waiting",
    "Looking Up",    "Looking Down",
    "Walking",       "Running",
    "Skidding",      "Super Peel Out",
    "Spin Dash",     "Jumping",
    "Bouncing",      "Hurt",
    "Dying",         "Life Icon",
    "Drowning",      "Fan Rotate",
    "Breathing",     "Pushing",
    "Flailing Left", "Flailing Right",
    "Sliding",       "Hanging",
    "Dropping",      "Finish Pose",
    "CorkScrew",     "Retro Sonic Animation #26",
    "Fly Tired",     "Climbing",
    "Ledge Pull Up", "Glide Slide",
    "Bonus Spin",    "Special Stop",
    "Special Walk",  "Special Jump",
};

void RSDKv1::Animation::read(Reader &reader, bool dcVer)
{
    filePath = reader.filePath;

    unknown    = reader.read<byte>();
    playerType = reader.read<byte>();

    int sheetCount = dcVer ? 2 : 3;
    int animCount  = reader.read<byte>();

    for (int s = 0; s < sheetCount; ++s) sheets.append(reader.readString());

    for (int a = 0; a < animCount; ++a) {
        animations.append(AnimationEntry(reader));
        animations.last().name =
            a < animNamesv1.count() ? animNamesv1[a] : ("Retro Sonic Animation #" + QString::number(a));
    }
}

void RSDKv1::Animation::write(Writer &writer, bool dcVer)
{
    filePath = writer.filePath;

    writer.write(unknown);
    writer.write(playerType);

    writer.write((byte)animations.count());

    for (int s = 0; s < (dcVer ? 2 : 3); ++s) writer.write(sheets[s]);

    for (int a = 0; a < animations.count(); ++a) animations[a].write(writer);

    writer.flush();
}
