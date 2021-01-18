#ifndef GFX_V1_H
#define GFX_V1_H

namespace RSDKv2
{

class GFX
{
public:
    GFX() {}
    GFX(QString filename) { read(filename); }
    GFX(Reader &reader) { read(reader); }

    inline void read(QString filename)
    {
        Reader reader(filename);
        read(reader);
    }
    void read(Reader &reader, bool dcGFX = false);

    inline void write(QString filename)
    {
        if (filename == "")
            filename = m_filename;
        if (filename == "")
            return;
        Writer writer(filename);
        write(writer);
    }
    void write(Writer &writer, bool dcGFX = false);

    void importImage(QImage image);
    QImage exportImage();

    Colour m_palette[255];
    ushort m_width  = 16;
    ushort m_height = 16;
    QByteArray m_pixelData;

    QString m_filename = "";
};

} // namespace RSDKv2

#endif // GFX_V1_H
