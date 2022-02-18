#include "includes.hpp"
#include "ui_rsvtool.h"

RSVTool::RSVTool(QWidget *parent) : QDialog(parent), ui(new Ui::RSVTool)
{
    ui->setupUi(this);

    // remove question mark from the title bar & disable resizing
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    this->setFixedSize(QSize(this->width(), this->height()));

    ui->selExtractFolder->setDisabled(true);
    ui->saveRSV->setDisabled(true);

    connect(ui->selRSV, &QPushButton::pressed, [this] {
        QFileDialog filedialog(this, tr("Open RSV file"), "", tr("Retro-Sonic Video files (*.rsv)"));
        filedialog.setAcceptMode(QFileDialog::AcceptOpen);
        if (filedialog.exec() == QDialog::Accepted) {
            rsvPath = filedialog.selectedFiles()[0];

            ui->selExtractFolder->setDisabled(!QFile::exists(rsvPath));
        }
    });
    connect(ui->selExtractFolder, &QPushButton::pressed, [this] {
        QFileDialog filedialog(this, tr("Select Extraction folder"), "", "");
        filedialog.setFileMode(QFileDialog::Directory);
        if (filedialog.exec() == QDialog::Accepted) {
            QString path = filedialog.selectedFiles()[0];
            QDir(QDir::tempPath()).mkpath(path + "/Frames/");
            setStatus("Loading RSV...", true);
            RSDKv3::Video rsv(rsvPath);

            int id = 0;
            setStatus("Extracting Frames...", true);
            for (auto &f : rsv.frames) {
                f.write(QString(path + "/Frames/Frame%1.gif").arg(id++, 6, 10, QLatin1Char('0')));
                setStatusProgress((float)id / rsv.frames.count());
            }
            setStatus(QString("Extracted %1 Frames to: %2/Frames/").arg(rsv.frames.count()).arg(path));
        }
    });

    connect(ui->selFrames, &QPushButton::pressed, [this] {
        QFileDialog filedialog(this, tr("Select Frames folder"), "", "");
        filedialog.setFileMode(QFileDialog::Directory);
        if (filedialog.exec() == QDialog::Accepted) {
            framesPath = filedialog.selectedFiles()[0];
            ui->saveRSV->setDisabled(!QDir().exists(framesPath));
        }
    });

    connect(ui->saveRSV, &QPushButton::pressed, [this] {
        QFileDialog filedialog(this, tr("Open RSV file"), "", tr("Retro-Sonic Video files (*.rsv)"));
        filedialog.setAcceptMode(QFileDialog::AcceptSave);
        if (filedialog.exec() == QDialog::Accepted) {
            QString savePath = filedialog.selectedFiles()[0];

            QList<QString> framePaths;

            QDirIterator framesDir(framesPath, QStringList() << "*.gif", QDir::Files,
                                   QDirIterator::Subdirectories);
            while (framesDir.hasNext()) {
                QString file = framesDir.next();
                framePaths.append(file);
            }

            std::sort(framePaths.begin(), framePaths.end(),
                      [](const QString &a, const QString &b) -> bool { return a < b; });

            setStatus("Importing Frames...", true);
            RSDKv3::Video rsv;
            int w = -1, h = -1;

            for (QString path : framePaths) {
                FormatHelpers::Gif frame;
                frame.read(path);

                if (w == -1)
                    w = rsv.width = frame.width;
                // else if (w != frame.width)
                //     throw new Exception($ "Frame sizes have to match! {f.Name} width did not
                //     match!");

                if (h == -1)
                    h = rsv.height = frame.height;
                // else if (h != frame.height)
                //     throw new Exception($ "Frame sizes have to match! {f.Name} height did not
                //     match!");

                rsv.frames.append(frame);
            }

            setStatus("Saving RSV...", true);
            rsv.write(savePath);
            setStatus(QString("Saved RSV %1").arg(savePath));
        }
    });
}

RSVTool::~RSVTool() { delete ui; }

#include "moc_rsvtool.cpp"
