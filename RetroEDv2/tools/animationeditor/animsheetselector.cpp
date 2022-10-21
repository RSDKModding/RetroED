#include "includes.hpp"
#include "ui_animsheetselector.h"

#include "animsheetselector.hpp"

#include "tools/paletteeditor/colourdialog.hpp"

#include "dependencies/imageviewer/src/image-viewer.hpp"
#include <QTimer>

AnimSheetSelector::AnimSheetSelector(QString sheetPath, QImage *sheet, QWidget *parent)
    : QDialog(parent), ui(new Ui::AnimSheetSelector), sheetPath(sheetPath), sheet(sheet)
{
    ui->setupUi(this);
    this->setWindowTitle("Select Bounding Box");

    pal::ImageViewer *viewer = new pal::ImageViewer();
    viewer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    viewer->setText(sheetPath);
    // viewer->setToolBarMode(pal::ImageViewer::ToolBarMode::Hidden);
    viewer->setImage(*sheet);
    ui->viewerFrame->layout()->addWidget(viewer);
    ui->colorButton->setMaximumWidth(ui->colorButton->height());

    bgColor = QColor(0xFFFFFFFF);
    if (this->sheet->format() == QImage::Format_Indexed8) {
        // last color in the palette (should) always be bg
        bgColor = this->sheet->colorTable().last();
    }

    QPalette pal(bgColor);
    ui->colorButton->setPalette(pal);

    connect(ui->colorButton, &QPushButton::pressed, [this] {
        RSDKColorDialog dlg(bgColor);
        if (dlg.exec() == QDialog::Accepted) {
            bgColor = dlg.color().toQColor();
        }
        QPalette pal(bgColor);
        ui->colorButton->setPalette(pal);
    });

    connect(ui->buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(ui->buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);

    connect(viewer->view, &pal::GraphicsView::mouseDownL, [this, viewer] {
        mouseDownL = true;

        returnRect.x = mousePos.x;
        returnRect.y = mousePos.y;
        returnRect.w = returnRect.h = 0;
        viewer->pixmapItem()->rect.setX(returnRect.x);
        viewer->pixmapItem()->rect.setY(returnRect.y);
        viewer->pixmapItem()->rect.setWidth(returnRect.w);
        viewer->pixmapItem()->rect.setHeight(returnRect.h);
        PrintLog(QString("ClickPos: X: %1, Y: %2, W: %3, H: %4")
                     .arg(returnRect.x)
                     .arg(returnRect.y)
                     .arg(returnRect.w)
                     .arg(returnRect.h));

        viewer->pixmapItem()->update();
    });
    connect(viewer->view, &pal::GraphicsView::mouseUpL, [this] { 
        mouseDownL = false; 
        });
    connect(viewer->view, &pal::GraphicsView::mouseDownM, [this] { mouseDownM = true; });
    connect(viewer->view, &pal::GraphicsView::mouseUpM, [this] { mouseDownM = false; });
    connect(viewer->view, &pal::GraphicsView::mouseDownR, [this, viewer] {
        mouseDownR = true;
        selecting  = false;
        int mouseX = mousePos.x;
        int mouseY = mousePos.y;
        int X = 0, Y = 0, W = 0, H = 0;

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
        // check top border
        if (returnRect.y) {
            bool swap = this->sheet->pixelColor(returnRect.x, returnRect.y - 1) != bgColor;
            for (int i = 1; i < returnRect.w; ++i) {
                if ((this->sheet->pixelColor(returnRect.x + i, returnRect.y - 1) == bgColor) == swap) {
                    if (swap) {
                        returnRect.x += i;
                        returnRect.w -= i;
                    }
                    else
                        returnRect.w = i - 1;
                    break;
                }
            }
        }

        // bottom border
        if (returnRect.y + returnRect.h < this->sheet->height()) {
            bool swap = this->sheet->pixelColor(returnRect.x, returnRect.y + returnRect.h) != bgColor;
            for (int i = 1; i < returnRect.w; ++i) {
                if ((this->sheet->pixelColor(returnRect.x + i, returnRect.y + returnRect.h) == bgColor)
                    == swap) {
                    if (swap) {
                        returnRect.x += i;
                        returnRect.w -= i;
                    }
                    else
                        returnRect.w = i - 1;
                    break;
                }
            }
        }

        // left border
        if (returnRect.x) {
            bool swap = this->sheet->pixelColor(returnRect.x - 1, returnRect.y) != bgColor;
            for (int i = 1; i < returnRect.h; ++i) {
                if ((this->sheet->pixelColor(returnRect.x - 1, returnRect.y + i) == bgColor) == swap) {
                    if (swap) {
                        returnRect.y += i;
                        returnRect.h -= i;
                    }
                    else
                        returnRect.h = i - 1;
                    break;
                }
            }
        }

        // right border
        if (returnRect.x + returnRect.w < this->sheet->height()) {
            bool swap = this->sheet->pixelColor(returnRect.x + returnRect.w, returnRect.y) != bgColor;
            for (int i = 1; i < returnRect.h; ++i) {
                if ((this->sheet->pixelColor(returnRect.x + returnRect.w, returnRect.y + i) == bgColor)
                    == swap) {
                    if (swap) {
                        returnRect.y += i;
                        returnRect.h -= i;
                    }
                    else
                        returnRect.h = i - 1;
                    break;
                }
            }
        }

        viewer->pixmapItem()->rect.setX(returnRect.x);
        viewer->pixmapItem()->rect.setY(returnRect.y);
        viewer->pixmapItem()->rect.setWidth(returnRect.w);
        viewer->pixmapItem()->rect.setHeight(returnRect.h);
        PrintLog(QString("AutoPos: X: %1, Y: %2, W: %3, H: %4")
                     .arg(returnRect.x)
                     .arg(returnRect.y)
                     .arg(returnRect.w)
                     .arg(returnRect.h));

        viewer->pixmapItem()->update();
    });
    connect(viewer->view, &pal::GraphicsView::mouseUpR, [this] { mouseDownR = false; });

    connect(viewer->pixmapItem(), &pal::PixmapItem::mouseMoved, [this, viewer](int x, int y) {
        mousePos.x = x;
        mousePos.y = y;

        if (mouseDownL) {
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
