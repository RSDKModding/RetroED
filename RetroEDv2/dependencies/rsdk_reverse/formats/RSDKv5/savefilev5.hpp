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
        m_filename = reader.m_filepath;
        for (int i = 0; i < 0x8000; ++i) m_saveRAM[i] = reader.read<int>();
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
    void write(Writer &writer)
    {
        m_filename = writer.m_filename;
        for (int i = 0; i < 0x8000; ++i) writer.write(m_saveRAM[i]);
        writer.flush();
    }

    int m_saveRAM[0x8000];

    QString m_filename = "";
};

} // namespace RSDKv5

#endif // SAVEFILE_V5_H
