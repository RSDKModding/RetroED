#include "includes.hpp"
#include "ui_rsvtool.h"

RSVTool::RSVTool(QWidget *parent) : QWidget(parent), ui(new Ui::RSVTool)
{
    ui->setupUi(this);

    ui->selExtractFolder->setDisabled(true);
    connect(ui->selRSV, &QPushButton::pressed, [this] {
        QFileDialog filedialog(this, tr("Open RSV file"), "", tr("Retro-Sonic Video files (*.rsv)"));
        filedialog.setAcceptMode(QFileDialog::AcceptOpen);
        if (filedialog.exec() == QDialog::Accepted) {
            m_rsvPath = filedialog.selectedFiles()[0];

            ui->selExtractFolder->setDisabled(!QFile::exists(m_rsvPath));
        }
    });
    connect(ui->selExtractFolder, &QPushButton::pressed, [this] {
        QFileDialog filedialog(this, tr("Select Extraction folder"), "", "");
        filedialog.setFileMode(QFileDialog::Directory);
        if (filedialog.exec() == QDialog::Accepted) {
            QString path = filedialog.selectedFiles()[0];
            QDir(QDir::tempPath()).mkpath(path + "/Frames/");
            RSDKv3::Video rsv(m_rsvPath);

            int id = 0;
            setStatus("Extracting Frames...");
            for (auto &f : rsv.m_frames)
                f.toImage().save(QString(path + "/Frames/Frame%1.png").arg(id++));
            setStatus(
                QString("Extracted %1 Frames to: %2/Frames/").arg(rsv.m_frames.count()).arg(path));
        }
    });
}

RSVTool::~RSVTool() { delete ui; }

#include "moc_rsvtool.cpp"
