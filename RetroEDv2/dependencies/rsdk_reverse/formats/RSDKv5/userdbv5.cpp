#include "include.hpp"

void RSDKv5::UserDB::read(Reader &reader)
{
    m_filename     = reader.m_filepath;
    Reader creader = reader.getCReader();

    uint sig = creader.read<uint>();
    if (sig != signature) {
        // error
        creader.close();
        reader.close();
        return;
    }

    int dataSize = creader.read<int>(); // total size of the buffer (may not all be used)
    Q_UNUSED(dataSize);

    ushort rowCount = creader.read<ushort>();
    byte colCount   = creader.read<byte>();

    for (int i = 0; i < colCount; i++) columns.append(TableColumn(creader));

    for (int i = 0; i < rowCount; i++) rows.append(TableRow(creader, this));

    creader.close();
    reader.close();
}

void RSDKv5::UserDB::write(Writer &writer)
{
    m_filename = writer.filename();

    QByteArray compressed;
    QBuffer buffer(&compressed);
    buffer.open(QIODevice::Append);
    QDataStream *cmem = new QDataStream(&buffer);
    Writer cwriter(cmem);

    cwriter.write(signature);
    int loop = 0;
    while (loop++ < 2) {
        if (loop == 1)
            cwriter.write((uint)(compressed.size() - 4)); // data size (real)
        else
            cwriter.write(0x00); // data size (fake)

        cwriter.write((ushort)rows.count());
        cwriter.write((byte)columns.count());

        for (int i = 0; i < columns.count(); i++) columns[i].write(cwriter);

        for (int i = 0; i < rows.count(); i++) rows[i].write(cwriter);

        if (loop == 1)
            cwriter.seek(4);
    }

    writer.writeCompressed(compressed);
    writer.flush();
}
