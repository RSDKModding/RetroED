#ifndef COLORDIALOG_H
#define COLORDIALOG_H

namespace Ui
{
class ColorDialog;
}

class ColorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ColorDialog(PaletteColor color, QWidget *parent = nullptr);
    ~ColorDialog();
    static PaletteColor getColor(PaletteColor color, bool *ok, QWidget *parent = nullptr);

    inline PaletteColor color() { return m_color; }

private slots:
    void setColor(QColor col);
    void setHsv();
    void setRGB();

private:
    Ui::ColorDialog *ui;
    PaletteColor m_color;
};

#endif // COLORDIALOG_H
