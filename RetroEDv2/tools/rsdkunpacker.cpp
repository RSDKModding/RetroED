#include "includes.hpp"
#include "ui_rsdkunpacker.h"

#include "rsdkunpacker.hpp"

#include <RSDKv1/datapackv1.hpp>
#include <RSDKv2/datapackv2.hpp>
#include <RSDKv3/datapackv3.hpp>
#include <RSDKv4/datapackv4.hpp>
#include <RSDKv5/datapackv5.hpp>
// special
#include <RSDKv3/arccontainerv3.hpp>

RSDKUnpacker::RSDKUnpacker(QWidget *parent) : QWidget(parent), ui(new Ui::RSDKUnpacker)
{
    ui->setupUi(this);

    connect(ui->selectDatapack, &QPushButton::clicked, [this] {
        QList<QString> types = {
            "RSDKv5 Datapacks (*.rsdk*)", "RSDKv4 Datapacks (*.rsdk*)",
            "RSDKv3 Datapacks (*.rsdk*)", "RSDKv2 Datapacks (*.bin*)",
            "RSDKv1 Datapacks (*.bin*)",  "RSDKv3 Arc Containers (*.arc*)",
        };

        QFileDialog filedialog(this, tr("Open Datapack"), "",
                               tr(QString("%1;;%2;;%3;;%4;;%5;;%6")
                                      .arg(types[0])
                                      .arg(types[1])
                                      .arg(types[2])
                                      .arg(types[3])
                                      .arg(types[4])
                                      .arg(types[5])
                                      .toStdString()
                                      .c_str()));
        filedialog.setAcceptMode(QFileDialog::AcceptOpen);
        if (filedialog.exec() == QDialog::Accepted) {
            int filter = types.indexOf(filedialog.selectedNameFilter());

            QString fileList = "";
            if (filter <= 1) {
                QFileDialog listdialog(this, tr("Open File List"), homeDir, tr("File Lists (*.txt)"));
                listdialog.setAcceptMode(QFileDialog::AcceptOpen);

                if (listdialog.exec() == QDialog::Accepted)
                    fileList = listdialog.selectedFiles()[0];
            }

            LoadPack(filedialog.selectedFiles()[0], filter, fileList);
        }
    });
    connect(ui->exportDatapack, &QPushButton::clicked, [this] {
        QFileDialog filedialog(this, tr("Open File"), "", tr("All Files (*)"));
        filedialog.setFileMode(QFileDialog::Directory);
        filedialog.setAcceptMode(QFileDialog::AcceptOpen);
        if (filedialog.exec() == QDialog::Accepted) {
            QString baseDir = filedialog.selectedFiles()[0];
            SetStatus("Unpacking Datapack...", true);
            float total = files.count();
            int count   = 0;

            for (FileInfo &file : files) {
                QString fDir = file.filename;
                fDir         = fDir.replace(QFileInfo(fDir).fileName(), "");
                QString dir  = baseDir + "/" + fDir;
                if (!QDir(QDir::tempPath()).exists(dir))
                    QDir(QDir::tempPath()).mkpath(dir);

                QString path = baseDir + "/" + file.filename;
                QFile f(path);
                f.open(QIODevice::WriteOnly);
                f.write(file.fileData);
                f.close();
                SetStatusProgress(++count / total);
            }

            SetStatus("Datapack unpacked to " + baseDir);
        }
    });

    connect(ui->selectDataFolder, &QPushButton::clicked, [this] {
        QFileDialog filedialog(this, tr("Open Datafolder"), "", tr("Folder"));
        filedialog.setFileMode(QFileDialog::Directory);
        filedialog.setAcceptMode(QFileDialog::AcceptOpen);
        if (filedialog.exec() == QDialog::Accepted) {
            SetStatus("Creating Datapack...", true);
            QString dir  = filedialog.selectedFiles()[0] + "/";
            QDir dirInfo = QDir(dir);
            dirInfo.cdUp();
            QString absDir = dirInfo.path() + "/";

            files.clear();
            ui->fileList->blockSignals(true);
            ui->fileList->clear();

            // Search [Folder]/
            QDirIterator itData(dir, QStringList() << "*", QDir::Files, QDirIterator::Subdirectories);
            while (itData.hasNext()) {
                QString file = itData.next();
                FileInfo info;
                info.encrypted = false;
                info.fileSize  = QFileInfo(file).size();

                info.filename = file;
                info.filename.replace(absDir, "");
                Reader reader(file);
                info.fileData = reader.readByteArray(reader.filesize);
                reader.close();
                files.append(info);
                ui->fileList->addItem(info.filename);
            }

            // Search [FolderAbsDir]/ByteCode/
            QDirIterator itBC(absDir + "ByteCode/", QStringList() << "*", QDir::Files,
                              QDirIterator::Subdirectories);
            while (itBC.hasNext()) {
                QString file = itBC.next();
                FileInfo info;
                info.encrypted = false;
                info.fileSize  = QFileInfo(file).size();

                info.filename = file;
                info.filename.replace(absDir, "");
                Reader reader(file);
                info.fileData = reader.readByteArray(reader.filesize);
                reader.close();
                files.append(info);
                ui->fileList->addItem(info.filename);
            }

            // Search [FolderAbsDir]/Scripts/
            QDirIterator itScr(absDir + "Scripts/", QStringList() << "*", QDir::Files,
                               QDirIterator::Subdirectories);
            while (itScr.hasNext()) {
                QString file = itScr.next();
                FileInfo info;
                info.encrypted = false;
                info.fileSize  = QFileInfo(file).size();

                info.filename = file;
                info.filename.replace(absDir, "");
                Reader reader(file);
                info.fileData = reader.readByteArray(reader.filesize);
                reader.close();
                files.append(info);
                ui->fileList->addItem(info.filename);
            }
            ui->fileList->blockSignals(false);
        }
    });
    connect(ui->buildDatapack, &QPushButton::clicked, [this] {
        QList<QString> types = {
            "RSDKv5 Datapacks (*.rsdk)", "RSDKv4 Datapacks (*.rsdk)", "RSDKv3 Datapacks (*.rsdk)",
            "RSDKv2 Datapacks (*.bin)",  "RSDKv1 Datapacks (*.bin)",  "RSDKv3 Arc Containers (*.arc)",
        };

        QFileDialog filedialog(this, tr("Save Datapack"), "",
                               tr(QString("%1;;%2;;%3;;%4;;%5;;%6")
                                      .arg(types[0])
                                      .arg(types[1])
                                      .arg(types[2])
                                      .arg(types[3])
                                      .arg(types[4])
                                      .arg(types[5])
                                      .toStdString()
                                      .c_str()));
        filedialog.setAcceptMode(QFileDialog::AcceptSave);
        if (filedialog.exec() == QDialog::Accepted) {
            SetStatus("Saving Datapack...");
            SavePack(filedialog.selectedFiles()[0], types.indexOf(filedialog.selectedNameFilter()));

            SetStatus("Datapack Saved Successfully!");
        }
    });

    connect(ui->fileList, &QListWidget::currentRowChanged, [this](int c) {
        ui->encrypted->setDisabled(c == -1 || gameVer >= ENGINE_v3);
        ui->rmFile->setDisabled(c == -1);

        ui->filename->setText("");
        ui->filenameHash->setText("");
        ui->filesize->setText(QString("File Size: %1 bytes").arg(QString::number(0)));

        ui->encrypted->blockSignals(true);
        ui->encrypted->setChecked(false);
        ui->encrypted->blockSignals(false);

        if (c == -1)
            return;

        ui->rmFile->setDisabled(files.count() == 0);

        ui->filename->setText(files[c].filename);
        QString fname = files[c].filename;
        ui->filenameHash->setText(
            Utils::getMd5HashString(QByteArray((const char *)files[c].hash, 0x10)));
        ui->filesize->setText(QString("File Size: %1 bytes").arg(QString::number(files[c].fileSize)));

        ui->encrypted->blockSignals(true);

        ui->encrypted->setChecked(files[c].encrypted);
        ui->encrypted->setDisabled(false);
        ui->encrypted->blockSignals(false);
    });

    connect(ui->encrypted, &QCheckBox::toggled,
            [this](bool v) { files[ui->fileList->currentRow()].encrypted = v; });

    connect(ui->addFile, &QToolButton::clicked, [this] {
        QFileDialog filedialog(this, tr("Open File"), "", tr("All Files (*)"));
        filedialog.setAcceptMode(QFileDialog::AcceptOpen);
        if (filedialog.exec() == QDialog::Accepted) {
            FileInfo file;
            QString fname =
                QFileInfo(filedialog.selectedFiles()[0]).fileName().replace("\\", "/").toLower();

            if (fname.indexOf("/data/") >= 0)
                fname = fname.mid(fname.indexOf("/data/") + QString("/data/").length());

            file.filename = fname;
            Reader reader(filedialog.selectedFiles()[0]);

            file.fileSize  = reader.filesize;
            file.fileData  = reader.readByteArray(reader.filesize);
            file.encrypted = false;
            files.append(file);
            ui->fileList->addItem(file.filename);
        }
    });

    connect(ui->rmFile, &QToolButton::clicked, [this] {
        ui->fileList->blockSignals(true);
        files.removeAt(ui->fileList->currentRow());
        delete ui->fileList->item(ui->fileList->currentRow());
        ui->fileList->blockSignals(false);
    });
}

RSDKUnpacker::~RSDKUnpacker() { delete ui; }

void RSDKUnpacker::LoadPack(QString filepath, byte ver, QString fileNameList)
{
    Reader reader(filepath);

    ui->fileList->clear();
    gameVer = ver;

    ui->fileList->blockSignals(true);

    fileList.clear();
    if (QFile(fileNameList).exists()) {
        QFile file(fileNameList);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream txtreader(&file);
            while (!txtreader.atEnd()) fileList.append(txtreader.readLine());

            file.close();
        }
    }

    SetStatus("Loading Datapack...", true);
    // TODO: segment this somehow
    files.clear();
    switch (gameVer) {
        case ENGINE_v5: // RSDKv5
        {
            RSDKv5::Datapack datapackv5(reader, fileList);
            int count   = 0;
            float total = datapackv5.files.count();

            for (RSDKv5::Datapack::FileInfo &file : datapackv5.files) {
                FileInfo info;
                info.filename  = file.fileName;
                info.fileSize  = file.fileSize;
                info.fileData  = file.fileData;
                info.encrypted = file.encrypted;
                memcpy(info.hash, file.hash, sizeof(info.hash));
                files.append(info);

                ui->fileList->addItem(file.fileName);
                SetStatusProgress(++count / total);
            }
            break;
        }
        case ENGINE_v4: // RSDKv4
        {
            RSDKv4::Datapack datapackv4(reader, fileList);
            int count   = 0;
            float total = datapackv4.files.count();

            for (RSDKv4::Datapack::FileInfo &file : datapackv4.files) {
                FileInfo info;
                info.filename  = file.fileName;
                info.fileSize  = file.fileSize;
                info.fileData  = file.fileData;
                info.encrypted = file.encrypted;
                memcpy(info.hash, file.hash, sizeof(info.hash));
                files.append(info);

                ui->fileList->addItem(file.fileName);
                SetStatusProgress(++count / total);
            }
            break;
        }
        case ENGINE_v3: // RSDKv3
        {
            RSDKv3::Datapack datapackv3(reader);
            int count   = 0;
            float total = datapackv3.files.count();

            for (RSDKv3::Datapack::FileInfo &file : datapackv3.files) {
                FileInfo info;
                info.filename  = file.fullFileName;
                info.fileSize  = file.fileSize;
                info.fileData  = file.fileData;
                info.encrypted = false;
                memset(info.hash, 0, sizeof(info.hash));
                files.append(info);

                ui->fileList->addItem(file.fullFileName);
                SetStatusProgress(++count / total);
            }
            break;
        }
        case ENGINE_v2: // RSDKv2
        {
            RSDKv2::Datapack datapackv2(reader);
            int count   = 0;
            float total = datapackv2.files.count();

            for (RSDKv2::Datapack::FileInfo &file : datapackv2.files) {
                FileInfo info;
                info.filename  = file.fullFileName;
                info.fileSize  = file.fileSize;
                info.fileData  = file.fileData;
                info.encrypted = false;
                files.append(info);

                ui->fileList->addItem(file.fullFileName);
                SetStatusProgress(++count / total);
            }
            break;
        }
        case ENGINE_v1: // RSDKv1
        {
            RSDKv1::Datapack datapackv1(reader);
            int count   = 0;
            float total = datapackv1.files.count();

            for (RSDKv1::Datapack::FileInfo &file : datapackv1.files) {
                FileInfo info;
                info.filename  = file.fullFilename;
                info.fileSize  = file.fileSize;
                info.fileData  = file.fileData;
                info.encrypted = false;
                files.append(info);

                ui->fileList->addItem(file.fullFilename);
                SetStatusProgress(++count / total);
            }
            break;
        }
        case ENGINE_v1
            + 1: // ARC
        {
            RSDKv3::ArcContainer arcContainer(reader);
            int count   = 0;
            float total = arcContainer.files.count();

            for (RSDKv3::ArcContainer::FileInfo &file : arcContainer.files) {
                FileInfo info;
                info.filename  = file.fileName;
                info.fileSize  = file.fileSize;
                info.fileData  = file.fileData;
                info.encrypted = false;
                files.append(info);

                ui->fileList->addItem(file.fileName);
                SetStatusProgress(++count / total);
            }
            break;
        }
    }

    ui->fileList->blockSignals(false);

    ui->fileList->setCurrentRow(-1);

    SetStatus("Datapack loaded successfully!");
}

void RSDKUnpacker::SavePack(QString filepath, byte ver)
{

    QList<FileInfo> files = this->files;
    QList<QString> dirs;
    std::sort(files.begin(), files.end(),
              [](const FileInfo &a, const FileInfo &b) -> bool { return a.filename < b.filename; });

    switch (ver) {
        case ENGINE_v5: // RSDKv5
        {
            RSDKv5::Datapack datapack;

            datapack.files.clear();
            for (FileInfo &file : files) {
                RSDKv5::Datapack::FileInfo info;
                info.fileName  = file.filename;
                info.fileSize  = file.fileSize;
                info.fileData  = file.fileData;
                info.encrypted = file.encrypted;
                datapack.files.append(info);
            }

            datapack.write(filepath);
            break;
        }
        case ENGINE_v4: // RSDKv4
        {
            RSDKv4::Datapack datapack;

            datapack.files.clear();
            for (FileInfo &file : files) {
                RSDKv4::Datapack::FileInfo info;
                info.fileName  = file.filename;
                info.fileSize  = file.fileSize;
                info.fileData  = file.fileData;
                info.encrypted = file.encrypted;
                datapack.files.append(info);
            }

            datapack.write(filepath);
            break;
        }
        case ENGINE_v3: // RSDKv3
        {
            RSDKv3::Datapack datapack;

            datapack.files.clear();
            datapack.directories.clear();
            int dirID = -1;
            for (FileInfo &file : files) {
                QString dir = file.filename;
                dir         = dir.replace(QFileInfo(file.filename).fileName(), "");
                if (dirs.indexOf(dir) < 0) {
                    dirs.append(dir);
                    ++dirID;
                }

                RSDKv3::Datapack::FileInfo info;
                info.fileName = QFileInfo(file.filename).fileName();
                info.fileSize = file.fileSize;
                info.fileData = file.fileData;
                info.dirID    = dirID;
                datapack.files.append(info);
            }

            for (QString &dir : dirs) {
                RSDKv3::Datapack::DirInfo d;
                d.directory = dir;
                datapack.directories.append(d);
            }

            datapack.write(filepath);
            break;
        }
        case ENGINE_v2: // RSDKv2
        {
            RSDKv2::Datapack datapack;

            datapack.files.clear();
            datapack.directories.clear();
            int dirID = -1;
            for (FileInfo &file : files) {
                QString dir = file.filename;
                dir         = dir.replace(QFileInfo(file.filename).fileName(), "");
                if (dirs.indexOf(dir) < 0) {
                    dirs.append(dir);
                    ++dirID;
                }

                RSDKv2::Datapack::FileInfo info;
                info.fileName = QFileInfo(file.filename).fileName();
                info.fileSize = file.fileSize;
                info.fileData = file.fileData;
                info.dirID    = dirID;
                datapack.files.append(info);
            }

            for (QString &dir : dirs) {
                RSDKv2::Datapack::DirInfo d;
                d.directory = dir;
                datapack.directories.append(d);
            }

            datapack.write(filepath);
            break;
        }
        case ENGINE_v1: // RSDKv1
        {
            RSDKv1::Datapack datapack;

            datapack.files.clear();
            datapack.directories.clear();
            int dirID = -1;
            for (FileInfo &file : files) {
                QString dir = file.filename;
                dir         = dir.replace(QFileInfo(file.filename).fileName(), "");
                if (dirs.indexOf(dir) < 0) {
                    dirs.append(dir);
                    ++dirID;
                }

                RSDKv1::Datapack::FileInfo info;
                info.filename = QFileInfo(file.filename).fileName();
                info.fileSize = file.fileSize;
                info.fileData = file.fileData;
                info.dirID    = dirID;
                datapack.files.append(info);
            }

            for (QString &dir : dirs) {
                RSDKv1::Datapack::DirInfo d;
                d.directory = dir;
                datapack.directories.append(d);
            }

            datapack.write(filepath);
            break;
        }
        case ENGINE_v1
            + 1: // Arc
        {
            RSDKv3::ArcContainer container;

            container.files.clear();
            for (FileInfo &file : files) {
                RSDKv3::ArcContainer::FileInfo info;
                info.fileName = file.filename;
                info.fileSize = file.fileSize;
                info.fileData = file.fileData;
                container.files.append(info);
            }

            container.write(filepath);
            break;
        }
    }
}

#include "moc_rsdkunpacker.cpp"
