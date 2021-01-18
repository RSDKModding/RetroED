#ifndef VIDEO_V1_H
#define VIDEO_V1_H

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
            filename = m_filename;
        if (filename == "")
            return;
        Writer writer(filename);
        write(writer);
    }
    void write(Writer &writer);

    QString m_filename = "";
};

} // namespace RSDKv2

#endif // VIDEO_V1_H
