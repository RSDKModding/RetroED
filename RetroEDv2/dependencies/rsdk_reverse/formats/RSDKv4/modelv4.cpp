#include "rsdkreverse.hpp"

void RSDKv4::Model::read(Reader &reader)
{
    filePath = reader.filePath;

    if (!reader.matchesSignature(signature, 4))
        return;

    ushort vertCount = reader.read<ushort>();

    texCoords.clear();
    for (int v = 0; v < vertCount; ++v) texCoords.append(TexCoord(reader));

    ushort indexCount = reader.read<ushort>();
    indices.clear();
    for (int i = 0; i < indexCount * 3; ++i) indices.append(reader.read<ushort>());

    frames.clear();
    ushort frameCount = reader.read<ushort>();
    for (int f = 0; f < frameCount; ++f) {
        Frame frame;
        for (int v = 0; v < vertCount; ++v) {
            Frame::Vertex vert;
            vert.x = reader.read<float>();
            vert.y = reader.read<float>();
            vert.z = reader.read<float>();

            vert.nx = reader.read<float>();
            vert.ny = reader.read<float>();
            vert.nz = reader.read<float>();
            frame.vertices.append(vert);
        }
        frames.append(frame);
    }
}

void RSDKv4::Model::write(Writer &writer)
{
    filePath = writer.filePath;
    writer.write(signature, 4);

    int vertCount = frames.count() >= 0 ? frames[0].vertices.count() : 0;
    writer.write((ushort)vertCount);

    for (int v = 0; v < vertCount; ++v) texCoords[v].write(writer);

    writer.write((ushort)(indices.count() / 3));
    for (int i = 0; i < indices.count(); ++i) writer.write(indices[i]);

    writer.write((ushort)frames.count());
    for (int f = 0; f < frames.count(); ++f) {
        for (int v = 0; v < vertCount; ++v) {
            writer.write(frames[f].vertices[v].x);
            writer.write(frames[f].vertices[v].y);
            writer.write(frames[f].vertices[v].z);

            writer.write(frames[f].vertices[v].nx);
            writer.write(frames[f].vertices[v].ny);
            writer.write(frames[f].vertices[v].nz);
        }
    }

    writer.flush();
}

void RSDKv4::Model::writeAsOBJ(QString filePath, int exportFrame)
{
    for (int f = (exportFrame < 0 ? 0 : exportFrame); f < frames.count(); ++f) {
        QString path    = filePath;
        QString extLess = path.replace("." + QFileInfo(filePath).suffix(), "");

        Writer writer(extLess + (frames.count() > 1 ? (" Frame " + QString::number(f) + ".") : ".")
                      + QFileInfo(filePath).suffix());

        // Object
        writer.writeLine("o RSDKModelv4");
        for (int v = 0; v < frames[f].vertices.count(); ++v)
            writer.writeLine(QString("v %1 %2 %3")
                                 .arg(frames[f].vertices[v].x)
                                 .arg(frames[f].vertices[v].y)
                                 .arg(frames[f].vertices[v].z));

        for (int v = 0; v < frames[f].vertices.count(); ++v)
            writer.writeLine(QString("vn %1 %2 %3")
                                 .arg(frames[f].vertices[v].nx)
                                 .arg(frames[f].vertices[v].ny)
                                 .arg(frames[f].vertices[v].nz));

        for (int v = 0; v < texCoords.count(); ++v)
            writer.writeLine(QString("vt %1 %2").arg(texCoords[v].x).arg(texCoords[v].y));

        writer.writeLine("usemtl None");
        writer.writeLine("s off");

        for (int v = 0; v < indices.count(); v += 3) {
            QList<ushort> verts;
            for (int ii = 0; ii < 3; ++ii) verts.append(indices[v + ii]);
            writer.writeLine(
                QString("f %1 %2 %3").arg(verts[0] + 1).arg(verts[1] + 1).arg(verts[2] + 1));
        }

        writer.flush();
    }
}
