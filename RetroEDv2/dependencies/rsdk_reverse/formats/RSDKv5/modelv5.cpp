#include "rsdkreverse.hpp"

void RSDKv5::Model::read(Reader &reader)
{
    filePath = reader.filePath;

    if (!reader.matchesSignature(signature, 4))
        return;

    byte flags  = reader.read<byte>();
    hasNormals  = Utils::getBit(flags, 0);
    hasTextures = Utils::getBit(flags, 1);
    hasColours  = Utils::getBit(flags, 2);

    faceVerticesCount = reader.read<byte>();

    ushort vertCount  = reader.read<ushort>();
    ushort frameCount = reader.read<ushort>();

    colours.clear();
    if (hasColours) {
        for (int v = 0; v < vertCount; ++v) colours.append(Colour(reader));
    }

    texCoords.clear();
    if (hasTextures) {
        for (int v = 0; v < vertCount; ++v) texCoords.append(TexCoord(reader));
    }

    ushort indexCount = reader.read<ushort>();
    indices.clear();
    for (int i = 0; i < indexCount; ++i) indices.append(reader.read<ushort>());

    frames.clear();
    for (int f = 0; f < frameCount; ++f) {
        Frame frame;
        for (int v = 0; v < vertCount; ++v) {
            Frame::Vertex vert;
            vert.x = reader.read<float>();
            vert.y = reader.read<float>();
            vert.z = reader.read<float>();

            if (hasNormals) {
                vert.nx = reader.read<float>();
                vert.ny = reader.read<float>();
                vert.nz = reader.read<float>();
            }
            frame.vertices.append(vert);
        }
        frames.append(frame);
    }
}

void RSDKv5::Model::write(Writer &writer)
{
    filePath = writer.filePath;
    writer.write(signature, 4);

    byte flags = 0;
    Utils::setBit(flags, hasNormals, 0);
    Utils::setBit(flags, hasTextures, 1);
    Utils::setBit(flags, hasColours, 2);
    writer.write(flags);

    writer.write(faceVerticesCount);

    int vertCount = frames.count() ? frames[0].vertices.count() : 0;
    writer.write((ushort)vertCount);
    writer.write((ushort)frames.count());

    if (hasColours) {
        for (int v = 0; v < vertCount; ++v) colours[v].write(writer);
    }

    if (hasTextures) {
        for (int v = 0; v < vertCount; ++v) texCoords[v].write(writer);
    }

    writer.write((ushort)indices.count());
    for (int i = 0; i < indices.count(); ++i) writer.write(indices[i]);

    for (int f = 0; f < frames.count(); ++f) {
        Frame frame;
        for (int v = 0; v < vertCount; ++v) {
            writer.write(frames[f].vertices[v].x);
            writer.write(frames[f].vertices[v].y);
            writer.write(frames[f].vertices[v].z);
            if (hasNormals) {
                writer.write(frames[f].vertices[v].nx);
                writer.write(frames[f].vertices[v].ny);
                writer.write(frames[f].vertices[v].nz);
            }
        }
    }

    writer.flush();
}

void RSDKv5::Model::writeAsOBJ(QString filePath, int exportFrame)
{
    for (int f = (exportFrame < 0 ? 0 : exportFrame); f < frames.count(); ++f) {
        QString path    = filePath;
        QString extLess = path.replace("." + QFileInfo(filePath).suffix(), "");

        Writer writer(extLess + (frames.count() > 1 ? (" Frame " + QString::number(f) + ".") : ".")
                      + QFileInfo(filePath).suffix());

        // Link Material
        writer.writeLine("mtllib " + QFileInfo(filePath).baseName() + ".mtl");
        // Object
        writer.writeLine("o RSDKModelv5");
        for (int v = 0; v < frames[f].vertices.count(); ++v)
            writer.writeLine(QString("v %1 %2 %3")
                                 .arg(frames[f].vertices[v].x)
                                 .arg(frames[f].vertices[v].y)
                                 .arg(frames[f].vertices[v].z));

        if (!hasNormals) {
            writer.writeLine("vn 0.0000 1.0000 0.0000");
        }
        else {
            for (int v = 0; v < frames[f].vertices.count(); ++v)
                writer.writeLine(QString("vn %1 %2 %3")
                                     .arg(frames[f].vertices[v].nx)
                                     .arg(frames[f].vertices[v].ny)
                                     .arg(frames[f].vertices[v].nz));
        }

        writer.writeLine("usemtl None");
        writer.writeLine("s off");

        if (!hasTextures) {
            writer.writeLine("vt 0.0000 0.0000");
        }
        else {
            for (int v = 0; v < texCoords.count(); ++v)
                writer.writeLine(QString("vt %1 %2").arg(texCoords[v].x).arg(texCoords[v].y));
        }

        for (int v = 0; v < indices.count(); v += faceVerticesCount) {
            QList<ushort> verts;
            for (int ii = 0; ii < faceVerticesCount; ++ii) verts.append(indices[v + ii]);
            writer.writeLine(QString("usemtl RSDKModelv5.Colour.%1").arg(indices[v]));
            writer.writeLine(
                QString("f %1 %2 %3").arg(verts[0] + 1).arg(verts[1] + 1).arg(verts[2] + 1));
            if (faceVerticesCount == 4)
                writer.writeLine(
                    QString("f %1 %2 %3").arg(verts[0] + 1).arg(verts[2] + 1).arg(verts[3] + 1));
        }

        writeMTL(extLess + (frames.count() > 1 ? (" Frame " + QString::number(f)) : " ") + ".mtl");

        writer.flush();
    }
}

void RSDKv5::Model::writeMTL(QString filepath)
{
    Writer writer(filepath);
    for (int i = 0; i < colours.count(); ++i) {
        writer.writeLine(QString("newmtl RSDKModelv5.Colour.%1").arg(i));
        writer.writeLine(QString("kd %1 %2 %3")
                             .arg(colours[i].r / 255.0f)
                             .arg(colours[i].g / 255.0f)
                             .arg(colours[i].b / 255.0f));
    }
    writer.flush();
}
