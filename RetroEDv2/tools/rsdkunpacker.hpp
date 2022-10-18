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
    void loadPack(QString filepath, byte ver, QString fileList = "");
    void savePack(QString filepath, byte ver);

    Ui::RSDKUnpacker *ui;

    // RSDKv1::Datafile m_datafilev1;
    // RSDKv2::Datafile m_datafilev2;
    // RSDKv3::Datafile m_datafilev3;
    // RSDKv4::Datafile m_datafilev4;
    // RSDKv5::Datafile m_datafilev5;
    // RSDKv3::ArcContainer m_arcContainer;

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


