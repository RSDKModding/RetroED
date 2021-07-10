#include "include.hpp"

void RSDKv5::Replay::read(Reader &reader)
{
    filepath       = reader.filepath;
    Reader creader = reader.getCReader();

    uint sig = creader.read<uint>();
    if (sig != signature) {
        // error
        creader.close();
        reader.close();
        return;
    }

    unknown1             = creader.read<int>();
    packed               = creader.read<int>() != 0;
    bool active          = creader.read<int>() != 0;
    int entryCount       = creader.read<int>();
    int frameCount       = creader.read<int>();
    zone                 = creader.read<int>();
    act                  = creader.read<int>();
    player               = creader.read<int>();
    plusLayout           = creader.read<int>() != 0;
    oscillation          = creader.read<int>();
    int compressedSize   = creader.read<int>();
    int uncompressedSize = creader.read<int>();
    unknown3             = creader.read<int>();

    Q_UNUSED(active);
    Q_UNUSED(compressedSize);
    Q_UNUSED(uncompressedSize);

    for (int f = 0; f < frameCount; ++f) {
        for (int i = 0; i < entryCount; ++i) {
            ReplayEntry entry;
            if (packed) {
                if (entries.count() > 0)
                    entry = entries[entries.count() - 1];
                entry.state  = creader.read<byte>();
                byte changed = creader.read<byte>();

                bool flag = entry.state == 1 || entry.state == 3;

                if ((changed & 0x01) != 0 || flag) {
                    entry.inputs = creader.read<byte>();
                }
                if ((changed & 0x02) != 0 || flag) {
                    entry.posX = creader.read<int>();
                    entry.posY = creader.read<int>();
                }
                if ((changed & 0x04) != 0 || flag) {
                    entry.velX = creader.read<int>();
                    entry.velY = creader.read<int>();
                }
                if ((changed & 0x08) != 0 /*|| flag*/) {
                }

                if ((changed & 0x20) != 0 || flag) {
                    entry.rotation = creader.read<byte>() * 2;
                }
                if ((changed & 0x10) != 0 || flag) {
                    entry.direction = creader.read<byte>();
                }
                if ((changed & 0x40) != 0 || flag) {
                    entry.anim = creader.read<byte>();
                }
                if ((changed & 0x80) != 0 || flag) {
                    entry.frame = creader.read<byte>();
                }
                entries.append(entry);
            }
            else {
                entry.state       = creader.read<byte>();
                entry.changedVars = creader.read<byte>();
                entry.inputs      = creader.read<byte>();
                entry.direction   = creader.read<byte>();
                entry.posX        = creader.read<int>();
                entry.posY        = creader.read<int>();
                entry.velX        = creader.read<int>();
                entry.velY        = creader.read<int>();
                entry.rotation    = creader.read<int>();
                entry.anim        = creader.read<byte>();
                entry.frame       = creader.read<byte>();
                entry.unknown2    = creader.read<byte>();
                entry.unknown3    = creader.read<byte>();
                entries.append(entry);
            }
        }
    }

    creader.close();
    reader.close();
}

void RSDKv5::Replay::write(Writer &writer) { filepath = writer.filePath; }
