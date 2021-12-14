#include "includes.hpp"
#include "ui_colourdialog.h"

ColourDialog::ColourDialog(PaletteColour colour, QWidget *parent)
    : QDialog(parent), ui(new Ui::ColourDialog), m_colour(colour)
{
    setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    ui->setupUi(this);
    setFixedSize(550, 300);
    ui->oldPreview->setColor(m_colour.toQColor());
    // connect(rSlider, &colour_widgets::GradientSlider::)
    connect(ui->wheel, &color_widgets::ColorWheel::colorChanged, this, &ColourDialog::setColour);
    connect(ui->hSlider, &QSlider::valueChanged, this, &ColourDialog::setHsv);
    connect(ui->sSlider, &QSlider::valueChanged, this, &ColourDialog::setHsv);
    connect(ui->vSlider, &QSlider::valueChanged, this, &ColourDialog::setHsv);
    connect(ui->hSpin, SIGNAL(valueChanged(int)), ui->hSlider,
            SLOT(setValue(int))); // fucking new style syntax not working
    connect(ui->sSpin, SIGNAL(valueChanged(int)), ui->sSlider, SLOT(setValue(int)));
    connect(ui->vSpin, SIGNAL(valueChanged(int)), ui->vSlider, SLOT(setValue(int)));
    connect(ui->rSlider, &QSlider::valueChanged, this, &ColourDialog::setRGB);
    connect(ui->gSlider, &QSlider::valueChanged, this, &ColourDialog::setRGB);
    connect(ui->bSlider, &QSlider::valueChanged, this, &ColourDialog::setRGB);
    connect(ui->rSpin, SIGNAL(valueChanged(int)), ui->rSlider, SLOT(setValue(int)));
    connect(ui->gSpin, SIGNAL(valueChanged(int)), ui->gSlider, SLOT(setValue(int)));
    connect(ui->bSpin, SIGNAL(valueChanged(int)), ui->bSlider, SLOT(setValue(int)));

    connect(ui->htmlInput, &color_widgets::ColorLineEdit::colorEdited,
            [=](QColor color) { setColour(color); });
    connect(ui->htmlInput, &color_widgets::ColorLineEdit::colorEditingFinished, [=](QColor color) {
        ui->htmlInput->setModified(false);
        setColour(color);
    });
    setColour(m_colour.toQColor());
}

void ColourDialog::setHsv()
{
    if (signalsBlocked())
        return;
    QColor col =
        QColor::fromHsv(ui->hSlider->value(), ui->sSlider->value(), ui->vSlider->value(), 0xFF);
    setColour(col);
}

void ColourDialog::setRGB()
{
    if (signalsBlocked())
        return;
    QColor col(ui->rSlider->value(), ui->gSlider->value(), ui->bSlider->value(), 0xFF);
    if (col.saturation() == 0)
        col = QColor::fromHsv(ui->hSlider->value(), 0, col.value());
    setColour(col);
}

void ColourDialog::setColour(QColor col)
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
    m_colour.r = col.red();
    m_colour.g = col.green();
    m_colour.b = col.blue();

    // emit selectedColourModified(col);
}

ColourDialog::~ColourDialog() { delete ui; }

PaletteColour ColourDialog::getColour(PaletteColour color, bool *ok, QWidget *parent)
{
    PaletteColour clr = PaletteColour(color);
    ColourDialog c(clr, parent);
    *ok = (c.exec() == DialogCode::Accepted);
    return clr;
}

#include "moc_colourdialog.cpp"
