#ifndef SAVEFILE_VRS_H
#define SAVEFILE_VRS_H

namespace RSDKv1
{

class SaveFile
{
public:
    class SaveFileEntry
    {
    public:
        SaveFileEntry() {}
        SaveFileEntry(Reader &reader) { read(reader); }

        inline void read(Reader &reader)
        {
            stage    = reader.read<byte>();
            emeralds = reader.read<byte>();
            lives    = reader.read<byte>();
        }

        inline void write(Writer &writer)
        {
            writer.write(stage);
            writer.write(emeralds);
            writer.write(lives);
        }

        byte stage    = 0;
        byte emeralds = 0x0;
        byte lives    = 4;
    };

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
        filePath = reader.filePath;
        for (int i = 0; i < 10; ++i) saveFiles[i].read(reader);
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
        for (int i = 0; i < 10; ++i) saveFiles[i].write(writer);
        writer.flush();
    }

    SaveFileEntry saveFiles[10];

    QString filePath = "";
};

} // namespace RSDKv1

#endif // SAVEFILE_VRS_H
