#include "includes.hpp"
#include "ui_rsdkunpacker.h"

RSDKUnpacker::RSDKUnpacker(QWidget *parent) : QWidget(parent), ui(new Ui::RSDKUnpacker)
{
    ui->setupUi(this);

    connect(ui->selectDatafile, &QPushButton::clicked, [this] {
        QList<QString> types = {
            "RSDKv5 Datafiles (*data*.rsdk)", "RSDKv4 Datafiles (*data*.rsdk)",
            "RSDKv3 Datafiles (*data*.rsdk)", "RSDKv2 Datafiles (*data*.bin)",
            "RSDKv1 Datafiles (*data*.bin)",  "RSDKv3 Arc Containers (*.arc)",
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
            setStatus("Unpacking Datafile...");

            for (FileInfo &file : m_files) {
                QString fDir = file.m_filename;
                fDir         = fDir.replace(QFileInfo(fDir).fileName(), "");
                QString dir  = baseDir + "/" + fDir;
                if (!QDir(QDir::tempPath()).exists(dir))
                    QDir(QDir::tempPath()).mkpath(dir);

                QString path = baseDir + "/" + file.m_filename;
                QFile f(path);
                f.open(QIODevice::WriteOnly);
                f.write(file.m_filedata);
                f.close();
            }

            setStatus("Datafile unpacked to: " + baseDir);
        }
    });

    connect(ui->selectDataFolder, &QPushButton::clicked, [this] {
        QFileDialog filedialog(this, tr("Open Datafolder"), "", tr("Folder"));
        filedialog.setFileMode(QFileDialog::Directory);
        filedialog.setAcceptMode(QFileDialog::AcceptOpen);
        if (filedialog.exec() == QDialog::Accepted) {
            setStatus("Creating Datafile...");
            QString dir  = filedialog.selectedFiles()[0] + "/";
            QDir dirInfo = QDir(dir);
            dirInfo.cdUp();
            QString absDir = dirInfo.path() + "/";

            m_files.clear();
            ui->fileList->blockSignals(true);
            ui->fileList->clear();

            // Search [Folder]/
            QDirIterator itData(dir, QStringList() << "*", QDir::Files, QDirIterator::Subdirectories);
            while (itData.hasNext()) {
                QString file = itData.next();
                FileInfo info;
                info.m_encrypted = false;
                info.m_fileSize  = QFileInfo(file).size();

                info.m_filename = file;
                info.m_filename.replace(absDir, "");
                Reader reader(file);
                info.m_filedata = reader.readByteArray(reader.m_filesize);
                reader.close();
                m_files.append(info);
                ui->fileList->addItem(info.m_filename);
            }

            // Search [FolderAbsDir]/ByteCode/
            QDirIterator itBC(absDir + "ByteCode/", QStringList() << "*", QDir::Files,
                              QDirIterator::Subdirectories);
            while (itBC.hasNext()) {
                QString file = itBC.next();
                FileInfo info;
                info.m_encrypted = false;
                info.m_fileSize  = QFileInfo(file).size();

                info.m_filename = file;
                info.m_filename.replace(absDir, "");
                Reader reader(file);
                info.m_filedata = reader.readByteArray(reader.m_filesize);
                reader.close();
                m_files.append(info);
                ui->fileList->addItem(info.m_filename);
            }

            // Search [FolderAbsDir]/Scripts/
            QDirIterator itScr(absDir + "Scripts/", QStringList() << "*", QDir::Files,
                               QDirIterator::Subdirectories);
            while (itScr.hasNext()) {
                QString file = itScr.next();
                FileInfo info;
                info.m_encrypted = false;
                info.m_fileSize  = QFileInfo(file).size();

                info.m_filename = file;
                info.m_filename.replace(absDir, "");
                Reader reader(file);
                info.m_filedata = reader.readByteArray(reader.m_filesize);
                reader.close();
                m_files.append(info);
                ui->fileList->addItem(info.m_filename);
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
            setStatus("Saving Datafile...");
            savePack(filedialog.selectedFiles()[0], types.indexOf(filedialog.selectedNameFilter()));

            appConfig.addRecentFile(m_gameVer, TOOL_RSDKUNPACKER, filedialog.selectedFiles()[0],
                                    QList<QString>{ /**/ });
            setStatus("Datafile Saved Successfully!");
        }
    });

    connect(ui->fileList, &QListWidget::currentRowChanged, [this](int c) {
        ui->encrypted->setDisabled(c == -1 || m_gameVer >= 2);
        ui->rmFile->setDisabled(c == -1);

        ui->filename->setText("");
        ui->filenameHash->setText("");
        ui->filesize->setText(QString("File Size: %1 bytes").arg(QString::number(0)));

        ui->encrypted->blockSignals(true);
        ui->encrypted->setChecked(false);
        ui->encrypted->blockSignals(false);

        if (c == -1)
            return;

        ui->rmFile->setDisabled(m_files.count() == 0);

        ui->filename->setText(m_files[c].m_filename);
        QString fname = m_files[c].m_filename;
        ui->filenameHash->setText(Utils::getMd5HashString(fname.replace("\\", "/").toLower()));
        ui->filesize->setText(
            QString("File Size: %1 bytes").arg(QString::number(m_files[c].m_fileSize)));

        ui->encrypted->blockSignals(true);

        ui->encrypted->setChecked(m_files[c].m_encrypted);
        ui->encrypted->setDisabled(false);
        ui->encrypted->blockSignals(false);
    });

    connect(ui->encrypted, &QCheckBox::toggled,
            [this](bool v) { m_files[ui->fileList->currentRow()].m_encrypted = v; });

    connect(ui->addFile, &QToolButton::clicked, [this] {
        QFileDialog filedialog(this, tr("Open File"), "", tr("All Files (*)"));
        filedialog.setAcceptMode(QFileDialog::AcceptOpen);
        if (filedialog.exec() == QDialog::Accepted) {
            FileInfo file;
            QString fname =
                QFileInfo(filedialog.selectedFiles()[0]).fileName().replace("\\", "/").toLower();

            if (fname.indexOf("/data/") >= 0)
                fname = fname.mid(fname.indexOf("/data/") + QString("/data/").length());

            file.m_filename = fname;
            Reader reader(filedialog.selectedFiles()[0]);

            file.m_fileSize  = reader.m_filesize;
            file.m_filedata  = reader.readByteArray(reader.m_filesize);
            file.m_encrypted = false;
            m_files.append(file);
            ui->fileList->addItem(file.m_filename);
        }
    });

    connect(ui->rmFile, &QToolButton::clicked, [this] {
        ui->fileList->blockSignals(true);
        m_files.removeAt(ui->fileList->currentRow());
        delete ui->fileList->item(ui->fileList->currentRow());
        ui->fileList->blockSignals(false);
    });
}

RSDKUnpacker::~RSDKUnpacker() { delete ui; }

void RSDKUnpacker::loadPack(QString filepath, byte ver, QString fileList)
{
    Reader reader(filepath);

    ui->fileList->clear();
    m_gameVer = ver;

    ui->fileList->blockSignals(true);

    m_fileList.clear();
    if (QFile(fileList).exists()) {
        QFile file(fileList);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream txtreader(&file);
            while (!txtreader.atEnd()) m_fileList.append(txtreader.readLine());

            file.close();
        }
    }

    setStatus("Loading Datafile...");
    m_files.clear();
    switch (m_gameVer) {
        case 0: // RSDKv5
        {
            RSDKv5::Datafile datafilev5(reader, m_fileList);

            for (RSDKv5::Datafile::FileInfo &file : datafilev5.m_files) {
                FileInfo info;
                info.m_filename  = file.m_filename;
                info.m_fileSize  = file.m_fileSize;
                info.m_filedata  = file.m_filedata;
                info.m_encrypted = file.m_encrypted;
                m_files.append(info);

                ui->fileList->addItem(file.m_filename);
            }
            break;
        }
        case 1: // RSDKv4
        {
            RSDKv4::Datafile datafilev4(reader, m_fileList);

            for (RSDKv4::Datafile::FileInfo &file : datafilev4.m_files) {
                FileInfo info;
                info.m_filename  = file.m_filename;
                info.m_fileSize  = file.m_fileSize;
                info.m_filedata  = file.m_filedata;
                info.m_encrypted = file.m_encrypted;
                m_files.append(info);

                ui->fileList->addItem(file.m_filename);
            }
            break;
        }
        case 2: // RSDKv3
        {
            RSDKv3::Datafile datafilev3(reader);

            for (RSDKv3::Datafile::FileInfo &file : datafilev3.m_files) {
                FileInfo info;
                info.m_filename  = file.m_fullFilename;
                info.m_fileSize  = file.m_fileSize;
                info.m_filedata  = file.m_filedata;
                info.m_encrypted = false;
                m_files.append(info);

                ui->fileList->addItem(file.m_fullFilename);
            }
            break;
        }
        case 3: // RSDKv2
        {
            RSDKv2::Datafile datafilev2(reader);

            for (RSDKv2::Datafile::FileInfo &file : datafilev2.m_files) {
                FileInfo info;
                info.m_filename  = file.m_fullFilename;
                info.m_fileSize  = file.m_fileSize;
                info.m_filedata  = file.m_filedata;
                info.m_encrypted = false;
                m_files.append(info);

                ui->fileList->addItem(file.m_fullFilename);
            }
            break;
        }
        case 4: // RSDKv1
        {
            RSDKv1::Datafile datafilev1(reader);

            for (RSDKv1::Datafile::FileInfo &file : datafilev1.m_files) {
                FileInfo info;
                info.m_filename  = file.m_fullFilename;
                info.m_fileSize  = file.m_fileSize;
                info.m_filedata  = file.m_filedata;
                info.m_encrypted = false;
                m_files.append(info);

                ui->fileList->addItem(file.m_fullFilename);
            }
            break;
        }
        case 5: // ARC
        {
            RSDKv3::ArcContainer arcContainer(reader);

            for (RSDKv3::ArcContainer::FileInfo &file : arcContainer.m_files) {
                FileInfo info;
                info.m_filename  = file.m_name;
                info.m_fileSize  = file.m_fileSize;
                info.m_filedata  = file.m_fileData;
                info.m_encrypted = false;
                m_files.append(info);

                ui->fileList->addItem(file.m_name);
            }
            break;
        }
    }

    ui->fileList->blockSignals(false);

    ui->fileList->setCurrentRow(-1);

    appConfig.addRecentFile(m_gameVer, TOOL_RSDKUNPACKER, filepath, QList<QString>{ fileList });

    setStatus("Datafile loaded successfully!");
}

void RSDKUnpacker::savePack(QString filepath, byte ver)
{

    QList<FileInfo> files = m_files;
    QList<QString> dirs;
    std::sort(files.begin(), files.end(),
              [](const FileInfo &a, const FileInfo &b) -> bool { return a.m_filename < b.m_filename; });

    switch (ver) {
        case 0: // RSDKv5
        {
            RSDKv5::Datafile datafile;

            datafile.m_files.clear();
            for (FileInfo &file : files) {
                RSDKv5::Datafile::FileInfo info;
                info.m_filename  = file.m_filename;
                info.m_fileSize  = file.m_fileSize;
                info.m_filedata  = file.m_filedata;
                info.m_encrypted = file.m_encrypted;
                datafile.m_files.append(info);
            }

            datafile.write(filepath);
            break;
        }
        case 1: // RSDKv4
        {
            RSDKv4::Datafile datafile;

            datafile.m_files.clear();
            for (FileInfo &file : files) {
                RSDKv4::Datafile::FileInfo info;
                info.m_filename  = file.m_filename;
                info.m_fileSize  = file.m_fileSize;
                info.m_filedata  = file.m_filedata;
                info.m_encrypted = file.m_encrypted;
                datafile.m_files.append(info);
            }

            datafile.write(filepath);
            break;
        }
        case 2: // RSDKv3
        {
            RSDKv3::Datafile datafile;

            datafile.m_files.clear();
            datafile.m_directories.clear();
            int dirID = -1;
            for (FileInfo &file : files) {
                QString dir = file.m_filename;
                dir         = dir.replace(QFileInfo(file.m_filename).fileName(), "");
                if (dirs.indexOf(dir) < 0) {
                    dirs.append(dir);
                    ++dirID;
                }

                RSDKv3::Datafile::FileInfo info;
                info.m_filename = QFileInfo(file.m_filename).fileName();
                info.m_fileSize = file.m_fileSize;
                info.m_filedata = file.m_filedata;
                info.m_dirID    = dirID;
                datafile.m_files.append(info);
            }

            for (QString &dir : dirs) {
                RSDKv3::Datafile::DirInfo d;
                d.m_directory = dir;
                datafile.m_directories.append(d);
            }

            datafile.write(filepath);
            break;
        }
        case 3: // RSDKv2
        {
            RSDKv2::Datafile datafile;

            datafile.m_files.clear();
            datafile.m_directories.clear();
            int dirID = -1;
            for (FileInfo &file : files) {
                QString dir = file.m_filename;
                dir         = dir.replace(QFileInfo(file.m_filename).fileName(), "");
                if (dirs.indexOf(dir) < 0) {
                    dirs.append(dir);
                    ++dirID;
                }

                RSDKv2::Datafile::FileInfo info;
                info.m_filename = QFileInfo(file.m_filename).fileName();
                info.m_fileSize = file.m_fileSize;
                info.m_filedata = file.m_filedata;
                info.m_dirID    = dirID;
                datafile.m_files.append(info);
            }

            for (QString &dir : dirs) {
                RSDKv2::Datafile::DirInfo d;
                d.m_directory = dir;
                datafile.m_directories.append(d);
            }

            datafile.write(filepath);
            break;
        }
        case 4: // RSDKv1
        {
            RSDKv1::Datafile datafile;

            datafile.m_files.clear();
            datafile.m_directories.clear();
            int dirID = -1;
            for (FileInfo &file : files) {
                QString dir = file.m_filename;
                dir         = dir.replace(QFileInfo(file.m_filename).fileName(), "");
                if (dirs.indexOf(dir) < 0) {
                    dirs.append(dir);
                    ++dirID;
                }

                RSDKv1::Datafile::FileInfo info;
                info.m_filename = QFileInfo(file.m_filename).fileName();
                info.m_fileSize = file.m_fileSize;
                info.m_filedata = file.m_filedata;
                info.m_dirID    = dirID;
                datafile.m_files.append(info);
            }

            for (QString &dir : dirs) {
                RSDKv1::Datafile::DirInfo d;
                d.m_directory = dir;
                datafile.m_directories.append(d);
            }

            datafile.write(filepath);
            break;
        }
        case 5: // Arc
        {
            RSDKv3::ArcContainer container;

            container.m_files.clear();
            for (FileInfo &file : files) {
                RSDKv3::ArcContainer::FileInfo info;
                info.m_name     = file.m_filename;
                info.m_fileSize = file.m_fileSize;
                info.m_fileData = file.m_filedata;
                container.m_files.append(info);
            }

            container.write(filepath);
            break;
        }
    }
}

#include "moc_rsdkunpacker.cpp"
