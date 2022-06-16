#include "includes.hpp"
#include "ui_rsdkunpacker.h"

RSDKUnpacker::RSDKUnpacker(QWidget *parent) : QWidget(parent), ui(new Ui::RSDKUnpacker)
{
    ui->setupUi(this);

    connect(ui->selectDatafile, &QPushButton::clicked, [this] {
        QList<QString> types = {
            "RSDKv5 Datafiles (*data*.rsdk*)", "RSDKv4 Datafiles (*data*.rsdk*)",
            "RSDKv3 Datafiles (*data*.rsdk*)", "RSDKv2 Datafiles (*data*.bin*)",
            "RSDKv1 Datafiles (*data*.bin*)",  "RSDKv3 Arc Containers (*.arc*)",
        };

        QFileDialog filedialog(this, tr("Open Datafile"), "",
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

            loadPack(filedialog.selectedFiles()[0], filter, fileList);
        }
    });
    connect(ui->exportDatafile, &QPushButton::clicked, [this] {
        QFileDialog filedialog(this, tr("Open File"), "", tr("All Files (*)"));
        filedialog.setFileMode(QFileDialog::Directory);
        filedialog.setAcceptMode(QFileDialog::AcceptOpen);
        if (filedialog.exec() == QDialog::Accepted) {
            QString baseDir = filedialog.selectedFiles()[0];
            SetStatus("Unpacking Datafile...", true);
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

            SetStatus("Datafile unpacked to " + baseDir);
        }
    });

    connect(ui->selectDataFolder, &QPushButton::clicked, [this] {
        QFileDialog filedialog(this, tr("Open Datafolder"), "", tr("Folder"));
        filedialog.setFileMode(QFileDialog::Directory);
        filedialog.setAcceptMode(QFileDialog::AcceptOpen);
        if (filedialog.exec() == QDialog::Accepted) {
            SetStatus("Creating Datafile...", true);
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
    connect(ui->buildDatafile, &QPushButton::clicked, [this] {
        QList<QString> types = {
            "RSDKv5 Datafiles (*data*.rsdk)", "RSDKv4 Datafiles (*data*.rsdk)",
            "RSDKv3 Datafiles (*data*.rsdk)", "RSDKv2 Datafiles (*data*.bin)",
            "RSDKv1 Datafiles (*data*.bin)",  "RSDKv3 Arc Containers (*.arc)",
        };

        QFileDialog filedialog(this, tr("Save Datafile"), "",
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
            SetStatus("Saving Datafile...");
            savePack(filedialog.selectedFiles()[0], types.indexOf(filedialog.selectedNameFilter()));

            SetStatus("Datafile Saved Successfully!");
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

void RSDKUnpacker::loadPack(QString filepath, byte ver, QString fileNameList)
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

    SetStatus("Loading Datafile...", true);
    // TODO: segment this somehow
    files.clear();
    switch (gameVer) {
        case ENGINE_v5: // RSDKv5
        {
            RSDKv5::Datafile datafilev5(reader, fileList);
            int count   = 0;
            float total = datafilev5.files.count();

            for (RSDKv5::Datafile::FileInfo &file : datafilev5.files) {
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
            RSDKv4::Datafile datafilev4(reader, fileList);
            int count   = 0;
            float total = datafilev4.files.count();

            for (RSDKv4::Datafile::FileInfo &file : datafilev4.files) {
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
            RSDKv3::Datafile datafilev3(reader);
            int count   = 0;
            float total = datafilev3.files.count();

            for (RSDKv3::Datafile::FileInfo &file : datafilev3.files) {
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
            RSDKv2::Datafile datafilev2(reader);
            int count   = 0;
            float total = datafilev2.files.count();

            for (RSDKv2::Datafile::FileInfo &file : datafilev2.files) {
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
            RSDKv1::Datafile datafilev1(reader);
            int count   = 0;
            float total = datafilev1.files.count();

            for (RSDKv1::Datafile::FileInfo &file : datafilev1.files) {
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

    SetStatus("Datafile loaded successfully!");
}

void RSDKUnpacker::savePack(QString filepath, byte ver)
{

    QList<FileInfo> files = this->files;
    QList<QString> dirs;
    std::sort(files.begin(), files.end(),
              [](const FileInfo &a, const FileInfo &b) -> bool { return a.filename < b.filename; });

    switch (ver) {
        case ENGINE_v5: // RSDKv5
        {
            RSDKv5::Datafile datafile;

            datafile.files.clear();
            for (FileInfo &file : files) {
                RSDKv5::Datafile::FileInfo info;
                info.fileName  = file.filename;
                info.fileSize  = file.fileSize;
                info.fileData  = file.fileData;
                info.encrypted = file.encrypted;
                datafile.files.append(info);
            }

            datafile.write(filepath);
            break;
        }
        case ENGINE_v4: // RSDKv4
        {
            RSDKv4::Datafile datafile;

            datafile.files.clear();
            for (FileInfo &file : files) {
                RSDKv4::Datafile::FileInfo info;
                info.fileName  = file.filename;
                info.fileSize  = file.fileSize;
                info.fileData  = file.fileData;
                info.encrypted = file.encrypted;
                datafile.files.append(info);
            }

            datafile.write(filepath);
            break;
        }
        case ENGINE_v3: // RSDKv3
        {
            RSDKv3::Datafile datafile;

            datafile.files.clear();
            datafile.directories.clear();
            int dirID = -1;
            for (FileInfo &file : files) {
                QString dir = file.filename;
                dir         = dir.replace(QFileInfo(file.filename).fileName(), "");
                if (dirs.indexOf(dir) < 0) {
                    dirs.append(dir);
                    ++dirID;
                }

                RSDKv3::Datafile::FileInfo info;
                info.fileName = QFileInfo(file.filename).fileName();
                info.fileSize = file.fileSize;
                info.fileData = file.fileData;
                info.dirID    = dirID;
                datafile.files.append(info);
            }

            for (QString &dir : dirs) {
                RSDKv3::Datafile::DirInfo d;
                d.directory = dir;
                datafile.directories.append(d);
            }

            datafile.write(filepath);
            break;
        }
        case ENGINE_v2: // RSDKv2
        {
            RSDKv2::Datafile datafile;

            datafile.files.clear();
            datafile.directories.clear();
            int dirID = -1;
            for (FileInfo &file : files) {
                QString dir = file.filename;
                dir         = dir.replace(QFileInfo(file.filename).fileName(), "");
                if (dirs.indexOf(dir) < 0) {
                    dirs.append(dir);
                    ++dirID;
                }

                RSDKv2::Datafile::FileInfo info;
                info.fileName = QFileInfo(file.filename).fileName();
                info.fileSize = file.fileSize;
                info.fileData = file.fileData;
                info.dirID    = dirID;
                datafile.files.append(info);
            }

            for (QString &dir : dirs) {
                RSDKv2::Datafile::DirInfo d;
                d.directory = dir;
                datafile.directories.append(d);
            }

            datafile.write(filepath);
            break;
        }
        case ENGINE_v1: // RSDKv1
        {
            RSDKv1::Datafile datafile;

            datafile.files.clear();
            datafile.directories.clear();
            int dirID = -1;
            for (FileInfo &file : files) {
                QString dir = file.filename;
                dir         = dir.replace(QFileInfo(file.filename).fileName(), "");
                if (dirs.indexOf(dir) < 0) {
                    dirs.append(dir);
                    ++dirID;
                }

                RSDKv1::Datafile::FileInfo info;
                info.filename = QFileInfo(file.filename).fileName();
                info.fileSize = file.fileSize;
                info.fileData = file.fileData;
                info.dirID    = dirID;
                datafile.files.append(info);
            }

            for (QString &dir : dirs) {
                RSDKv1::Datafile::DirInfo d;
                d.directory = dir;
                datafile.directories.append(d);
            }

            datafile.write(filepath);
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
