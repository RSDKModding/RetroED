#include "libRSDK.hpp"

#include "userdbv5.hpp"

void RSDKv5::UserDB::read(Reader &reader)
{
    filePath       = reader.filePath;
    Reader creader = reader.getCReaderRaw();

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
    filePath = writer.filePath;

    QByteArray compressed;
    QBuffer buffer(&compressed);
    buffer.open(QIODevice::Append);
    QDataStream *cmem = new QDataStream(&buffer);
    Writer cwriter(cmem);

    // entry limits
    ushort rowCount = rows.count();
    if (rowCount > 0x400)
        rowCount = 0x400;

    byte colCount = columns.count();
    if (colCount > 8)
        colCount = 8;

    cwriter.write(signature);
    int loop = 0;
    while (loop++ < 2) {
        if (loop == 2)
            cwriter.write((uint)(compressed.size() - 4)); // data size (real)
        else
            cwriter.write(0x00); // data size (fake)

        cwriter.write(rowCount);
        cwriter.write(colCount);

        for (int c = 0; c < colCount; c++) columns[c].write(cwriter);

        for (int r = 0; r < rowCount; r++) rows[r].write(cwriter);

        if (loop == 1)
            cwriter.seek(4);
    }

    writer.writeCompressedRaw(compressed);
    writer.flush();
}
