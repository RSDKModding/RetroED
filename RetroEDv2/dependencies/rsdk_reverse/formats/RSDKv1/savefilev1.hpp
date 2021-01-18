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
            m_currentLevel = reader.read<byte>();
            m_emeralds     = reader.read<byte>();
            m_lives        = reader.read<byte>();
        }

        inline void write(Writer &writer)
        {
            writer.write(m_currentLevel);
            writer.write(m_emeralds);
            writer.write(m_lives);
        }

        byte m_currentLevel = 0;
        byte m_emeralds     = 0x0;
        byte m_lives        = 4;
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
        m_filename = reader.m_filepath;
        for (int i = 0; i < 10; ++i) m_saveFiles[i].read(reader);
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
        for (int i = 0; i < 10; ++i) m_saveFiles[i].write(writer);
        writer.flush();
    }

    SaveFileEntry m_saveFiles[10];

    QString m_filename = "";
};

} // namespace RSDKv1

#endif // SAVEFILE_VRS_H
