#include "includes.hpp"
#include "ui_animsheetselector.h"

#include "dependencies/imageviewer/src/image-viewer.h"
#include <QTimer>

AnimSheetSelector::AnimSheetSelector(QString sheetPath, QImage *sheet, QWidget *parent)
    : QDialog(parent), ui(new Ui::AnimSheetSelector), sheetPath(sheetPath), sheet(sheet)
{
    ui->setupUi(this);
    this->setWindowTitle("Select Bounding Box");

    pal::ImageViewer *viewer = new pal::ImageViewer();
    viewer->setText(sheetPath);
    // viewer->setToolBarMode(pal::ImageViewer::ToolBarMode::Hidden);
    ui->gridLayout->addWidget(viewer);
    viewer->setImage(*sheet);
    QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    ui->gridLayout->addWidget(buttons);

    connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);

    connect(viewer->pixmapItem(), &pal::PixmapItem::mouseDownL, [this, viewer] {
        mouseDownL = true;
        selecting ^= 1;
        if (selecting) {
            returnRect.x = mousePos.x;
            returnRect.y = mousePos.y;
            returnRect.w = returnRect.h = 0;
            viewer->pixmapItem()->rect.setX(returnRect.x);
            viewer->pixmapItem()->rect.setY(returnRect.y);
            viewer->pixmapItem()->rect.setWidth(returnRect.w);
            viewer->pixmapItem()->rect.setHeight(returnRect.h);
            printLog(QString("ClickPos: X: %1, Y: %2, W: %3, H: %4")
                         .arg(returnRect.x)
                         .arg(returnRect.y)
                         .arg(returnRect.w)
                         .arg(returnRect.h));
        }
        else {
            returnRect.w = mousePos.x - returnRect.x;
            returnRect.h = mousePos.y - returnRect.y;

            if (returnRect.w < 0) {
                returnRect.x -= abs(returnRect.w);
                returnRect.w = abs(returnRect.w);
                viewer->pixmapItem()->rect.setX(returnRect.x);
            }

            if (returnRect.h < 0) {
                returnRect.y -= abs(returnRect.h);
                returnRect.h = abs(returnRect.h);
                viewer->pixmapItem()->rect.setY(returnRect.y);
            }

            viewer->pixmapItem()->rect.setWidth(returnRect.w);
            viewer->pixmapItem()->rect.setHeight(returnRect.h);
            printLog(QString("RectPos: X: %1, Y: %2, W: %3, H: %4")
                         .arg(returnRect.x)
                         .arg(returnRect.y)
                         .arg(returnRect.w)
                         .arg(returnRect.h));
        }
        viewer->pixmapItem()->update();
    });
    connect(viewer->pixmapItem(), &pal::PixmapItem::mouseUpL, [this] { mouseDownL = false; });
    connect(viewer->pixmapItem(), &pal::PixmapItem::mouseDownM, [this] { mouseDownM = true; });
    connect(viewer->pixmapItem(), &pal::PixmapItem::mouseUpM, [this] { mouseDownM = false; });
    connect(viewer->pixmapItem(), &pal::PixmapItem::mouseDownR, [this, viewer] {
        mouseDownR = true;
        selecting  = false;
        int mouseX = mousePos.x;
        int mouseY = mousePos.y;
        int X = 0, Y = 0, W = 0, H = 0;

        QColor bgColor = QColor(0xFFFFFFFF);
        if (this->sheet->format() == QImage::Format_Indexed8) {
            // last color in the palette (should) always be bg
            // bgColor = this->sheet->colorTable().last();
        }

        int px = mouseX;
        while (true) {
            if (this->sheet->pixelColor(px, mouseY) == bgColor)
                break;
            else {
                X--;
                if (--px < 0)
                    break;
            }
        }

        px = mouseX;
        while (true) {
            if (this->sheet->pixelColor(px, mouseY) == bgColor)
                break;
            else {
                W++;
                if (++px >= this->sheet->width())
                    break;
            }
        }

        int py = mouseY;
        while (true) {
            if (this->sheet->pixelColor(mouseX, py) == bgColor)
                break;
            else {
                Y--;
                if (--py < 0)
                    break;
            }
        }

        py = mouseY;
        while (true) {
            if (this->sheet->pixelColor(mouseX, py) == bgColor)
                break;
            else {
                H++;
                if (++py >= this->sheet->height())
                    break;
            }
        }

        ++X;
        ++Y;
        returnRect = Rect<int>(mouseX + X, mouseY + Y, abs(X) + W, abs(Y) + H);
        viewer->pixmapItem()->rect.setX(returnRect.x);
        viewer->pixmapItem()->rect.setY(returnRect.y);
        viewer->pixmapItem()->rect.setWidth(returnRect.w);
        viewer->pixmapItem()->rect.setHeight(returnRect.h);
        printLog(QString("AutoPos: X: %1, Y: %2, W: %3, H: %4")
                     .arg(returnRect.x)
                     .arg(returnRect.y)
                     .arg(returnRect.w)
                     .arg(returnRect.h));

        viewer->pixmapItem()->update();
    });
    connect(viewer->pixmapItem(), &pal::PixmapItem::mouseUpR, [this] { mouseDownR = false; });

    connect(viewer->pixmapItem(), &pal::PixmapItem::mouseMoved, [this, viewer](int x, int y) {
        mousePos.x = x;
        mousePos.y = y;

        if (selecting) {
            returnRect.w = mousePos.x - returnRect.x;
            returnRect.h = mousePos.y - returnRect.y;

            if (returnRect.w < 0) {
                returnRect.x -= abs(returnRect.w);
                returnRect.w = abs(returnRect.w);
                viewer->pixmapItem()->rect.setX(returnRect.x);
            }

            if (returnRect.h < 0) {
                returnRect.y -= abs(returnRect.h);
                returnRect.h = abs(returnRect.h);
                viewer->pixmapItem()->rect.setY(returnRect.y);
            }

            viewer->pixmapItem()->rect.setWidth(returnRect.w);
            viewer->pixmapItem()->rect.setHeight(returnRect.h);
        }

        viewer->pixmapItem()->update();
    });
}

AnimSheetSelector::~AnimSheetSelector() { delete ui; }

void AnimSheetSelector::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
        case QEvent::LanguageChange: ui->retranslateUi(this); break;
        default: break;
    }
}

#include "moc_animsheetselector.cpp"
