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

    int m_saveRAM[0x8000 / 4];

    QString m_filename = "";
};

} // namespace RSDKv5

#endif // SAVEFILE_V5_H
