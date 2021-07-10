#ifndef VIDEO_V3_H
#define VIDEO_V3_H

namespace RSDKv3
{

class Video
{
public:
    class VideoFrame
    {
    public:
        VideoFrame() {}
        VideoFrame(Reader &reader, RSDKv3::Video *vid) { read(reader, vid); }

        void read(Reader &reader, RSDKv3::Video *vid);
        void write(Writer &writer);

        QImage toImage();
        void fromImage(QImage img);

        ushort width      = 0x100;
        ushort height     = 0x100;
        uint nextFramePos = 0;

        ushort m_imageLeft   = 0;
        ushort m_imageTop    = 0;
        ushort m_imageWidth  = 0;
        ushort m_imageHeight = 0;
        uint interlaced      = 0;
        byte flags           = 0;

        QByteArray m_imageData;
        bool m_extendedCodeTable = false;
        QList<QColor> m_framePalette;
    };
    Video() {}
    Video(QString filename) { read(filename); }
    Video(Reader &reader) { read(reader); }

    inline void read(QString filename)
    {
        Reader reader(filename);
        read(reader);
    }
    inline void read(Reader &reader)
    {
        filepath          = reader.filepath;
        ushort frameCount = reader.read<ushort>();
        width             = reader.read<ushort>();
        height            = reader.read<ushort>();

        for (int f = 0; f < frameCount; ++f) frames.append(VideoFrame(reader, this));
    }

    inline void write(QString filename)
    {
        if (filename == "")
            filename = filepath;
        if (filename == "")
            return;
        Writer writer(filename);
        write(writer);
    }
    inline void write(Writer &writer)
    {
        filepath = writer.filePath;
        writer.write((ushort)frames.count());
        writer.write(width);
        writer.write(height);

        for (int f = 0; f < frames.count(); ++f) frames[f].write(writer);
        writer.flush();
    }

    QList<VideoFrame> frames;

    ushort width  = 0x100;
    ushort height = 0x100;

    QString filepath = "";
};

} // namespace RSDKv3

#endif // VIDEO_V3_H
