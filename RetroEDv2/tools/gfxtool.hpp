#ifndef GFXTOOL_H
#define GFXTOOL_H

#include <QDialog>

namespace Ui
{
class GFXTool;
}

class GFXTool : public QDialog
{
    Q_OBJECT

public:
    explicit GFXTool(QWidget *parent = nullptr);
    ~GFXTool();

    QString gfxPath = "";
    QString imgPath = "";

    byte type = 0;

private:
    Ui::GFXTool *ui;
};

#endif // GFXTOOL_H
