#ifndef SAVEFILE_V5_H
#define SAVEFILE_V5_H

namespace RSDKv5
{

class SaveFile
{
public:
    SaveFile() {}
    SaveFile(QString filename) { read(filename); }
    SaveFile(Reader &reader) { read(reader); }

    inline void read(QString filename)
    {
        Reader reader(filename);
        read(reader);
    }
    void read(Reader &reader)
    {
        filepath = reader.filepath;
        for (int i = 0; i < 0x8000; ++i) saveRAM[i] = reader.read<int>();
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
    void write(Writer &writer)
    {
        filepath = writer.filePath;
        for (int i = 0; i < 0x8000; ++i) writer.write(saveRAM[i]);
        writer.flush();
    }

    int saveRAM[0x8000];

    QString filepath = "";
};

} // namespace RSDKv5

#endif // SAVEFILE_V5_H
