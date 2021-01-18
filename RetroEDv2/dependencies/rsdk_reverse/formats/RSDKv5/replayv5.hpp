#ifndef REPLAY_V5_H
#define REPLAY_V5_H

namespace RSDKv5
{

class Replay
{
public:
    Replay() {}
    Replay(QString filename) { read(filename); }
    Replay(Reader &reader) { read(reader); }

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

#endif // REPLAY_V5_H
