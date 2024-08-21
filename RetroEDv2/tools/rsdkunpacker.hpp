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
    void CreateList(QList<QFileInfo> &list, QString absPath, float progressTotal);

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

    enum RSDK_SIGNATURES{
        V5U_SIGNATURE = 0x3576,
        V4U_SIGNATURE = 0x3476, // in theory that's the value, in reality is the same as v5U
        V3U_SIGNATURE = 0x3376,
        V5_SIGNATURE  = 0x0076,
        V4_SIGNATURE  = 0x4276
    };

    QList<FileInfo> files;

    QList<QString> fileList;

    int sig = 0x4B445352;

    char gameVer = -1;
};
