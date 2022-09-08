#ifndef COLORDIALOG_H
#define COLORDIALOG_H

namespace Ui
{
class RSDKColorDialog;
}

class RSDKColorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RSDKColorDialog(PaletteColor color, QWidget *parent = nullptr);
    ~RSDKColorDialog();
    static PaletteColor getColor(PaletteColor color, bool *ok, QWidget *parent = nullptr);

    inline PaletteColor color() { return m_color; }

private slots:
    void setColor(QColor col);
    void setHsv();
    void setRGB();

private:
    Ui::RSDKColorDialog *ui;
    PaletteColor m_color;
};

#endif // COLORDIALOG_H
