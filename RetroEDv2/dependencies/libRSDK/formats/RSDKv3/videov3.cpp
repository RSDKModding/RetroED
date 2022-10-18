#include "libRSDK.hpp"

#include "videov3.hpp"

void RSDKv3::Video::read(Reader &reader)
{
    filePath = reader.filePath;

    ushort frameCount = reader.read<ushort>();
    width             = reader.read<ushort>();
    height            = reader.read<ushort>();

    int videoFilePos = (int)reader.tell();
    frames.clear();
    for (int f = 0; f < frameCount; ++f) {
        uint frameSize = reader.read<uint>();

        FormatHelpers::Gif frame;
        frame.width  = width;
        frame.height = height;
        frame.read(reader, true, 0x80);

        frames.append(frame);

        videoFilePos += frameSize;
        reader.seek(videoFilePos);
    }
}

void RSDKv3::Video::write(Writer &writer)
{
    filePath = writer.filePath;

    writer.write<ushort>(frames.count());
    writer.write(width);
    writer.write(height);

    for (FormatHelpers::Gif frame : frames) {

        QByteArray gifData;
        QBuffer sbuffer(&gifData);
        sbuffer.open(QIODevice::Append);
        QDataStream *smem = new QDataStream(&sbuffer);
        Writer swriter(smem);

        frame.write(swriter, true, true);
        swriter.flush();

        writer.write<uint>(gifData.count() + 4);
        writer.write(gifData);
    }
    writer.flush();
}
