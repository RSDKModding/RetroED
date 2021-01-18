#include "includes.hpp"
#include "ui_gfxtool.h"

#include "dependencies/qtgifimage/src/gifimage/qgifimage.h"

GFXTool::GFXTool(QWidget *parent) : QWidget(parent), ui(new Ui::GFXTool)
{
    ui->setupUi(this);

    QList<QString> imgTypes = { "GIF Images (*.gif)", "BMP Images (*.bmp)", "PNG Images (*.png)" };

    QList<QString> gfxTypes = { "Retro-Sonic (2007, PC) GFX Images (*.gfx)",
                                "Retro-Sonic (2006, Dreamcast) GFX Images (*.gfx)" };

    connect(ui->importGFX, &QPushButton::clicked, [this, gfxTypes] {
        QFileDialog filedialog(
            this, tr("Open GFX Image"), "",
            tr(QString("%1;;%2").arg(gfxTypes[0]).arg(gfxTypes[1]).toStdString().c_str()));
        filedialog.setAcceptMode(QFileDialog::AcceptOpen);
        if (filedialog.exec() == QDialog::Accepted) {
            m_type    = gfxTypes.indexOf(filedialog.selectedNameFilter());
            m_gfxPath = filedialog.selectedFiles()[0];
            m_imgPath = "";
            appConfig.addRecentFile(ENGINE_v1, TOOL_GFXTOOL, m_gfxPath, QList<QString>{ /**/ });
        }
    });

    connect(ui->exportImage, &QPushButton::clicked, [this, imgTypes] {
        if (m_gfxPath == "")
            return;

        QFileDialog filedialog(this, tr("Save Image"), "",
                               tr(QString("%1;;%2;;%3")
                                      .arg(imgTypes[0])
                                      .arg(imgTypes[1])
                                      .arg(imgTypes[2])
                                      .toStdString()
                                      .c_str()));
        filedialog.setAcceptMode(QFileDialog::AcceptSave);
        if (filedialog.exec() == QDialog::Accepted) {
            int filter = imgTypes.indexOf(filedialog.selectedNameFilter());

            RSDKv1::GFX gfx;
            gfx.read(m_gfxPath, m_type == 1);
            if (filter == 0) {
                QGifImage gif;
                gif.addFrame(QImage(gfx.exportImage()));
                gif.save(filedialog.selectedFiles()[0]);
            }
            else {
                QImage img = gfx.exportImage();
                img.save(filedialog.selectedFiles()[0]);
            }
        }
    });

    connect(ui->ImportImage, &QPushButton::clicked, [this, imgTypes] {
        QFileDialog filedialog(this, tr("Open Image"), "",
                               tr(QString("%1;;%2;;%3")
                                      .arg(imgTypes[0])
                                      .arg(imgTypes[1])
                                      .arg(imgTypes[2])
                                      .toStdString()
                                      .c_str()));
        filedialog.setAcceptMode(QFileDialog::AcceptOpen);
        if (filedialog.exec() == QDialog::Accepted) {
            m_type    = imgTypes.indexOf(filedialog.selectedNameFilter());
            m_imgPath = filedialog.selectedFiles()[0];
            m_gfxPath = "";
        }
    });

    connect(ui->exportGFX, &QPushButton::clicked, [this, gfxTypes] {
        if (m_imgPath == "")
            return;

        QFileDialog filedialog(
            this, tr("Save GFX Image"), "",
            tr(QString("%1;;%2").arg(gfxTypes[0]).arg(gfxTypes[1]).toStdString().c_str()));
        filedialog.setAcceptMode(QFileDialog::AcceptSave);
        if (filedialog.exec() == QDialog::Accepted) {
            int filter = gfxTypes.indexOf(filedialog.selectedNameFilter());

            RSDKv1::GFX gfx;
            if (m_type == 0) {
                QGifImage gif(m_imgPath);
                gfx.importImage(gif.frame(0));
            }
            else {
                gfx.importImage(QImage(m_imgPath));
            }
            gfx.write(filedialog.selectedFiles()[0], filter == 1);
        }
    });
}

GFXTool::~GFXTool() { delete ui; }

#include "moc_gfxtool.cpp"
