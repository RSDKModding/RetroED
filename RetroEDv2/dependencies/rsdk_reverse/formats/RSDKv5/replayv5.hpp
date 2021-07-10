#ifndef REPLAY_V5_H
#define REPLAY_V5_H

namespace RSDKv5
{

class Replay
{
public:
    static const uint signature = 0xF6057BED;

    class ReplayEntry
    {
    public:
        byte state       = 0;
        byte changedVars = 0;
        byte inputs      = 0;
        byte direction   = 0;
        int posX         = 0;
        int posY         = 0;
        int velX         = 0;
        int velY         = 0;
        int rotation     = 0;
        byte anim        = 0;
        byte frame       = 0;
        byte unknown2    = 0;
        byte unknown3    = 0;

        ReplayEntry() {}
    };

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
            filename = filepath;
        if (filename == "")
            return;
        Writer writer(filename);
        write(writer);
    }
    void write(Writer &writer);

    // Header
    int unknown1    = 0;
    bool packed     = false;
    int frameCount  = 0;
    int zone        = 0;
    int act         = 0;
    int player      = 0;
    bool plusLayout = false;
    int oscillation = 0;
    int unknown3    = 0;

    QList<ReplayEntry> entries = QList<ReplayEntry>();

    QString filepath = "";
};

} // namespace RSDKv5

#endif // REPLAY_V5_H
