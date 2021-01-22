#ifndef COLOURDIALOG_H
#define COLOURDIALOG_H

namespace Ui
{
class ColourDialog;
}

class ColourDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ColourDialog(PaletteColour color, QWidget *parent = nullptr);
    ~ColourDialog();
    static PaletteColour getColour(PaletteColour colour, bool *ok, QWidget *parent = nullptr);

    inline PaletteColour colour() { return m_colour; }

private slots:
    void setColour(QColor col);
    void setHsv();
    void setRGB();
    void setAlpha();

private:
    Ui::ColourDialog *ui;
    PaletteColour m_colour;
};

#endif // COLOURDIALOG_H
