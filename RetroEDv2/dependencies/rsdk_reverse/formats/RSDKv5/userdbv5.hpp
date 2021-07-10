#ifndef USERDB_V5_H
#define USERDB_V5_H

namespace RSDKv5
{

class UserDB
{
public:
    static const uint signature = 0x80074B1E;

    class TimeStamp
    {
    public:
        int tm_sec;
        int tm_min;
        int tm_hour;
        int tm_mday;
        int tm_mon;
        int tm_year;
        int tm_wday;
        int tm_yday;
        int tm_isdst;

        TimeStamp() {}
        TimeStamp(Reader &reader) { read(reader); }

        void read(Reader &reader)
        {
            tm_sec   = reader.read<int>();
            tm_min   = reader.read<int>();
            tm_hour  = reader.read<int>();
            tm_mday  = reader.read<int>();
            tm_mon   = reader.read<int>();
            tm_year  = reader.read<int>();
            tm_wday  = reader.read<int>();
            tm_yday  = reader.read<int>();
            tm_isdst = reader.read<int>();
        }

        void write(Writer &writer)
        {
            writer.write(tm_sec);
            writer.write(tm_min);
            writer.write(tm_hour);
            writer.write(tm_mday);
            writer.write(tm_mon);
            writer.write(tm_year);
            writer.write(tm_wday);
            writer.write(tm_yday);
            writer.write(tm_isdst);
        }

        QString toString()
        {
            return QString("%1/%2/%3 at %4:%5:%6")
                .arg(tm_mday)
                .arg(tm_mon)
                .arg(tm_year + 1900)
                .arg(tm_hour)
                .arg(tm_min)
                .arg(tm_sec, 2, 10, QLatin1Char('0'));
        }
    };

    class TableColumn
    {
    public:
        QString name = "";
        byte type    = 0;

        TableColumn() {}
        TableColumn(Reader &reader) { read(reader); }

        void read(Reader &reader)
        {
            type = reader.read<byte>();
            name = QString(reader.readByteArray(0x10)).replace("\0", "");
        }

        void write(Writer &writer)
        {
            writer.write(type);

            int i = 0;
            for (; i < 0x10; ++i) writer.write<byte>(name[i].toLatin1());
            for (; i < 0x10; ++i) writer.write<byte>(0x00);
        }
    };

    class TableRow
    {
    public:
        struct Value {
            QByteArray data;
        };

        uint uuid            = 0;
        TimeStamp timeStampA = TimeStamp();
        TimeStamp timeStampB = TimeStamp();
        QList<Value> entries = QList<Value>();

        TableRow() {}
        TableRow(Reader &reader, UserDB *db) { read(reader, db); }

        void read(Reader &reader, UserDB *db)
        {
            uuid = reader.read<uint>();
            timeStampA.read(reader);
            timeStampB.read(reader);

            for (int v = 0; v < db->columns.count(); ++v) {
                Value entry;
                entry.data = reader.readByteArray(reader.read<byte>());
                entries.append(entry);
            }
        }

        void write(Writer &writer)
        {
            writer.write(uuid);
            timeStampA.write(writer);
            timeStampB.write(writer);

            for (int v = 0; v < entries.count(); ++v) {
                writer.write((byte)entries[v].data.size());
                writer.write(entries[v].data);
            }
        }
    };

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
            filename = filepath;
        if (filename == "")
            return;
        Writer writer(filename);
        write(writer);
    }
    void write(Writer &writer);

    QList<TableColumn> columns = QList<TableColumn>();
    QList<TableRow> rows       = QList<TableRow>();

    QString filepath = "";
};

} // namespace RSDKv5

#endif // USERDB_V5_H
