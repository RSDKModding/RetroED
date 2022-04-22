#include "includes.hpp"
#include "ui_colourdialog.h"

ColorDialog::ColorDialog(PaletteColor color, QWidget *parent)
    : QDialog(parent), ui(new Ui::ColorDialog), m_color(color)
{
    setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    ui->setupUi(this);
    setFixedSize(550, 300);
    ui->oldPreview->setColor(m_color.toQColor());
    // connect(rSlider, &color_widgets::GradientSlider::)
    connect(ui->wheel, &color_widgets::ColorWheel::colorChanged, this, &ColorDialog::setColor);
    connect(ui->hSlider, &QSlider::valueChanged, this, &ColorDialog::setHsv);
    connect(ui->sSlider, &QSlider::valueChanged, this, &ColorDialog::setHsv);
    connect(ui->vSlider, &QSlider::valueChanged, this, &ColorDialog::setHsv);
    connect(ui->hSpin, SIGNAL(valueChanged(int)), ui->hSlider,
            SLOT(setValue(int))); // fucking new style syntax not working
    connect(ui->sSpin, SIGNAL(valueChanged(int)), ui->sSlider, SLOT(setValue(int)));
    connect(ui->vSpin, SIGNAL(valueChanged(int)), ui->vSlider, SLOT(setValue(int)));
    connect(ui->rSlider, &QSlider::valueChanged, this, &ColorDialog::setRGB);
    connect(ui->gSlider, &QSlider::valueChanged, this, &ColorDialog::setRGB);
    connect(ui->bSlider, &QSlider::valueChanged, this, &ColorDialog::setRGB);
    connect(ui->rSpin, SIGNAL(valueChanged(int)), ui->rSlider, SLOT(setValue(int)));
    connect(ui->gSpin, SIGNAL(valueChanged(int)), ui->gSlider, SLOT(setValue(int)));
    connect(ui->bSpin, SIGNAL(valueChanged(int)), ui->bSlider, SLOT(setValue(int)));

    connect(ui->htmlInput, &color_widgets::ColorLineEdit::colorEdited,
            [=](QColor color) { setColor(color); });
    connect(ui->htmlInput, &color_widgets::ColorLineEdit::colorEditingFinished, [=](QColor color) {
        ui->htmlInput->setModified(false);
        setColor(color);
    });
    setColor(m_color.toQColor());
}

void ColorDialog::setHsv()
{
    if (signalsBlocked())
        return;
    QColor col =
        QColor::fromHsv(ui->hSlider->value(), ui->sSlider->value(), ui->vSlider->value(), 0xFF);
    setColor(col);
}

void ColorDialog::setRGB()
{
    if (signalsBlocked())
        return;
    QColor col(ui->rSlider->value(), ui->gSlider->value(), ui->bSlider->value(), 0xFF);
    if (col.saturation() == 0)
        col = QColor::fromHsv(ui->hSlider->value(), 0, col.value());
    setColor(col);
}

void ColorDialog::setColor(QColor col)
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

ColorDialog::~ColorDialog() { delete ui; }

PaletteColor ColorDialog::getColor(PaletteColor color, bool *ok, QWidget *parent)
{
    PaletteColor clr = PaletteColor(color);
    ColorDialog c(clr, parent);
    *ok = (c.exec() == DialogCode::Accepted);
    return clr;
}

#include "moc_colourdialog.cpp"
