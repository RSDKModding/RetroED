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
            type    = gfxTypes.indexOf(filedialog.selectedNameFilter());
            gfxPath = filedialog.selectedFiles()[0];
            imgPath = "";
        }
    });

    connect(ui->exportImage, &QPushButton::clicked, [this, imgTypes] {
        if (gfxPath == "")
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
            gfx.read(gfxPath, type == 1);
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
            type    = imgTypes.indexOf(filedialog.selectedNameFilter());
            imgPath = filedialog.selectedFiles()[0];
            gfxPath = "";
        }
    });

    connect(ui->exportGFX, &QPushButton::clicked, [this, gfxTypes] {
        if (imgPath == "")
            return;

        QFileDialog filedialog(
            this, tr("Save GFX Image"), "",
            tr(QString("%1;;%2").arg(gfxTypes[0]).arg(gfxTypes[1]).toStdString().c_str()));
        filedialog.setAcceptMode(QFileDialog::AcceptSave);
        if (filedialog.exec() == QDialog::Accepted) {
            int filter = gfxTypes.indexOf(filedialog.selectedNameFilter());

            RSDKv1::GFX gfx;
            if (type == 0) {
                QGifImage gif(imgPath);
                gfx.importImage(gif.frame(0));
            }
            else {
                gfx.importImage(QImage(imgPath));
            }
            gfx.write(filedialog.selectedFiles()[0], filter == 1);
        }
    });
}

GFXTool::~GFXTool() { delete ui; }

#include "moc_gfxtool.cpp"
