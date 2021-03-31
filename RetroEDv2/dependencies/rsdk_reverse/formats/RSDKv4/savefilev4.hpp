#ifndef SAVEFILE_V4_H
#define SAVEFILE_V4_H

namespace RSDKv4
{

class SaveFile
{
public:
    SaveFile() { memset(m_saveRAM, 0, 0x2000 * sizeof(int)); }
    SaveFile(QString filename) { read(filename); }
    SaveFile(Reader &reader) { read(reader); }

    inline void read(QString filename)
    {
        Reader reader(filename);
        read(reader);
    }
    inline void read(Reader &reader)
    {
        m_filename = reader.m_filepath;
        for (int i = 0; i < 0x2000; ++i) m_saveRAM[i] = reader.read<int>();
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
        for (int i = 0; i < 0x2000; ++i) writer.write(m_saveRAM[i]);
        writer.flush();
    }

    int m_saveRAM[0x2000];

    QString m_filename = "";
};

} // namespace RSDKv4

#endif // SAVEFILE_V4_H
