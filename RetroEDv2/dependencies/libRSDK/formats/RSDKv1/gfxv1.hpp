#ifndef GFX_VRS_H
#define GFX_VRS_H

namespace RSDKv1
{

class GFX
{
public:
    GFX() {}
    GFX(QString filename) { read(filename); }
    GFX(Reader &reader) { read(reader); }

    inline void read(QString filename, bool dcGFX = false)
    {
        Reader reader(filename);
        read(reader, dcGFX);
    }
    void read(Reader &reader, bool dcGFX = false);

    inline void write(QString filename, bool dcGFX = false)
    {
        if (filename == "")
            filename = filePath;
        if (filename == "")
            return;
        Writer writer(filename);
        write(writer, dcGFX);
    }
    void write(Writer &writer, bool dcGFX = false);

    void importImage(QImage image);
    void importImage(FormatHelpers::Gif image);

    QImage exportImage();
    FormatHelpers::Gif exportGif();

    Color palette[255];
    ushort width  = 16;
    ushort height = 16;
    QByteArray pixels;

    QString filePath = "";
};

} // namespace RSDKv1

#endif // GFX_VRS_H
