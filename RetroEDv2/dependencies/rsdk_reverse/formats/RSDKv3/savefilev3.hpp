#ifndef SAVEFILE_V3_H
#define SAVEFILE_V3_H

namespace RSDKv3
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
        if (filePath == "")
            filePath = filename;
        if (filePath == "")
            return;
        Writer writer(filePath);
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

} // namespace RSDKv3

#endif // SAVEFILE_V3_H
