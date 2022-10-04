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

        for (int i = 0; i < indices.count(); i += 3) {
            QList<ushort> verts;
            for (int v = 0; v < 3; ++v) verts.append(indices[i + v]);

            writer.writeLine(
                QString("f %1 %2 %3").arg(verts[0] + 1).arg(verts[1] + 1).arg(verts[2] + 1));
        }

        writer.flush();
    }
}

int RSDKv4::Model::getPLYPropertyType(QString propertyType)
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

void RSDKv4::Model::loadPLY(QString filePath)
{
    texCoords.clear();
    frames.clear();
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
            texCoords.clear();

            // Fill up all the blanks
            for (int v = 0; v < element.propertyValues.count(); ++v) {
                frames[0].vertices.append(Frame::Vertex());
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
                    int faceVerticesCount = propertyValue.values.count();

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

void RSDKv4::Model::writeAsPLY(QString filePath, int exportFrame)
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
        writer.writeLine("property float nx");
        writer.writeLine("property float ny");
        writer.writeLine("property float nz");
        writer.writeLine("property float u");
        writer.writeLine("property float v");
        writer.writeLine("element face " + QString::number(indices.count() / 3));
        writer.writeLine("property list uint8 uint16 vertex_indices");
        writer.writeLine("end_header");

        if (binary) {
            for (int v = 0; v < frames[f].vertices.count(); ++v) {
                writer.write(frames[f].vertices[v].x);
                writer.write(frames[f].vertices[v].y);
                writer.write(frames[f].vertices[v].z);

                writer.write(frames[f].vertices[v].nx);
                writer.write(frames[f].vertices[v].ny);
                writer.write(frames[f].vertices[v].nz);

                writer.write(texCoords[v].x);
                writer.write(texCoords[v].y);
            }

            for (int i = 0; i < indices.count(); i += 3) {
                writer.write<byte>(3);
                for (int v = 0; v < 3; ++v) writer.write<ushort>(indices[i + v]);
            }
        }
        else {
            for (int v = 0; v < frames[f].vertices.count(); ++v) {
                QString vertex = "";

                vertex += QString("%1 %2 %3")
                              .arg(frames[f].vertices[v].x)
                              .arg(frames[f].vertices[v].y)
                              .arg(frames[f].vertices[v].z);

                vertex += QString(" %1 %2 %3")
                              .arg(frames[f].vertices[v].nx)
                              .arg(frames[f].vertices[v].ny)
                              .arg(frames[f].vertices[v].nz);

                vertex += QString(" %1 %2").arg(texCoords[v].x).arg(texCoords[v].y);

                writer.writeLine(vertex);
            }

            for (int i = 0; i < indices.count(); i += 3) {
                QString face = "";

                face += QString::number(3) + " ";

                for (int v = 0; v < 3; ++v) {
                    face += QString::number(indices[i + v]);
                    if (v + 1 < 3)
                        face += " ";
                }

                writer.writeLine(face);
            }
        }

        writer.flush();
    }
}
