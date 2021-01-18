#ifndef MODEL_V5_H
#define MODEL_V5_H

namespace RSDKv5
{

class Model
{
public:
    Model() {}
    Model(QString filename) { read(filename); }
    Model(Reader &reader) { read(reader); }

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

    byte m_signature[4] = { 'M', 'D', 'L', 0 };

    QString m_filename = "";
};

} // namespace RSDKv5

#endif // MODEL_V5_H
