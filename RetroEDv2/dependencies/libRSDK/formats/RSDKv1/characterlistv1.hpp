#pragma once

namespace RSDKv1
{

class CharacterList
{
public:
    class PlayerInfo
    {
    public:
        QString displayName = "";
        QString playerName  = "";
        bool hasP2          = false;
        QString player1Anim = "Player.Ani";
        QString player2Anim = "NULL";

        PlayerInfo() {}

        PlayerInfo(Reader &reader) { read(reader); }

        void read(Reader &reader)
        {
            displayName = readString(reader);
            playerName  = readString(reader);
            hasP2       = readString(reader) == "2";
            player1Anim = readString(reader);
            player2Anim = readString(reader);
            if (!hasP2)
                player2Anim = "NULL";

            reader.readLine();
        }

        void write(Writer &writer)
        {
            writeString(writer, displayName);
            writeString(writer, playerName);
            writeString(writer, hasP2 ? "2" : "1");
            writeString(writer, player1Anim);
            writeString(writer, hasP2 ? player2Anim : "NULL");

            writer.write<char>('\r');
            writer.write<char>('\n');
        }

    private:
        QString readString(Reader &reader)
        {
            QString str = "";

            while (true) {
                char buf = (char)reader.read<char>();
                if (buf == '^')
                    break;
                else
                    str += buf;
            }

            return str;
        }

        void writeString(Writer &writer, QString str)
        {
            for (int i = 0; i < str.length(); i++) writer.write<char>(str[i].toLatin1());
            writer.write<char>('^');
        }
    };

    CharacterList() {}
    CharacterList(QString filename) { read(filename); }
    CharacterList(Reader &reader) { read(reader); }

    inline void read(QString filename)
    {
        Reader reader(filename);
        read(reader);
    }
    inline void read(Reader &reader)
    {
        filePath = reader.filePath;
        players.clear();
        while (!reader.isEOF()) players.append(PlayerInfo(reader));
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

        for (PlayerInfo &player : players) player.write(writer);

        writer.flush();
    }

    QList<PlayerInfo> players;

    QString filePath = "";
};

} // namespace RSDKv1


