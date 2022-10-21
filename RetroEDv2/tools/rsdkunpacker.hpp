#pragma once

#include <QWidget>

namespace Ui
{
class RSDKUnpacker;
}

class RSDKUnpacker : public QWidget
{
    Q_OBJECT

public:
    explicit RSDKUnpacker(QWidget *parent = nullptr);
    ~RSDKUnpacker();

private:
    void LoadPack(QString filepath, byte ver, QString fileList = "");
    void SavePack(QString filepath, byte ver);

    Ui::RSDKUnpacker *ui;

    class FileInfo
    {
    public:
        FileInfo() {}

        QString filename = "File.ext";
        byte hash[0x10];
        uint fileSize  = 0;
        bool encrypted = false;
        QByteArray fileData;
    };

    QList<FileInfo> files;

    QList<QString> fileList;

    char gameVer = -1;
};
