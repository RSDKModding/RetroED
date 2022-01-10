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
        byte info             = 0;
        byte flags            = 0;
        byte inputs           = 0;
        byte direction        = 0;
        Vector2<int> position = Vector2<int>(0, 0);
        Vector2<int> velocity = Vector2<int>(0, 0);
        int rotation          = 0;
        byte anim             = 0;
        byte frame            = 0;

        ReplayEntry() {}

        int unpack(Reader &reader, bool isPacked);
        int pack(Writer &writer, bool isPacked);
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
            filename = filePath;
        if (filename == "")
            return;
        Writer writer(filename);
        write(writer);
    }
    void write(Writer &writer);

    // Header
    int gameVer       = 0;
    bool isPacked     = false;
    int startingFrame = 1;
    int zoneID        = 0;
    int act           = 0;
    int characterID   = 0;
    bool isPlusLayout = false;
    int oscillation   = 0;
    float unknown1    = 0;
    int unknown2      = 0;

    QList<ReplayEntry> frames = QList<ReplayEntry>();

    QString filePath = "";
};

} // namespace RSDKv5

#endif // REPLAY_V5_H
