#include "libRSDK.hpp"

#include "animationv2.hpp"

QList<QString> animNamesv2 = QList<QString>{
    "Stopped",        "Waiting",       "Bored!",        "Looking Up",     "Looking Down",
    "Walking",        "Running",       "Skidding",      "Super Peel Out", "Spin Dash",
    "Jumping",        "Bouncing",      "Hurt",          "Dying",          "Drowning",
    "Life Icon",      "Fan Rotate",    "Breathing",     "Pushing",        "Flailing Left",
    "Flailing Right", "Sliding",       "Hanging",       "Dropping",       "Clinging On",
    "Flying",         "Flying Tired",  "Swimming",      "Swimming Tired", "Gliding Drop",
    "Gliding Stop",   "Climbing",      "Ledge Pull Up", "CorkScrew H",    "CorkScrew V",
    "Finish Pose",    "Animation #36", "Animation #37", "Animation #38",  "Animation #39",
    "Animation #40",  "Animation #41", "Animation #42", "Grabbing On",    "Animation #44",
    "Animation #45",  "Animation #46", "Animation #47", "Animation #48",  "Animation #49",
    "Animation #50",  "Animation #51",
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
            a < animNamesv2.count() ? animNamesv2[a] : ("Animation #" + QString::number(a));
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
