#ifndef SAVEFILE_V4_H
#define SAVEFILE_V4_H

namespace RSDKv4
{

class SaveFile
{
public:
    SaveFile() { memset(saveRAM, 0, 0x2000 * sizeof(int)); }
    SaveFile(QString filename) { read(filename); }
    SaveFile(Reader &reader) { read(reader); }

    inline void read(QString filename)
    {
        Reader reader(filename);
        read(reader);
    }
    inline void read(Reader &reader)
    {
        filePath = reader.filePath;
        for (int i = 0; i < 0x2000; ++i) saveRAM[i] = reader.read<int>();
    }

    inline void write(QString filename)
    {
        if (filename == "")
            filename = filePath;
        if (filename == "")
            return;
        Writer writer(filename);
        write(writer);
    }
    inline void write(Writer &writer)
    {
        filePath = writer.filePath;
        for (int i = 0; i < 0x2000; ++i) writer.write(saveRAM[i]);
        writer.flush();
    }

    int saveRAM[0x2000];

    QString filePath = "";
};

} // namespace RSDKv4

#endif // SAVEFILE_V4_H
