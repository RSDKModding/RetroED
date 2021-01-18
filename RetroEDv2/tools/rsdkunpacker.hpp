#ifndef RSDKUNPACKER_H
#define RSDKUNPACKER_H

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

        QString m_filename = "File.ext";
        uint m_fileSize    = 0;
        bool m_encrypted   = false;
        QByteArray m_filedata;
    };

    QList<FileInfo> m_files;

    QList<QString> m_fileList;

    char m_gameVer = -1;
};

#endif // RSDKUNPACKER_H
