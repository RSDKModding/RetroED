#ifndef ANIMSHEETSELECTOR_H
#define ANIMSHEETSELECTOR_H

#include <QDialog>

namespace Ui
{
class AnimSheetSelector;
}

class AnimSheetSelector : public QDialog
{
    Q_OBJECT

public:
    explicit AnimSheetSelector(QString sheetPath, QImage *sheet, QWidget *parent = nullptr);
    ~AnimSheetSelector();

    Rect<int> returnRect = Rect<int>(-1, -1, -1, -1);

protected:
    void changeEvent(QEvent *e);

private:
    QString sheetPath = "";
    QImage *sheet;

    Vector2<int> mousePos;
    Vector2<float> reference;

    bool selecting = false;

    bool mouseDownL = false;
    bool mouseDownM = false;
    bool mouseDownR = false;

    Ui::AnimSheetSelector *ui;
};

#endif // ANIMSHEETSELECTOR_H
