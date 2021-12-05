#ifndef RSDKCONFIG_V5_H
#define RSDKCONFIG_V5_H

namespace RSDKv5
{

class RSDKConfig
{
public:
    class Variable
    {
    public:
        QString name  = "";
        QString type  = "";
        QString value = "";

        Variable(QString name = "", QString type = "", QString value = "")
        {
            name  = name;
            type  = type;
            value = value;
        }
        Variable(Reader &reader) { read(reader); }

        inline void read(Reader &reader)
        {
            name  = reader.readString();
            type  = reader.readString();
            value = reader.readString();
        }

        inline void write(Writer &writer)
        {
            writer.write(name);
            writer.write(type);
            writer.write(value);
        }
    };

    class Constant
    {
    public:
        QString name  = "";
        QString value = "";

        Constant() {}
        Constant(Reader &reader) { read(reader); }

        inline void read(Reader &reader)
        {
            name  = reader.readString();
            value = reader.readString();
        }

        inline void write(Writer &writer)
        {
            writer.write(name);
            writer.write(value);
        }
    };
    RSDKConfig() {}
    RSDKConfig(QString filename) { read(filename); }
    RSDKConfig(Reader &reader) { read(reader); }

    inline void read(QString filename)
    {
        Reader reader(filename);
        read(reader);
    }
    inline void read(Reader &reader)
    {
        filepath    = reader.filepath;
        byte vcount = reader.read<byte>();
        variables.clear();
        for (int v = 0; v < vcount; ++v) variables.append(Variable(reader));

        byte ccount = reader.read<byte>();
        constants.clear();
        for (int c = 0; c < ccount; ++c) constants.append(Constant(reader));
    }

    inline void write(QString filename)
    {
        if (filename == "")
            filename = filepath;
        if (filename == "")
            return;
        Writer writer(filename);
        write(writer);
    }
    inline void write(Writer &writer)
    {
        filepath = writer.filePath;
        writer.write((byte)variables.count());
        for (int v = 0; v < variables.count(); ++v) variables[v].write(writer);

        writer.write((byte)constants.count());
        for (int c = 0; c < constants.count(); ++c) constants[c].write(writer);

        writer.flush();
    }

    QList<Variable> variables;
    QList<Constant> constants;

    QString filepath = "";
};

} // namespace RSDKv5

#endif // RSDKCONFIG_V5_H
