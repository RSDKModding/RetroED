#ifndef STAGELIST_V1_H
#define STAGELIST_V1_H

namespace RSDKv1
{

class StageList
{
public:
    class StageInfo
    {
    public:
        QString name     = "STAGE";
        QString folder   = "Folder";
        QString id       = "1";
        bool highlighted = false;

        StageInfo() {}

        StageInfo(Reader &reader) { read(reader); }

        void read(Reader &reader)
        {
            name        = readString(reader);
            folder      = readString(reader);
            id          = readString(reader);
            highlighted = readString(reader) != "0";

            reader.readLine();
        }

        void write(Writer &writer)
        {
            writeString(writer, name);
            writeString(writer, folder);
            writeString(writer, id);
            writeString(writer, highlighted ? "1" : "0");

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

    StageList() {}
    StageList(QString filename) { read(filename); }
    StageList(Reader &reader) { read(reader); }

    inline void read(QString filename)
    {
        Reader reader(filename);
        read(reader);
    }
    inline void read(Reader &reader)
    {
        filePath = reader.filePath;
        stages.clear();
        while (!reader.isEOF()) stages.append(StageInfo(reader));
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

        for (StageInfo &stage : stages) stage.write(writer);

        writer.flush();
    }

    QList<StageInfo> stages;

    QString filePath = "";
};

} // namespace RSDKv1

#endif // STAGELIST_V1_H
