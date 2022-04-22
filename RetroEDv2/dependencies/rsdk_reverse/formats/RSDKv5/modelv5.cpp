#include "rsdkreverse.hpp"

void RSDKv5::Model::read(Reader &reader)
{
    filePath = reader.filePath;

    if (!reader.matchesSignature(signature, 4))
        return;

    byte flags  = reader.read<byte>();
    hasNormals  = Utils::getBit(flags, 0);
    hasTextures = Utils::getBit(flags, 1);
    hasColors   = Utils::getBit(flags, 2);

    faceVerticesCount = reader.read<byte>();

    ushort vertCount  = reader.read<ushort>();
    ushort frameCount = reader.read<ushort>();

    colors.clear();
    if (hasColors) {
        for (int v = 0; v < vertCount; ++v) colors.append(Color(reader));
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
    Utils::setBit(flags, hasColors, 2);
    writer.write(flags);

    writer.write(faceVerticesCount);

    int vertCount = frames.count() ? frames[0].vertices.count() : 0;
    writer.write((ushort)vertCount);
    writer.write((ushort)frames.count());

    if (hasColors) {
        for (int v = 0; v < vertCount; ++v) colors[v].write(writer);
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
            writer.writeLine(QString("usemtl RSDKModelv5.Color.%1").arg(indices[v]));
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
    for (int i = 0; i < colors.count(); ++i) {
        writer.writeLine(QString("newmtl RSDKModelv5.Color.%1").arg(i));
        writer.writeLine(QString("kd %1 %2 %3")
                             .arg(colors[i].r / 255.0f)
                             .arg(colors[i].g / 255.0f)
                             .arg(colors[i].b / 255.0f));
    }
    writer.flush();
}

int RSDKv5::Model::getPLYPropertyType(QString propertyType)
{
    QString type = propertyType;
    type         = type.toLower();

    if (type == "char" || type == "int8") {
        return PLYPropTypes::INT8;
    }
    else if (type == "uchar" || type == "uint8") {
        return PLYPropTypes::UINT8;
    }
    else if (type == "short" || type == "int16") {
        return PLYPropTypes::INT16;
    }
    else if (type == "ushort" || type == "uint16") {
        return PLYPropTypes::UINT16;
    }
    else if (type == "int32" || type == "int") {
        return PLYPropTypes::INT32;
    }
    else if (type == "uint32" || type == "uint") {
        return PLYPropTypes::UINT32;
    }
    else if (type == "float" || type == "float32") {
        return PLYPropTypes::FLOAT;
    }
    else if (type == "double64" || type == "double" || type == "float64") {
        return PLYPropTypes::DOUBLE;
    }
    else if (type == "list") {
        return PLYPropTypes::LIST;
    }

    return PLYPropTypes::UNKNOWN;
}

void RSDKv5::Model::loadPLY(QString filePath)
{
    hasColors         = false;
    hasNormals        = false;
    hasTextures       = false;
    faceVerticesCount = 3;

    texCoords.clear();
    frames.clear();
    colors.clear();
    indices.clear();

    frames.append(Frame());

    Reader reader(filePath);

    QString signature = reader.readLine().toLower();
    if (signature != "ply")
        return;

    QStringList format = reader.readLine().split(" ");
    if (format.count() < 3 || format[0].toLower() != "format")
        return;

    bool binary    = format[1].toLower() != "ascii";
    bool bigEndian = format[1].toLower().contains("big_endian");

    QList<PLYElement> elements;

    while (!reader.isEOF()) {
        QStringList line = reader.readLine().split(" ");
        if (!line.count())
            continue;

        QString instruction = line[0].toLower();
        if (instruction == "end_header")
            break;

        if (instruction == "element") {
            PLYElement element;
            element.name = line[1];

            bool ok  = false;
            int size = line[2].toInt(&ok);
            if (ok) {
                element.propertyValues.clear();
                for (int i = 0; i < size; ++i) element.propertyValues.append(PLYPropertyValue());
            }

            elements.append(element);
        }

        if (instruction == "property") {
            if (!elements.count()) {
                // error
                continue;
            }

            PLYProperty property;
            property.type = line[1].toLower();
            if (property.type == "list") {
                property.sizeType = line[2].toLower();
                property.itemType = line[3].toLower();
                property.name     = line[4];
            }
            else {
                property.name = line[2];
            }

            elements.last().properties.append(property);
        }
    }

    if (binary) {
        if (bigEndian) {
            // TODO: this
            for (auto &element : elements) {

                // read each property value
            }
        }
        else {
            // little endian :)
            for (auto &element : elements) {

                // read each property value

                for (int i = 0; i < element.propertyValues.count(); ++i) {
                    int p = 0;
                    element.propertyValues[i].values.clear();

                    for (auto &property : element.properties) {

                        if (property.type == "list") {
                            int size = 0;
                            switch (getPLYPropertyType(property.sizeType)) {
                                default: break;
                                case PLYPropTypes::INT8: size = reader.read<sbyte>(); break;
                                case PLYPropTypes::UINT8: size = reader.read<byte>(); break;
                                case PLYPropTypes::INT16: size = reader.read<short>(); break;
                                case PLYPropTypes::UINT16: size = reader.read<ushort>(); break;
                                case PLYPropTypes::INT32: size = reader.read<int>(); break;
                                case PLYPropTypes::UINT32: size = (int)reader.read<uint>(); break;
                                case PLYPropTypes::FLOAT: size = (int)reader.read<float>(); break;
                                case PLYPropTypes::DOUBLE: size = (int)reader.read<double>(); break;
                            }

                            for (int s = 0; s < size; ++s) {
                                QVariant value = "";
                                switch (getPLYPropertyType(property.itemType)) {
                                    default: break;
                                    case PLYPropTypes::INT8: value = reader.read<sbyte>(); break;
                                    case PLYPropTypes::UINT8: value = reader.read<byte>(); break;
                                    case PLYPropTypes::INT16: value = reader.read<short>(); break;
                                    case PLYPropTypes::UINT16: value = reader.read<ushort>(); break;
                                    case PLYPropTypes::INT32: value = reader.read<int>(); break;
                                    case PLYPropTypes::UINT32: value = reader.read<uint>(); break;
                                    case PLYPropTypes::FLOAT: value = reader.read<float>(); break;
                                    case PLYPropTypes::DOUBLE: value = reader.read<double>(); break;
                                }

                                element.propertyValues[i].values.append(value);
                            }
                            break;
                        }
                        else {
                            QVariant value = "";
                            switch (getPLYPropertyType(property.type)) {
                                default: break;
                                case PLYPropTypes::INT8: value = reader.read<sbyte>(); break;
                                case PLYPropTypes::UINT8: value = reader.read<byte>(); break;
                                case PLYPropTypes::INT16: value = reader.read<short>(); break;
                                case PLYPropTypes::UINT16: value = reader.read<ushort>(); break;
                                case PLYPropTypes::INT32: value = reader.read<int>(); break;
                                case PLYPropTypes::UINT32: value = reader.read<uint>(); break;
                                case PLYPropTypes::FLOAT: value = reader.read<float>(); break;
                                case PLYPropTypes::DOUBLE: value = reader.read<double>(); break;
                            }

                            element.propertyValues[i].values.append(value);

                            ++p;
                        }
                    }
                }
            }
        }
    }
    else {
        // ascii
        for (auto &element : elements) {
            // assume each index is a property value
            QStringList line = reader.readLine().split(" ");

            for (int i = 0; i < element.propertyValues.count(); ++i) {
                int p = 0;
                for (auto &property : element.properties) {

                    if (property.type == "list") {
                        int size = 0;
                        bool ok  = false;
                        switch (getPLYPropertyType(property.sizeType)) {
                            default: break;
                            case PLYPropTypes::INT8: size = (sbyte)line[0].toInt(&ok); break;
                            case PLYPropTypes::UINT8: size = (byte)line[0].toInt(&ok); break;
                            case PLYPropTypes::INT16: size = (short)line[0].toInt(&ok); break;
                            case PLYPropTypes::UINT16: size = (ushort)line[0].toInt(&ok); break;
                            case PLYPropTypes::INT32: size = line[0].toInt(&ok); break;
                            case PLYPropTypes::UINT32: size = (int)line[0].toInt(&ok); break;
                            case PLYPropTypes::FLOAT: size = (int)line[0].toFloat(&ok); break;
                            case PLYPropTypes::DOUBLE: size = (int)line[0].toDouble(&ok); break;
                        }

                        for (int s = 0; s < size; ++s)
                            element.propertyValues[i].values.append(QVariant(line[1 + s]));

                        break;
                    }
                    else {
                        element.propertyValues[i].values.append(QVariant(line[p]));

                        ++p;
                    }
                }
            }
        }
    }

    for (auto &element : elements) {
        QString name = element.name.toLower();

        if (name == "vertex") {
            frames[0].vertices.clear();
            colors.clear();
            texCoords.clear();

            // Fill up all the blanks
            for (int v = 0; v < element.propertyValues.count(); ++v) {
                frames[0].vertices.append(Frame::Vertex());
                colors.append(Color());
                texCoords.append(TexCoord());
            }

            int p = 0;
            for (auto &property : element.properties) {
                QString name = property.name.toLower();
                int v        = 0;

                // VERTEX POSITION
                {
                    if (name == "x") {
                        for (auto &propertyValue : element.propertyValues) {
                            bool ok = false;
                            switch (getPLYPropertyType(property.type)) {
                                default: break;
                                case PLYPropTypes::INT8:
                                    frames[0].vertices[v].x = propertyValue.values[p].toInt(&ok);
                                    break;
                                case PLYPropTypes::UINT8:
                                    frames[0].vertices[v].x = propertyValue.values[p].toInt(&ok);
                                    break;
                                case PLYPropTypes::INT16:
                                    frames[0].vertices[v].x = propertyValue.values[p].toInt(&ok);
                                    break;
                                case PLYPropTypes::UINT16:
                                    frames[0].vertices[v].x = propertyValue.values[p].toInt(&ok);
                                    break;
                                case PLYPropTypes::INT32:
                                    frames[0].vertices[v].x = propertyValue.values[p].toInt(&ok);
                                    break;
                                case PLYPropTypes::UINT32:
                                    frames[0].vertices[v].x = propertyValue.values[p].toInt(&ok);
                                    break;
                                case PLYPropTypes::FLOAT:
                                    frames[0].vertices[v].x = propertyValue.values[p].toFloat(&ok);
                                    break;
                                case PLYPropTypes::DOUBLE:
                                    frames[0].vertices[v].x = propertyValue.values[p].toDouble(&ok);
                                    break;
                            }

                            ++v;
                        }
                    }

                    if (name == "y") {
                        for (auto &propertyValue : element.propertyValues) {
                            bool ok = false;
                            switch (getPLYPropertyType(property.type)) {
                                default: break;
                                case PLYPropTypes::INT8:
                                    frames[0].vertices[v].y = propertyValue.values[p].toInt(&ok);
                                    break;
                                case PLYPropTypes::UINT8:
                                    frames[0].vertices[v].y = propertyValue.values[p].toInt(&ok);
                                    break;
                                case PLYPropTypes::INT16:
                                    frames[0].vertices[v].y = propertyValue.values[p].toInt(&ok);
                                    break;
                                case PLYPropTypes::UINT16:
                                    frames[0].vertices[v].y = propertyValue.values[p].toInt(&ok);
                                    break;
                                case PLYPropTypes::INT32:
                                    frames[0].vertices[v].y = propertyValue.values[p].toInt(&ok);
                                    break;
                                case PLYPropTypes::UINT32:
                                    frames[0].vertices[v].y = propertyValue.values[p].toInt(&ok);
                                    break;
                                case PLYPropTypes::FLOAT:
                                    frames[0].vertices[v].y = propertyValue.values[p].toFloat(&ok);
                                    break;
                                case PLYPropTypes::DOUBLE:
                                    frames[0].vertices[v].y = propertyValue.values[p].toDouble(&ok);
                                    break;
                            }

                            ++v;
                        }
                    }

                    if (name == "z") {
                        for (auto &propertyValue : element.propertyValues) {
                            bool ok = false;
                            switch (getPLYPropertyType(property.type)) {
                                default: break;
                                case PLYPropTypes::INT8:
                                    frames[0].vertices[v].z = propertyValue.values[p].toInt(&ok);
                                    break;
                                case PLYPropTypes::UINT8:
                                    frames[0].vertices[v].z = propertyValue.values[p].toInt(&ok);
                                    break;
                                case PLYPropTypes::INT16:
                                    frames[0].vertices[v].z = propertyValue.values[p].toInt(&ok);
                                    break;
                                case PLYPropTypes::UINT16:
                                    frames[0].vertices[v].z = propertyValue.values[p].toInt(&ok);
                                    break;
                                case PLYPropTypes::INT32:
                                    frames[0].vertices[v].z = propertyValue.values[p].toInt(&ok);
                                    break;
                                case PLYPropTypes::UINT32:
                                    frames[0].vertices[v].z = propertyValue.values[p].toInt(&ok);
                                    break;
                                case PLYPropTypes::FLOAT:
                                    frames[0].vertices[v].z = propertyValue.values[p].toFloat(&ok);
                                    break;
                                case PLYPropTypes::DOUBLE:
                                    frames[0].vertices[v].z = propertyValue.values[p].toDouble(&ok);
                                    break;
                            }

                            ++v;
                        }
                    }
                }

                // VERTEX NORMALS
                {
                    if (name == "nx") {
                        if (element.propertyValues.count())
                            hasNormals = true;

                        for (auto &propertyValue : element.propertyValues) {
                            bool ok = false;
                            switch (getPLYPropertyType(property.type)) {
                                default: break;
                                case PLYPropTypes::INT8:
                                    frames[0].vertices[v].nx = propertyValue.values[p].toInt(&ok);
                                    break;
                                case PLYPropTypes::UINT8:
                                    frames[0].vertices[v].nx = propertyValue.values[p].toInt(&ok);
                                    break;
                                case PLYPropTypes::INT16:
                                    frames[0].vertices[v].nx = propertyValue.values[p].toInt(&ok);
                                    break;
                                case PLYPropTypes::UINT16:
                                    frames[0].vertices[v].nx = propertyValue.values[p].toInt(&ok);
                                    break;
                                case PLYPropTypes::INT32:
                                    frames[0].vertices[v].nx = propertyValue.values[p].toInt(&ok);
                                    break;
                                case PLYPropTypes::UINT32:
                                    frames[0].vertices[v].nx = propertyValue.values[p].toInt(&ok);
                                    break;
                                case PLYPropTypes::FLOAT:
                                    frames[0].vertices[v].nx = propertyValue.values[p].toFloat(&ok);
                                    break;
                                case PLYPropTypes::DOUBLE:
                                    frames[0].vertices[v].nx = propertyValue.values[p].toDouble(&ok);
                                    break;
                            }

                            ++v;
                        }
                    }

                    if (name == "ny") {
                        if (element.propertyValues.count())
                            hasNormals = true;

                        for (auto &propertyValue : element.propertyValues) {
                            bool ok = false;
                            switch (getPLYPropertyType(property.type)) {
                                default: break;
                                case PLYPropTypes::INT8:
                                    frames[0].vertices[v].ny = propertyValue.values[p].toInt(&ok);
                                    break;
                                case PLYPropTypes::UINT8:
                                    frames[0].vertices[v].ny = propertyValue.values[p].toInt(&ok);
                                    break;
                                case PLYPropTypes::INT16:
                                    frames[0].vertices[v].ny = propertyValue.values[p].toInt(&ok);
                                    break;
                                case PLYPropTypes::UINT16:
                                    frames[0].vertices[v].ny = propertyValue.values[p].toInt(&ok);
                                    break;
                                case PLYPropTypes::INT32:
                                    frames[0].vertices[v].ny = propertyValue.values[p].toInt(&ok);
                                    break;
                                case PLYPropTypes::UINT32:
                                    frames[0].vertices[v].ny = propertyValue.values[p].toInt(&ok);
                                    break;
                                case PLYPropTypes::FLOAT:
                                    frames[0].vertices[v].ny = propertyValue.values[p].toFloat(&ok);
                                    break;
                                case PLYPropTypes::DOUBLE:
                                    frames[0].vertices[v].ny = propertyValue.values[p].toDouble(&ok);
                                    break;
                            }

                            ++v;
                        }
                    }

                    if (name == "nz") {
                        if (element.propertyValues.count())
                            hasNormals = true;

                        for (auto &propertyValue : element.propertyValues) {
                            bool ok = false;
                            switch (getPLYPropertyType(property.type)) {
                                default: break;
                                case PLYPropTypes::INT8:
                                    frames[0].vertices[v].nz = propertyValue.values[p].toInt(&ok);
                                    break;
                                case PLYPropTypes::UINT8:
                                    frames[0].vertices[v].nz = propertyValue.values[p].toInt(&ok);
                                    break;
                                case PLYPropTypes::INT16:
                                    frames[0].vertices[v].nz = propertyValue.values[p].toInt(&ok);
                                    break;
                                case PLYPropTypes::UINT16:
                                    frames[0].vertices[v].nz = propertyValue.values[p].toInt(&ok);
                                    break;
                                case PLYPropTypes::INT32:
                                    frames[0].vertices[v].nz = propertyValue.values[p].toInt(&ok);
                                    break;
                                case PLYPropTypes::UINT32:
                                    frames[0].vertices[v].nz = propertyValue.values[p].toInt(&ok);
                                    break;
                                case PLYPropTypes::FLOAT:
                                    frames[0].vertices[v].nz = propertyValue.values[p].toFloat(&ok);
                                    break;
                                case PLYPropTypes::DOUBLE:
                                    frames[0].vertices[v].nz = propertyValue.values[p].toDouble(&ok);
                                    break;
                            }

                            ++v;
                        }
                    }
                }

                // VERTEX TEXTURE COORDS
                {
                    if (name == "u" || name == "s" || name == "tx" || name == "texture_u") {
                        if (element.propertyValues.count())
                            hasTextures = true;

                        for (auto &propertyValue : element.propertyValues) {
                            bool ok = false;
                            switch (getPLYPropertyType(property.type)) {
                                default: break;
                                case PLYPropTypes::INT8:
                                    texCoords[v].x = propertyValue.values[p].toInt(&ok);
                                    break;
                                case PLYPropTypes::UINT8:
                                    texCoords[v].x = propertyValue.values[p].toInt(&ok);
                                    break;
                                case PLYPropTypes::INT16:
                                    texCoords[v].x = propertyValue.values[p].toInt(&ok);
                                    break;
                                case PLYPropTypes::UINT16:
                                    texCoords[v].x = propertyValue.values[p].toInt(&ok);
                                    break;
                                case PLYPropTypes::INT32:
                                    texCoords[v].x = propertyValue.values[p].toInt(&ok);
                                    break;
                                case PLYPropTypes::UINT32:
                                    texCoords[v].x = propertyValue.values[p].toInt(&ok);
                                    break;
                                case PLYPropTypes::FLOAT:
                                    texCoords[v].x = propertyValue.values[p].toFloat(&ok);
                                    break;
                                case PLYPropTypes::DOUBLE:
                                    texCoords[v].x = propertyValue.values[p].toDouble(&ok);
                                    break;
                            }

                            ++v;
                        }
                    }

                    if (name == "v" || name == "t" || name == "ty" || name == "texture_v") {
                        if (element.propertyValues.count())
                            hasTextures = true;

                        for (auto &propertyValue : element.propertyValues) {
                            bool ok = false;
                            switch (getPLYPropertyType(property.type)) {
                                default: break;
                                case PLYPropTypes::INT8:
                                    texCoords[v].y = propertyValue.values[p].toInt(&ok);
                                    break;
                                case PLYPropTypes::UINT8:
                                    texCoords[v].y = propertyValue.values[p].toInt(&ok);
                                    break;
                                case PLYPropTypes::INT16:
                                    texCoords[v].y = propertyValue.values[p].toInt(&ok);
                                    break;
                                case PLYPropTypes::UINT16:
                                    texCoords[v].y = propertyValue.values[p].toInt(&ok);
                                    break;
                                case PLYPropTypes::INT32:
                                    texCoords[v].y = propertyValue.values[p].toInt(&ok);
                                    break;
                                case PLYPropTypes::UINT32:
                                    texCoords[v].y = propertyValue.values[p].toInt(&ok);
                                    break;
                                case PLYPropTypes::FLOAT:
                                    texCoords[v].y = propertyValue.values[p].toFloat(&ok);
                                    break;
                                case PLYPropTypes::DOUBLE:
                                    texCoords[v].y = propertyValue.values[p].toDouble(&ok);
                                    break;
                            }

                            ++v;
                        }
                    }
                }

                // VERTEX COLORS
                {
                    if (name == "r" || name == "red") {
                        if (element.propertyValues.count())
                            hasColors = true;

                        for (auto &propertyValue : element.propertyValues) {
                            bool ok = false;
                            switch (getPLYPropertyType(property.type)) {
                                default: break;
                                case PLYPropTypes::INT8:
                                    colors[v].r = propertyValue.values[p].toInt(&ok);
                                    break;
                                case PLYPropTypes::UINT8:
                                    colors[v].r = propertyValue.values[p].toInt(&ok);
                                    break;
                                case PLYPropTypes::INT16:
                                    colors[v].r = propertyValue.values[p].toInt(&ok);
                                    break;
                                case PLYPropTypes::UINT16:
                                    colors[v].r = propertyValue.values[p].toInt(&ok);
                                    break;
                                case PLYPropTypes::INT32:
                                    colors[v].r = propertyValue.values[p].toInt(&ok);
                                    break;
                                case PLYPropTypes::UINT32:
                                    colors[v].r = propertyValue.values[p].toInt(&ok);
                                    break;
                                case PLYPropTypes::FLOAT:
                                    colors[v].r = propertyValue.values[p].toFloat(&ok) * 255.0f;
                                    break;
                                case PLYPropTypes::DOUBLE:
                                    colors[v].r = propertyValue.values[p].toDouble(&ok) * 255.0f;
                                    break;
                            }

                            ++v;
                        }
                    }

                    if (name == "g" || name == "green") {
                        if (element.propertyValues.count())
                            hasColors = true;

                        for (auto &propertyValue : element.propertyValues) {
                            bool ok = false;
                            switch (getPLYPropertyType(property.type)) {
                                default: break;
                                case PLYPropTypes::INT8:
                                    colors[v].g = propertyValue.values[p].toInt(&ok);
                                    break;
                                case PLYPropTypes::UINT8:
                                    colors[v].g = propertyValue.values[p].toInt(&ok);
                                    break;
                                case PLYPropTypes::INT16:
                                    colors[v].g = propertyValue.values[p].toInt(&ok);
                                    break;
                                case PLYPropTypes::UINT16:
                                    colors[v].g = propertyValue.values[p].toInt(&ok);
                                    break;
                                case PLYPropTypes::INT32:
                                    colors[v].g = propertyValue.values[p].toInt(&ok);
                                    break;
                                case PLYPropTypes::UINT32:
                                    colors[v].g = propertyValue.values[p].toInt(&ok);
                                    break;
                                case PLYPropTypes::FLOAT:
                                    colors[v].g = propertyValue.values[p].toFloat(&ok) * 255.0f;
                                    break;
                                case PLYPropTypes::DOUBLE:
                                    colors[v].g = propertyValue.values[p].toDouble(&ok) * 255.0f;
                                    break;
                            }

                            ++v;
                        }
                    }

                    if (name == "b" || name == "blue") {
                        if (element.propertyValues.count())
                            hasColors = true;

                        for (auto &propertyValue : element.propertyValues) {
                            bool ok = false;
                            switch (getPLYPropertyType(property.type)) {
                                default: break;
                                case PLYPropTypes::INT8:
                                    colors[v].b = propertyValue.values[p].toInt(&ok);
                                    break;
                                case PLYPropTypes::UINT8:
                                    colors[v].b = propertyValue.values[p].toInt(&ok);
                                    break;
                                case PLYPropTypes::INT16:
                                    colors[v].b = propertyValue.values[p].toInt(&ok);
                                    break;
                                case PLYPropTypes::UINT16:
                                    colors[v].b = propertyValue.values[p].toInt(&ok);
                                    break;
                                case PLYPropTypes::INT32:
                                    colors[v].b = propertyValue.values[p].toInt(&ok);
                                    break;
                                case PLYPropTypes::UINT32:
                                    colors[v].b = propertyValue.values[p].toInt(&ok);
                                    break;
                                case PLYPropTypes::FLOAT:
                                    colors[v].b = propertyValue.values[p].toFloat(&ok) * 255.0f;
                                    break;
                                case PLYPropTypes::DOUBLE:
                                    colors[v].b = propertyValue.values[p].toDouble(&ok) * 255.0f;
                                    break;
                            }

                            ++v;
                        }
                    }

                    if (name == "a" || name == "alpha") {
                        if (element.propertyValues.count())
                            hasColors = true;

                        for (auto &propertyValue : element.propertyValues) {
                            bool ok = false;
                            switch (getPLYPropertyType(property.type)) {
                                default: break;
                                case PLYPropTypes::INT8:
                                    colors[v].a = propertyValue.values[p].toInt(&ok);
                                    break;
                                case PLYPropTypes::UINT8:
                                    colors[v].a = propertyValue.values[p].toInt(&ok);
                                    break;
                                case PLYPropTypes::INT16:
                                    colors[v].a = propertyValue.values[p].toInt(&ok);
                                    break;
                                case PLYPropTypes::UINT16:
                                    colors[v].a = propertyValue.values[p].toInt(&ok);
                                    break;
                                case PLYPropTypes::INT32:
                                    colors[v].a = propertyValue.values[p].toInt(&ok);
                                    break;
                                case PLYPropTypes::UINT32:
                                    colors[v].a = propertyValue.values[p].toInt(&ok);
                                    break;
                                case PLYPropTypes::FLOAT:
                                    colors[v].a = propertyValue.values[p].toFloat(&ok) * 255.0f;
                                    break;
                                case PLYPropTypes::DOUBLE:
                                    colors[v].a = propertyValue.values[p].toDouble(&ok) * 255.0f;
                                    break;
                            }

                            ++v;
                        }
                    }
                }

                ++p;
            }
            // read vertices
        }
    }

    for (auto &element : elements) {
        QString name = element.name.toLower();

        if (name == "face") {
            // read indices

            int p = 0;
            for (auto &property : element.properties) {
                if (property.type == "list")
                    break;
                ++p;
            }

            indices.clear();
            if (p != element.properties.count()) {
                for (auto &propertyValue : element.propertyValues) {
                    // they should all be the same, but in the 1% chance it's not, update it
                    faceVerticesCount = propertyValue.values.count();

                    for (int i = 0; i < faceVerticesCount; ++i) {
                        ushort index = 0;

                        bool ok = false;
                        switch (getPLYPropertyType(element.properties[p].itemType)) {
                            default: break;
                            case PLYPropTypes::INT8:
                                index = (sbyte)propertyValue.values[i].toInt(&ok);
                                break;
                            case PLYPropTypes::UINT8:
                                index = (byte)propertyValue.values[i].toInt(&ok);
                                break;
                            case PLYPropTypes::INT16:
                                index = (short)propertyValue.values[i].toInt(&ok);
                                break;
                            case PLYPropTypes::UINT16:
                                index = propertyValue.values[i].toInt(&ok);
                                break;
                            case PLYPropTypes::INT32:
                                index = (ushort)propertyValue.values[i].toInt(&ok);
                                break;
                            case PLYPropTypes::UINT32:
                                index = (ushort)propertyValue.values[i].toInt(&ok);
                                break;
                            case PLYPropTypes::FLOAT:
                                index = (ushort)propertyValue.values[i].toFloat(&ok);
                                break;
                            case PLYPropTypes::DOUBLE:
                                index = (ushort)propertyValue.values[i].toDouble(&ok);
                                break;
                        }

                        indices.append(index);
                    }
                }
            }
        }
    }
}

void RSDKv5::Model::writeAsPLY(QString filePath, int exportFrame)
{
    for (int f = (exportFrame < 0 ? 0 : exportFrame); f < frames.count(); ++f) {
        QString path    = filePath;
        QString extLess = path.replace("." + QFileInfo(filePath).suffix(), "");

        Writer writer(extLess + (frames.count() > 1 ? (" Frame " + QString::number(f) + ".") : ".")
                      + QFileInfo(filePath).suffix());

        bool binary = true;

        // Header
        writer.writeLine("ply");
        writer.writeLine(QString("format ") + (binary ? "binary_little_endian" : "ascii") + " 1.0");
        writer.writeLine("comment Created by RetroED");
        writer.writeLine("element vertex " + QString::number(frames[f].vertices.count()));
        writer.writeLine("property float x");
        writer.writeLine("property float y");
        writer.writeLine("property float z");
        if (hasNormals) {
            writer.writeLine("property float nx");
            writer.writeLine("property float ny");
            writer.writeLine("property float nz");
        }
        if (hasTextures) {
            writer.writeLine("property float u");
            writer.writeLine("property float v");
        }
        if (hasColors) {
            writer.writeLine("property uint8 red");
            writer.writeLine("property uint8 green");
            writer.writeLine("property uint8 blue");
            writer.writeLine("property uint8 alpha");
        }
        writer.writeLine("element face " + QString::number(indices.count() / faceVerticesCount));
        writer.writeLine("property list uint8 uint16 vertex_indices");
        writer.writeLine("end_header");

        if (binary) {
            for (int v = 0; v < frames[f].vertices.count(); ++v) {
                writer.write(frames[f].vertices[v].x);
                writer.write(frames[f].vertices[v].y);
                writer.write(frames[f].vertices[v].z);

                if (hasNormals) {
                    writer.write(frames[f].vertices[v].nx);
                    writer.write(frames[f].vertices[v].ny);
                    writer.write(frames[f].vertices[v].nz);
                }

                if (hasTextures) {
                    writer.write(texCoords[v].x);
                    writer.write(texCoords[v].y);
                }

                if (hasColors) {
                    writer.write(colors[v].r);
                    writer.write(colors[v].g);
                    writer.write(colors[v].b);
                    writer.write(colors[v].a);
                }
            }

            for (int i = 0; i < indices.count(); i += faceVerticesCount) {
                writer.write<byte>(faceVerticesCount);
                for (int ii = 0; ii < faceVerticesCount; ++ii) writer.write<ushort>(indices[i + ii]);
            }
        }
        else {
            for (int v = 0; v < frames[f].vertices.count(); ++v) {
                QString vertex = "";

                vertex += QString("%1 %2 %3")
                              .arg(frames[f].vertices[v].x)
                              .arg(frames[f].vertices[v].y)
                              .arg(frames[f].vertices[v].z);

                if (hasNormals) {
                    vertex += QString(" %1 %2 %3")
                                  .arg(frames[f].vertices[v].nx)
                                  .arg(frames[f].vertices[v].ny)
                                  .arg(frames[f].vertices[v].nz);
                }

                if (hasTextures) {
                    vertex += QString(" %1 %2").arg(texCoords[v].x).arg(texCoords[v].y);
                }

                if (hasColors) {
                    vertex += QString(" %1 %2 %3 %4")
                                  .arg(colors[v].r)
                                  .arg(colors[v].g)
                                  .arg(colors[v].b)
                                  .arg(colors[v].a);
                }

                writer.writeLine(vertex);
            }

            for (int i = 0; i < indices.count(); i += faceVerticesCount) {
                QString face = "";

                face += QString::number(faceVerticesCount) + " ";

                for (int ii = 0; ii < faceVerticesCount; ++ii) {
                    face += QString::number(indices[i + ii]);
                    if (ii + 1 < faceVerticesCount)
                        face += " ";
                }

                writer.writeLine(face);
            }
        }

        writer.flush();
    }
}
