#include "includes.hpp"
#include "ui_paletteimport.h"
#include "paletteimport.hpp"

PaletteImport::PaletteImport(QList<PaletteColor> import, QList<PaletteColor> &base, bool enableBanks, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PaletteImport)
{
    if (!import.count() || !base.count())
        return;
    setWindowFlag(Qt::WindowStaysOnTopHint);
    ui->setupUi(this);

    this->setWindowTitle("Palette Import");

    // remove question mark from the title bar
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);


    QToolButton *bankSwitches[] = { ui->bank1, ui->bank2, ui->bank3, ui->bank4,
                                    ui->bank5, ui->bank6, ui->bank7, ui->bank8 };
    if (enableBanks)
        for (int b = 0; b < 8; ++b) bankSwitches[b]->setEnabled(true);

    importPalWidget = new ViewerWidget(this);
    curPalwidget    = new ViewerWidget(this);
    importPalWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    curPalwidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    curPalwidget->palette    = &base;
    storedPal                = base;
    curPalwidget->palSize    = base.count();

    ui->importFrame->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->importFrame->setDragDropMode(QListView::NoDragDrop);
    ui->curFrame->layout()->addWidget(curPalwidget);

    for (int c = 0; c < import.count(); c++){
        auto *item = new QListWidgetItem(NULL, ui->importFrame);
        QImage swatch(16, 16, QImage::Format_RGB888);
        PaletteColor test = import.at(c);
        swatch.fill(test.toQColor().rgb());
        item->setIcon(QPixmap::fromImage(swatch));
    }

    connect(ui->importFrame, &QListWidget::itemSelectionChanged,[this, import, enableBanks]{
        int c = ui->importFrame->currentRow();
        auto item = ui->importFrame->item(c);

        item->setSelected(true);


        curPalwidget->blockSignals(true);
        curPalwidget->importPalette.clear();

        if (enableBanks){
            QList<PaletteColor> bankPal;
            for (int r = 0; r < 16; ++r) {
                for (int c = 0; c < 16; ++c) {
                    bankPal.append(PaletteColor(banks[activeBank].colors[r][c].red(),
                                                banks[activeBank].colors[r][c].green(),
                                                banks[activeBank].colors[r][c].blue()));
                }
            }
            if (ui->importFrame->selectedItems().count() > 1){
                for (int s = 0; s < import.count(); s++){
                    if (ui->importFrame->item(s)->isSelected()){
                        curPalwidget->importPalette.append(bankPal.at(s));
                    }
                }
            } else
                curPalwidget->importPalette.append(import.at(c));
        } else {
            if (ui->importFrame->selectedItems().count() > 1){
                for (int s = 0; s < import.count(); s++){
                    if (ui->importFrame->item(s)->isSelected()){
                        curPalwidget->importPalette.append(import.at(s));
                    }
                }
            } else
                curPalwidget->importPalette.append(import.at(c));
        }
        curPalwidget->blockSignals(false);
        curPalwidget->update();
    });

    connect(curPalwidget, &ViewerWidget::pastePalette, [this](byte x, byte y){
        int index = 0;
        int startPos = (y * 0x10) + x;
        for (int s = startPos; s < startPos + curPalwidget->importPalette.count(); s++){
            if (s >= curPalwidget->palette->count())
                break;
            curPalwidget->palette->replace(s, curPalwidget->importPalette.at(index++));
        }
        curPalwidget->update();
    });

    connect(ui->restorePal, &QPushButton::clicked, [this]{
        for (int s = 0; s < curPalwidget->palette->count(); s++)
            curPalwidget->palette->replace(s, storedPal.at(s));
        curPalwidget->update();
    });

    for (int b = 0; b < 8; ++b) {
        connect(bankSwitches[b], &QToolButton::clicked, [this, b] {
            ui->importFrame->clear();
            for (int r = 0; r < 16; ++r) {
                for (int c = 0; c < 16; ++c) {
                    auto *item = new QListWidgetItem(NULL, ui->importFrame);
                    QImage swatch(16, 16, QImage::Format_RGB888);
                    PaletteColor test = banks[b].colors[r][c];
                    swatch.fill(test.toQColor().rgb());
                    item->setIcon(QPixmap::fromImage(swatch));
                }
            }
            activeBank = b;
        });
    }
}

PaletteImport::~PaletteImport(){ delete ui; }


bool ViewerWidget::event(QEvent *e)
{
    switch (e->type()) {
        default: break;
        case QEvent::Paint: {
            paintEvent((QPaintEvent *)e);
            break;
        }
        case QEvent::MouseButtonPress: {
            QMouseEvent *mEvent = static_cast<QMouseEvent *>(e);
            byte x = floor((float)(mEvent->x() / ((qreal)width() / 16)));
            byte y = floor((float)(mEvent->y() / ((qreal)height() / 16)));
            emit pastePalette(x, y);
            break;
        }
    }
    return QWidget::event(e);
}

void ViewerWidget::leaveEvent(QEvent *)
{
    row = -1;
    col = -1;
    update();
}

void ViewerWidget::mouseReleaseEvent(QMouseEvent *) { pressed = false; }

void ViewerWidget::mousePressEvent(QMouseEvent *event)
{
    enabling  = (event->button() & Qt::LeftButton) == Qt::LeftButton;
    pressed   = true;
    update();
}


void ViewerWidget::mouseDoubleClickEvent(QMouseEvent *){}

void ViewerWidget::mouseMoveEvent(QMouseEvent *event)
{
    byte x = floor((float)(event->x() / ((qreal)width() / 16)));
    byte y = floor((float)(event->y() / ((qreal)height() / 16)));

    if (x > 15)
        x = 15;

    if (y > 15)
        y = 15;

    row = x;
    col = y;
    update();
}

void ViewerWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);

    QRectF rect(0, 0, (qreal)width() / 16, (qreal)height() / 16);
    short index = -1;
    QPen pen(qApp->palette().base(), 2);
    p.setPen(pen);
    PaletteColor clr;

    if (!palette)
        return;

    for (byte y = 0; y < palSize / 0x10; ++y) {
        for (byte x = 0; x < 0x10; ++x) {
            ++index;
            if (index >= palSize)
                return;

            clr = palette->at(index);
            p.setBrush(clr.toQColor());

            p.drawRect(rect.translated(x * (qreal)width() / 16, y * (qreal)height() / 16));
        }
    }
    if (col == -1 || row == -1)
        return;
    short storeIndex = 0;
    for (byte y = col; y < palSize / 0x10; ++y) {
        byte x = y == col ? row : 0;
        for (; x < 0x10; ++x) {
            if (storeIndex >= importPalette.count())
                return;
            clr = importPalette.at(storeIndex);
            storeIndex++;
            p.setBrush(clr.toQColor());
            p.drawRect(rect.translated(x * (qreal)width() / 16, y * (qreal)height() / 16));
        }
    }
}
