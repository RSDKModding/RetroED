#ifndef VIDEO_V2_H
#define VIDEO_V2_H

namespace RSDKv2
{

class Video
{
public:
    Video() {}
    Video(QString filename) { read(filename); }
    Video(Reader &reader) { read(reader); }

    inline void read(QString filename)
    {
        Reader reader(filename);
        read(reader);
    }
    void read(Reader &reader);

    inline void write(QString filename)
    {
        if (filename == "")
            filename = filePath;
        if (filename == "")
            return;
        Writer writer(filename);
        write(writer);
    }
    void write(Writer &writer);

    QList<FormatHelpers::Gif> frames;

    ushort width  = 0x100;
    ushort height = 0x100;

    QString filePath = "";
};

} // namespace RSDKv2

#endif // VIDEO_V2_H
