#pragma once

#include <QDialog>

namespace Ui
{
class AnimSheetSelector;
}

class AnimSheetSelector : public QDialog
{
    Q_OBJECT

public:
    explicit AnimSheetSelector(QString sheetPath, QImage *sheet, bool toggle = false, QWidget *parent = nullptr);
    ~AnimSheetSelector();

    Rect<int> returnRect = Rect<int>(-1, -1, -1, -1);
    bool pivotToggle = false;

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

    QColor bgColor = QColor(0xFFFFFFFF);

    Ui::AnimSheetSelector *ui;
};


