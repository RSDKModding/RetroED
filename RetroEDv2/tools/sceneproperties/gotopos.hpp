#pragma once

#include <QDialog>

namespace Ui
{
class GoToPos;
}

class GoToPos : public QDialog
{
    Q_OBJECT
public:
    explicit GoToPos(int scnMaxX = 0, int scnMaxY = 0, QString selLayer = "None", QWidget *parent = nullptr);
    ~GoToPos();

    void setupUI();
    int posX = 0;
    int posY = 0;

private:
    Ui::GoToPos *ui;
};
