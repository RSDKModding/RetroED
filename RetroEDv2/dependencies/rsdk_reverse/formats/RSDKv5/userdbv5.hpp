#ifndef USERDB_V5_H
#define USERDB_V5_H

namespace RSDKv5
{

class UserDB
{
public:
    UserDB() {}
    UserDB(QString filename) { read(filename); }
    UserDB(Reader &reader) { read(reader); }

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

    QString m_filename = "";
};

} // namespace RSDKv5

#endif // USERDB_V5_H
