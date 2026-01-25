#include "includes.hpp"
#include "ui_colourdialog.h"

#include "colourdialog.hpp"

RSDKColorDialog::RSDKColorDialog(PaletteColor color, QWidget *parent)
    : QDialog(parent), ui(new Ui::RSDKColorDialog), m_color(color)
{
    setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint & ~Qt::WindowStaysOnTopHint);
    ui->setupUi(this);
    setFixedSize(550, 300);
    ui->oldPreview->setColor(m_color.toQColor());
    connect(ui->wheel, &color_widgets::ColorWheel::colorChanged, this, &RSDKColorDialog::setColor);
    connect(ui->hSlider, &QSlider::valueChanged, this, &RSDKColorDialog::setHsv);
    connect(ui->sSlider, &QSlider::valueChanged, this, &RSDKColorDialog::setHsv);
    connect(ui->vSlider, &QSlider::valueChanged, this, &RSDKColorDialog::setHsv);
    connect(ui->hSpin, SIGNAL(valueChanged(int)), ui->hSlider, SLOT(setValue(int)));
    connect(ui->sSpin, SIGNAL(valueChanged(int)), ui->sSlider, SLOT(setValue(int)));
    connect(ui->vSpin, SIGNAL(valueChanged(int)), ui->vSlider, SLOT(setValue(int)));
    connect(ui->rSlider, &QSlider::valueChanged, this, &RSDKColorDialog::setRGB);
    connect(ui->gSlider, &QSlider::valueChanged, this, &RSDKColorDialog::setRGB);
    connect(ui->bSlider, &QSlider::valueChanged, this, &RSDKColorDialog::setRGB);
    connect(ui->rSpin, SIGNAL(valueChanged(int)), ui->rSlider, SLOT(setValue(int)));
    connect(ui->gSpin, SIGNAL(valueChanged(int)), ui->gSlider, SLOT(setValue(int)));
    connect(ui->bSpin, SIGNAL(valueChanged(int)), ui->bSlider, SLOT(setValue(int)));

    connect(ui->htmlInput, &color_widgets::ColorLineEdit::colorEdited,
            [=](QColor color) { setColor(color); });
    connect(ui->htmlInput, &color_widgets::ColorLineEdit::colorEditingFinished, [=](QColor color) {
        ui->htmlInput->setModified(false);
        setColor(color);
    });

    connect(ui->colorPick, &QToolButton::pressed, this, &RSDKColorDialog::pickScreenColor);

    setColor(m_color.toQColor());
}

void RSDKColorDialog::setHsv()
{
    if (signalsBlocked())
        return;
    QColor col =
        QColor::fromHsv(ui->hSlider->value(), ui->sSlider->value(), ui->vSlider->value(), 0xFF);
    setColor(col);
}

void RSDKColorDialog::setRGB()
{
    if (signalsBlocked())
        return;
    QColor col(ui->rSlider->value(), ui->gSlider->value(), ui->bSlider->value(), 0xFF);
    if (col.saturation() == 0)
        col = QColor::fromHsv(ui->hSlider->value(), 0, col.value());
    setColor(col);
}

bool RSDKColorDialog::mouseMoveColorEvent(QMouseEvent *e)
{
    setColor(grabScreenColor(e->globalPos()));
    //PrintLog(QString("Move %1 %2").arg(e->globalPos().x()).arg(e->globalPos().y()));
    return true;
}

bool RSDKColorDialog::mouseReleaseColorEvent(QMouseEvent *e)
{
    setColor(grabScreenColor(e->globalPos()));
    ui->colorPick->setDown(false);

    releaseMouse();
    releaseKeyboard();
    colorPickMode = false;
    setMouseTracking(false);
    return true;
}

bool RSDKColorDialog::keyPressColorEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Escape) {
        setColor(PrevCol.toQColor());
        releaseMouse();
        releaseKeyboard();
        colorPickMode = false;
        setMouseTracking(false);
        ui->colorPick->setDown(false);
        return true;
    }
    return false;
}

bool RSDKColorDialog::event(QEvent *event)
{
    if (colorPickMode){
        switch ((int)event->type()) {
            default: break;
            case QEvent::MouseMove:
                return mouseMoveColorEvent(static_cast<QMouseEvent *>(event));
            case QEvent::MouseButtonRelease:
                return mouseReleaseColorEvent(static_cast<QMouseEvent *>(event));
            case QEvent::KeyPress:
                return keyPressColorEvent(static_cast<QKeyEvent *>(event));
        }
    }
    return QWidget::event(event);
}

void RSDKColorDialog::pickScreenColor()
{
    PrevCol = m_color;

#ifndef QT_NO_CURSOR
    grabMouse(Qt::CrossCursor);
#else
    grabMouse();
#endif

    grabKeyboard();
    setMouseTracking(true);
    colorPickMode = true;
    const QPoint globalPos = QCursor::pos();
    setColor(grabScreenColor(globalPos));
}

void RSDKColorDialog::setColor(QColor col)
{

    bool blocked = signalsBlocked();
    blockSignals(true);

    for (int i = 0; i < findChildren<QWidget *>().length(); i++) {
        findChildren<QWidget *>()[i]->blockSignals(true);
    }

    ui->wheel->setColor(col);

    ui->rSlider->setValue(col.red());
    ui->rSpin->setValue(col.red());
    ui->rSlider->setFirstColor(QColor(0, col.green(), col.blue()));
    ui->rSlider->setFirstColor(QColor(255, col.green(), col.blue()));

    ui->gSlider->setValue(col.green());
    ui->gSpin->setValue(col.green());
    ui->gSlider->setFirstColor(QColor(col.red(), 0, col.blue()));
    ui->gSlider->setFirstColor(QColor(col.red(), 255, col.blue()));

    ui->bSlider->setValue(col.blue());
    ui->bSpin->setValue(col.blue());
    ui->bSlider->setFirstColor(QColor(col.red(), col.green(), 0));
    ui->bSlider->setFirstColor(QColor(col.red(), col.green(), 255));

    ui->hSlider->setValue(qRound(ui->wheel->hue() * 360.0));
    ui->hSlider->setColorSaturation(ui->wheel->saturation());
    ui->hSlider->setColorValue(ui->wheel->value());
    ui->hSpin->setValue(ui->hSlider->value());

    ui->sSlider->setValue(qRound(ui->wheel->saturation() * 255.0));
    ui->sSpin->setValue(ui->sSlider->value());
    ui->sSlider->setFirstColor(QColor::fromHsvF(ui->wheel->hue(), 0, ui->wheel->value()));
    ui->sSlider->setLastColor(QColor::fromHsvF(ui->wheel->hue(), 1, ui->wheel->value()));

    ui->vSlider->setValue(qRound(ui->wheel->value() * 255.0));
    ui->vSpin->setValue(ui->vSlider->value());
    ui->vSlider->setFirstColor(QColor::fromHsvF(ui->wheel->hue(), ui->wheel->saturation(), 0));
    ui->vSlider->setLastColor(QColor::fromHsvF(ui->wheel->hue(), ui->wheel->saturation(), 1));

    ui->newPreview->setColor(col);

    blockSignals(blocked);

    for (int i = 0; i < findChildren<QWidget *>().length(); i++) {
        findChildren<QWidget *>()[i]->blockSignals(false);
    }

    if (!ui->htmlInput->isModified())
        ui->htmlInput->setColor(col);

    ui->htmlInput->setColor(col);
    m_color.r = col.red();
    m_color.g = col.green();
    m_color.b = col.blue();

    // emit selectedColorModified(col);
}

RSDKColorDialog::~RSDKColorDialog() { delete ui; }

PaletteColor RSDKColorDialog::getColor(PaletteColor color, bool *ok, QWidget *parent)
{
    PaletteColor clr = PaletteColor(color);
    RSDKColorDialog c(clr, parent);
    *ok = (c.exec() == DialogCode::Accepted);
    return clr;
}

QColor RSDKColorDialog::grabScreenColor(const QPoint &p)
{
    const QDesktopWidget *desktop = QApplication::desktop();
    const QPixmap pixmap = QGuiApplication::primaryScreen()->grabWindow(desktop->winId(), p.x(), p.y(), 1, 1);
    QImage i = pixmap.toImage();
    return QColor(i.pixel(0, 0));
}


#include "moc_colourdialog.cpp"
