#include "include.hpp"

void RSDKv5::Replay::read(Reader &reader)
{
    filepath       = reader.filepath;
    Reader creader = reader.getCReaderRaw();

    uint sig = creader.read<uint>();
    if (sig != signature) {
        // error
        creader.close();
        reader.close();
        return;
    }

    gameVer         = creader.read<int>();
    isPacked        = creader.read<int>() != 0;
    bool isNotEmpty = creader.read<int>() != 0;
    int frameCount  = creader.read<int>();
    startingFrame   = creader.read<int>();
    zoneID          = creader.read<int>();
    act             = creader.read<int>();
    characterID     = creader.read<int>();
    isPlusLayout    = creader.read<int>() != 0;
    oscillation     = creader.read<int>();
    int bufferSize  = creader.read<int>();
    unknown1        = creader.read<float>();
    unknown2        = creader.read<int>();

    Q_UNUSED(isNotEmpty);
    Q_UNUSED(startingFrame);
    Q_UNUSED(bufferSize);
    Q_UNUSED(unknown1);
    Q_UNUSED(unknown2);

    for (int f = 0; f < frameCount; ++f) {
        ReplayEntry entry;
        entry.unpack(creader, isPacked);
        entries.append(entry);
    }

    creader.close();
    reader.close();
}

void RSDKv5::Replay::write(Writer &writer)
{
    filepath = writer.filePath;

    QByteArray compressed;
    QBuffer buffer(&compressed);
    buffer.open(QIODevice::Append);
    QDataStream *cmem = new QDataStream(&buffer);
    Writer cwriter(cmem);

    QByteArray frameBuffer;
    QBuffer fbuffer(&frameBuffer);
    fbuffer.open(QIODevice::Append);
    QDataStream *fmem = new QDataStream(&fbuffer);
    Writer fwriter(fmem);

    int bufferSize = 0;
    for (int f = 0; f < entries.count(); ++f) {
        bufferSize += entries[f].pack(fwriter, isPacked);
    }
    fwriter.flush();

    if (!isPacked) {
        bufferSize = 28 * (entries.count() + 2);
    }

    cwriter.write<uint>(signature);
    cwriter.write<int>(gameVer);
    cwriter.write<bool32>(isPacked);
    cwriter.write<bool32>(true);
    cwriter.write<int>(entries.count());
    cwriter.write<int>(startingFrame);
    cwriter.write<int>(zoneID);
    cwriter.write<int>(act);
    cwriter.write<int>(characterID);
    cwriter.write<bool32>(isPlusLayout);
    cwriter.write<int>(oscillation);
    cwriter.write<int>(bufferSize);
    cwriter.write<float>(unknown1);
    cwriter.write<int>(unknown2);
    cwriter.write(frameBuffer);
    cwriter.flush();

    writer.writeCompressedRaw(compressed);
    writer.flush();
}

int RSDKv5::Replay::ReplayEntry::unpack(Reader &reader, bool isPacked)
{
    int pos = reader.tell();

    info  = reader.read<byte>();
    flags = reader.read<byte>();
    if (isPacked) {

        bool flag = info == 1 || info == 3;

        if ((flags & 0x01) || flag)
            inputs = reader.read<byte>();

        if ((flags & 0x02) || flag) {
            position.x = reader.read<int>();
            position.y = reader.read<int>();
        }
        if ((flags & 0x04) || flag) {
            velocity.x = reader.read<int>();
            velocity.y = reader.read<int>();
        }

        if ((flags & 0x20) || flag)
            rotation = reader.read<byte>() << 1;

        if ((flags & 0x10) || flag)
            direction = reader.read<byte>();

        if ((flags & 0x40) || flag)
            anim = reader.read<byte>();

        if ((flags & 0x80) || flag)
            frame = reader.read<byte>();
    }
    else {
        inputs     = reader.read<byte>();
        position.x = reader.read<int>();
        position.y = reader.read<int>();
        velocity.x = reader.read<int>();
        velocity.y = reader.read<int>();
        rotation   = reader.read<int>();
        direction  = reader.read<byte>();
        anim       = reader.read<byte>();
        frame      = reader.read<byte>();
    }
    return reader.tell() - pos;
}
int RSDKv5::Replay::ReplayEntry::pack(Writer &writer, bool isPacked)
{
    int pos = writer.tell();

    writer.write(info);

    bool32 flag = info == 1 || info == 3;
    writer.write(flags);

    if (isPacked) {
        if (flag || (flags & 0x01))
            writer.write(inputs);

        if (flag || (flags & 0x02)) {
            writer.write(position.x);
            writer.write(position.y);
        }

        if (flag || (flags & 0x04)) {
            writer.write(velocity.x);
            writer.write(velocity.y);
        }

        if (flag || (flags & 0x20))
            writer.write((byte)(rotation >> 1));

        if (flag || (flags & 0x10))
            writer.write(direction);

        if (flag || (flags & 0x40))
            writer.write(anim);

        if (flag || (flags & 0x80))
            writer.write(frame);
    }
    else {
        writer.write(inputs);
        writer.write(position.x);
        writer.write(position.y);
        writer.write(velocity.x);
        writer.write(velocity.y);
        writer.write(rotation);
        writer.write(direction);
        writer.write(anim);
        writer.write(frame);
    }

    return writer.tell() - pos;
}
