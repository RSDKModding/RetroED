#include "rsdkreverse.hpp"

QList<QString> animNamesv2 = QList<QString>{
    "Stopped",
    "Waiting",
    "Bored",
    "Looking Up",
    "Looking Down",
    "Walking",
    "Running",
    "Skidding",
    "Super Peel Out",
    "Spin Dash",
    "Jumping",
    "Bouncing",
    "Hurt",
    "Dying",
    "Drowning",
    "Life Icon",
    "Fan Rotate",
    "Breathing",
    "Pushing",
    "Flailing Left",
    "Flailing Right",
    "Sliding",
    "Sonic Nexus Animation #23",
    "Finish Pose",
    "Sonic Nexus Animation #24",
    "Sonic Nexus Animation #25",
    "Sonic Nexus Animation #26",
    "Sonic Nexus Animation #27",
    "Sonic Nexus Animation #28",
    "Sonic Nexus Animation #29",
    "Sonic Nexus Animation #30",
    "Sonic Nexus Animation #31",
    "Sonic Nexus Animation #32",
    "Sonic Nexus Animation #33",
    "CorkScrew",
    "Sonic Nexus Animation #35",
    "Sonic Nexus Animation #36",
    "Sonic Nexus Animation #37",
    "Sonic Nexus Animation #38",
    "Sonic Nexus Animation #39",
    "Sonic Nexus Animation #40",
    "Sonic Nexus Animation #41",
    "Sonic Nexus Animation #42",
    "Hanging",
    "Sonic Nexus Animation #44",
    "Sonic Nexus Animation #45",
    "Sonic Nexus Animation #46",
    "Sonic Nexus Animation #47",
    "Sonic Nexus Animation #48",
    "Sonic Nexus Animation #49",
    "Sonic Nexus Animation #50",
    "Sonic Nexus Animation #51",
};

void RSDKv2::Animation::read(Reader &reader)
{
    filePath = reader.filePath;

    for (int u = 0; u < 5; ++u) unknown[u] = reader.read<byte>();

    for (int s = 0; s < 4; ++s) sheets.append(reader.readString());

    int animCount = reader.read<byte>();
    for (int a = 0; a < animCount; ++a) {
        animations.append(AnimationEntry(reader));
        animations.last().name =
            a < animNamesv2.count() ? animNamesv2[a] : ("Sonic Nexus Animation #" + QString::number(a));
    }

    int hitboxCount = reader.read<byte>();
    for (int h = 0; h < hitboxCount; ++h) hitboxes.append(Hitbox(reader));
}

void RSDKv2::Animation::write(Writer &writer)
{
    filePath = writer.filePath;

    for (int u = 0; u < 5; ++u) {
        writer.write(unknown[u]);
    }

    for (int s = 0; s < 4; ++s) {
        writer.write(sheets[s]);
    }

    writer.write((byte)animations.count());
    for (int a = 0; a < animations.count(); ++a) {
        animations[a].write(writer);
    }

    writer.write((byte)hitboxes.count());
    for (int h = 0; h < hitboxes.count(); ++h) {
        hitboxes[h].write(writer);
    }

    writer.flush();
}
