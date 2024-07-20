#pragma once

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
    QColor grabScreenColor(const QPoint &p);

protected:
    bool event(QEvent *event);
    bool mouseMoveColorEvent(QMouseEvent *e);
    bool mouseReleaseColorEvent(QMouseEvent *e);
    bool keyPressColorEvent(QKeyEvent *e);

private slots:
    void setColor(QColor col);
    void setHsv();
    void setRGB();
    void pickScreenColor();

private:
    Ui::RSDKColorDialog *ui;
    PaletteColor m_color;
    PaletteColor PrevCol;
    bool colorPickMode = false;
};


