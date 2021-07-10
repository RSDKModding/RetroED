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
        QString m_name  = "";
        QString m_type  = "";
        QString m_value = "";

        Variable(QString name = "", QString type = "", QString value = "")
        {
            m_name  = name;
            m_type  = type;
            m_value = value;
        }
        Variable(Reader &reader) { read(reader); }

        inline void read(Reader &reader)
        {
            m_name  = reader.readString();
            m_type  = reader.readString();
            m_value = reader.readString();
        }

        inline void write(Writer &writer)
        {
            writer.write(m_name);
            writer.write(m_type);
            writer.write(m_value);
        }
    };

    class Constant
    {
    public:
        QString m_name  = "";
        QString m_value = "";

        Constant() {}
        Constant(Reader &reader) { read(reader); }

        inline void read(Reader &reader)
        {
            m_name  = reader.readString();
            m_value = reader.readString();
        }

        inline void write(Writer &writer)
        {
            writer.write(m_name);
            writer.write(m_value);
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
        m_variables.clear();
        for (int v = 0; v < vcount; ++v) m_variables.append(Variable(reader));

        byte ccount = reader.read<byte>();
        m_constants.clear();
        for (int c = 0; c < ccount; ++c) m_constants.append(Constant(reader));
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
        writer.write((byte)m_variables.count());
        for (int v = 0; v < m_variables.count(); ++v) m_variables[v].write(writer);

        writer.write((byte)m_constants.count());
        for (int c = 0; c < m_constants.count(); ++c) m_constants[c].write(writer);

        writer.flush();
    }

    QList<Variable> m_variables;
    QList<Constant> m_constants;

    QString filepath = "";
};

} // namespace RSDKv5

#endif // RSDKCONFIG_V5_H
