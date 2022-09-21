#include "includes.hpp"
#include "ui_userdbmanager.h"

#include <time.h>
#include <iomanip>
#include <sstream>

UserDBManager::UserDBManager(QWidget *parent) : QDialog(parent), ui(new Ui::UserDBManager)
{
    ui->setupUi(this);

    // remove question mark from the title bar & disable resizing
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    this->setFixedSize(QSize(this->width(), this->height()));

    connect(ui->exportCSV, &QPushButton::pressed, [this] {
        QFileDialog filedialog(this, tr("Open UserDB file"), "", tr("RSDKv5 UserDB files (*.bin)"));
        filedialog.setAcceptMode(QFileDialog::AcceptOpen);
        if (filedialog.exec() == QDialog::Accepted) {
            QString dbPath = filedialog.selectedFiles()[0];

            QFileDialog filedialogCSV(this, tr("Save CSV file"), "", tr("csv (*.csv)"));
            filedialogCSV.setAcceptMode(QFileDialog::AcceptSave);
            if (filedialogCSV.exec() == QDialog::Accepted) {
                QString csvPath = filedialogCSV.selectedFiles()[0];

                convertDBToCSV(dbPath, csvPath);
            }
        }
    });
    connect(ui->importDB, &QPushButton::pressed, [this] {
        QFileDialog filedialogCSV(this, tr("Open CSV file"), "", tr("csv (*.csv)"));
        filedialogCSV.setAcceptMode(QFileDialog::AcceptOpen);
        if (filedialogCSV.exec() == QDialog::Accepted) {
            QString csvPath = filedialogCSV.selectedFiles()[0];

            QFileDialog filedialog(this, tr("Save UserDB file"), "", tr("RSDKv5 UserDB files (*.bin)"));
            filedialog.setAcceptMode(QFileDialog::AcceptSave);
            if (filedialog.exec() == QDialog::Accepted) {
                QString dbPath = filedialog.selectedFiles()[0];

                convertCSVToDB(dbPath, csvPath);
            }
        }
    });
}

UserDBManager::~UserDBManager() { delete ui; }

char *UserDBManager::strptime(const char *s, const char *f, struct tm *tm)
{
    std::istringstream input(s);
    input.imbue(std::locale(setlocale(LC_ALL, nullptr)));
    input >> std::get_time(tm, f);
    if (input.fail()) {
        return nullptr;
    }
    return (char *)(s + input.tellg());
}

void UserDBManager::convertDBToCSV(QString dbPath, QString csvPath)
{
    RSDKv5::UserDB userDB(dbPath);

    Writer writer(csvPath);

    writer.writeText("uuid");
    writer.writeText(",");
    writer.writeText("Creation Date");
    writer.writeText(",");
    writer.writeText("Modified Date");
    writer.writeText(",");
    for (auto &col : userDB.columns) {
        writer.writeText(col.name);
        writer.writeText(",");
    }
    writer.writeLine();

    writer.writeText("uint32");
    writer.writeText(",");
    writer.writeText("string");
    writer.writeText(",");
    writer.writeText("string");
    writer.writeText(",");
    QList<QString> types = { "invalid", "bool",    "uint8",   "uint16", "uint32", "uint64",
                             "int8",    "int16",   "int32",   "int64",  "float",  "double",
                             "vector2", "vector3", "vector4", "color",  "string", "hashMD5" };
    for (auto &col : userDB.columns) {
        writer.writeText(types[col.type]);
        writer.writeText(",");
    }
    writer.writeLine();

    for (auto &row : userDB.rows) {
        int c = 0;
        writer.writeText("0x" + QString::number(row.uuid, 0x10).toUpper());
        writer.writeText(",");

        char timeBuffer[24];
        strftime(timeBuffer, 23, "%Y/%m/%d %H:%M:%S", (tm *)&row.createDate);
        writer.writeText(timeBuffer);
        writer.writeText(",");

        strftime(timeBuffer, 23, "%Y/%m/%d %H:%M:%S", (tm *)&row.modifyDate);
        writer.writeText(timeBuffer);
        writer.writeText(",");

        for (auto &val : row.entries) {
            QDataStream stream(val.data);
            switch (userDB.columns[c].type) {
                default:
                case RSDKv5::UserDB::TableColumn::Invalid: writer.writeText("[Invalid]"); break;

                case RSDKv5::UserDB::TableColumn::Bool: {
                    byte value = false;
                    stream >> value;
                    writer.writeText(value ? "y" : "n");
                    break;
                }

                case RSDKv5::UserDB::TableColumn::UInt8: {
                    byte value = 0;
                    stream >> value;
                    writer.writeText(QString::number(value));
                    break;
                }

                case RSDKv5::UserDB::TableColumn::UInt16: {
                    ushort value = 0;
                    stream >> value;
                    writer.writeText(QString::number(value));
                    break;
                }

                case RSDKv5::UserDB::TableColumn::UInt32:
                case RSDKv5::UserDB::TableColumn::Color: {
                    uint value = 0;
                    stream >> value;
                    writer.writeText("0x" + QString::number(value, 0x10).toUpper());
                    break;
                }

                case RSDKv5::UserDB::TableColumn::UInt64: {
                    uint64 value = 0;
                    stream >> value;
                    writer.writeText(QString::number(value));
                    break;
                }

                case RSDKv5::UserDB::TableColumn::Int8: {
                    sbyte value = 0;
                    stream >> value;
                    writer.writeText(QString::number(value));
                    break;
                }

                case RSDKv5::UserDB::TableColumn::Int16: {
                    short value = 0;
                    stream >> value;
                    writer.writeText(QString::number(value));
                    break;
                }

                case RSDKv5::UserDB::TableColumn::Int32: {
                    int value = 0;
                    stream >> value;
                    writer.writeText("0x" + QString::number(value, 0x10).toUpper());
                    break;
                }

                case RSDKv5::UserDB::TableColumn::Int64: {
                    int64 value = 0;
                    stream >> value;
                    writer.writeText(QString::number(value));
                    break;
                }

                case RSDKv5::UserDB::TableColumn::Float: {
                    float value = 0;
                    stream >> value;
                    writer.writeText(QString::number(value));
                    break;
                }

                case RSDKv5::UserDB::TableColumn::Double: {
                    double value = 0;
                    stream >> value;
                    writer.writeText(QString::number(value));
                    break;
                }

                case RSDKv5::UserDB::TableColumn::Vector2: {
                    QString value(val.data);
                    writer.writeText("[Unimplemented]");
                    break;
                }

                case RSDKv5::UserDB::TableColumn::Vector3: {
                    QString value(val.data);
                    writer.writeText("[Unimplemented]");
                    break;
                }

                case RSDKv5::UserDB::TableColumn::Vector4: {
                    QString value(val.data);
                    writer.writeText("[Unimplemented]");
                    break;
                }

                case RSDKv5::UserDB::TableColumn::String: {
                    QString value(val.data);
                    writer.writeText(value);
                    break;
                }

                case RSDKv5::UserDB::TableColumn::HashMD5: {
                    QString value(val.data);
                    writer.writeText("[Unimplemented]");
                    break;
                }
            }
            writer.writeText(",");
            ++c;
        }
        writer.writeLine();
    }

    writer.flush();
}

void UserDBManager::convertCSVToDB(QString dbPath, QString csvPath)
{
    RSDKv5::UserDB userDB;

    Reader reader(csvPath);

    QList<QString> types = { "invalid", "bool",    "uint8",   "uint16", "uint32", "uint64",
                             "int8",    "int16",   "int32",   "int64",  "float",  "double",
                             "vector2", "vector3", "vector4", "color",  "string", "hashMD5" };

    int mode = 0;
    while (!reader.isEOF()) {
        QString line    = reader.readLine();
        QStringList sep = line.split(",");
        if (line[line.length() - 1] == ",")
            sep.removeLast();

        switch (mode) {
            default: break;

            case 0: // read names
                if (sep.length() < 3) {
                    // invalid formatted file!
                    mode = 3;
                }
                for (int i = 3; i < sep.count(); ++i) {
                    RSDKv5::UserDB::TableColumn column;
                    column.name = sep[i];
                    userDB.columns.append(column);
                }
                ++mode;
                break;

            case 1: // read types
                if (sep.length() < 3) {
                    // invalid formatted file!
                    mode = 3;
                }

                for (int i = 3; i < sep.count(); ++i) {
                    userDB.columns[i - 3].type =
                        (RSDKv5::UserDB::TableColumn::Types)types.indexOf(sep[i]);
                }
                ++mode;
                break;

            case 2: // read values
            {
                RSDKv5::UserDB::TableRow row;
                for (int i = 0; i < sep.count(); ++i) {
                    QString value = sep[i];
                    tm tmBuf;
                    switch (i) {
                        case 0:
                            if (value.startsWith("0x"))
                                row.uuid = value.toUInt(nullptr, 0x10);
                            else
                                row.uuid = value.toUInt(nullptr, 10);
                            break;

                        case 1:
                            strptime(value.toStdString().c_str(), "%Y/%m/%d %H:%M:%S", &tmBuf);
                            *(tm *)&row.createDate = *&tmBuf;
                            break;

                        case 2:
                            strptime(value.toStdString().c_str(), "%Y/%m/%d %H:%M:%S", &tmBuf);
                            *(tm *)&row.modifyDate = *&tmBuf;
                            break;

                        default: {
                            RSDKv5::UserDB::TableRow::Value entry;
                            entry.data.clear();
                            QDataStream stream(&entry.data, QIODevice::ReadWrite);
                            switch (userDB.columns[i - 3].type) {
                                default:
                                case RSDKv5::UserDB::TableColumn::Invalid:
                                    // Invalid, do nothing
                                    break;

                                case RSDKv5::UserDB::TableColumn::Bool:
                                    stream << (byte)(value.toLower().startsWith("y") ? 1 : 0);
                                    break;

                                case RSDKv5::UserDB::TableColumn::UInt8:
                                    if (value.startsWith("0x"))
                                        stream << (byte)value.toInt(nullptr, 0x10);
                                    else
                                        stream << (byte)value.toInt(nullptr, 10);
                                    break;

                                case RSDKv5::UserDB::TableColumn::UInt16:
                                    if (value.startsWith("0x"))
                                        stream << value.toUShort(nullptr, 0x10);
                                    else
                                        stream << value.toUShort(nullptr, 10);
                                    break;

                                case RSDKv5::UserDB::TableColumn::UInt32:
                                case RSDKv5::UserDB::TableColumn::Color:
                                    if (value.startsWith("0x"))
                                        stream << value.toUInt(nullptr, 0x10);
                                    else
                                        stream << value.toUInt(nullptr, 10);
                                    break;

                                case RSDKv5::UserDB::TableColumn::UInt64:
                                    if (value.startsWith("0x"))
                                        stream << value.toULongLong(nullptr, 0x10);
                                    else
                                        stream << value.toULongLong(nullptr, 10);
                                    break;

                                case RSDKv5::UserDB::TableColumn::Int8:
                                    if (value.startsWith("0x"))
                                        stream << (sbyte)value.toInt(nullptr, 0x10);
                                    else
                                        stream << (sbyte)value.toInt(nullptr, 10);
                                    break;

                                case RSDKv5::UserDB::TableColumn::Int16:
                                    if (value.startsWith("0x"))
                                        stream << value.toShort(nullptr, 0x10);
                                    else
                                        stream << value.toShort(nullptr, 10);
                                    break;

                                case RSDKv5::UserDB::TableColumn::Int32:
                                    if (value.startsWith("0x"))
                                        stream << value.toInt(nullptr, 0x10);
                                    else
                                        stream << value.toInt(nullptr, 10);
                                    break;

                                case RSDKv5::UserDB::TableColumn::Int64:
                                    if (value.startsWith("0x"))
                                        stream << value.toLongLong(nullptr, 0x10);
                                    else
                                        stream << value.toLongLong(nullptr, 10);
                                    break;

                                case RSDKv5::UserDB::TableColumn::Float:
                                    stream << value.toFloat();
                                    break;

                                case RSDKv5::UserDB::TableColumn::Double:
                                    stream << value.toDouble();
                                    break;

                                case RSDKv5::UserDB::TableColumn::Vector2:
                                    // Unimplemented, do nothing
                                    break;

                                case RSDKv5::UserDB::TableColumn::Vector3:
                                    // Unimplemented, do nothing
                                    break;

                                case RSDKv5::UserDB::TableColumn::Vector4:
                                    // Unimplemented, do nothing
                                    break;

                                case RSDKv5::UserDB::TableColumn::String: stream << value; break;

                                case RSDKv5::UserDB::TableColumn::HashMD5:
                                    // Unimplemented, do nothing
                                    break;
                            }
                            row.entries.append(entry);
                            break;
                        }
                    }
                }
                userDB.rows.append(row);
                break;
            }
        }
    }

    // if we didn't get to mode 2, we didn't make a valid DB
    if (mode == 2)
        userDB.write(dbPath);
}
