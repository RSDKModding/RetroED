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

        ushort m_width  = 0x100;
        ushort m_height = 0x100;
        uint m_filePos  = 0;

        ushort m_imageLeft   = 0;
        ushort m_imageTop    = 0;
        ushort m_imageWidth  = 0;
        ushort m_imageHeight = 0;
        uint m_interlaced    = 0;
        byte m_paletteType   = 0;

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
        m_filename        = reader.m_filepath;
        ushort frameCount = reader.read<ushort>();
        m_width           = reader.read<ushort>();
        m_height          = reader.read<ushort>();

        for (int f = 0; f < frameCount; ++f) m_frames.append(VideoFrame(reader, this));
    }

    inline void write(QString filename)
    {
        if (filename == "")
            filename = m_filename;
        if (filename == "")
            return;
        Writer writer(filename);
        write(writer);
    }
    inline void write(Writer &writer)
    {
        m_filename = writer.m_filename;
        writer.write((ushort)m_frames.count());
        writer.write(m_width);
        writer.write(m_height);

        for (int f = 0; f < m_frames.count(); ++f) m_frames[f].write(writer);
        writer.flush();
    }

    QList<VideoFrame> m_frames;

    ushort m_width  = 0x100;
    ushort m_height = 0x100;

    QString m_filename = "";
};

} // namespace RSDKv3

#endif // VIDEO_V3_H
